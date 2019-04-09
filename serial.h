#ifndef _SERIAL_H_INCLUDED
#define _SERIAL_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define FOSC 7372800
#define BAUD 9600
#define UBRR (FOSC/16/BAUD-1) //Value for UBBR0

void serial_init(void);

char receive_rx(void);
void send_tx(char c);

void sprint(char* str);
void sprintln(char* str);

#endif
