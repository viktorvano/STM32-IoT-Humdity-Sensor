/*
 * AHT15.h
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */

#ifndef INC_AHT15_H_
#define INC_AHT15_H_

#include "main.h"

#define AHT15_device_address		(uint8_t)0b01110000
#define AHT15_initialization		(uint8_t)0b11100001
#define AHT15_measurement_trigger	(uint8_t)0b10101100
#define AHT15_soft_reset			(uint8_t)0b10111010
#define I2C_read					(uint8_t)0b00000001
#define I2C_write					(uint8_t)0b00000000

#define LED_ON						HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
#define LED_OFF						HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)

extern I2C_HandleTypeDef hi2c1;

extern float AHT15_relative_humidity, AHT15_temperature;
extern uint8_t measure_humidity;

extern void AHT15_init();
extern void AHT15_reset();
extern void AHT15_read_data();

#endif /* INC_AHT15_H_ */
