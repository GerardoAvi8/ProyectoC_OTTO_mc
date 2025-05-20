/*
 * PWM.h
 *
 * Created: 18/05/2025 23:09:43
 *  Author: gerav
 */ 

#include <avr/io.h>


#define invert 1				//Config PWM
#define non_invert 0


#ifndef PWM_H_
#define PWM_H_


/*------------------PWM---------------------------*/
void init_pwm0(uint8_t invertido, uint16_t prescaler);
void init_pwm1(uint8_t invertido, uint16_t prescaler);
/*------------------------------------------------*/

/*----------------DUTYCycle-----------------------*/
void DutyCycle0(uint8_t duty_cycle0);
void DutyCycle1(uint16_t duty_cycle1);
/*------------------------------------------------*/


#endif /* PWM_H_ */