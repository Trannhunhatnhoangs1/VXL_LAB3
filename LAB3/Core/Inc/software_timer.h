/*
 * software_timer.h
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE 10

extern int timer0_flag;
extern int led_7_flag ;
extern int blink_flag;
extern int timer1_flag;
void setTimer(int duration);
void setLED7Timer(int duration);
void setBlinkLedTimer(int duration);
void setTimer1(int duration);
void timer_run();
#endif /* INC_SOFTWARE_TIMER_H_ */
