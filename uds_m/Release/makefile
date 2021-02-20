################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 14
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ARCHIVES += \
libuds_m.a \

ARCHIVES_QUOTED += \
"libuds_m.a" \

ARCHIVES_OS_FORMAT += \
libuds_m.a \


# All Target
all: warning-messages explicit-dependencies libuds_m.a secondary-outputs

# Tool invocations
libuds_m.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #15 $@'
	@echo 'Invoking: Standard S32DS Archiver'
	arm-none-eabi-ar -r  "libuds_m.a" $(OBJS_QUOTED) $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ARCHIVES_QUOTED) ./*/*/*.d  ./*/*/*.o 
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 

