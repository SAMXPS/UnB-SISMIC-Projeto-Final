#ifndef LEDS_H
#define LEDS_H

void LED_config();

//Led 1 (P1.0) Vermelho
#define LED1_NAME       "LED VERMELHO, ESQUERDA"
#define LED1_ON         (P1OUT |=  BIT0)
#define LED1_OFF        (P1OUT &= ~BIT0)
#define LED1_TOGGLE     (P1OUT ^=  BIT0)

//Led 2 (P4.7) Verde
#define LED2_NAME       "LED VERDE, DIREITA"
#define LED2_ON         (P4OUT |=  BIT7)
#define LED2_OFF        (P4OUT &= ~BIT7)
#define LED2_TOGGLE     (P4OUT ^=  BIT7)

#endif
