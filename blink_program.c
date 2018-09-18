#include <avr/io.h>
#include "util/delay.h"

int main(void)
{
	DDRB = 0x20;
	
	while(1) //loop forever
	{
		PORTB = PINB << 5;		
		//PORTB = (PINB && 0x01) ^ 0xFF;

		//PORTB &= 0xDF;
	}
}
