#include <avr/io.h>
#include "util/delay.h"


#define BUTTON1 0x01
#define LEDON 0x20

int main(void)
{
    DDRB = LEDON;
    PORTB = BUTTON1;
    
    while(1){
        if (!(PINB & BUTTON1)){
            PORTB |= LEDON;
        }
        else{
          PORTB = 0xDF;
        }
    }
}




