#include "fsm.h"

#define RED   0
#define AMBER 1
#define GREEN 2

enum MODE { MODE_1 , MODE_2 , MODE_3 , MODE_4 };
enum MODE Mode = MODE_1;

static uint8_t counter_mode2, counter_mode3, counter_mode4;
static uint8_t buttonflag0, buttonflag1, buttonflag2;

int init = 0;
int index_led7 = 0;

// biến để lưu lại giá trị thực tế
static uint8_t time_red = 5;
static uint8_t time_amber = 2;
static uint8_t time_green = 3;

// mau luu trc do
static int8_t prev_saved = -1; // -1 = none, 0=RED,1=AMBER,2=GREEN

// dieu chinh mau con lai
static void reconcile_on_pair(int saved_color){
    if (prev_saved == -1){
        prev_saved = saved_color;
        return;
    }
    if (prev_saved == saved_color){
        prev_saved = saved_color;
        return;
    }

    switch(prev_saved){
        case RED:
            switch(saved_color){
                case AMBER:
                    if (time_red > time_amber) time_green = time_red - time_amber;
                    else time_green = 1;
                    break;

                case GREEN:
                    if (time_red > time_green) time_amber = time_red - time_green;
                    else time_amber = 1;
                    break;
            }
            break;

        case AMBER:
            switch(saved_color){
                case RED:
                    if (time_red > time_amber) time_green = time_red - time_amber;
                    else time_green = 1;
                    break;
                case GREEN:
                    time_red = time_amber + time_green;
                    break;
            }
            break;
        case GREEN:
            switch(saved_color){
                case RED:
                    if (time_red > time_green) time_amber = time_red - time_green;
                    else time_amber = 1;
                    break;
                case AMBER:
                    time_red = time_amber + time_green;
                    break;
            }
            break;
    }

    LED_TRAFFIC_SET_RED(time_red);
    LED_TRAFFIC_SET_AMBER(time_amber);
    LED_TRAFFIC_SET_GREEN(time_green);
    LED_TRAFFIC_APPLY_TIMES();
    LED_TRAFFIC_RESET_COUNTER();


    prev_saved = -1;
}

void adjust_time_auto(){
    if(time_red != time_amber + time_green){
        time_red = time_amber + time_green;
    }
}

void fsm_traffic_run(void){
    if(init == 0){
        LED_TRAFFIC_LOAD_BUFFER();
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
    case MODE_1: // chạy bình thường
        if (timer0_flag) {
            timer0_flag = 0;
            LED_TRAFFIC_RUN();
            setTimer(1000);
        }

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
    case MODE_2: // chỉnh RED
        update_all_clock_buffer(counter_mode2, 2);
        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_RED1_GPIO_Port, LED_RED1_Pin);
            HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin);
            setBlinkLedTimer(250);
        }

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

        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode2++;
            if(counter_mode2 > 99) counter_mode2 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 save RED
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            time_red = counter_mode2;
            LED_TRAFFIC_SET_RED(time_red);
            reconcile_on_pair(RED);
        } else if(!is_button_pressed(2)) buttonflag2 = 0;

        if(led_7_flag){
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    // ==========================================================
    case MODE_3: // chỉnh AMBER
        update_all_clock_buffer(counter_mode3, 3);

        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin);
            HAL_GPIO_TogglePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin);
            setBlinkLedTimer(250);
        }

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

        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode3++;
            if(counter_mode3 > 99) counter_mode3 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 save AMBER
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            time_amber = counter_mode3;
            LED_TRAFFIC_SET_AMBER(time_amber);
            reconcile_on_pair(AMBER);
        } else if(!is_button_pressed(2)) buttonflag2 = 0;

        if(led_7_flag){
            led_7_flag = 0;
            update_LED7_driver(index_led7);
            index_led7 = (index_led7 + 1) % 4;
            setLED7Timer(250);
        }
        break;

    // ==========================================================
    case MODE_4: // chỉnh GREEN
        update_all_clock_buffer(counter_mode4, 4);

        if(blink_flag){
            blink_flag = 0;
            HAL_GPIO_TogglePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin);
            HAL_GPIO_TogglePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin);
            setBlinkLedTimer(250);
        }

        // BTN0:-> MODE_1
        if(is_button_pressed(0) && buttonflag0 == 0){
            buttonflag0 = 1;

            adjust_time_auto();

            // ap dung tim moi lan chay mode 1 tiep theo
            LED_TRAFFIC_SET_RED(time_red);
            LED_TRAFFIC_SET_AMBER(time_amber);
            LED_TRAFFIC_SET_GREEN(time_green);
            LED_TRAFFIC_APPLY_TIMES();
            LED_TRAFFIC_RESET_COUNTER();

            // clear prev_saved
            prev_saved = -1;

            LED_TRAFFIC_INIT();
            LED_TRAFFIC_RUN();
            setTimer(1000);
            Mode = MODE_1;
            index_led7 = 0;
        } else if(!is_button_pressed(0)) buttonflag0 = 0;

        if(is_button_pressed(1) && buttonflag1 == 0){
            buttonflag1 = 1;
            counter_mode4++;
            if(counter_mode4 > 99) counter_mode4 = 1;
        } else if(!is_button_pressed(1)) buttonflag1 = 0;

        // BTN2 save GREEN
        if(is_button_pressed(2) && buttonflag2 == 0){
            buttonflag2 = 1;
            time_green = counter_mode4;
            LED_TRAFFIC_SET_GREEN(time_green);
            reconcile_on_pair(GREEN);
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
