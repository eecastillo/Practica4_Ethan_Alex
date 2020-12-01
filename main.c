#include <stdio.h>
#include <WM87313.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "freertos_i2c.h"
#include "task.h"
#include "semphr.h"
#include "fsl_uart.h"
#include "freertos_I2S.h"

/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t Buffer[BUFFER_NUMBER * BUFFER_SIZE], 4);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle);
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t rxHandle);
edma_handle_t dmaTxHandle = {0}, dmaRxHandle = {0};


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

SemaphoreHandle_t initialization_sem;
uint32_t Buffer[4*1024];
uint32_t rxBuffer = 0;

void init_project(void *parameters);
void codec_get_audio(void *parameters);


int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    initialization_sem = xSemaphoreCreateBinary();

    xTaskCreate(init_project, "init project", 110, NULL, 1, NULL);
    xTaskCreate(codec_get_audio, "get audio", 110, NULL, 1, NULL);

    vTaskStartScheduler();

    for(;;)
    {

    }
    return 0 ;
}

void init_project(void *parameters)
{
	uint8_t sucess = freertos_i2c_fail;
	sucess = config_codec(); //configurar I2C
	if(freertos_i2c_sucess == sucess)
	{
		PRINTF("Inicializacion I2C finalizada\n\r");
	}
	else
	{
		PRINTF("Inicializacion I2C fallida");
	}

	//CONFIGURAR DMA
	//ASOCIAR DMA CON I2S
	//ASOCIAR INTERRUPCIONES DE CUANDO EL DMA ESTE LLENO Y PROCESAR Y VOLVER A LLENAR DMA RX
	//PROCESAR LA INFORMACION Y PONERLA EN EL CANAL DE TRANSMICION DE DMA
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


	//CONFIGURAR I2S
	codec_i2s_config();


	while(1)
	{
		xSemaphoreGive(initialization_sem);

		vTaskDelay(portMAX_DELAY);
	}

}

void codec_get_audio(void *parameters)
{
	xSemaphoreTake(initialization_sem, portMAX_DELAY);

	while(1)
	{
		codec_rx((uint8_t*)(Buffer+rxBuffer),1024);

		rxBuffer++;

		if(rxBuffer >= 4)
		{
			rxBuffer = 0U;
		}
	}
}

