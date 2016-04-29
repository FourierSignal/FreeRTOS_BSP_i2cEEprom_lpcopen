################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/UART/CLI-commands.c \
../Source/Examples/UART/UART-interrupt-driven-command-console.c \
../Source/Examples/UART/UART-operation-modes-test.c 

OBJS += \
./Source/Examples/UART/CLI-commands.o \
./Source/Examples/UART/UART-interrupt-driven-command-console.o \
./Source/Examples/UART/UART-operation-modes-test.o 

C_DEPS += \
./Source/Examples/UART/CLI-commands.d \
./Source/Examples/UART/UART-interrupt-driven-command-console.d \
./Source/Examples/UART/UART-operation-modes-test.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/UART/%.o: ../Source/Examples/UART/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_LPCOPEN -D__REDLIB__ -DCORE_M3 -DDEBUG -D__CODE_RED -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_chip_175x_6x/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-CLI" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailD_semi/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


