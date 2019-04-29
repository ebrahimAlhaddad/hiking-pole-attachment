#include <util/delay.h>

#include "serial.h"
#include "spi.h"
#include "i2c.h"
#include "tft_lcd.h"
#include "touch.h"
#include "gui.h"
#include "gps.h"
#include "compass.h"
#include "pulse.h"

//volatile uint16_t global_bpm;

int main(void) {
  serial_init();
  spi_init();
  i2c_init();
  lcd_init();
  //pulse_init();
  compass_init();

  while (1) {
    // fill_rect(100, 100, 12*4, 16*4, LCD_CYAN);
    // draw_char(100, 100, t, LCD_RED, 4);
    // t += 1;
    // if (t > 0x5F) {
    //   t = 'A';
    // }
    // draw_exclaim(100, 100, LCD_RED, 1);
    gui_loop();
    _delay_ms(50);
  }

}


// ISR (TIMER0_COMPA_vect){
//   global_bpm = calc_BPM();
// }
// sprintln("Hello World");
// _delay_ms(1000);
