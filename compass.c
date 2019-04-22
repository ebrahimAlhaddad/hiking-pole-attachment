#include <avr/io.h>
#include <stdlib.h>
#include "touch.h"
#include "i2c.h"
#include "serial.h"

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
bool Adafruit_LSM303::begin(){
	// Enable the accelerometer
	compass_accel_reg_write8(LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x27);
	  
	// Enable the magnetometer
 	compass_mag_reg_write8(LSM303_REGISTER_MAG_MR_REG_M, 0x00);
	
	return true;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
 
 void Adafruit_LSM303::compass_read(){
   // Read the accelerometer
   uint8_t compass_accel_set[];
   compass_accel_reg_write8(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80, 1);/////////right?
   compass_accel_set[] = compass_accel_reg_read8(LSM303_ADDRESS_ACCEL, 6);
   
   // Wait around until enough data is available
   while (sizeof(compass_accel_set) < 6);
   
  uint8_t xlo = compass_accel_set[0];
  uint8_t xhi = compass_accel_set[1];
  uint8_t ylo = compass_accel_set[2];
  uint8_t yhi = compass_accel_set[3];
  uint8_t zlo = compass_accel_set[4];
  uint8_t zhi = compass_accel_set[5];
  
  accelData.x = (int16_t)((xhi << 8) | xlo);
  accelData.y = (int16_t)((yhi << 8) | ylo);
  accelData.z = (int16_t)((zhi << 8) | zlo);
  
  // Read the magnetometer
   uint8_t compass_mag_set[];
   compass_mag_reg_write8(LSM303_REGISTER_MAG_OUT_X_H_M, 1);////////what to write
   compass_mag_set[] = compass_mag_reg_read8(LSM303_ADDRESS_MAGL, 6);
   
   // Wait around until enough data is available
  while (sizeof(compass_mag_set) < 6);
   // Note high before low (different than accel)  
  xhi = compass_mag_set[0];
  xlo = compass_mag_set[1];
  zhi = compass_mag_set[2];
  zlo = compass_mag_set[3];
  yhi = compass_mag_set[4];
  ylo = compass_mag_set[5];
  
  magData.x = (xlo | (xhi << 8));
  magData.y = (ylo | (yhi << 8));
  magData.z = (zlo | (zhi << 8));  
  
  // ToDo: Calculate orientation
  magData.orientation = 0.0;
  
 }
 
 
 
 
 


/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
 void Adafruit_LSM303::compass_accel_reg_write8(uint8_t addr, uint8_t data){
 	i2c_io(LSM303_ADDRESS_ACCEL, &addr, 1, &data, 1, NULL, 0);
 }
 
 void Adafruit_LSM303::compass_mag_reg_write8(uint8_t addr, uint8_t data){
 	i2c_io(LSM303_ADDRESS_MAG, &addr, 1, &data, 1, NULL, 0);
 }
 
uint8_t Adafruit_LSM303::compass_accel_reg_read8(uint8_t addr, int rn) {
	uint8_t read_byte;
	i2c_io(LSM303_ADDRESS_ACCE, &addr, 1, NULL, 0, &read_byte, rn);
	return read_byte;
}

uint8_t Adafruit_LSM303::compass_mag_reg_read8(uint8_t addr, int rn) {
	uint8_t read_byte;
	i2c_io(LSM303_ADDRESS_MAG, &addr, 1, NULL, 0, &read_byte, rn);
	return read_byte;
}
 