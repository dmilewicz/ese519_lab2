//
// Created by David Milewicz on 9/19/18.
//

#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"


#define DAC_OUT     (_BV(PB2) | _BV(PB3) | _BV(PB4))

void dac_init() {
    DDRB |= _BV(PB2) | _BV(PB3) | _BV(PB4);
}


ISR(ADC_vect) {
    printf("bucket %d\n", (adc_bucket(ADC)));
    PORTB = (PORTB & ~DAC_OUT) | (adc_bucket(ADC) << PB2);
    ADCSRA |= _BV(ADSC);
}

int main() {
    uart_init();
    sei();
    adc_init();
    dac_init();

    while(1);
}






