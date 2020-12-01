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

#define I2C_SCL		2
#define I2C_SDA		3
#define BAUD_RATE	100000
#define I2C_DELAY	100

#define _WM8731_LEFT_LINEIN         0X18     // MIC SETTINGS: ENABLE MUTE, ENABLE SIMULTANEOUS LOAD TO LEFT AND RIGHT CHANNELS
#define _WM8731_RIGHT_LINEIN        0X10     // MIC SETTINGS: ENABLE MUTE, ENABLE SIMULTANEOUS LOAD TO LEFT AND RIGHT CHANNELS
#define _WM8731_LEFT_HP             0XF0     // HEADPHONE SETTINGS : -9DB OUTPUT
#define _WM8731_RIGHT_HP            0XF0     // HEADPHONE SETTINGS : -9DB OUTPUT
#define _WM8731_ANALOGAUDIO         0X11       // DAC SELECT
#define _WM8731_DIGITALAUDIO        0X00
#define _WM8731_POWER               0X00       // DISABLE POWER DOWN
#define _WM8731_DAIF                0X5A       // ENABLE MASTER MODE, 24BIT DATA I2S
#define _WM8731_SAMPLING            0X18       // 32000HZ,12.288MHz oscillator.
#define _WM8731_ACTIVATE            0X01       // MODULE IS ON
#define _WM8731_DEACTIVATE          0X00       // MODULE IS OFF
#define _WM8731_RESET               0X00       // RESET VALUE

freertos_i2c_flag_t config_codec(void);

void codec_audio_play(void);

void codec_sampling(void);
void codec_rx(uint8_t * buffer, uint32_t size);


#endif /* WM87313_H_ */
