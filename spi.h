#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <avr/io.h>

#define SCK_PIN   PB5
#define MISO_PIN  PB4
#define MOSI_PIN  PB3
#define SS_PIN    PB2

void spi_init(void);

void spi_write(uint8_t data);
uint8_t spi_read(void);

#endif
