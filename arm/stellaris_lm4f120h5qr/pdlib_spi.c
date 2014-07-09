/* Authur:
 * 
 * 
 * Description:
 * 
 * The library contains functions to configure SSI interface in
 * LM4f120H5QR processor.
 * 
 */
 
#define SSIPERIPH   0
#define SSIBASE     1

#define GPIOPERIPH  0
#define SSICLK      1
#define SSIFSS      2
#define SSIRX       3
#define SSITX       4
#define GPIOBASE    5
#define GPIOPINS    6
 
static unsigned char g_SSI = 6;

static const unsigned long g_SSIModule[5][2] =
{
	 {SYSCTL_PERIPH_SSI0, SSI0_BASE},
	 {SYSCTL_PERIPH_SSI1, SSI1_BASE},
	 {SYSCTL_PERIPH_SSI2, SSI2_BASE},
	 {SYSCTL_PERIPH_SSI3, SSI3_BASE},
	 {SYSCTL_PERIPH_SSI1, SSI1_BASE}
};
 
static const unsigned long g_GPIOConfigure[5][7] =
{
	 {SYSCTL_PERIPH_GPIOA, GPIO_PA2_SSI0CLK, GPIO_PA3_SSI0FSS, GPIO_PA4_SSI0RX, GPIO_PA5_SSI0TX, GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2},
	 {SYSCTL_PERIPH_GPIOF, GPIO_PF2_SSI0CLK, GPIO_PF3_SSI0FSS, GPIO_PF0_SSI0RX, GPIO_PF1_SSI0TX, GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1},
	 {SYSCTL_PERIPH_GPIOB, GPIO_PB4_SSI0CLK, GPIO_PB5_SSI0FSS, GPIO_PB6_SSI0RX, GPIO_PB7_SSI0TX, GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7},
	 {SYSCTL_PERIPH_GPIOD, GPIO_PD0_SSI0CLK, GPIO_PD1_SSI0FSS, GPIO_PD2_SSI0RX, GPIO_PD3_SSI0TX, GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3},
	 {SYSCTL_PERIPH_GPIOD, GPIO_PD0_SSI0CLK, GPIO_PD1_SSI0FSS, GPIO_PD2_SSI0RX, GPIO_PD3_SSI0TX, GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3}
}

char *g_plRxData[256];
 
void
ConfigureSPIInterface(unsigned char ucSSI)
{
	g_SSI = ucSSI;

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
								SSI_MODE_MASTER, 8000000, 8);
		ROM_SSIEnable(g_SSIModule[ucSSI][SSIBASE]);
		
		/* Clear initial data */
		while(ROM_SSIDataGetNonBlocking(g_SSIModule[ucSSI][SSIBASE], &g_plRxData[0]);
	}
}

void
SendData(char *pcData, unsigned int uiLength)
{
	//unsigned long ulData;
	int iIndex = 0;
	/* Validate parameters */
	if((pcData != NULL) && (uiLength > 0) && (g_SSI < 5))
	{
			/* Wait until previous transmission is over */
			while(ROM_SSIBusy(g_SSIModule[g_SSI][SSIBASE]);
			
			while(iIndex < uiLength)
			{
				//ulData = pcData;
				ROM_SSIDataPut(g_SSIModule[g_SSI][SSIBASE], pcData[iIndex++]);
			}
			
			/* Wait until current transmission is over */
			while(ROM_SSIBusy(g_SSIModule[g_SSI][SSIBASE]);
	}
}

void
ReceiveDataBlocking(char *pcData)
{
	unsigned long ulRxData;
	/* Validate the arguments */
	if(pcData != NULL)
	{
		ROM_SSIDataGet(g_SSIModule[g_SSI][SSIBASE], &ulRxData);
	}
	
	(*pcData) = (char)(ulRxData);
}

unsigned int 
ReceiveDataNonBlocking(char *pcData)
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
