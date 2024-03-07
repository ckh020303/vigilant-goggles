################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/SPI_Flash/SPI_Flash.c 

OBJS += \
./app/SPI_Flash/SPI_Flash.o 

C_DEPS += \
./app/SPI_Flash/SPI_Flash.d 


# Each subdirectory must supply rules for building sources it contributes
app/SPI_Flash/%.o app/SPI_Flash/%.su app/SPI_Flash/%.cyclo: ../app/SPI_Flash/%.c app/SPI_Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/STM32CubeIDE/workspace_1.14.0/Bootloader/app/SPI_SD_driver" -I"D:/STM32CubeIDE/workspace_1.14.0/Bootloader/app/Bsp" -I"D:/STM32CubeIDE/workspace_1.14.0/Bootloader/app/SPIFlashUpdate" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"D:/STM32CubeIDE/workspace_1.14.0/Bootloader/app/InternalFlashUpdater" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-SPI_Flash

clean-app-2f-SPI_Flash:
	-$(RM) ./app/SPI_Flash/SPI_Flash.cyclo ./app/SPI_Flash/SPI_Flash.d ./app/SPI_Flash/SPI_Flash.o ./app/SPI_Flash/SPI_Flash.su

.PHONY: clean-app-2f-SPI_Flash

