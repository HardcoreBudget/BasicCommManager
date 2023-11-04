################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Services/BCM/Source/BCM_Imp.c 

OBJS += \
./Services/BCM/Source/BCM_Imp.o 

C_DEPS += \
./Services/BCM/Source/BCM_Imp.d 


# Each subdirectory must supply rules for building sources it contributes
Services/BCM/Source/%.o: ../Services/BCM/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


