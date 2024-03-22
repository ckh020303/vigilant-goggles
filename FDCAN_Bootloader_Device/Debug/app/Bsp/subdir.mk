################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/Bsp/Bsp.c 

OBJS += \
./app/Bsp/Bsp.o 

C_DEPS += \
./app/Bsp/Bsp.d 


# Each subdirectory must supply rules for building sources it contributes
app/Bsp/%.o app/Bsp/%.su app/Bsp/%.cyclo: ../app/Bsp/%.c app/Bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ftftf/Desktop/vigilant-goggles-FDCAN_Bootloader/FDCAN_Bootloader_Device/app/Bsp" -I"C:/Users/ftftf/Desktop/vigilant-goggles-FDCAN_Bootloader/FDCAN_Bootloader_Device/app/FDCAN_Flash" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-Bsp

clean-app-2f-Bsp:
	-$(RM) ./app/Bsp/Bsp.cyclo ./app/Bsp/Bsp.d ./app/Bsp/Bsp.o ./app/Bsp/Bsp.su

.PHONY: clean-app-2f-Bsp

