################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/CI2C1.c \
../Generated_Code/Cpu.c \
../Generated_Code/CsIO1.c \
../Generated_Code/FX1.c \
../Generated_Code/GI2C1.c \
../Generated_Code/IO1.c \
../Generated_Code/MCUC1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/Pins1.c \
../Generated_Code/SM1.c \
../Generated_Code/WAIT1.c 

OBJS += \
./Generated_Code/CI2C1.o \
./Generated_Code/Cpu.o \
./Generated_Code/CsIO1.o \
./Generated_Code/FX1.o \
./Generated_Code/GI2C1.o \
./Generated_Code/IO1.o \
./Generated_Code/MCUC1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/Pins1.o \
./Generated_Code/SM1.o \
./Generated_Code/WAIT1.o 

C_DEPS += \
./Generated_Code/CI2C1.d \
./Generated_Code/Cpu.d \
./Generated_Code/CsIO1.d \
./Generated_Code/FX1.d \
./Generated_Code/GI2C1.d \
./Generated_Code/IO1.d \
./Generated_Code/MCUC1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/Pins1.d \
./Generated_Code/SM1.d \
./Generated_Code/WAIT1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/Users/amanc/Documents/KSDK/EEProjectFinal/Static_Code/System" -I"C:/Users/amanc/Documents/KSDK/EEProjectFinal/Static_Code/PDD" -I"C:/Users/amanc/Documents/KSDK/EEProjectFinal/Static_Code/IO_Map" -I"C:\Freescale\KDS_v3\eclipse\ProcessorExpert/lib/Kinetis/pdd/inc" -I"C:/Users/amanc/Documents/KSDK/EEProjectFinal/Sources" -I"C:/Users/amanc/Documents/KSDK/EEProjectFinal/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


