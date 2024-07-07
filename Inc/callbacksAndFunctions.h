/*
 * callbacksAndFunctions.h
 *
 *  Created on: Jul 6, 2024
 *      Author: lenovo
 */

#ifndef INC_CALLBACKSANDFUNCTIONS_H_
#define INC_CALLBACKSANDFUNCTIONS_H_

#include "main.h"

enum GameState {
	INTRO,
	MENU,
	START,
	SETTING,
	MODE,
	ABOUT
};

enum Directions {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef struct Node {
	uint8_t custom_char_ind;
	struct Node *next;
	uint8_t col;
	uint8_t row;
} Node;


typedef struct Snake{
	Node *snake_tail;
	Node *snake_head;
} Snake;

enum Threads{
	INTRO_T = 0,
	MENU_T,
	START_T,
	SETTING_T,
	MODE_T,
	ABOUT_T
};


enum ChooseState {
	CHOOSING,
	CHANGING,

};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



#endif /* INC_CALLBACKSANDFUNCTIONS_H_ */
