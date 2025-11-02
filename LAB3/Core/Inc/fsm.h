/*
 * fsm.h
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */
#include <button.h>
#include "main.h"
#include "software_timer.h"
#include "traffic_led.h"
#include "led_display.h"


#ifndef INC_FSM_H_
#define INC_FSM_H_

typedef enum {
    MODE_1,
    MODE_2,
    MODE_3,
    MODE_4,
	MODE_MANUAL,
} MODE;
extern MODE Mode;
extern int init;

void reset_to_default_mode1(void);
void fsm_traffic_run( void );
#endif /* INC_FSM_H_ */
