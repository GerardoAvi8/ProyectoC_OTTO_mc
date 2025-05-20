/*
 * PWM.c
 *
 * Created: 18/05/2025 23:10:40
 *  Author: gerav
 */ 

#include "PWM.h"

uint8_t duty_cycle0 = 35;
uint16_t duty_cycle1 = 375;

void init_pwm0(uint8_t invertido, uint16_t prescaler){
	DDRD |= (1 << DDD6) | (1 << DDD5);				//PD6 Y PD5
	TCCR0A = 0;
	TCCR0B = 0;			//Limpieza de bits
	
	//---------------------------------------------------------//
	if(invertido == invert){
		TCCR0A |= (1 << COM0A0) | (1 << COM0A1);
		TCCR0A |= (1 << COM0B0) | (1 << COM0B1);
	}
	else{
		TCCR0A |= (1 << COM0A1);
		TCCR0A |= (1 << COM0B1);
	}
	//---------------------------------------------------------//
	TCCR0A |= (1 << WGM01) | (1 << WGM00);			//Seleccionamos FAST MODE
	//--------------------------------------------------------//
	switch(prescaler){
		case 1:
		TCCR0B |= (1 << CS00);
		break;
		
		case 8:
		TCCR0B |= (1 << CS01);
		break;
		
		case 64:
		TCCR0B |= (1 << CS01) | (1 << CS00);
		break;
		
		case 256:
		TCCR0B |= (1 << CS02);
		break;
		
		case 1024:
		TCCR0B |= (1 << CS02) | (1 << CS00);
		break;
		
		default:
		TCCR0B |= (1 << CS00);
		break;
	}
	
	//--------------------------------------------------------//
	DutyCycle0(duty_cycle0);					//Llamamos funcion de cargar duty cycle
}

void init_pwm1(uint8_t invertido, uint16_t prescaler){
	DDRB |= (1 << PORTB1) | (1 << PORTB2);				//PD6 Y PD5
	
	TCCR1A = 0;
	TCCR1B = 0;			//Limpieza de bits
	
	//---------------------------------------------------------//
	if(invertido == invert){
		TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
		TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
	}
	else{
		TCCR1A |= (1 << COM1A1);
		TCCR1A |= (1 << COM1B1);
	}
	//---------------------------------------------------------//
	TCCR1A |= (1 << WGM11);							//Seleccionamos FAST MODE
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	//--------------------------------------------------------//
	switch(prescaler){
		case 1:
		TCCR1B |= (1 << CS10);
		break;
		
		case 8:
		TCCR1B |= (1 << CS11);
		break;
		
		case 64:
		TCCR1B |= (1 << CS11) | (1 << CS10);
		break;
		
		case 256:
		TCCR1B |= (1 << CS12);
		break;
		
		case 1024:
		TCCR1B |= (1 << CS12) | (1 << CS10);
		break;
		
		default:
		TCCR1B |= (1 << CS10);
		break;
	}
	
	ICR1 = 4999;
	//--------------------------------------------------------//
	DutyCycle1(duty_cycle1);					//Llamamos funcion de cargar duty cycle
}

void DutyCycle0(uint8_t DT0){
	OCR0A = DT0;
	OCR0B = DT0;
}
void DutyCycle1(uint16_t DT1){
	OCR1A = DT1;
	OCR1B = DT1;
}