//
// Created by David Milewicz on 9/13/18.
//

#define ECHO

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "notes.h"
#include "ping.h"

#define PING_OCR     80
#define OUT          PB1
#define IN           PB0
#define BYTE         8
#define TIMER_MAX    65535 // 2^16 -1
#define TICKS_PER_US 16

volatile uint32_t oca = 0;
volatile uint32_t oca_i = 0;
uint8_t mode_toggle = 1;

// *******************************************************

// count the overflows
ISR(TIMER1_OVF_vect)
{
    overflow_counter++;
}

ISR(TIMER1_CAPT_vect)
{ 
    // check if found rising edge
    if (TCCR1B & _BV(ICES1)) {
        start_count = load_int16(ICR1H, ICR1L); // load 16 bit integer
        init_count_overflows();
        listen_fall();
    }
    // check if found falling edge
    else {
        end_count = load_int16(ICR1H, ICR1L);
        uint32_t ticks = calculate_ticks(start_count, end_count, overflow_counter);
        uint32_t time = ticks_to_time_us(ticks);
        close_listen();
//	    printf("ping width: %lu us\n\n", time);

        oca = range_convert(time);
        oca_i = list_index(time);
    	count++;
        ping_5ms();
    }
}

ISR(TIMER1_COMPA_vect)
{
    close_ping();
    listen_rise();
}

ISR(TIMER0_COMPA_vect) {
    if (mode_toggle) {
        OCR0A = oca;
    } else {
        OCR0A = note_list[oca_i];
    }
}

// *************************************


void main() {
    uart_init();
    timer_init();

    DDRB |= _BV(OUT);
    DDRB &= _CV(IN);

    // enable button pull-up
    PORTB |= _BV(PB5);

    sei();

    ping_5ms();

    int change = 1;

    while (1) {
        if (!(PINB & _BV(PB5)) && change) {
            mode_toggle ^= 1;
            change = 0;
        } else {
            change = 1;
        }
    }
}