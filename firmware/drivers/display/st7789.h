#pragma once
#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
  SPI_HandleTypeDef* spi;
  GPIO_TypeDef* cs_port; uint16_t cs_pin;
  GPIO_TypeDef* dc_port; uint16_t dc_pin;
  GPIO_TypeDef* rst_port; uint16_t rst_pin;
  uint16_t width;
  uint16_t height;
  uint16_t x_offset;
  uint16_t y_offset;
} st7789_init_t;

typedef struct {
  SPI_HandleTypeDef* spi;
  GPIO_TypeDef* cs_port; uint16_t cs_pin;
  GPIO_TypeDef* dc_port; uint16_t dc_pin;
  GPIO_TypeDef* rst_port; uint16_t rst_pin;
  uint16_t w,h;
  uint16_t xoff,yoff;
} ST7789;

void st7789_init(ST7789* lcd, const st7789_init_t* init);
void st7789_fill(ST7789* lcd, uint16_t rgb565);
void st7789_draw_pixel(ST7789* lcd, uint16_t x, uint16_t y, uint16_t rgb565);