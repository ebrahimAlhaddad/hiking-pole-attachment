#ifndef _TOUCH_H_INCLUDED
#define _TOUCH_H_INCLUDED

#include <stdbool.h>

// #define TOUCH_DEBUG

// 7-Bit Address: 0x38
#define FT6206_ADDR 0x70 // 8-Bit Address

// FT6206 Registers
#define FT6206_DEV_MODE     0x00 // Device Mode
#define FT6206_GEST_ID      0x01 // Gesture ID
#define FT6206_NUM_TOUCHES  0x02 // Number of Touches
#define FT6206_TOUCH1_XH    0x03 // 1st Touch Event Flag and X Position [11:8] (first 3 bits)
#define FT6206_TOUCH1_XL    0x04 // 1st Touch X Position [7:0]
#define FT6206_TOUCH1_YH    0x05 // 1st Touch ID and Y Position [11:8] (first 3 bits)
#define FT6206_TOUCH1_YL    0x06 // 1st Touch Y Position [7:0]
#define FT6206_TOUCH2_XH    0x09 // 2nd Touch Event Flag and X Position [11:8] (first 3 bits)
#define FT6206_TOUCH2_XL    0x0A // 2nd Touch X Position [7:0]
#define FT6206_TOUCH2_YH    0x0B // 2nd Touch ID and Y Position [11:8] (first 3 bits)
#define FT6206_TOUCH2_YL    0x0C // 2nd Touch Y Position [7:0]
#define FT6206_THRESHOLD    0x80 // Touch Threshold (Force)

typedef struct point {
  uint8_t id;
  uint16_t x, y; // TODO: Add force
} point_t;

uint8_t num_touches;
uint8_t touchID[2]; // 4-bit touch id
uint16_t touchX[2], touchY[2]; // 12-bit touch coords

uint8_t touch_reg_read8(uint8_t addr);
void touch_reg_write8(uint8_t addr, uint8_t data);
void read_touch_data(void);

bool touch_pressed(void);
point_t touch_get_point(uint8_t index);

#endif
