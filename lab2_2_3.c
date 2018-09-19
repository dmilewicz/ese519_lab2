//
// Created by David Milewicz on 9/18/18.
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "notes.h"

volatile unsigned int oca = 0;
int counter = 0;

ISR(ADC_vect) {
    if (counter % 30 == 0)
        oca = range_convert(ADC);
    counter++;
    ADCSRA |= _BV(ADSC);
}

ISR(TIMER0_COMPA_vect) {
    OCR0A = oca;
}


void main() {
    uart_init();

    sei();

    adc_init();
    timer_init();

    while(1) {
    }


}


