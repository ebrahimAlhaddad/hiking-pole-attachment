#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "tft_lcd.h"
#include "serial.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

void lcd_init(void) {
  DDRB |= LCD_PORTB;

  lcd_reset();

  lcd_reg_write8(ILI9341_SOFTRESET, 0);
  _delay_ms(150);
  lcd_reg_write8(ILI9341_DISPLAYOFF, 0);

  // LCD_DC_Command;
  // lcd_write_byte(0xEF);
  // LCD_DC_Data;
  // lcd_write_byte(0x03);
  // lcd_write_byte(0x80);
  // lcd_write_byte(0x02);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xCF);
  // LCD_DC_Data;
  // lcd_write_byte(0x00);
  // lcd_write_byte(0xC1);
  // lcd_write_byte(0x30);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xED);
  // LCD_DC_Data;
  // lcd_write_byte(0x64);
  // lcd_write_byte(0x03);
  // lcd_write_byte(0x12);
  // lcd_write_byte(0x81);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xE8);
  // LCD_DC_Data;
  // lcd_write_byte(0x85);
  // lcd_write_byte(0x00);
  // lcd_write_byte(0x78);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xCB);
  // LCD_DC_Data;
  // lcd_write_byte(0x39);
  // lcd_write_byte(0x2C);
  // lcd_write_byte(0x00);
  // lcd_write_byte(0x34);
  // lcd_write_byte(0x02);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xF7);
  // LCD_DC_Data;
  // lcd_write_byte(0x20);
  //
  // LCD_DC_Command;
  // lcd_write_byte(0xEA);
  // LCD_DC_Data;
  // lcd_write_byte(0x00);
  // lcd_write_byte(0x00);

  lcd_reg_write8(ILI9341_POWERCONTROL1, 0x23);
  lcd_reg_write8(ILI9341_POWERCONTROL2, 0x10);
  lcd_reg_write16(ILI9341_VCOMCONTROL1, 0x3E28);
  lcd_reg_write8(ILI9341_VCOMCONTROL2, 0x86);
  lcd_reg_write8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
  lcd_reg_write8(ILI9341_PIXELFORMAT, 0x55);
  lcd_reg_write16(ILI9341_FRAMECONTROL, 0x0018);

  lcd_reg_write8(ILI9341_ENTRYMODE, 0x07);

  lcd_reg_write8(ILI9341_SLEEPOUT, 0x0);
  _delay_ms(150);
  lcd_reg_write8(ILI9341_DISPLAYON, 0x0);
  _delay_ms(500);
}

/*
  lcd_reset - Force reset LCD
*/
void lcd_reset() {
  PORTB &= ~(1 << RST_PIN);
  _delay_ms(1);
  PORTB |= (1 << RST_PIN);
  _delay_ms(200);
}

/*
  lcd_write_byte - Using SPI, write a byte to the LCD
*/
void lcd_write_byte(uint8_t byte) {
  LCD_CS_Active;
  spi_write(byte);
  LCD_CS_Negate;
}

/*
  lcd_reg_write8 - Output a 8-bit register address and 8-bit data value to the LCD
*/
void lcd_reg_write8(uint8_t addr, uint8_t data) {
  LCD_CS_Active;

  LCD_DC_Command;
  lcd_write_byte(addr);

  LCD_DC_Data;
  lcd_write_byte(data);

  LCD_CS_Negate;
}

/*
  lcd_reg_write16 - Output a 8-bit register address and 16-bit data value to the LCD
*/
void lcd_reg_write16(uint8_t addr, uint16_t data) {
  uint8_t hi, lo;

  LCD_CS_Active;

  LCD_DC_Command;
  lcd_write_byte(addr);

  hi = data >> 8;
  lo = data & 0xff;
  LCD_DC_Data;
  lcd_write_byte(hi);
  lcd_write_byte(lo);

  LCD_CS_Negate;
}

void lcd_reg_write32(uint8_t addr, uint32_t data) {
  LCD_CS_Active;

  LCD_DC_Command;
  lcd_write_byte(addr);

  LCD_DC_Data;
  lcd_write_byte(data >> 24);
  lcd_write_byte(data >> 16);
  lcd_write_byte(data >> 8);
  lcd_write_byte(data & 0xff);

  LCD_CS_Negate;
}

/*
  lcd_set_addr_window - Sets the LCD address window
*/
void lcd_set_addr_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  uint16_t x2 = x + width - 1;
  uint16_t y2 = y + height - 1;

  // Check bounds
  if (x < 0) {
    x = 0;
  }
  if (x2 >= LCD_Width) {
    x2 = LCD_Width - 1;
  }
  if (y < 0) {
    y = 0;
  }
  if (y2 >= LCD_Height) {
    y2 = LCD_Height - 1;
  }

  uint32_t w;

  // Create 32 bit int (x1 concat x2) and set column address window
  w = (uint32_t) x << 16;
  w |= x2;
  lcd_reg_write32(ILI9341_COLADDRSET, w);

  w = (uint32_t) y << 16;
  w |= y2;
  lcd_reg_write32(ILI9341_PAGEADDRSET, w);
}

/*
  fill - Fills the screen with "color"
*/
void fill(uint16_t color) {
  draw_box(0, 0, LCD_Width, LCD_Height, color);
}

/*
  draw_box - Draw a box from (x, y), upper-left, to (x + width, y + height), lower-right
  and fill it with the 16-bit color value in "color"
*/
void draw_box(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  lcd_set_addr_window(x, y, width, height);

  LCD_CS_Active;

  LCD_DC_Command;
  lcd_write_byte(ILI9341_MEMORYWRITE);

  LCD_DC_Data;
  uint32_t num_pixels = (uint32_t) width * height;

  uint8_t hi, lo;
  hi = color >> 8;
  lo = color & 0xff;

  while (num_pixels--) {
    lcd_write_byte(hi);
    lcd_write_byte(lo);
  }

  LCD_CS_Negate;
}


void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
  draw_box(x, y, 1, 1, color);
}

void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
  // === Taken from https://github.com/adafruit/Adafruit-GFX-Library ===
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
      _swap_int16_t(x0, y0);
      _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
      _swap_int16_t(x0, x1);
      _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
      ystep = 1;
  } else {
      ystep = -1;
  }

  for (; x0 <= x1; x0++) {
      if (steep) {
          draw_pixel(y0, x0, color);
      } else {
          draw_pixel(x0, y0, color);
      }
      err -= dy;
      if (err < 0) {
          y0 += ystep;
          err += dx;
      }
  }
}

void draw_vert_line(int16_t x0, int16_t y0, int16_t height, int16_t color) {
  draw_line(x0, y0, x0, y0 + height - 1, color);
}

void draw_circle(int16_t x0, int16_t y0, int16_t r, int16_t color) {
  // === Taken from https://github.com/adafruit/Adafruit-GFX-Library ===
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  // Top, Right, Bottom, Left
  draw_pixel(x0, y0 + r, color);
  draw_pixel(x0, y0 - r, color);
  draw_pixel(x0 + r, y0, color);
  draw_pixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    draw_pixel(x0 + x, y0 + y, color);
    draw_pixel(x0 - x, y0 + y, color);
    draw_pixel(x0 + x, y0 - y, color);
    draw_pixel(x0 - x, y0 - y, color);
    draw_pixel(x0 + y, y0 + x, color);
    draw_pixel(x0 - y, y0 + x, color);
    draw_pixel(x0 + y, y0 - x, color);
    draw_pixel(x0 - y, y0 - x, color);
  }
}

void fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  draw_vert_line(x0, y0-r, 2*r + 1, color);
  fill_circle_helper(x0, y0, r, 0b11, 0, color);
}

/*
* corners is a mask that indicates which quarters to fill
*/
void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (x < (y + 1)) {
      if (corners & 0b01) draw_vert_line(x0 + x, y0 + y, 2*y, color);
      if (corners & 0b11) draw_vert_line(x0 - x, y0 - y, 2*y, color);
    }

    if (y != py) {
      if (corners & 0b01) draw_vert_line(x0 + py, y0 + px, 2*x, color);
      if (corners & 0b11) draw_vert_line(x0 - py, y0 - px, 2*x, color);
      py = y;
    }
    px = x;
  }
}
