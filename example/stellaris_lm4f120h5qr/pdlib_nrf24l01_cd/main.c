/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdlib_nrf24l01.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "inc/hw_ints.h"
//#include "uart_debug.h"

int main(void) {
	unsigned char address[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};

	/* PS: Set the clock frequency of the processor */
	ROM_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);

	//InitUARTDebug();

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE, 0x03);

	/* Set the address */
	NRF24L01_SetRxAddress(PDLIB_NRF24_PIPE0, address);
	NRF24L01_EnableRxMode();
	ROM_SysCtlDelay(ROM_SysCtlClockGet() / 60);

	int i = 0;
	while(1)
	{

		for(i=0; i<64; i++){

			/* PS: Change RF channel */
			NRF24L01_SetRFChannel(i);
			ROM_SysCtlDelay(ROM_SysCtlClockGet() / 30);

			if(NRF24L01_CarrierDetect()){
				/* PS: Carrier detected on current RF channel */
				//PrintRegValue("Detected: ",i);

				NRF24L01_FlushRX();
			}else{
				//PrintRegValue("None: ",i);
			}
		}
	}
}
