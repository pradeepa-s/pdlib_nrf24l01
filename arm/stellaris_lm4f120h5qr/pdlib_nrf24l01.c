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
 * Data bytes:		LSByte to MSByte (MSBit first in every byte)
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
 
#include "nRF24L01.h"

#define TYPE_RX		0x01
#define TYPE_TX		0x02

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
static unsigned char g_ucPdlibStatus;
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
	g_ucPdlibStatus = 0x00;
	/* PS: Initialize communication */
#ifdef PDLIB_SPI
	pdlibSPI_ConfigureSPIInterface(ucSSIIndex);
#endif

	/* PS: Set the CE pin */
	g_ulCEBase = ulCEBase;
	g_ulCEPin = ulCEPin;
	
	/* PS: Configure the CE pin to be GPIO output */
	ROM_SysCtlPeripheralEnable(g_ulCEConf[ulCEBase]);
	ROM_GPIOPinTypeGPIOOutput(g_ulCEBase, g_ulCEPin);
	
	g_ucPdlibStatus = 0x01;
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
	g_ucStatus = _NRF24L01_RegisterRead_8(RF24_NOP);
	return g_ucStatus;
}

/* PS:
 * 
 * Function		: 	NRF24L01_SetAirDataRate
 * 
 * Arguments	: 	ucDataRate	:	0 for 1 Mbps
 * 									1 for 2 Mbps
 * 
 * Return		: 	None
 * 
 * Description	: 	Sets the air data rate (default 2Mbps)
 * 
 */
 
void
NRF24L01_SetAirDataRate(unsigned char ucDataRate)
{
	unsigned char ucCurrentVal = _NRF24L01_RegisterRead_8(RF24_RF_SETUP);
	ucCurrentVal |= (ucDataRate << 3);
	
	_NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
}
 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetRFChannel
 * 
 * Arguments	: 	ucRFChannel	:	RF channel value (only 0:6 bits valid)
 * 
 * Return		: 	None
 * 
 * Description	: 	Sets the RF channel frequency based on the below
 * 					formula,
 * 						2400 + ucRFChannel (MHz)
 * 
 */
 
void
NRF24L01_SetRFChannel(unsigned char ucRFChannel)
{
	_NRF24L01_RegisterWrite_8(RF24_RF_CH, ucRFChannel);
}
 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetPAGain
 * 
 * Arguments	: 	ucPAGain	: Only bits 0:1 are valid.
 * 
 * Return		: 	None
 * 
 * Description	: 	Sets the power amplifier gain based on the ucPAGain.
 * 					11 = 0 dBm (default)
 * 					10 = -6 dBm
 * 					01 = -12 dBm
 * 					00 = -18 dBm
 * 
 */
 
void
NRF24L01_SetPAGain(unsigned char ucPAGain)
{
	unsigned char ucCurrentVal = _NRF24L01_RegisterRead_8(RF24_RF_SETUP);
	ucCurrentVal |= (ucPAGain << 1);
	
	_NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
} 
 
 
/* PS:
 * 
 * Function		: 	NRF24L01_SetLNAGain
 * 
 * Arguments	: 	ucLNAGain	:	0 - Disable LNA gain
 * 									1 - Enable LNA gain
 * 
 * Return		: 	None
 * 
 * Description	: 	Function will setup the LNA gain of the module.
 * 
 */
 
void
NRF24L01_SetLNAGain(unsigned char ucLNAGain)
{
	unsigned char ucCurrentVal = _NRF24L01_RegisterRead_8(RF24_RF_SETUP);
	ucCurrentVal |= (ucLNAGain << 0);
	
	_NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
}


/* PS:
 * 
 * Function		: 	NRF24L01_PowerDown
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Go to power down mode
 * 
 */
 
void
NRF24L01_PowerDown()
{
	unsigned char ucCurrentVal = _NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal &= (~RF24_PWR_UP);
	
	_NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);
}

/* PS:
 * 
 * Function		: 	NRF24L01_PowerUp
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Go to Standby mode. Takes 1.5 ms to go to Standby.
 * 
 */
 
void
NRF24L01_PowerUp()
{
	unsigned char ucCurrentVal = _NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal |= (RF24_PWR_UP);
	
	_NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);
}


/* PS:
 * 
 * Function		: 	NRF24L01_SetModuleTXAddress
 * 
 * Arguments	: 	pucAddress	:	Buffer which contains the five bytes to put to address.
 * 
 * Return		: 	None
 * 
 * Description	: 	Set the address of the module in TX mode. The data 
 * 					packet which is transmitted from this RF module will
 * 					contain this address.
 * 
 */
 
void 
NRF24L01_SetModuleTXAddress(unsigned char* pucAddress)
{
	_NRF24L01_RegisterWrite_Multi(RF24_TX_ADDR, pucAddress, 5);
}


/* PS:
 * 
 * Function		: 	NRF24L01_SendData_8
 * 
 * Arguments	: 	pucAddress	:	Buffer which contains the five bytes to put to address.
 * 
 * Return		: 	None
 * 
 * Description	: 	Set the address of the receiving node.
 * 
 */
 
void 
NRF24L01_SendData_8(unsigned char* pucAddress)
{
	_NRF24L01_RegisterWrite_Multi(RF24_TX_ADDR, pucAddress, 5);
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
 * 					pucData		:	Tx: Payload of the command.
 * 									Rx: Buffer to receive data.
 * 					uiLength	:	Length of the payload/buffer.
 * 									(If there is no data buffer this value
 * 									must be set to zero)
 * 
 * 					pucStatus	: 	If not NULL status register value will
 * 									be saved here.
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

int
NRF24L01_ExecuteCommand(unsigned char ucType, unsigned char ucCommand, unsigned char *pucData, unsigned int uiLength, unsigned char *pucStatus)
{
	int iReturn = -1;
	unsigned char *pucDataBuffer = NULL;
	
	/* PS: Validating arguments */
	if(uiLength > 0)
	{
		if(NULL == pucData)
		{
			return iReturn;
		}
	}else
	{
		if((TYPE_TX == ucType) && (RF24_NOP != ucCommand))
		{
			/* PS: Buffer length should be at least one */
			return iReturn;
		}
	}
	
	if(TYPE_RX == ucType)
	{
		/* PS: Rx operation */
	}else if(TYPE_TX == ucType)
	{
		/* PS: Tx operation */
		
		/* TODO:
		 * Sometimes malloc and memcpy functions take longer time.
		 * Need to check whether we can improve performance by 
		 * executing the pdlibSPI_SendData function twice. That is 
		 * once for command and next for payload. Then we can 
		 * avoid using memcpy and malloc here.
		 */
#ifdef PDLIB_SPI		 
		pucDataBuffer = (unsigned char*) malloc(sizeof(char) * (uiLength+1));
		if(pucDataBuffer)
		{
			pucDataBuffer[0] = ucCommand;
			memcpy(&pucDataBuffer[1],pucData,uiLength);
			
			iReturn = pdlibSPI_SendData(pucDataBuffer, (uiLength+1));
			free(pucDataBuffer);
			/* PS: Update the current status register value */
			pdlibSPI_ReceiveDataBlocking(&g_ucStatus);
			(*pucStatus) = g_ucStatus;
		}else
		{
			iReturn = -1;
		}
		
#endif
	}else
	{
	}
	
	
	return iReturn;
}


/* PS:
 * 
 * Function		: 	_NRF24L01_CELow
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	This function drives the CE pin low
 * 
 */
 
static void
_NRF24L01_CELow()
{
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0x00);
}
 
  
/* PS:
 * 
 * Function		: 	_NRF24L01_CEHigh
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	This function drives the CE pin high
 * 
 */

static void
_NRF24L01_CEHigh()
{
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0xFF);
}



/* PS:
 * 
 * Function		: 	_NRF24L01_RegisterWrite_8
 * 
 * Arguments	: 	ucRegister	:	Address of the register
 * 					ucValue		:	Value to write to the register
 * 
 * Return		: 	None
 * 
 * Description	: 	This function will write 1 byte of data to an 8 bit
 * 					register. The function will update the Status
 * 					variable too.
 * 
 */
 
static void
_NRF24L01_RegisterWrite_8(unsigned char ucRegister, unsigned char ucValue)
{
	unsigned char ucData[2];
	
	ucData[0] = (RF24_W_REGISTER | ucRegister);
	ucDate[1] = ucValue;
	
	pdlibSPI_SendData(ucData, 2);
	
	g_ucStatus = pdlibSPI_ReceiveDataBlocking();
}


/* PS:
 * 
 * Function		: 	_NRF24L01_RegisterWrite_Multi
 * 
 * Arguments	: 	ucRegister	:	Address of the register
 * 					pucData	:	Value to write to the register
 * 					uiLength	:	Length of the data field
 * 
 * Return		: 	None
 * 
 * Description	: 	This function will write more than one byte of data 
 * 					to an the specified register. 
 * 					Function will update the Status	variable too.
 * 
 */
 
static void
_NRF24L01_RegisterWrite_Multi(unsigned char ucRegister, unsigned char *pucData, unsigned int uiLength)
{
	if(NULL != pucData)
	{
		unsigned char *pucBuffer = (unsigned char*) malloc(sizeof(unsigned char) * uiLength);
		
		if(NULL != pucBuffer)
		{
			pucBuffer[0] = (RF24_W_REGISTER | ucRegister);
			
			memcpy(&pucBuffer[1], pucData, uiLength);
			
			pdlibSPI_SendData(ucData, uiLength+1);
			
			g_ucStatus = pdlibSPI_ReceiveDataBlocking();
			
			free(pucBuffer);
		}
	}
	
}


/* PS:
 * 
 * Function		: 	_NRF24L01_RegisterRead_8
 * 
 * Arguments	: 	ucRegister	:	Address of the register
 * 
 * Return		: 	The value in the 8 bit register
 * 
 * Description	: 	This function will write 1 byte of data to an 8 bit
 * 					register. The function will update the Status
 * 					variable too.
 * 
 */
 
static unsigned char
_NRF24L01_RegisterRead_8(unsigned char ucRegister)
{
	pdlibSPI_SendData((RF24_R_REGISTER | ucRegister), 1);
	
	g_ucStatus = pdlibSPI_ReceiveDataBlocking();
	
	return pdlibSPI_ReceiveDataBlocking();
}
