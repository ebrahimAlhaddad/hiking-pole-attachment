#include <avr/io.h>
#include <stdlib.h>

#include "touch.h"
#include "i2c.h"
#include "serial.h"


uint8_t touch_reg_read8(uint8_t addr) {
  uint8_t read_byte;
  i2c_io(FT6206_ADDR, &addr, 1, NULL, 0, &read_byte, 1);

  return read_byte;
}

void touch_reg_write8(uint8_t addr, uint8_t data) {
  i2c_io(FT6206_ADDR, &addr, 1, &data, 1, NULL, 0);
}

/*
  Reads all of the touch information at once
  This way is faster since all data comes immediately one after another
*/
void read_touch_data(void) {
  uint8_t addr = 0x00;
  uint8_t touch_data[16];
  i2c_io(FT6206_ADDR, &addr, 1, NULL, 0, touch_data, 16);

  #ifdef TOUCH_DEBUG
    sprintln("TOUCH RECEIVING: ");
    uint16_t i;
    for (i = 0; i < 16; i++) {
      char str[10];
      sprintf(str, "%x ", touch_data[i]);
      sprint(str);
    }
    sprintln(" ");
  #endif

  num_touches = touch_data[FT6206_NUM_TOUCHES];
  if (num_touches > 2) {
    // FT6206 only allows for 2 touches
    num_touches = 0;
  }

  if (num_touches > 0) {
    touchX[0] = touch_data[FT6206_TOUCH1_XH] & 0x0F;
    touchX[0] <<= 8;
    touchX[0] |= touch_data[FT6206_TOUCH1_XL];
    touchY[0] = touch_data[FT6206_TOUCH1_YH] & 0x0F;
    touchY[0] <<= 8;
    touchY[0] |= touch_data[FT6206_TOUCH1_YL];
    touchID[0] = (touch_data[FT6206_TOUCH1_YH] & 0xF0) >> 4;
  }

  if (num_touches > 1) {
    touchX[1] = touch_data[FT6206_TOUCH2_XH] & 0x0F;
    touchX[1] <<= 8;
    touchX[1] |= touch_data[FT6206_TOUCH2_XL];
    touchY[1] = touch_data[FT6206_TOUCH2_YH] & 0x0F;
    touchX[1] <<= 8;
    touchY[1] |= touch_data[FT6206_TOUCH2_YL];
    touchID[1] = (touch_data[FT6206_TOUCH2_YH] & 0xF0) >> 4;
  }
}

bool touch_pressed(void) {
  // num_touches = 0, 1, or 2
  return num_touches > 0;
}

point_t touch_get_point(uint8_t index) {
  point_t point = {touchID[index], touchX[index], touchY[index]};
  return point;
}
