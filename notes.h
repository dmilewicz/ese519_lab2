//
// Created by David Milewicz on 9/18/18.
//

#ifndef LAB2_NOTES_H
#define LAB2_NOTES_H

#include <avr/io.h>

#define C6_OCR   238
#define D6_OCR   212
#define E6_OCR   189
#define F6_OCR   178
#define G6_OCR   158
#define A6_OCR   141
#define B6_OCR   126
#define C7_OCR   118


#define OCA_LOW    C6_OCR
#define OCA_HIGH   C7_OCR


#if defined(ECHO)

#define INPUT_HIGH       4000
#define INPUT_LOW        100

#elif defined(ADC)

#define INPUT_HIGH       1024
#define INPUT_LOW        0

#endif


#define RATIO       (float)(OCA_HIGH - OCA_LOW) / (float)(INPUT_HIGH - INPUT_LOW)

uint8_t note_list[] = {C6_OCR, D6_OCR, E6_OCR, F6_OCR, G6_OCR, A6_OCR, B6_OCR, C7_OCR};

uint8_t range_convert(uint32_t input) {
    return (RATIO * input) + OCA_LOW;
}


int list_index(uint32_t input) {
    if (input > INPUT_HIGH) {
        return OCA_HIGH;
    }
    else if (input < INPUT_LOW) {
        return OCA_LOW;
    }

    // get list length
    uint8_t list_len = sizeof(note_list) / sizeof(note_list[0]);

    return (int) ((input - INPUT_LOW)/ ((float)(INPUT_HIGH - INPUT_LOW) / list_len));
}

void timer_init() {
    DDRD = _BV(PD6); //Setting output pin

    TCCR0A = _BV(WGM01) | _BV(COM0A0); //Set CTC bits
    OCR0A = OCA_LOW; //arbitrary initial value
    TCCR0B = _BV(CS01) | _BV(CS00); //Set prescaler to 64
    TIMSK0 = (1 << OCIE0A); //Interrupt mask set bit
}

#endif //LAB2_NOTES_H
