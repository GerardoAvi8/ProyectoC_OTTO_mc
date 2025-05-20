/*
 * ADC.c
 *
 * Created: 19/05/2025 0:47:34
 *  Author: gerav
 */ 

#include "ADC.h"


void init_adc(){
	ADMUX = 0;
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));				//Entradas ADC
	ADMUX |= (1 << REFS0);				//5V
	ADMUX |= (1 << ADLAR);				//Izquierda
	
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);			//Frecuencia adecuada
	ADCSRA |= (1 << ADIE);				//Interrupcion
	ADCSRA |= (1 << ADEN);				//Habilitamos ADC
	
	ADCSRA |= (1 << ADSC);				//Comienza
}
