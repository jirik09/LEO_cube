################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Maeval/Documents/GitHub_CubeIDE/LEO_cube/Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.o: C:/Users/Maeval/Documents/GitHub_CubeIDE/LEO_cube/Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DSTM32F303xE -DDEBUG -DUSE_SCOPE -DUSE_GEN -DUSE_GEN_PWM -DUSE_SYNC_PWM -DUSE_LOG_ANLYS -DUSE_COUNTER -c -I../Inc -I"../../../Common" -I"../../../Drivers/CMSIS/Include" -I"../../../Drivers/CMSIS/Device/ST/STM32F3xx/Include" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc" -I"../../../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy" -I"../../../Drivers/BSP/STM32F3xx-Nucleo" -I"../../../Middlewares/FreeRTOS/Source/include" -I"../../../Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I../../../Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/STM32F3xx-Nucleo/stm32f3xx_nucleo.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

