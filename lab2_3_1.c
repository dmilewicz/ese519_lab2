//
// Created by David Milewicz on 9/19/18.
//

#include <avr/interrupt.h>
#include "adc.h"
#include "uart.h"



ISR(ADC_vect) {
    printf("ADC: %d\n", ADC);
    ADCSRA |= _BV(ADSC);
}


int main() {
    uart_init();
    sei();
    adc_init();

    while(1);
}




