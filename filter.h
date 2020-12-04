/*
<<<<<<< HEAD
 * filter.h
=======
 * freertos_I2C.h
>>>>>>> fb30df9368ec3ad9b243d372b8d96817c565bfdb
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
#define NUM_BLOCKS TEST_LENGTH_SAMPLES/BLOCK_SIZE
typedef enum {LOW_PASS, HIGH_PASS, BAND_PASS,DISABLE} filters_t;


void init_LP();
void init_HP();
void init_BP();
void init_buff(uint32_t *buffer);
void apply_filter(uint8_t select);


#endif
