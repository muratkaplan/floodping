################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../services/moodlight/moodlight.c \
../services/moodlight/moodlight_ecmd.c 

OBJS += \
./services/moodlight/moodlight.o \
./services/moodlight/moodlight_ecmd.o 

C_DEPS += \
./services/moodlight/moodlight.d \
./services/moodlight/moodlight_ecmd.d 


# Each subdirectory must supply rules for building sources it contributes
services/moodlight/%.o: ../services/moodlight/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/workspace1/netio" -I"/root/workspace1/netio/core/portio" -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


