/*
 * traffic_light.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Louis
 */
#include "main.h"
#include "traffic_light.h"

typedef enum {
  TRAFFIC_INIT,
  TRAFFIC_GREEN_1,
  TRAFFIC_YELLOW_1,
  TRAFFIC_RED_1,
  TRAFFIC_GREEN_2,
  TRAFFIC_YELLOW_2,
  TRAFFIC_RED_2,
  TRAFFIC_ERROR,
} Traffic_Light_State;

static Traffic_Light_State traffic_light_state = TRAFFIC_INIT;
static uint32_t tick_counter = 0;

static void switch_state(Traffic_Light_State new_state);

void traffic_light_init(void)
{
  HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_SET);

  switch_state(TRAFFIC_INIT);
}

void traffic_light_handler(void)
{
  tick_counter += 1;
  switch (traffic_light_state)
  {
  case TRAFFIC_INIT:
	if (tick_counter >= INIT_CYCLE_TIME_MS)
	{
	  HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_GREEN_1);
	}
    break;
  case TRAFFIC_GREEN_1:
	if (tick_counter >= GREEN_1_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_YELLOW_1);
	}
	break;
  case TRAFFIC_YELLOW_1:
	if (tick_counter >= YELLOW_1_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_RED_1);
	}
	break;
  case TRAFFIC_RED_1:
	if (tick_counter >= RED_1_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_GREEN_2);
	}
	break;
  case TRAFFIC_GREEN_2:
	if (tick_counter >= GREEN_2_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_YELLOW_2);
	}
	break;
  case TRAFFIC_YELLOW_2:
	if (tick_counter >= YELLOW_2_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_RED_2);
	}
	break;
  case TRAFFIC_RED_2:
	if (tick_counter >= RED_2_CYCLE_TIME_MS)
	{
      HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_SET);
      switch_state(TRAFFIC_GREEN_1);
	}
	break;
  case TRAFFIC_ERROR:
  default:
	if (tick_counter >= ERROR_FLASH_TOGGLE_TIME_MS)
	{
	  HAL_GPIO_TogglePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin);
	  HAL_GPIO_TogglePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin);
	  tick_counter = 0;
	}
	break;
  }
}

void goto_error_state(void)
{
  HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(LED_GREEN_2_GPIO_Port, LED_GREEN_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_YELLOW_2_GPIO_Port, LED_YELLOW_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_RED_2_GPIO_Port, LED_RED_2_Pin, GPIO_PIN_SET);

  switch_state(TRAFFIC_ERROR);
}

static void switch_state(Traffic_Light_State new_state)
{
  traffic_light_state = new_state;
  tick_counter = 0;
}
