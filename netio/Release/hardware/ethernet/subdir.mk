################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../hardware/ethernet/enc28j60.o \
../hardware/ethernet/enc28j60_process.o \
../hardware/ethernet/enc28j60_transmit.o \
../hardware/ethernet/ethernet_config.o \
../hardware/ethernet/ethernet_ecmd.o 

C_SRCS += \
../hardware/ethernet/enc28j60.c \
../hardware/ethernet/enc28j60_process.c \
../hardware/ethernet/enc28j60_transmit.c \
../hardware/ethernet/ethernet_config.c \
../hardware/ethernet/ethernet_ecmd.c 

OBJS += \
./hardware/ethernet/enc28j60.o \
./hardware/ethernet/enc28j60_process.o \
./hardware/ethernet/enc28j60_transmit.o \
./hardware/ethernet/ethernet_config.o \
./hardware/ethernet/ethernet_ecmd.o 

C_DEPS += \
./hardware/ethernet/enc28j60.d \
./hardware/ethernet/enc28j60_process.d \
./hardware/ethernet/enc28j60_transmit.d \
./hardware/ethernet/ethernet_config.d \
./hardware/ethernet/ethernet_ecmd.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/ethernet/%.o: ../hardware/ethernet/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


