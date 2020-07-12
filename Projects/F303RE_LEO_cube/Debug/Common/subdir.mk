################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Git\ Repos/LEO_cube/Common/cmd_parser.c \
D:/Git\ Repos/LEO_cube/Common/comms.c \
D:/Git\ Repos/LEO_cube/Common/counter.c \
D:/Git\ Repos/LEO_cube/Common/generator.c \
D:/Git\ Repos/LEO_cube/Common/logic_analyzer.c \
D:/Git\ Repos/LEO_cube/Common/scope.c \
D:/Git\ Repos/LEO_cube/Common/sync_pwm.c 

OBJS += \
./Common/cmd_parser.o \
./Common/comms.o \
./Common/counter.o \
./Common/generator.o \
./Common/logic_analyzer.o \
./Common/scope.o \
./Common/sync_pwm.o 

C_DEPS += \
./Common/cmd_parser.d \
./Common/comms.d \
./Common/counter.d \
./Common/generator.d \
./Common/logic_analyzer.d \
./Common/scope.d \
./Common/sync_pwm.d 


# Each subdirectory must supply rules for building sources it contributes
Common/cmd_parser.o: D:/Git\ Repos/LEO_cube/Common/cmd_parser.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/cmd_parser.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/comms.o: D:/Git\ Repos/LEO_cube/Common/comms.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/comms.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/counter.o: D:/Git\ Repos/LEO_cube/Common/counter.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/counter.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/generator.o: D:/Git\ Repos/LEO_cube/Common/generator.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/generator.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/logic_analyzer.o: D:/Git\ Repos/LEO_cube/Common/logic_analyzer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/logic_analyzer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/scope.o: D:/Git\ Repos/LEO_cube/Common/scope.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/scope.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/sync_pwm.o: D:/Git\ Repos/LEO_cube/Common/sync_pwm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/sync_pwm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

