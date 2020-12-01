/*
 * freertos_I2C.h
 *
 *  Created on: Oct 15, 2020
 *      Author: eduar
 */

#ifndef FILTER_H_
#define FILTER_H
#include "arm_math.h"

#define NUM_TAPS 51
#define BLOCK_SIZE 1042
#define TEST_LENGTH_SAMPLES 4*1024
typedef enum {LOW_PASS, HIGH_PASS, BAND_PASS} filter_t;

void init_LP();
void init_HP();
void init_BP();
void init_buff(uint32_t *buffer);
void apply_filter(uint8_t select);


#endif /* FREERTOS_I2C_H_ */
