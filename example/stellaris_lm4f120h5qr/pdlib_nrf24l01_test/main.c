/*
 * main.c
 */


#include "pdlib_nrf24l01.h"
#include "pdlib_spi.h"
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"

int main(void) {
	
	unsigned char ucStatus;

	/* PS: Set the clock frequency of the processor */
	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	/* PS: Initialize the module, need to provide CE pin, CSN pin and SSI module information */
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, GPIO_PORTE_BASE, GPIO_PIN_2, 0x03);

	while(1)
	{
		NRF24L01_PowerUp();
		ucStatus = NRF24L01_RegisterRead_8(0x00);
		NRF24L01_PowerDown();
		ucStatus = NRF24L01_RegisterRead_8(0x00);
	}

	return 0;
}
