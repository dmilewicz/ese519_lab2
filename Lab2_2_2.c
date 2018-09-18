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


uint32_t overflow_counter;
uint32_t start_count = 0;
uint32_t end_count = 0;

uint16_t load_int16(unsigned int high, unsigned int low) {
    return ((high << BYTE) | low);
}

void ping_5ms() {
    DDRB = _BV(OUT);
    TCCR1A |= _BV(COM1A1); //Normal mode, clear OC1A on output compare
    TIMSK1 |= _BV(OCIE1A); // set the interrupt flag
    OCR1A = PING_OCR; // set counter
    TCNT1 = 0; // reset counter
    PORTB |= _BV(OUT); // set output high
    TCCR1B = _BV(CS10); // start clock without prescaler
}

/*
 * End 5us initialization ping to the sensor
 */
void close_ping()
{
    //PORTB &= _CV(OUT);

    TCCR1A &= _CV(COM1A1); // remove action
    TIMSK1 &= _CV(OCIE1A); // revove interrupt flag
    TCCR1B = 0x00; // close clock
}

void listen_rise()
{
    TIMSK1 |= _BV(ICIE1); // activate input capture interrut
    TCCR1B |= _BV(ICES1) | _BV(CS10); // start clock without prescaler, Input capture
}

void listen_fall()
{
    //TIMSK1 |= _BV(ICIE1); // activate input capture register (falling edge)
    //TCCR1B |= _BV(CS10); // start clock without prescaler
    TCCR1B &= _CV(ICES1); // set input capture to falling edge
}

void close_listen() {
    TIMSK1 &= _CV(ICIE1);
}

// Count the number
void init_count_overflows() {
    overflow_counter = 0; // reset counter
    TIMSK1 |= _BV(TOIE1); // enable overflow interrupt
}
int count = 0;

uint32_t calculate_ticks(uint32_t start, uint32_t end, uint32_t overflows) {
    if (overflows == 0) {
//    	if (start > end) {
//        if (count %20 == 0)printf("ERROR %lu, %lu\n", start, end);
//        }
        return end - start;
    }
    else {
        uint32_t time_sum = 0;
        time_sum += (TIMER_MAX - start); // add the
//        if (count %20 == 0)
//        printf("tsum: %lu overflows: %lu\n", time_sum, overflows);
        time_sum += ((overflows - 1) * TIMER_MAX);
//        if (count %20 == 0)printf("timecounter %lu\n", time_sum);
        time_sum += end;
        return time_sum;

    }
}

uint32_t ticks_to_time_us(uint32_t tick_count) {
    return tick_count / TICKS_PER_US;
}


// *******************************************************

// count the overflows
ISR(TIMER1_OVF_vect)
{
    overflow_counter++;
    //printf("OVERFLOW %d!\n", overflow_counter);
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
//        if (count % 20 ==0) {
//            printf("end: %lu\n", end_count);}
        uint32_t ticks = calculate_ticks(start_count, end_count, overflow_counter);
        uint32_t time = ticks_to_time_us(ticks);
        close_listen();
//        if (count % 20 ==0) {
	        // printf("Ticks: %lu\n", ticks);

	        printf("ping width: %lu us\n\n", time);
//    	}
    	count++;
        ping_5ms();
    }
}

ISR(TIMER1_COMPA_vect)
{
    close_ping();
    listen_rise();
}

// *************************************


void main() {
    uart_init();

    DDRB |= _BV(OUT);
    DDRB &= _CV(IN);
    sei();

    ping_5ms();

    while (1);
}