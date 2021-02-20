################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/Alarm.c" \

C_SRCS += \
../src/Alarm.c \

OBJS_OS_FORMAT += \
./src/Alarm.o \

C_DEPS_QUOTED += \
"./src/Alarm.d" \

OBJS += \
./src/Alarm.o \

OBJS_QUOTED += \
"./src/Alarm.o" \

C_DEPS += \
./src/Alarm.d \


# Each subdirectory must supply rules for building sources it contributes
src/Alarm.o: ../src/Alarm.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Alarm.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/Alarm.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


