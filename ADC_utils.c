#include <msp430.h>
#include "ADC_utils.h"

extern unsigned int ADC_vx[8];
extern unsigned int ADC_vy[8];
extern unsigned int ADC_ready;
void ADC_config_timer();

void ADC_config() {
    P6SEL |= BIT0;                              // Configurando o P6.0 para o pino A0 do ADC.
    P6SEL |= BIT1;                              // Configurando o P6.1 para o pino A1 do ADC.
    ADC12CTL0 &= ~ADC12ENC;                     // Desliga o módulo.

    ADC12CTL0 = ADC12SHT0_3 |                   // Usando 32 ciclos para o tsample.
                ADC12ON;                        // Liga o ADC.

    ADC12CTL1 = ADC12CSTARTADD_0 |              // Start address: 0
                ADC12SHS_1 |                    // Conversão via TimerA0.1
                ADC12SHP |                      // Sample and Hold Pulse mode: input
                ADC12DIV_0 |                    // Divide o clock por 1
                ADC12SSEL_0 |                   // Escolhe o clock MODCLK: 4.8 MHz
                ADC12CONSEQ_3;                  // Repeated Autoscan Mode

    ADC12CTL2 = ADC12TCOFF |                    // Desliga o sensor de temperatura.
                ADC12RES_2;                     // 12 bits resolution.
    // ADC12SREF_0 : Vcc/Vss = 3.3V/0V
    // ADC12INCH_0 : Amostra o pino A0
    // ADC12INCH_1 : Amostra o pino A1
    // ADC12EOS    : END OF SEQUENCE
    ADC12MCTL0  = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL1  = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL2  = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL3  = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL4  = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL5  = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL6  = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL7  = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL8  = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL9  = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL10 = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL11 = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL12 = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL13 = ADC12SREF_0 | ADC12INCH_1;
    ADC12MCTL14 = ADC12SREF_0 | ADC12INCH_0;
    ADC12MCTL15 = ADC12SREF_0 | ADC12INCH_1|
                  ADC12EOS;                     // End of Sequence
    ADC12IE = ADC12IE15;                        // Liga a interrupção do canal 15.
    ADC12CTL0 |= ADC12ENC;                      // Liga o ADC.

    ADC_config_timer();
}

void ADC_config_timer() {
    TA0CTL = TASSEL__ACLK |                     // Usa o ACLK: 32768
             MC__STOP;                          // Timer parado

    TA0CCTL1 = OUTMOD_2;

    TA0CCR0 = 2048;                             // 16 Hz
    TA0CCR1 = 1024;
}

void ADC_start() {
    TA0CTL |= (MC__UP | TACLR);                 // Start timer.
}

/*************************************************
 * INTERRUPT FUNCTIONS
 *************************************************/

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_interrupt(void) {
    switch (_even_in_range(ADC12IV, 0x24)) {
    case ADC12IV_NONE:
        break;
    case ADC12IV_ADC12OVIFG:                    // MEMx overflow
        break;
    case ADC12IV_ADC12TOVIFG:                   // Conversion Time overflow
        break;
    case ADC12IV_ADC12IFG0:                     // MEM0 Ready
        break;
    case ADC12IV_ADC12IFG1:                     // MEM1 Ready
        break;
    case ADC12IV_ADC12IFG2:                     // MEM2 Ready
        break;
    case ADC12IV_ADC12IFG3:                     // MEM3 Ready
        break;
    case ADC12IV_ADC12IFG4:                     // MEM4 Ready
        break;
    case ADC12IV_ADC12IFG5:                     // MEM5 Ready
        break;
    case ADC12IV_ADC12IFG6:                     // MEM6 Ready
        break;
    case ADC12IV_ADC12IFG7:                     // MEM7 Ready
        break;
    case ADC12IV_ADC12IFG8:                     // MEM8 Ready
        break;
    case ADC12IV_ADC12IFG9:                     // MEM9 Ready
        break;
    case ADC12IV_ADC12IFG10:                    // MEM10 Ready
        break;
    case ADC12IV_ADC12IFG11:                    // MEM11 Ready
        break;
    case ADC12IV_ADC12IFG12:                    // MEM12 Ready
        break;
    case ADC12IV_ADC12IFG13:                    // MEM13 Ready
        break;
    case ADC12IV_ADC12IFG14:                    // MEM14 Ready
        break;
    case ADC12IV_ADC12IFG15:                    // MEM15 Ready
        ADC_vx[0] = ADC12MEM0;
        ADC_vx[1] = ADC12MEM2;
        ADC_vx[2] = ADC12MEM4;
        ADC_vx[3] = ADC12MEM6;
        ADC_vx[4] = ADC12MEM8;
        ADC_vx[5] = ADC12MEM10;
        ADC_vx[6] = ADC12MEM12;
        ADC_vx[7] = ADC12MEM14;
        ADC_vy[0] = ADC12MEM1;
        ADC_vy[1] = ADC12MEM3;
        ADC_vy[2] = ADC12MEM5;
        ADC_vy[3] = ADC12MEM7;
        ADC_vy[4] = ADC12MEM9;
        ADC_vy[5] = ADC12MEM11;
        ADC_vy[6] = ADC12MEM13;
        ADC_vy[7] = ADC12MEM15;
        ADC_ready = 1;
        break;
    }
}
