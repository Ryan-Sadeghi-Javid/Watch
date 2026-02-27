#pragma once
#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;

// ------------ LCD GPIO ------------
#define LCD_CS_GPIO_Port   GPIOA
#define LCD_CS_Pin         GPIO_PIN_4

#define LCD_DC_GPIO_Port   GPIOA
#define LCD_DC_Pin         GPIO_PIN_6

#define LCD_RST_GPIO_Port  GPIOA
#define LCD_RST_Pin        GPIO_PIN_7

#define LCD_BL_GPIO_Port   GPIOB
#define LCD_BL_Pin         GPIO_PIN_0

// ------------ Optional LED ------------
#define LED_GPIO_Port      GPIOC
#define LED_Pin            GPIO_PIN_13

// ------------ Buzzer PWM ------------
#define BUZZER_TIM         (&htim3)
#define BUZZER_TIM_CH      TIM_CHANNEL_1

// Board init hooks
void Board_Init(void);
void Board_DelayMs(uint32_t ms);
uint32_t Board_Millis(void);