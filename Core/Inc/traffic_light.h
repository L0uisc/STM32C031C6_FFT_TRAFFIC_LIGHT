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

#define INIT_CYCLE_TIME_MS (2*1000)
#define RED_1_CYCLE_TIME_MS (1*1000)
#define YELLOW_1_CYCLE_TIME_MS (2*1000)
#define GREEN_1_CYCLE_TIME_MS (7*1000)
#define RED_2_CYCLE_TIME_MS (1*1000)
#define YELLOW_2_CYCLE_TIME_MS (2*1000)
#define GREEN_2_CYCLE_TIME_MS (5*1000)
#define ERROR_FLASH_TOGGLE_TIME_MS (1*1000)

#endif /* INC_TRAFFIC_LIGHT_H_ */
