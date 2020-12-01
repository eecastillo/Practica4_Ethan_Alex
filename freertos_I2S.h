/*
 * freertos_I2S.h
 *
 *  Created on: Nov 29, 2020
 *      Author: eduar
 */

#ifndef FREERTOS_I2S_H_
#define FREERTOS_I2S_H_

#include "fsl_sai_edma.h"
#include "fsl_sai.h"

#define DEMO_SAI              I2S0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_CLKSRC       kCLOCK_CoreSysClk
#define DEMO_SAI_CLK_FREQ     CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_SAI_IRQ          I2S0_Tx_IRQn
#define DEMO_SAITxIRQHandler  I2S0_Tx_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MCLK_OUTPUT  true
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_I2C_CLKSRC kCLOCK_BusClk
#define DEMO_I2C_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

//DMA
#define DEMO_DMA             DMA0
#define DEMO_EDMA_CHANNEL    (0)
#define DEMO_DMAMUX          DMAMUX
#define DEMO_TX_EDMA_CHANNEL (0U)
#define DEMO_RX_EDMA_CHANNEL (1U)
#define DEMO_SAI_TX_SOURCE   kDmaRequestMux0I2S0Tx
#define DEMO_SAI_RX_SOURCE   kDmaRequestMux0I2S0Rx

#define DEMO_AUDIO_DATA_CHANNEL (1U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (3000)
#define DEMO_AUDIO_MASTER_CLOCK 12288000U


#define BOARD_MASTER_CLOCK_CONFIG BOARD_MasterClockConfig
#define BUFFER_SIZE   (1024U)
#define BUFFER_NUMBER (2U) // 2 for TX 2 for RX
/* demo audio data channel */
//#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
//#define DEMO_AUDIO_BIT_WIDTH (kSAI_WordWidth16bits)
/* demo audio sample rate */
//#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
//#define DEMO_AUDIO_MASTER_CLOCK 12288000U

void freertos_I2S_initialize();
void freertos_I2S_transfer_stop();

#endif /* FREERTOS_I2S_H_ */
