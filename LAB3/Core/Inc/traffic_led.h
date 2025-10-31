/*
 * traffic_led.h
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */

#ifndef INC_TRAFFIC_LED_H_
#define INC_TRAFFIC_LED_H_


#include "main.h"
#include "stm32f1xx_hal.h"
#include"software_timer.h"
void LED_TRAFFIC_STORE_BUFFER(uint8_t time, uint8_t index);
void LED_TRAFFIC_LOAD_BUFFER(void);
void LED_TRAFFIC_RUN(void);
void LED_TRAFFIC_INIT(void);
uint8_t LED_TRAFFIC_GET_RED(void);
uint8_t LED_TRAFFIC_GET_AMBER(void);
uint8_t LED_TRAFFIC_GET_GREEN(void);
uint8_t ERROR_HANDLING(uint8_t time);
void LED_TRAFFIC_SET_RED(uint8_t val);
void LED_TRAFFIC_SET_AMBER(uint8_t val);
void LED_TRAFFIC_SET_GREEN(uint8_t val);
void LED_TRAFFIC_APPLY_TIMES(void);

void LED_TRAFFIC_RESET_COUNTER(void);
#endif /* INC_TRAFFIC_LED_H_ */
