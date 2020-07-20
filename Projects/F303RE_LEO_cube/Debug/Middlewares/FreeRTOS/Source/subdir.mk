################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/croutine.c \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/event_groups.c \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/list.c \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/queue.c \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/tasks.c \
C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/timers.c 

OBJS += \
./Middlewares/FreeRTOS/Source/croutine.o \
./Middlewares/FreeRTOS/Source/event_groups.o \
./Middlewares/FreeRTOS/Source/list.o \
./Middlewares/FreeRTOS/Source/queue.o \
./Middlewares/FreeRTOS/Source/tasks.o \
./Middlewares/FreeRTOS/Source/timers.o 

C_DEPS += \
./Middlewares/FreeRTOS/Source/croutine.d \
./Middlewares/FreeRTOS/Source/event_groups.d \
./Middlewares/FreeRTOS/Source/list.d \
./Middlewares/FreeRTOS/Source/queue.d \
./Middlewares/FreeRTOS/Source/tasks.d \
./Middlewares/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FreeRTOS/Source/croutine.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/croutine.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/croutine.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FreeRTOS/Source/event_groups.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/event_groups.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/event_groups.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FreeRTOS/Source/list.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/list.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/list.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FreeRTOS/Source/queue.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/queue.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/queue.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FreeRTOS/Source/tasks.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/tasks.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/tasks.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FreeRTOS/Source/timers.o: C:/Users/janmu/Documents/GitHub/LEO_cube/Middlewares/FreeRTOS/Source/timers.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/timers.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

