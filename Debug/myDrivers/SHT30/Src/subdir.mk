################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myDrivers/SHT30/Src/port.c \
../myDrivers/SHT30/Src/sht30.c 

OBJS += \
./myDrivers/SHT30/Src/port.o \
./myDrivers/SHT30/Src/sht30.o 

C_DEPS += \
./myDrivers/SHT30/Src/port.d \
./myDrivers/SHT30/Src/sht30.d 


# Each subdirectory must supply rules for building sources it contributes
myDrivers/SHT30/Src/%.o myDrivers/SHT30/Src/%.su myDrivers/SHT30/Src/%.cyclo: ../myDrivers/SHT30/Src/%.c myDrivers/SHT30/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-myDrivers-2f-SHT30-2f-Src

clean-myDrivers-2f-SHT30-2f-Src:
	-$(RM) ./myDrivers/SHT30/Src/port.cyclo ./myDrivers/SHT30/Src/port.d ./myDrivers/SHT30/Src/port.o ./myDrivers/SHT30/Src/port.su ./myDrivers/SHT30/Src/sht30.cyclo ./myDrivers/SHT30/Src/sht30.d ./myDrivers/SHT30/Src/sht30.o ./myDrivers/SHT30/Src/sht30.su

.PHONY: clean-myDrivers-2f-SHT30-2f-Src

