/*
 * traffic_led.c
 *
 * Rewritten using switch-case for clarity.
 */

#include "main.h"
#include "traffic_led.h"
#include "led_display.h"

#define N0_OF_LED 3

#define RED 0
#define AMBER 1
#define GREEN 2

// Timer buffer for each LED color
static uint8_t bufferTimerForLED[N0_OF_LED];

// Counters for each LED group
static uint8_t counterRED1, counterAMBER1, counterGREEN1;
static uint8_t counterRED2, counterAMBER2, counterGREEN2;

// Duration for each light
static uint8_t timeRED, timeAMBER, timeGREEN;

// State variables (for switch-case)
static uint8_t state_vertical = GREEN;
static uint8_t state_horizontal = RED;

void LED_TRAFFIC_INIT(void) {
	HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
}

void LED_TRAFFIC_LOAD_BUFFER(void) {
	counterRED1 = bufferTimerForLED[RED];
	counterAMBER1 = bufferTimerForLED[AMBER];
	counterGREEN1 = bufferTimerForLED[GREEN];
	counterRED2 = bufferTimerForLED[RED];
	counterAMBER2 = bufferTimerForLED[AMBER];
	counterGREEN2 = bufferTimerForLED[GREEN];
	timeRED = bufferTimerForLED[RED];
	timeAMBER = bufferTimerForLED[AMBER];
	timeGREEN = bufferTimerForLED[GREEN];
}
uint8_t LED_TRAFFIC_GET_RED(void) {
    return bufferTimerForLED[RED];
}

uint8_t LED_TRAFFIC_GET_AMBER(void) {
    return bufferTimerForLED[AMBER];
}

uint8_t LED_TRAFFIC_GET_GREEN(void) {
    return bufferTimerForLED[GREEN];
}

void LED_TRAFFIC_STORE_BUFFER(uint8_t time, uint8_t index) {
	bufferTimerForLED[index] = time;
}

/* ===================== VERTICAL TRAFFIC ===================== */
void LED_VERTICAL_RUN(void) {
	switch (state_vertical) {
	case GREEN:
		update_vertical_clock_buffer(counterGREEN2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, RESET);

		if (--counterGREEN2 == 0) {
			counterAMBER2 = timeAMBER;
			state_vertical = AMBER;
		}
		break;

	case AMBER:
		update_vertical_clock_buffer(counterAMBER2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, RESET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

		if (--counterAMBER2 == 0) {
			counterRED2 = timeRED;
			state_vertical = RED;
		}
		break;

	case RED:
		update_vertical_clock_buffer(counterRED2);
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, RESET);
		HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

		if (--counterRED2 == 0) {
			counterGREEN2 = timeGREEN;
			state_vertical = GREEN;
		}
		break;
	}
}

/* ===================== HORIZONTAL TRAFFIC ===================== */
void LED_HORIZONTAL_RUN(void) {
	switch (state_horizontal) {
	case RED:
		update_horizontal_clock_buffer(counterRED1);
		HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, RESET);
		HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

		if (--counterRED1 == 0) {
			counterAMBER1 = timeAMBER;
			state_horizontal = AMBER;
		}
		break;

	case AMBER:
		update_horizontal_clock_buffer(counterAMBER1);
		HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, RESET);
		HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

		if (--counterAMBER1 == 0) {
			counterGREEN1 = timeGREEN;
			state_horizontal = GREEN;
		}
		break;

	case GREEN:
		update_horizontal_clock_buffer(counterGREEN1);
		HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
		HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
		HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, RESET);

		if (--counterGREEN1 == 0) {
			counterRED1 = timeRED;
			state_horizontal = RED;
		}
		break;
	}
}

/* ===================== MAIN RUN ===================== */
void LED_TRAFFIC_RUN(void) {
	LED_HORIZONTAL_RUN();
	LED_VERTICAL_RUN();
}
