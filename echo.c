//
// Created by David Milewicz on 9/13/18.
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define PING_OCR     80
#define OUT          PB1
#define IN           PB0
#define BYTE         8
#define TIMER_MAX    65535 // 2^16 -1
#define TICKS_PER_US 16


#define _CV(bit) ~_BV(bit)


int overflow_counter;
int32_t start_count;
int32_t end_count;

uint16_t load_int16(int high, int low) {
    return ((high << BYTE) | low);
}

void ping_5us() {
    DDRB = _BV(OUT);
    TCCR1A |= _BV(COM1A1); //Normal mode, clear OC1A on output compare
    TIMSK1 |= _BV(OCIE1A); // set the interrupt flag
    OCR1A = PING_OCR; // set counter
    TCNT1 = 0; // reset counter
    PORTB |= _BV(OUT); // set output high (will be cleared automatically after 5us)
    TCCR1B = _BV(CS10); // start clock without prescaler
}

/*
 * End 5us initialization ping to the sensor
 */
void close_ping()
{
    TCCR1A &= _CV(COM1A1); // remove action
    TIMSK1 &= _CV(OCIE1A); // remove interrupt flag
//    TCCR1B = 0x00; // close clock
}

void listen_rise()
{
    TIMSK1 |= _BV(ICIE1); // activate input capture interrut
    TCCR1B |= _BV(ICES1) | _BV(CS10); // start clock without prescaler, Input capture
}

void listen_fall()
{
    TIMSK1 |= _BV(ICIE1); // activate input capture register (falling edge)
    TCCR1B |= _BV(CS10); // start clock without prescaler
    TCCR1B &= _CV(ICES1); // set input capture to falling edge
}

void close_listen() {
    TIMSk1 &= _CV(ICIE1);
}

// Count the number
void init_count_overflows() {
    overflow_counter = 0; // reset counter
    TIMSK1 |= _BV(TOIE1); // enable overflow interrupt
}

int32_t calculate_ticks(int32_t start, int32_t end, int overflows) {
    if (overflows == 0) {
        return end - start;
    }
    else {
        int32_t time_sum = 0;
        time_sum += TIMER_MAX - start; // add the
        time_sum += ((overflows - 1) * TIMER_MAX);
        time_sum += end;
        return time_sum;
    }
}

int32_t ticks_to_time_us(int32_t tick_count) {
    return tick_count / TICKS_PER_US;
}


// *******************************************************

// count the overflows
ISR(TIMER1_OVF_vect)
{
    overflow_counter++;
}

ISR(TIMER1_CAPT_vect)
{
    // check if found rising edge
    if (TCCR1B && _BV(ICES1)) {
        start_count = load_int16(ICR1H, ICR1L); // load 16 bit integer
        init_count_overflows();
        listen_fall();
    }
    // check if found falling edge
    else {
        end_count = load_int16(ICR1H, ICR1L);
        int32_t ticks = calculate_ticks(start_count, end_count, overflow_counter);
        int32_t time = ticks_to_time_us(ticks);
        close_listen();
        ping_5us();
        printf("ping width: %d us\n", time);
    }
}

ISR(TIMER1_COMPA_vect)
{
    close_ping();
    listen_rise();
}

// *************************************


void main() {
//    // init
    uart_init();
    DDRB |= _BV(OUT);
    DDRB &= _CV(IN);
    sei();

    // run
    ping_5us();
}