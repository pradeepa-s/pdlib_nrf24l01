#ifndef _PDLIB_SPI
#define _PDLIB_SPI



void pdlibSPI_ConfigureSPIInterface(unsigned char ucSSI);
unsigned char pdlibSPI_ReceiveDataBlocking();
unsigned int pdlibSPI_ReceiveDataNonBlocking(char *pcData);
unsigned char pdlibSPI_TransferByte(unsigned char ucData);
int pdlibSPI_SendData(unsigned char *pucData, unsigned int uiLength);

#endif
