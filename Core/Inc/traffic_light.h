/*
 * traffic_light.h
 *
 *  Created on: Dec 8, 2023
 *      Author: Louis
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

/**
 * @brief	initializes traffic light to red in both directions.
 * @params	none
 * @retval  void
 */
void traffic_light_init(void);

/**
 * @brief	handler function for traffic light state machine.
 * 			to be called every 1ms tick.
 * @params	none
 * @retval  void
 */
void traffic_light_handler(void);

/**
 * @brief	puts the traffic light in error state
 * @params  none
 * @retval  void
 */
void goto_error_state(void);

#define INIT_CYCLE_TIME_S 2
#define RED_1_CYCLE_TIME_S 9
#define YELLOW_1_CYCLE_TIME_S 2
#define GREEN_1_CYCLE_TIME_S 7
#define RED_2_CYCLE_TIME_S 11
#define YELLOW_2_CYCLE_TIME_S 2
#define GREEN_2_CYCLE_TIME_S 5
#define RED_CYCLE_OVERLAP_S 1
#define ERROR_FLASH_TOGGLE_TIME_S 1

#endif /* INC_TRAFFIC_LIGHT_H_ */
