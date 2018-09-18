#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define F_CPU 16000000L
#define BUZZER 0x40;

int main(void)
{
    DDRD = 0x40; //Setting output pin

    TCCR0A = (1 << COM0A0); //Normal timer mode
    OCR0A = 70; //Comparator value to attain 440 Hz with 256 prescaler
    TIMSK0 = (1 << OCIE0A); //Interrupt mask set bit

    sei();

    TCCR0B = (1 << CS02); //Set prescaler to 256

    while(1){}
}


ISR(TIMER0_COMPA_vect){
    OCR0A = (TCNT0 + 70); //Determines new OCR0A to maintain 440 Hz=
  }


//Compare match interrupt enabled
