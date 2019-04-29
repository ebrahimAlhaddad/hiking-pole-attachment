#include <util/delay.h>
#include <math.h>

#include "gui.h"
#include "tft_lcd.h"
#include "gps.h"
#include "pulse.h"
#include "compass.h"
#include "touch.h"

int state = INITIAL;

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
  while (1) {
    fill(LCD_DARKGREY);
    draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 7) / 2) * 2, 30, "COMPASS", LCD_WHITE, 2);

    uint8_t compass_width = 60;
    draw_horiz_line(lcd_width() / 2 - compass_width / 2, lcd_height() / 2 + 5, compass_width, LCD_WHITE);
    draw_vert_line(lcd_width() / 2, lcd_height() / 2 - compass_width / 2 + 5, compass_width, LCD_WHITE);
    draw_text(lcd_width() / 2 - compass_width / 2 - CHAR_WIDTH - 10, lcd_height() / 2 + 5 - CHAR_HEIGHT / 2, "W", LCD_WHITE, 1);
    draw_text(lcd_width() / 2 + compass_width / 2, lcd_height() / 2 + 5 - CHAR_HEIGHT / 2, "E", LCD_WHITE, 1);
    draw_text(lcd_width() / 2 - CHAR_WIDTH, lcd_height() / 2 - compass_width / 2 + 5 - CHAR_HEIGHT - 10, "N", LCD_WHITE, 1);
    draw_text(lcd_width() / 2 - CHAR_WIDTH, lcd_height() / 2 + compass_width / 2 + 5 + 10, "S", LCD_WHITE, 1);

    coord_data_t accel = compass_get_accel();
    coord_data_t mag = compass_get_mag();

    uint16_t accel_angle = (uint16_t) atan2(accel.y, accel.x);
    uint16_t mag_angle = (uint16_t) atan2(mag.y, mag.x);

    uint16_t draw_angle = accel_angle + mag_angle;
    uint16_t x2 = (uint16_t) cos(draw_angle) * compass_width;
    uint16_t y2 = (uint16_t) sin(draw_angle) * compass_width;

    draw_line(lcd_width() / 2, lcd_height() / 2 + 20, lcd_width() / 2 + x2, lcd_height() / 2 + y2 + 20, LCD_RED);

    char accel_str[20];
    char mag_str[20];
    sprintf(accel_str, "ACCEL: %d %d %d", accel.x, accel.y, accel.z);
    sprintf(mag_str, "MAG: %d %d %d", mag.x, mag.y, mag.z);
    draw_text(10, lcd_height() - CHAR_HEIGHT * 2 - 20, accel_str, LCD_WHITE, 1);
    draw_text(10, lcd_height() - CHAR_HEIGHT - 10, mag_str, LCD_WHITE, 1);

    _delay_ms(1000);

    // EXTREMELY HACKY ONLY CAUSE WE'RE IN A LOOP
    read_touch_data();
    if (touch_pressed()) {
      point_t touch = touch_get_point(0);

      // TODO Points must match rotation
      if (touch.x >= 0 ) {
        if (touch.y >= 0) {
          break;
        }
      }
    }

    _delay_ms(1000);
  }

  gui_display_home();
}

void gui_display_pulse(void)
{
  fill(LCD_BLACK);
  draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 5) / 2) * 2, 30, "PULSE", LCD_WHITE, 2);
  uint16_t bpm_test = calc_BPM();
  if(bpm_test < 35 || bpm_test > 230){
    draw_text(lcd_width() / 2 - ((CHAR_WIDTH * 8) / 2) * 2, 130, "BAD DATA", LCD_RED, 2);
  }
  else {
    char bpm_string[5];
    sprintf(bpm_string, "%d", bpm_test);
    draw_text(lcd_width() / 2 - (CHAR_WIDTH * strlen(bpm_string)) * 3, 130, bpm_string, LCD_WHITE, 3);
  }
}

void gui_loop(void) {
  if (state == INITIAL) {
    gui_display_home();
    state = HOME;
  }
  else if (state == HOME) {
    // state is one wait for next touch
    read_touch_data();
    if (touch_pressed()) {
      point_t touch = touch_get_point(0);
      //state = HOME;

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
        state = HOME; // ALSO A HACK
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

      // TODO Points must match rotation
      if (touch.x >= 0 ) {
        if (touch.y >= 0) {
          gui_display_home();
          state = HOME;
        }
      }
    }
  }
}
