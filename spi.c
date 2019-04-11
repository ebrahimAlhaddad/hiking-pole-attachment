#include "spi.h"
#include "serial.h"

#include <stdio.h>

uint8_t dummy;

void spi_init() {
  DDRB |= (1 << SCK_PIN);
  DDRB |= (1 << MISO_PIN);
  DDRB |= (1 << MOSI_PIN);
  DDRB |= (1 << SS_PIN);

  // SPI: Master Mode, MSB first, SCK leading rising edge
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_write(uint8_t data) {
  SPDR = data;

  while (!(SPSR & (1 << SPIF)));
  dummy = SPDR; // from the slave
}

uint8_t spi_read() {
  uint8_t recv_data;

  SPDR = 0xff; // irrelevant, can be anything
  while (!(SPSR & (1 << SPIF)));
  recv_data = SPDR;

  return recv_data;
}
