################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Bluetooth/BT_Console.c \
../Drivers/Bluetooth/BT_Fns.c \
../Drivers/Bluetooth/BT_Machine.c 

OBJS += \
./Drivers/Bluetooth/BT_Console.o \
./Drivers/Bluetooth/BT_Fns.o \
./Drivers/Bluetooth/BT_Machine.o 

C_DEPS += \
./Drivers/Bluetooth/BT_Console.d \
./Drivers/Bluetooth/BT_Fns.d \
./Drivers/Bluetooth/BT_Machine.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Bluetooth/%.o Drivers/Bluetooth/%.su: ../Drivers/Bluetooth/%.c Drivers/Bluetooth/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SerialLogging" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSensors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineErrors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/SysObserver" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/userButtons" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MotorComms" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MachineSettings" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN/MotherBoard" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/FDCAN" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/Eeprom" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Carding/Drivers/MCP23017" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Bluetooth

clean-Drivers-2f-Bluetooth:
	-$(RM) ./Drivers/Bluetooth/BT_Console.d ./Drivers/Bluetooth/BT_Console.o ./Drivers/Bluetooth/BT_Console.su ./Drivers/Bluetooth/BT_Fns.d ./Drivers/Bluetooth/BT_Fns.o ./Drivers/Bluetooth/BT_Fns.su ./Drivers/Bluetooth/BT_Machine.d ./Drivers/Bluetooth/BT_Machine.o ./Drivers/Bluetooth/BT_Machine.su

.PHONY: clean-Drivers-2f-Bluetooth

