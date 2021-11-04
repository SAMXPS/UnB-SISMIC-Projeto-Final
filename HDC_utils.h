/**
 * Biblioteca para sensor de temperatura HDC1080.
 * */
#ifndef HDC_UTILS_H_
#define HDC_UTILS_H_
#include "I2C_utils.h"

/**
 * Fun��o que configura a comunica��o com o m�dulo HDC1080.
 */
void HDC_config();

/**
 * Fun��o que l� temperatura.
 */
int  HDC_readTemperature();

/**
 * Fun��o que l� humidade.
 */
int  HDC_readHumidity();

#endif /* HDC_UTILS_H_ */
