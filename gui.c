#include <util/delay.h>

#include "gui.h"
#include "tft_lcd.h"
#include "gps.h"
#include "pulse.h"
void gui_display_home(void) {
  fill(LCD_BLACK);

  // SOS Button
  uint16_t sos_center_x = lcd_width() / 2;
  uint16_t sos_center_y = lcd_height() / 2 - 50;
  fill_circle(sos_center_x, sos_center_y, BTN_RADIUS, LCD_DARKGREY);
  draw_text(
    sos_center_x - text_width(3) / 2,
    sos_center_y - (CHAR_HEIGHT / 2),
    "SOS", LCD_WHITE, 1);

  // GPS Button
  uint16_t gps_center_x = lcd_width() / 2 - 50;
  uint16_t gps_center_y = lcd_height() / 2 ;
  fill_circle(gps_center_x, gps_center_y, BTN_RADIUS, LCD_DARKGREY);
  draw_gps(
    gps_center_x - 16,
    gps_center_y - 16,
    LCD_WHITE, 1);

  // Heart Button
	uint16_t heart_center_x = lcd_width() / 2 + 50;
	uint16_t heart_center_y = lcd_height() / 2;
	fill_circle(heart_center_x, heart_center_y, BTN_RADIUS, LCD_DARKGREY);
	draw_heart(
  	heart_center_x - 16,
  	heart_center_y - 16,
  	LCD_WHITE, 1);

	// Compass Button
	uint16_t compass_center_x = lcd_width() / 2 ;
	uint16_t compass_center_y= lcd_height() / 2 + 50;
	fill_circle(compass_center_x, compass_center_y, BTN_RADIUS, LCD_DARKGREY);
  draw_compass(
  	compass_center_x - 16,
  	compass_center_y - 16,
  	LCD_WHITE, 1);
}

void gui_display_sos(void) {
  fill(LCD_RED);

  draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 3) / 2) * 3, 80, "SOS", LCD_BLACK, 3);
  draw_text(lcd_width() / 2 - (CHAR_WIDTH * 2) * 3, 130, "SENT", LCD_BLACK, 3);
}

void gui_display_gps(void){
	fill(LCD_DARKGREY);
  draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 3) / 2) * 2, 20, "GPS", LCD_WHITE, 2);
	gps_get_data();
}

void gui_display_compass(void)
{
	fill(LCD_DARKGREY);
	draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 7) / 2) * 2, 30, "COMPASS", LCD_WHITE, 2);
}

void gui_display_pulse(void)
{
  fill(LCD_BLACK);
  draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 5) / 2) * 2, 30, "PULSE", LCD_DARKGREY, 2);
  uint16_t test = calc_BPM();
  if(test < 35 || test > 230){
      draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 5) / 2) * 2, 130, "BAD DATA", LCD_RED, 2);
  } else {
    char temp_bpm[5];
    sprintf(temp_bpm," %d ",test);
    draw_text(lcd_width() / 2 - (CHAR_WIDTH * 2) * 3, 130, temp_bpm, LCD_WHITE, 3);
  }
}
