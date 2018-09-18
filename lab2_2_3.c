//
// Created by David Milewicz on 9/18/18.
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "ping.h"
#include "notes.h"

volatile unsigned int oca = 0;
int counter = 0;

ISR(ADC_vect) {
    if (counter % 30 == 0)
        OCR0A = range_convert(ADC);
    counter++;
//    OCR0A+=5;
//    printf("result %d\n\n", OCR0A);
    ADCSRA |= _BV(ADSC);
}

//ISR(TIMER0_COMPA_vect) {
//    OCR0A = oca;
//}

void adc_init() {
    ADMUX |= _BV(REFS0); // reference V
    ADCSRA |= _BV(ADPS0) | _BV(ADPS1) |_BV(ADPS2); // set prescaler
    ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADSC); // enables ADC
    ADCSRB = 0;
}

void timer_init() {
    DDRD = _BV(PD6); //Setting output pin

    TCCR0A = _BV(WGM02) | _BV(COM0A0); //Set CTC bits
    OCR0A = OCA_HIGH; //arbitrary initial value
    TCCR0B = (1 << CS01) | _BV(CS00); //Set prescaler to 64
//    TIMSK0 = (1 << OCIE0A); //Interrupt mask set bit
}

void main() {
    uart_init();

    sei();

    adc_init();
    timer_init();

    while(1)
        printf("oca: %d\n", OCR0A);



}


