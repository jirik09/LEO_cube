################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/GitHub/LEO_cube/Common/cmd_parser.c \
D:/GitHub/LEO_cube/Common/comms.c \
D:/GitHub/LEO_cube/Common/counter.c \
D:/GitHub/LEO_cube/Common/generator.c \
D:/GitHub/LEO_cube/Common/logic_analyzer.c \
D:/GitHub/LEO_cube/Common/scope.c \
D:/GitHub/LEO_cube/Common/sync_pwm.c 

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
Common/cmd_parser.o: D:/GitHub/LEO_cube/Common/cmd_parser.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/cmd_parser.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/comms.o: D:/GitHub/LEO_cube/Common/comms.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/comms.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/counter.o: D:/GitHub/LEO_cube/Common/counter.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/counter.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/generator.o: D:/GitHub/LEO_cube/Common/generator.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/generator.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/logic_analyzer.o: D:/GitHub/LEO_cube/Common/logic_analyzer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/logic_analyzer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/scope.o: D:/GitHub/LEO_cube/Common/scope.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/scope.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Common/sync_pwm.o: D:/GitHub/LEO_cube/Common/sync_pwm.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../../../Common -I../Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I../../Core/Inc -I../../../Drivers/CMSIS/Include -I../../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../../Drivers/STM32F4xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Common/sync_pwm.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

