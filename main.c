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
  draw_line(20, 50, 70, 60, LCD_RED);

  while (1) {

  }
}

// sprintln("Hello World");
// _delay_ms(1000);
