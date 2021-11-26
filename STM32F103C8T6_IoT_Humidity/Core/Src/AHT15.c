/*
 * AHT15.c
 *
 *  Created on: Nov 17, 2021
 *      Author: vikto
 */

#include "AHT15.h"

float AHT15_relative_humidity = -100.0f, AHT15_temperature = -100.0f;
uint8_t measure_humidity = 0;

void AHT15_init()
{
	uint8_t init = AHT15_initialization;
	HAL_Delay(25);//needs 20 ms for i2c to stabilize after power up
	LED_ON;
	HAL_I2C_Master_Transmit(&hi2c1, AHT15_device_address | I2C_write, &init, 1, 50);
	LED_OFF;
}

void AHT15_reset()
{
	uint8_t reset = AHT15_soft_reset;
	LED_ON;
	HAL_I2C_Master_Transmit(&hi2c1, AHT15_device_address | I2C_write, &reset, 1, 50);
	LED_OFF;
	HAL_Delay(25);//takes some time to reset
}

void AHT15_read_data()
{
	uint8_t data[3] = { AHT15_measurement_trigger, 0b00110011, 0b00000000 };
	uint8_t received_data[6];
	LED_ON;
	HAL_I2C_Master_Transmit(&hi2c1, AHT15_device_address | I2C_write, &data[0], 3, 50);
	LED_OFF;
	uint8_t attempts = 5;
	do{
		attempts--;
		HAL_Delay(200);//measurement takes 75 ms
		LED_ON;
		HAL_I2C_Master_Receive(&hi2c1, AHT15_device_address | I2C_read, &received_data[0], 6, 50);
		LED_OFF;
	}while((received_data[5] & 0b10000000) == 128 && attempts > 0);


	if((received_data[5] & 0b10000000) == 0)
	{
		const uint32_t constant = 1048576;

		uint32_t humidity_bytes = (((uint32_t)received_data[4]) << 12) + (((uint32_t)received_data[3]) << 4) + (((uint32_t)received_data[2] & 0b11110000) >> 4);
		AHT15_relative_humidity = ((float)humidity_bytes / (float)constant) * 100.0f;

		uint32_t temperature_bytes = ((uint32_t)(received_data[2] & 0b00001111) << 16) + ((uint32_t)received_data[1] << 8) + (uint32_t)received_data[0];
		AHT15_temperature = (((float)temperature_bytes / (float)constant) * 200.0f) - 50.0f;
	}else{
		  AHT15_reset();
		  HAL_Delay(100);
		  AHT15_init();
		  HAL_Delay(100);
	}
}
