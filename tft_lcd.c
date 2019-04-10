#include <avr/io.h>
#include <util/delay.h>
#include "tft_lcd.h"
#include "serial.h"

void lcd_init(void) {
  DDRB |= LCD_PORTB;

  sprintln("Test");
  lcd_reg_write8(ILI9341_SOFTRESET, 0);
  _delay_ms(50);
  sprintln("Test2");
  lcd_reg_write8(ILI9341_DISPLAYOFF, 0);

  lcd_reg_write8(ILI9341_POWERCONTROL1, 0x23);
  lcd_reg_write8(ILI9341_POWERCONTROL2, 0x10);
  lcd_reg_write16(ILI9341_VCOMCONTROL1, 0x2B2B);
  lcd_reg_write8(ILI9341_VCOMCONTROL2, 0xC0);
  lcd_reg_write8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
  lcd_reg_write8(ILI9341_PIXELFORMAT, 0x55);
  lcd_reg_write16(ILI9341_FRAMECONTROL, 0x001B);

  lcd_reg_write8(ILI9341_ENTRYMODE, 0x07);
  lcd_reg_write8(ILI9341_SLEEPOUT, 0);
  _delay_ms(150);
  lcd_reg_write8(ILI9341_DISPLAYON, 0);
  _delay_ms(500);
}

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
