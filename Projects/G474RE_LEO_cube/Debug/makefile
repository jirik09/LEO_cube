################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/FreeRTOS/Source/subdir.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include Common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
G474RE_LEO_cube.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
G474RE_LEO_cube.list \

OBJCOPY_BIN += \
G474RE_LEO_cube.bin \


# All Target
all: G474RE_LEO_cube.elf secondary-outputs

# Tool invocations
G474RE_LEO_cube.elf: $(OBJS) $(USER_OBJS) D:\GitHub\LEO_cube\Projects\G474RE_LEO_cube\STM32G474RETX_FLASH.ld
	arm-none-eabi-gcc -o "G474RE_LEO_cube.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\GitHub\LEO_cube\Projects\G474RE_LEO_cube\STM32G474RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="G474RE_LEO_cube.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

G474RE_LEO_cube.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "G474RE_LEO_cube.list"
	@echo 'Finished building: $@'
	@echo ' '

G474RE_LEO_cube.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "G474RE_LEO_cube.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
