/**
 * Nome: Samuel James de Lima Barroso
 * Matrícula: 19/0019948
 */
#include <msp430.h> 
#include "I2C_utils.h"
#include "LCD_utils.h"
#include "LED_utils.h"

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    I2C_config(0, 1, 1, 9600);
    //LCD_config();
    LED_config();

    //__enable_interrupt();

}
