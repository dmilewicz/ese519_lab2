#include <avr/io.h> 
#include <avr/interrupt.h>

int main() {
	DDRD = _BV(PD6);
	TIMSK0 = _BV(TOIE0); // set overflow bit register
	sei();
	TCCR0B = (1 << CS02) | (1 << CS00); // prescaler
	PORTD ^= _BV(PD6);

	while (1) {}
}
