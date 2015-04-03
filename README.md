pdlib_nrf24l01
==============

The library is to interface NRF24L01 modules with micro-controllers. The library is written in C to make sure the code size is less.

Change Log
===========
[2015-04-03]:	Release V1.01
			Provides support to all the functions except Dynamic Payload and Auto Ack payload.

Porting the library:
====================

If you change the library, and if you are willing please send me the SPI library so that I can merge that library to the main code. Or send me pull request. 
Please use a separate #define to separate the changes. 

To change the SPI library you need to change the following functions.

	NRF24L01_Init
	NRF24L01_RegisterWrite_8
	NRF24L01_RegisterWrite_Multi
	NRF24L01_RegisterRead_8
	NRF24L01_RegisterRead_Multi
	NRF24L01_SendCommand
	NRF24L01_SendRcvCommand

To change the processor you need to change following functions,

	NRF24L01_Init
	NRF24L01_InterruptInit
	_NRF24L01_CELow
	_NRF24L01_CEHigh
	_NRF24L01_CSNLow
	_NRF24L01_CSNHigh

LM4F120H5QR
===========

Processor has four SSI modules that can be configured as Freescale SPI to communicate with the NRF24L01 modules.

Data transfer path:

Processor ---> SSI module ---> NRF24L01

SSI modules can be configured in several ways. For instance SSI1 can be configured to use GPIOF or GPIOD.
