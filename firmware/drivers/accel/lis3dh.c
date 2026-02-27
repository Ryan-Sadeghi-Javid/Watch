#include "lis3dh.h"

#define LIS3DH_REG_WHOAMI     0x0F
#define LIS3DH_REG_CTRL1      0x20
#define LIS3DH_REG_CTRL4      0x23
#define LIS3DH_REG_OUT_X_L    0x28

static bool i2c_read(Lis3dh* d, uint8_t reg, uint8_t* buf, uint16_t n) {
  return HAL_I2C_Mem_Read(d->i2c, (uint16_t)(d->addr_7bit << 1),
                         reg, I2C_MEMADD_SIZE_8BIT, buf, n, 100) == HAL_OK;
}

static bool i2c_write(Lis3dh* d, uint8_t reg, uint8_t val) {
  return HAL_I2C_Mem_Write(d->i2c, (uint16_t)(d->addr_7bit << 1),
                          reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100) == HAL_OK;
}

bool lis3dh_init(Lis3dh* dev, const lis3dh_init_t* init) {
  dev->i2c = init->i2c;
  dev->addr_7bit = init->addr_7bit;

  uint8_t who = 0;
  if (!i2c_read(dev, LIS3DH_REG_WHOAMI, &who, 1)) return false;

  if (!i2c_write(dev, LIS3DH_REG_CTRL1, 0x57)) return false;

  if (!i2c_write(dev, LIS3DH_REG_CTRL4, 0x08)) return false;

  return true;
}

bool lis3dh_read_xyz(Lis3dh* dev, int16_t* x, int16_t* y, int16_t* z) {
  uint8_t raw[6] = {0};

  if (!i2c_read(dev, LIS3DH_REG_OUT_X_L | 0x80, raw, 6)) return false;

  int16_t rx = (int16_t)((raw[1] << 8) | raw[0]);
  int16_t ry = (int16_t)((raw[3] << 8) | raw[2]);
  int16_t rz = (int16_t)((raw[5] << 8) | raw[4]);

  *x = rx >> 4;
  *y = ry >> 4;
  *z = rz >> 4;
  return true;
}