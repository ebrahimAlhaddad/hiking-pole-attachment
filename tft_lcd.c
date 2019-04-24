#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "tft_lcd.h"
#include "spi.h"
#include "serial.h"

#include "font_bitmap.h"
#include "icon_bitmap.h"

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

  lcd_set_rotation(3);
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

uint16_t lcd_width(void) {
  return _width;
}

uint16_t lcd_height(void) {
  return _height;
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
  if (x2 >= _width) {
    x2 = _width - 1;
  }
  if (y < 0) {
    y = 0;
  }
  if (y2 >= _height) {
    y2 = _height - 1;
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
  Set origin of (0,0) and orientation of TFT display
  'rotation' is between 0-3 inclusive
*/
void lcd_set_rotation(uint8_t rotation) {
  rotation %= 4;
  uint8_t lcd_rot_data;
  switch (rotation) {
    case 0:
      // Portrait
      lcd_rot_data = (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
      _width = LCD_Width;
      _height = LCD_Height;
      break;
    case 1:
      // Landscape
      lcd_rot_data = (ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
      _width = LCD_Height;
      _height = LCD_Width;
      break;
    case 2:
      // Portrait Reversed
      lcd_rot_data = (ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
      _width = LCD_Width;
      _height = LCD_Height;
      break;
    case 3:
      // Landscape Reversed
      lcd_rot_data = (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
      _width = LCD_Height;
      _height = LCD_Width;
      break;
  }

  lcd_reg_write8(ILI9341_MEMCONTROL, lcd_rot_data);
}

/*
  fill - Fills the screen with "color"
*/
void fill(uint16_t color) {
  fill_rect(0, 0, _width, _height, color);
}

/*
  fill_rect - Draw a box from (x, y), upper-left, to (x + width, y + height), lower-right
  and fill it with the 16-bit color value in "color"
*/
void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
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
  fill_rect(x, y, 1, 1, color);
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

void draw_horiz_line(int16_t x0, int16_t y0, int16_t width, int16_t color) {
  draw_line(x0, y0, x0 + width - 1, y0, color);
}

void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
  draw_vert_line(x, y, height, color);
  draw_horiz_line(x, y, width, color);
  draw_vert_line(x + width - 1, y, height, color);
  draw_horiz_line(x, y + height - 1, width, color);
}

void draw_circle(int16_t x0, int16_t y0, int16_t r, int16_t color) {
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

void fill_circle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color) {
  draw_vert_line(x0, y0-r, 2*r + 1, color);
  fill_circle_helper(x0, y0, r, 0b11, color);
}

/*
  corners is a mask that indicates which quarters to fill
*/
void fill_circle_helper(int16_t x0, int16_t y0, int16_t r,
    uint8_t corners, uint16_t color) {
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
      if (corners & 0b01) draw_vert_line(x0 + x, y0 - y, 2*y+1, color);
      if (corners & 0b10) draw_vert_line(x0 - x, y0 - y, 2*y+1, color);
    }

    if (y != py) {
      if (corners & 0b01) draw_vert_line(x0 + py, y0 - px, 2*px+1, color);
      if (corners & 0b10) draw_vert_line(x0 - py, y0 - px, 2*px+1, color);
      py = y;
    }
    px = x;
  }
}

/*
  Writes a 12x16 character (0x2E - 0x5F or '.' - '_') using the font bitmap at (x, y)
*/
void draw_char(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size) {
  if (c < 0x2E || c > 0x5F) {
    // Out of charset range
    return;
  }

  const uint8_t *char_bitmap = font[c - 0x2E];
  uint16_t i = 0, j;
  // From the LSB, read 6 bits at a time
  // for (i = 0; i < 4; ++i) { // 4 int16's per line
  //   uint16_t entry = char_bitmap[i]; // Each element represents 2 lines
  //   uint8_t line1 = (entry >> 6) & 0xF;
  //   uint8_t line2 = entry & 0xF;
  //
  //   for (j = 0; j < 6; ++j, line1 >>= 1, line2 >>= 1) {
  //     if (line1 & 0x1) {
  //       fill_rect(x + j*size, y + (2*i)*size, size, size, color);
  //     }
  //     if (line2 & 0x1) {
  //       fill_rect(x + j*size, y + (2*i + 1)*size, size, size, color);
  //     }
  //   }
  // }

  for (i = 0; i < 8; i++) {
    // Read 3 entries at a time (2 lines)
    // Format:
    // Line 1: aab
    // Line 2: bcc
    uint8_t a = char_bitmap[3*i];
    uint8_t b = char_bitmap[3*i + 1];
    uint8_t c = char_bitmap[3*i + 2];

    uint16_t line1, line2; // 12 bit lines
    line1 = ((uint16_t)a << 4) | (b >> 4);
    line2 = ((uint16_t)(b & 0x0F) << 8) | ((uint16_t)c);

    for (j = 0; j < 12; j++, line1 >>= 1, line2 >>= 1) {
      if (line1 & 0x1) {
        fill_rect(x + (11 - j)*size, y + (2*i)*size, size, size, color);
      }
      if (line2 & 0x1) {
        fill_rect(x + (11 - j)*size, y + (2*i + 1)*size, size, size, color);
      }
    }
  }
}

void draw_text(uint16_t x, uint16_t y, char* str, uint16_t color, uint8_t size) {
  uint16_t letter_spacing = 5;

  unsigned char i;
  for (i = 0; i < (unsigned) strlen(str); ++i) {
    draw_char(x + i*size*CHAR_WIDTH + letter_spacing, y, str[i], color, size);
  }
}

uint16_t text_width(int num_chars) {
  return CHAR_WIDTH * num_chars + (5 * (num_chars - 1));
}

/*
  Draw 32x32 icon at (x, y) specified by the icon bitmap 'icon'
*/
void draw_icon(uint16_t x, uint16_t y, const uint8_t icon[32][4], uint16_t color, uint8_t size) {
  uint16_t i, j, k;
  for (i = 0; i < 32; ++i) {
    // Create 32-bit line
    uint32_t line = 0;
    for (j = 0; j < 4; ++j) {
      line |= (uint32_t) icon[i][j] << (8 * (3 - j));
    }

    for (k = 0; k < 32; ++k, line >>= 1) {
      if (line & 0x01) {
        fill_rect(x + (31 - k)*size, y + i*size, size, size, color);
      }
    }
  }
}

void draw_gps(uint16_t x, uint16_t y, uint16_t color, uint8_t size) {
  draw_icon(x, y, gps_icon, color, size);
}

void draw_heart(uint16_t x, uint16_t y, uint16_t color, uint8_t size) {
  draw_icon(x, y, heart_icon, color, size);
}

void draw_compass(uint16_t x, uint16_t y, uint16_t color, uint8_t size) {
  draw_icon(x, y, compass_icon, color, size);
}
