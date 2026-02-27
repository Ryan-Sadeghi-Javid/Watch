#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef struct {
  I2C_HandleTypeDef* i2c;
  uint8_t addr_7bit;
} lis3dh_init_t;

typedef struct {
  I2C_HandleTypeDef* i2c;
  uint8_t addr_7bit;
} Lis3dh;

bool lis3dh_init(Lis3dh* dev, const lis3dh_init_t* init);
bool lis3dh_read_xyz(Lis3dh* dev, int16_t* x, int16_t* y, int16_t* z);