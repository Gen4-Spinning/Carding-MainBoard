################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MachineSettings/machineEepromFns.c \
../Drivers/MachineSettings/machineSettings.c 

OBJS += \
./Drivers/MachineSettings/machineEepromFns.o \
./Drivers/MachineSettings/machineSettings.o 

C_DEPS += \
./Drivers/MachineSettings/machineEepromFns.d \
./Drivers/MachineSettings/machineSettings.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MachineSettings/%.o Drivers/MachineSettings/%.su: ../Drivers/MachineSettings/%.c Drivers/MachineSettings/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SerialLogging" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSensors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineErrors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SysObserver" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/userButtons" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MotorComms" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSettings" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN/MotherBoard" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/Eeprom" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MCP23017" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MachineSettings

clean-Drivers-2f-MachineSettings:
	-$(RM) ./Drivers/MachineSettings/machineEepromFns.d ./Drivers/MachineSettings/machineEepromFns.o ./Drivers/MachineSettings/machineEepromFns.su ./Drivers/MachineSettings/machineSettings.d ./Drivers/MachineSettings/machineSettings.o ./Drivers/MachineSettings/machineSettings.su

.PHONY: clean-Drivers-2f-MachineSettings

