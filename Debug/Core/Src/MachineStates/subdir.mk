################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MachineStates/DebugState.c \
../Core/Src/MachineStates/DiagnosticsState.c \
../Core/Src/MachineStates/ErrorState.c \
../Core/Src/MachineStates/IdleState.c \
../Core/Src/MachineStates/RunState.c \
../Core/Src/MachineStates/SettingsState.c 

OBJS += \
./Core/Src/MachineStates/DebugState.o \
./Core/Src/MachineStates/DiagnosticsState.o \
./Core/Src/MachineStates/ErrorState.o \
./Core/Src/MachineStates/IdleState.o \
./Core/Src/MachineStates/RunState.o \
./Core/Src/MachineStates/SettingsState.o 

C_DEPS += \
./Core/Src/MachineStates/DebugState.d \
./Core/Src/MachineStates/DiagnosticsState.d \
./Core/Src/MachineStates/ErrorState.d \
./Core/Src/MachineStates/IdleState.d \
./Core/Src/MachineStates/RunState.d \
./Core/Src/MachineStates/SettingsState.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/MachineStates/%.o Core/Src/MachineStates/%.su: ../Core/Src/MachineStates/%.c Core/Src/MachineStates/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SerialLogging" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SMPS" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSensors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/TowerLamp" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MB_LEDs" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineErrors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SysObserver" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/Bluetooth" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/userButtons" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSettings" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MotorComms" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN/MotherBoard" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/Eeprom" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MCP23017" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-MachineStates

clean-Core-2f-Src-2f-MachineStates:
	-$(RM) ./Core/Src/MachineStates/DebugState.d ./Core/Src/MachineStates/DebugState.o ./Core/Src/MachineStates/DebugState.su ./Core/Src/MachineStates/DiagnosticsState.d ./Core/Src/MachineStates/DiagnosticsState.o ./Core/Src/MachineStates/DiagnosticsState.su ./Core/Src/MachineStates/ErrorState.d ./Core/Src/MachineStates/ErrorState.o ./Core/Src/MachineStates/ErrorState.su ./Core/Src/MachineStates/IdleState.d ./Core/Src/MachineStates/IdleState.o ./Core/Src/MachineStates/IdleState.su ./Core/Src/MachineStates/RunState.d ./Core/Src/MachineStates/RunState.o ./Core/Src/MachineStates/RunState.su ./Core/Src/MachineStates/SettingsState.d ./Core/Src/MachineStates/SettingsState.o ./Core/Src/MachineStates/SettingsState.su

.PHONY: clean-Core-2f-Src-2f-MachineStates

