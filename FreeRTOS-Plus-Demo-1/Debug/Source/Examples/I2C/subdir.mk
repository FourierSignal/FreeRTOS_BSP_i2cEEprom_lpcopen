################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.c \
../Source/Examples/I2C/I2C-coordinator.c 

OBJS += \
./Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.o \
./Source/Examples/I2C/I2C-coordinator.o 

C_DEPS += \
./Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.d \
./Source/Examples/I2C/I2C-coordinator.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.o: ../Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_LPCOPEN -D__REDLIB__ -DCORE_M3 -DDEBUG -D__CODE_RED -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_chip_175x_6x/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-CLI" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"Source/Examples/I2C/Copy of I2C-to-and-from-EEPROM.d" -MT"Source/Examples/I2C/Copy\ of\ I2C-to-and-from-EEPROM.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/Examples/I2C/%.o: ../Source/Examples/I2C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_LPCOPEN -D__REDLIB__ -DCORE_M3 -DDEBUG -D__CODE_RED -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_chip_175x_6x/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-CLI" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


