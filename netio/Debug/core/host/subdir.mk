################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../core/host/eeprom.c \
../core/host/printf.c \
../core/host/stdin.c \
../core/host/tap.c \
../core/host/vfs.c 

OBJS += \
./core/host/eeprom.o \
./core/host/printf.o \
./core/host/stdin.o \
./core/host/tap.o \
./core/host/vfs.o 

C_DEPS += \
./core/host/eeprom.d \
./core/host/printf.d \
./core/host/stdin.d \
./core/host/tap.d \
./core/host/vfs.d 


# Each subdirectory must supply rules for building sources it contributes
core/host/%.o: ../core/host/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


