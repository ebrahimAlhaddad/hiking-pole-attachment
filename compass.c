#include <stdbool.h>
#include <util/delay.h>

#include "i2c.h"
#include "compass.h"
#include "serial.h"

/***************************************************************************
FUNCTIONS
 ***************************************************************************/
void compass_init(){
	// Enable the accelerometer
	compass_accel_reg_write8(LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);

	// uint8_t reg1_a;
	// compass_accel_reg_read8(LSM303_REGISTER_ACCEL_CTRL_REG1_A, &reg1_a, 1);
	// char str1[10];
	// sprintf(str1, "reg1_a %d", reg1_a);
	// sprintln(str1);
  // if (reg1_a != 0x57)
  // {
  //   sprintln("broken");
  // }

	// sprintln("true");
	// Enable the magnetometer
 	// compass_mag_reg_write8(LSM303_REGISTER_MAG_MR_REG_M, 0x00);

}


void compass_read(){
	// Read the accelerometer
	uint8_t compass_accel_set[6];
	compass_accel_reg_write8(LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80, 0);
	compass_accel_reg_read8(LSM303_ADDRESS_ACCEL, compass_accel_set, 6);
	//
	// // Wait around until enough data is available
	// while (sizeof(compass_accel_set) < 6);

	uint8_t xlo = compass_accel_set[0];
	uint8_t xhi = compass_accel_set[1];
	uint8_t ylo = compass_accel_set[2];
	uint8_t yhi = compass_accel_set[3];
	uint8_t zlo = compass_accel_set[4];
	uint8_t zhi = compass_accel_set[5];

	accelData.x = (int16_t)((xhi << 8) | xlo);
	accelData.y = (int16_t)((yhi << 8) | ylo);
	accelData.z = (int16_t)((zhi << 8) | zlo);

	char str[10];
  sprintf(str, "Acceleration - (%d, %d, %d)", accelData.x, accelData.y, accelData.z);
	sprintln(str);

	// // Read the magnetometer
	// uint8_t compass_mag_set[6];
	// compass_mag_reg_write8(LSM303_REGISTER_MAG_OUT_X_H_M, 0);
	// compass_mag_reg_read8(LSM303_ADDRESS_MAG, compass_mag_set, 6);
	//
	//  // Wait around until enough data is available
	// while (sizeof(compass_mag_set) < 6);
	//  // Note high before low (different than accel)
	// xhi = compass_mag_set[0];
	// xlo = compass_mag_set[1];
	// zhi = compass_mag_set[2];
	// zlo = compass_mag_set[3];
	// yhi = compass_mag_set[4];
	// ylo = compass_mag_set[5];
	//
	// magData.x = (xlo | (xhi << 8));
	// magData.y = (ylo | (yhi << 8));
	// magData.z = (zlo | (zhi << 8));
	//
	// // ToDo: Calculate orientation
	// magData.orientation = 0.0;
}


void compass_accel_reg_write8(uint8_t addr, uint8_t data){
 	i2c_io(LSM303_ADDRESS_ACCEL, &addr, 1, &data, 1, NULL, 0);
}

void compass_mag_reg_write8(uint8_t addr, uint8_t data){
 	i2c_io(LSM303_ADDRESS_MAG, &addr, 1, &data, 1, NULL, 0);
}

void compass_accel_reg_read8(uint8_t addr, uint8_t* readbuf, int rn) {
	i2c_io(LSM303_ADDRESS_ACCEL, &addr, 1, NULL, 0, readbuf, rn);
}

void compass_mag_reg_read8(uint8_t addr, uint8_t* readbuf, int rn) {
	i2c_io(LSM303_ADDRESS_MAG, &addr, 1, NULL, 0, readbuf, rn);
}
