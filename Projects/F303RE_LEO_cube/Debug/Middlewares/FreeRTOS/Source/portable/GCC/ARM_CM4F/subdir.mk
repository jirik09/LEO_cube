################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Git\ Repos/LEO_cube/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o: D:/Git\ Repos/LEO_cube/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_FULL_ASSERT -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_COUNTER -DUSE_GEN -DUSE_LOG_ANLYS -DUSE_GEN_PWM -DUSE_SYNC_PWM -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

