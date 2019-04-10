#include "spi.h"
#include "serial.h"

#include <stdio.h>

uint8_t dummy;

void spi_init() {
  DDRB |= SPI_PORTB;

  SPCR = 0x53; // SPI: Master Mode, MSB first, SCK leading rising edge
  SPSR = 0x00;
}

void spi_write(uint8_t data) {
  // char buffer[60];
  // sprintf(buffer, "SPI 1Write: %d", data);
  // sprintln(buffer);
  SPDR = data;

  while (!(SPSR & (1 << SPIF)));
  dummy = SPDR; // from the slave

  // sprintln("SPI WRITE DONE");
}

uint8_t spi_read() {
  uint8_t recv_data;

  SPDR = 0xff; // irrelevant, can be anything
  while (!(SPSR & (1 << SPIF)));
  recv_data = SPDR;

  return recv_data;
}
