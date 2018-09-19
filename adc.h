//
// Created by David Milewicz on 9/19/18.
//

#ifndef LAB2_ADC_H
#define LAB2_ADC_H

#include <avr/io.h>

#define NUM_BUCKETS  8

#define ADC_MIN 0
#define ADC_MAX 1024

void adc_init() {
    ADMUX |= _BV(REFS0); // reference V
    ADCSRA |= _BV(ADPS0) | _BV(ADPS1) |_BV(ADPS2); // set prescaler
    ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADSC); // enables ADC
    ADCSRB = 0;
}

uint8_t adc_bucket(unsigned int input) {
    return (uint8_t) ((input - ADC_MIN)/ ((float)(ADC_MAX - ADC_MIN) / NUM_BUCKETS));
}

#endif //LAB2_ADC_H
