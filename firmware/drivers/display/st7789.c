#include "st7789.h"
#include "platform/board.h"

#define CMD_SWRESET  0x01
#define CMD_SLPOUT   0x11
#define CMD_COLMOD   0x3A
#define CMD_MADCTL   0x36
#define CMD_CASET    0x2A
#define CMD_RASET    0x2B
#define CMD_RAMWR    0x2C
#define CMD_DISPON   0x29

static inline void cs(ST7789* l, GPIO_PinState s){ HAL_GPIO_WritePin(l->cs_port, l->cs_pin, s); }
static inline void dc(ST7789* l, GPIO_PinState s){ HAL_GPIO_WritePin(l->dc_port, l->dc_pin, s); }

static void spi_tx(ST7789* l, const uint8_t* data, uint16_t n){
  HAL_SPI_Transmit(l->spi, (uint8_t*)data, n, 100);
}

static void cmd(ST7789* l, uint8_t c){
  cs(l, GPIO_PIN_RESET);
  dc(l, GPIO_PIN_RESET);
  spi_tx(l, &c, 1);
  cs(l, GPIO_PIN_SET);
}

static void data8(ST7789* l, const uint8_t* d, uint16_t n){
  cs(l, GPIO_PIN_RESET);
  dc(l, GPIO_PIN_SET);
  spi_tx(l, d, n);
  cs(l, GPIO_PIN_SET);
}

static void data16_repeat(ST7789* l, uint16_t color, uint32_t count){
  uint8_t buf[64];
  for (int i=0;i<64;i+=2){ buf[i]=color>>8; buf[i+1]=color&0xFF; }

  cs(l, GPIO_PIN_RESET);
  dc(l, GPIO_PIN_SET);
  while (count){
    uint32_t chunk = (count > 32) ? 32 : count;
    HAL_SPI_Transmit(l->spi, buf, (uint16_t)(chunk*2), 100);
    count -= chunk;
  }
  cs(l, GPIO_PIN_SET);
}

static void set_window(ST7789* l, uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1){
  x0 += l->xoff; x1 += l->xoff;
  y0 += l->yoff; y1 += l->yoff;

  uint8_t b[4];

  cmd(l, CMD_CASET);
  b[0]=x0>>8; b[1]=x0&0xFF; b[2]=x1>>8; b[3]=x1&0xFF;
  data8(l,b,4);

  cmd(l, CMD_RASET);
  b[0]=y0>>8; b[1]=y0&0xFF; b[2]=y1>>8; b[3]=y1&0xFF;
  data8(l,b,4);

  cmd(l, CMD_RAMWR);
}

void st7789_init(ST7789* lcd, const st7789_init_t* init){
  lcd->spi = init->spi;
  lcd->cs_port=init->cs_port; lcd->cs_pin=init->cs_pin;
  lcd->dc_port=init->dc_port; lcd->dc_pin=init->dc_pin;
  lcd->rst_port=init->rst_port; lcd->rst_pin=init->rst_pin;
  lcd->w=init->width; lcd->h=init->height;
  lcd->xoff=init->x_offset; lcd->yoff=init->y_offset;

  HAL_GPIO_WritePin(lcd->rst_port, lcd->rst_pin, GPIO_PIN_RESET);
  Board_DelayMs(20);
  HAL_GPIO_WritePin(lcd->rst_port, lcd->rst_pin, GPIO_PIN_SET);
  Board_DelayMs(120);

  cmd(lcd, CMD_SWRESET);
  Board_DelayMs(150);

  cmd(lcd, CMD_SLPOUT);
  Board_DelayMs(120);

  cmd(lcd, CMD_COLMOD);
  uint8_t col = 0x55; // 16bpp
  data8(lcd, &col, 1);
  Board_DelayMs(10);

  cmd(lcd, CMD_MADCTL);
  uint8_t madctl = 0x00;
  data8(lcd, &madctl, 1);

  cmd(lcd, CMD_DISPON);
  Board_DelayMs(100);
}

void st7789_fill(ST7789* lcd, uint16_t rgb565){
  set_window(lcd, 0,0, lcd->w-1, lcd->h-1);
  data16_repeat(lcd, rgb565, (uint32_t)lcd->w * (uint32_t)lcd->h);
}

void st7789_draw_pixel(ST7789* lcd, uint16_t x, uint16_t y, uint16_t rgb565){
  if (x>=lcd->w || y>=lcd->h) return;
  set_window(lcd, x,y,x,y);
  uint8_t b[2] = { rgb565>>8, rgb565&0xFF };
  data8(lcd, b, 2);
}