/***************************************************************************
  This is a library for the LSM303 Accelerometer and magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#ifndef __LSM303_H__
#define __LSM303_H__
/*
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"*/

#define LSM303_ADDRESS_ACCEL          (0x32 >> 1)         // 0011001x
#define LSM303_ADDRESS_MAG            (0x3C >> 1)         // 0011110x
#define LSM303_ID                     (0b11010100)

#define LSM303_REGISTER_ACCEL_CTRL_REG1_A          0x20,   // 00000111   rw
#define LSM303_REGISTER_ACCEL_CTRL_REG2_A          0x21,   // 00000000   rw
#define LSM303_REGISTER_ACCEL_CTRL_REG3_A          0x22,   // 00000000   rw
#define LSM303_REGISTER_ACCEL_CTRL_REG4_A          0x23,   // 00000000   rw
#define LSM303_REGISTER_ACCEL_CTRL_REG5_A          0x24,   // 00000000   rw
#define LSM303_REGISTER_ACCEL_CTRL_REG6_A          0x25,   // 00000000   rw
#define LSM303_REGISTER_ACCEL_REFERENCE_A          0x26,   // 00000000   r
#define LSM303_REGISTER_ACCEL_STATUS_REG_A         0x27,   // 00000000   r
#define LSM303_REGISTER_ACCEL_OUT_X_L_A            0x28,//00101000
#define LSM303_REGISTER_ACCEL_OUT_X_H_A            0x29,
#define LSM303_REGISTER_ACCEL_OUT_Y_L_A            0x2A,
#define LSM303_REGISTER_ACCEL_OUT_Y_H_A            0x2B,
#define LSM303_REGISTER_ACCEL_OUT_Z_L_A            0x2C,
#define LSM303_REGISTER_ACCEL_OUT_Z_H_A            0x2D,
#define LSM303_REGISTER_ACCEL_FIFO_CTRL_REG_A      0x2E,
#define LSM303_REGISTER_ACCEL_FIFO_SRC_REG_A       0x2F,
#define LSM303_REGISTER_ACCEL_INT1_CFG_A           0x30,
#define LSM303_REGISTER_ACCEL_INT1_SOURCE_A        0x31,
#define LSM303_REGISTER_ACCEL_INT1_THS_A           0x32,
#define LSM303_REGISTER_ACCEL_INT1_DURATION_A      0x33,
#define LSM303_REGISTER_ACCEL_INT2_CFG_A           0x34,
#define LSM303_REGISTER_ACCEL_INT2_SOURCE_A        0x35,
#define LSM303_REGISTER_ACCEL_INT2_THS_A           0x36,
#define LSM303_REGISTER_ACCEL_INT2_DURATION_A      0x37,
#define LSM303_REGISTER_ACCEL_CLICK_CFG_A          0x38,
#define LSM303_REGISTER_ACCEL_CLICK_SRC_A          0x39,
#define LSM303_REGISTER_ACCEL_CLICK_THS_A          0x3A,
#define LSM303_REGISTER_ACCEL_TIME_LIMIT_A         0x3B,
#define LSM303_REGISTER_ACCEL_TIME_LATENCY_A       0x3C,
#define LSM303_REGISTER_ACCEL_TIME_WINDOW_A        0x3D


#define LSM303_REGISTER_MAG_CRA_REG_M              0x00,
#define LSM303_REGISTER_MAG_CRB_REG_M              0x01,
#define LSM303_REGISTER_MAG_MR_REG_M               0x02,
#define LSM303_REGISTER_MAG_OUT_X_H_M              0x03,
#define LSM303_REGISTER_MAG_OUT_X_L_M              0x04,
#define LSM303_REGISTER_MAG_OUT_Z_H_M              0x05,
#define LSM303_REGISTER_MAG_OUT_Z_L_M              0x06,
#define LSM303_REGISTER_MAG_OUT_Y_H_M              0x07,
#define LSM303_REGISTER_MAG_OUT_Y_L_M              0x08,
#define LSM303_REGISTER_MAG_SR_REG_Mg              0x09,
#define LSM303_REGISTER_MAG_IRA_REG_M              0x0A,
#define LSM303_REGISTER_MAG_IRB_REG_M              0x0B,
#define LSM303_REGISTER_MAG_IRC_REG_M              0x0C,
#define LSM303_REGISTER_MAG_TEMP_OUT_H_M           0x31,
#define LSM303_REGISTER_MAG_TEMP_OUT_L_M           0x32

typedef enum
{
	LSM303_MAGGAIN_1_3                        = 0x20,  // +/- 1.3
	LSM303_MAGGAIN_1_9                        = 0x40,  // +/- 1.9
	LSM303_MAGGAIN_2_5                        = 0x60,  // +/- 2.5
	LSM303_MAGGAIN_4_0                        = 0x80,  // +/- 4.0
	LSM303_MAGGAIN_4_7                        = 0xA0,  // +/- 4.7
    LSM303_MAGGAIN_5_6                        = 0xC0,  // +/- 5.6
	LSM303_MAGGAIN_8_1                        = 0xE0   // +/- 8.1
} lsm303MagGain;	
	
typedef struct lsm303AccelData_s
{
     float x;
     float y;
     float z;
   } lsm303AccelData;
	
typedef struct lsm303MagData_s
{
     float x;
     float y;
     float z;
	 float orientation;
  } lsm303MagData;

bool begin(void);
void compass_read(void);
//void setMagGain(lsm303MagGain gain);

lsm303AccelData accelData;    // Last read accelerometer data will be available here
lsm303MagData magData;        // Last read magnetometer data will be available here

void compass_accel_reg_write8(uint8_t addr, uint8_t data);
void compass_mag_reg_write8(uint8_t addr, uint8_t data);
uint8_t compass_accel_reg_read8(uint8_t addr, int rn);
uint8_t compass_mag_reg_read8(uint8_t addr, int rn);


#endif
