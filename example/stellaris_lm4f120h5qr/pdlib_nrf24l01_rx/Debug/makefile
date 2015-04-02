################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := F:/ti/ccsv5/tools/compiler/arm_5.0.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./uart_debug.obj" \
"./startup_ccs.obj" \
"./pdlib_spi.obj" \
"./pdlib_nrf24l01.obj" \
"./main.obj" \
-l"libc.a" \
-l"F:\StellarisWare\driverlib\ccs-cm4f\Debug\driverlib-cm4f.lib" \
"../lm4f120h5qr.cmd" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pdlib_nrf24l01_rx.out

# Tool invocations
pdlib_nrf24l01_rx.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --define=PART_LM4F120H5QR --define=TARGET_IS_BLIZZARD_RA1 --define=PDLIB_SPI --diag_warning=225 --display_error_number --diag_wrap=off -z --stack_size=256 -m"pdlib_nrf24l01_rx.map" --heap_size=100 -i"F:/ti/ccsv5/tools/compiler/arm_5.0.1/lib" -i"F:/ti/ccsv5/tools/compiler/arm_5.0.1/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --rom_model -o "pdlib_nrf24l01_rx.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "pdlib_nrf24l01_rx.out"
	-$(RM) "main.pp" "pdlib_nrf24l01.pp" "pdlib_spi.pp" "startup_ccs.pp" "uart_debug.pp" 
	-$(RM) "main.obj" "pdlib_nrf24l01.obj" "pdlib_spi.obj" "startup_ccs.obj" "uart_debug.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"F:/ti/ccsv5/utils/tiobj2bin/tiobj2bin" "pdlib_nrf24l01_rx.out" "pdlib_nrf24l01_rx.bin" "F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armofd" "F:/ti/ccsv5/tools/compiler/arm_5.0.1/bin/armhex" "F:/ti/ccsv5/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
