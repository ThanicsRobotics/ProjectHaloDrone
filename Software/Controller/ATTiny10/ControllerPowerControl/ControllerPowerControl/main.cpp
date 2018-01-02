/*
 * ControllerPowerControl.cpp
 *
 * Created: 1/1/2018 7:29:31 PM
 * Author : Bill 2012
 */ 

#include <avr/io.h>
//#include <time.h>
#include <util/delay.h>
#define F_CPU 12000000UL

int main(void)
{
	
	DDRB |= 0x02;
	//DDRB &= ~(1<<DDB2);
	PORTB |= (1<<PORTB1);
	PORTB &= ~(1<<PORTB2);
	
	while(1) {
		//if ((PINB & 0x04) == 0) {
		//PORTB == 0x0E
		//if (PINB & 0x04) {
		//	//_delay_ms(10000000000);
		//PORTB &= 0xFD; //11111101
		//	break;
		//}
		//_delay_ms(1000000000);
		
		if (PINB & (1<<PINB2)) {
			//PORTB &= 0xFD; //11111101
			_delay_ms(10000000000);
			PORTB &= ~(1<<PORTB1);
			
		}
		//else {
		//	PORTB |= (1<<PORTB1);
		//}
	}
	
	return 0;
}