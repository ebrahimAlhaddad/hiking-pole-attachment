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

  fill(LCD_OLIVE);
  _delay_ms(2);
  // draw_line(20, 50, 70, 60, LCD_RED);
  // fill_circle(100, 100, 50, LCD_CYAN);
  // draw_char(100, 100, 'A', LCD_RED, 3);

  uint8_t t = 'A';
  while (1) {
    fill_rect(100, 100, 12*4, 16*4, LCD_CYAN);
    draw_char(100, 100, t, LCD_RED, 4);
    t += 1;
    if (t > 0x5F) {
      t = 'A';
    }

    _delay_ms(1000);
  }
}

// sprintln("Hello World");
// _delay_ms(1000);
