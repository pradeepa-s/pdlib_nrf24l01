#ifndef _PDLIB_NRF24L01
#define _PDLIB_NRF24L01

/* PS: Function prototypes */

void NRF24L01_Init(unsigned long ulCEBase, unsigned long ulCEPin, unsigned long ulCSNBase, unsigned long ulCSNPin,unsigned char ucSSIIndex);
void NRF24L01_PowerDown();
void NRF24L01_PowerUp();
void NRF24L01_FlushTX();
void NRF24L01_FlushRX();
void NRF24L01_SetAirDataRate(unsigned char ucDataRate);
void NRF24L01_SetLNAGain(unsigned char ucLNAGain);
void NRF24L01_SetPAGain(unsigned char ucPAGain);
void NRF24L01_SetRFChannel(unsigned char ucRFChannel);
unsigned char NRF24L01_GetStatus();

void NRF24L01_SetTXAddress(unsigned char* pucAddress);
void NRF24L01_SetTxPayload(unsigned char* pucData, unsigned int uiLength);
void NRF24L01_EnableTxMode();
//unsigned char NRF24L01_IsTxComplete();

void NRF24L01_SetRxAddress(unsigned char ucDataPipe, unsigned char *pucAddress);
void NRF24L01_EnableRxMode();
unsigned char NRF24L01_IsDataReadyRx();
unsigned char NRF24L01_GetRxDataAmount(unsigned char ucDataPipe);

unsigned char NRF24L01_RegisterRead_8(unsigned char ucRegister);

#endif

