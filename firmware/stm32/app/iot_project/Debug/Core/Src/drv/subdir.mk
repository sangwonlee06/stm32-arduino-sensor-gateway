################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drv/drv_uart.c 

OBJS += \
./Core/Src/drv/drv_uart.o 

C_DEPS += \
./Core/Src/drv/drv_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drv/%.o Core/Src/drv/%.su Core/Src/drv/%.cyclo: ../Core/Src/drv/%.c Core/Src/drv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/drv" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/iot" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/util" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drv

clean-Core-2f-Src-2f-drv:
	-$(RM) ./Core/Src/drv/drv_uart.cyclo ./Core/Src/drv/drv_uart.d ./Core/Src/drv/drv_uart.o ./Core/Src/drv/drv_uart.su

.PHONY: clean-Core-2f-Src-2f-drv

