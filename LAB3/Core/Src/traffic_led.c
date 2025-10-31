/*
 * traffic_led.c
 */

#include "main.h"
#include "traffic_led.h"
#include "led_display.h"

#define N0_OF_LED 3

#define RED   0
#define AMBER 1
#define GREEN 2

// buffer lưu thời gian từng đèn
static uint8_t bufferTimerForLED[N0_OF_LED];

// counter cho từng LED ở 2 hướng
static uint8_t counterRED1, counterAMBER1, counterGREEN1;
static uint8_t counterRED2, counterAMBER2, counterGREEN2;

// thời gian thực tế của từng đèn
static uint8_t timeRED = 5;
static uint8_t timeAMBER = 2;
static uint8_t timeGREEN = 3;

// trạng thái LED theo màu
static uint8_t verticalState = GREEN;
static uint8_t horizontalState = RED;

// ------------------------- INIT -------------------------
void LED_TRAFFIC_INIT(void){
    HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
    HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
    HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
    HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
    HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
    HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
}

// ------------------------- LOAD/SAVE BUFFER -------------------------
void LED_TRAFFIC_LOAD_BUFFER(void){
    counterRED1   = bufferTimerForLED[RED];
    counterAMBER1 = bufferTimerForLED[AMBER];
    counterGREEN1 = bufferTimerForLED[GREEN];
    counterRED2   = bufferTimerForLED[RED];
    counterAMBER2 = bufferTimerForLED[AMBER];
    counterGREEN2 = bufferTimerForLED[GREEN];
    timeRED       = bufferTimerForLED[RED];
    timeAMBER     = bufferTimerForLED[AMBER];
    timeGREEN     = bufferTimerForLED[GREEN];
}

void LED_TRAFFIC_STORE_BUFFER(uint8_t time, uint8_t index){
    bufferTimerForLED[index] = time;
}

uint8_t LED_TRAFFIC_GET_RED(void) { return bufferTimerForLED[RED]; }
uint8_t LED_TRAFFIC_GET_AMBER(void) { return bufferTimerForLED[AMBER]; }
uint8_t LED_TRAFFIC_GET_GREEN(void) { return bufferTimerForLED[GREEN]; }

// ------------------------- SET -------------------------
void LED_TRAFFIC_SET_RED(uint8_t val){
    if(val < 1) val = 1;
    timeRED = val;
    bufferTimerForLED[RED] = val;
}
void LED_TRAFFIC_SET_AMBER(uint8_t val){
    if(val < 1) val = 1;
    timeAMBER = val;
    bufferTimerForLED[AMBER] = val;
}
void LED_TRAFFIC_SET_GREEN(uint8_t val){
    if(val < 1) val = 1;
    timeGREEN = val;
    bufferTimerForLED[GREEN] = val;
}
void LED_TRAFFIC_APPLY_TIMES(void){
    bufferTimerForLED[RED]   = timeRED;
    bufferTimerForLED[AMBER] = timeAMBER;
    bufferTimerForLED[GREEN] = timeGREEN;

    // cập nhật counters mới ngay
    counterRED1   = timeRED;
    counterAMBER1 = timeAMBER;
    counterGREEN1 = timeGREEN;
    counterRED2   = timeRED;
    counterAMBER2 = timeAMBER;
    counterGREEN2 = timeGREEN;

    // reset trạng thái về mode ban đầu
    verticalState   = GREEN;
    horizontalState = RED;
}
// ------------------------- RESET COUNTER -------------------------
void LED_TRAFFIC_RESET_COUNTER(void){
    counterRED1   = bufferTimerForLED[RED];
    counterAMBER1 = bufferTimerForLED[AMBER];
    counterGREEN1 = bufferTimerForLED[GREEN];
    counterRED2   = bufferTimerForLED[RED];
    counterAMBER2 = bufferTimerForLED[AMBER];
    counterGREEN2 = bufferTimerForLED[GREEN];
}

// ------------------------- RUN LOGIC -------------------------
void LED_VERTICAL_RUN(void){
    switch(verticalState){
        case GREEN:
            update_vertical_clock_buffer(counterGREEN2);
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   SET);
            HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, RESET);

            if(counterGREEN2 > 0) counterGREEN2--;
            if(counterGREEN2 == 0){
                counterAMBER2 = timeAMBER;
                verticalState = AMBER;
            }
            break;

        case AMBER:
            update_vertical_clock_buffer(counterAMBER2);
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   SET);
            HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, RESET);
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

            if(counterAMBER2 > 0) counterAMBER2--;
            if(counterAMBER2 == 0){
                counterRED2 = timeRED;
                verticalState = RED;
            }
            break;

        case RED:
            update_vertical_clock_buffer(counterRED2);
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port,   LED_RED1_Pin,   RESET);
            HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);

            if(counterRED2 > 0) counterRED2--;
            if(counterRED2 == 0){
                counterGREEN2 = timeGREEN;
                verticalState = GREEN;
            }
            break;
    }
}

void LED_HORIZONTAL_RUN(void){
    switch(horizontalState){
        case GREEN:
            update_horizontal_clock_buffer(counterGREEN1);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   SET);
            HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, RESET);

            if(counterGREEN1 > 0) counterGREEN1--;
            if(counterGREEN1 == 0){
                counterAMBER1 = timeAMBER;
                horizontalState = AMBER;
            }
            break;

        case AMBER:
            update_horizontal_clock_buffer(counterAMBER1);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   SET);
            HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, RESET);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

            if(counterAMBER1 > 0) counterAMBER1--;
            if(counterAMBER1 == 0){
                counterRED1 = timeRED;
                horizontalState = RED;
            }
            break;

        case RED:
            update_horizontal_clock_buffer(counterRED1);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port,   LED_RED2_Pin,   RESET);
            HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);

            if(counterRED1 > 0) counterRED1--;
            if(counterRED1 == 0){
                counterGREEN1 = timeGREEN;
                horizontalState = GREEN;
            }
            break;
    }
}

// ------------------------- MAIN RUNNER -------------------------
void LED_TRAFFIC_RUN(void){
    LED_HORIZONTAL_RUN();
    LED_VERTICAL_RUN();
}
