#ifndef _GPS_H_INCLUDED
#define _GPS_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

void output_string(char*);
//void output_number(int);
void gps_get_data(void);

#endif
