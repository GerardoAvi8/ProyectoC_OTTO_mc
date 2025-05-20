/*
 * EEPROMUART.h
 *
 * Created: 19/05/2025 0:32:04
 *  Author: gerav
 */ 

#include <avr/io.h>

#define EEPROM_SIZE 4			//Limite de localidades
#define MIN 145					//OCR TMR1
#define MAX 600
extern volatile uint8_t eeprom_address;
extern volatile uint16_t eeprom_val[4];
extern volatile uint8_t servo_selected;


#ifndef EEPROMUART_H_
#define EEPROMUART_H_


/*------------------UART--------------------------*/
void initUART();
void writeString(const char* str);
void updatePosition(uint8_t ascii_val);
/*------------------------------------------------*/

/*-------------------EEPROM-----------------------*/
void writeEEPROM(uint8_t addr, uint16_t value);
uint16_t readEEPROM(uint8_t addr);
void changeADDR();
void positionEEPROM();
void loadEEPROM();
void EEPROMleds();
/*------------------------------------------------*/


#endif /* EEPROMUART_H_ */