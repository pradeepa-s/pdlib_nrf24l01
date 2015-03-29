/* 
 * Please find the license in the GIT repo.
 * 
 * Description:
 * 
 * The library contains functions to configure SSI interface in
 * LM4f120H5QR processor. The library is designed to be used to
 * interface the NRF24L01 module with the processor.
 * 
 * Git repo:
 * 
 * https://github.com/pradeepa-s/pdlib_nrf24l01.git
 * 
 * Contributors:
 * 
 * Pradeepa Senanayake : pradeepa.kck@gmail.com
 * 
 * Change log:
 * 
 * 2014-07-09 : Support for LM4F120H5QR processor
 * 				Added configurations required for Freescale SPI
 * 				Added GPIO configurations for all SSI modules
 * 				Added function to get one byte(blocking and none blocking)
 * 				Added function to send data(blocking)
 * 
 */

#include <stdio.h>
#include "pdlib_spi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"

#define SSIPERIPH   0
#define SSIBASE     1

#define GPIOPERIPH  0
#define SSICLK      1
#define SSIFSS      2
#define SSIRX       3
#define SSITX       4
#define GPIOBASE    5
#define GPIOPINS    6
 
/* PS: Variable to track which SSI module is being used */
static unsigned char g_SSI = 6;

/* PS: SSI Base and SSI Peripheral defines mapping */
static const unsigned long g_SSIModule[5][2] =
{
	 {SYSCTL_PERIPH_SSI0, SSI0_BASE},
	 {SYSCTL_PERIPH_SSI1, SSI1_BASE},
	 {SYSCTL_PERIPH_SSI2, SSI2_BASE},
	 {SYSCTL_PERIPH_SSI3, SSI3_BASE},
	 {SYSCTL_PERIPH_SSI1, SSI1_BASE}
};

/* PS: GPIO configurations for SSI modules*/
static const unsigned long g_GPIOConfigure[5][7] =
{
	 {SYSCTL_PERIPH_GPIOA, GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX, GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2},
	 {SYSCTL_PERIPH_GPIOF, GPIO_PF2_SSI1CLK, GPIO_PF3_SSI1FSS, GPIO_PF0_SSI1RX, GPIO_PF1_SSI1TX, GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1},
	 {SYSCTL_PERIPH_GPIOB, GPIO_PB4_SSI2CLK, GPIO_PB5_SSI2FSS, GPIO_PB6_SSI2RX, GPIO_PB7_SSI2TX, GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7},
	 {SYSCTL_PERIPH_GPIOD, GPIO_PD0_SSI3CLK, GPIO_PD1_SSI3FSS, GPIO_PD2_SSI3RX, GPIO_PD3_SSI3TX, GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3},
	 {SYSCTL_PERIPH_GPIOD, GPIO_PD0_SSI1CLK, GPIO_PD1_SSI1FSS, GPIO_PD2_SSI1RX, GPIO_PD3_SSI1TX, GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3}
};

/* PS: RX data */
char g_plRxData[256];

#ifdef PART_LM4F120H5QR

/* PS:
 * 
 * Function		: 	pdlibSPI_ConfigureSPIInterface
 * 
 * Arguments	: 	ucSSI - SSI module index (0,1,2,3,4,5) [5 is for SSI1 with GPIO PORTD]
 * 
 * Return		: 	None
 * 
 * Description	: 	The function will setup the SSI module to make it same as Freescale SPI module.
 * 					GPIO configurations required and Clock configurations also done in the function.
 * 
 */

void
pdlibSPI_ConfigureSPIInterface(unsigned char ucSSI)
{
	g_SSI = ucSSI;

#ifdef PART_LM4F120H5QR
	if(g_SSI < 6)
	{
		 /* Enable clock for SSI */
		ROM_SysCtlPeripheralEnable(g_SSIModule[ucSSI][SSIPERIPH]);
		
		/* Disable SSI module */
		ROM_SSIDisable(g_SSIModule[ucSSI][SSIBASE]);
		
		/* Enable Clock for GPIO port used */
		ROM_SysCtlPeripheralEnable(g_GPIOConfigure[ucSSI][GPIOPERIPH]);
		 
		/* Configure GPIO pins */
		ROM_GPIOPinConfigure(g_GPIOConfigure[ucSSI][SSICLK]);
		ROM_GPIOPinConfigure(g_GPIOConfigure[ucSSI][SSIFSS]);
		ROM_GPIOPinConfigure(g_GPIOConfigure[ucSSI][SSIRX]);
		ROM_GPIOPinConfigure(g_GPIOConfigure[ucSSI][SSITX]);
		
		ROM_GPIOPinTypeSSI(g_GPIOConfigure[ucSSI][GPIOBASE], g_GPIOConfigure[ucSSI][GPIOPINS]);
		
		/* Configure SSI */
		ROM_SSIClockSourceSet(g_SSIModule[ucSSI][SSIBASE], SSI_CLOCK_SYSTEM);
		ROM_SSIConfigSetExpClk(g_SSIModule[ucSSI][SSIBASE], SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
								SSI_MODE_MASTER, 500000, 8);
		ROM_SSIEnable(g_SSIModule[ucSSI][SSIBASE]);
		
		/* Clear initial data */
		while(ROM_SSIDataGetNonBlocking(g_SSIModule[ucSSI][SSIBASE], (unsigned long*)&g_plRxData[0]));
/*
		HWREG(g_SSIModule[ucSSI][SSIBASE] + SSI_O_CPSR) = 8;

		HWREG(g_SSIModule[ucSSI][SSIBASE] + SSI_O_CR0) &= ~(SSI_CR0_SPO | SSI_CR0_SPH);

		HWREG(g_SSIModule[ucSSI][SSIBASE] + SSI_O_CR0) |= 0x00;
*/
	}
#endif
}


/* PS:
 * 
 * Function		: 	pdlibSPI_SendData
 * 
 * Arguments	: 	pucData 		- Char array of data to be sent. 
 * 					uiLength		- Length of the data array
 * 
 * Return		: 	If success the function will return the number of data
 * 					bytes it has written to the SPI module. 
 * 					If failed the function will return ZERO.
 * 
 * Description	: 	The function will submit data byte by byte to the SPI module
 * 					for transmission and will wait until the total transmission
 * 					is over. The function is a blocking function.
 * 
 */

int
pdlibSPI_SendData(unsigned char *pucData, unsigned int uiLength)
{
	int iIndex = 0;
	/* Validate parameters */
	if((pucData != NULL) && (uiLength > 0) && (g_SSI < 5))
	{
#ifdef PART_LM4F120H5QR
			while(iIndex < uiLength)
			{
				pdlibSPI_TransferByte(pucData[iIndex++]);
			}
#endif
	}
	
	return iIndex;
}


/* PS:
 * 
 * Function		: 	pdlibSPI_TransferByte
 *
 * Arguments	: 	ucData	:	Data byte to transfer
 *
 * Return		: 	Function will return whatever data received from the module during the transfer.
 *
 * Description	: 	The function will submit a data byte the SPI module
 * 					for transmission and will wait until the total transmission
 * 					is over. Then it will read the TX buffer and read one byte out from the buffer.
 * 					This will clear the TX buffer.
 *
 */

unsigned char
pdlibSPI_TransferByte(unsigned char ucData)
{
	unsigned long ulRxData;
	/* Validate parameters */
	if(g_SSI < 5)
	{
#ifdef PART_LM4F120H5QR

			ROM_SSIDataPut(g_SSIModule[g_SSI][SSIBASE], ucData);

			/* Wait until current transmission is over */
			while(ROM_SSIBusy(g_SSIModule[g_SSI][SSIBASE]));

			ROM_SSIDataGet(g_SSIModule[g_SSI][SSIBASE], &ulRxData);

			/* Wait until current transmission is over */
			while(ROM_SSIBusy(g_SSIModule[g_SSI][SSIBASE]));
#endif
	}

	return ((unsigned char)(ulRxData & 0xFF));
}


/* PS:
 *
 * Function		: 	pdlibSPI_ReceiveDataBlocking
 * 
 * Arguments	: 	None
 * 
 * Return		: 	Returns the byte value read
 * 
 * Description	: 	The function will read one byte of data from the SSI
 * 					module RX FIFO. The function will not return until 
 * 					data is available.
 * 
 */

unsigned char
pdlibSPI_ReceiveDataBlocking()
{
	unsigned long ulRxData;

	ROM_SSIDataGet(g_SSIModule[g_SSI][SSIBASE], &ulRxData);
	
	return ((unsigned char)(ulRxData & 0xFF));
}

/* PS:
 * 
 * Function		: 	pdlibSPI_ReceiveDataNonBlocking
 * 
 * Arguments	: 	pcData - Pre allocated 'char' value to receive data
 * 
 * Return		: 	unsigned int value, which contains the number of bytes read.
 * 
 * Description	: 	The function will read one byte of data from the SSI
 * 					module RX FIFO. The function is none blocking. It will
 * 					return zero if there are no data in the RX-FIFO of the
 * 					SSI module
 * 
 */


unsigned int 
pdlibSPI_ReceiveDataNonBlocking(char *pcData)
{
	unsigned int iReturn = 0;
	unsigned long ulRxData;
	
	/* Validate the arguments */
	if(pcData != NULL)
	{
		iReturn = ROM_SSIDataGetNonBlocking(g_SSIModule[g_SSI][SSIBASE], &ulRxData);
	}
	
	if(iReturn > 0)
	{
		(*pcData) = (char)(ulRxData);
	}
	
	return iReturn;
}

#endif
