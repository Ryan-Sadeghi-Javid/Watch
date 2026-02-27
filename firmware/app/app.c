#include "app.h"
#include "platform/board.h"
#include "platform/log.h"
#include "drivers/accel/lis3dh.h"
#include "drivers/display/st7789.h"
#include "drivers/display/gfx.h"
#include "drivers/buzzer/buzzer.h"

static Lis3dh accel;
static ST7789 lcd;
static Buzzer buzzer;

static uint32_t last_led = 0;
static uint32_t last_ui  = 0;

// --------- lightweight int -> string helpers (no snprintf/printf) ---------
static char* write_u16(char* p, uint16_t v) {
  char tmp[5];
  int n = 0;

  if (v == 0) {
    *p++ = '0';
    return p;
  }

  while (v > 0 && n < (int)sizeof(tmp)) {
    tmp[n++] = (char)('0' + (v % 10));
    v /= 10;
  }
  while (n--) *p++ = tmp[n];
  return p;
}

static char* write_i16(char* p, int16_t v) {
  if (v < 0) {
    *p++ = '-';
    uint16_t mag = (uint16_t)(-(int32_t)v);
    return write_u16(p, mag);
  }
  return write_u16(p, (uint16_t)v);
}

static void build_xyz_line(char* out, size_t out_sz, int16_t x, int16_t y, int16_t z) {
  if (out_sz == 0) return;

  char* p = out;
  char* end = out + out_sz - 1;

  if (p < end) *p++ = 'X';
  if (p < end) *p++ = ':';
  if (p < end) p = write_i16(p, x);

  if (p < end) *p++ = ' ';
  if (p < end) *p++ = 'Y';
  if (p < end) *p++ = ':';
  if (p < end) p = write_i16(p, y);

  if (p < end) *p++ = ' ';
  if (p < end) *p++ = 'Z';
  if (p < end) *p++ = ':';
  if (p < end) p = write_i16(p, z);

  for (int i = 0; i < 6 && p < end; i++) *p++ = ' '; // clear old chars

  *p = '\0';
}

void App_Init(void) {
  // ----- Accelerometer -----
  lis3dh_init_t ainit = {
    .i2c = &hi2c1,
    .addr_7bit = 0x18,
  };
  if (!lis3dh_init(&accel, &ainit)) {
    LOGE("Accel init failed");
  } else {
    LOGI("Accel ok");
  }

  // ----- LCD -----
  st7789_init_t linit = {
    .spi = &hspi1,
    .cs_port = LCD_CS_GPIO_Port, .cs_pin = LCD_CS_Pin,
    .dc_port = LCD_DC_GPIO_Port, .dc_pin = LCD_DC_Pin,
    .rst_port = LCD_RST_GPIO_Port, .rst_pin = LCD_RST_Pin,
    .width = 240,
    .height = 280,
    .x_offset = 0,
    .y_offset = 20,
  };

  st7789_init(&lcd, &linit);
  st7789_fill(&lcd, 0x0000);

  // ----- Buzzer -----
  buzzer_init(&buzzer, BUZZER_TIM, BUZZER_TIM_CH);
  buzzer_tone(&buzzer, 1200, 20, 120);

  gfx_draw_string(&lcd, 10, 10, "WATCH BOOT", 0xFFFF, 0x0000);
}

void App_Tick(void) {
  uint32_t now = Board_Millis();

  // 2Hz BLINKING
  if (now - last_led > 250) {
    last_led = now;
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  }

  // 10Hz UI updates
  if (now - last_ui > 100) {
    last_ui = now;

    int16_t x=0,y=0,z=0;
    if (lis3dh_read_xyz(&accel, &x, &y, &z)) {
      char buf[48];
        build_xyz_line(buf, sizeof(buf), x, y, z);
        gfx_draw_string(&lcd, 10, 40, buf, 0x07E0, 0x0000);
    }
  }
}