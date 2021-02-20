OBJS += \
	dea.o \
	DeaCoding.o \
	DeaDiag.o \
	main.o \
	rtos.o \
	SWVersion.o \
	Task_Alarm.o \
	audio_control.o \
	audio_ipc_protocol.o \
	dsp_radio_init.o \
	dsp_spi.o \
	Task_Audio.o \
	tuner.o \
	afc.o \
	afc_cfg.o \
	avs.o \
	avs_cfg.o \
	bc_config_para.o \
	Clk.o \
	alga.o \
	cti.o \
	cti_cfg.o \
	dte.o \
	Dte_Cfg.o \
	FuelConfig.o \
	FuelProc.o \
	Gear.o \
	HistoryAfc.o \
	IdleStopTime.o \
	ifc.o \
	ifc_cfg.o \
	IpcClr.o \
	oat.o \
	odo.o \
	Odo_cfg.o \
	Rev.o \
	Rev_cfg.o \
	MaintainService.o \
	MaintainServiceCfg.o \
	Task_BC.o \
	tii.o \
	tii_cfg.o \
	TravelTime.o \
	unit.o \
	Vel.o \
	Vel_cfg.o \
	VEL_LT.o \
	VEL_LT_cfg.o \
	CanApp.o \
	CanAppDiagClient.o \
	CanApp_Cbk.o \
	CanApp_Cfg.o \
	CanApp_PduChanged.o \
	CanApp_PduMsgProc.o \
	CanApp_TxProc.o \
	CanMixProc.o \
	semi_active_check.o \
	Task_Com.o \
	VelSpdPulseOut.o \
	Task_Diag.o \
	Task_DiagApp.o \
	amp_control.o \
	amp_control_tdf853x.o \
	amp_i2c.o \
	Task_DigitalAMP.o \
	tdf853x.o \
	tdf853x_common.o \
	Task_General.o \
	a3g4250d_driver.o \
	ais328dq_driver.o \
	gyroacc_service.o \
	i2c_mems.o \
	Task_GyroAcc.o \
	sys_setting.o \
	Task_IpcApp.o \
	Task_IpcSem.o \
	ipc_hb_with_ivi.o \
	Task_M_IpcApp.o \
	Task_Nm.o \
	pmStateMachine.o \
	Task_PowerModing.o \
	adc0_pal_cfg.o \
	adc1_pal_cfg.o \
	can0_cfg.o \
	can1_cfg.o \
	clockManager_cfg.o \
	Cpu.o \
	dmaController_cfg.o \
	FanBack_Pwm2_Cap.o \
	flash_cfg.o \
	flexTimer_mc1.o \
	flexTimer_oc1.o \
	flexTimer_pwm1_cfg.o \
	flexTimer_pwm3_cfg.o \
	flexTimer_pwm4_cfg.o \
	flexTimer_pwm5_cfg.o \
	flexTimer_pwm7_cfg.o \
	hwCrc_cfg.o \
	lpi2c0_cfg.o \
	lpi2c1_cfg.o \
	pin_mux.o \
	pwrManager_cfg.o \
	rtcTimer_cfg.o \
	spi_ipc_pal_cfg.o \
	spi_M_ipc_pal_cfg.o \
	spi_saf775d.o \
	uart0_pal_cfg.o \
	uart_acpanel_cfg.o \
	uart_debug_pal.o \
	uart_gps_cfg.o \
	uart_ipc_pal.o \
	wdog_pal_cfg.o \
	acce.o \
	AC_CTL.o \
	adc_wrapper.o \
	adc_wrapper_cfg.o \
	AppInfo.o \
	audio.o \
	KeyBacklight.o \
	LcdBacklight.o \
	backram_var.o \
	BinSig_Acc.o \
	BinSig_Ign.o \
	CanIf.o \
	CanIf_Cfg.o \
	CanNm.o \
	CanNm_Cfg.o \
	Com.o \
	Com_Cbk.o \
	Com_Cfg.o \
	Com_If.o \
	Nm.o \
	Nm_Cfg.o \
	CircleList.o \
	CircleList_cfg.o \
	CircularQueue.o \
	CircularQueue_Cfg.o \
	Cpu_UsageI.o \
	Delay.o \
	ame.o \
	ame_cbk.o \
	ame_cfg.o \
	diagAppIpc_cbk.o \
	diagAppSid0x22.o \
	diagAppSid0x2f.o \
	diagAppSid0x31.o \
	diagAppWllr_cbk.o \
	diagAppWllr_cfg.o \
	download.o \
	dtcCore.o \
	sid0x10.o \
	sid0x11.o \
	sid0x14.o \
	sid0x22.o \
	sid0x23.o \
	sid0x27.o \
	sid0x28.o \
	sid0x2f.o \
	sid0x31.o \
	sid0x85.o \
	uds.o \
	uds_sid.o \
	diagCommon_cfg.o \
	download_cbk.o \
	download_cfg.o \
	dtcApp_cbk.o \
	dtcApp_cfg.o \
	dtcCore_cbk.o \
	dtcCore_cfg.o \
	SecurityAlgorithm.o \
	sid0x10_cbk.o \
	sid0x10_cfg.o \
	sid0x11_cbk.o \
	sid0x11_cfg.o \
	sid0x14_cbk.o \
	sid0x14_cfg.o \
	sid0x22_cbk.o \
	sid0x22_cfg.o \
	sid0x23_cbk.o \
	sid0x23_cfg.o \
	sid0x27_cbk.o \
	sid0x27_cfg.o \
	sid0x28_cbk.o \
	sid0x28_cfg.o \
	sid0x2f_cbk.o \
	sid0x2f_cfg.o \
	sid0x31_cbk.o \
	sid0x31_cfg.o \
	sid0x85_cbk.o \
	sid0x85_cfg.o \
	sid0x99_cbk.o \
	uds_cbk.o \
	uds_cfg.o \
	uds_sid_cbk.o \
	uds_sid_cfg.o \
	Eeprom.o \
	EncoderEC11.o \
	ExternRTC.o \
	Ex_Wdg.o \
	FanCtrl.o \
	SimFlag.o \
	Flash_if.o \
	Gps_Parse.o \
	gyro.o \
	HardwareVersion.o \
	iic_if.o \
	iic_if_cfg.o \
	tp.o \
	tpConfig.o \
	AdKey.o \
	CanKey.o \
	HardKey.o \
	Key.o \
	PanelKey.o \
	Crc.o \
	Crc_Cfg.o \
	Math_NumFormat.o \
	Math_Random.o \
	Ea_Cbk.o \
	Ea_Cfg.o \
	Memif.o \
	NvM_Cbk.o \
	NvM_Cfg.o \
	PowerSupply.o \
	BattVoltage.o \
	PriQueue_Cfg.o \
	rtc.o \
	SpeedOutput.o \
	flowcontrol.o \
	hashmap.o \
	ipc.o \
	SPI_CircularQueue.o \
	Spi_IpcCore.o \
	ipc_gw_core.o \
	Spi_Ipc_gw_api.o \
	ipc_gw_adaptor.o \
	SpiIpc_cfg.o \
	spiMutex.o \
	spiOsDelay.o \
	spiSem.o \
	Spi_if.o \
	TaskSpi_IpcCore.o \
	Standby.o \
	Swdl.o \
	SysWakeUp.o \
	TempAmp.o \
	TempBoard.o \
	TempConvert.o \
	trace.o \
	WaterTempWarn.o \
	Alarm_Cfg.o \
	System_Mutex.o \
	System_Task_Cfg.o \
	System_Tick.o \
	VirtualSig_Cfg.o \
	system_S32K148.o \
	startup.o \
	adc_driver.o \
	clock_manager.o \
	clock_S32K1xx.o \
	crc_driver.o \
	crc_hw_access.o \
	edma_driver.o \
	edma_hw_access.o \
	edma_irq.o \
	flash_driver.o \
	flexcan_driver.o \
	flexcan_hw_access.o \
	flexcan_irq.o \
	flexio_common.o \
	flexio_i2c_driver.o \
	flexio_i2s_driver.o \
	flexio_spi_driver.o \
	flexio_uart_driver.o \
	ftm_common.o \
	ftm_hw_access.o \
	ftm_ic_driver.o \
	ftm_mc_driver.o \
	ftm_oc_driver.o \
	ftm_pwm_driver.o \
	ftm_qd_driver.o \
	interrupt_manager.o \
	lpi2c_driver.o \
	lpi2c_hw_access.o \
	lpi2c_irq.o \
	lpit_driver.o \
	lpit_hw_access.o \
	lpspi_hw_access.o \
	lpspi_irq.o \
	lpspi_master_driver.o \
	lpspi_shared_function.o \
	lpspi_slave_driver.o \
	lin_lpuart_driver.o \
	lpuart_driver.o \
	lpuart_hw_access.o \
	lpuart_irq.o \
	mpu_driver.o \
	mpu_hw_access.o \
	pdb_driver.o \
	pdb_hw_access.o \
	pins_driver.o \
	pins_port_hw_access.o \
	power_manager.o \
	power_manager_S32K1xx.o \
	power_pmc_hw_access.o \
	power_rcm_hw_access.o \
	power_smc_hw_access.o \
	rtc_driver.o \
	rtc_hw_access.o \
	rtc_irq.o \
	trgmux_driver.o \
	trgmux_hw_access.o \
	wdog_driver.o \
	wdog_hw_access.o \
	adc_irq.o \
	adc_pal.o \
	spi_pal.o \
	uart_pal.o \
	wdg_pal.o \
	croutine.o \
	event_groups.o \
	list.o \
	port.o \
	heap_2.o \
	queue.o \
	tasks.o \
	timers.o \
	osif_freertos.o \


dea.o: ../APP/DEA/dea.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/DEA/dea.c -o output/dea.o
DeaCoding.o: ../APP/DEA/DeaCoding.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/DEA/DeaCoding.c -o output/DeaCoding.o
DeaDiag.o: ../APP/DEA/DeaDiag.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/DEA/DeaDiag.c -o output/DeaDiag.o
main.o: ../APP/main.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/main.c -o output/main.o
rtos.o: ../APP/rtos.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/rtos.c -o output/rtos.o
SWVersion.o: ../APP/SWVersion/SWVersion.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/SWVersion/SWVersion.c -o output/SWVersion.o
Task_Alarm.o: ../APP/Task_Alarm/Task_Alarm.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Alarm/Task_Alarm.c -o output/Task_Alarm.o
audio_control.o: ../APP/Task_Audio/audio_control/audio_control.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/audio_control/audio_control.c -o output/audio_control.o
audio_ipc_protocol.o: ../APP/Task_Audio/audio_control/audio_ipc_protocol.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/audio_control/audio_ipc_protocol.c -o output/audio_ipc_protocol.o
dsp_radio_init.o: ../APP/Task_Audio/spi/dsp_radio_init.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/spi/dsp_radio_init.c -o output/dsp_radio_init.o
dsp_spi.o: ../APP/Task_Audio/spi/dsp_spi.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/spi/dsp_spi.c -o output/dsp_spi.o
Task_Audio.o: ../APP/Task_Audio/Task_Audio.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/Task_Audio.c -o output/Task_Audio.o
tuner.o: ../APP/Task_Audio/tuner/tuner.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Audio/tuner/tuner.c -o output/tuner.o
afc.o: ../APP/Task_BC/AFC/afc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/AFC/afc.c -o output/afc.o
afc_cfg.o: ../APP/Task_BC/AFC/afc_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/AFC/afc_cfg.c -o output/afc_cfg.o
avs.o: ../APP/Task_BC/AVS/avs.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/AVS/avs.c -o output/avs.o
avs_cfg.o: ../APP/Task_BC/AVS/avs_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/AVS/avs_cfg.c -o output/avs_cfg.o
bc_config_para.o: ../APP/Task_BC/bc_config_para/bc_config_para.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/bc_config_para/bc_config_para.c -o output/bc_config_para.o
Clk.o: ../APP/Task_BC/CLK/Clk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/CLK/Clk.c -o output/Clk.o
alga.o: ../APP/Task_BC/Common/alga.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Common/alga.c -o output/alga.o
cti.o: ../APP/Task_BC/CTI/cti.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/CTI/cti.c -o output/cti.o
cti_cfg.o: ../APP/Task_BC/CTI/cti_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/CTI/cti_cfg.c -o output/cti_cfg.o
dte.o: ../APP/Task_BC/DTE/dte.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/DTE/dte.c -o output/dte.o
Dte_Cfg.o: ../APP/Task_BC/DTE/Dte_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/DTE/Dte_Cfg.c -o output/Dte_Cfg.o
FuelConfig.o: ../APP/Task_BC/Fuel/FuelConfig.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Fuel/FuelConfig.c -o output/FuelConfig.o
FuelProc.o: ../APP/Task_BC/Fuel/FuelProc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Fuel/FuelProc.c -o output/FuelProc.o
Gear.o: ../APP/Task_BC/Gear/Gear.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Gear/Gear.c -o output/Gear.o
HistoryAfc.o: ../APP/Task_BC/H_AFV/HistoryAfc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/H_AFV/HistoryAfc.c -o output/HistoryAfc.o
IdleStopTime.o: ../APP/Task_BC/IdleStopTime/IdleStopTime.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/IdleStopTime/IdleStopTime.c -o output/IdleStopTime.o
ifc.o: ../APP/Task_BC/IFC/ifc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/IFC/ifc.c -o output/ifc.o
ifc_cfg.o: ../APP/Task_BC/IFC/ifc_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/IFC/ifc_cfg.c -o output/ifc_cfg.o
IpcClr.o: ../APP/Task_BC/IpcClear/IpcClr.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/IpcClear/IpcClr.c -o output/IpcClr.o
oat.o: ../APP/Task_BC/OAT/oat.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/OAT/oat.c -o output/oat.o
odo.o: ../APP/Task_BC/odo/odo.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/odo/odo.c -o output/odo.o
Odo_cfg.o: ../APP/Task_BC/odo/Odo_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/odo/Odo_cfg.c -o output/Odo_cfg.o
Rev.o: ../APP/Task_BC/REV/Rev.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/REV/Rev.c -o output/Rev.o
Rev_cfg.o: ../APP/Task_BC/REV/Rev_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/REV/Rev_cfg.c -o output/Rev_cfg.o
MaintainService.o: ../APP/Task_BC/SERV/MaintainService.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/SERV/MaintainService.c -o output/MaintainService.o
MaintainServiceCfg.o: ../APP/Task_BC/SERV/MaintainServiceCfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/SERV/MaintainServiceCfg.c -o output/MaintainServiceCfg.o
Task_BC.o: ../APP/Task_BC/Task_BC.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Task_BC.c -o output/Task_BC.o
tii.o: ../APP/Task_BC/TII/tii.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/TII/tii.c -o output/tii.o
tii_cfg.o: ../APP/Task_BC/TII/tii_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/TII/tii_cfg.c -o output/tii_cfg.o
TravelTime.o: ../APP/Task_BC/TravelTime/TravelTime.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/TravelTime/TravelTime.c -o output/TravelTime.o
unit.o: ../APP/Task_BC/unit/unit.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/unit/unit.c -o output/unit.o
Vel.o: ../APP/Task_BC/Vel/Vel.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Vel/Vel.c -o output/Vel.o
Vel_cfg.o: ../APP/Task_BC/Vel/Vel_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/Vel/Vel_cfg.c -o output/Vel_cfg.o
VEL_LT.o: ../APP/Task_BC/VEL_LT/VEL_LT.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/VEL_LT/VEL_LT.c -o output/VEL_LT.o
VEL_LT_cfg.o: ../APP/Task_BC/VEL_LT/VEL_LT_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_BC/VEL_LT/VEL_LT_cfg.c -o output/VEL_LT_cfg.o
CanApp.o: ../APP/Task_Com/CanApp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp.c -o output/CanApp.o
CanAppDiagClient.o: ../APP/Task_Com/CanAppDiagClient.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanAppDiagClient.c -o output/CanAppDiagClient.o
CanApp_Cbk.o: ../APP/Task_Com/CanApp_Cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp_Cbk.c -o output/CanApp_Cbk.o
CanApp_Cfg.o: ../APP/Task_Com/CanApp_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp_Cfg.c -o output/CanApp_Cfg.o
CanApp_PduChanged.o: ../APP/Task_Com/CanApp_PduChanged.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp_PduChanged.c -o output/CanApp_PduChanged.o
CanApp_PduMsgProc.o: ../APP/Task_Com/CanApp_PduMsgProc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp_PduMsgProc.c -o output/CanApp_PduMsgProc.o
CanApp_TxProc.o: ../APP/Task_Com/CanApp_TxProc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanApp_TxProc.c -o output/CanApp_TxProc.o
CanMixProc.o: ../APP/Task_Com/CanMixProc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/CanMixProc.c -o output/CanMixProc.o
semi_active_check.o: ../APP/Task_Com/semi_active_check.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/semi_active_check.c -o output/semi_active_check.o
Task_Com.o: ../APP/Task_Com/Task_Com.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/Task_Com.c -o output/Task_Com.o
VelSpdPulseOut.o: ../APP/Task_Com/VelSpdPulseOut.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Com/VelSpdPulseOut.c -o output/VelSpdPulseOut.o
Task_Diag.o: ../APP/Task_Diag/Task_Diag.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Diag/Task_Diag.c -o output/Task_Diag.o
Task_DiagApp.o: ../APP/Task_DiagApp/Task_DiagApp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DiagApp/Task_DiagApp.c -o output/Task_DiagApp.o
amp_control.o: ../APP/Task_DigitalAMP/amp_control/amp_control.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/amp_control/amp_control.c -o output/amp_control.o
amp_control_tdf853x.o: ../APP/Task_DigitalAMP/amp_control/amp_control_tdf853x.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/amp_control/amp_control_tdf853x.c -o output/amp_control_tdf853x.o
amp_i2c.o: ../APP/Task_DigitalAMP/i2c/amp_i2c.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/i2c/amp_i2c.c -o output/amp_i2c.o
Task_DigitalAMP.o: ../APP/Task_DigitalAMP/Task_DigitalAMP.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/Task_DigitalAMP.c -o output/Task_DigitalAMP.o
tdf853x.o: ../APP/Task_DigitalAMP/tdf853x/tdf853x.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/tdf853x/tdf853x.c -o output/tdf853x.o
tdf853x_common.o: ../APP/Task_DigitalAMP/tdf853x/tdf853x_common.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_DigitalAMP/tdf853x/tdf853x_common.c -o output/tdf853x_common.o
Task_General.o: ../APP/Task_General/Task_General.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_General/Task_General.c -o output/Task_General.o
a3g4250d_driver.o: ../APP/Task_GyroAcc/a3g4250d_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_GyroAcc/a3g4250d_driver.c -o output/a3g4250d_driver.o
ais328dq_driver.o: ../APP/Task_GyroAcc/ais328dq_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_GyroAcc/ais328dq_driver.c -o output/ais328dq_driver.o
gyroacc_service.o: ../APP/Task_GyroAcc/gyroacc_service.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_GyroAcc/gyroacc_service.c -o output/gyroacc_service.o
i2c_mems.o: ../APP/Task_GyroAcc/i2c_mems.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_GyroAcc/i2c_mems.c -o output/i2c_mems.o
Task_GyroAcc.o: ../APP/Task_GyroAcc/Task_GyroAcc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_GyroAcc/Task_GyroAcc.c -o output/Task_GyroAcc.o
sys_setting.o: ../APP/Task_IpcApp/sys_setting.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_IpcApp/sys_setting.c -o output/sys_setting.o
Task_IpcApp.o: ../APP/Task_IpcApp/Task_IpcApp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_IpcApp/Task_IpcApp.c -o output/Task_IpcApp.o
Task_IpcSem.o: ../APP/Task_IpcApp/Task_IpcSem.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_IpcApp/Task_IpcSem.c -o output/Task_IpcSem.o
ipc_hb_with_ivi.o: ../APP/Task_M_IpcApp/ipc_hb_with_ivi.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_M_IpcApp/ipc_hb_with_ivi.c -o output/ipc_hb_with_ivi.o
Task_M_IpcApp.o: ../APP/Task_M_IpcApp/Task_M_IpcApp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_M_IpcApp/Task_M_IpcApp.c -o output/Task_M_IpcApp.o
Task_Nm.o: ../APP/Task_Nm/Task_Nm.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_Nm/Task_Nm.c -o output/Task_Nm.o
pmStateMachine.o: ../APP/Task_PowerModing/pmStateMachine.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_PowerModing/pmStateMachine.c -o output/pmStateMachine.o
Task_PowerModing.o: ../APP/Task_PowerModing/Task_PowerModing.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../APP/Task_PowerModing/Task_PowerModing.c -o output/Task_PowerModing.o
adc0_pal_cfg.o: ../Generated_Code/adc0_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/adc0_pal_cfg.c -o output/adc0_pal_cfg.o
adc1_pal_cfg.o: ../Generated_Code/adc1_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/adc1_pal_cfg.c -o output/adc1_pal_cfg.o
can0_cfg.o: ../Generated_Code/can0_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/can0_cfg.c -o output/can0_cfg.o
can1_cfg.o: ../Generated_Code/can1_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/can1_cfg.c -o output/can1_cfg.o
clockManager_cfg.o: ../Generated_Code/clockManager_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/clockManager_cfg.c -o output/clockManager_cfg.o
Cpu.o: ../Generated_Code/Cpu.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/Cpu.c -o output/Cpu.o
dmaController_cfg.o: ../Generated_Code/dmaController_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/dmaController_cfg.c -o output/dmaController_cfg.o
FanBack_Pwm2_Cap.o: ../Generated_Code/FanBack_Pwm2_Cap.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/FanBack_Pwm2_Cap.c -o output/FanBack_Pwm2_Cap.o
flash_cfg.o: ../Generated_Code/flash_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flash_cfg.c -o output/flash_cfg.o
flexTimer_mc1.o: ../Generated_Code/flexTimer_mc1.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_mc1.c -o output/flexTimer_mc1.o
flexTimer_oc1.o: ../Generated_Code/flexTimer_oc1.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_oc1.c -o output/flexTimer_oc1.o
flexTimer_pwm1_cfg.o: ../Generated_Code/flexTimer_pwm1_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_pwm1_cfg.c -o output/flexTimer_pwm1_cfg.o
flexTimer_pwm3_cfg.o: ../Generated_Code/flexTimer_pwm3_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_pwm3_cfg.c -o output/flexTimer_pwm3_cfg.o
flexTimer_pwm4_cfg.o: ../Generated_Code/flexTimer_pwm4_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_pwm4_cfg.c -o output/flexTimer_pwm4_cfg.o
flexTimer_pwm5_cfg.o: ../Generated_Code/flexTimer_pwm5_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_pwm5_cfg.c -o output/flexTimer_pwm5_cfg.o
flexTimer_pwm7_cfg.o: ../Generated_Code/flexTimer_pwm7_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/flexTimer_pwm7_cfg.c -o output/flexTimer_pwm7_cfg.o
hwCrc_cfg.o: ../Generated_Code/hwCrc_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/hwCrc_cfg.c -o output/hwCrc_cfg.o
lpi2c0_cfg.o: ../Generated_Code/lpi2c0_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/lpi2c0_cfg.c -o output/lpi2c0_cfg.o
lpi2c1_cfg.o: ../Generated_Code/lpi2c1_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/lpi2c1_cfg.c -o output/lpi2c1_cfg.o
pin_mux.o: ../Generated_Code/pin_mux.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/pin_mux.c -o output/pin_mux.o
pwrManager_cfg.o: ../Generated_Code/pwrManager_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/pwrManager_cfg.c -o output/pwrManager_cfg.o
rtcTimer_cfg.o: ../Generated_Code/rtcTimer_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/rtcTimer_cfg.c -o output/rtcTimer_cfg.o
spi_ipc_pal_cfg.o: ../Generated_Code/spi_ipc_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/spi_ipc_pal_cfg.c -o output/spi_ipc_pal_cfg.o
spi_M_ipc_pal_cfg.o: ../Generated_Code/spi_M_ipc_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/spi_M_ipc_pal_cfg.c -o output/spi_M_ipc_pal_cfg.o
spi_saf775d.o: ../Generated_Code/spi_saf775d.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/spi_saf775d.c -o output/spi_saf775d.o
uart0_pal_cfg.o: ../Generated_Code/uart0_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/uart0_pal_cfg.c -o output/uart0_pal_cfg.o
uart_acpanel_cfg.o: ../Generated_Code/uart_acpanel_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/uart_acpanel_cfg.c -o output/uart_acpanel_cfg.o
uart_debug_pal.o: ../Generated_Code/uart_debug_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/uart_debug_pal.c -o output/uart_debug_pal.o
uart_gps_cfg.o: ../Generated_Code/uart_gps_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/uart_gps_cfg.c -o output/uart_gps_cfg.o
uart_ipc_pal.o: ../Generated_Code/uart_ipc_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/uart_ipc_pal.c -o output/uart_ipc_pal.o
wdog_pal_cfg.o: ../Generated_Code/wdog_pal_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../Generated_Code/wdog_pal_cfg.c -o output/wdog_pal_cfg.o
acce.o: ../SDK/middleware/Acce/acce.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Acce/acce.c -o output/acce.o
AC_CTL.o: ../SDK/middleware/AcCtl/AC_CTL.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/AcCtl/AC_CTL.c -o output/AC_CTL.o
adc_wrapper.o: ../SDK/middleware/adc_wrapper/adc_wrapper.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/adc_wrapper/adc_wrapper.c -o output/adc_wrapper.o
adc_wrapper_cfg.o: ../SDK/middleware/adc_wrapper/adc_wrapper_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/adc_wrapper/adc_wrapper_cfg.c -o output/adc_wrapper_cfg.o
AppInfo.o: ../SDK/middleware/AppInfo/AppInfo.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/AppInfo/AppInfo.c -o output/AppInfo.o
audio.o: ../SDK/middleware/Audio/audio.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Audio/audio.c -o output/audio.o
KeyBacklight.o: ../SDK/middleware/Backlight/KeyBacklight/KeyBacklight.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Backlight/KeyBacklight/KeyBacklight.c -o output/KeyBacklight.o
LcdBacklight.o: ../SDK/middleware/Backlight/LcdBacklight/LcdBacklight.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Backlight/LcdBacklight/LcdBacklight.c -o output/LcdBacklight.o
backram_var.o: ../SDK/middleware/backRam/backram_var.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/backRam/backram_var.c -o output/backram_var.o
BinSig_Acc.o: ../SDK/middleware/BinarySignal/BinSig_Acc/BinSig_Acc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/BinarySignal/BinSig_Acc/BinSig_Acc.c -o output/BinSig_Acc.o
BinSig_Ign.o: ../SDK/middleware/BinarySignal/BinSig_Ign/BinSig_Ign.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/BinarySignal/BinSig_Ign/BinSig_Ign.c -o output/BinSig_Ign.o
CanIf.o: ../SDK/middleware/CAN/CanIf/CanIf.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/CanIf/CanIf.c -o output/CanIf.o
CanIf_Cfg.o: ../SDK/middleware/CAN/CanIf/CanIf_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/CanIf/CanIf_Cfg.c -o output/CanIf_Cfg.o
CanNm.o: ../SDK/middleware/CAN/CanNm/CanNm.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/CanNm/CanNm.c -o output/CanNm.o
CanNm_Cfg.o: ../SDK/middleware/CAN/CanNm/CanNm_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/CanNm/CanNm_Cfg.c -o output/CanNm_Cfg.o
Com.o: ../SDK/middleware/CAN/Com/Com.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/Com/Com.c -o output/Com.o
Com_Cbk.o: ../SDK/middleware/CAN/Com/Com_Cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/Com/Com_Cbk.c -o output/Com_Cbk.o
Com_Cfg.o: ../SDK/middleware/CAN/Com/Com_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/Com/Com_Cfg.c -o output/Com_Cfg.o
Com_If.o: ../SDK/middleware/CAN/Com/Com_If.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/Com/Com_If.c -o output/Com_If.o
Nm.o: ../SDK/middleware/CAN/NmIf/Nm.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/NmIf/Nm.c -o output/Nm.o
Nm_Cfg.o: ../SDK/middleware/CAN/NmIf/Nm_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CAN/NmIf/Nm_Cfg.c -o output/Nm_Cfg.o
CircleList.o: ../SDK/middleware/CircleList/CircleList.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CircleList/CircleList.c -o output/CircleList.o
CircleList_cfg.o: ../SDK/middleware/CircleList/CircleList_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CircleList/CircleList_cfg.c -o output/CircleList_cfg.o
CircularQueue.o: ../SDK/middleware/CircularQueue/CircularQueue.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CircularQueue/CircularQueue.c -o output/CircularQueue.o
CircularQueue_Cfg.o: ../SDK/middleware/CircularQueue/CircularQueue_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CircularQueue/CircularQueue_Cfg.c -o output/CircularQueue_Cfg.o
Cpu_UsageI.o: ../SDK/middleware/CPUUsage/Cpu_UsageI.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/CPUUsage/Cpu_UsageI.c -o output/Cpu_UsageI.o
Delay.o: ../SDK/middleware/Delay/Delay.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Delay/Delay.c -o output/Delay.o
ame.o: ../SDK/middleware/diag/ame/ame.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/ame/ame.c -o output/ame.o
ame_cbk.o: ../SDK/middleware/diag/ame/ame_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/ame/ame_cbk.c -o output/ame_cbk.o
ame_cfg.o: ../SDK/middleware/diag/ame/ame_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/ame/ame_cfg.c -o output/ame_cfg.o
diagAppIpc_cbk.o: ../SDK/middleware/diag/diagApp/diagAppIpc_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppIpc_cbk.c -o output/diagAppIpc_cbk.o
diagAppSid0x22.o: ../SDK/middleware/diag/diagApp/diagAppSid0x22.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppSid0x22.c -o output/diagAppSid0x22.o
diagAppSid0x2f.o: ../SDK/middleware/diag/diagApp/diagAppSid0x2f.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppSid0x2f.c -o output/diagAppSid0x2f.o
diagAppSid0x31.o: ../SDK/middleware/diag/diagApp/diagAppSid0x31.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppSid0x31.c -o output/diagAppSid0x31.o
diagAppWllr_cbk.o: ../SDK/middleware/diag/diagApp/diagAppWllr_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppWllr_cbk.c -o output/diagAppWllr_cbk.o
diagAppWllr_cfg.o: ../SDK/middleware/diag/diagApp/diagAppWllr_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/diagApp/diagAppWllr_cfg.c -o output/diagAppWllr_cfg.o
download.o: ../SDK/middleware/diag/uds/core/download.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/download.c -o output/download.o
dtcCore.o: ../SDK/middleware/diag/uds/core/dtcCore.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/dtcCore.c -o output/dtcCore.o
sid0x10.o: ../SDK/middleware/diag/uds/core/sid0x10.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x10.c -o output/sid0x10.o
sid0x11.o: ../SDK/middleware/diag/uds/core/sid0x11.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x11.c -o output/sid0x11.o
sid0x14.o: ../SDK/middleware/diag/uds/core/sid0x14.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x14.c -o output/sid0x14.o
sid0x22.o: ../SDK/middleware/diag/uds/core/sid0x22.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x22.c -o output/sid0x22.o
sid0x23.o: ../SDK/middleware/diag/uds/core/sid0x23.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x23.c -o output/sid0x23.o
sid0x27.o: ../SDK/middleware/diag/uds/core/sid0x27.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x27.c -o output/sid0x27.o
sid0x28.o: ../SDK/middleware/diag/uds/core/sid0x28.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x28.c -o output/sid0x28.o
sid0x2f.o: ../SDK/middleware/diag/uds/core/sid0x2f.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x2f.c -o output/sid0x2f.o
sid0x31.o: ../SDK/middleware/diag/uds/core/sid0x31.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x31.c -o output/sid0x31.o
sid0x85.o: ../SDK/middleware/diag/uds/core/sid0x85.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/sid0x85.c -o output/sid0x85.o
uds.o: ../SDK/middleware/diag/uds/core/uds.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/uds.c -o output/uds.o
uds_sid.o: ../SDK/middleware/diag/uds/core/uds_sid.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/core/uds_sid.c -o output/uds_sid.o
diagCommon_cfg.o: ../SDK/middleware/diag/uds/platform/diagCommon_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/diagCommon_cfg.c -o output/diagCommon_cfg.o
download_cbk.o: ../SDK/middleware/diag/uds/platform/download/download_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/download/download_cbk.c -o output/download_cbk.o
download_cfg.o: ../SDK/middleware/diag/uds/platform/download/download_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/download/download_cfg.c -o output/download_cfg.o
dtcApp_cbk.o: ../SDK/middleware/diag/uds/platform/dtcApp/dtcApp_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/dtcApp/dtcApp_cbk.c -o output/dtcApp_cbk.o
dtcApp_cfg.o: ../SDK/middleware/diag/uds/platform/dtcApp/dtcApp_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/dtcApp/dtcApp_cfg.c -o output/dtcApp_cfg.o
dtcCore_cbk.o: ../SDK/middleware/diag/uds/platform/dtcCore/dtcCore_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/dtcCore/dtcCore_cbk.c -o output/dtcCore_cbk.o
dtcCore_cfg.o: ../SDK/middleware/diag/uds/platform/dtcCore/dtcCore_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/dtcCore/dtcCore_cfg.c -o output/dtcCore_cfg.o
SecurityAlgorithm.o: ../SDK/middleware/diag/uds/platform/SecurityAlgorithm.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/SecurityAlgorithm.c -o output/SecurityAlgorithm.o
sid0x10_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x10/sid0x10_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x10/sid0x10_cbk.c -o output/sid0x10_cbk.o
sid0x10_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x10/sid0x10_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x10/sid0x10_cfg.c -o output/sid0x10_cfg.o
sid0x11_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x11/sid0x11_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x11/sid0x11_cbk.c -o output/sid0x11_cbk.o
sid0x11_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x11/sid0x11_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x11/sid0x11_cfg.c -o output/sid0x11_cfg.o
sid0x14_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x14/sid0x14_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x14/sid0x14_cbk.c -o output/sid0x14_cbk.o
sid0x14_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x14/sid0x14_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x14/sid0x14_cfg.c -o output/sid0x14_cfg.o
sid0x22_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x22/sid0x22_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x22/sid0x22_cbk.c -o output/sid0x22_cbk.o
sid0x22_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x22/sid0x22_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x22/sid0x22_cfg.c -o output/sid0x22_cfg.o
sid0x23_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x23/sid0x23_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x23/sid0x23_cbk.c -o output/sid0x23_cbk.o
sid0x23_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x23/sid0x23_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x23/sid0x23_cfg.c -o output/sid0x23_cfg.o
sid0x27_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x27/sid0x27_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x27/sid0x27_cbk.c -o output/sid0x27_cbk.o
sid0x27_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x27/sid0x27_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x27/sid0x27_cfg.c -o output/sid0x27_cfg.o
sid0x28_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x28/sid0x28_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x28/sid0x28_cbk.c -o output/sid0x28_cbk.o
sid0x28_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x28/sid0x28_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x28/sid0x28_cfg.c -o output/sid0x28_cfg.o
sid0x2f_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x2f/sid0x2f_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x2f/sid0x2f_cbk.c -o output/sid0x2f_cbk.o
sid0x2f_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x2f/sid0x2f_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x2f/sid0x2f_cfg.c -o output/sid0x2f_cfg.o
sid0x31_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x31/sid0x31_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x31/sid0x31_cbk.c -o output/sid0x31_cbk.o
sid0x31_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x31/sid0x31_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x31/sid0x31_cfg.c -o output/sid0x31_cfg.o
sid0x85_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x85/sid0x85_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x85/sid0x85_cbk.c -o output/sid0x85_cbk.o
sid0x85_cfg.o: ../SDK/middleware/diag/uds/platform/sid0x85/sid0x85_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x85/sid0x85_cfg.c -o output/sid0x85_cfg.o
sid0x99_cbk.o: ../SDK/middleware/diag/uds/platform/sid0x99/sid0x99_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/sid0x99/sid0x99_cbk.c -o output/sid0x99_cbk.o
uds_cbk.o: ../SDK/middleware/diag/uds/platform/uds_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/uds_cbk.c -o output/uds_cbk.o
uds_cfg.o: ../SDK/middleware/diag/uds/platform/uds_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/uds_cfg.c -o output/uds_cfg.o
uds_sid_cbk.o: ../SDK/middleware/diag/uds/platform/uds_sid_cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/uds_sid_cbk.c -o output/uds_sid_cbk.o
uds_sid_cfg.o: ../SDK/middleware/diag/uds/platform/uds_sid_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/diag/uds/platform/uds_sid_cfg.c -o output/uds_sid_cfg.o
Eeprom.o: ../SDK/middleware/Eeprom/Eeprom.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Eeprom/Eeprom.c -o output/Eeprom.o
EncoderEC11.o: ../SDK/middleware/EncoderEC11/EncoderEC11.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/EncoderEC11/EncoderEC11.c -o output/EncoderEC11.o
ExternRTC.o: ../SDK/middleware/ExternRTC/ExternRTC.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/ExternRTC/ExternRTC.c -o output/ExternRTC.o
Ex_Wdg.o: ../SDK/middleware/Ex_Wdg/Ex_Wdg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Ex_Wdg/Ex_Wdg.c -o output/Ex_Wdg.o
FanCtrl.o: ../SDK/middleware/FanCtrl/FanCtrl.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/FanCtrl/FanCtrl.c -o output/FanCtrl.o
SimFlag.o: ../SDK/middleware/Flags/SimFlag.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Flags/SimFlag.c -o output/SimFlag.o
Flash_if.o: ../SDK/middleware/Flash_if/Flash_if.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Flash_if/Flash_if.c -o output/Flash_if.o
Gps_Parse.o: ../SDK/middleware/GpsParse/Gps_Parse.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/GpsParse/Gps_Parse.c -o output/Gps_Parse.o
gyro.o: ../SDK/middleware/Gyro/gyro.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Gyro/gyro.c -o output/gyro.o
HardwareVersion.o: ../SDK/middleware/HardwareVersion/HardwareVersion.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/HardwareVersion/HardwareVersion.c -o output/HardwareVersion.o
iic_if.o: ../SDK/middleware/Iic_if/iic_if.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Iic_if/iic_if.c -o output/iic_if.o
iic_if_cfg.o: ../SDK/middleware/Iic_if/iic_if_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Iic_if/iic_if_cfg.c -o output/iic_if_cfg.o
tp.o: ../SDK/middleware/isotp/tp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/isotp/tp.c -o output/tp.o
tpConfig.o: ../SDK/middleware/isotp/tpConfig.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/isotp/tpConfig.c -o output/tpConfig.o
AdKey.o: ../SDK/middleware/Keyboard/AdKey.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Keyboard/AdKey.c -o output/AdKey.o
CanKey.o: ../SDK/middleware/Keyboard/CanKey.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Keyboard/CanKey.c -o output/CanKey.o
HardKey.o: ../SDK/middleware/Keyboard/HardKey.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Keyboard/HardKey.c -o output/HardKey.o
Key.o: ../SDK/middleware/Keyboard/Key.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Keyboard/Key.c -o output/Key.o
PanelKey.o: ../SDK/middleware/Keyboard/PanelKey.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Keyboard/PanelKey.c -o output/PanelKey.o
Crc.o: ../SDK/middleware/MathLib/Crc/Crc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/MathLib/Crc/Crc.c -o output/Crc.o
Crc_Cfg.o: ../SDK/middleware/MathLib/Crc/Crc_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/MathLib/Crc/Crc_Cfg.c -o output/Crc_Cfg.o
Math_NumFormat.o: ../SDK/middleware/MathLib/Math_NumFormat/Math_NumFormat.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/MathLib/Math_NumFormat/Math_NumFormat.c -o output/Math_NumFormat.o
Math_Random.o: ../SDK/middleware/MathLib/Math_Random/Math_Random.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/MathLib/Math_Random/Math_Random.c -o output/Math_Random.o
Ea_Cbk.o: ../SDK/middleware/NVRAM/Ea/Ea_Cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/NVRAM/Ea/Ea_Cbk.c -o output/Ea_Cbk.o
Ea_Cfg.o: ../SDK/middleware/NVRAM/Ea/Ea_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/NVRAM/Ea/Ea_Cfg.c -o output/Ea_Cfg.o
Memif.o: ../SDK/middleware/NVRAM/Ea/Memif.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/NVRAM/Ea/Memif.c -o output/Memif.o
NvM_Cbk.o: ../SDK/middleware/NVRAM/NvM/NvM_Cbk.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/NVRAM/NvM/NvM_Cbk.c -o output/NvM_Cbk.o
NvM_Cfg.o: ../SDK/middleware/NVRAM/NvM/NvM_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/NVRAM/NvM/NvM_Cfg.c -o output/NvM_Cfg.o
PowerSupply.o: ../SDK/middleware/Power/PowerSupply/PowerSupply.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Power/PowerSupply/PowerSupply.c -o output/PowerSupply.o
BattVoltage.o: ../SDK/middleware/Power/Voltage/BattVoltage.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Power/Voltage/BattVoltage.c -o output/BattVoltage.o
PriQueue_Cfg.o: ../SDK/middleware/PriQueue/PriQueue_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/PriQueue/PriQueue_Cfg.c -o output/PriQueue_Cfg.o
rtc.o: ../SDK/middleware/Rtc/rtc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Rtc/rtc.c -o output/rtc.o
SpeedOutput.o: ../SDK/middleware/SpeedOutput/SpeedOutput.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/SpeedOutput/SpeedOutput.c -o output/SpeedOutput.o
flowcontrol.o: ../SDK/middleware/spiipc/core/flowcontrol.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/core/flowcontrol.c -o output/flowcontrol.o
hashmap.o: ../SDK/middleware/spiipc/core/hashmap.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/core/hashmap.c -o output/hashmap.o
ipc.o: ../SDK/middleware/spiipc/core/ipc.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/core/ipc.c -o output/ipc.o
SPI_CircularQueue.o: ../SDK/middleware/spiipc/core/SPI_CircularQueue.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/core/SPI_CircularQueue.c -o output/SPI_CircularQueue.o
Spi_IpcCore.o: ../SDK/middleware/spiipc/core/Spi_IpcCore.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/core/Spi_IpcCore.c -o output/Spi_IpcCore.o
ipc_gw_core.o: ../SDK/middleware/spiipc/gateway/ipc_gw_core.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/gateway/ipc_gw_core.c -o output/ipc_gw_core.o
Spi_Ipc_gw_api.o: ../SDK/middleware/spiipc/gateway/Spi_Ipc_gw_api.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/gateway/Spi_Ipc_gw_api.c -o output/Spi_Ipc_gw_api.o
ipc_gw_adaptor.o: ../SDK/middleware/spiipc/platform/ipc_gw_adaptor.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/ipc_gw_adaptor.c -o output/ipc_gw_adaptor.o
SpiIpc_cfg.o: ../SDK/middleware/spiipc/platform/SpiIpc_cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/SpiIpc_cfg.c -o output/SpiIpc_cfg.o
spiMutex.o: ../SDK/middleware/spiipc/platform/spiMutex.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/spiMutex.c -o output/spiMutex.o
spiOsDelay.o: ../SDK/middleware/spiipc/platform/spiOsDelay.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/spiOsDelay.c -o output/spiOsDelay.o
spiSem.o: ../SDK/middleware/spiipc/platform/spiSem.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/spiSem.c -o output/spiSem.o
Spi_if.o: ../SDK/middleware/spiipc/platform/Spi_if.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/Spi_if.c -o output/Spi_if.o
TaskSpi_IpcCore.o: ../SDK/middleware/spiipc/platform/TaskSpi_IpcCore.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/spiipc/platform/TaskSpi_IpcCore.c -o output/TaskSpi_IpcCore.o
Standby.o: ../SDK/middleware/Standby/Standby.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Standby/Standby.c -o output/Standby.o
Swdl.o: ../SDK/middleware/Swdl/Swdl.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Swdl/Swdl.c -o output/Swdl.o
SysWakeUp.o: ../SDK/middleware/SysWakeUp/SysWakeUp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/SysWakeUp/SysWakeUp.c -o output/SysWakeUp.o
TempAmp.o: ../SDK/middleware/Temperature/TempAmp/TempAmp.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Temperature/TempAmp/TempAmp.c -o output/TempAmp.o
TempBoard.o: ../SDK/middleware/Temperature/TempBoard/TempBoard.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Temperature/TempBoard/TempBoard.c -o output/TempBoard.o
TempConvert.o: ../SDK/middleware/Temperature/TempCommon/TempConvert.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Temperature/TempCommon/TempConvert.c -o output/TempConvert.o
trace.o: ../SDK/middleware/Trace/trace.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/Trace/trace.c -o output/trace.o
WaterTempWarn.o: ../SDK/middleware/WaterTempWarn/WaterTempWarn.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/middleware/WaterTempWarn/WaterTempWarn.c -o output/WaterTempWarn.o
Alarm_Cfg.o: ../SDK/OSAL/Alarm/Alarm_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/OSAL/Alarm/Alarm_Cfg.c -o output/Alarm_Cfg.o
System_Mutex.o: ../SDK/OSAL/System_Mutex/System_Mutex.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/OSAL/System_Mutex/System_Mutex.c -o output/System_Mutex.o
System_Task_Cfg.o: ../SDK/OSAL/System_Task/System_Task_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/OSAL/System_Task/System_Task_Cfg.c -o output/System_Task_Cfg.o
System_Tick.o: ../SDK/OSAL/System_Tick/System_Tick.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/OSAL/System_Tick/System_Tick.c -o output/System_Tick.o
VirtualSig_Cfg.o: ../SDK/OSAL/Virtual_Signal/VirtualSig_Cfg.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/OSAL/Virtual_Signal/VirtualSig_Cfg.c -o output/VirtualSig_Cfg.o
system_S32K148.o: ../SDK/platform/devices/S32K148/startup/system_S32K148.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/devices/S32K148/startup/system_S32K148.c -o output/system_S32K148.o
startup.o: ../SDK/platform/devices/startup.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/devices/startup.c -o output/startup.o
adc_driver.o: ../SDK/platform/drivers/src/adc/adc_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/adc/adc_driver.c -o output/adc_driver.o
clock_manager.o: ../SDK/platform/drivers/src/clock/clock_manager.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/clock/clock_manager.c -o output/clock_manager.o
clock_S32K1xx.o: ../SDK/platform/drivers/src/clock/S32K1xx/clock_S32K1xx.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/clock/S32K1xx/clock_S32K1xx.c -o output/clock_S32K1xx.o
crc_driver.o: ../SDK/platform/drivers/src/crc/crc_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/crc/crc_driver.c -o output/crc_driver.o
crc_hw_access.o: ../SDK/platform/drivers/src/crc/crc_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/crc/crc_hw_access.c -o output/crc_hw_access.o
edma_driver.o: ../SDK/platform/drivers/src/edma/edma_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/edma/edma_driver.c -o output/edma_driver.o
edma_hw_access.o: ../SDK/platform/drivers/src/edma/edma_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/edma/edma_hw_access.c -o output/edma_hw_access.o
edma_irq.o: ../SDK/platform/drivers/src/edma/edma_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/edma/edma_irq.c -o output/edma_irq.o
flash_driver.o: ../SDK/platform/drivers/src/flash/flash_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flash/flash_driver.c -o output/flash_driver.o
flexcan_driver.o: ../SDK/platform/drivers/src/flexcan/flexcan_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexcan/flexcan_driver.c -o output/flexcan_driver.o
flexcan_hw_access.o: ../SDK/platform/drivers/src/flexcan/flexcan_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexcan/flexcan_hw_access.c -o output/flexcan_hw_access.o
flexcan_irq.o: ../SDK/platform/drivers/src/flexcan/flexcan_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexcan/flexcan_irq.c -o output/flexcan_irq.o
flexio_common.o: ../SDK/platform/drivers/src/flexio/flexio_common.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexio/flexio_common.c -o output/flexio_common.o
flexio_i2c_driver.o: ../SDK/platform/drivers/src/flexio/flexio_i2c_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexio/flexio_i2c_driver.c -o output/flexio_i2c_driver.o
flexio_i2s_driver.o: ../SDK/platform/drivers/src/flexio/flexio_i2s_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexio/flexio_i2s_driver.c -o output/flexio_i2s_driver.o
flexio_spi_driver.o: ../SDK/platform/drivers/src/flexio/flexio_spi_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexio/flexio_spi_driver.c -o output/flexio_spi_driver.o
flexio_uart_driver.o: ../SDK/platform/drivers/src/flexio/flexio_uart_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/flexio/flexio_uart_driver.c -o output/flexio_uart_driver.o
ftm_common.o: ../SDK/platform/drivers/src/ftm/ftm_common.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_common.c -o output/ftm_common.o
ftm_hw_access.o: ../SDK/platform/drivers/src/ftm/ftm_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_hw_access.c -o output/ftm_hw_access.o
ftm_ic_driver.o: ../SDK/platform/drivers/src/ftm/ftm_ic_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_ic_driver.c -o output/ftm_ic_driver.o
ftm_mc_driver.o: ../SDK/platform/drivers/src/ftm/ftm_mc_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_mc_driver.c -o output/ftm_mc_driver.o
ftm_oc_driver.o: ../SDK/platform/drivers/src/ftm/ftm_oc_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_oc_driver.c -o output/ftm_oc_driver.o
ftm_pwm_driver.o: ../SDK/platform/drivers/src/ftm/ftm_pwm_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_pwm_driver.c -o output/ftm_pwm_driver.o
ftm_qd_driver.o: ../SDK/platform/drivers/src/ftm/ftm_qd_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/ftm/ftm_qd_driver.c -o output/ftm_qd_driver.o
interrupt_manager.o: ../SDK/platform/drivers/src/interrupt/interrupt_manager.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/interrupt/interrupt_manager.c -o output/interrupt_manager.o
lpi2c_driver.o: ../SDK/platform/drivers/src/lpi2c/lpi2c_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpi2c/lpi2c_driver.c -o output/lpi2c_driver.o
lpi2c_hw_access.o: ../SDK/platform/drivers/src/lpi2c/lpi2c_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpi2c/lpi2c_hw_access.c -o output/lpi2c_hw_access.o
lpi2c_irq.o: ../SDK/platform/drivers/src/lpi2c/lpi2c_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpi2c/lpi2c_irq.c -o output/lpi2c_irq.o
lpit_driver.o: ../SDK/platform/drivers/src/lpit/lpit_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpit/lpit_driver.c -o output/lpit_driver.o
lpit_hw_access.o: ../SDK/platform/drivers/src/lpit/lpit_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpit/lpit_hw_access.c -o output/lpit_hw_access.o
lpspi_hw_access.o: ../SDK/platform/drivers/src/lpspi/lpspi_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpspi/lpspi_hw_access.c -o output/lpspi_hw_access.o
lpspi_irq.o: ../SDK/platform/drivers/src/lpspi/lpspi_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpspi/lpspi_irq.c -o output/lpspi_irq.o
lpspi_master_driver.o: ../SDK/platform/drivers/src/lpspi/lpspi_master_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpspi/lpspi_master_driver.c -o output/lpspi_master_driver.o
lpspi_shared_function.o: ../SDK/platform/drivers/src/lpspi/lpspi_shared_function.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpspi/lpspi_shared_function.c -o output/lpspi_shared_function.o
lpspi_slave_driver.o: ../SDK/platform/drivers/src/lpspi/lpspi_slave_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpspi/lpspi_slave_driver.c -o output/lpspi_slave_driver.o
lin_lpuart_driver.o: ../SDK/platform/drivers/src/lpuart/lin_lpuart_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpuart/lin_lpuart_driver.c -o output/lin_lpuart_driver.o
lpuart_driver.o: ../SDK/platform/drivers/src/lpuart/lpuart_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpuart/lpuart_driver.c -o output/lpuart_driver.o
lpuart_hw_access.o: ../SDK/platform/drivers/src/lpuart/lpuart_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpuart/lpuart_hw_access.c -o output/lpuart_hw_access.o
lpuart_irq.o: ../SDK/platform/drivers/src/lpuart/lpuart_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/lpuart/lpuart_irq.c -o output/lpuart_irq.o
mpu_driver.o: ../SDK/platform/drivers/src/mpu/mpu_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/mpu/mpu_driver.c -o output/mpu_driver.o
mpu_hw_access.o: ../SDK/platform/drivers/src/mpu/mpu_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/mpu/mpu_hw_access.c -o output/mpu_hw_access.o
pdb_driver.o: ../SDK/platform/drivers/src/pdb/pdb_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/pdb/pdb_driver.c -o output/pdb_driver.o
pdb_hw_access.o: ../SDK/platform/drivers/src/pdb/pdb_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/pdb/pdb_hw_access.c -o output/pdb_hw_access.o
pins_driver.o: ../SDK/platform/drivers/src/pins/pins_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/pins/pins_driver.c -o output/pins_driver.o
pins_port_hw_access.o: ../SDK/platform/drivers/src/pins/pins_port_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/pins/pins_port_hw_access.c -o output/pins_port_hw_access.o
power_manager.o: ../SDK/platform/drivers/src/power/power_manager.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/power/power_manager.c -o output/power_manager.o
power_manager_S32K1xx.o: ../SDK/platform/drivers/src/power/S32K1xx/power_manager_S32K1xx.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/power/S32K1xx/power_manager_S32K1xx.c -o output/power_manager_S32K1xx.o
power_pmc_hw_access.o: ../SDK/platform/drivers/src/power/S32K1xx/power_pmc_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/power/S32K1xx/power_pmc_hw_access.c -o output/power_pmc_hw_access.o
power_rcm_hw_access.o: ../SDK/platform/drivers/src/power/S32K1xx/power_rcm_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/power/S32K1xx/power_rcm_hw_access.c -o output/power_rcm_hw_access.o
power_smc_hw_access.o: ../SDK/platform/drivers/src/power/S32K1xx/power_smc_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/power/S32K1xx/power_smc_hw_access.c -o output/power_smc_hw_access.o
rtc_driver.o: ../SDK/platform/drivers/src/rtc/rtc_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/rtc/rtc_driver.c -o output/rtc_driver.o
rtc_hw_access.o: ../SDK/platform/drivers/src/rtc/rtc_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/rtc/rtc_hw_access.c -o output/rtc_hw_access.o
rtc_irq.o: ../SDK/platform/drivers/src/rtc/rtc_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/rtc/rtc_irq.c -o output/rtc_irq.o
trgmux_driver.o: ../SDK/platform/drivers/src/trgmux/trgmux_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/trgmux/trgmux_driver.c -o output/trgmux_driver.o
trgmux_hw_access.o: ../SDK/platform/drivers/src/trgmux/trgmux_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/trgmux/trgmux_hw_access.c -o output/trgmux_hw_access.o
wdog_driver.o: ../SDK/platform/drivers/src/wdog/wdog_driver.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/wdog/wdog_driver.c -o output/wdog_driver.o
wdog_hw_access.o: ../SDK/platform/drivers/src/wdog/wdog_hw_access.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/drivers/src/wdog/wdog_hw_access.c -o output/wdog_hw_access.o
adc_irq.o: ../SDK/platform/pal/adc/src/adc_irq.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/pal/adc/src/adc_irq.c -o output/adc_irq.o
adc_pal.o: ../SDK/platform/pal/adc/src/adc_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/pal/adc/src/adc_pal.c -o output/adc_pal.o
spi_pal.o: ../SDK/platform/pal/spi/src/spi_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/pal/spi/src/spi_pal.c -o output/spi_pal.o
uart_pal.o: ../SDK/platform/pal/uart/src/uart_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/pal/uart/src/uart_pal.c -o output/uart_pal.o
wdg_pal.o: ../SDK/platform/pal/wdg/src/wdg_pal.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/platform/pal/wdg/src/wdg_pal.c -o output/wdg_pal.o
croutine.o: ../SDK/rtos/FreeRTOS_S32K/Source/croutine.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/croutine.c -o output/croutine.o
event_groups.o: ../SDK/rtos/FreeRTOS_S32K/Source/event_groups.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/event_groups.c -o output/event_groups.o
list.o: ../SDK/rtos/FreeRTOS_S32K/Source/list.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/list.c -o output/list.o
port.o: ../SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.c -o output/port.o
heap_2.o: ../SDK/rtos/FreeRTOS_S32K/Source/portable/MemMang/heap_2.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/portable/MemMang/heap_2.c -o output/heap_2.o
queue.o: ../SDK/rtos/FreeRTOS_S32K/Source/queue.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/queue.c -o output/queue.o
tasks.o: ../SDK/rtos/FreeRTOS_S32K/Source/tasks.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/tasks.c -o output/tasks.o
timers.o: ../SDK/rtos/FreeRTOS_S32K/Source/timers.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/FreeRTOS_S32K/Source/timers.c -o output/timers.o
osif_freertos.o: ../SDK/rtos/osif/osif_freertos.c
	arm-none-eabi-gcc "@cfg/gen/gcc_header_cfg.args" "@cfg/gcc_c_options.args" ../SDK/rtos/osif/osif_freertos.c -o output/osif_freertos.o
