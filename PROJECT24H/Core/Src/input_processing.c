#include "main.h"
#include "button.h"
#include "input_processing.h"
#include "SoftwareTimer.h"
#include "7Seg_Led.h"

#define BTN_RESET 0
#define BTN_INC 1
#define BTN_DEC 2

#define TRUE 1
#define FALSE 0

enum State{INIT,NORMAL, INCREASE, DECREASE, RES, INCREASE_3S, DECREASE_3S,AUTO_COUNT_DOWN};
enum State state=INIT;

int value;
int btn_increase_executed=0;
int btn_decrease_executed=0;
int LED_BLINKING_INTERVAL=1000;

/* BTN PRESSED 3S VAR AND FUNC START */
int counter3s_DEC=3;
int counter3s_INC=3;
int flag3s_DEC=0;
int flag3s_INC=0;
int temp_value;
void count_BTN_pressed3s(int BTN);
int BTN_INC_PRESSED_3s();
int BTN_DEC_PRESSED_3s();
void reset_var_counter_3s();
/* BTN PRESSED 3S VAR AND FUNC END */

/* BTN RELEASED 10S VAR AND FUNC START */
int count_released10s=10;
int flag10s_released=0;
void count_BTN_released10s();
void reset_var_released_10s();
int BTN_RELEASED_10s();
int BTN_EVENT=0;
int AUTO_COUNT_DOWN_INTERVAL=1000;
/* BTN RELEASED 10S VAR AND FUNC END */

/* 7SEG_LED GPIO PORT PIN START */
GPIO_TypeDef*SEG_LED1_PORT[]={a1_GPIO_Port,b1_GPIO_Port,c1_GPIO_Port,d1_GPIO_Port,e1_GPIO_Port,
							f1_GPIO_Port,g1_GPIO_Port};
uint16_t SEG_LED1_PIN[]={a1_Pin,b1_Pin,c1_Pin,d1_Pin,e1_Pin,f1_Pin,g1_Pin};
/* 7SEG_LED GPIO PORT PIN END */
void fsm_simple_buttons_run(){
	switch(state){
		case INIT:
			BTN_INIT(BTN_RESET_GPIO_Port, BTN_RESET_Pin, 0);
			BTN_INIT(BTN_INC_GPIO_Port, BTN_INC_Pin, 1);
			BTN_INIT(BTN_DEC_GPIO_Port,BTN_DEC_Pin,2);
			value=5;
			setTimer3(1000);
			setTimer4(LED_BLINKING_INTERVAL);
			state=NORMAL;
			break;
		case NORMAL:
			reset_var_counter_3s();
			/* NORMAL EXECUTION START */
			display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* NORMAL EXECUTION END */

			/* BTN RELEASED 10S EXECUTION START */
			if(timer3_flag==1){
				setTimer3(1000);
				count_BTN_released10s();
			}
			if(BTN_RELEASED_10s()){
				state=AUTO_COUNT_DOWN;
				timer3_flag=1;
				temp_value=value;
			}
			/* BTN RELEASED 10S EXECUTION END */

			/* INC AND DEC BTN CHANGE STATE START */
			if(button_pressed(BTN_INC)){  //INCREASE BTN PRESSED
				state=INCREASE;
				setTimer2(1000); //set timer for BTN INC time counter
				setTimer1(1000); //set timer for changing value at BTN INC DEC
			}else if(button_pressed(BTN_DEC)){  //DECREASE BTN PRESSED
				state=DECREASE;
				setTimer2(1000); //set timer for BTN INC time counter
				setTimer1(1000); //set timer for changing value at BTN INC DEC
			}
			/* INC BTN AND DEC CHANGE STATE END */

			/* RESET BTN CHANGE STATE START */
			if(button_pressed(BTN_RESET)){   //RESET BTN PRESSED
				state=RES;
			}
			/* RESET BTN CHANGE STATE END */
			break;
		case INCREASE:
			/* INCREASE EXECUTION START */
			if(btn_increase_executed==0){
				value++;
				if(value==10)
					value=0;
				display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
				btn_increase_executed=1;
			}
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* INCREASE EXECUTION END */


			/* INC BTN PRESSED 3S CHANGE STATE START */
				if(timer2_flag==1){
					setTimer2(1000);
					count_BTN_pressed3s(BTN_INC);
				}
				if(BTN_INC_PRESSED_3s()){
					state=INCREASE_3S;
				}
			/* INC BTN PRESSED 3S CHANGE STATE END */

			/* INC BTN CHANGE NORMAL STATE START */
			if(button_pressed(BTN_INC)==FALSE){  //INC BTN RELEASED
				state=NORMAL;
				btn_increase_executed=0;
				reset_var_released_10s();
				setTimer3(1000);
			}
			/* INC BTN CHANGE NORMAL STATE END */
			break;
		case DECREASE:
			/* DECREASE EXECUTION START */
			if(btn_decrease_executed==0){
				value--;
				if(value<0)
					value=9;
				display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
				btn_decrease_executed=1;
			}
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* DECREASE EXECUTION END */

			/* INC BTN PRESSED 3S CHANGE STATE START */
			if(timer2_flag==1){
				setTimer2(1000);
				count_BTN_pressed3s(BTN_DEC);
			}
			if(BTN_DEC_PRESSED_3s()){
				state=DECREASE_3S;
			}
			/* INC BTN PRESSED 3S CHANGE STATE END */

			/* DEC BTN CHANGE NORMAL STATE START */
			if(button_pressed(BTN_DEC)==FALSE){  //INC BTN RELEASED
				state=NORMAL;
				btn_decrease_executed=0;
				reset_var_released_10s();
				setTimer3(1000);
			}
			/* DEC BTN CHANGE NORMAL STATE END */
			break;
		case RES:
			/* RESET EXECUTION START */
			value=0;
			display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* RESET EXECUTION END */

			/* RESET BTN CHANGE START START */
			if(button_pressed(BTN_RESET)==FALSE){ //RES BTN RELEASED
				state=NORMAL;
				setTimer3(1000);
				reset_var_released_10s();
			}
			/* RESET BTN CHANGE START END */
			break;

		case INCREASE_3S:
			/* INCREASE EVERY 1S EXECUTION START */
			if(timer1_flag==1){
				setTimer1(1000);
				value++;
				if(value==10)
					value=0;
				display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
			}
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* INCREASE EVERY 1S EXECUTION END */

			/* INC BTN CHANGE NORMAL STATE START */
			if(button_pressed(BTN_INC)==FALSE){  //INC BTN RELEASED
				state=NORMAL;
				btn_increase_executed=0;
				reset_var_released_10s();
				setTimer3(1000);
			}
			/* INC BTN CHANGE NORMAL STATE END */
			break;
		case DECREASE_3S:
			/* DECREASE EVERY 1S EXECUTION START */
			if(timer1_flag==1){
				setTimer1(1000);
				value--;
				if(value<0)
					value=9;
				display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, value);
			}
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* DECREASE EVERY 1S EXECUTION END */

			/* DEC BTN CHANGE NORMAL STATE START */
			if(button_pressed(BTN_DEC)==FALSE){  //INC BTN RELEASED
				state=NORMAL;
				btn_decrease_executed=0;
				reset_var_released_10s();
				setTimer3(1000);
			}
			/* DEC BTN CHANGE NORMAL STATE END */
			break;
		case AUTO_COUNT_DOWN:
			/* AUTO COUNT DOWN EXECUTION START */
			if(timer3_flag==1){
				setTimer3(AUTO_COUNT_DOWN_INTERVAL);
				if(temp_value>0)
					temp_value--;
				display7SEG(SEG_LED1_PORT, SEG_LED1_PIN, temp_value);
			}
			/* LED BLINKING START */
			if(timer4_flag==1){
				setTimer4(LED_BLINKING_INTERVAL);
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
			}
			/* LED BLINKING END */
			/* AUTO COUNT DOWN EXECUTION END */
			if(button_pressed(BTN_INC)){
				value--;
				state=NORMAL;
				reset_var_released_10s();
				setTimer3(1000);
			}else if(button_pressed(BTN_DEC)){
				value++;
				state=NORMAL;
				reset_var_released_10s();
				setTimer3(1000);
			}
			break;
	}
}


void count_BTN_pressed3s(int BTN){
	if(BTN==BTN_DEC){
		if(button_pressed(BTN)){
			if(counter3s_DEC>0)
				counter3s_DEC--;
			if(counter3s_DEC<=0)
				flag3s_DEC=1;
		}
	}else if(BTN==BTN_INC){
		if(button_pressed(BTN)){
			if(counter3s_INC>0)
				counter3s_INC--;
			if(counter3s_INC<=0)
				flag3s_INC=1;
		}
	}
}


int BTN_INC_PRESSED_3s(){
	return flag3s_INC==1;
}
int BTN_DEC_PRESSED_3s(){
	return flag3s_DEC==1;
}

void reset_var_counter_3s(){
	flag3s_INC=0;
	counter3s_INC=3;
	flag3s_DEC=0;
	counter3s_DEC=3;
}

void count_BTN_released10s(){
	if(count_released10s>0)
		count_released10s--;
	if(count_released10s<=0){
		flag10s_released=1;
	}
}

void reset_var_released_10s(){
	count_released10s=10;
	flag10s_released=0;
}

int BTN_RELEASED_10s(){
	return flag10s_released==1;
}



