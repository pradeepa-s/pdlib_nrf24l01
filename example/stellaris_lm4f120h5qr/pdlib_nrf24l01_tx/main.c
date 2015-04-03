/*
 * main.c
 */


#include "pdlib_nrf24l01.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
//#include "uart_debug.h"

int main(void) {
	int status;
	unsigned char address[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
	char data[] = "12345678901234567890123";

	/* PS: Set the clock frequency of the processor */
	ROM_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);

	//InitUARTDebug();

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE, 0x03);

	/* Set the address */
	NRF24L01_SetTXAddress(address);

	while(1)
	{
		/* Send data */
		status = NRF24L01_SendData(data, 23);

		if(PDLIB_NRF24_TX_FIFO_FULL == status)
		{
			/* If TX Fifo is full, we can flush it */
			NRF24L01_FlushTX();
		}else if(PDLIB_NRF24_TX_ARC_REACHED == status)
		{
			while(status == PDLIB_NRF24_TX_ARC_REACHED)
			{
				/* Automatic retransmission count reached, we'll attempt the TX continuously until the TX completes */
				status = NRF24L01_AttemptTx();
			}
		}

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3,0xFF);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 4);

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 4);
	}
}
