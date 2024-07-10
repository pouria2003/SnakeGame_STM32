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


Snake snake = {NULL};

uint8_t direction = RIGHT;

void flowHandler(uint8_t keypad_button_number, uint8_t item_selected) {
	switch(game_state) {
	case INTRO:
		tsignals[INTRO_T] = 0;
		game_state = MENU;
//		osSignalSet(MenuPageHandle, 0);
		break;
	case MENU:
		tsignals[MENU_T] = keypad_button_number;
		osSignalSet(MenuPageHandle, 0);
		break;
	case START:
		tsignals[START_T] = keypad_button_number;
		switch(keypad_button_number){
		case 2:
			if(direction != DOWN)
				direction = UP;
			break;
		case 6:
			if(direction != LEFT)
				direction = RIGHT;
			break;
		case 4:
			if(direction != RIGHT)
				direction = LEFT;
			break;
		case 8:
			if(direction != UP)
				direction = DOWN;
			break;
		}
//		osSignalSet(startHandle, 0);
		break;
	case SETTING:
		tsignals[SETTING_T] = keypad_button_number;
		osSignalSet(settingHandle, 0);
		break;
	case MODE:
		tsignals[MODE_T] = keypad_button_number;
		osSignalSet(modeHandle, 0);
		break;
	case ABOUT:
		tsignals[ABOUT_T] = keypad_button_number;
		osSignalSet(aboutHandle, 0);
		break;
	}

}

void addNodeFront(uint8_t col, uint8_t row)
{
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->col = col;
	new_node->row = row;

	if(snake.snake_head == NULL){
		snake.snake_head = new_node;
		snake.snake_head->next = snake.snake_head;
	}
	else {
		new_node->next = snake.snake_head->next;
		snake.snake_head->next = new_node;
	}
}

void initialGame()
{
	createChar(0, snake_body);
	createChar(1, head_up);
	createChar(2, head_right);
	createChar(3, head_down);
	createChar(4, head_left);

	addNodeFront(2, 0);
	addNodeFront(1, 0);
	addNodeFront(0, 0);

}

void moveSnake() {
//	// clear one node of snake body
//	setCursor(snake.snake_head->col, snake.snake_head->row);
//	print(" ");
//
//	uint8_t prev_col = snake.snake_head->col;
//	uint8_t prev_row = snake.snake_head->row;
//
//	switch(direction) {
//	case UP:
//		if(snake.snake_head->row == 0)
//			snake.snake_head->row = 4;
//		snake.snake_head->row -= 1;
//		break;
//	case RIGHT:
//		snake.snake_head->col += 1;
//		if(snake.snake_head->col == 20)
//			snake.snake_head->col = 0;
//		break;
//	case DOWN:
//		snake.snake_head->row += 1;
//		if(snake.snake_head->row == 4)
//			snake.snake_head->row = 0;
//		break;
//	case LEFT:
//		if(snake.snake_head->col == 0)
//			snake.snake_head->row = 20;
//		snake.snake_head->row -= 1;
//		break;
//	}
//
//	setCursor(snake.snake_head->col, snake.snake_head->row);
//	write();
//
//
//	Node* current_node = snake.snake_head->next;
//
//	while(current_node != NULL) {
//		// clear one node of snake body
//		setCursor(current_node->col, current_node->row);
//		print(" ");
//		// updating node
//		current_node->col = prev_col;
//		current_node->row = prev_row;
//		// printing it in new position
//		setCursor(current_node->col, current_node->row);
//		write(current_node->custom_char_ind);
//		current_node = current_node->next;
//	}
}

uint8_t strcmpwithlength(const char * str1, const char * str2, const uint8_t len)
{
	for(int i = 0 ; i < len; ++i) {
		if(str1[i] != str2[i])
			return 0;
	}
	return 1;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	const char* HEALTH = "health";
	const char* SPEED = "speed";
	const char* SOUNDS = "sounds"; const char* ON = "on"; const char* OFF = "off";
	const char* BLOCKS = "blocks";
	const char* NAME = "name";
	if(huart->Instance == USART1) {

		HAL_UART_Receive_IT(&huart1, &receive, 1);
		received_data[data_index] = receive;
		++data_index;

		if(receive == '\r') {
//			for(int i = 0; received_data[i] != '\r'; ++i) {
//				pressed_number *= 10;
//				pressed_number += received_data[i] - '0';
//			}
			if(received_data[0] <= '9' && received_data[0] >= '0') {
				flowHandler(received_data[0] - '0', 0);
				transmit_data[0] = '\0';

			} else if(strcmpwithlength(received_data, HEALTH, 6) && received_data[6] == ':' && data_index < 10) {
				initial_health = received_data[7] - '0';
				sprintf(transmit_data, "Initial health changed to %d (max = 9)", initial_health);
				sprintf(setting_options[0] + 10, "%d", initial_health);

			} else if(strcmpwithlength(received_data, SPEED, 5) && received_data[5] == ':' && data_index < 9) {
				initial_speed = received_data[6] - '0';
				sprintf(transmit_data, "Initial speed changed to %d (max = 9)", initial_speed);
				sprintf(setting_options[1] + 10, "%d", initial_speed);

			} else if(strcmpwithlength(received_data, SOUNDS, 6) && received_data[6] == ':' && data_index < 12
					  && (strcmpwithlength(received_data + 7, ON, 2) || strcmpwithlength(received_data + 7, OFF, 3))) {
				sound_state = received_data[8] == 'n' ? 1 : 0;
				sprintf(transmit_data, "Sounds changed to %s", sound_state ? "on" : "off");
				sprintf(setting_options[2] + 10, "%s", sound_state ? "on " : "off");

			} else if(strcmpwithlength(received_data, BLOCKS, 6) && received_data[6] == ':' && data_index < 10) {
				initial_health = received_data[7] - '0';
				if(blocks_number > 6) blocks_number = 6;
				  sprintf(setting_options[3] + 10, "%d", blocks_number);
				sprintf(transmit_data, "Number of blocks changed to %d (max = 6)", blocks_number);

			} else if(strcmpwithlength(received_data, NAME, 4) && received_data[4] == ':' && data_index < 13) {
				sprintf(player_name, "%s", received_data + 5);
				player_name[data_index - 6] = '\0';
				//name:amin/t
				sprintf(transmit_data, "Player name changed to %s (max character = 6)", player_name);
				sprintf(setting_options[4] + 10, "%s", player_name);

			} else {
				sprintf(transmit_data, "Unvalid command try again", player_name);
			}
			HAL_UART_Transmit(&huart1, transmit_data, strlen(transmit_data), HAL_MAX_DELAY);
			data_index = 0;
		}
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
	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
  if (last_gpio_exti + 200 > HAL_GetTick()) // Simple button debouncing
  {
    return;
  }
//  last_gpio_exti = HAL_GetTick();
//
//  int8_t row_number = -1;
//  int8_t column_number = -1;
//
//  if (GPIO_Pin == GPIO_PIN_0)
//  {
//    // blue_button_pressed = 1;
//    // return;
//  }
//
//  for (uint8_t row = 0; row < 4; row++) // Loop through Rows
//  {
//    if (GPIO_Pin == Row_pins[row])
//    {
//      row_number = row;
//    }
//  }
//
//  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 0);
//  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 0);
//  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 0);
//  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 0);
//
//  for (uint8_t col = 0; col < 4; col++) // Loop through Columns
//  {
//    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
//    if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number]))
//    {
//      column_number = col;
//    }
////    HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
//  }
//
////  HAL_GPIO_WritePin(Column_ports[0], Column_pins[0], 1);
////  HAL_GPIO_WritePin(Column_ports[1], Column_pins[1], 1);
////  HAL_GPIO_WritePin(Column_ports[2], Column_pins[2], 1);
////  HAL_GPIO_WritePin(Column_ports[3], Column_pins[3], 1);
//
//  if (row_number == -1 || column_number == -1)
//  {
//    return; // Reject invalid scan
//  }
//  //   C0   C1   C2   C3
//  // +----+----+----+----+
//  // | 1  | 2  | 3  | 13  |  R0   9
//  // +----+----+----+----+
//  // | 4  | 5  | 6  | 14 |  R1    7
//  // +----+----+----+----+
//  // | 7  | 8  | 9  | 15 |  R2    5
//  // +----+----+----+----+
//  // | 10 | 11 | 12 | 16 |  R3    3
//  // +----+----+----+----+
//
//  uint8_t button_number = row_number * 3 + column_number + 1;
//  if(column_number == 3) {
//	  button_number += 2 * (4-row_number) + 1;
//  }
//
//
//  flowHandler(button_number, 0);

}
