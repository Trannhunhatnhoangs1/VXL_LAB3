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

//we define the buffer to store time_of_led
static uint8_t bufferTimerForLED[N0_OF_LED];

//we define some sub-variable counter for each led
static uint8_t counterRED1, counterAMBER1, counterGREEN1,  counterRED2, counterAMBER2, counterGREEN2;

//we define variable timer to save time each led
static uint8_t timeRED, timeAMBER, timeGREEN;

// Function to turn off all led
void LED_TRAFFIC_INIT(void){
	HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

}

// Function to load value are saved in buffer to counter and time of each led

void LED_TRAFFIC_LOAD_BUFFER(void){
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

// Function to store value in buffer
void LED_TRAFFIC_STORE_BUFFER(uint8_t time, uint8_t index){
	bufferTimerForLED[index] = time;
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

// Function to setting led in vertical
void LED_VERTICAL_RUN(void) {
    // --- GREEN ---
    if (counterGREEN2 > 0) {
        update_vertical_clock_buffer(counterGREEN2);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   SET);
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, RESET);

        counterGREEN2--;
        if (counterGREEN2 == 0) {
            counterAMBER2 = timeAMBER;
        }
        return;
    }

    // --- AMBER ---
    if (counterAMBER2 > 0) {
        update_vertical_clock_buffer(counterAMBER2);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   SET);
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, RESET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

        counterAMBER2--;
        if (counterAMBER2 == 0) {
            counterRED2 = timeRED;
        }
        return;
    }

    // --- RED ---
    if (counterRED2 > 0) {
        update_vertical_clock_buffer(counterRED2);
        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   RESET);
        HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

        counterRED2--;
        if (counterRED2 == 0) {
            counterGREEN2 = timeGREEN;
        }
        return;
    }
}


void LED_HORIZONTAL_RUN(void) {
    // --- RED ---
    if (counterRED1 > 0) {
        update_horizontal_clock_buffer(counterRED1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   RESET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

        counterRED1--;
        if (counterRED1 == 0) {
            counterGREEN1 = timeGREEN;
        }
        return;
    }

    // --- GREEN ---
    if (counterGREEN1 > 0) {
        update_horizontal_clock_buffer(counterGREEN1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, RESET);

        counterGREEN1--;
        if (counterGREEN1 == 0) {
            counterAMBER1 = timeAMBER;
        }
        return;
    }

    // --- AMBER ---
    if (counterAMBER1 > 0) {
        update_horizontal_clock_buffer(counterAMBER1);
        HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   SET);
        HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, RESET);
        HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

        counterAMBER1--;
        if (counterAMBER1 == 0) {
            counterRED1 = timeRED;
        }
        return;
    }
}
// Funtion is call when we want to display led in each mode
void LED_TRAFFIC_RUN(void) {
	LED_HORIZONTAL_RUN();
	LED_VERTICAL_RUN();
}
