/* NRF24L01 module contains following pins to connect with the controller	
.
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
 * 	USAGE TX (Simplest) Steps:
 * =====================================================================
 *
 * Version: 1.01
 *
 * [1]. Define the processor type
 * 				Support: PART_LM4F120H5QR
 * [2]. Define the SPI library.
 *				Support: PDLIB_SPI
 *
 * [3]. Call NRF24L01_Init() function with correct SPI settings. (function may changed based on architecture)
 * [4]. Call NRF24L01_SendData() or NRF24L01_SendDataTo() to send data. (TX and RX data sizes should match) - Dynamic payload not supported
 *
 *
 * =====================================================================
 * 	USAGE RX (Simplest) Steps:
 * =====================================================================
 *
 * Version: 1.01
 *
 * [1]. Define the processor type
 * 				Support: PART_LM4F120H5QR
 * [2]. Define the SPI library.
 *				Support: PDLIB_SPI
 *
 * [3]. Call NRF24L01_Init() function with correct SPI settings. (function may changed based on architecture)
 * [4]. Set the RX packet size for required pipe using NRF24L01_SetRXPacketSize()
 * [5]. Wait for data using NRF24L01_WaitForDataRx()
 * [6]. Get the amount of received data using NRF24L01_GetRxDataAmount()
 * [7]. Get the received data using NRF24L01_GetData()
 *
 *
 * =====================================================================
 * 	Registering Interrupt (Simplest) Steps:
 * =====================================================================
 *
 *	[1]. Define 'NRF24L01_CONF_INTERRUPT_PIN' in project settings.
 *	[2]. Call NRF24L01_InterruptInit() function providing required information
 *	[3]. Create the ISR function.
 *	[4]. Register the ISR in the Interrupt Vector
 *
 * =====================================================================
 * Change Log
 * =====================================================================
 *
 * Version: 1.01
 *
 * [1]. Initial release.
 * [2]. Contains function support to change all the basic settings.
 * [3]. Supports LM4F120H5QR processor. (Stellaris-launchpad)
 * [4]. Three function levels defined, Simple, Average and Advanced
 * [5]. Library is compatible for both interrupt and polling
 * [6]. PDLIB_SPI library is supported by default
 *
 *
 * Version: 1.02
 *
 * [1]. Added support for dynamic payload.
 * [2]. Added support for ack payload.
 *
 * =====================================================================
 * Known Issues
 * =====================================================================
 *
 * Version: 1.01
 *
 * [1]. Dynamic payload support is not added.
 * [2]. Auto ACK with payload support is not added
 *
 * Version: 1.02
 *
 * None
 *
 * =====================================================================
 * LM4F120H5QR (Stellaris)
 * =====================================================================
 * 
 * Stellaris SSI can be configured as Freescale SPI.
 * 
 * The CSN, SCK, MOSI and MISO pins are analogous to Freescale SPI.
 * 
 * Need to have a separate CE pin and IRQ pin.
 * 
 * Tested connection: 	PE1 <-> CE
 * (Using SSI3)			PE2 <-> CSN
 * 						PD0 <-> SCK
 * 						PD3 <-> MOSI
 * 						PD2 <-> MISO
 * 						PE3	<-> IRQ
 */

#define PDLIB_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include "pdlib_nrf24l01.h"

#ifdef PDLIB_DEBUG
#include "uart_debug.h"
#endif

// SPI library
#ifdef PDLIB_SPI
#include "pdlib_spi.h"
#endif

// Processor architecture
#ifdef PART_LM4F120H5QR
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#endif

#define TYPE_RX		0x01
#define TYPE_TX		0x02

#define INTERNAL_STATE_INIT				(1 << 0)
#define INTERNAL_STATE_DYNPL			(1 << 1)
#define INTERNAL_STATE_ACKPL			(1 << 2)
#define INTERNAL_STATE_FEATURE_ENABLED	(1 << 3)
#define INTERNAL_STATE_POWER_UP			(1 << 4)
#define INTERNAL_STATE_STAND_BY			(1 << 5)

static void _NRF24L01_CEHigh();
static void _NRF24L01_CELow();

static void _NRF24L01_CSNHigh();
static void _NRF24L01_CSNLow();

static unsigned long g_ulCEPin;
static unsigned long g_ulCEBase;
static unsigned long g_ulCSNPin;
static unsigned long g_ulCSNBase;

static unsigned char g_ucStatus;

static unsigned int internal_states;

/* PS:
 * 
 * Function		: 	NRF24L01_Init
 * 
 * Arguments	: 	ulCEBase	:	This is the base of the CE pin
 * 					ulCEPin		:	This is the pin to be used for CE
 * 					ulCEPeriph	:	This is the peripheral bit mask
 * 					ulCSNBase	:	This is the base of the CSN pin
 * 					ulCSNPin	:	This is the pin to be used for CSN
 * 					ulCSNPeriph	:	This is the peripheral bit mask
 * 					ucSSIIndex	:	The index of the SSI module
 * 
 * Return		: 	None
 * 
 * Description	: 	The function will initialize the SSI communication 
 * 					for the module. Also the function initializes and
 * 					configures the CE pin. It will reset the registers
 * 					to their default values.
 *
 * 					We cannot use the CSN pin (ie. FSS) in the SSI module
 * 					because we need to keep it LOW throughout the data
 * 					transmission. (ie: Write and Read passes) Therefore
 * 					a different pin should be configured for this purpose
 * 
 */
 	
  
#ifdef PART_LM4F120H5QR

void
NRF24L01_Init(	unsigned long ulCEBase,
				unsigned long ulCEPin,
				unsigned long ulCEPeriph,
				unsigned long ulCSNBase,
				unsigned long ulCSNPin,
				unsigned long ulCSNPeriph,
				unsigned char ucSSIIndex)
{
	internal_states = 0x00;

	/* PS: Initialize communication */
#ifdef PDLIB_SPI
	pdlibSPI_ConfigureSPIInterface(ucSSIIndex);
#endif

	/* PS: Set the CE pin */
	g_ulCEBase = ulCEBase;
	g_ulCEPin = ulCEPin;
	
	/* PS: Set the CSN pin */
	g_ulCSNBase = ulCSNBase;
	g_ulCSNPin = ulCSNPin;

	/* PS: Configure the CE pin to be GPIO output */
	ROM_SysCtlPeripheralEnable(ulCEPeriph);
	ROM_GPIOPinTypeGPIOOutput(g_ulCEBase, g_ulCEPin);
	
	_NRF24L01_CELow();

	/* PS: Configure the CSN pin to be GPIO output */
	ROM_SysCtlPeripheralEnable(ulCSNPeriph);
	ROM_GPIOPinTypeGPIOOutput(ulCSNBase, ulCSNPin);

	_NRF24L01_CSNHigh();

	NRF24L01_RegisterInit();

	internal_states |= INTERNAL_STATE_INIT;
}

#endif


/* PS:
 *
 * Function		: 	NRF24L01_InterruptInit
 *
 * Arguments	:	None
 *
 * Return		: 	None
 *
 * Description	:	Registers the IRQ pin as an interrupt
 *
 */

#ifdef NRF24L01_CONF_INTERRUPT_PIN

#ifdef PART_LM4F120H5QR

void NRF24L01_InterruptInit(unsigned long ulIRQBase,
							unsigned long ulIRQPin,
							unsigned long ulIRQPeriph,
							unsigned long ulInterrupt){

	/* PS: Configure the CE pin to be GPIO output */
	ROM_SysCtlPeripheralEnable(ulIRQPeriph);

	ROM_GPIOPinTypeGPIOInput(ulIRQBase, ulIRQPin);
	ROM_GPIOPinIntClear(ulIRQBase, ulIRQPin);
	ROM_GPIOIntTypeSet(ulIRQBase, ulIRQPin, GPIO_LOW_LEVEL);
	ROM_GPIOPinIntEnable(ulIRQBase, ulIRQPin);

	ROM_IntEnable(ulInterrupt);
	ROM_IntMasterEnable();
}
#endif

#endif

/* PS:
 *
 * Function		: 	NRF24L01_RegisterInit
 *
 * Arguments	:	None
 *
 * Return		: 	None
 *
 * Description	:	This function will reset the CE pin and reset all the registers.
 *
 */

void
NRF24L01_RegisterInit()
{
	unsigned char ucRxAddr1[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	unsigned char ucRxAddr2[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
	
	NRF24L01_FlushTX();
	NRF24L01_FlushRX();

	_NRF24L01_CELow();

	NRF24L01_RegisterWrite_8(RF24_CONFIG,0x09);
	NRF24L01_RegisterWrite_8(RF24_EN_AA,0x3F);
	NRF24L01_RegisterWrite_8(RF24_EN_RXADDR,0x03);
	NRF24L01_RegisterWrite_8(RF24_SETUP_AW,0x03);
	NRF24L01_RegisterWrite_8(RF24_SETUP_RETR,0x03);
	NRF24L01_RegisterWrite_8(RF24_RF_CH,0x02);
	NRF24L01_RegisterWrite_8(RF24_RF_SETUP,0x0F);
	NRF24L01_RegisterWrite_8(RF24_STATUS,0x70);
	NRF24L01_RegisterWrite_8(RF24_CD, 0x00);
	NRF24L01_RegisterWrite_Multi(RF24_RX_ADDR_P0,ucRxAddr1,5);
	NRF24L01_RegisterWrite_Multi(RF24_RX_ADDR_P1,ucRxAddr2,5);
	NRF24L01_RegisterWrite_8(RF24_RX_ADDR_P2,0xC3);
	NRF24L01_RegisterWrite_8(RF24_RX_ADDR_P3,0xC4);
	NRF24L01_RegisterWrite_8(RF24_RX_ADDR_P4,0xC5);
	NRF24L01_RegisterWrite_8(RF24_RX_ADDR_P5,0xC6);
	NRF24L01_RegisterWrite_Multi(RF24_TX_ADDR,ucRxAddr1,5);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P0,0x00);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P1,0x00);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P2,0x00);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P3,0x00);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P4,0x00);
	NRF24L01_RegisterWrite_8(RF24_RX_PW_P5,0x00);
	NRF24L01_RegisterWrite_8(RF24_DYNPD,0x00);
	NRF24L01_RegisterWrite_8(RF24_FEATURE,0x00);
}

/* PS:
 * 
 * Function		: 	NRF24L01_GetStatus
 * 
 * Arguments	: 	None.
 * 
 * Return		: 	Status register value.
 * 
 * Description	: 	Gets the status register of the module
 * 
 */

unsigned char 
NRF24L01_GetStatus()
{
	NRF24L01_RegisterRead_8(RF24_NOP);
	return g_ucStatus;
}

/* PS:
 * 
 * Function		: 	NRF24L01_SetAirDataRate
 * 
 * Arguments	: 	ucDataRate	:	1 for 1 Mbps
 * 									2 for 2 Mbps
 * 
 * Return		: 	None
 * 
 * Description	: 	Sets the air data rate (default 2Mbps)
 * 
 */
 
void
NRF24L01_SetAirDataRate(unsigned char ucDataRate)
{
	unsigned char ucCurrentVal = NRF24L01_RegisterRead_8(RF24_RF_SETUP);

	ucDataRate--;	// 0 for 1 Mbps, 1 for 2 Mbps

	if(ucDataRate)
	{
		ucCurrentVal |= (ucDataRate << 3);
	}else
	{
		ucCurrentVal &= ~(ucDataRate << 3);
	}
	
	NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
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
	NRF24L01_RegisterWrite_8(RF24_RF_CH, ucRFChannel);
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
 * 					0 = 0 dBm (default)
 * 					-6 = -6 dBm
 * 					-12 = -12 dBm
 * 					-18 = -18 dBm
 * 
 */
 
void
NRF24L01_SetPAGain(int iPAGain)
{
	unsigned char ucCurrentVal = NRF24L01_RegisterRead_8(RF24_RF_SETUP);

	if(iPAGain < -18)
	{
		iPAGain = -18;
	}else if(iPAGain > 0)
	{
		iPAGain = 0;
	}

	 /* PS:
	  * 				11 = 0 dBm (default)
	  *					10 = -6 dBm
	  *					01 = -12 dBm
	  *					00 = -18 dBm
	  */

	iPAGain = 3 - (-1*(iPAGain) / 6);

	if(ucCurrentVal)
	{
		ucCurrentVal |= ((iPAGain && 0x03) << 1);
	}else
	{
		ucCurrentVal &= ~((iPAGain && 0x03) << 1);
	}
	
	NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
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
	unsigned char ucCurrentVal = NRF24L01_RegisterRead_8(RF24_RF_SETUP);
	
	if(ucCurrentVal)
	{
		ucCurrentVal |= (ucLNAGain << 0);
	}else
	{
		ucCurrentVal &= ~(ucLNAGain << 0);
	}

	NRF24L01_RegisterWrite_8(RF24_RF_SETUP, ucCurrentVal);
}


/* PS:
 *
 * Function		: 	NRF24L01_SetARC
 *
 * Arguments	: 	val			:	Minimum is 0(disbaled), Maximum is 15.
 *
 * Return		: 	None
 *
 * Description	: 	Set automatic retransmission count.
 *
 */
void NRF24L01_SetARC(unsigned char val){
	unsigned char cur_val = 0;

	if(val > 15){
		val = 15;
	}

	val &= 0x0F;

	cur_val = NRF24L01_RegisterRead_8(RF24_SETUP_RETR);
	cur_val &= 0xF0;
	cur_val |= val;
	NRF24L01_RegisterWrite_8(RF24_SETUP_RETR, cur_val);
}


/* PS:
 *
 * Function		: 	NRF24L01_SetARD
 *
 * Arguments	: 	usVal		:	Minimum is 250, Maximum is 4000. (in microseconds)
 *
 * Return		: 	None
 *
 * Description	: 	Value should be a multiple of 250. If the input value is not
 * 					a multiple of 250. It will be rounded up to the nearest 250 multiple.
 *
 */
void NRF24L01_SetARD(unsigned short usVal){

	unsigned char reg_val = 0;
	unsigned char cur_val = 0;

	if(usVal < 250){
		usVal = 250;
	}

	// 250 - 0x0000
	// 500 - 0x0001
	// ...
	// 4000 - 0x1111
	usVal = ((usVal / 250) - 1);

	reg_val = ((usVal << 4) & 0xF0);

	cur_val = NRF24L01_RegisterRead_8(RF24_SETUP_RETR);
	cur_val &= 0x0F;
	cur_val |= reg_val;
	NRF24L01_RegisterWrite_8(RF24_SETUP_RETR, cur_val);
}


/* PS:
 *
 * Function		: 	NRF24L01_SetAddressWidth
 *
 * Arguments	: 	ucVal		:	Minimum 3, Maximum 5
 *
 * Return		: 	None
 *
 * Description	: 	Set the address width for TX/RX address
 *
 */
void NRF24L01_SetAddressWidth(unsigned char ucVal){
	unsigned char ucWidth = 3;

	if(ucVal > 5){
		ucWidth = 5;
	}else if(ucVal < 3){
		ucWidth = 3;
	}else{
		ucWidth = ucVal;
	}

	// 3 - 0x01
	// 4 - 0x10
	// 5 - 0x11

	ucWidth -= 2;

	NRF24L01_RegisterWrite_8(RF24_SETUP_AW, ucWidth);
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
	unsigned char ucCurrentVal = NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal &= (~RF24_PWR_UP);
	
	NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);
	
	_NRF24L01_CELow();

	internal_states &= (~INTERNAL_STATE_POWER_UP);
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
	unsigned char ucCurrentVal = NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal |= (RF24_PWR_UP);
	NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);

	internal_states |= INTERNAL_STATE_POWER_UP;
}


/* PS:
 * 
 * Function		: 	NRF24L01_FlushTX
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Flush tx.
 * 
 */
 
void
NRF24L01_FlushTX()
{
	NRF24L01_SendCommand(RF24_FLUSH_TX, NULL, 0);
#ifdef PDLIB_DEBUG
	PrintString("TX buffer flushed \n\r");
#endif
}


/* PS:
 * 
 * Function		: 	NRF24L01_FlushRX
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Flush rx.
 * 
 */
 
void
NRF24L01_FlushRX()
{
	NRF24L01_SendCommand(RF24_FLUSH_RX, NULL, 0);
}


/* PS:
 * 
 * Function		: 	NRF24L01_EnableRxMode
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Put the module into RX mode and powers up the module
 * 
 */

void
NRF24L01_EnableRxMode()
{
	unsigned char ucCurrentVal = NRF24L01_GetStatus();

	NRF24L01_PowerUp();

	// PS: Clear RX_DR interrupt TODO: Why?
	NRF24L01_ClearInterruptFlag(PDLIB_INTERRUPT_DATA_READY);

	ucCurrentVal = NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal |= (RF24_PRIM_RX | RF24_PWR_UP);
	
	NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);
	
	_NRF24L01_CEHigh();
}


/* PS:
 *
 * Function		: 	NRF24L01_DisableRxMode
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	Put the module into Standby I state with RX mode. Inline function.
 *
 */
void NRF24L01_DisableRxMode()
{
	_NRF24L01_CELow();
}



/* PS:
 * 
 * Function		: 	NRF24L01_EnableTxMode
 * 
 * Arguments	: 	None
 * 
 * Return		: 	None
 * 
 * Description	: 	Put the module into TX mode (Standby II state)
 * 
 */

void
NRF24L01_EnableTxMode()
{
	unsigned char ucCurrentVal = 0;

	// PS: Power up the device
	NRF24L01_PowerUp();

	// PS: Clear TX_DS and MAX_RT interrupts TODO: why?
	NRF24L01_ClearInterruptFlag(PDLIB_INTERRUPT_MAX_RT | PDLIB_INTERRUPT_DATA_SENT);

	// PS: Set to TX mode
	ucCurrentVal = NRF24L01_RegisterRead_8(RF24_CONFIG);
	ucCurrentVal &= (~RF24_PRIM_RX);
	
	NRF24L01_RegisterWrite_8(RF24_CONFIG, ucCurrentVal);

	_NRF24L01_CEHigh();

#ifdef PDLIB_DEBUG
	PrintString("TX mode enabled\n\r");
#endif
}

/* PS:
 *
 * Function		: 	NRF24L01_DisableTxMode
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	Put the module into Standby I state with TX mode.
 *
 */
void NRF24L01_DisableTxMode()
{
	unsigned char ucCurrentVal = NRF24L01_GetStatus();

	_NRF24L01_CELow();

	// PS: Clear TX_DS and MAX_RT interrupts TODO: why?
	NRF24L01_ClearInterruptFlag(PDLIB_INTERRUPT_MAX_RT | PDLIB_INTERRUPT_DATA_SENT);

#ifdef PDLIB_DEBUG
	PrintString("TX mode disabled\n\r");
#endif
}


/* PS:
 * 
 * Function		: 	NRF24L01_IsDataReadyRx
 * 
 * Arguments	: 	pcPipeNo [out] : Pipe number which contains the RX payload
 * 
 * Return		: 	PDLIB_NRF24_INVALID_ARGUMENT	:	Invalid input argument
 * 					PDLIB_NRF24_ERROR				:	Data is not in RX FIFO
 * 					PDLIB_NRF24_SUCCESS				:	Data is in RX FIFO
 * 
 * Description	: 	Get the state of RX data.
 * 					If the return is 'PDLIB_NRF24_SUCCESS' then the pcPipeNo will contain the pipe which has data.
 * 
 */
 
int
NRF24L01_IsDataReadyRx(char *pcPipeNo)
{
	int ret = PDLIB_NRF24_ERROR;
	char cDataReady;

	if(NULL == pcPipeNo)
	{
		ret = PDLIB_NRF24_INVALID_ARGUMENT;
	}else
	{
		NRF24L01_GetStatus();

		cDataReady = ((g_ucStatus & RF24_RX_DR) ? 1 : 0);
		*pcPipeNo = ((g_ucStatus & ( BIT3 | BIT2 | BIT1)) >> 1);

		// *pcPipeNo will be 7 if the RX fifo is empty
		if(cDataReady && (*pcPipeNo < 6))
		{
			ret = PDLIB_NRF24_SUCCESS;
		}else
		{
			// Reset the value in the pipe number
			*pcPipeNo = 0xFF;
			ret = PDLIB_NRF24_ERROR;
		}
	}

	return ret;
}


/* PS:
 *
 * Function		: 	NRF24L01_WaitForDataRx
 *
 * Arguments	: 	pcPipeNo [out] : Pipe number which contains the RX payload
 *
 * Return		: 	PDLIB_NRF24_ERROR	:	Invalid input argument
 * 					PDLIB_NRF24_SUCCESS	:	Data is in RX FIFO
 *
 * Description	: 	Wait until any RX pipe has data
 *
 */

int NRF24L01_WaitForDataRx(char *pcPipeNo)
{
	int iRet = PDLIB_NRF24_ERROR;

	NRF24L01_EnableRxMode();

#ifdef PDLIB_DEBUG
	PrintString("Waiting for data...\n\r");

	/*
		PrintRegValue("CONFIG: ",NRF24L01_RegisterRead_8(RF24_CONFIG));
		PrintRegValue("EN_RXADDR: ",NRF24L01_RegisterRead_8(RF24_EN_RXADDR));
		PrintRegValue("EN_AA: ",NRF24L01_RegisterRead_8(RF24_EN_AA));
		PrintRegValue("RX_PW_P0: ",NRF24L01_RegisterRead_8(RF24_RX_PW_P0));*/
#endif


	while(iRet == PDLIB_NRF24_ERROR)
	{
		iRet = NRF24L01_IsDataReadyRx(pcPipeNo);
	}


	NRF24L01_DisableRxMode();

	return iRet;
}



/* PS:
 *
 * Function		: 	NRF24L01_IsTxFifoFull
 *
 * Arguments	: 	None
 *
 * Return		: 	0	:	TX FIFO is NOT full
 * 					1	:	TX FIFO is full
 *
 * Description	: 	Get the state of TX FIFO
 *
 */

int
NRF24L01_IsTxFifoFull()
{
	unsigned char ucTxFifo;

	ucTxFifo = NRF24L01_RegisterRead_8(RF24_FIFO_STATUS);

	return ((ucTxFifo & RF24_FIFO_FULL) ? 1 : 0);
}


/* PS:
 *
 * Function		: 	NRF24L01_IsTxFifoEmpty
 *
 * Arguments	: 	None
 *
 * Return		: 	0	:	TX FIFO is NOT empty
 * 					1	:	TX FIFO is empty
 *
 * Description	: 	Get the state of TX FIFO
 *
 */

int NRF24L01_IsTxFifoEmpty()
{
	unsigned char ucTxFifo;
	ucTxFifo = NRF24L01_RegisterRead_8(RF24_FIFO_STATUS);

	return ((ucTxFifo & RF24_TX_EMPTY) ? 1 : 0);
}


/* PS:
 * 
 * Function		: 	NRF24L01_GetRxDataAmount
 * 
 * Arguments	: 	ucDataPipe	:	Index of the pipe
 * 
 * Return		: 	Available number of bytes.
 * 
 * Description	: 	Reading the number of data bytes available
 * 					in the specified pipe (for static payload mode).
 *
 * 					For dynamic payload mode this will tell
 * 					the top most RX_FIFO payload length.
 * 
 */

char
NRF24L01_GetRxDataAmount(unsigned char ucDataPipe)
{
	char reg;
	char ret = 0;

	if((INTERNAL_STATE_DYNPL & internal_states) == 0){
		if(ucDataPipe < 6)
		{
			reg = NRF24L01_RegisterRead_8(RF24_RX_PW_P0 + ucDataPipe);
			ret = (reg & 0x3F);
		}else
		{
			ret = 0;
		}
	}else{
		NRF24L01_SendRcvCommand(RF24_R_RX_PL_WID, &reg, 1);

		ret = reg;
	}

	return ret;
}
	
	
/* PS:
 * 
 * Function		: 	NRF24L01_SetTXAddress
 * 
 * Arguments	: 	address	:	Buffer which contains the five bytes to put to address.
 * 
 * Return		: 	None
 * 
 * Description	: 	Set the address of the module for TX mode. The data
 * 					packet which is transmitted from this RF module will
 * 					contain this address as destination address.
 * 
 */
 
void 
NRF24L01_SetTXAddress(unsigned char* address)
{
	NRF24L01_RegisterWrite_Multi(RF24_TX_ADDR, (unsigned char*)address, 5);
}

/* PS:
 *
 * Function		: 	NRF24L01_WaitForTxComplete
 *
 * Arguments	: 	busy_wait :	If this is 1 then the function will wait until DS or RT
 * 								bit in status register become '1'.
 *
 * 								If this is '0' the state of DS and RT interrupts will return.
 *
 * Return		: 	PDLIB_NRF24_SUCCESS			:	TX completed successfully
 *					PDLIB_NRF24_TX_ARC_REACHED	:	Maximum retransmissions elapsed
 *					PDLIB_NRF24_ERROR			:	None of the TX interrupts asserted (only when busy_wait = 0)
 *
 * Description	: 	The function can wait until the
 * 						-TX payload is successfully delivered (If ACK is available)
 * 						or
 * 						-TX payload has successfully transmitted.
 *
 *					This function will return 0 or a negative error code.
 */

int
NRF24L01_WaitForTxComplete(char busy_wait)
{
	int ret = PDLIB_NRF24_SUCCESS;

	NRF24L01_GetStatus();

#ifdef PDLIB_DEBUG
	PrintRegValue("Current status :",g_ucStatus);
#endif

	if(busy_wait){
		while((g_ucStatus & (RF24_MAX_RT | RF24_TX_DS)) == 0)
		{
			NRF24L01_GetStatus();
		}
	}else{
		if((g_ucStatus & (RF24_MAX_RT | RF24_TX_DS)) == 0){
			ret = PDLIB_NRF24_ERROR;
		}
	}


	if(g_ucStatus & RF24_MAX_RT)
	{
#ifdef PDLIB_DEBUG
		PrintRegValue("Maximum retransmissions reached!!! : status >> ",g_ucStatus);
#endif
		ret = PDLIB_NRF24_TX_ARC_REACHED;
	}

	return ret;
}


/* PS:
 *
 * Function		: 	NRF24L01_GetInterruptState
 *
 * Arguments	: 	None
 *
 * Return		: 	PDLIB_NRF24_ERROR			:	No interrupts asserted
 *					PDLIB_INTERRUPT_MAX_RT		:	Maximum retransmissions elapsed
 *					PDLIB_INTERRUPT_DATA_READY	:	Data ready on RX FIFO
 *					PDLIB_INTERRUPT_DATA_SENT	:	Data sent
 *
 * Description	: 	This function can be used to check the interrupt status
 * 					of the module.
 */

char
NRF24L01_GetInterruptState()
{
	char state = NRF24L01_GetStatus();

	state &= (RF24_RX_DR | RF24_TX_DS | RF24_MAX_RT);
	state = (state >> 4);

	return state;

}


/* PS:
 *
 * Function		: 	NRF24L01_GetInterruptState
 *
 * Arguments	: 	PDLIB_INTERRUPT_MAX_RT		:	Maximum retransmissions elapsed interrupt
 *					PDLIB_INTERRUPT_DATA_READY	:	Data ready on RX FIFO interrupt
 *					PDLIB_INTERRUPT_DATA_SENT	:	Data sent interrupt
 *
 * Return		:	None
 *
 * Description	: 	This function will clear the interrupt associated with the BM
 */

void
NRF24L01_ClearInterruptFlag(char interrupt_bm)
{
	char status = NRF24L01_GetStatus();

	status &= ~(RF24_RX_DR | RF24_TX_DS | RF24_MAX_RT);

	if(interrupt_bm & PDLIB_INTERRUPT_MAX_RT){
		status |= RF24_MAX_RT;
	}

	if(interrupt_bm & PDLIB_INTERRUPT_DATA_READY){
		status |= RF24_RX_DR;
	}

	if(interrupt_bm & PDLIB_INTERRUPT_DATA_SENT){
		status |= RF24_TX_DS;
	}

	NRF24L01_RegisterWrite_8(RF24_STATUS, status);
}



/* PS:
 *
 * Function		: 	NRF24L01_AttemptTx
 *
 * Arguments	: 	None
 *
 * Return		: 	PDLIB_NRF24_SUCCESS			:	TX completed successfully
 *					PDLIB_NRF24_TX_ARC_REACHED	:	Maximum retransmissions elapsed
 *
 * Description	: 	This function will attempt to TX whatever data available in the
 * 					TX payload. It waits until the TX is done or maximum retransmissions
 * 					occur.
 *
 * 					Module will be in Power Down mode when it returns.
 */

int NRF24L01_AttemptTx()
{
	int ret = PDLIB_NRF24_SUCCESS;

#ifdef PDLIB_DEBUG
	PrintString("Attempting TX...\n\r");
#endif

	NRF24L01_EnableTxMode();

	ret = NRF24L01_WaitForTxComplete(1);

	NRF24L01_DisableTxMode();

	NRF24L01_PowerDown();

	return ret;
}

/* PS:
 * 
 * Function		: 	NRF24L01_SetRXAddress
 * 
 * Arguments	: 	ucDataPipe	:	Data pipe number
 * 					pucAddress	:	Buffer which contains the one/five bytes to put to address.
 * 
 * Return		: 	None
 * 
 * Description	: 	Set the RX address. P0 and P1 pipes have 5 byte address
 * 					other pipes have 1 byte address(LSB). Other bytes are taken from
 * 					the P1 pipe address.
 * 
 */
 
void 
NRF24L01_SetRxAddress(	unsigned char ucDataPipe,
						unsigned char *pucAddress)
{
	if(pucAddress)
	{
		switch(ucDataPipe)
		{
			case 0:
			case 1:
				NRF24L01_RegisterWrite_Multi((RF24_RX_ADDR_P0 + ucDataPipe), pucAddress, 5);
				break;
			case 2:
			case 3:
			case 4:
			case 5:
				NRF24L01_RegisterWrite_8((RF24_RX_ADDR_P0 + ucDataPipe),pucAddress[0]);
				break;
			default:
				break;
		}
	}
}


/* PS:
 * 
 * Function		: 	NRF24L01_SetRXPacketSize
 *
 * Arguments	: 	ucDataPipe		:	Data pipe number
 * 					ucPacketSize	:	Packet size. (Maximum is 32)
 *
 * Return		: 	None
 *
 * Description	: 	Set the packet size for a RX pipe.
 *
 */

void
NRF24L01_SetRXPacketSize(	unsigned char ucDataPipe,
							unsigned char ucPacketSize)
{
	if(ucPacketSize < 32)
	{
		NRF24L01_RegisterWrite_8((RF24_RX_PW_P0 + ucDataPipe), ucPacketSize);
	}

}


/* PS:
 *
 * Function		: 	NRF24L01_CarrierDetect
 *
 * Arguments	: 	None
 *
 * Return		: 	'1' - There is a carrier signal
 * 					'0' - There is no carrier signal
 *
 * Description	: 	Check whether there is any RF carrier in the current frequency channel
 *
 */

unsigned char NRF24L01_CarrierDetect(){
	unsigned char ucTemp = NRF24L01_RegisterRead_8(RF24_CD);
	return (ucTemp & 0x01);
}


/* PS:
 *
 * Function		: 	NRF24L01_SetTxPayload
 * 
 * Arguments	: 	pucData		:	Buffer which contains the data to be written to TX fifo
 * 					uiLength	:	Length of the data buffer
 * 
 * Return		: 	PDLIB_NRF24_TX_FIFO_FULL 	: Tx FIFO full
 * 					PDLIB_NRF24_SUCCESS			: Success
 * 
 * Description	: 	Set the TX payload. 
 * 
 */
 
int
NRF24L01_SetTxPayload(	char* pcData,
						unsigned int uiLength)
{
	int ret = PDLIB_NRF24_SUCCESS;

	if(pcData && uiLength > 0)
	{
		// PS: Check whether TX fifo is full
		if(NRF24L01_IsTxFifoFull())
		{
			ret = PDLIB_NRF24_TX_FIFO_FULL;
#ifdef PDLIB_DEBUG
			PrintString("TX FIFO is full\n\r");
#endif
		}else
		{
			NRF24L01_SendCommand(RF24_W_TX_PAYLOAD, pcData, uiLength);
		}
	}else
	{
		ret = PDLIB_NRF24_ERROR;
	}


	return ret;
}


/* PS:
 *
 * Function		: 	NRF24L01_EnableFeatureAckPL
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	Enable payload with ACK
 *
 */

void
NRF24L01_EnableFeatureAckPL()
{
	char data = 0x73;

	if((internal_states & INTERNAL_STATE_STAND_BY) || (0 == (internal_states & INTERNAL_STATE_POWER_UP)))
	{
		/* PS: Enable dynpl for pipe0 */
		NRF24L01_EnableFeatureDynPL(0x00);

		/* PS: Check whether retransmission delay is sufficient */
		data = NRF24L01_RegisterRead_8(RF24_SETUP_RETR);

		if(0 == ((data & 0xF0) >> 4)){
			NRF24L01_SetARD(500);
		}

		/* PS: Enable auto ack payload */
		data = NRF24L01_RegisterRead_8(RF24_FEATURE);
		NRF24L01_RegisterWrite_8(RF24_FEATURE, (data | RF24_EN_ACK_PAY));

		internal_states |= INTERNAL_STATE_ACKPL;
	}
}


/* PS:
 *
 * Function		: 	NRF24L01_EnableFeatureDynPL
 *
 * Arguments	: 	pipe : Pipe number to enable the feature
 *
 * Return		: 	None
 *
 * Description	: 	Enable dynamic payload
 *
 */

void
NRF24L01_EnableFeatureDynPL(unsigned char pipe)
{
	char data = 0x73;

	if((internal_states & INTERNAL_STATE_STAND_BY) || (0 == (internal_states & INTERNAL_STATE_POWER_UP)))
	{
		/* PS: Check whether features register is activated */
		if(0 == (internal_states & INTERNAL_STATE_FEATURE_ENABLED))
		{
			char data = 0x73;
			NRF24L01_SendCommand(RF24_ACTIVATE, &data, 1);

			internal_states |= INTERNAL_STATE_FEATURE_ENABLED;
		}

		/* PS: Check whether DYN-PL feature is activated */
		data = NRF24L01_RegisterRead_8(RF24_FEATURE);

		if(0 == (data & RF24_EN_DPL)){
			NRF24L01_RegisterWrite_8(RF24_FEATURE, (data | RF24_EN_DPL));
		}

		if(pipe <= 6)
		{
			/* PS: Check whether DYN-PD for 'pipe' is activated */
			data = NRF24L01_RegisterRead_8(RF24_DYNPD);

			if(0 == (data & pipe)){
				NRF24L01_RegisterWrite_8(RF24_DYNPD, (data | (1 << pipe)));
			}
		}

		internal_states |= INTERNAL_STATE_DYNPL;
	}
}


/* PS:
 *
 * Function		: 	NRF24L01_EnableFeatureNoAckTx
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	Enable W_TX_PAYLOAD_NOACK command
 *
 */


void
NRF24L01_EnableFeatureNoAckTx()
{
	char data = 0x73;

	if((internal_states & INTERNAL_STATE_STAND_BY) || (0 == (internal_states & INTERNAL_STATE_POWER_UP)))
	{
		/* PS: Check whether features register is activated */
		if(0 == (internal_states & INTERNAL_STATE_FEATURE_ENABLED))
		{
			char data = 0x73;
			NRF24L01_SendCommand(RF24_ACTIVATE, &data, 1);

			internal_states |= INTERNAL_STATE_FEATURE_ENABLED;
		}

		/* PS: Check whether DYN-PL feature is activated */
		data = NRF24L01_RegisterRead_8(RF24_FEATURE);

		if(0 == (data & RF24_EN_DYN_ACK)){
			NRF24L01_RegisterWrite_8(RF24_FEATURE, (data | RF24_EN_DYN_ACK));
		}
	}
}


// PS: Implement TX reuse feature
		// TODO

/* PS:
 *
 * Function		: 	NRF24L01_GetData
 *
 * Arguments	:	pipe				:	Pipe number
 * 					pcData [out]		:	Allocated buffer to store the RX data
 * 					length	[in/out]	:	Length in bytes of pcData (only used in dynamic payload mode) // TODO
 *
 * Return		:
 * 					Positive	:	Number of bytes read
 *
 * Description	:
 * 					This function will read the length amount of data from the RX FIFO
 * 					If the available data amount is more than the specified value this
 * 					will return an error.
 *
 * 					Payload is automatically deleted from the FIFO once it is read.
 *
 * 					This will clear the data ready status bit in the module.
 *
 */

int
NRF24L01_GetData(	char pipe,
					char* pcData,
					char *length)
{
	int ret = PDLIB_NRF24_SUCCESS; // Amount of data read
	char cFifoStatus;
	char cTemp;

	// Check whether dynamic payload is available TODO

	if(NULL == pcData)
	{
		ret = PDLIB_NRF24_INVALID_ARGUMENT;
	}else{

		// PS: Validate pipe, Check FIFO status, Get packet size, Read data
		if(PDLIB_NRF24_SUCCESS == NRF24L01_IsDataReadyRx(&cTemp)){

			if(pipe == cTemp){
				// Get FIFO status
				cFifoStatus = NRF24L01_RegisterRead_8(RF24_FIFO_STATUS);

				// Check whether FIFO is empty
				if(0 == (cFifoStatus & RF24_RX_EMPTY)){

					// PS: Check whether data is available
					cTemp = NRF24L01_GetRxDataAmount(pipe);

					if((*length) >= cTemp){
						(*length) = cTemp;
					}else{
						// PS: If the actual buffer size is smaller than the available data, we can miss data.
						ret = PDLIB_NRF24_BUFFER_TOO_SMALL;
					}

					if(PDLIB_NRF24_SUCCESS == ret){

						// PS: Read RX payload
						NRF24L01_ReadRxPayload(pcData, cTemp);

						// TODO: Check whether we need to check the actual FIFO state before clearing the interrupt.
						// Clear RX_DR
						NRF24L01_ClearInterruptFlag(PDLIB_INTERRUPT_DATA_READY);

						ret = cTemp;
					}

				}else{
					ret = PDLIB_NRF24_ERROR;
				}
			}else{
				ret = PDLIB_NRF24_INVALID_ARGUMENT;
			}
		}else{
			ret = PDLIB_NRF24_ERROR;
		}
	}

	return ret;
}

/* PS:
 *
 * Function		: 	NRF24L01_ReadRxPayload
 *
 * Arguments	:	pcData [out]	:	Allocated buffer to store the RX data
 * 					cLength	[in]	:	Required data amount. This should be a valid data amount. (TODO: Verify whether this is neccessary to be valid)
 *
 * Return		:	None
 *
 * Description	:
 * 					This function will read the cLength amount of data from the RX FIFO.
 *
 * 					This function will not perform any validation of data sizes, buffer size, etc.
 *
 * 					Payload is automatically deleted from the FIFO once it is read.
 *
 */

void
NRF24L01_ReadRxPayload(	char* pcData,
						char cLength)
{
	NRF24L01_SendRcvCommand(RF24_R_RX_PAYLOAD, pcData, cLength);
}
 

/* PS:
 *
 * Function		: 	NRF24L01_SetAckPayload
 *
 * Arguments	: 	pucData		:	Buffer which contains the data to be written to TX fifo
 * 					pipe		:	Which pipe to use (0~5)
 * 					uiLength	:	Length of the data buffer
 *
 * Return		: 	PDLIB_NRF24_TX_FIFO_FULL 	: Tx FIFO full
 * 					PDLIB_NRF24_SUCCESS			: Success
 *
 * Description	: 	Set the Ack payload.
 *
 */

int
NRF24L01_SetAckPayload(	char* pcData,
						char pipe,
						unsigned int uiLength)
{
	int ret = PDLIB_NRF24_SUCCESS;
	char address = pipe;

	if(pipe < 5 && pcData && uiLength > 0)
	{
		// PS: Check whether TX fifo is full
		if(NRF24L01_IsTxFifoFull())
		{
			ret = PDLIB_NRF24_TX_FIFO_FULL;
#ifdef PDLIB_DEBUG
			PrintString("TX FIFO is full\n\r");
#endif
		}else
		{
			address = address & 0x07;
			address |= RF24_W_ACK_PAYLOAD;

			NRF24L01_SendCommand(address , pcData, uiLength);
		}
	}else
	{
		ret = PDLIB_NRF24_ERROR;
	}


	return ret;
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

char
NRF24L01_GetAckDataAmount()
{
	char data_amount = 0;

	NRF24L01_SendRcvCommand(RF24_R_RX_PL_WID,&data_amount,1);

	return data_amount;
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


// ----------------  Normal user level functions ------------------ //

/* PS:
 *
 * Function		: 	NRF24L01_SendData
 *
 * Arguments	: 	pcData		:	Data packet to send
 * 					ulLength	:	Length of the packet
 *
 * Return		:	PDLIB_NRF24_SUCCESS			: Success
 * 					PDLIB_NRF24_TX_FIFO_FULL 	: Tx FIFO full
 *					PDLIB_NRF24_TX_ARC_REACHED	: Maximum retransmissions elapsed
 *
 * Description	: 	The function will send the specified data using
 * 					current configuration. (current air data rate,
 * 					current power, current TX address, ... )
 *
 * 					The function will return after the data is successfully transmitted or
 * 					maximum retransmissions were done. The module will be in Power Down state
 * 					when this function returns.
 *
 */

int NRF24L01_SendData(char *pcData, unsigned int uiLength)
{
	int ret;

	ret = NRF24L01_SubmitData(pcData, uiLength);

	if(ret == PDLIB_NRF24_SUCCESS)
	{

		ret = NRF24L01_AttemptTx();
	}

	return ret;
}


/* PS:
 *
 * Function		: 	NRF24L01_SubmitData
 *
 * Arguments	: 	pcData		:	Data packet to send
 * 					ulLength	:	Length of the packet
 *
 * Return		:	PDLIB_NRF24_SUCCESS			: Success
 * 					PDLIB_NRF24_TX_FIFO_FULL 	: Tx FIFO full
 *
 * Description	: 	The function will send the specified data using
 * 					current configuration. (current air data rate,
 * 					current power, current TX address, ... )
 *
 * 					This function will write data to payload buffer and make
 * 					sure correct addresses are there for automatic ack if available.
 *
 * 					The module needs to be Powered-Up and put into TX mode in order
 * 					to perform the transmission.
 *
 */

int NRF24L01_SubmitData(char *pcData, unsigned int uiLength)
{
	int ret;
	unsigned char address[5];
	unsigned char cTemp;

	// PS: Check the Auto Ack feature and make sure the data pipe 0 has the correct PTX address
	cTemp = NRF24L01_RegisterRead_8(RF24_EN_AA);

	if(cTemp & RF24_ENAA_P0){
		NRF24L01_RegisterRead_Multi(RF24_TX_ADDR, address, 5);
		NRF24L01_SetRxAddress(PDLIB_NRF24_PIPE0, address);
	}

	ret = NRF24L01_SetTxPayload(pcData, uiLength);

	return ret;
}

/* PS:
 *
 * Function		: 	NRF24L01_SendDataTo
 *
 * Arguments	: 	pcData		:	Data packet to send
 * 					ulLength	:	Length of the packet
 * 					pcTxAddr	:	TX address
 *
 * Return		:	PDLIB_NRF24_SUCCESS			: Success
 * 					PDLIB_NRF24_TX_FIFO_FULL 	: Tx FIFO full
 *					PDLIB_NRF24_TX_ARC_REACHED	: Maximum retransmissions elapsed
 *
 * Description	: 	The function will send the specified data using
 * 					current configuration but to the specified address. (current air data rate,
 * 					current power, ... )
 *
 * 					The function will return after the data is successfully transmitted or
 * 					maximum retransmissions were done.
 *
 * 					This function will change the TX address in the module.
 *
 */

int NRF24L01_SendDataTo(unsigned char *address, char *pcData, unsigned int uiLength)
{
	int iRet;

	NRF24L01_SetTXAddress(address);

	iRet = NRF24L01_SendData(pcData, uiLength);

	return iRet;
}





// ----------------------- Register Read/Write functions ---------------------- //


/* PS:
 * 
 * Function		: 	NRF24L01_RegisterWrite_8
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
 
void
NRF24L01_RegisterWrite_8(unsigned char ucRegister, unsigned char ucValue)
{
	unsigned char ucData[2];
	
	ucData[0] = (RF24_W_REGISTER | ucRegister);
	ucData[1] = ucValue;
	
	_NRF24L01_CSNLow();
	
#ifdef PDLIB_SPI
	/* PS: Send address */
	g_ucStatus = pdlibSPI_TransferByte(ucData[0]);

	/* PS: Send data */
	pdlibSPI_TransferByte(ucData[1]);
#endif

	_NRF24L01_CSNHigh();
}


/* PS:
 * 
 * Function		: 	NRF24L01_RegisterWrite_Multi
 * 
 * Arguments	: 	ucRegister	:	Address of the register
 * 					pucData		:	Value to write to the register
 * 					uiLength	:	Length of the data field
 * 
 * Return		: 	None
 * 
 * Description	: 	This function will write more than one byte of data 
 * 					to an the specified register. 
 * 					Function will update the Status	variable too.
 * 
 */
 
void
NRF24L01_RegisterWrite_Multi(	unsigned char ucRegister,
								unsigned char *pucData,
								unsigned int uiLength)
{
	if(NULL != pucData)
	{
		unsigned char *pucBuffer = (unsigned char*) malloc(sizeof(unsigned char) * (uiLength));
		
		if(NULL != pucBuffer)
		{
			memcpy(pucBuffer, pucData, uiLength);

			_NRF24L01_CSNLow();

#ifdef PDLIB_SPI
			g_ucStatus = pdlibSPI_TransferByte(RF24_W_REGISTER | ucRegister);
			pdlibSPI_SendData(pucBuffer, uiLength);
#endif
			_NRF24L01_CSNHigh();

			free(pucBuffer);
		}
	}
}


/* PS:
 * 
 * Function		: 	NRF24L01_RegisterRead_8
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
 
unsigned char
NRF24L01_RegisterRead_8(unsigned char ucRegister)
{
	unsigned char ucData;

	_NRF24L01_CSNLow();

#ifdef PDLIB_SPI
	g_ucStatus = pdlibSPI_TransferByte(RF24_R_REGISTER | ucRegister);
	
	ucData = pdlibSPI_TransferByte(RF24_NOP);
#endif

	_NRF24L01_CSNHigh();

	return ucData;
}


/* PS:
 *
 * Function		: 	NRF24L01_RegisterRead_Multi
 *
 * Arguments	: 	ucRegister	:	Address of the register
 * 					pucBuffer	: 	Buffer to store the reply
 * 					uiLength	:	Length of the buffer
 *
 * Return		: 	Status register value will be returned.
 *
 * Description	: 	This function will read more than 1 byte of data from a
 * 					register. The function will update the Status
 * 					variable too.
 *
 */

unsigned char
NRF24L01_RegisterRead_Multi(	unsigned char ucRegister,
								unsigned char *pucBuffer,
								unsigned int uiLength)
{
	int i;

	_NRF24L01_CSNLow();

#ifdef PDLIB_SPI
	g_ucStatus = pdlibSPI_TransferByte(RF24_R_REGISTER | ucRegister);

	for(i = 0; i<uiLength; i++)
	{
		pucBuffer[i] = pdlibSPI_TransferByte(RF24_NOP);
	}
#endif

	_NRF24L01_CSNHigh();

	return g_ucStatus;
}


/* PS:
 *
 * Function		: 	NRF24L01_SendCommand
 *
 * Arguments	: 	ucCommand	:	Command to send.
 * 					pucData		:	Data associated with the command
 * 					uiLength	:	Length of the data field
 *
 * Return		: 	None
 *
 * Description	: 	This function will send any command with the data buffer
 * 					to the RF module. This will also update the status register.
 * 					If there is no payload for the command set the pucData NULL and
 * 					make the uiLength as 0
 *
 */

void
NRF24L01_SendCommand(	unsigned char ucCommand,
						char *pcData,
						unsigned int uiLength)
{
	unsigned char *pucBuffer = (unsigned char*) malloc(sizeof(unsigned char) * (uiLength + 1));

	if(NULL != pucBuffer)
	{
		pucBuffer[0] = ucCommand;

		if(NULL != pcData)
		{
			memcpy(&pucBuffer[1], pcData, uiLength);
		}

		_NRF24L01_CSNLow();

#ifdef PDLIB_SPI
		pdlibSPI_SendData(pucBuffer, uiLength+1);
#endif

		_NRF24L01_CSNHigh();

		free(pucBuffer);
	}
}


/* PS:
 *
 * Function		: 	NRF24L01_SendRcvCommand
 *
 * Arguments	: 	ucCommand	:	Command to send.
 * 					pucData		:	Buffer to get data
 * 					uiLength	:	Length of the buffer
 *
 * Return		: 	None
 *
 * Description	: 	This function will send any command with the data buffer
 * 					to the RF module. After that it will read the specifed
 * 					amount of data and store it as the reply in the provided
 * 					buffer.
 *
 */

void NRF24L01_SendRcvCommand(unsigned char ucCommand, char *pcData, unsigned int uiLength)
{
	if(pcData){
		int i = 0;

		_NRF24L01_CSNLow();

#ifdef PDLIB_SPI
		g_ucStatus = pdlibSPI_TransferByte(ucCommand);

		for(i = 0; i < uiLength; i++)
		{
			pcData[i] = pdlibSPI_TransferByte(RF24_NOP);
		}
#endif
		_NRF24L01_CSNHigh();
	}
}


// ----------------  Hardware Pin Control ------------------ //


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
#ifdef PART_LM4F120H5QR
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0x00);
#endif

	if(internal_states & INTERNAL_STATE_POWER_UP){
		internal_states |= INTERNAL_STATE_STAND_BY;
	}else{
		internal_states &= (~INTERNAL_STATE_STAND_BY);
	}
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
#ifdef PART_LM4F120H5QR
	ROM_GPIOPinWrite(g_ulCEBase, g_ulCEPin, 0xFF);
#endif

	if(internal_states & INTERNAL_STATE_POWER_UP){
		internal_states &= (~INTERNAL_STATE_STAND_BY);
	}
}


/* PS:
 *
 * Function		: 	_NRF24L01_CSNLow
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	This function drives the CSN pin low
 *
 */

static void
_NRF24L01_CSNLow()
{
#ifdef PART_LM4F120H5QR
	ROM_GPIOPinWrite(g_ulCSNBase, g_ulCSNPin, 0x00);
#endif
}


/* PS:
 *
 * Function		: 	_NRF24L01_CSNHigh
 *
 * Arguments	: 	None
 *
 * Return		: 	None
 *
 * Description	: 	This function drives the CSN pin high
 *
 */

static void
_NRF24L01_CSNHigh()
{
#ifdef PART_LM4F120H5QR
	ROM_GPIOPinWrite(g_ulCSNBase, g_ulCSNPin, 0xFF);
#endif
}


