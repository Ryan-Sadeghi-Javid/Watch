#include "board.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_SPI1_Init(void);
void MX_TIM3_Init(void);

void Board_Init(void) {
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();

  // turn on backlight
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
}

void Board_DelayMs(uint32_t ms) { HAL_Delay(ms); }
uint32_t Board_Millis(void) { return HAL_GetTick(); }