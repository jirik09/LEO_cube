################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/clock.c \
../Src/comms_hal.c \
../Src/dac.c \
../Src/dma.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/main.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/tim.c \
../Src/tim_counter.c \
../Src/tim_gen.c \
../Src/tim_scope.c \
../Src/tim_sync_pwm.c \
../Src/usart.c 

OBJS += \
./Src/adc.o \
./Src/clock.o \
./Src/comms_hal.o \
./Src/dac.o \
./Src/dma.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/tim.o \
./Src/tim_counter.o \
./Src/tim_gen.o \
./Src/tim_scope.o \
./Src/tim_sync_pwm.o \
./Src/usart.o 

C_DEPS += \
./Src/adc.d \
./Src/clock.d \
./Src/comms_hal.d \
./Src/dac.d \
./Src/dma.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/tim.d \
./Src/tim_counter.d \
./Src/tim_gen.d \
./Src/tim_scope.d \
./Src/tim_sync_pwm.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/adc.o: ../Src/adc.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/clock.o: ../Src/clock.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/clock.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/comms_hal.o: ../Src/comms_hal.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/comms_hal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dac.o: ../Src/dac.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dac.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dma.o: ../Src/dma.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/freertos.o: ../Src/freertos.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/gpio.o: ../Src/gpio.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_msp.o: ../Src/stm32f4xx_hal_msp.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_timebase_tim.o: ../Src/stm32f4xx_hal_timebase_tim.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_timebase_tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_it.o: ../Src/stm32f4xx_it.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f4xx.o: ../Src/system_stm32f4xx.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim.o: ../Src/tim.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim_counter.o: ../Src/tim_counter.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim_counter.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim_gen.o: ../Src/tim_gen.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim_gen.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim_scope.o: ../Src/tim_scope.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim_scope.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim_sync_pwm.o: ../Src/tim_sync_pwm.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim_sync_pwm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usart.o: ../Src/usart.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DNOUSE_SYNC_PWM -c -I"../Inc" -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

