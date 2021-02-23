OBJS += \
	startup_S32K148.o \

startup_S32K148.o: ../Project_Settings/Startup_Code/startup_S32K148.S
	arm-none-eabi-gcc "@cfg/startup_S32K148.args" -c ../Project_Settings/Startup_Code/startup_S32K148.S -o output/startup_S32K148.o
