pdlib_nrf24l01
==============

The library is to interface NRF24L01 modules with micro-controllers. The library is written in C to make sure the code size is less.

LM4F120H5QR
===========

Processor has four SSI modules that can be configured as Freescale SPI to communicate with the NRF24L01 modules.

Data transfering path:

Processor ---> SSI module ---> NRF24L01

SSI modules can be configured in several ways. For instance SSI1 can be onfigured to use GPIOF or GPIOD.

