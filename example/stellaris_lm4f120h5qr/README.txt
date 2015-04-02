Date: 2015-04-02
Authur: Pradeepa Senanayake

***************************
Project creation guidlines
***************************

--------------------------------------------
IDE: 	Code Composer Studio v5.3.0.00090 
Part:	LM4F120H5QR in Stellaris Launchpad
--------------------------------------------

[1]. Create a blank project
[2]. Include the stellarisware, nrf24l01.h, pdlib_spi.h and pdlib_nrf24l01.h
	*Properties -> CCS Build -> Arm Compiler -> Include Options

"${PROJECT_ROOT}\..\..\..\arm\stellaris_lm4f120h5qr"
"${PROJECT_ROOT}\..\..\..\common"
"${STELLARISWARE_PATH}"

[3]. Link to the pdlib_spi.c and pdlib_nrf24l01.c
	*Add sources to the project

[4]. Add the predefined symbols,
	*Properties -> CCS Build -> Arm Compiler -> Advanced Options -> Predefined Symbols

PART_LM4F120H5QR
TARGET_IS_BLIZZARD_RA1
PDLIB_SPI
NRF24L01_CONF_INTERRUPT_PIN     -- for interrupts in NRF24L01

[5]. Make sure the Heap has sufficient (around 100 bytes) space
	*Properties -> CCS Build -> Arm Linker -> Basic Options

[6]. Link to the driver library	
	*Properties -> CCS Build -> Arm Linker -> File Search Path

"${STELLARISWARE_PATH}\driverlib\ccs-cm4f\Debug\driverlib-cm4f.lib"

[7]. Include the startup_css.c file to make sure the startup is there for the bin file

[8]. Define the System Variable 'STELLARISWARE_PATH'
	* Properties -> Build -> Variables

[9]. Add post build steps
	* Properties -> Build -> Steps

"${CCS_INSTALL_ROOT}/utils/tiobj2bin/tiobj2bin" "${BuildArtifactFileName}" "${BuildArtifactFileBaseName}.bin" "${CG_TOOL_ROOT}/bin/armofd" "${CG_TOOL_ROOT}/bin/armhex" "${CCS_INSTALL_ROOT}/utils/tiobj2bin/mkhex4bin"
	