#ifndef ESP_UTILS_H_
#define ESP_UTILS_H_
#include "I2C_utils.h" // typedef byte

void ESP_restart();
void ESP_send_data(char* data);
void ESP_send_command(char* command);
void toDec(unsigned int value, byte decimal_places, char* buffer);

#endif /* ESP_UTILS_H_ */
