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

void LED_TRAFFIC_INIT(void);
void LED_TRAFFIC_RUN(void);
void LED_TRAFFIC_STORE_BUFFER(uint8_t val, uint8_t colorType);
void LED_TRAFFIC_LOAD_BUFFER(void);
uint8_t LED_TRAFFIC_GET_RED(void);
uint8_t LED_TRAFFIC_GET_AMBER(void);
uint8_t LED_TRAFFIC_GET_GREEN(void);

#endif /* INC_TRAFFIC_LED_H_ */
