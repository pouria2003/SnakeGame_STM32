/*
 * buzzer.h
 *
 *  Created on: Jul 7, 2024
 *      Author: amin
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_




#include "main.h"

typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;


void PWM_Start();
void PWM_Change_Tone(uint16_t pwm_freq, uint16_t volume);
void Change_Melody(const Tone *melody, uint16_t tone_count);
void Update_Melody();
uint16_t array_length(Tone* arr);









#endif /* INC_BUZZER_H_ */
