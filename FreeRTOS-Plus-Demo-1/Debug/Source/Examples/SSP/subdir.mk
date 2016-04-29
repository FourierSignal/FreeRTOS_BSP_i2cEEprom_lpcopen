################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/SSP/SPI-writes-to-7-seg-display.c 

OBJS += \
./Source/Examples/SSP/SPI-writes-to-7-seg-display.o 

C_DEPS += \
./Source/Examples/SSP/SPI-writes-to-7-seg-display.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/SSP/%.o: ../Source/Examples/SSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCORE_M3 -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/lpc_chip_175x_6x/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/lpc_board_nxp_lpcxpresso_1769/inc" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/SupportedBoards" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS-Plus-CLI" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/FreeRTOS-Products/FreeRTOS/include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source/Examples/Include" -I"/home/jaguar/Documents/FreeRtos/nxp/lpcopen_trailA/FreeRTOS-Plus-Demo-1/Source" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


