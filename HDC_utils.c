#include <msp430.h>
#include "HDC_utils.h"
#include "DELAY_utils.h"

#define HDC_I2C_PORT        0x00
#define HDC_I2C_ADDRESS     0x40 // Address         I2C address of the HDC1080 sensor.
#define HDC_REG_TEMPERATURE 0x00 // Temperature     Temperature measurement output.
#define HDC_REG_HUMITY      0x01 // Humidity        Relative Humidity measurement output.
#define HDC_REG_CONF        0x02 // Configuration   HDC1080 configuration and status.
#define HDC_REG_SID0        0xFB // Serial ID       First 2 bytes of the serial ID of the part.
#define HDC_REG_SID1        0xFC // Serial ID       Mid 2 bytes of the serial ID of the part.
#define HDC_REG_SID2        0xFD // Serial ID       Last byte bit of the serial ID of the part.
#define HDC_REG_MID         0xFE // Manufacturer ID 0x5449 ID of Texas Instruments.
#define HDC_REG_DID         0xFF // Device ID       0x1050 ID of the device.

void HDC_config() {
    // TODO: custom configuration ?
    int conf = 0;
    //1. Configure the acquisition parameters in register address 0x02:
    //(a) Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1.
    //(b) Set the desired temperature measurement resolution:
    //– Set Bit[10] to 0 for 14 bit resolution.
    //– Set Bit[10] to 1 for 11 bit resolution.
    //(c) Set the desired humidity measurement resolution:
    //– Set Bit[9:8] to 00 for 14 bit resolution.
    //– Set Bit[9:8] to 01 for 11 bit resolution.
    //– Set Bit[9:8] to 10 for 8 bit resolution.

    if (I2C_txbyte(HDC_I2C_PORT, HDC_I2C_ADDRESS, HDC_REG_CONF)){
        // ERROR
    }
}

int  HDC_read(word* temperature, word* humidity) {
    // Escrevendo endereço do registrador de temperatura no Pointer Register
    if (I2C_txbyte(HDC_I2C_PORT, HDC_I2C_ADDRESS, HDC_REG_TEMPERATURE)){
        return 0;
    }

    // Agora devemos esperar o tempo de medição:
    // 8  bit resolution 2.500 ms = 2500 us
    // 11 bit resolution 3.850 ms = 3850 us
    // 14 bit resolution 6.500 ms = 6500 us
    delay_us(6500);
    // Esperar leitura de humidade
    delay_us(6500);
    // Tempo extra, por que não?
    delay_us(6500);
    if (I2C_rxword(HDC_I2C_PORT, HDC_I2C_ADDRESS, temperature) == -1) {
        return 0;
    }

    // Escrevendo endereço do registrador de humidade no Pointer Register
    if (I2C_txbyte(HDC_I2C_PORT, HDC_I2C_ADDRESS, HDC_REG_HUMITY)){
        return 0;
    }

    if (I2C_rxword(HDC_I2C_PORT, HDC_I2C_ADDRESS, humidity) == -1) {
        return 0;
    }

    return 1;
}
