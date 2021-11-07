/**
 * Biblioteca para sensor de temperatura HDC1080.
 * */
#ifndef HDC_UTILS_H_
#define HDC_UTILS_H_
#include "I2C_utils.h"

/**
 * Função que configura a comunicação com o módulo HDC1080.
 */
void HDC_config();

/**
 * Função que lê temperatura e humidade.
 * Retorna 0 em caso de erro de comunicação.
 */
int  HDC_read(word* temperature, word* humidity);

#endif /* HDC_UTILS_H_ */
