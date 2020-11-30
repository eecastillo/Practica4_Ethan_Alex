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
/* TODO: insert other definitions and declarations here. */
/*
 * @brief   Application entry point.
 */

void init_wm8731(void *parameters)
{
	uint8_t sucess = freertos_i2c_fail;
	sucess = config_codec();
	if(freertos_i2c_sucess == sucess)
	{
		PRINTF("Configuracion finalizada\n\r");
	}
	vTaskSuspend(NULL);
}


int main(void)
{
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif


    xTaskCreate(init_wm8731, "init_codec", 110, NULL, 1, NULL);

    vTaskStartScheduler();

    for(;;)
    {

    }
    return 0 ;
}
