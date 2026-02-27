#include "gfx.h"

static void draw_box(ST7789* lcd, uint16_t x, uint16_t y, uint16_t fg, uint16_t bg){
  for (int dy=0; dy<8; dy++){
    for (int dx=0; dx<6; dx++){
      uint16_t c = (dx==0||dx==5||dy==0||dy==7) ? fg : bg;
      st7789_draw_pixel(lcd, x+dx, y+dy, c);
    }
  }
}

void gfx_draw_char(ST7789* lcd, uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg){
  (void)c;
  draw_box(lcd, x, y, fg, bg);
}

void gfx_draw_string(ST7789* lcd, uint16_t x, uint16_t y, const char* s, uint16_t fg, uint16_t bg){
  uint16_t cx=x;
  while (*s){
    if (*s=='\n'){ y += 10; cx = x; s++; continue; }
    gfx_draw_char(lcd, cx, y, *s, fg, bg);
    cx += 7;
    s++;
  }
}