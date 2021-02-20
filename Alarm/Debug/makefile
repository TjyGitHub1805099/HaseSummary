################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 1
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ARCHIVES += \
libAlarm.a \

ARCHIVES_QUOTED += \
"libAlarm.a" \

ARCHIVES_OS_FORMAT += \
libAlarm.a \


# All Target
all: warning-messages explicit-dependencies libAlarm.a secondary-outputs

# Tool invocations
libAlarm.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #2 $@'
	@echo 'Invoking: Standard S32DS Archiver'
	arm-none-eabi-ar -r  "libAlarm.a" $(OBJS_QUOTED) $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(ARCHIVES_QUOTED) ./*/*.d  ./*/*.o 
	-@echo ' '

post-build:
	-@echo 'Post build: Copy library to platform and release folder.'
	-sh.exe E:\workspace\S32K\ioc\Alarm\postBuild.sh E:\workspace\S32K\ioc\Alarm Alarm
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY: post-build

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

