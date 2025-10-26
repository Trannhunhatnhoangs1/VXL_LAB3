#include "fsm.h"

#define RED   0
#define AMBER 1
#define GREEN 2

enum MODE { MODE_1 , MODE_2 , MODE_3 , MODE_4 };
enum MODE Mode = MODE_1;

// lưu thời gian từng đèn
static uint8_t counter_mode2, counter_mode3, counter_mode4;

// tránh nhảy mode liên tục
static uint8_t buttonflag0, buttonflag1, buttonflag2;

int init = 0;
int index_led7 = 0;

void fsm_for_output_processing(void){
    if(init == 0){
        LED_TRAFFIC_INIT();
        LED_TRAFFIC_RUN();
        init = 1;
        setTimer(1000);
        setBlinkLedTimer(0);
        setLED7Timer(TIMER_CYCLE);
        return;
    }

    switch(Mode){
    // ==========================================================
    case MODE_1: // chế độ chạy bình thường
        if (timer0_flag) {
            timer0_flag = 0;
            LED_TRAFFIC_RUN();
            setTimer(1000);
        }

        // Nhấn BTN0 để sang MODE_2
        if (is_button_pressed(0) && buttonflag0 == 0) {
            buttonflag0 = 1;
            Mode = MODE_2;
            counter_mode2 = LED_TRAFFIC_GET_RED();
            LED_TRAFFIC_INIT();
            HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET);
            setBlinkLedTimer(250);
            index_led7 = 0;
        } else if (!is_button_pressed(0)) buttonflag0 = 0;

        if (led_7_flag) {
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    // ==========================================================
    case MODE_2: // Modify time for RED
        update_all_clock_buffer(counter_mode2, 2);
        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_RED1_GPIO_Port, LED_RED1_Pin);
            HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin);
            setBlinkLedTimer(250); // 2Hz
        }

        // BTN0 → sang MODE_3
        if(is_button_pressed(0) && buttonflag0 == 0){
            buttonflag0 = 1;
            Mode = MODE_3;
            counter_mode3 = LED_TRAFFIC_GET_AMBER();
            LED_TRAFFIC_INIT();
            HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, GPIO_PIN_RESET);
            setBlinkLedTimer(250);
            index_led7 = 0;
        } else if(!is_button_pressed(0)) buttonflag0 = 0;

        // BTN1 → tăng giá trị
        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode2++;
            if(counter_mode2 > 99) counter_mode2 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 → lưu lại
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            LED_TRAFFIC_STORE_BUFFER(counter_mode2, RED);
        } else if(!is_button_pressed(2)) buttonflag2 = 0;

        if(led_7_flag){
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    // ==========================================================
    case MODE_3: // Modify time for AMBER
        update_all_clock_buffer(counter_mode3, 3);

        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin);
            HAL_GPIO_TogglePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin);
            setBlinkLedTimer(250);
        }

        // BTN0 → sang MODE_4
        if(is_button_pressed(0) && buttonflag0 == 0){
            buttonflag0 = 1;
            Mode = MODE_4;
            counter_mode4 = LED_TRAFFIC_GET_GREEN();
            LED_TRAFFIC_INIT();
            HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_RESET);
            setBlinkLedTimer(250);
            index_led7 = 0;
        } else if(!is_button_pressed(0)) buttonflag0 = 0;

        // BTN1 → tăng
        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode3++;
            if(counter_mode3 > 99) counter_mode3 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 → lưu
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            LED_TRAFFIC_STORE_BUFFER(counter_mode3, AMBER);
        } else if(!is_button_pressed(2)) buttonflag2 = 0;

        if(led_7_flag){
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    // ==========================================================
    case MODE_4: // Modify time for GREEN
        update_all_clock_buffer(counter_mode4, 4);

        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin);
            HAL_GPIO_TogglePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin);
            setBlinkLedTimer(250);
        }

        // BTN0 → quay lại MODE_1
        if(is_button_pressed(0) && buttonflag0 == 0){
            buttonflag0 = 1;
            Mode = MODE_1;
            LED_TRAFFIC_LOAD_BUFFER();
            LED_TRAFFIC_INIT();
            LED_TRAFFIC_RUN();
            setTimer(1000);
            index_led7 = 0;
        } else if(!is_button_pressed(0)) buttonflag0 = 0;

        // BTN1 → tăng
        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode4++;
            if(counter_mode4 > 99) counter_mode4 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 → lưu
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            LED_TRAFFIC_STORE_BUFFER(counter_mode4, GREEN);
        } else if(!is_button_pressed(2)) buttonflag2 = 0;

        if(led_7_flag){
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    default:
        Mode = MODE_1;
        break;
    }
}
