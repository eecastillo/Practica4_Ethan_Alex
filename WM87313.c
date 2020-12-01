/*
 * WM8731.c
 *
 *  Created on: Nov 13, 2020
 *      Author: eduar
 */

#include <WM87313.h>
static freertos_i2c_config_t codec_i2c_config;
static  sai_transceiver_t config;
static sai_handle_t sai_rx_handle;
static edma_config_t dmaConfig = {0};

static uint32_t tx_index = 0U, rx_index = 0U;
volatile uint32_t emptyBlock = BUFFER_NUMBER;

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle);
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};

static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
static void rxCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData);

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
		data[1] = CODEC_LEFT_LINEIN;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = RIGHT_LINE_IN_REG;
		data[1] = CODEC_RIGHT_LINEIN;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = LEFT_HEADPHONE_OUT_REG;
		data[1] = CODEC_LEFT_HP;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = RIGHT_HEADPHONE_OUT_REG;
		data[1] = CODEC_RIGHT_HP;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = ANALOGUE_AUDIO_PATH_REG;
		data[1] = CODEC_ANALOGAUDIO;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = DIGITAL_AUDIO_PATH_REG;
		data[1] = CODEC_DIGITALAUDIO;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = POWER_DOWN_CTRL_REG;
		data[1] = CODEC_POWER;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = DIG_AUDIO_INTERFACE_REG;
		data[1] = CODEC_DAIF;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = SAMPLING_CTRL_REG;
		data[1] = CODEC_SAMPLING;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));

		data[0] = ENABLE_CODEC;
		data[1] = 0x01;
		codec_sucess = freertos_i2c_send(WM8731_ADDRESS, data ,2);
		vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));
		activate_codec();

	}
	return codec_sucess;
}
void codec_i2s_config(void)
{
	CLOCK_EnableClock(kCLOCK_PortC);

	PORTC->PCR[1] = PORT_PCR_MUX(6);	//I2S0 TXD0 (Alt 6) PTC1
	PORTC->PCR[5] = PORT_PCR_MUX(4);	//I2S0 RXD0 (Alt 4) PTC5
	PORTC->PCR[9] = PORT_PCR_MUX(4);	//I2S0 RX_BCLK (Alt 4) PTC9
	PORTC->PCR[7] = PORT_PCR_MUX(4);	//I2S0 RX_FS (Alt 4) PTC1

	SAI_Init(I2S0);

	//SAI_TransferRxCreateHandle(I2S0, &sai_rx_handle, rxCallback, NULL);

    SAI_TransferTxCreateHandleEDMA(DEMO_SAI, &txHandle, tx_callback, NULL, &dmaTxHandle);
    SAI_TransferRxCreateHandleEDMA(DEMO_SAI, &rxHandle, rx_callback, NULL, &dmaRxHandle);

	SAI_GetClassicI2SConfig(&config, kSAI_WordWidth24bits, kSAI_Stereo, 1<<0);

	config.bitClock.bclkSource = kSAI_BclkSourceBusclk;
//	config.syncMode = kSAI_ModeAsync;
//	config.masterSlave = kSAI_Slave;
//	config.frameSync.frameSyncPolarity = kSAI_SampleOnRisingEdge;
//	SAI_TransferRxSetConfig(I2S0, &sai_rx_handle, &config);
	config.syncMode = DEMO_SAI_TX_SYNC_MODE;
	SAI_TransferTxSetConfigEDMA(DEMO_SAI, &txHandle, &config);
	config.syncMode = DEMO_SAI_RX_SYNC_MODE;
	SAI_TransferRxSetConfigEDMA(DEMO_SAI, &rxHandle, &config);

	 /* set bit clock divider */
	SAI_TxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, kSAI_WordWidth24bits,
						  DEMO_AUDIO_DATA_CHANNEL);
	SAI_RxSetBitClockRate(DEMO_SAI, DEMO_AUDIO_MASTER_CLOCK, DEMO_AUDIO_SAMPLE_RATE, kSAI_WordWidth24bits,
						  DEMO_AUDIO_DATA_CHANNEL);

}

void rxCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

/*	xSemaphoreGiveFromISR(
	 wm8731_handle.rxSemWM8731,
	 &xHigherPriorityTaskWoken
	);*/

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void txCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{

}

void codec_rx(uint8_t * buffer, uint32_t size)
{
	sai_transfer_t xfer;
	xfer.data = buffer;
	xfer.dataSize = size;

	if(kStatus_Success == SAI_TransferReceiveNonBlocking(I2S0, &sai_rx_handle, &xfer))
	{
	//	xSemaphoreTake(wm8731_handle.rxSemWM8731, portMAX_DELAY);
	}
}

uint8_t i2c_data[2] = {0};
void codec_audio_play(void)
{
	i2c_data[0] = ACTIVE_CTRL_REG;
	i2c_data[1] = CODEC_DEACTIVATE;
	freertos_i2c_send(WM8731_ADDRESS, i2c_data ,2);
	vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));
}
void activate_codec(void)
{
	i2c_data[0] = ACTIVE_CTRL_REG;
	i2c_data[1] = CODEC_ACTIVATE;
	freertos_i2c_send(WM8731_ADDRESS, i2c_data ,2);
	vTaskDelay(pdMS_TO_TICKS(I2C_DELAY));
}
void edma_initialize(void)
{
	 /* Init DMA and create handle for DMA */
	EDMA_GetDefaultConfig(&dmaConfig);
	EDMA_Init(EXAMPLE_DMA, &dmaConfig);
	EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
	EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);

	/* Init DMAMUX */
	DMAMUX_Init(EXAMPLE_DMAMUX);
	DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
	DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
	DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL, (uint8_t)EXAMPLE_SAI_RX_SOURCE);
	DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_RX_CHANNEL);
}

static void rx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyBlock--;
    }
}

static void tx_callback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_TxError == status)
    {
        /* Handle the error. */
    }
    else
    {
        emptyBlock++;
    }
}

