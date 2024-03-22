################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/FDCAN_Flash/FDCAN_Flash_host.c 

OBJS += \
./app/FDCAN_Flash/FDCAN_Flash_host.o 

C_DEPS += \
./app/FDCAN_Flash/FDCAN_Flash_host.d 


# Each subdirectory must supply rules for building sources it contributes
app/FDCAN_Flash/%.o app/FDCAN_Flash/%.su app/FDCAN_Flash/%.cyclo: ../app/FDCAN_Flash/%.c app/FDCAN_Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ftftf/Desktop/vigilant-goggles-FDCAN_Bootloader/FDCAN_Bootloader_Host/app/Bsp" -I"C:/Users/ftftf/Desktop/vigilant-goggles-FDCAN_Bootloader/FDCAN_Bootloader_Host/app/FDCAN_Flash" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-FDCAN_Flash

clean-app-2f-FDCAN_Flash:
	-$(RM) ./app/FDCAN_Flash/FDCAN_Flash_host.cyclo ./app/FDCAN_Flash/FDCAN_Flash_host.d ./app/FDCAN_Flash/FDCAN_Flash_host.o ./app/FDCAN_Flash/FDCAN_Flash_host.su

.PHONY: clean-app-2f-FDCAN_Flash

