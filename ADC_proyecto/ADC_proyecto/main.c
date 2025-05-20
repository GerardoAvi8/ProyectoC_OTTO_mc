/*
 * ADC_proyecto.c
 *
 * Created: 28/04/2025 21:26:27
 * Author : gerav
 */ 

//Encabezado
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ADClib/ADC.h"
#include "PWMlib/PWM.h"
#include "UART_EEPROMlib/EEPROMUART.h"

#define MANUAL 1				//Estados a usar
#define UART 2
#define EEPROM 3

volatile uint8_t servo_selected = 1;
volatile uint8_t eeprom_address = 0;
volatile uint16_t eeprom_val[4] = {375, 375, 25, 25};

volatile uint8_t adc_channel = 0;
volatile uint8_t servo_pos2 = 25;
volatile uint8_t servo_pos3 = 25;
volatile uint16_t servo_pos0 = 375;
volatile uint16_t servo_pos1 = 375;
volatile uint8_t estado_actual = MANUAL;

/***************************************************/
//	Prototipo de funciones

/*--------------Configuraciones-------------------*/
void setup();
/*------------------------------------------------*/

/*----------------Control-------------------------*/
void change();
void LEDS();
/*------------------------------------------------*/


/**************************************************/
//	Loop Principal
int main(void)
{
	setup();
	
	while (1)							//Ciclo para verificar el estado de botones
	{
		if(!(PIND & (1 << PORTD4))){
			_delay_ms(50);
			if(!(PIND & (1 << PORTD4))){
				while(!(PIND & (1 << PORTD4))){}
				change();					//Cambio de estados
			}	
		}
		if(estado_actual == EEPROM){
			if(!(PIND & (1 << PORTD3))){
				_delay_ms(50);
				if(!(PIND & (1 << PORTD3))){
					while(!(PIND & (1 << PORTD3))){}
					changeADDR();			//Cambio de direccion EEPROM
				}
			}
			
			if(!(PIND & (1 << PORTD2))){
				_delay_ms(50);
				if(!(PIND & (1 << PORTD2))){
					while(!(PIND & (1 << PORTD2))){}
					positionEEPROM();		//Guardar posicion
				}
			}
			
			if(!(PIND & (1 << PORTD7))){
				_delay_ms(50);
				if(!(PIND & (1 << PORTD7))){
					while(!(PIND & (1 << PORTD7))){}
					loadEEPROM();			//Mostrar posicion
				}
			}
		}
		_delay_ms(10);
	}
}

/**************************************************/
//	Funciones (no imterrupciones)
void setup(){
	cli();
	
	CLKPR = (1 << CLKPCE);				//Reloj de 16MHz
	
	//Entradas
	DDRD &= ~((1 << PORTD7) | (1 << PORTD4) | (1 << PORTD3) | (1 << PORTD2));
	PORTD |= (1 << PORTD7) | (1 << PORTD4) | (1 << PORTD3) | (1 << PORTD2);
	
	//Salidas
	DDRC |= (1 << PORTC4) | (1 << PORTC5);
	PORTC &= ~((1 << PORTC4) | (1 << PORTC5));
	DDRB |= (1 << PORTB4) | (1 << PORTB5);
	PORTB &= ~((1 << PORTB4) | (1 << PORTB5));
	
	init_adc();
	init_pwm0(non_invert, 1024);
	init_pwm1(non_invert, 64);
	initUART();
	
	LEDS();						//Control leds RGB
	
	sei();
	
}

void change(){
	if(estado_actual == MANUAL){
		estado_actual = UART;
		ADCSRA &= ~(1 << ADEN);
		writeString("\nModo UART activado\r\n");
		writeString("Seleccione servo (1-4): ");
	}
	else if(estado_actual == UART){
		estado_actual = EEPROM;
		ADCSRA |= (1 << ADEN);
		ADCSRA |= (1 << ADSC);
		writeString("\nModo EEPROM activado\r\n");
		writeString("Direccion actual: 0\r\n");
		EEPROMleds();
	}
	else {
		estado_actual = MANUAL;
		ADCSRA |= (1 << ADEN);
		ADCSRA |= (1 << ADSC);
		
		PORTB &= ~((1 << PORTB4) | (1 << PORTB5));
	}
	LEDS();
}

void LEDS(){
	switch(estado_actual){
		case MANUAL:
			PORTC |= (1 << PORTC4);
			PORTC &= ~(1 << PORTC5);
			break;
		case UART:
			PORTC |= (1 << PORTC5);
			PORTC &= ~(1 << PORTC4);
			break;
		case EEPROM:
			PORTC |= (1 << PORTC4) | (1 << PORTC5);
			break;	
	}
}

/**************************************************/
//	Funciones de interrupcion
ISR(ADC_vect){
	
	if(estado_actual == MANUAL){
		if(adc_channel == 0 || adc_channel == 1){
			ADMUX &= ~(1 << ADLAR);
			uint16_t adc_value1 = ADC;
			if(adc_channel == 0){
				servo_pos0 = ((uint32_t)adc_value1 * (MAX - MIN)) / 1023 + MIN;
				OCR1A = servo_pos0;
		
				ADMUX = (ADMUX & 0xF0) | 0b00000001;
			}else{
				servo_pos1 = ((uint32_t)adc_value1 * (MAX - MIN)) / 1023 + MIN;
				OCR1B = servo_pos1;

				ADMUX = (ADMUX & 0xF0) | 0b00000010;
				ADMUX |= (1 << ADLAR);
			}
			adc_channel++;
		}else{
			uint8_t adc_value0 = ADCH;
			if(adc_channel == 2){
				servo_pos2 = (adc_value0 * (35 - 6)) / 255 + 6;
				OCR0A = servo_pos2;
			
				ADMUX = (ADMUX & 0xF0) | 0b00000011;
			}else{
				servo_pos3 = (adc_value0 * (35 - 6) / 255 + 6);
				OCR0B = servo_pos3;
			
				ADMUX = (ADMUX & 0xF0) | 0b00000000;
				ADMUX &= ~(1 << ADLAR);
			}
			adc_channel = (adc_channel + 1) % 4;
		}
		ADCSRA |= (1 << ADSC);
	}
}

ISR(USART_RX_vect) {
	uint8_t status = UCSR0A;
	(void)status;
	
	uint8_t received = UDR0;
	
	if(estado_actual == UART) {
		if(received >= '1' && received <= '4') {
			servo_selected = received - '0';
			writeString("\r\nServo seleccionado: ");
			UDR0 = received;
			writeString("\r\nIngrese valor ASCII: ");
		}
		else {
			updatePosition(received);
		}
	}
}
