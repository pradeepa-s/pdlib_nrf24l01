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
	
	unsigned char ucStatus, ucStatus1, ucStatus3;

	SysCtlClockSet(SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_SYSDIV_5);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

	pdlibSPI_ConfigureSPIInterface(0x03);
	NRF24L01_Init(GPIO_PORTE_BASE,GPIO_PIN_1, GPIO_PORTE_BASE, GPIO_PIN_2, 0x03);

	//NRF24L01_PowerDown();

	ucStatus3 = 0;

	while(1)
	{
		ucStatus = NRF24L01_GetStatus();
	//	ucStatus1 = NRF24L01_RegisterRead_8(ucStatus3);
		ucStatus3++;
		SysCtlDelay(0xFFFFFF);
	}

	return 0;
}