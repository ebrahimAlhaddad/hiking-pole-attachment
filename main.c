#include <util/delay.h>

#include "serial.h"
#include "spi.h"
#include "i2c.h"
#include "tft_lcd.h"
#include "touch.h"
#include "gui.h"
#include "gps.h"

int main(void) {
  serial_init();
  spi_init();
  i2c_init();
  lcd_init();

  int flag = 0; 
  //gui_display_home();

  while (1) {
    // fill_rect(100, 100, 12*4, 16*4, LCD_CYAN);
    // draw_char(100, 100, t, LCD_RED, 4);
    // t += 1;
    // if (t > 0x5F) {
    //   t = 'A';
    // }
    // draw_exclaim(100, 100, LCD_RED, 1);
	if (flag == 0)
	{
		gui_display_home();
		flag = 1; 
	}
	else if (flag ==1){
		// flag is one wait for next touch 
		read_touch_data();
		if (touch_pressed()) {
			point_t touch = touch_get_point(0);
			flag = 1; 
			char str[10];
			sprintf(str, "Touch - (%d, %d)", touch.x, touch.y);
			sprintln(str);

			// TODO Points must match rotation
			if (touch.x >= 150 && touch.x <= 170) {
				if (touch.y >= 150 && touch.y <= 170) {
					gui_display_SOS();
					flag = 2; 
				}
			}
			else if (touch.x >= 100 && touch.x <= 140) {
				if (touch.y >= 100 && touch.y <= 140) {
					gui_display_GPS();
					flag = 2; 
				}
			}

		}
	}
	if(flag == 2)
	{
		read_touch_data();
		if (touch_pressed()) {
			point_t touch = touch_get_point(0);
			flag = 1; 
			char str[10];
			sprintf(str, "Touch - (%d, %d)", touch.x, touch.y);
			sprintln(str);

			// TODO Points must match rotation
			if (touch.x >= 0 ) {
				if (touch.y >= 0) {
					gui_display_home();
					flag = 1;  
				}
			}
			
		}
	}
    _delay_ms(50);
  }
  
}

// sprintln("Hello World");
// _delay_ms(1000);
