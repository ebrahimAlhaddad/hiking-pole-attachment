#ifndef _GPS_H_INCLUDED
#define _GPS_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define FOSC 7372800
#define BAUD 9600
#define UBRR (FOSC/16/BAUD-1) //Value for UBBR0


void output_string(char*);

void output_number(int);

void get_data(void);

#endif
