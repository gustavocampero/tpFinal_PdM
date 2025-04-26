################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../API/Src/API_debounce.c \
../API/Src/API_delay.c \
../API/Src/API_led.c \
../API/Src/API_uart.c 

OBJS += \
./API/Src/API_debounce.o \
./API/Src/API_delay.o \
./API/Src/API_led.o \
./API/Src/API_uart.o 

C_DEPS += \
./API/Src/API_debounce.d \
./API/Src/API_delay.d \
./API/Src/API_led.d \
./API/Src/API_uart.d 


# Each subdirectory must supply rules for building sources it contributes
API/Src/%.o API/Src/%.su API/Src/%.cyclo: ../API/Src/%.c API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/API/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SDcard/Src" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Inc" -I"C:/Users/Gus/Documents/GitHub/PdM_workspace/tpFinal_PdM_PCSE/myDrivers/SHT30/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-API-2f-Src

clean-API-2f-Src:
	-$(RM) ./API/Src/API_debounce.cyclo ./API/Src/API_debounce.d ./API/Src/API_debounce.o ./API/Src/API_debounce.su ./API/Src/API_delay.cyclo ./API/Src/API_delay.d ./API/Src/API_delay.o ./API/Src/API_delay.su ./API/Src/API_led.cyclo ./API/Src/API_led.d ./API/Src/API_led.o ./API/Src/API_led.su ./API/Src/API_uart.cyclo ./API/Src/API_uart.d ./API/Src/API_uart.o ./API/Src/API_uart.su

.PHONY: clean-API-2f-Src

