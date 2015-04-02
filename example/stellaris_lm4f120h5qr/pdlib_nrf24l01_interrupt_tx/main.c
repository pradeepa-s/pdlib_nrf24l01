/*
 * main.c
 */


#include "pdlib_nrf24l01.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "uart_debug.h"
#include "inc/hw_ints.h"

void TransmitDataISR();

int main(void) {
	int status;
	unsigned char address[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
	char data[] = "12345678901234567890123";

	/* PS: Set the clock frequency of the processor */
	ROM_SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);

	InitUARTDebug();

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, SYSCTL_PERIPH_GPIOE, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE, 0x03);

	/* PS: Initialize interrupt */
	NRF24L01_InterruptInit(GPIO_PORTE_BASE, GPIO_PIN_3, SYSCTL_PERIPH_GPIOE, INT_GPIOE);

	/* Set the address */
	NRF24L01_SetTXAddress(address);

	while(1)
	{
		if(0 == NRF24L01_IsTxFifoFull()){
			/* Send data */
			status = NRF24L01_SubmitData(data, 23);

			if(PDLIB_NRF24_SUCCESS == status){
				NRF24L01_EnableTxMode();
			}
		}
#if 0
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
#endif

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3,0xFF);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 4);

		ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 4);
	}
}

void TransmitDataISR(){
	int status;
	long interrupts;

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

		/* Check the interrupt status from module to see whether
		 * the interrupt is due to Data-Sent or ARC reached.
		 */

		status = NRF24L01_WaitForTxComplete(0);

		if(PDLIB_NRF24_TX_ARC_REACHED == status){

			// Retry transmission
			NRF24L01_EnableTxMode();

		}else if(PDLIB_NRF24_SUCCESS == status){

			if(0 == NRF24L01_IsTxFifoEmpty()){
				// Send the rest of the data
				NRF24L01_EnableTxMode();
			}else{
				NRF24L01_DisableTxMode();
				NRF24L01_PowerDown();
			}
		}
	}

	// Enable GPIO interrupts for PORTE pin 3
	ROM_GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_3);

	// Enable global interrupts
	ROM_IntMasterEnable();
}
