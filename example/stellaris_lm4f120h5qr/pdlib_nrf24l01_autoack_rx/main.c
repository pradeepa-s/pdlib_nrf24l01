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

void ReceiveDataISR();

int main(void) {
	//int status;
	//char pipe;
	//char temp;
	unsigned char address[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
	char data[32] = "data received!!!";
	//unsigned char buf[5];

	/* PS: Set the clock frequency of the processor */
	ROM_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);

//	InitUARTDebug();

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE, 0x03);

	/* PS: Initialize interrupt */
	NRF24L01_InterruptInit(GPIO_PORTE_BASE, GPIO_PIN_3, SYSCTL_PERIPH_GPIOE, INT_GPIOE);

	/* Set the address */
	NRF24L01_SetRxAddress(PDLIB_NRF24_PIPE0, address);

	/* Enable ack payload */
	NRF24L01_EnableFeatureAckPL();

	/* Set the first ack payload */
	NRF24L01_SetAckPayload(data, 0x00, 17);

	NRF24L01_EnableRxMode();

	while(1)
	{
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,0xFF);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 6);

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2,0);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 6);
	}
}


void ReceiveDataISR(){
	int status;
	char pipe_no = 0;
	char temp;
	char data[32];
	long interrupts;
	char interrupt_flag = 0;
	char data_ack[18] = "data received!!!";

	// Disable global interrupts
	ROM_IntMasterDisable();

	// Disable GPIO interrupt for PORTE
	ROM_GPIOPinIntDisable(GPIO_PORTE_BASE, GPIO_PIN_3);

	// Get the raw interrupt pin status
	interrupts = GPIOPinIntStatus(GPIO_PORTE_BASE,false);

	// Check whether triggered interrupt is the one we need
	if(interrupts & GPIO_PIN_3){
		// Clear the interrupt
		ROM_GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_3);

		// Check which pipe contains data
		status = NRF24L01_IsDataReadyRx(&pipe_no);

		if(PDLIB_NRF24_SUCCESS == status)
		{
			// Get data amount in the pipe
			temp = NRF24L01_GetRxDataAmount(pipe_no);
		//	PrintRegValue("Data Available in: ",pipe_no);
		//	PrintRegValue("Data amount available : ",temp);
			memset(data,0x00,32);

			// Get data from the pipe
			status = NRF24L01_GetData(pipe_no, data, &temp);
		//	PrintString("Data Read: ");
		//	PrintString((const char*)data);
		//	PrintString("\n\r");

			/* Put the next ack payload */
			NRF24L01_SetAckPayload(data_ack, 0x00, 17);
		}

		/* Check whether ACK payload is properly sent */
		interrupt_flag = NRF24L01_GetInterruptState();

		if(PDLIB_INTERRUPT_DATA_SENT & interrupt_flag){
			NRF24L01_ClearInterruptFlag(PDLIB_INTERRUPT_DATA_SENT | PDLIB_INTERRUPT_MAX_RT);
		}
	}

	// Enable GPIO interrupts for PORTE pin 3
	ROM_GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_3);

	// Enable global interrupts
	ROM_IntMasterEnable();
}
