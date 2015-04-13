################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="F:/StellarisWare" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/common" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr" --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --define=NRF24L01_CONF_INTERRUPT_PIN --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pdlib_nrf24l01.obj: F:/Other/Nrf24l01/Stellaris\ library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr/pdlib_nrf24l01.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="F:/StellarisWare" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/common" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr" --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --define=NRF24L01_CONF_INTERRUPT_PIN --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="pdlib_nrf24l01.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pdlib_spi.obj: F:/Other/Nrf24l01/Stellaris\ library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr/pdlib_spi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="F:/StellarisWare" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/common" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr" --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --define=NRF24L01_CONF_INTERRUPT_PIN --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="pdlib_spi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="F:/StellarisWare" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/common" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr" --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --define=NRF24L01_CONF_INTERRUPT_PIN --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_debug.obj: ../uart_debug.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --include_path="F:/StellarisWare" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/common" --include_path="F:/Other/Nrf24l01/Stellaris library/pdlib_nrf24l01/git/pdlib_nrf24l01/arm/stellaris_lm4f120h5qr" --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --define=NRF24L01_CONF_INTERRUPT_PIN --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="uart_debug.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


