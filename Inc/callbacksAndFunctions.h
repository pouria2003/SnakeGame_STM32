/*
 * callbacksAndFunctions.h
 *
 *  Created on: Jul 6, 2024
 *      Author: lenovo
 */

#ifndef INC_CALLBACKSANDFUNCTIONS_H_
#define INC_CALLBACKSANDFUNCTIONS_H_

#include "main.h"

//--------number to 7448
extern GPIO_TypeDef * pinType;
extern uint16_t bit0;
extern uint16_t bit1;
extern uint16_t bit2;
extern uint16_t bit3;

//--------digit to be turned on
extern uint16_t bitActive0;
extern uint16_t bitActive1;
extern uint16_t bitActive2;
extern uint16_t bitActive3;

//--------Digits
extern uint8_t digits[4];



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
	struct Node *next;
	uint8_t col;
	uint8_t row;
} Node;


typedef struct Snake {
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
	SCROLL0,
	SCROLL1,
};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t strcmpwithlength(const char * str1, const char * str2, const uint8_t len);
void moveSnake();
void initialGame();
void addNodeFront(uint8_t col, uint8_t row);
void flowHandler(uint8_t keypad_button_number, uint8_t item_selected);
void display_digit(uint8_t num, uint8_t digit, uint8_t dcpoint);



#endif /* INC_CALLBACKSANDFUNCTIONS_H_ */
