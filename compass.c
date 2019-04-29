#include <stdbool.h>
#include <util/delay.h>
#include <math.h>

#include "i2c.h"
#include "compass.h"
#include "serial.h"

/***************************************************************************
FUNCTIONS
 ***************************************************************************/
void compass_init(){
	// Enable the accelerometer
	compass_accel_reg_write8(LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);

	// Enable the magnetometer
 	compass_mag_reg_write8(LSM303_REGISTER_MAG_MR_REG_M, 0x00);

}


void compass_read_accel(){
	// Read the accelerometer
	uint8_t compass_accel_set[6];

	// Compass returns in 2s Complement
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_X_L_A, &compass_accel_set[0], 1);
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_X_H_A, &compass_accel_set[1], 1);
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_Y_L_A, &compass_accel_set[2], 1);
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_Y_H_A, &compass_accel_set[3], 1);
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_Z_L_A, &compass_accel_set[4], 1);
	compass_accel_reg_read8(LSM303_REGISTER_ACCEL_OUT_Z_H_A, &compass_accel_set[5], 1);

	uint8_t xlo = compass_accel_set[0];
	uint8_t xhi = compass_accel_set[1];
	uint8_t ylo = compass_accel_set[2];
	uint8_t yhi = compass_accel_set[3];
	uint8_t zlo = compass_accel_set[4];
	uint8_t zhi = compass_accel_set[5];

	raw_accel.x = (xlo | (int16_t)(xhi << 8)) >> 4;
	raw_accel.y = (ylo | (int16_t)(yhi << 8)) >> 4;
	raw_accel.z = (zlo | (int16_t)(zhi << 8)) >> 4;

	// char str[30];
	// sprintf(str, "compass: (%d, %d, %d)", raw_accel.x, raw_accel.y, raw_accel.z);
	// sprintln(str);
}

void compass_read_mag(){
	// Read the accelerometer
	uint8_t compass_mag_set[6];

	// Compass returns in 2s Complement
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_X_L_M, &compass_mag_set[0], 1);
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_X_H_M, &compass_mag_set[1], 1);
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_Y_L_M, &compass_mag_set[2], 1);
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_Y_H_M, &compass_mag_set[3], 1);
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_Z_L_M, &compass_mag_set[4], 1);
	compass_mag_reg_read8(LSM303_REGISTER_MAG_OUT_Z_H_M, &compass_mag_set[5], 1);

	uint8_t xlo = compass_mag_set[0];
	uint8_t xhi = compass_mag_set[1];
	uint8_t ylo = compass_mag_set[2];
	uint8_t yhi = compass_mag_set[3];
	uint8_t zlo = compass_mag_set[4];
	uint8_t zhi = compass_mag_set[5];

	raw_mag.x = (xlo | (int16_t)(xhi << 8));
	raw_mag.y = (ylo | (int16_t)(yhi << 8));
	raw_mag.z = (zlo | (int16_t)(zhi << 8));
}

void compass_set_mag_gain(mag_gain_t gain) {
	compass_mag_reg_write8(LSM303_REGISTER_MAG_CRB_REG_M, (uint8_t)gain);

	mag_gain = gain;

	switch (gain) {
		case LSM303_MAGGAIN_1_3:
      gauss_xy = 1100;
      gauss_z  = 980;
      break;
    case LSM303_MAGGAIN_1_9:
      gauss_xy = 855;
      gauss_z  = 760;
      break;
    case LSM303_MAGGAIN_2_5:
      gauss_xy = 670;
      gauss_z  = 600;
      break;
    case LSM303_MAGGAIN_4_0:
      gauss_xy = 450;
      gauss_z  = 400;
      break;
    case LSM303_MAGGAIN_4_7:
      gauss_xy = 400;
      gauss_z  = 355;
      break;
    case LSM303_MAGGAIN_5_6:
      gauss_xy = 330;
      gauss_z  = 295;
      break;
    case LSM303_MAGGAIN_8_1:
      gauss_xy = 230;
      gauss_z  = 205;
      break;
	}
}

coord_data_t compass_get_accel() {
	compass_read_accel();

	coord_data_t accel_data;

	accel_data.x = raw_accel.x / 30;
	accel_data.y = raw_accel.y / 30;
	accel_data.z = raw_accel.z / 30;

	// char str[30];
	// sprintf(str, "(%d, %d, %d)", accel_data.x, accel_data.y, accel_data.z);
	// sprintln(str);

	return accel_data;
}

coord_data_t compass_get_mag() {
	bool reading_valid = false;
	compass_set_mag_gain(LSM303_MAGGAIN_1_3);

	while (!reading_valid) {
		compass_read_mag();

		if ((raw_mag.x >= 2040) | (raw_mag.x <= -2040) |
        (raw_mag.y >= 2040) | (raw_mag.y <= -2040) |
        (raw_mag.z >= 2040) | (raw_mag.z <= -2040)) {
			switch (mag_gain) {
				case LSM303_MAGGAIN_5_6:
	        compass_set_mag_gain(LSM303_MAGGAIN_8_1);

					#ifdef LSM303_DEBUG
					sprintln("Changing range to +/- 8.1");
					#endif

	        break;
	      case LSM303_MAGGAIN_4_7:
	        compass_set_mag_gain(LSM303_MAGGAIN_5_6);

					#ifdef LSM303_DEBUG
					sprintln("Changing range to +/- 5.6");
					#endif

	        break;
	      case LSM303_MAGGAIN_4_0:
	        compass_set_mag_gain(LSM303_MAGGAIN_4_7);

					#ifdef LSM303_DEBUG
					sprintln("Changing range to +/- 4.7");
					#endif

	        break;
	      case LSM303_MAGGAIN_2_5:
	        compass_set_mag_gain(LSM303_MAGGAIN_4_0);

					#ifdef LSM303_DEBUG
					sprintln("Changing range to +/- 4.0");
					#endif

	        break;
	      case LSM303_MAGGAIN_1_9:
	        compass_set_mag_gain(LSM303_MAGGAIN_2_5);

					#ifdef LSM303_DEBUG
					sprintln("Changing range to +/- 2.5");
					#endif

	        break;
	      case LSM303_MAGGAIN_1_3:
	        compass_set_mag_gain(LSM303_MAGGAIN_1_9);

					#ifdef LSM303_DEBUG
	        sprintln("Changing range to +/- 1.9");
					#endif

	        break;
				default:
					break;
			}
		} else {
			// Readings are valid
			reading_valid = true;
		}
	}

	// 1 Gauss = 1 Microtesla
	coord_data_t mag_data;
	mag_data.x = raw_mag.x / gauss_xy * 30;
	mag_data.y = raw_mag.y / gauss_xy * 30;
	mag_data.z = raw_mag.z / gauss_z * 30;

	// char str[30];
  // sprintf(str, "good mag: (%d, %d, %d)", mag_data.x, mag_data.y, mag_data.z);
	// sprintln(str);

	return mag_data;
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
