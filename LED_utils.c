#include <LED_utils.h>
#include <msp430.h>

/**
 * Configuração de pinos para leds.
 * */
void LED_config() {
    P1DIR |=  BIT0;  // Enable left led output
    P4DIR |=  BIT7;  // Enable right led output
}
