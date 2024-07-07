/*
 * callbacksAndFunctions.c
 *
 *  Created on: Jul 6, 2024
 *      Author: lenovo
 */


#include "callbacksAndFunctions.h"
#include "customChars.h"
#include "main.h"
#include "cmsis_os.h"
#include <malloc.h>


#define NTHREADS 6

extern uint8_t game_state;

extern osThreadId MenuPageHandle;
extern osThreadId startHandle;
extern osThreadId settingHandle;
extern osThreadId modeHandle;
extern osThreadId aboutHandle;
extern enum Threads;

extern uint8_t tsignals[NTHREADS];

Snake snake = {NULL};

uint8_t direction = RIGHT;

void flowHandler(uint8_t keypad_button_number, uint8_t item_selected) {
	switch(game_state) {
	case INTRO:
		tsignals[INTRO_T] = 0;
		game_state = MENU;
		osSignalSet(MenuPageHandle, 0);
		break;
	case MENU:
		tsignals[MENU_T] = keypad_button_number;
		osSignalSet(MenuPageHandle, 0);
		break;
	case START:
		//to do
		break;
	case SETTING:
		tsignals[SETTING_T] = keypad_button_number;
		osSignalSet(settingHandle, 0);
		break;
	case MODE:
		tsignals[MODE_T] = keypad_button_number;
		osSignalSet(modeHandle, 0);
	case ABOUT:
		tsignals[ABOUT_T] = keypad_button_number;
//		osSignalSet(aboutHandle, 0);
	}

}

void addNodeFront(uint8_t col, uint8_t row, uint8_t custom_char)
{
	Node *new_node = (Node *)malloc(sizeof(Node));

	new_node->next = NULL;
	if(snake.snake_tail != NULL)
		snake.snake_tail->next = new_node;
	new_node->col = col;
	new_node->row = row;
	new_node->custom_char_ind = custom_char;
	if(snake.snake_head == NULL)
		snake.snake_head = new_node;
	snake.snake_tail = new_node;
}

void initialGame()
{
	createChar(0, snake_body);
	createChar(1, head_up);
	createChar(2, head_right);
	createChar(3, head_down);
	createChar(4, head_left);

	addNodeFront(1, 0, 2);
	addNodeFront(0, 0, 0);


}


void moveSnake() {
	// clear one node of snake body
	setCursor(snake.snake_head->col, snake.snake_head->row);
	print(" ");

	uint8_t prev_col = snake.snake_head->col;
	uint8_t prev_row = snake.snake_head->row;

	switch(direction) {
	case UP:
		if(snake.snake_head->row == 0)
			snake.snake_head->row = 4;
		snake.snake_head->row -= 1;
		break;
	case RIGHT:
		snake.snake_head->col += 1;
		if(snake.snake_head->col == 20)
			snake.snake_head->col = 0;
		break;
	case DOWN:
		snake.snake_head->row += 1;
		if(snake.snake_head->row == 4)
			snake.snake_head->row = 0;
		break;
	case LEFT:
		if(snake.snake_head->col == 0)
			snake.snake_head->row = 20;
		snake.snake_head->row -= 1;
		break;
	}

	setCursor(snake.snake_head->col, snake.snake_head->row);
	write(snake.snake_head->custom_char_ind);


	Node* current_node = snake.snake_head->next;

	while(current_node != NULL) {
		// clear one node of snake body
		setCursor(current_node->col, current_node->row);
		print(" ");
		// updating node
		current_node->col = prev_col;
		current_node->row = prev_row;
		// printing it in new position
		setCursor(current_node->col, current_node->row);
		write(current_node->custom_char_ind);
		current_node = current_node->next;
	}



}







// Input pull down rising edge trigger interrupt pins:
// Row1 PD3, Row2 PD5, Row3 PD7, Row4 PB4
GPIO_TypeDef *const Row_ports[] = {GPIOD, GPIOC, GPIOC, GPIOC};
const uint16_t Row_pins[] = {GPIO_PIN_15, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8};
// Output pins: Column1 PD4, Column2 PD6, Column3 PB3, Column4 PB5
GPIO_TypeDef *const Column_ports[] = {GPIOC, GPIOA, GPIOA, GPIOA};
const uint16_t Column_pins[] = {GPIO_PIN_9, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10};
volatile uint32_t last_gpio_exti;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (last_gpio_exti + 200 > HAL_GetTick()) // Simple button debouncing
  {
    return;
  }
  last_gpio_exti = HAL_GetTick();

  int8_t row_number = -1;
  int8_t column_number = -1;

  if (GPIO_Pin == GPIO_PIN_0)
  {
    // blue_button_pressed = 1;
    // return;
  }

  for (uint8_t row = 0; row < 4; row++) // Loop through Rows
  {
    if (GPIO_Pin == Row_pins[row])
    {
      row_number = row;
    }
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);

  for (uint8_t col = 0; col < 4; col++) // Loop through Columns
  {
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
    if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number]))
    {
      column_number = col;
    }
    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
  }

  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);

  if (row_number == -1 || column_number == -1)
  {
    return; // Reject invalid scan
  }
  //   C0   C1   C2   C3
  // +----+----+----+----+
  // | 1  | 2  | 3  | 4  |  R0
  // +----+----+----+----+
  // | 5  | 6  | 7  | 8  |  R1
  // +----+----+----+----+
  // | 9  | 10 | 11 | 12 |  R2
  // +----+----+----+----+
  // | 13 | 14 | 15 | 16 |  R3
  // +----+----+----+----+
  const uint8_t button_number = row_number * 4 + column_number + 1;


  flowHandler(button_number, 0);

}
