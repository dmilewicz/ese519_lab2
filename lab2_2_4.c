//
// Created by David Milewicz on 9/19/18.
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "notes.h"


volatile unsigned int oca = 0;
volatile unsigned int oca_i = 0;
uint8_t mode_toggle = 1;
int counter = 0;

ISR(ADC_vect) {
    if (counter % 30 == 0) {
        oca = range_convert(ADC);
        oca_i = list_index(ADC);
    }

    counter++;
    ADCSRA |= _BV(ADSC);
}

ISR(TIMER0_COMPA_vect) {
    if (mode_toggle) {
        OCR0A = oca;
    } else {
        OCR0A = note_list[oca_i];
    }
}

int c = 0;

ISR(TIMER1_CAPT_vect) {
    printf("hello%d\n", c++);
    mode_toggle ^= 1;
}

void main() {
    uart_init();

    sei();

    adc_init();
    timer_init();

    PORTB = _BV(PB0);
    listen_rise();

    while(1) {
    }
}