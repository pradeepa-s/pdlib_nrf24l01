#ifndef _PDLIB_NRF24L01
#define _PDLIB_NRF24L01

#include "nRF24L01.h"

/* Configurations */

//#define NRF24L01_CONF_INTERRUPT_PIN


#define PDLIB_NRF24_SUCCESS				0
#define PDLIB_NRF24_ERROR				-1
#define PDLIB_NRF24_TX_FIFO_FULL		-2
#define PDLIB_NRF24_TX_ARC_REACHED		-3
#define PDLIB_NRF24_INVALID_ARGUMENT	-4
#define PDLIB_NRF24_BUFFER_TOO_SMALL	-5

#define PDLIB_NRF24_PIPE0	0
#define PDLIB_NRF24_PIPE1	1
#define PDLIB_NRF24_PIPE2	2
#define PDLIB_NRF24_PIPE3	3
#define PDLIB_NRF24_PIPE4	4
#define PDLIB_NRF24_PIPE5	5

#define PDLIB_INTERRUPT_MAX_RT		1 << 0
#define PDLIB_INTERRUPT_DATA_SENT	1 << 1
#define PDLIB_INTERRUPT_DATA_READY	1 << 2

/* PS: Function prototypes */

/* PS: Basic APIs */
void NRF24L01_Init(unsigned long ulCEBase, unsigned long ulCEPin, unsigned long ulCEPeriph, unsigned long ulCSNBase, unsigned long ulCSNPin, unsigned long ulCSNPeriph, unsigned char ucSSIIndex);
int NRF24L01_SendData(char *pcData, unsigned int uiLength);
int NRF24L01_SendDataTo(unsigned char *address, char *pcData, unsigned int uiLength);
int NRF24L01_WaitForDataRx(char *pcPipeNo);
char NRF24L01_GetRxDataAmount(unsigned char ucDataPipe);
int NRF24L01_GetData(char pipe, char* pcData, char *length);

/* Intermediate APIs */
/* PS: Configuration APIs */

void NRF24L01_RegisterInit();

#ifdef NRF24L01_CONF_INTERRUPT_PIN
void NRF24L01_InterruptInit(unsigned long ulIRQBase, unsigned long ulIRQPin, unsigned long ulIRQPeriph, unsigned long ulInterrupt);
#endif

void NRF24L01_PowerDown();
void NRF24L01_PowerUp();
void NRF24L01_SetAirDataRate(unsigned char ucDataRate);
void NRF24L01_SetLNAGain(unsigned char ucLNAGain);
void NRF24L01_SetPAGain(int iPAGain);
void NRF24L01_SetRFChannel(unsigned char ucRFChannel);
void NRF24L01_SetARC(unsigned char ucVal);
void NRF24L01_SetARD(unsigned short ucVal);
void NRF24L01_SetAddressWidth(unsigned char ucVal);
unsigned char NRF24L01_GetStatus();

void NRF24L01_EnableFeatureDynPL(unsigned char pipe);
void NRF24L01_EnableFeatureAckPL();
void NRF24L01_EnableFeatureNoAckTx();
char NRF24L01_GetInterruptState();
void NRF24L01_ClearInterruptFlag(char interrupt_bm);

/* TX mode related */
void NRF24L01_FlushTX();
void NRF24L01_SetTXAddress(unsigned char* address);
int NRF24L01_SetTxPayload(char* pcData, unsigned int uiLength);
int NRF24L01_SubmitData(char *pcData, unsigned int uiLength);
void NRF24L01_EnableTxMode();
void NRF24L01_DisableTxMode();
int NRF24L01_IsTxFifoFull();
int NRF24L01_IsTxFifoEmpty();
int NRF24L01_AttemptTx();
int NRF24L01_WaitForTxComplete(char busy_wait);
char NRF24L01_GetAckDataAmount();

/* RX mode related */
void NRF24L01_FlushRX();
void NRF24L01_SetRxAddress(unsigned char ucDataPipe, unsigned char *pucAddress);
void NRF24L01_SetRXPacketSize(unsigned char ucDataPipe, unsigned char ucPacketSize);
void NRF24L01_EnableRxMode();
void NRF24L01_DisableRxMode();
int NRF24L01_IsDataReadyRx(char *pcPipeNo);
void NRF24L01_ReadRxPayload(char* pcData, char cLength);
int NRF24L01_SetAckPayload(char* pcData, char pipe, unsigned int uiLength);
unsigned char NRF24L01_CarrierDetect();

/* PS: Advanced APIs, Register level access */
unsigned char NRF24L01_RegisterRead_8(unsigned char ucRegister);
unsigned char NRF24L01_RegisterRead_Multi(unsigned char ucRegister, unsigned char *pucBuffer, unsigned int uiLength);
void NRF24L01_RegisterWrite_8(unsigned char ucRegister, unsigned char ucValue);
void NRF24L01_RegisterWrite_Multi(unsigned char ucRegister, unsigned char *pucData, unsigned int uiLength);
void NRF24L01_SendCommand(unsigned char ucCommand, char *pcData, unsigned int uiLength);
void NRF24L01_SendRcvCommand(unsigned char ucCommand, char *pcData, unsigned int uiLength);

#endif

