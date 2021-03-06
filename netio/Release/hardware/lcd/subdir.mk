################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/lcd/ecmd.c \
../hardware/lcd/hd44780.c \
../hardware/lcd/hd44780_2wire.c \
../hardware/lcd/hd44780_core.c \
../hardware/lcd/hd44780_i2c.c \
../hardware/lcd/hd44780_serlcd.c \
../hardware/lcd/hr20-charset.c \
../hardware/lcd/hr20-ecmd.c \
../hardware/lcd/hr20.c 

OBJS += \
./hardware/lcd/ecmd.o \
./hardware/lcd/hd44780.o \
./hardware/lcd/hd44780_2wire.o \
./hardware/lcd/hd44780_core.o \
./hardware/lcd/hd44780_i2c.o \
./hardware/lcd/hd44780_serlcd.o \
./hardware/lcd/hr20-charset.o \
./hardware/lcd/hr20-ecmd.o \
./hardware/lcd/hr20.o 

C_DEPS += \
./hardware/lcd/ecmd.d \
./hardware/lcd/hd44780.d \
./hardware/lcd/hd44780_2wire.d \
./hardware/lcd/hd44780_core.d \
./hardware/lcd/hd44780_i2c.d \
./hardware/lcd/hd44780_serlcd.d \
./hardware/lcd/hr20-charset.d \
./hardware/lcd/hr20-ecmd.d \
./hardware/lcd/hr20.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/lcd/%.o: ../hardware/lcd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


