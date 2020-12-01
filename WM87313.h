/*
 * WM8731.h
 *
 *  Created on: Nov 13, 2020
 *      Author: eduar
 */

#ifndef WM87313_H_
#define WM87313_H_

#include "freertos_I2C.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "fsl_sai.h"

#define WM8731_ADDRESS 0x1A

#define LEFT_LINE_IN_REG     	0x00
#define RIGHT_LINE_IN_REG	 	0x01
#define LEFT_HEADPHONE_OUT_REG  0x02
#define RIGHT_HEADPHONE_OUT_REG 0x03
#define ANALOGUE_AUDIO_PATH_REG 0x04
#define DIGITAL_AUDIO_PATH_REG	0x05
#define POWER_DOWN_CTRL_REG		0x0C
#define DIG_AUDIO_INTERFACE_REG 0x07
#define SAMPLING_CTRL_REG		0x08
#define RESET_REG				0x0F
#define ENABLE_CODEC			0x12
#define ACTIVE_CTRL_REG	        0x09       // Activar Control


#define I2C_SCL		2
#define I2C_SDA		3
#define BAUD_RATE	100000
#define I2C_DELAY	100

#define CODEC_LEFT_LINEIN         0X18     // MIC SETTINGS: ENABLE MUTE, ENABLE SIMULTANEOUS LOAD TO LEFT AND RIGHT CHANNELS
#define CODEC_RIGHT_LINEIN        0X10     // MIC SETTINGS: ENABLE MUTE, ENABLE SIMULTANEOUS LOAD TO LEFT AND RIGHT CHANNELS
#define CODEC_LEFT_HP             0XF0     // HEADPHONE SETTINGS : -9DB OUTPUT
#define CODEC_RIGHT_HP            0XF0     // HEADPHONE SETTINGS : -9DB OUTPUT
#define CODEC_ANALOGAUDIO         0X11       // DAC SELECT
#define CODEC_DIGITALAUDIO        0X00
#define CODEC_POWER               0X00       // DISABLE POWER DOWN
#define CODEC_DAIF                0X5A       // ENABLE MASTER MODE, 24BIT DATA I2S
#define CODEC_SAMPLING            0X18       // 32000HZ,12.288MHz oscillator.
#define CODEC_ACTIVATE            0X01       // MODULE IS ON
#define CODEC_DEACTIVATE          0X00       // MODULE IS OFF
#define CODEC_RESET               0X00       // RESET VALUE

freertos_i2c_flag_t config_codec(void);

void codec_audio_play(void);

void codec_sampling(void);
void codec_i2s_config(void);
void codec_rx(uint8_t * buffer, uint32_t size);
void activate_codec(void);


#endif /* WM87313_H_ */
