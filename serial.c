#include "serial.h"

void serial_init() {
  UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable TX & RX
  UCSR0B |= (1 << RXCIE0); //Enable Interrupts for RX (Reciever)
  UCSR0C |= (1 << UCSZ00 | 1 << UCSZ01); //Async, No Parity Bit, 1 stop bit, 8 data bits
  UBRR0 = UBRR;
}

char receive_rx(){
  // Wait for receive complete flag to go high
  while (!( UCSR0A & (1 << RXC0 )));
  return UDR0;
}

void send_tx(char c){
  // Wait for transmitter data register empty
  while ((UCSR0A & (1 << UDRE0)) == 0);
  UDR0 = c;
}

void sprint(char* str) {
  unsigned char i;
  for (i = 0; i < (unsigned) strlen(str); ++i) {
    send_tx(str[i]);
  }
}

void sprintln(char* str) {
  sprint(str);
  send_tx('\n');
}
