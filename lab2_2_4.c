//
// Created by David Milewicz on 9/18/18.
//

#include "ping.h"

#define C6_OCR   238
#define D6_OCR   212
#define E6_OCR   189
#define F6_OCR   178
#define G6_OCR   158
#define A6_OCR   141
#define B6_OCR   126
#define C7_OCR   118







void main() {

	//BG Comment
    // ccccc
    // modified line 23

    uart_init();

    DDRB |= _BV(OUT);
    DDRB &= _CV(IN);
    sei();

    ping_5ms();

    while (1);
}