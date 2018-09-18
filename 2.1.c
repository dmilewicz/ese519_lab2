#include <avr/io.h>


int main(void)
{
    DDRD = _BV(PD6); //Setting output pin

    TCCR0A = _BV(WGM02) | _BV(COM0A0); //Set CTC bits
    OCR0A = 70; //Comparator value
    TCCR0B = (1 << CS01) | _BV(CS00); //Set prescaler to 256

    while(1){}
}


