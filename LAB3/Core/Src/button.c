/*
 * button.c
 *
 *  Created on: Aug 1, 2025
 *      Author: dell pro
 */

#include <button.h>
#include "main.h"
#define N0_OF_BUTTONS 					4
#define BUTTON_IS_PRESSED 				GPIO_PIN_RESET
#define BUTTON_IS_RELEASED 				GPIO_PIN_SET

static GPIO_PinState buttonBuffer[N0_OF_BUTTONS];

// 2 trang thai nut nhan lien tiep
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS];

static uint16_t buttonPin[N0_OF_BUTTONS] = {BUTTON0_Pin, BUTTON1_Pin, BUTTON2_Pin, BUTTON3_Pin};

//xet dang o trang thai ko nhan
void button_Init(void){
	for (int i = 0; i < N0_OF_BUTTONS; i++){
		debounceButtonBuffer1[i] = BUTTON_IS_RELEASED;
		debounceButtonBuffer2[i] = BUTTON_IS_RELEASED;
		buttonBuffer[i] = BUTTON_IS_RELEASED;
	}
}
//chong doi, xet button ở 2 lan lien tiep
void getKeyInput(void){
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
