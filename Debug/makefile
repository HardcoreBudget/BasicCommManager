################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Services/BCM/Source/subdir.mk
-include MCAL/USART/Source/subdir.mk
-include MCAL/TMR2/Source/subdir.mk
-include MCAL/TMR1/Source/subdir.mk
-include MCAL/TMR0/Source/subdir.mk
-include MCAL/SPI/Source/subdir.mk
-include MCAL/I2C/Source/subdir.mk
-include MCAL/GIE/Source/subdir.mk
-include MCAL/EXTI/Source/subdir.mk
-include MCAL/DIO/Source/subdir.mk
-include MCAL/ADC/Source/subdir.mk
-include HAL/SevSeg/Source/subdir.mk
-include HAL/LCD/Source/subdir.mk
-include HAL/Keypad/Source/subdir.mk
-include App/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
BasicCommStack.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: BasicCommStack.elf secondary-outputs

# Tool invocations
BasicCommStack.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,BasicCommStack.map -mmcu=atmega32 -o "BasicCommStack.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

BasicCommStack.lss: BasicCommStack.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S BasicCommStack.elf  >"BasicCommStack.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: BasicCommStack.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 BasicCommStack.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) BasicCommStack.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
