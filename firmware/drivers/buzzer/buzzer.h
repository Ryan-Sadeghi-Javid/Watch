#pragma once
#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
  TIM_HandleTypeDef* tim;
  uint32_t channel;
} Buzzer;

void buzzer_init(Buzzer* b, TIM_HandleTypeDef* tim, uint32_t channel);
void buzzer_tone(Buzzer* b, uint32_t hz, uint8_t duty_percent, uint32_t duration_ms);
void buzzer_stop(Buzzer* b);