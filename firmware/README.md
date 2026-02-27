# Watch Firmware

Firmware for Watch-style Wearable PCB:
- STM32 (HAL)
- I2C accelerometer (LIS3DH-style driver; adaptable)
- SPI LCD (ST7789 driver; adaptable)
- PWM buzzer
- SWD debugging

## Build steps
1) Create STM32CubeIDE project with CubeMX (.ioc)
2) Enable I2C, SPI, PWM timer, GPIO pins listed in platform/board.h
3) Drop these sources into Core/Src and headers into Core/Inc (or keep folder structure and update include paths)
4) Compile + flash via ST-Link (SWD)

## Bring-up order
1) Blink LED
2) I2C WHO_AM_I read
3) LCD init + fill screen
4) PWM buzzer tone test
5) Integrate main app loop