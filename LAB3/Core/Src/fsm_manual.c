#include "fsm_manual.h"
#include "fsm.h"
#include "button.h"
#include "main.h"
#include "led_display.h"
#include "software_timer.h"

typedef enum {
STATE_DO_XANH,
STATE_XANH_DO,
STATE_EXIT
} ManualState;

static uint8_t manual_active = 0;   // 0 = tắt, 1 = bật
static ManualState manual_state = STATE_DO_XANH;

static uint8_t btn0_flag = 0;
static uint8_t btn1_flag = 0;
static uint8_t btn3_flag = 0;
static uint8_t waiting_timer = 0;

extern int timer1_flag;
extern int init;
extern MODE Mode;

// ====================== HÀM HỖ TRỢ ===============================
static void turn_off_all_leds(void)
{
	HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET);
	HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, SET);
	HAL_GPIO_WritePin(LED_MANUAL_GPIO_Port, LED_MANUAL_Pin, SET);
}

static void led_manual(uint8_t on)
{
	HAL_GPIO_WritePin(LED_MANUAL_GPIO_Port, LED_MANUAL_Pin,on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static void set_led_state(ManualState state)
{
	turn_off_all_leds();
	switch (state){
	case STATE_DO_XANH:
		HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin, GPIO_PIN_RESET);
		break;

	case STATE_XANH_DO:
		HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, GPIO_PIN_RESET);
		break;

		default:
		break;
		}
}

void fsm_manual_init(void)
{
	turn_off_all_leds();
	clearAllLED7();
	manual_state = STATE_DO_XANH;
	set_led_state(manual_state);
	led_manual(0);
	waiting_timer = 0;
}

void fsm_manual_run(void)
{
	switch (manual_active){
// ---------------- MANUAL CHƯA BẬT ----------------
	case 0:
		led_manual(0);
		if (is_button_pressed(3) && btn3_flag == 0 && Mode == MODE_1){
		btn3_flag = 1;
		manual_active = 1;
		fsm_manual_init();
		}
		if (!is_button_pressed(3)) btn3_flag = 0;
		break;

// ---------------- MANUAL ĐANG BẬT ----------------
case 1:
	switch (manual_state){
	case STATE_DO_XANH:
	case STATE_XANH_DO:
		// Nút 1 → đổi trạng thái đèn
	  if (is_button_pressed(1) && btn1_flag == 0 && waiting_timer == 0){
		btn1_flag = 1;
		led_manual(1);
		setTimer1(2000);
		waiting_timer = 1;
		}
	  if (!is_button_pressed(1)) btn1_flag = 0;
		// Khi hết timer, đổi đèn
	  if (waiting_timer && timer1_flag)
		{
			timer1_flag = 0;
			waiting_timer = 0;
			manual_state = (manual_state == STATE_DO_XANH)? STATE_XANH_DO: STATE_DO_XANH;
			set_led_state(manual_state);
			led_manual(0);
		}

		// Nút 0 → chuyển sang STATE_EXIT
	   if (is_button_pressed(0) && btn0_flag == 0)
		{
			btn0_flag = 1;
			manual_state = STATE_EXIT;
		}
	   if (!is_button_pressed(0)) btn0_flag = 0;
		break;

	 case STATE_EXIT:
			turn_off_all_leds();
			clearAllLED7();
			while (is_button_pressed(0));
			manual_active = 0;
			reset_to_default_mode1();
			Mode = MODE_1;
			init = 0;
			manual_state = STATE_DO_XANH;
			break;

		default:
			manual_state = STATE_DO_XANH;
			break;
		}
		break;
	}
}
uint8_t is_manual_active(void){
	return manual_active;
}
