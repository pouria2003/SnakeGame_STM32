/*
 * callbacksAndFunctions.h
 *
 *  Created on: Jul 6, 2024
 *      Author: lenovo
 */

#ifndef INC_CALLBACKSANDFUNCTIONS_H_
#define INC_CALLBACKSANDFUNCTIONS_H_

#include "main.h"

enum Threads{
	INTRO_PAGE = 0,
	MENU_PAGE,
	START,
	SETTING,
	MODE,
	ABOUT
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



#endif /* INC_CALLBACKSANDFUNCTIONS_H_ */
