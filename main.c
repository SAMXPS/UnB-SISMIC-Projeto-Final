/**
 * Nome: Samuel James de Lima Barroso
 * Matr�cula: 19/0019948
 */
#include <msp430.h> 
#include "I2C_utils.h"
#include "LCD_utils.h"
#include "LED_utils.h"
#include "HDC_utils.h"
#include "ESP_utils.h"
#include "DELAY_utils.h"

void displayData(int tempC10, int humidity100);
long int temp_c10;
long int hum100;

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    I2C_config(0, 1, 1, 9600);
    LCD_config();
    LED_config();

    HDC_config();
    word temperature;
    word humidity;

    __enable_interrupt();
    while (1) {
        if (HDC_read(&temperature, &humidity)) {
            temp_c10 = (((long int) temperature) * 1650)/(((long int) 1) << 16) - 400;
            hum100 = (((long int) humidity) * 100)/(((long int) 1) << 16);
            if (temp_c10 > 300) {
                LED2_ON;
            } else {
                LED2_OFF;
            }
            displayData(temp_c10, hum100);
            LED1_ON;
        } else {
            LED1_OFF;
            displayData(0, 0);
        }

        ESP_restart();
        ESP_send_data("Hello World");

        // Tempo extra, por que n�o?
        delay_s(20);
    }

}

void displayData(int tempC10, int humidity100) {
    static char modo_01[] = "T=XX.X�C\nH=XX%";

    /*
     * 0 1 2 3 4 5 6 7 8 9 A B C D E F
       A X = d , d D d V       N N N N
       M n = d , d d     M x = d , d d
     * */

    toDec(tempC10, 3, modo_01 + 2);
    toDec(humidity100, 2, modo_01 + 11);
    modo_01[5] = modo_01[4];
    modo_01[4] = '.';

    LCD_print(modo_01);
}

