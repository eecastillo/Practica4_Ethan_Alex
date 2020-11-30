/*
 * WM8731.c
 *
 *  Created on: Nov 13, 2020
 *      Author: eduar
 */

#include <WM87313.h>
static freertos_i2c_config_t codec_i2c_config;

freertos_i2c_flag_t config_codec(void)
{
	freertos_i2c_flag_t codec_sucess = freertos_i2c_fail;
	codec_i2c_config.baudrate = BAUD_RATE;
	codec_i2c_config.i2c_number = 	freertos_i2c_0;
	codec_i2c_config.port = freertos_i2c_portB;
	codec_i2c_config.scl_pin = I2C_SCL;
	codec_i2c_config.sda_pin = I2C_SDA;
	codec_i2c_config.pin_mux = kPORT_MuxAlt2;

	codec_sucess = freertos_i2c_init(codec_i2c_config);
	vTaskDelay(pdMS_TO_TICKS(10));

	if(freertos_i2c_sucess == codec_sucess)
	{
		codec_sucess = freertos_i2c_fail;

		uint8_t data[2] = {POWER_DOWN_CTRL_REG, 0x00};

		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = LEFT_LINE_IN_REG;
		data[1] = 23;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = RIGHT_LINE_IN_REG;
		data[1] = 23;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));


		data[0] = ANALOGUE_AUDIO_PATH_REG;
		data[1] = 0x05;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = ENABLE_CODEC;
		data[1] = 0x01;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));
	}
	return codec_sucess;
}

