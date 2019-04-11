#include <util/delay.h>

#include "serial.h"
#include "spi.h"
#include "tft_lcd.h"

void init_ports() {

}

void init_memory() {

}

int main(void) {
  serial_init();
  spi_init();
  lcd_init();

  while (1) {
    lcd_set_addr_window(0, 0, LCD_Width - 1, LCD_Height - 1);

    LCD_CS_Active;

    LCD_DC_Command;
    lcd_write_byte(ILI9341_MEMORYWRITE);
    sprintln("Memory Write");

    LCD_DC_Data;
    uint8_t hi, lo;
    hi = LCD_RED >> 8;
    lo = LCD_RED & 0xff;

    int r, c;
    for (r = 0; r < LCD_Width; ++r) {
      for (c = 0; c < LCD_Height; ++c) {
        lcd_write_byte(hi);
        lcd_write_byte(lo);
      }
    }

    LCD_CS_Negate;
  }
}

// sprintln("Hello World");
// _delay_ms(1000);
