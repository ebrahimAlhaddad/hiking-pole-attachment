#include <util/delay.h>

#include "gui.h"
#include "tft_lcd.h"
#include "gps.h"

void gui_display_home(void) {
  fill(LCD_BLACK);

  // SOS Button
  uint16_t sos_center_x = lcd_width() / 2;
  uint16_t sos_center_y = lcd_height() / 2 - 40;
  fill_circle(sos_center_x, sos_center_y, BTN_RADIUS, LCD_DARKGREY);
  draw_text(
    sos_center_x - text_width(3) / 2,
    sos_center_y - (CHAR_HEIGHT / 2),
    "SOS", LCD_WHITE, 1);

  // '!' Button
  uint16_t exclaim_center_x = lcd_width() / 2 - 50;
  uint16_t exclaim_center_y = lcd_height() / 2 + 20;
  fill_circle(exclaim_center_x, exclaim_center_y, BTN_RADIUS, LCD_DARKGREY);
  draw_exclaim(
    exclaim_center_x - 16,
    exclaim_center_y - 16,
    LCD_WHITE, 1);

  // Heart Button
	uint16_t heart_center_x = lcd_width() / 2 + 50;
	uint16_t heart_center_y = lcd_height() / 2 + 20;
	fill_circle(heart_center_x, heart_center_y, BTN_RADIUS, LCD_DARKGREY);
	draw_heart(
	heart_center_x - 16,
	heart_center_y - 16,
	LCD_WHITE, 1);

	// Compass Button 
	uint16_t compass_center_x = lcd_width() / 2 ;
	uint16_t compass_center_y= lcd_height() / 2 + 80;
	fill_circle(compass_center_x, compass_center_y, BTN_RADIUS, LCD_DARKGREY);
	draw_text(compass_center_x - text_width(3) / 2 - 5, compass_center_y - text_width(3) / 2 + 15, "COMP",	LCD_WHITE, 1);
}

void gui_display_SOS(void) {
  fill(LCD_RED);

  draw_text(85, 80, "SOS", LCD_BLACK, 3);
  draw_text(70, 130, "SENT", LCD_BLACK, 3);
}

void gui_display_GPS(void){
	fill(LCD_DARKGREY);
	get_data();
	draw_text(130, 20, "GPS", LCD_WHITE, 2);
}

void gui_display_COMPASS(void)
{
	fill(LCD_DARKGREY);
	draw_text(70,30, "COMPASS", LCD_WHITE, 2);
}

void gui_display_PULSE(void)
{
	fill(LCD_BLACK);
	draw_text(90,30, "PULSE", LCD_DARKGREY, 2);
}
