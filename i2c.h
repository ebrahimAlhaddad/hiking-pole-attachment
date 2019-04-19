#ifndef _I2C_H_INCLUDED
#define _I2C_H_INCLUDED

#define FOSC 7372800
#define BDIV (FOSC / 100000 - 16) / 2 + 1

#define I2C_SUCCESS       0x0
#define I2C_NAK_WR_ADDR   0x20 // NAK received after sending device address for writing
#define I2C_NAK_DATA      0x30 // NAK received after sending data
#define I2C_LOST          0x38 // Arbitration lost with address or data
#define I2C_NACK_RD_ADDR  0x48 // NAK received after sending device address for reading

void i2c_init();
uint8_t i2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an,
               uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn);

#endif
