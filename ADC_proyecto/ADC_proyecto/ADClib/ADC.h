/*
 * ADC.h
 *
 * Created: 19/05/2025 0:47:22
 *  Author: gerav
 */ 

#include <avr/io.h>

extern volatile uint8_t adc_channel;
extern volatile uint8_t servo_pos2;
extern volatile uint8_t servo_pos3;
extern volatile uint16_t servo_pos0;
extern volatile uint16_t servo_pos1;


#ifndef ADC_H_
#define ADC_H_


void init_adc();



#endif /* ADC_H_ */