#include <util/delay.h>

#include "serial.h"
#include "spi.h"
#include "i2c.h"
#include "tft_lcd.h"
#include "touch.h"
#include "gui.h"

int main(void) {
  serial_init();
  spi_init();
  i2c_init();
  lcd_init();

  gui_display_home();

  while (1) {
    // fill_rect(100, 100, 12*4, 16*4, LCD_CYAN);
    // draw_char(100, 100, t, LCD_RED, 4);
    // t += 1;
    // if (t > 0x5F) {
    //   t = 'A';
    // }
    // draw_exclaim(100, 100, LCD_RED, 1);

    read_touch_data();
    if (touch_pressed()) {
      point_t touch = touch_get_point(0);

      char str[10];
      sprintf(str, "Touch - (%d, %d)", touch.x, touch.y);
      sprintln(str);

      // TODO Points must match rotation
      if (touch.x >= 150 && touch.x <= 170) {
        if (touch.y >= 150 && touch.y <= 170) {
          gui_display_SOS();
        }
      }

    }
    _delay_ms(50);
  }
}

// sprintln("Hello World");
// _delay_ms(1000);
