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

static unsigned char g_ucStatus;
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
	g_ucStatus = 0x00;
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
	
	g_ucStatus = 0x01;
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
 * Function		: 	NRF24L01_GetData
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void
NRF24L01_GetData()
{
}


/* PS:
 * 
 * Function		: 	NRF24L01_SetModuleAddress
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
void 
NRF24L01_SetModuleAddress()
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
 * Function		: 	NRF24L01_ExecuteCommand
 * 
 * Arguments	: 	ucType		: 	0x01 - For RX
 * 									0x02 - For TX
 * 					ucCommand	:	Command to be executed.
 * 					pucPayload	:	Payload of the command.
 * 					uiLength	:	Length of the payload.
 * 
 * Return		: 	Number of bytes transferred or received if success.
 * 					Negetive error code if failed.
 * 
 * Description	: 	ExecuteCommand function can be used to send commands
 * 					to the RF module 
 * 
 * 					For TX:
 * 
 * 					The payload	of the command can also be submitted to 
 * 					the function for easier operation. The ucType parameter 
 * 					should be set to 0x02. The function will return after
 * 					all the data is submitted to the module.
 * 
 * 					For RX:
 * 
 * 					A buffer should be preallocated and provided to the 
 * 					function. The length of the buffer is also expected.
 * 					The ucType parameter should be set to 0x01. Function 
 * 					will return as soon as it received the number of bytes 
 * 					specified by the uiLength or when the module has no
 * 					more data in the Rx FIFO.
 * 
 */

static int
NRF24L01_ExecuteCommand(unsigned char ucType, unsigned char ucCommand, unsigned char *pucPayload, unsigned int uiLength)
{
	int iReturn = -1;
	
	if(0x01 == ucType)
	{
		/* PS: Rx operation */
	}else if(0x02 == ucType)
	{
		/* PS: Tx operation */
	}else
	{
	}
	
	return iReturn;
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
