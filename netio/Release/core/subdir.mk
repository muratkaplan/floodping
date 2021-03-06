################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../core/debug.o \
../core/eeprom.o \
../core/periodic.o \
../core/spi.o 

C_SRCS += \
../core/debug.c \
../core/eeprom.c \
../core/heartbeat.c \
../core/periodic.c \
../core/soft_spi.c \
../core/soft_uart.c \
../core/spi.c \
../core/spi_usart.c \
../core/usart.c 

OBJS += \
./core/debug.o \
./core/eeprom.o \
./core/heartbeat.o \
./core/periodic.o \
./core/soft_spi.o \
./core/soft_uart.o \
./core/spi.o \
./core/spi_usart.o \
./core/usart.o 

C_DEPS += \
./core/debug.d \
./core/eeprom.d \
./core/heartbeat.d \
./core/periodic.d \
./core/soft_spi.d \
./core/soft_uart.d \
./core/spi.d \
./core/spi_usart.d \
./core/usart.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


