/* NRF24L01 module contains following pins to connect with the controller.
 * 
 * CE	->	Chip enable (Activates RX and TX) 
 * CSN	->	SPI chip select signal
 * SCK	->	SPI clock
 * MOSI	->	SPI data input (Master Out Slave Input)
 * MISO	->	SPI data output
 * IRQ	->	Maskable interrupt pin. Active Low
 * 
 * Required SPI operation: (Section 8 nRF24L01 Product Specification)
 * 
 * New command start with HIGH -> LOW on CSN
 * 
 * Command word: 	MSBit to LSBit
 * 
 * Data bytes:		LSByte to LSByte (MSBit first in every byte)
 * 
 * 	
 * =====================================================================
 * LM4F120H5QR (Stellaris)
 * =====================================================================
 * 
 * Stellaris SSI can be configured as Freescale SPI.
 * 
 * The CSN, SCK, MOSI and MISO pins are analogus to Freescale SPI.
 * 
 * Need to have a seperate CE pin.
 * 
 * 
 */
 

#ifdef LM4F120H5QR

static unsigned long g_ulCEPin;
static unsigned long g_ulCEBase;

static unsigned long g_ulCEConf[5] = 
{	SYSCTL_PERIPH_GPIOA,
	SYSCTL_PERIPH_GPIOC,
	SYSCTL_PERIPH_GPIOD,
	SYSCTL_PERIPH_GPIOE,
	SYSCTL_PERIPH_GPIOF
};

#endif


/* PS:
 * 
 * Function		: 	NRF24L01_Init
 * 
 * Arguments	: 	ulCEBase	:	This is the base of the CE pin
 * 					ulCEPin		:	This is the pin to be used for CE pin
 * 					ucSSIIndex	:	The index of the SSI module
 * 
 * Return		: 	None
 * 
 * Description	: 	The function will initialize the SSI communication 
 * 					for the module. Also the function intializes and
 * 					configures the CE pin.
 * 
 */
  
#ifdef LM4F120H5QR

void
NRF24L01_Init(unsigned long ulCEBase, unsigned long ulCEPin, unsigned char ucSSIIndex)
{
	/* PS: Initialize communication */
#ifdef PDLIB_SPI
	ConfigureSPIInterface(ucSSIIndex);
#endif

	/* PS: Set the CE pin */
	g_ulCEBase = ulCEBase;
	g_ulCEPin = ulCEPin;
	
	/* PS: Configure the CE pin to be GPIO output */
	ROM_SysCtlPeripheralEnable(g_ulCEConf[ulCEBase]);
	ROM_GPIOPinTypeGPIOOutput(g_ulCEBase, g_ulCEPin);
}

#endif


/* PS:
 * 
 * Function		: 	NRF24L01_GetStatus
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */

unsigned char 
NRF24L01_GetStatus()
{
}

/* PS:
 * 
 * Function		: 	NRF24L01_SetAirDataRate
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_SetAirDataRate(unsigned char ucDataRate)
{
}
 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetRFChannel
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_SetRFChannel(unsigned char ucRFChannel)
{
}
 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetPAGain
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_SetPAGain(unsigned char ucPAGain)
{
} 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetLNAGain
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_SetLNAGain(unsigned char ucLNAGain)
{
}


/* PS:
 * 
 * Function		: 	NRF24L01_PowerDown
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_PowerDown()
{
}

/* PS:
 * 
 * Function		: 	NRF24L01_
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */


/* PS:
 * 
 * Function		: 	NRF24L01_SendCommand
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */

static void
NRF24L01_SendCommand(unsigned char ucCommand)
{
	
}


/* PS:
 * 
 * Function		: 	NRF24L01_CELow
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	This function drives the CE pin low
 * 
 */
 
static void
NRF24L01_CELow()
{
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0x00);
}
 
  
/* PS:
 * 
 * Function		: 	NRF24L01_CEHigh
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	This function drives the CE pin high
 * 
 */

static void
NRF24L01_CEHigh()
{
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0xFF);
}
