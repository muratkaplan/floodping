################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../protocols/zbus/process.c \
../protocols/zbus/zbus.c \
../protocols/zbus/zbus_raw_net.c 

OBJS += \
./protocols/zbus/process.o \
./protocols/zbus/zbus.o \
./protocols/zbus/zbus_raw_net.o 

C_DEPS += \
./protocols/zbus/process.d \
./protocols/zbus/zbus.d \
./protocols/zbus/zbus_raw_net.d 


# Each subdirectory must supply rules for building sources it contributes
protocols/zbus/%.o: ../protocols/zbus/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


