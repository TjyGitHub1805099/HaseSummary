#Progress monitor hint: 320
first : all

RM := rm -rf


-include cfg/gen/c_makefile.mk
-include cfg/gen/assmble_makefile.mk


ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif



# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
app.elf \

EXECUTABLES_QUOTED += \
"app.elf" \

EXECUTABLES_OS_FORMAT += \
app.elf \

SECONDARY_FLASH += \
app.bin \

SECONDARY_FLASH_QUOTED += \
"app.bin" \

SECONDARY_FLASH_OS_FORMAT += \
app.bin \

SECONDARY_SIZE += \
app.siz \

SECONDARY_SIZE_QUOTED += \
"app.siz" \

SECONDARY_SIZE_OS_FORMAT += \
app.siz \


# All Target
all: main-build

# Main-build Target
main-build: warning-messages explicit-dependencies app.elf secondary-outputs

# Tool invocations
app.elf: $(OBJS) ../Project_Settings/Linker_Files/S32K148_256_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #321 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "app.elf" "@cfg/gen/objListFile.args" "@cfg/gcc_lib_cfg.args" "@cfg/app.args" $(USER_OBJS_QUOTED) 
	@echo 'Finished building target: $@'
	@echo ' '

app.bin: $(EXECUTABLES)
	@echo 'Executing target #322 $@'
	@echo 'Invoking: Standard S32DS Create Flash Image'
	arm-none-eabi-objcopy -O binary $(EXECUTABLES)  "app.bin"
	@echo 'Finished building: $@'
	@echo ' '

app.siz: $(EXECUTABLES)
	@echo 'Executing target #323 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_FLASH_QUOTED) ./output/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

# Additional tool dependencies as separate targets to avoid errors while using parallel builds when pre-build steps are generating those dependencies.

warning-messages: 

