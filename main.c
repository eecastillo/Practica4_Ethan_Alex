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
SemaphoreHandle_t i2c_sem;
SemaphoreHandle_t initialization_sem;
uint32_t Buffer[4*1024];
uint32_t rxBuffer = 0;

void init_project(void *parameters);
void codec_get_audio(void *parameters);

void init_wm8731(void *parameters)
{
	uint8_t sucess = freertos_i2c_fail;
	sucess = config_codec();
	if(freertos_i2c_sucess == sucess)
	{
		PRINTF("Configuracion finalizada\n\r");
	}
	xSemaphoreGive(i2c_sem);
	vTaskSuspend(NULL);
}

void codec_audio(void *parameters)
{

	xSemaphoreTake(i2c_sem, portMAX_DELAY);

	for(;;)
	{
		/*TODO AUDIO FUNCTIONS*/
		vTaskDelay(pdMS_TO_TICKS(300));
	}
}

int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    //freertos_I2S_initialize();
    i2c_sem = xSemaphoreCreateBinary();
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

