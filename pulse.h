#ifndef _H_PULSE_INCLUDED
#define _H_PULSE_INCLUDED

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ADC_PIN1			1
#define ADC_THRESH	370
//bpm


uint16_t adc_read(uint8_t adcx);
void pulse_init(void);
uint16_t calc_BPM(void);
#endif