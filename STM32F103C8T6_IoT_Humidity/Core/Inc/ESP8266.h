/*
 * ESP8266.h
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */

#ifndef INC_ESP8266_H_
#define INC_ESP8266_H_

#include "main.h"
#include "AHT15.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WiFi_Credentials	"AT+CWJAP=\"WiFiSSID\",\"WiFiPASSWORD\"\r\n"

extern UART_HandleTypeDef huart1;

uint32_t seconds;
uint8_t buffer[2000];
uint16_t buffer_index, timeout, messageHandlerFlag;

void ESP_RESET();
void ESP_Server_Init();
void ESP_Clear_Buffer();
uint8_t string_compare(char array1[], char array2[], uint16_t length);
int string_contains(char bufferArray[], char searchedString[], uint16_t length);
void messageHandler();
void sendData();

#endif /* INC_ESP8266_H_ */
