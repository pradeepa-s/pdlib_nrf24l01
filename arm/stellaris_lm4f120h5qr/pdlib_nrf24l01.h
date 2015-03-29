#ifndef _PDLIB_NRF24L01
#define _PDLIB_NRF24L01

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

/* PS: Function prototypes */

void NRF24L01_Init(unsigned long ulCEBase, unsigned long ulCEPin, unsigned long ulCEPeriph, unsigned long ulCSNBase, unsigned long ulCSNPin, unsigned long ulCSNPeriph, unsigned char ucSSIIndex);
void NRF24L01_RegisterInit();
void NRF24L01_PowerDown();
void NRF24L01_PowerUp();
int NRF24L01_SendData(char *pcData, unsigned int uiLength);
int NRF24L01_SendDataTo(unsigned char *address, char *pcData, unsigned int uiLength);

/* Configuration related */
void NRF24L01_FlushTX();
void NRF24L01_FlushRX();
void NRF24L01_SetAirDataRate(unsigned char ucDataRate);
void NRF24L01_SetLNAGain(unsigned char ucLNAGain);
void NRF24L01_SetPAGain(int iPAGain);
void NRF24L01_SetRFChannel(unsigned char ucRFChannel);
void NRF24L01_SetARC(unsigned char ucVal);
void NRF24L01_SetARD(unsigned short ucVal);
void NRF24L01_SetAddressWidth(unsigned char ucVal);
unsigned char NRF24L01_GetStatus();

/* TX mode related */
void NRF24L01_SetTXAddress(unsigned char* address);
int NRF24L01_SetTxPayload(char* pcData, unsigned int uiLength);
void NRF24L01_EnableTxMode();
int NRF24L01_AttemptTx();
void NRF24L01_DisableTxMode();
int NRF24L01_IsTxFifoFull();
int NRF24L01_WaitForTxComplete();

/* RX mode related */
void NRF24L01_SetRxAddress(unsigned char ucDataPipe, unsigned char *pucAddress);
void NRF24L01_SetRXPacketSize(unsigned char ucDataPipe, unsigned char ucPacketSize);
void NRF24L01_EnableRxMode();
inline void NRF24L01_DisableRxMode();
int NRF24L01_IsDataReadyRx(char *pcPipeNo);
unsigned char NRF24L01_GetRxDataAmount(unsigned char ucDataPipe);
int NRF24L01_GetData(char pipe, char* pcData, char *length);
void NRF24L01_ReadRxPayload(char* pcData, char cLength);
int NRF24L01_WaitForDataRx(char *pcPipeNo);
unsigned char NRF24L01_CarrierDetect();

/* Register level access */
void NRF24L01_SendCommand(unsigned char ucCommand, char *pcData, unsigned int uiLength);
void NRF24L01_SendRcvCommand(unsigned char ucCommand, char *pcData, unsigned int uiLength);
unsigned char NRF24L01_RegisterRead_8(unsigned char ucRegister);
unsigned char NRF24L01_RegisterRead_Multi(unsigned char ucRegister, unsigned char *pucBuffer, unsigned int uiLength);
void NRF24L01_RegisterWrite_8(unsigned char ucRegister, unsigned char ucValue);
void NRF24L01_RegisterWrite_Multi(unsigned char ucRegister, unsigned char *pucData, unsigned int uiLength);

#endif

