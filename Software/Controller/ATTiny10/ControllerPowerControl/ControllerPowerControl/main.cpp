/*
 * ControllerPowerControl.cpp
 *
 * Created: 1/1/2018 7:29:31 PM
 * Author : Bill 2012
 */ 
#include "usi_i2c_slave.h"
#include "usi_i2c_master.h"
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 12000000UL

int main(void)
{
	DDRB |= 0x02;
	PORTB |= (1<<PORTB1);
	PORTB &= ~(1<<PORTB2);
	
	while(1) {
		if (PINB & (1<<PINB2)) {
			_delay_ms(10000000000);
			PORTB &= ~(1<<PORTB1);
			
		}
	}
	
	return 0;
}