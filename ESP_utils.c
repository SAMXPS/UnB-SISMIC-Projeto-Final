#include <msp430.h>
#include <string.h>
#include "ESP_utils.h"
#include "DELAY_utils.h"
#include "WIFI_config.h"

void ESP_restart();
void ESP_send_command(char* command);

void  UART_TX_config();
int   UART_TX(byte* data, int len);
int   UART_RX(byte* buff, int len);

#define UART_RX_BUFF_SIZE 64
#define UART_TX_BUFF_SIZE 64
byte  UART_RX_BUFF[UART_RX_BUFF_SIZE] = {0};
byte  UART_RX_POS     = 0;
byte  UART_RX_FULL    = 0;

byte  UART_TX_BUFF[UART_TX_BUFF_SIZE] = {0};
byte  UART_TX_POS = 0;
byte  UART_TX_LEN = 0;
byte  UART_TX_READY    = 1;

void ESP_restart() {
    UART_TX_config();
    delay_ms(200);
    ESP_send_command("AT+RST");
    delay_s(5);
    ESP_send_command("AT+CWJAP_CUR=\"" WIFI_SSID "\",\"" WIFI_PASSWORD "\"");
    delay_s(10);
}

void ESP_send_data(char* data) {
    ESP_send_command("AT+CIPCLOSE");
    delay_s(1);
    ESP_send_command("AT+CIPSTART=\"TCP\",\"" WIFI_SERVER "\",7767");
    delay_s(1);

    static char command[] = "AT+CIPSEND=##";
    toDec(strlen(data), 2, command+11);
    ESP_send_command(command);
    ESP_send_command(data);

    delay_s(1);
    ESP_send_command("AT+CIPCLOSE");
    delay_s(1);
}

void ESP_send_command(char* command) {
    while (!UART_TX(command, strlen(command)));
    while (!UART_TX("\r\n", 2));
    delay_ms(10);
}

void toDec(unsigned int value, byte decimal_places, char* buffer) {
    while(decimal_places){
        buffer[--decimal_places] = value%10 + '0';
        value/=10;
    }
}

// P3.3 TX
// P3.4 RX
void UART_TX_config() {
    P3DIR |=  BIT3; // Output
    P3SEL |=  BIT3; // Peripheral
    P3REN &= ~BIT3; // Internal Resistor

    P3DIR &= ~BIT4; // Input
    P3SEL |=  BIT4; // Peripheral

    //Desliga o módulo
    UCA0CTL1 |= UCSWRST;

    /**
     * Baud Rate 115200 bps
     * LSB First
     * Sem paridade
     * 8 bits de dados
     * 1 stop bits
     */

    UCA0CTL0 = //UCPEN |    //Parity enable: 1=ON, 0=OFF
               //UCPAR |    //Parity: 0:ODD, 1:EVEN
               //UCMSB |    //LSB First: 0, MSB First:1
               //UC7BIT |   //8bit Data: 0, 7bit Data:1
               //UCSPB |    //StopBit: 0:1 Stop Bit, 1: 2 Stop Bits
               UCMODE_0 | //USCI Mode: 00:UART, 01:Idle-LineMultiprocessor, 10:AddressLine Multiprocessor, 11: UART with automatic baud rate
               //UCSYNC    //0:Assynchronous Mode, 1:Synchronous Mode
               0;

    UCA0CTL1 = UCSSEL__SMCLK | //00:UCAxCLK, 01:ACLK, 10:SMCLK, 11:SMCLK
               //UCRXEIE     | //Erroneous Character IE
               //UCBRKIE     | //Break Character IE
               //UCDORM      | //0:NotDormant, 1:Dormant
               //UCTXADDR    | //Next frame: 0:data, 1:address
               //UCTXBRK     | //TransmitBreak
               UCSWRST;        //Mantém reset.

    //BaudRate: 115200
    //BRCLK  = 1 MHz
    //UCBRx  = 9
    //UCBRSx = 0
    //UCBRFx = 0
    //UCOS16 = 0

    //BaudRate: 9600
    //BRCLK  = 1 MHz
    //UCBRx  = 104
    //UCBRSx = 1
    //UCBRFx = 0
    //UCOS16 = 0

    UCA0MCTL = //UCBRF_0 |    // Modulation Stage. Ignored when UCOS16 = 0
               UCBRS_0 |      // UCBRS = 1
               //UCOS16 |     // Oversampling Mode. 0:disabled, 1:enabled.
               0;

    UCA0BR0 = (9 & 0x00FF); // Low byte
    UCA0BR1 = (9 >> 8);     // High byte

    //Liga o módulo
    UCA0CTL1 &= ~UCSWRST;

    UCA0IE =   UCTXIE | // Interrupt on transmission
               UCRXIE | // Interrupt on Reception
               0;
}


int UART_TX(byte* data, int len) {
    int i;
    if (UART_TX_READY && len <= UART_TX_BUFF_SIZE) {
        for (i = 0; i < len; i++) {
            UART_TX_BUFF[i] = data[i];
        }

        UART_TX_READY = 0;
        UART_TX_POS = 0;
        UART_TX_LEN = len;
        UCA0TXBUF = UART_TX_BUFF[0];
        // Transmission started
        return 1;
    } else {
        // TX is busy
        return 0;
    }
}

int UART_RX(byte* buff, int len) {
    int i;
    if (UART_RX_POS >= len) {
        UART_RX_POS = 0;
        for (i = 0; i < len; i++) {
            buff[i] = UART_RX_BUFF[i];
        }
        return 1;
    }
    // Não tem bytes suficientes
    return 0;
}

/**
 * Interrupções
 **/

#pragma vector = USCI_A0_VECTOR
__interrupt void UART_TX_INTERRUPT(void) {
    switch(_even_in_range(UCA0IV,4)) {
        case 0: break;
        case 2:  // Reception Interrupt
            if (!UART_RX_FULL) {
                UART_RX_BUFF[UART_RX_POS++] = UCA0RXBUF;
            }
            if (UART_RX_POS >= UART_RX_BUFF_SIZE) {
                UART_RX_FULL = 1;
            }
            break;
        case 4:  // Transmission Interrupt
            UART_TX_POS++;
            if (UART_TX_POS >= UART_TX_LEN) {
                UART_TX_READY = 1;
            } else {
                UCA0TXBUF = UART_TX_BUFF[UART_TX_POS];
            }
            break;
        default: break;
    }
    __low_power_mode_off_on_exit();
}


