#include "fsm_manual.h"
#include "fsm.h"
#include "button.h"
#include "main.h"
#include "led_display.h"

#define STATE_DO_XANH   0
#define STATE_XANH_DO   1

extern MODE Mode;
extern int init;

static uint8_t manual_state = STATE_DO_XANH;
static uint8_t btn0_flag = 0;
static uint8_t btn1_flag = 0;
static uint8_t btn3_flag = 0;

static void turn_off_all_leds(void) {
    HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
    HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
    HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
    HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
    HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
    HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
    HAL_GPIO_WritePin(LED_MANUAL_GPIO_Port, LED_MANUAL_Pin,SET);
}

static void led_manual(uint8_t on) {
    HAL_GPIO_WritePin(LED_MANUAL_GPIO_Port, LED_MANUAL_Pin, on ? GPIO_PIN_RESET : SET);
}
static void set_led_state(uint8_t state) {
    turn_off_all_leds();

    switch (state) {
        case STATE_DO_XANH:
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_RESET);
            break;

        case STATE_XANH_DO:
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET);
            break;
    }
}


void fsm_manual_init(void) {
    turn_off_all_leds();
    clearAllLED7();
    manual_state = STATE_DO_XANH;
    set_led_state(manual_state);
    led_manual(0);
}

void fsm_manual_run(void) {

    if (Mode != MODE_MANUAL && !is_button_pressed(3)){
   	    led_manual(0);
    	return;
    }
    if (is_button_pressed(3)) {
        if (btn3_flag == 0) {
            btn3_flag = 1;
            if (Mode == MODE_1) {
                Mode = MODE_MANUAL;
                fsm_manual_init();
            }
        }
    } else btn3_flag = 0;

    if (Mode != MODE_MANUAL) return;

    if (is_button_pressed(1)) {
        if (btn1_flag == 0) {
            btn1_flag = 1;
            led_manual(1);
            HAL_Delay(2000);
            if (manual_state == STATE_DO_XANH)
                manual_state = STATE_XANH_DO;
            else
                manual_state = STATE_DO_XANH;

            set_led_state(manual_state);
            led_manual(0);
        }
    } else btn1_flag = 0;

    if (is_button_pressed(0)) {
        if (btn0_flag == 0) {
            btn0_flag = 1;
            turn_off_all_leds();
            clearAllLED7();
        }
    } else {
        if (btn0_flag == 1 && Mode == MODE_MANUAL) {
        	 reset_to_default_mode1();
        	Mode = MODE_1;
            init = 0;

        }
        btn0_flag = 0;
    }
}
