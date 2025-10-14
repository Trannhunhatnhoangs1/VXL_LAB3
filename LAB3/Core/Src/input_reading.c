/*
 * input_reading.c
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */

#include "main.h"
#include "input_reading.h"
// we aim to work with more than one buttons
#define N0_OF_BUTTONS 					3
#define BUTTON_IS_PRESSED 				GPIO_PIN_RESET
#define BUTTON_IS_RELEASED 				GPIO_PIN_SET

// the buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];

// we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

static uint16_t buttonPin[N0_OF_BUTTONS] = {BUTTON0_Pin, BUTTON1_Pin, BUTTON2_Pin};

//we define function to SET all buffers to RELEASED
void button_reading_Init(void){
	for (int i = 0; i < N0_OF_BUTTONS; i++){
		debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
		buttonBuffer[i] = BUTTON_IS_RELEASED;
	}
}

void button_reading(void){
	for(int i = 0; i < N0_OF_BUTTONS; i++){
		debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
		debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOB, buttonPin[i]);
		if(debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
			buttonBuffer[i] = debounceButtonBuffer1[i];
		}
	}
}

unsigned char is_button_pressed(uint8_t index){
	if(index >= N0_OF_BUTTONS) return 0;
	if (buttonBuffer[index] == BUTTON_IS_PRESSED){
		return 1;
	}
	else return 0;
}
