//
// Created by David Milewicz on 9/13/18.
//

#ifndef LAB2_TIMER_H
#define LAB2_TIMER_H

#include avr/io.h

#DEFINE 5MS_OCR   77
#DEFINE OUT       PB1

void ping_5ms() {
    TCCR1A = _BV(WGM02) | _BV(COM0A0); //Set CTC bits
    0CR1A = 5MS_OCR;
    TCCR0B = _BV(CS01);

    TCNT1 = 0;




}






#endif //LAB2_TIMER_H
