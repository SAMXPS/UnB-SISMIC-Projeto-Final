#include <msp430.h>
#include "DELAY_utils.h"

/*
 * Delay microsseconds.
 */
void delay_us(unsigned int time_us) {
    //Configure timer A0 and starts it.
    TA0CCR0 = time_us;
    TA0CTL = TASSEL__SMCLK | ID__1 | MC_1 | TACLR;

    //Locks, waiting for the timer.
    while((TA0CTL & TAIFG) == 0);

    //Stops the timer
    TA0CTL = MC_0 | TACLR;
}

void delay_ms(unsigned int time_ms) {
    int i;
    for (i=0;i<time_ms;i++) {
        delay_us(1000);
    }
}

void delay_s(unsigned int time_s) {
    int i;
    for (i=0;i<time_s;i++) {
        delay_ms(1000);
    }
}
