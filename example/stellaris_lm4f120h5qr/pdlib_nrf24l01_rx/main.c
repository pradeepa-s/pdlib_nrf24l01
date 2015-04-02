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
#include "uart_debug.h"

int main(void) {
	int status;
	char pipe;
	char temp;
	unsigned char address[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
	char data[32];
	//unsigned char buf[5];

	/* PS: Set the clock frequency of the processor */
	ROM_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);

	InitUARTDebug();

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE, 0x03);

	/* Set the address */
	NRF24L01_SetRxAddress(PDLIB_NRF24_PIPE0, address);

	/* Set the packet size */
	NRF24L01_SetRXPacketSize(PDLIB_NRF24_PIPE0, 23);

	while(1)
	{
		/* Receive data */
		status = NRF24L01_WaitForDataRx(&pipe);

		if(PDLIB_NRF24_SUCCESS == status)
		{
			temp = NRF24L01_GetRxDataAmount(pipe);
			PrintRegValue("Data Available in: ",pipe);
			PrintRegValue("Data amount available : ",temp);
			memset(data,0x00,32);
			status = NRF24L01_GetData(pipe, data, &temp);
			PrintString("Data Read: ");
			PrintString((const char*)data);
			PrintString("\n\r");
		}

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,0xFF);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 6);

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,0);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 6);
	}
}
