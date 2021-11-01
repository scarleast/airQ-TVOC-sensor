################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/oled/oled.c 

OBJS += \
./user/oled/oled.o 

C_DEPS += \
./user/oled/oled.d 


# Each subdirectory must supply rules for building sources it contributes
user/oled/oled.o: ../user/oled/oled.c user/oled/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F030x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"/Users/scarleast/codes/stm32/air_qx/user/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"user/oled/oled.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

