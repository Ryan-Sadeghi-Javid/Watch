#pragma once
#include <stdint.h>
#include "st7789.h"

void gfx_draw_char(ST7789* lcd, uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg);
void gfx_draw_string(ST7789* lcd, uint16_t x, uint16_t y, const char* s, uint16_t fg, uint16_t bg);