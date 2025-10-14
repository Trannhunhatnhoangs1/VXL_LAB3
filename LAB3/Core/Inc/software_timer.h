/*
 * software_timer.h
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_CYCLE 10;
extern int timer0_flag;
extern int timer1_flag;
extern int timer2_flag;

extern int longpress_flag;

void setTimer0(int duration);
void setTimer1(int duration);
void setTimer2(int duration);
void longpressTimer(int duration);
void timer_run();

#endif /* INC_SOFTWARE_TIMER_H_ */
