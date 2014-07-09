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

#endif


/* PS:
 * 
 * Function		: 	NRF24L01_Init
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
  
#ifdef LM4F120H5QR

void
NRF24L01_Init(unsigned long ulCEPin, unsigned char ucSSIIndex)
{
	/* PS: Initialize communication */
#ifdef PDLIB_SPI
	ConfigureSPIInterface(ucSSIIndex);
#endif

	/* PS: Set the CE pin */
	g_ulCEPin = ulCEPin;
	
	
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
NRF24L01_SetAirDataRate(unsigned char ucRFChannel)
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
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */
 
static void
NRF24L01_CELow()
{
}
 
  
/* PS:
 * 
 * Function		: 	NRF24L01_CEHigh
 * 
 * Arguments	: 	
 * 
 * Return		: 	None
 * 
 * Description	: 	
 * 
 */

static void
NRF24L01_CEHigh()
{
}
