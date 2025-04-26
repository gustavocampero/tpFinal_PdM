################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myDrivers/SDcard/Src/port.c \
../myDrivers/SDcard/Src/sd_card.c 

OBJS += \
./myDrivers/SDcard/Src/port.o \
./myDrivers/SDcard/Src/sd_card.o 

C_DEPS += \
./myDrivers/SDcard/Src/port.d \
./myDrivers/SDcard/Src/sd_card.d 


# Each subdirectory must supply rules for building sources it contributes
myDrivers/SDcard/Src/%.o myDrivers/SDcard/Src/%.su myDrivers/SDcard/Src/%.cyclo: ../myDrivers/SDcard/Src/%.c myDrivers/SDcard/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-myDrivers-2f-SDcard-2f-Src

clean-myDrivers-2f-SDcard-2f-Src:
	-$(RM) ./myDrivers/SDcard/Src/port.cyclo ./myDrivers/SDcard/Src/port.d ./myDrivers/SDcard/Src/port.o ./myDrivers/SDcard/Src/port.su ./myDrivers/SDcard/Src/sd_card.cyclo ./myDrivers/SDcard/Src/sd_card.d ./myDrivers/SDcard/Src/sd_card.o ./myDrivers/SDcard/Src/sd_card.su

.PHONY: clean-myDrivers-2f-SDcard-2f-Src

