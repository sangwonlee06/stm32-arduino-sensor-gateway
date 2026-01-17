################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/util/byte_queue.c 

OBJS += \
./Core/Src/util/byte_queue.o 

C_DEPS += \
./Core/Src/util/byte_queue.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/util/%.o Core/Src/util/%.su Core/Src/util/%.cyclo: ../Core/Src/util/%.c Core/Src/util/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/drv" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/iot" -I"/Users/sangwonlee/Practice/stm32-arduino-sensor-gateway/firmware/stm32/app/iot_project/Core/Inc/util" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-util

clean-Core-2f-Src-2f-util:
	-$(RM) ./Core/Src/util/byte_queue.cyclo ./Core/Src/util/byte_queue.d ./Core/Src/util/byte_queue.o ./Core/Src/util/byte_queue.su

.PHONY: clean-Core-2f-Src-2f-util

