//
// Created by David Milewicz on 9/20/18.
//

#define ECHO

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "notes.h"
#include "adc.h"
#include "ping.h"

#define PING_OCR     80
#define OUT          PB1
#define IN           PB0
#define BYTE         8
#define TIMER_MAX    65535 // 2^16 -1
#define TICKS_PER_US 16

#define DAC_OUT     (_BV(PB2) | _BV(PB3) | _BV(PB4))
//#define DAC_OUT

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
//        printf("ping width: %lu us\n\n", time);

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

// DAC Code
void dac_init() {
    DDRB |= _BV(PB2) | _BV(PB3) | _BV(PB4);
}


ISR(ADC_vect) {
//    printf("bucket %d\n", (adc_bucket(ADC)));
//    PORTB = (PORTB & ~DAC_OUT) | (adc_bucket(ADC) << PB2);
//    printf("out: %d\n", (PORTB & DAC_OUT) >> PB2);

    ADCSRA |= _BV(ADSC);
}

// *************************************


void main() {
    DDRB |= _BV(OUT);
    DDRB &= _CV(IN);
    DDRB |= _BV(PB2) | _BV(PB3) | _BV(PB4);


    uart_init();
    timer_init();
    adc_init();
    dac_init();
    PORTB |= _BV(OUT); // set output high

    // enable button pull-up
//    PORTB |= _BV(PB5);

    sei();

    ping_5ms();

    int change = 1;

    while (1) {
//        printf("adc: %d\n", adc_bucket(ADC));
        PORTB &= ~DAC_OUT;
//        PORTB = 0;
//        PORTB |= _BV(PB5);

//        PORTB = (adc_bucket(ADC) << PB2);
//
        switch (adc_bucket(ADC)) {
            case 0:
                PORTB &= ~DAC_OUT;
                break;
            case 1:
                PORTB &= ~DAC_OUT;
                PORTB |= _BV(PB2);
                PORTB &= ~(1<<PB3 | 1<<PB4);
                break;
            case 2:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB3);
                PORTB &= ~(1<<PB2 | 1<<PB4);
                break;
            case 3:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB2) | (1<<PB3);
                PORTB &= ~(1<<PB4);
                break;
            case 4:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB4);
                PORTB &= ~(1<<PB3 | 1<<PB2);
                break;
            case 5:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB4) | (1<<PB2);
                PORTB &= ~(1<<PB3);
                break;
            case 6:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB4) | (1<<PB3);
                PORTB &= ~(1<<PB2);
                break;
            case 7:
                PORTB &= ~DAC_OUT;
                PORTB |= (1<<PB2) |(1<<PB3)| (1<<PB4);
                break;
        }

        printf("out: %d\n", (PORTB & DAC_OUT) >> PB2);

        if (!(PINB & _BV(PB5)) && change) {
            mode_toggle ^= 1;
            change = 0;
        } else {
            change = 1;
        }
    }
}