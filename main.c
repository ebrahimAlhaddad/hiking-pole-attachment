#include <util/delay.h>

#include "serial.h"
#include "spi.h"
#include "i2c.h"
#include "tft_lcd.h"
#include "touch.h"
#include "gui.h"
#include "gps.h"
#include "compass.h"

// STATE CONSTANTS
#define INITIAL 0
#define HOME    1
#define OTHER   2  // In a screen other than Home

int main(void) {
  serial_init();
  spi_init();
  i2c_init();
  lcd_init();

  int state = 0;

  while (1) {
    // fill_rect(100, 100, 12*4, 16*4, LCD_CYAN);
    // draw_char(100, 100, t, LCD_RED, 4);
    // t += 1;
    // if (t > 0x5F) {
    //   t = 'A';
    // }
    // draw_exclaim(100, 100, LCD_RED, 1);
  	if (state == INITIAL) {
  		gui_display_home();
  		state = HOME;
  	}
  	else if (state == HOME) {
  		// state is one wait for next touch
  		read_touch_data();
  		if (touch_pressed()) {
  			point_t touch = touch_get_point(0);
  			state = HOME;

  			// char str[10];
  			// sprintf(str, "Touch - (%d, %d)", touch.x, touch.y);
  			// sprintln(str);

  			// TODO Points must match rotation
  			if (
          touch.x >= 145 && touch.x <= 170 &&
          touch.y >= 145 && touch.y <= 170
        ) {
  				gui_display_sos();
  				state = OTHER;
  			}
        else if (
          touch.x >= 95 && touch.x <= 135 &&
          touch.y >= 85 && touch.y <= 130
        ) {
  				gui_display_gps();
  				state = OTHER;
  			}
        else if (
          touch.x >= 40 && touch.x <= 90 &&
          touch.y >= 145 && touch.y <= 180
        ) {
  				gui_display_compass();
  				state = OTHER;
  			}
        else if (
          touch.x >= 100 && touch.x <= 140 &&
          touch.y >= 200 && touch.y <= 240
        ) {
  				gui_display_pulse();
  				state = OTHER;
  			}
  		}
  	}
  	else if(state == OTHER) {
  		read_touch_data();
  		if (touch_pressed()) {
  			point_t touch = touch_get_point(0);
  			state = HOME;


  			// TODO Points must match rotation
  			if (touch.x >= 0 ) {
  				if (touch.y >= 0) {
  					gui_display_home();
  					state = HOME;
  				}
  			}

  		}
  	}
    _delay_ms(50);
  }

}

// sprintln("Hello World");
// _delay_ms(1000);
