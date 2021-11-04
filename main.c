/**
 * Nome: Samuel James de Lima Barroso
 * Matrícula: 19/0019948
 */
#include <msp430.h> 
#include "I2C_utils.h"
#include "LCD_utils.h"
#include "ADC_utils.h"
#include "LED_utils.h"

unsigned int count = 0;
unsigned int buttom_pressed = 0;
unsigned int hist_x[10];
unsigned int hist_y[10];
unsigned int averageX[5];
unsigned int averageY[5];

void modo_0();
void modo_1();
void modo_2();

#define TO_VOLTAGE_4(x)     (x * 0.8056640625)  // 1000 * 3.3 / 4096
#define TO_VOLTAGE_3(x)     (x * 0.08056640625)  // 100 * 3.3 / 4096

// average, min, max, of 8 values
unsigned int average8(unsigned int* vector) {
    unsigned int i, average;
    average = 0;
    for (i = 0; i < 8; i++) {
        average += vector[i];
    }
    average = average >> 3;                     // Divisão por 8.
    return average;
}

void min_max10(unsigned int* vector) {
    unsigned int i, min, max;
    min = -1;
    max = 0;
    for (i = 0; i < 8; i++) {
        if (vector[i] > max) max = vector[i];
        if (vector[i] < min) min = vector[i];
    }
    vector[10] = min;
    vector[11] = max;
}

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    ADC_config();
    I2C_config(0, 1, 1, 9600);
    LCD_config();
    LCD_customchars();
    LED_config();

    P6DIR &= ~BIT2;  // Botão SW como input
    P6REN |= BIT2;   // Resistor
    P6OUT |= BIT2;   // Pull UP

    __enable_interrupt();

    ADC_ready = 0;
    ADC_start();

    byte modo = 0;

    while (1) {
        if (!buttom_pressed && !(P6IN & BIT2)) {
            buttom_pressed = 1;
            modo++;
            modo = modo%3;
        }
        if (ADC_ready) {
            ADC_ready = 0;
            buttom_pressed = 0;

            averageX[0] = average8(ADC_vx);
            averageX[1] = TO_VOLTAGE_4(averageX[0]);
            averageX[2] = TO_VOLTAGE_3(averageX[0]);
            averageY[0] = average8(ADC_vy);
            averageY[1] = TO_VOLTAGE_4(averageY[0]);
            averageY[2] = TO_VOLTAGE_3(averageY[0]);

            if (count == 0) {
                for (;count<10;count++){
                    hist_x[count] = averageX[2];
                    hist_y[count] = averageY[2];
                }
            } else {
                hist_x[count%8] = averageX[2];
                hist_y[count%8] = averageY[2];
            }

            min_max10(hist_x);
            min_max10(hist_y);
            averageX[3] = hist_x[10];
            averageX[4] = hist_x[11];
            averageY[3] = hist_y[10];
            averageY[4] = hist_y[11];

            if (modo < 2) {
                modo_01(modo);
            } else {
                modo_2();
            }
            LED1_TOGGLE;
            count++;
        }
    }
}


void toDec(unsigned int value, byte decimal_places, char* buffer) {
    while(decimal_places){
        buffer[--decimal_places] = value%10 + '0';
        value/=10;
    }
}

void modo_01(byte modo) {
    static char modo_01[] = "AX=d,dDdV   NNNN\nMn=d,dd  Mx=d,dd";
    unsigned int* data = modo ? averageY : averageX;

    /*
     * 0 1 2 3 4 5 6 7 8 9 A B C D E F
       A X = d , d D d V       N N N N
       M n = d , d d     M x = d , d d
     * */

    modo_01[1] = modo + '0';
    toDec(data[0], 4, modo_01 + 0x0C);
    toDec(data[1], 4, modo_01 + 0x04);
    toDec(data[3], 3, modo_01 + 0x04 + 17);
    toDec(data[4], 3, modo_01 + 0x0D + 17);
    modo_01[3] = modo_01[4];
    modo_01[4] = ',';
    modo_01[3 + 17] = modo_01[4 + 17];
    modo_01[4 + 17] = ',';
    modo_01[0xC + 17] = modo_01[0xD + 17];
    modo_01[0xD + 17] = ',';

    LCD_print(modo_01);
}

void modo_2() {
    LCD_clear();
    byte x = averageX[0] >> 8;
    byte y = averageY[0] >> 8;
    LCD_goto((y > 7 ? 0x40 : 0x00) + x);
    LCD_txchar((y%8)+8);
}
