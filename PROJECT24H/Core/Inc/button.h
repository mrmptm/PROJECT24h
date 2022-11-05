/*
 * button.h
 *
 *  Created on: Oct 7, 2022
 *      Author: DELL
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NUM_OF_BTN 3
#define NORMAL_STATE 	1
#define PRESSED_STATE 0
#define TIMER_CYCLE 10
#define DOUBLE_CLICK_INTERVAL 500
void BTN_INIT(GPIO_TypeDef*GPIO_Port,uint16_t GPIO_Pin,int idx);
void button_reading(int idx);

int button_pressed(int idx);

int button_pressed_1s(int idx);

#endif /* INC_BUTTON_H_ */
