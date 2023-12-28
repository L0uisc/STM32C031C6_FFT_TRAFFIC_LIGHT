/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "traffic_light.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static TX_THREAD TrafficLight_Phase1;
static TX_THREAD TrafficLight_Phase2;
static TX_THREAD ButtonHandler;
static TX_SEMAPHORE Start_Sync;
static TX_SEMAPHORE Phase1_Red_Entered;
static TX_SEMAPHORE Phase2_Red_Entered;
TX_EVENT_FLAGS_GROUP flags;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void App_TrafficLight_Phase1_Entry(ULONG thread_input);
void App_TrafficLight_Phase2_Entry(ULONG thread_input);
void App_ButtonHandler_Entry(ULONG thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

   /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;

  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
		  	  	  	   TX_MINIMUM_STACK, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
	goto error;
  }

  if (tx_thread_create(&TrafficLight_Phase1, "Phase1 Thread", App_TrafficLight_Phase1_Entry, 0,
		  	  	  	   pointer, TX_MINIMUM_STACK,
					   10, 10, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
	ret = TX_THREAD_ERROR;
    goto error;
  }

  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
  		  	  	  	   TX_MINIMUM_STACK, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
    goto error;
  }

  if (tx_thread_create(&TrafficLight_Phase2, "Phase2 Thread", App_TrafficLight_Phase2_Entry, 0,
		  	  	  	   pointer, TX_MINIMUM_STACK,
					   10, 10, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
    goto error;
  }

  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
		   	   	       TX_MINIMUM_STACK, TX_NO_WAIT) != TX_SUCCESS)
  {
	ret = TX_POOL_ERROR;
	goto error;
  }

  if (tx_thread_create(&ButtonHandler, "Button Thread", App_ButtonHandler_Entry, 0,
		  	  	  	   pointer, TX_MINIMUM_STACK,
					   5, 5, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
	ret = TX_THREAD_ERROR;
	goto error;
  }

  if (tx_semaphore_create(&Start_Sync, "Start Sync Semaphore", 2) != TX_SUCCESS)
  {
    ret = TX_SEMAPHORE_ERROR;
    goto error;
  }

  if (tx_semaphore_create(&Phase1_Red_Entered, "Phase 1 Red Entry Semaphore", 0) != TX_SUCCESS)
  {
	ret = TX_SEMAPHORE_ERROR;
	goto error;
  }

  if (tx_semaphore_create(&Phase2_Red_Entered, "Phase 2 Red Entry Semaphore", 1) != TX_SUCCESS)
  {
	ret = TX_SEMAPHORE_ERROR;
	goto error;
  }

  if (tx_event_flags_create(&flags, "Event Flag Group") != TX_SUCCESS)
  {
	ret = TX_GROUP_ERROR;
	goto error;
  }
error:
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/**
  * @brief  App_ThreadX_LowPower_Timer_Setup
  * @param  count : TX timer count
  * @retval None
  */
void App_ThreadX_LowPower_Timer_Setup(ULONG count)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Timer_Setup */

  /* USER CODE END  App_ThreadX_LowPower_Timer_Setup */
}

/**
  * @brief  App_ThreadX_LowPower_Enter
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Enter(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Enter */

  /* USER CODE END  App_ThreadX_LowPower_Enter */
}

/**
  * @brief  App_ThreadX_LowPower_Exit
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Exit(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */

  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/**
  * @brief  App_ThreadX_LowPower_Timer_Adjust
  * @param  None
  * @retval Amount of time (in ticks)
  */
ULONG App_ThreadX_LowPower_Timer_Adjust(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Timer_Adjust */
  return 0;
  /* USER CODE END  App_ThreadX_LowPower_Timer_Adjust */
}

/* USER CODE BEGIN 1 */
void App_TrafficLight_Phase1_Entry(ULONG thread_input)
{
  (void)thread_input;

  tx_semaphore_get(&Start_Sync, TX_WAIT_FOREVER);

  tx_thread_sleep(INIT_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

  while (1)
  {
	tx_semaphore_get(&Phase2_Red_Entered, TX_WAIT_FOREVER);
	tx_thread_sleep(RED_CYCLE_OVERLAP_S * TX_TIMER_TICKS_PER_SECOND);

    HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_SET);
    tx_thread_sleep(GREEN_1_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

    HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_SET);
    tx_thread_sleep(YELLOW_1_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

    HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_SET);
    tx_semaphore_ceiling_put(&Phase1_Red_Entered, 1);
  }
}

void App_TrafficLight_Phase2_Entry(ULONG thread_input)
{
  (void)thread_input;

  tx_semaphore_get(&Start_Sync, TX_WAIT_FOREVER);

  tx_thread_sleep(INIT_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

  while (1)
  {
	tx_semaphore_get(&Phase1_Red_Entered, TX_WAIT_FOREVER);
	tx_thread_sleep(RED_CYCLE_OVERLAP_S * TX_TIMER_TICKS_PER_SECOND);

    HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_SET);
    tx_thread_sleep(GREEN_2_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

    HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_SET);
    tx_thread_sleep(YELLOW_2_CYCLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);

	HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_SET);
    tx_semaphore_ceiling_put(&Phase2_Red_Entered, 1);
  }
}

void App_ButtonHandler_Entry(ULONG thread_input)
{
  (void)thread_input;

  ULONG flags_buffer;
  tx_event_flags_get(&flags, BUTTON_PUSHED_FLAG, TX_AND_CLEAR, &flags_buffer, TX_WAIT_FOREVER);

  tx_thread_terminate(&TrafficLight_Phase1);
  tx_thread_terminate(&TrafficLight_Phase2);

  HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_SET);

  while (1)
  {
	tx_thread_sleep(ERROR_FLASH_TOGGLE_TIME_S * TX_TIMER_TICKS_PER_SECOND);
	HAL_GPIO_TogglePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin);
	HAL_GPIO_TogglePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin);
  }
}
/* USER CODE END 1 */
