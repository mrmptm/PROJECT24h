/*
 * button.c
 *
 *  Created on: Oct 7, 2022
 *      Author: DELL
 */
#include "button.h"
int debounceButtonBuffer2[NUM_OF_BTN];
int debounceButtonBuffer1[NUM_OF_BTN];
int buttonBuffer[NUM_OF_BTN];

int flagForButtonPress1s[NUM_OF_BTN];
int counterForButtonPress1s[NUM_OF_BTN];

/* 	BUTTON1 PARAMETER END */
GPIO_TypeDef*BTN_GPIO_Port[NUM_OF_BTN];
uint16_t BTN_GPIO_Pin[NUM_OF_BTN];
/* 	BUTTON2 PARAMETER END */
void BTN_INIT(GPIO_TypeDef*GPIO_Port,uint16_t GPIO_Pin,int idx){
	BTN_GPIO_Port[idx]=GPIO_Port;
	BTN_GPIO_Pin[idx]=GPIO_Pin;
	debounceButtonBuffer1[idx]=NORMAL_STATE;
	debounceButtonBuffer2[idx]=NORMAL_STATE;
	buttonBuffer[idx]=NORMAL_STATE;
	flagForButtonPress1s[idx]=0;

	counterForButtonPress1s[idx]=1000;

}

void button_reading(int idx){
	debounceButtonBuffer2[idx] =debounceButtonBuffer1[idx];
	debounceButtonBuffer1[idx] = HAL_GPIO_ReadPin(BTN_GPIO_Port[idx], BTN_GPIO_Pin[idx]);
	if(debounceButtonBuffer1[idx] == debounceButtonBuffer2[idx])
		buttonBuffer[idx] = debounceButtonBuffer1[idx];

	if(buttonBuffer[idx] == PRESSED_STATE){
	//if a button is pressed, we start counting
		if(counterForButtonPress1s[idx] >0){
			counterForButtonPress1s[idx]=counterForButtonPress1s[idx]-TIMER_CYCLE;
		} else {
		//the flag is turned on when 1 second has passed
		//since the button is pressed.
			flagForButtonPress1s[idx] = 1;

		}

	}else if(buttonBuffer[idx] == NORMAL_STATE){
		/* BUTTON PRESSED START */
		counterForButtonPress1s[idx] = 1000;
		flagForButtonPress1s[idx] = 0;

		/* BUTTON PRESSED END */

	}
}


int button_pressed(int idx){
	return (buttonBuffer[idx] == PRESSED_STATE);
}


int button_pressed_1s(int idx){
	return (flagForButtonPress1s[idx] == 1);
}






