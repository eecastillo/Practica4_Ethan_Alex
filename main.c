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
#include "filter.h"

/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */


#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(SYS_CLK)
#define DEMO_UART          UART0

const char* ENABLE_LP = "PBE";
const char* ENABLE_HP = "HPE";
const char* ENABLE_BP = "BPE";
const char* DISABLE_LP = "PBD";
const char* DISABLE_HP = "HPD";
const char* DISABLE_BP = "BPD";

SemaphoreHandle_t initialization_sem;
uint32_t Buffer[4*1024];
uint32_t rxBuffer = 0;
static char uart_data[3];


void init_project(void *parameters);
void codec_get_audio(void *parameters);
void initialize_uart(void *parameters);


int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    initialization_sem = xSemaphoreCreateBinary();

    xTaskCreate(initialize_uart, "initialize UART", 110, NULL, 1, NULL);
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
	edma_initialize();


	//CONFIGURAR I2S
	codec_i2s_config();


	while(1)
	{
		xSemaphoreGive(initialization_sem);

		vTaskDelay(portMAX_DELAY);
	}

}

void initialize_uart(void *parameters)
{
	init_buff(Buffer);
	static uart_config_t configuration;
	UART_GetDefaultConfig(&configuration);
	configuration.baudRate_Bps = 9600;
	configuration.enableTx     = true;
	configuration.enableRx     = true;

	UART_Init(DEMO_UART, &configuration, DEMO_UART_CLK_FREQ);

	uint8_t buffer[]   = "Practica 4 Alejandro Gudiño \tEthan Castillo\r\nSeleccionar filtro\r\n\r\nPasa bajas (LP)\n\rPasa altas (HP)\n\rPasa bandas (BP)\r\n";
	UART_WriteBlocking(DEMO_UART, buffer, sizeof(buffer) - 1);

	/* Tomar datos de la uart*/
	uint8_t i = 0;
	uint8_t ch;
	do{
		UART_ReadBlocking(DEMO_UART, &ch, 1);
		UART_WriteBlocking(DEMO_UART,&ch, 1);
		uart_data[i] = ch;
		i++;
	} while(i < 3);
	if(strcmp(ENABLE_LP,uart_data) == 0)
	{
		apply_filter(LOW_PASS);
	}
	else if(strcmp(ENABLE_HP,uart_data) == 0)
	{
		apply_filter(HIGH_PASS);
	}
	else if(strcmp(ENABLE_BP,uart_data) == 0)
	{
		apply_filter(BAND_PASS);
	}
	else if(strcmp(DISABLE_LP,uart_data) == 0 || strcmp(DISABLE_HP,uart_data) == 0 || strcmp(DISABLE_BP,uart_data) == 0)
	{
		apply_filter(DISABLE);
	}

}

void codec_get_audio(void *parameters)
{
	xSemaphoreTake(initialization_sem, portMAX_DELAY);

	while(1)
	{
//		codec_rx((uint8_t*)(Buffer+rxBuffer),1024);
//
//		rxBuffer++;
//
//		if(rxBuffer >= 4)
//		{
//			rxBuffer = 0U;
//		}
	}
}

