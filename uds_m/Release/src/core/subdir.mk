################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/core/download.c" \
"../src/core/dtcCore.c" \
"../src/core/sid0x10.c" \
"../src/core/sid0x11.c" \
"../src/core/sid0x14.c" \
"../src/core/sid0x22.c" \
"../src/core/sid0x23.c" \
"../src/core/sid0x27.c" \
"../src/core/sid0x28.c" \
"../src/core/sid0x2f.c" \
"../src/core/sid0x31.c" \
"../src/core/sid0x85.c" \
"../src/core/uds.c" \
"../src/core/uds_sid.c" \

C_SRCS += \
../src/core/download.c \
../src/core/dtcCore.c \
../src/core/sid0x10.c \
../src/core/sid0x11.c \
../src/core/sid0x14.c \
../src/core/sid0x22.c \
../src/core/sid0x23.c \
../src/core/sid0x27.c \
../src/core/sid0x28.c \
../src/core/sid0x2f.c \
../src/core/sid0x31.c \
../src/core/sid0x85.c \
../src/core/uds.c \
../src/core/uds_sid.c \

OBJS_OS_FORMAT += \
./src/core/download.o \
./src/core/dtcCore.o \
./src/core/sid0x10.o \
./src/core/sid0x11.o \
./src/core/sid0x14.o \
./src/core/sid0x22.o \
./src/core/sid0x23.o \
./src/core/sid0x27.o \
./src/core/sid0x28.o \
./src/core/sid0x2f.o \
./src/core/sid0x31.o \
./src/core/sid0x85.o \
./src/core/uds.o \
./src/core/uds_sid.o \

C_DEPS_QUOTED += \
"./src/core/download.d" \
"./src/core/dtcCore.d" \
"./src/core/sid0x10.d" \
"./src/core/sid0x11.d" \
"./src/core/sid0x14.d" \
"./src/core/sid0x22.d" \
"./src/core/sid0x23.d" \
"./src/core/sid0x27.d" \
"./src/core/sid0x28.d" \
"./src/core/sid0x2f.d" \
"./src/core/sid0x31.d" \
"./src/core/sid0x85.d" \
"./src/core/uds.d" \
"./src/core/uds_sid.d" \

OBJS += \
./src/core/download.o \
./src/core/dtcCore.o \
./src/core/sid0x10.o \
./src/core/sid0x11.o \
./src/core/sid0x14.o \
./src/core/sid0x22.o \
./src/core/sid0x23.o \
./src/core/sid0x27.o \
./src/core/sid0x28.o \
./src/core/sid0x2f.o \
./src/core/sid0x31.o \
./src/core/sid0x85.o \
./src/core/uds.o \
./src/core/uds_sid.o \

OBJS_QUOTED += \
"./src/core/download.o" \
"./src/core/dtcCore.o" \
"./src/core/sid0x10.o" \
"./src/core/sid0x11.o" \
"./src/core/sid0x14.o" \
"./src/core/sid0x22.o" \
"./src/core/sid0x23.o" \
"./src/core/sid0x27.o" \
"./src/core/sid0x28.o" \
"./src/core/sid0x2f.o" \
"./src/core/sid0x31.o" \
"./src/core/sid0x85.o" \
"./src/core/uds.o" \
"./src/core/uds_sid.o" \

C_DEPS += \
./src/core/download.d \
./src/core/dtcCore.d \
./src/core/sid0x10.d \
./src/core/sid0x11.d \
./src/core/sid0x14.d \
./src/core/sid0x22.d \
./src/core/sid0x23.d \
./src/core/sid0x27.d \
./src/core/sid0x28.d \
./src/core/sid0x2f.d \
./src/core/sid0x31.d \
./src/core/sid0x85.d \
./src/core/uds.d \
./src/core/uds_sid.d \


# Each subdirectory must supply rules for building sources it contributes
src/core/download.o: ../src/core/download.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/download.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/download.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/dtcCore.o: ../src/core/dtcCore.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/dtcCore.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/dtcCore.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x10.o: ../src/core/sid0x10.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x10.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x10.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x11.o: ../src/core/sid0x11.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x11.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x11.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x14.o: ../src/core/sid0x14.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x14.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x14.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x22.o: ../src/core/sid0x22.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x22.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x22.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x23.o: ../src/core/sid0x23.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x23.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x23.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x27.o: ../src/core/sid0x27.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x27.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x27.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x28.o: ../src/core/sid0x28.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x28.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x28.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x2f.o: ../src/core/sid0x2f.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x2f.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x2f.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x31.o: ../src/core/sid0x31.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x31.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x31.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/sid0x85.o: ../src/core/sid0x85.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/sid0x85.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/sid0x85.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/uds.o: ../src/core/uds.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/uds.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/uds.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/uds_sid.o: ../src/core/uds_sid.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/core/uds_sid.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/core/uds_sid.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


