/*
 * EEPROMUART.c
 *
 * Created: 19/05/2025 0:32:19
 *  Author: gerav
 */ 


#include "EEPROMUART.h"


/*-------------------------------UART------------------------------*/
void initUART(){
	DDRD |= (1 << DDD1);
	DDRD &= ~(1 << DDD0);
	
	UCSR0A = 0;
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	UBRR0 = 103;						//BAUD 9600
}

void writeString(const char* str){
	while(*str != '\0'){
		while(!(UCSR0A & (1 << UDRE0))); // Esperar hasta que el buffer esté vacío
		UDR0 = *str++;
	}
}

void updatePosition(uint8_t ascii_val){
	switch(servo_selected){
		case 1: OCR1A = ((uint32_t)ascii_val * (MAX - MIN)) / 255 + MIN; break;
		case 2: OCR1B = ((uint32_t)ascii_val * (MAX - MIN)) / 255 + MIN; break;
		case 3: OCR0A = ((uint32_t)ascii_val * (35 - 6)) / 255 + 6; break;
		case 4: OCR0B = ((uint32_t)ascii_val * (35 - 6)) / 255 + 6; break;
	}
	
	writeString("\r\nServo ");
	UDR0 = '0' + servo_selected;
	writeString(" -> Caracter ingresado: ");
	
	if(ascii_val >= 100) UDR0 = '0' + (ascii_val/100)%10;
	if(ascii_val >= 10) UDR0 = '0' + (ascii_val/10)%10;
	UDR0 = '0' + ascii_val%10;
}
/*-----------------------------------------------------------------------*/

/*-----------------------------EEPROM------------------------------------*/
void writeEEPROM(uint8_t addr, uint16_t value) {
	while(EECR & (1 << EEPE));						//Espera de cualquier escritura previa
	
	//Byte bajo
	EEAR = addr * 2;						//Direccion para byte bajo
	EEDR = (uint8_t)value;					//8 bits - significativos
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
	
	while(EECR & (1 << EEPE));
	
	//Byte alto
	EEAR = addr * 2 + 1;
	EEDR = (uint8_t)(value >> 8);
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}

uint16_t readEEPROM(uint8_t addr) {
	while(EECR & (1 << EEPE));
	
	//Byte bajo
	EEAR = addr * 2;
	EECR |= (1 << EERE);
	uint8_t low = EEDR;
	
	while(EECR & (1 << EEPE));
	
	//Byte alto
	EEAR = addr * 2 + 1;
	EECR |= (1 << EERE);
	uint8_t high = EEDR;
	
	return((uint16_t)high << 8) | low;
}

void changeADDR(){
	eeprom_address = (eeprom_address + 1) % EEPROM_SIZE;			//Ciclo 0-3
	writeString("Direccion EEPROM cambiada");
	writeString("\r\n");
	EEPROMleds();										//Permutacion leds
}

//Cada bloque usa 8 bytes
void positionEEPROM() {
	uint8_t base_addr = eeprom_address * 8;					//Direccion base
	
	writeEEPROM(base_addr + 0, OCR1A);					//servo1
	writeEEPROM(base_addr + 2, OCR1B);					//servo2
	writeEEPROM(base_addr + 4, (uint16_t)OCR0A);		//servo3
	writeEEPROM(base_addr + 6, (uint16_t)OCR0B);		//servo4
	
	writeString("\r\nGuardado en direccion");
	writeString("\r\n");
}

void loadEEPROM() {
	uint8_t base_addr = eeprom_address * 8;				//Direccion base
	
	OCR1A = readEEPROM(base_addr + 0);					//servo1
	OCR1B = readEEPROM(base_addr + 2);					//servo2
	OCR0A = (uint8_t)readEEPROM(base_addr + 4);			//servo3
	OCR0B = (uint8_t)readEEPROM(base_addr + 6);			//servo4
	
	writeString("\r\nValor mostrado");
	writeString("\r\n");
}

void EEPROMleds(){
	PORTB &= ~((1 << PORTB4) | (1 << PORTB5));
	if(eeprom_address & 0x01) PORTB |= (1 << PORTB4);
	if(eeprom_address & 0x02) PORTB |= (1 << PORTB5);
}
/*---------------------------------------------------------------------------*/
