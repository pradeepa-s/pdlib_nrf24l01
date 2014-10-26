#ifndef _PDLIB_NRF24L01
#define _PDLIB_NRF24L01

/* PS: Function prototypes */

void NRF24L01_Init(unsigned long ulCEBase, unsigned long ulCEPin, unsigned long ulCEPeriph, unsigned long ulCSNBase, unsigned long ulCSNPin, unsigned long ulCSNPeriph, unsigned char ucSSIIndex);
void NRF24L01_RegisterInit();

/* Configuration related */
void NRF24L01_PowerDown();
void NRF24L01_PowerUp();
void NRF24L01_FlushTX();
void NRF24L01_FlushRX();
void NRF24L01_SetAirDataRate(unsigned char ucDataRate);
void NRF24L01_SetLNAGain(unsigned char ucLNAGain);
void NRF24L01_SetPAGain(int iPAGain);
void NRF24L01_SetRFChannel(unsigned char ucRFChannel);
unsigned char NRF24L01_GetStatus();

/* TX mode related */
void NRF24L01_SetTXAddress(unsigned char* pucAddress);
int NRF24L01_SetTxPayload(unsigned char* pucData, unsigned int uiLength);
void NRF24L01_EnableTxMode();
int NRF24L01_IsTxFifoFull();
int NRF24L01_WaitForTxComplete(unsigned char ucWaitMode);

/* RX mode related */
void NRF24L01_SetRxAddress(unsigned char ucDataPipe, unsigned char *pucAddress);
void NRF24L01_EnableRxMode();
int NRF24L01_IsDataReadyRx();
unsigned char NRF24L01_GetRxDataAmount(unsigned char ucDataPipe);

/* Register level access */
unsigned char NRF24L01_RegisterRead_8(unsigned char ucRegister);
unsigned char NRF24L01_RegisterRead_Multi(unsigned char ucRegister, unsigned char *pucBuffer, unsigned int uiLength);
void NRF24L01_RegisterWrite_8(unsigned char ucRegister, unsigned char ucValue);
void NRF24L01_RegisterWrite_Multi(unsigned char ucRegister, unsigned char *pucData, unsigned int uiLength);

#endif

