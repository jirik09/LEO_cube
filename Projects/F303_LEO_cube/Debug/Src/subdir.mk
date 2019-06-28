################################################################################
# Automatically-generated file. Do not edit!
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
../Src/stm32f3xx_hal_msp.c \
../Src/stm32f3xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f3xx.c \
../Src/tim.c \
../Src/usart.c \
../Src/usb_device.c \
../Src/usb_device_old.c \
../Src/usbd_cdc_if.c \
../Src/usbd_cdc_if_old.c \
../Src/usbd_conf.c \
../Src/usbd_conf_old.c \
../Src/usbd_desc.c \
../Src/usbd_desc_old.c 

OBJS += \
./Src/adc.o \
./Src/clock.o \
./Src/comms_hal.o \
./Src/dac.o \
./Src/dma.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/main.o \
./Src/stm32f3xx_hal_msp.o \
./Src/stm32f3xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f3xx.o \
./Src/tim.o \
./Src/usart.o \
./Src/usb_device.o \
./Src/usb_device_old.o \
./Src/usbd_cdc_if.o \
./Src/usbd_cdc_if_old.o \
./Src/usbd_conf.o \
./Src/usbd_conf_old.o \
./Src/usbd_desc.o \
./Src/usbd_desc_old.o 

C_DEPS += \
./Src/adc.d \
./Src/clock.d \
./Src/comms_hal.d \
./Src/dac.d \
./Src/dma.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/main.d \
./Src/stm32f3xx_hal_msp.d \
./Src/stm32f3xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f3xx.d \
./Src/tim.d \
./Src/usart.d \
./Src/usb_device.d \
./Src/usb_device_old.d \
./Src/usbd_cdc_if.d \
./Src/usbd_cdc_if_old.d \
./Src/usbd_conf.d \
./Src/usbd_conf_old.d \
./Src/usbd_desc.d \
./Src/usbd_desc_old.d 


# Each subdirectory must supply rules for building sources it contributes
Src/adc.o: ../Src/adc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/clock.o: ../Src/clock.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/clock.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/comms_hal.o: ../Src/comms_hal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/comms_hal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dac.o: ../Src/dac.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dac.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dma.o: ../Src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/freertos.o: ../Src/freertos.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/freertos.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/gpio.o: ../Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f3xx_hal_msp.o: ../Src/stm32f3xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f3xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f3xx_it.o: ../Src/stm32f3xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f3xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f3xx.o: ../Src/system_stm32f3xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f3xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/tim.o: ../Src/tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usart.o: ../Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usb_device.o: ../Src/usb_device.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usb_device.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usb_device_old.o: ../Src/usb_device_old.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usb_device_old.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_cdc_if.o: ../Src/usbd_cdc_if.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_cdc_if.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_cdc_if_old.o: ../Src/usbd_cdc_if_old.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_cdc_if_old.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_conf.o: ../Src/usbd_conf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_conf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_conf_old.o: ../Src/usbd_conf_old.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_conf_old.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_desc.o: ../Src/usbd_desc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_desc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usbd_desc_old.o: ../Src/usbd_desc_old.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usbd_desc_old.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

