################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/to1/to1.c \
../protocols/to1/to1_ecmd.c 

OBJS += \
./protocols/to1/to1.o \
./protocols/to1/to1_ecmd.o 

C_DEPS += \
./protocols/to1/to1.d \
./protocols/to1/to1_ecmd.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/to1/%.o: ../protocols/to1/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


