#include "sys_setting.h"
#include "CanAppDiagClient.h"
#include "CanApp_TxProc.h"
#include "dea.h"




//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED IPC_APP_SYS_SETTING_STRU g_sys_setting = 
{
	.language = 1,       //[0,1]璇█锛�?:鑻辫�?1:姹夎�?
    .unit = 0,           //[0,1]鍗曚綅锛�?:km/h,1:mile/h
    .tirePressure = 1,   //[0,1]鑳庡帇鎶ヨ寮�鍚�
    .driveAssistant = 1, //[0,1]椹鹃┒杈呭姪寮�鍚�
    .autoStopSingle = 1, //[0,1]鑷姩鍚仠-鍗曟寮�鍚�?
    .autoStopSum = 1,    //[0,1]鑷姩鍚仠-绱寮�鍚�?
    .sound = 1,          //[0,1]闊冲搷寮�鍚�?
    .powerCycle = 1,     //[0,1]鑳介噺寰幆寮�鍚�

    .mapType = 1,             //[0,1]鍏ㄥ眬鍦板浘绫诲�?0:2d,1:3d)
    .playEffect = 1,          //[0,1]鍔ㄦ�佹晥鏋滆缃紑鍚�?
    .alertSound = 1,          //[0,1]璇煶杈呭姪鎶ヨ寮�鍚�?
    .vehicleDeviateAlert = 1, //[0,1]杞﹁締鍋忕棰勮寮�鍚�?
    .blindAreaMonitoring = 1, //[0,1]鐩插尯鐩戞祴寮�鍚�
    .emergencyBraking = 1,    //[0,1]鍓嶆柟闃茬鎾炵揣鎬ュ埗鍔ㄥ紑鍚�?
    .parkAssistant = 1,       //[0,1]娉婅溅杈呭姪寮�鍚�
    .driveTimeAlert = 1,      //[0,1]椹鹃┒鏃堕棿鎻愰啋寮�鍚�?

    .driveTimeSet = 30, //[30,360](鍒嗛�?椹鹃┒鏃堕棿鎻愰啋璁惧畾鏃堕�?

    .maintenanceMileage = 0, //[0,10000](km)??閲岀▼淇濆吇锛�0鍏抽�?
    .maintenanceTime = 0,    //[0,365](澶�)??鏃堕棿淇濆吇,0鍏抽�?

    .backLightValue = 21,  //[0,255]浠〃鑳屽厜
    .backLightNightValue = 20,
    .speedLimit = 10,      //[0,24](鍗曚�?0km/h,0:鍏抽棴锛�?4:240km/h)
    .bestFuelTimeOut = 5, //[0,60]鏈�浣崇噧娌硅瀹氶粯璁よ秴鏃�(second,ui step:5)

    .frontTire = 2, //[0,0x0f]鍓嶈儙鍘嬪憡璀﹂槇鍊硷紝瀹氫箟瑙佽〃鏍�?
    .rearTire = 2,  //[0,0x0f]鍚庤儙鍘嬪憡璀﹂槇鍊硷紝瀹氫箟瑙佽〃鏍�?

    .hmiLastPage = 0, //[0,255]ui椤甸潰浠ｇ爜
	.IlluminationSetting = 2,
	.NaviMode = 0,
	.NavStyle = 2,
	.EAPM_Setting = 1,
	.TimeFormat = 0, //yangxl 24h 2019-12-30 09:41:27

	.HUDSwitch = 1,			      		//yangxl 2019-9-24 14:03:45
	.HUDNavigationDisplaySwitch = 1,		  
	.HUDDrivingAssistanceSwitch = 1,			
	.HUDTachometerSwitch = 0u,			      	
	.HUDFuelConsumptionSwitch = 0,			
	.HUDMusicStateSwitch = 1,			      	
	.HUDMixedModeSwitch = 0,			      	
	.HUDCallReminderSwitch = 1,			  

	.HUDLyricSwitch = 0,			      		
	.Custommenucombinations = 0,		
	.HUDRotationAngle = 12,
	.HUDStyle = 0,	
};

static uint8_t g_sys_setting_up_flag = 1;

const SysSettingFuncType SysSettingFuncList[] = 
{
	SysSettingLanguageSet,
	SysSettingUnitSet,
	SysSettingTirePressureSet,
	SysSettingDriveAssistantSet,
	SysSettingAutoStopSingleSet,
	SysSettingAutoStopSumSet,
	SysSettingSoundSet,
	SysSettingPowerCycleSet,
	SysSettingMapTypeSet,
	SysSettingPlayEffectSet,
	SysSettingAlertSoundSet,
	SysSettingVehicleDeviateAlertSet,
	SysSettingBlindAreaMonitoringSet,
	SysSettingEmergencyBrakingSet,
	SysSettingParkAssistantSet,
	SysSettingDriveTimeAlertSet,
	SysSettingDriveTimeSetSet,
	SysSettingMaintenanceMileageSet,
	SysSettingMaintenanceTimeSet,
	SysSettingbackLightValueSet,
	SysSettingbackLightNightValueSet,
	SysSettingSpeedLimitSet,
	SysSettingbestFuelTimeOutSet,
	SysSettingFrontTireSet,
	SysSettingRearTireSet,
	SysSettingHmiLastPageSet,
	SysSettingIlluminationSettingSet,
	SysSettingTimeFormatSet,
	SysSettingNaviModeSet,
	SysSettingNavStyleSet,
	SysSettingEAPM_SettingSet,
	SysSettingLightSensitivitySet,
	SysSettingLightOffDaySet,
	SysSettingSelectiveUnlockSet,
	SysSettingWiperWithSpeedSet,
	/* yangxl */
	SysSettingAutomaticClosingWindowSet,
 	SysSettingBrushAwayTheRainSet,
 	//SysSettingWiperWithSpeedSet,
 	SysSettingRearviewMirrorFoldsAutomaticallySet,
	SysSettingRearviewMirrorSwitchSet,
 	SysSettingAutomaticLockingSet,
 	SysSettingAutomaticUnlockSet,
 	//SysSettingSelectiveUnlockSet,
 	SysSettingIntensityOfLightSet,
 	//SysSettingLightSensitivitySet,
 	SysSettingTurnOffLightsDelaySet,
 	//SysSettingLightOffDaySet,//591 user
 	SysSettingCarLockPromptSet,
 	SysSettingRestoreFactorySettingsSet,
 	SysSettingRainfallSensitivitySet,
 	SysSettingFEBSwitchSet,
 	SysSettingLKASwitchSet,
 	SysSettingBSWSwitchSet,
 	SysSettingCTASwitchSet,
 	SysSettingEAPMSwitchSet,
 	SysSettingDOASwitchSet,
 	SysSettingLDWSwitchSet,
 	SysSettingECOSwitchSet,
 	SysSettingISSSwitchSet,

	SysSettingHUDSwitchSet,
	SysSettingHUDNavigationDisplaySwitchSet,
	SysSettingHUDDrivingAssistanceSwitchSet,
	SysSettingHUDTachometerSwitchSet,
	SysSettingHUDFuelConsumptionSwitchSet,
	SysSettingHUDMusicStateSwitchSet,
	SysSettingHUDMixedModeSwitchSet,
	SysSettingHUDCallReminderSwitchSet,
	
	SysSettingHUDLyricSwitchSet,
	SysSettingCustommenucombinationsSet,
	SysSettingHUDRotationAngleSet,
	SysSettingHUDStyleSet,
	SysSettingUPASwitchSet,
	
 	/* yangxl */
};


void SysSettingLanguageSet(uint16_t value)
{
	g_sys_setting.language = value;
	BACKRAM_LANGUAGE = g_sys_setting.language;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingUnitSet(uint16_t value)
{
	g_sys_setting.unit = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingTirePressureSet(uint16_t value)
{
	g_sys_setting.tirePressure = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingDriveAssistantSet(uint16_t value)
{
	g_sys_setting.driveAssistant = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingAutoStopSingleSet(uint16_t value)
{
	g_sys_setting.autoStopSingle = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint16_t SysSettingAutoStopSingleGet(void)
{
	return g_sys_setting.autoStopSingle;
}

void SysSettingAutoStopSumSet(uint16_t value)
{
	g_sys_setting.autoStopSum = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint16_t SysSettingAutoStopSumGet(void)
{
	return g_sys_setting.autoStopSum;
}


void SysSettingSoundSet(uint16_t value)
{
	g_sys_setting.sound = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingPowerCycleSet(uint16_t value)
{
	g_sys_setting.powerCycle = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingMapTypeSet(uint16_t value)
{
	g_sys_setting.mapType = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingPlayEffectSet(uint16_t value)
{
	g_sys_setting.playEffect = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingAlertSoundSet(uint16_t value)
{
	g_sys_setting.alertSound = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingVehicleDeviateAlertSet(uint16_t value)
{
	//g_sys_setting.vehicleDeviateAlert = value;
	CanApp_TxITS_LDWSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingVehicleDeviateAlertUpdate(uint16_t value)
{
	g_sys_setting.vehicleDeviateAlert = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingVehicleDeviateAlertGet(void)
{
	return g_sys_setting.vehicleDeviateAlert; //591
}

void SysSettingBlindAreaMonitoringSet(uint16_t value)
{
	//g_sys_setting.blindAreaMonitoring = value;
	CanApp_TxITS_BSWSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingBlindAreaMonitoringUpdate(uint16_t value)
{
	g_sys_setting.blindAreaMonitoring = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingBlindAreaMonitoringGet(void)
{
	return g_sys_setting.blindAreaMonitoring; //591
}


void SysSettingEmergencyBrakingSet(uint16_t value)
{
	//g_sys_setting.emergencyBraking = value;
	CanApp_TxITS_FEBSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingEmergencyBrakingUpdate(uint16_t value)
{
	g_sys_setting.emergencyBraking = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingEmergencyBrakingGet(void)
{
	return g_sys_setting.emergencyBraking; //591
}

/*
uint8_t SysSettingEAPMSwitchGet(void)
{
	//return g_sys_setting.emergencyBraking; //591
	return g_sys_setting.EAPMSwitch; //532
}
*/


void SysSettingParkAssistantSet(uint16_t value)
{
	//g_sys_setting.parkAssistant = value;
	CanApp_TxSonarSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingParkAssistantUpdate(uint16_t value)
{
	g_sys_setting.parkAssistant = value;
	g_sys_setting_up_flag = 1;
}


uint8_t SysSettingParkAssistantGet(void)
{
	return g_sys_setting.parkAssistant;
}


void SysSettingDriveTimeAlertSet(uint16_t value)
{
	g_sys_setting.driveTimeAlert = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingDriveTimeSetSet(uint16_t value)
{
	g_sys_setting.driveTimeSet = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingMaintenanceMileageSet(uint16_t value)
{
	g_sys_setting.maintenanceMileage = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingMaintenanceTimeSet(uint16_t value)
{
	g_sys_setting.maintenanceTime = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingbackLightValueSet(uint16_t value)
{
	g_sys_setting.backLightValue = value;
	//g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingbackLightNightValueSet(uint16_t value)
{
	g_sys_setting.backLightNightValue = value;
	//g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingSpeedLimitSet(uint16_t value)
{
	g_sys_setting.speedLimit = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingbestFuelTimeOutSet(uint16_t value)
{
	g_sys_setting.bestFuelTimeOut = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingFrontTireSet(uint16_t value)
{
	g_sys_setting.frontTire = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingRearTireSet(uint16_t value)
{
	g_sys_setting.rearTire = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingHmiLastPageSet(uint16_t value)
{
	g_sys_setting.hmiLastPage = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingIlluminationSettingSet(uint16_t value)
{
	g_sys_setting.IlluminationSetting = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingTimeFormatSet(uint16_t value)
{
	g_sys_setting.TimeFormat = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingNaviModeSet(uint16_t value)
{
	g_sys_setting.NaviMode = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingNavStyleSet(uint16_t value)
{
	g_sys_setting.NavStyle = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingEAPM_SettingSet(uint16_t value)
{
	//g_sys_setting.EAPM_Setting = value;
	CanApp_TxEAPMSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingEAPMUpdate(uint16_t value)
{
	g_sys_setting.EAPM_Setting = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingLightSensitivitySet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(LightSensitivity, value);
}

uint8_t SysSettingLightSensitivityGet(void)
{
	return g_sys_setting.LightSensitivity;
}

void SysSettingLightSensitivityUpdate(uint16_t value)
{
	g_sys_setting.LightSensitivity = value;
	g_sys_setting_up_flag = 1;
}

void SysSettingLightOffDaySet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(LightOffDay, TransLightOffDayFromHmiToCan(value));
}

uint8_t SysSettingLightOffDayGet(void)
{
	return TransLightOffDayFromHmiToCan(g_sys_setting.LightOffDay);
}

void SysSettingLightOffDayUpdate(uint16_t value)
{
	g_sys_setting.LightOffDay = TransLightOffDayFromCanToHmi(value);
	g_sys_setting_up_flag = 1;
}

void SysSettingSelectiveUnlockSet(uint16_t value)
{
/*
	CanAppDiagWriteRequestFlagSet(SelectiveUnlock, value);
	*/
}


uint8_t SysSettingSelectiveUnlockGet(void)
{
	return g_sys_setting.SelectiveUnlock;
}

void SysSettingSelectiveUnlockUpdate(uint16_t value)
{
	g_sys_setting.SelectiveUnlock = value;
	g_sys_setting_up_flag = 1;
}

/*yangxl*/
uint8_t SysSettingAutomaticClosingWindowGet(void)
{
	return g_sys_setting.AutomaticClosingWindow;
}

void SysSettingAutomaticClosingWindowSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(LockAutoWindowUp, value);
}

void SysSettingAutomaticClosingWindowUpdate(uint16_t value)
{
	g_sys_setting.AutomaticClosingWindow = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingBrushAwayTheRainGet(void)
{
	return g_sys_setting.BrushAwayTheRain;
}

void SysSettingBrushAwayTheRainSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(WiperWithWipingDrip, value);
}
void SysSettingBrushAwayTheRainUpdate(uint16_t value)
{
	g_sys_setting.BrushAwayTheRain = value;
	g_sys_setting_up_flag = 1;
}


uint8_t SysSettingRearviewMirrorFoldsAutomaticallyGet(void)
{
	return g_sys_setting.RearviewMirrorFoldsAutomatically;
}

//void SysSettingRearviewMirrorFoldsAutomaticallySet(uint16_t value)
void SysSettingRearviewMirrorFoldsAutomaticallyUpdate(uint16_t value)
{
	g_sys_setting.RearviewMirrorFoldsAutomatically = value;
	g_sys_setting_up_flag = 1;
}
//void SysSettingRearviewMirrorFoldsAutomaticallyUpdate(uint16_t value)
void SysSettingRearviewMirrorFoldsAutomaticallySet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(AutoFoldFunction, value);
}


void SysSettingRearviewMirrorSwitchSet(uint16_t value)
{
	g_sys_setting.RearviewMirrorSwitch = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingAutomaticLockingGet(void)
{
	return g_sys_setting.AutomaticLocking;
}

void SysSettingAutomaticLockingSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(AutoLockFunctionCustomize, value);
}
void SysSettingAutomaticLockingUpdate(uint16_t value)
{
	g_sys_setting.AutomaticLocking = value;
	g_sys_setting_up_flag = 1;
}


uint8_t SysSettingAutomaticUnlockGet(void)
{
	return g_sys_setting.AutomaticUnlock;
}

void SysSettingAutomaticUnlockSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(AutoUnlockFunctionCustomize, value);
}
void SysSettingAutomaticUnlockUpdate(uint16_t value)
{
	g_sys_setting.AutomaticUnlock = value;
	g_sys_setting_up_flag = 1;
}


uint8_t SysSettingIntensityOfLightGet(void)
{
	return g_sys_setting.IntensityOfLight;
}
void SysSettingIntensityOfLightSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(LightSensitivity, value);
}

void SysSettingIntensityOfLightUpdate(uint16_t value)
{
	g_sys_setting.IntensityOfLight = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingTurnOffLightsDelayGet(void)
{
	return g_sys_setting.TurnOffLightsDelay;
}

void SysSettingTurnOffLightsDelaySet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(LightOffDay, value);
}

void SysSettingTurnOffLightsDelayUpdate(uint16_t value)
{
	g_sys_setting.TurnOffLightsDelay = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingCarLockPromptGet(void)
{
	return g_sys_setting.CarLockPrompt;
}

void SysSettingCarLockPromptSet(uint16_t value)
{
	CanAppDiagWriteRequestFlagSet(AnswerBack, value);
}
void SysSettingCarLockPromptUpdate(uint16_t value)
{
	g_sys_setting.CarLockPrompt = value;
	g_sys_setting_up_flag = 1;
}


void SysSettingRestoreFactorySettingsSet(uint16_t value)
{
	g_sys_setting.RestoreFactorySettings = value;
	g_sys_setting_up_flag = 1;
}
void SysSettingRainfallSensitivitySet(uint16_t value)
{
	g_sys_setting.RainfallSensitivity = value;
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingFEBSwitchGet(void)
{
	return g_sys_setting.FEBSwitch;
}

void SysSettingFEBSwitchSet(uint16_t value)
{
	//g_sys_setting.FEBSwitch = value;
	CanApp_TxITS_FEBSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingFEBSwitchUpdate(uint16_t value)
{
	g_sys_setting.FEBSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint8_t SysSettingLKASwitchGet(void)
{
	return g_sys_setting.LKASwitch;
}

void SysSettingLKASwitchSet(uint16_t value)
{
	//g_sys_setting.LKASwitch = value;
	CanApp_TxITS_LKASetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingLKASwitchUpdate(uint16_t value)
{
	g_sys_setting.LKASwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


uint8_t SysSettingBSWSwitchGet(void)
{
	return g_sys_setting.BSWSwitch;
}

void SysSettingBSWSwitchSet(uint16_t value)
{
	//g_sys_setting.BSWSwitch = value;
	CanApp_TxITS_BSWSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingBSWSwitchUpdate(uint16_t value)
{
	g_sys_setting.BSWSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingCTASwitchSet(uint16_t value)
{
	g_sys_setting.CTASwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint8_t SysSettingEAPMSwitchGet(void)
{
	return g_sys_setting.EAPMSwitch;
}

void SysSettingEAPMSwitchSet(uint16_t value)
{
	//g_sys_setting.EAPMSwitch = value;
	CanApp_TxEAPMSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingEAPMSwitchUpdate(uint16_t value)
{
	g_sys_setting.EAPMSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingDOASwitchSet(uint16_t value)
{
	g_sys_setting.DOASwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint8_t SysSettingLDWSwitchGet(void)
{
	return g_sys_setting.LDWSwitch;
}


void SysSettingLDWSwitchSet(uint16_t value)
{
	//g_sys_setting.LDWSwitch = value;
	CanApp_TxITS_LDWSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingLDWSwitchUpdate(uint16_t value)
{
	g_sys_setting.LDWSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


void SysSettingECOSwitchSet(uint16_t value)
{
	//g_sys_setting.ECOSwitch = value;
	//g_sys_setting_up_flag = 1;
	uint8_t  s_value = 0u;
	s_value = value;
	s_value = 1u;
	CanApp_TxECOetting(s_value);
}
uint8_t SysSettingECOSwitchGet(void)
{
	return g_sys_setting.ECOSwitch;
}

void SysSettingECOSwitchUpdate(uint16_t value)
{
	g_sys_setting.ECOSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}


uint8_t SysSettingISSSwitchGet(void)
{
	return g_sys_setting.ISSSwitch;
}

void SysSettingISSSwitchSet(uint16_t value)
{
	//g_sys_setting.ISSSwitch = value;
	CanApp_TxITS_ISSSetting(value);
	//g_sys_setting_up_flag = 1;
}

void SysSettingISSSwitchUpdate(uint16_t value)
{
	g_sys_setting.ISSSwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

uint8_t SysSettingUPASwitchGet(void) //yangxl 2019-11-14 17:08:19
{
	return g_sys_setting.UPASwitch;
}


void SysSettingUPASwitchSet(uint16_t value) //yangxl 2019-11-14 17:08:19
{
	CanApp_TxITS_UPASetting(value);
}

void SysSettingUPASwitchUpdate(uint16_t value) //yangxl 2019-11-14 17:08:19
{
	g_sys_setting.UPASwitch = value;
	g_sys_setting_up_flag = 1;
	//NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}



/*yangxl*/

/* yangxl 2019-9-24 14:18:16 */

void SysSettingHUDSwitchSet(uint16_t value)
{
	g_sys_setting.HUDSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDNavigationDisplaySwitchSet(uint16_t value)
{
	g_sys_setting.HUDNavigationDisplaySwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDDrivingAssistanceSwitchSet(uint16_t value)
{
	g_sys_setting.HUDDrivingAssistanceSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDTachometerSwitchSet(uint16_t value)
{
	g_sys_setting.HUDTachometerSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDFuelConsumptionSwitchSet(uint16_t value)
{
	g_sys_setting.HUDFuelConsumptionSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDMusicStateSwitchSet(uint16_t value)
{
	g_sys_setting.HUDMusicStateSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDMixedModeSwitchSet(uint16_t value)
{
	g_sys_setting.HUDMixedModeSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDCallReminderSwitchSet(uint16_t value)
{
	g_sys_setting.HUDCallReminderSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDLyricSwitchSet(uint16_t value)
{
	g_sys_setting.HUDLyricSwitch = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingCustommenucombinationsSet(uint16_t value)
{
	g_sys_setting.Custommenucombinations = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDRotationAngleSet(uint16_t value)
{
	g_sys_setting.HUDRotationAngle = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}

void SysSettingHUDStyleSet(uint16_t value)
{
	g_sys_setting.HUDStyle = value;
	g_sys_setting_up_flag = 1;
	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
}




void SysSettingWiperWithSpeedSet(uint16_t value)
{
	/*
	if(value != 0)
	{
		CanAppDiagWriteRequestFlagSet(WiperWithSpeed, WiperWithSpeedOn);
	}
	else
	{
		CanAppDiagWriteRequestFlagSet(WiperWithSpeed, WiperWithSpeedOff);
	}
	*/
}

uint8_t SysSettingWiperWithSpeedGet(void)
{
	if(g_sys_setting.WiperWithSpeed)
	{
		return WiperWithSpeedOn;
	}
	else
	{
		return WiperWithSpeedOff;
	}
}

void SysSettingWiperWithSpeedUpdate(uint16_t value)
{
	if(value == WiperWithSpeedOn)
	{
		g_sys_setting.WiperWithSpeed = 1;
	}
	else
	{
		g_sys_setting.WiperWithSpeed = 0;
	}
	g_sys_setting_up_flag = 1;
}

uint8_t SysSettingEAPMGet(void)
{
	return g_sys_setting.EAPM_Setting;
}

void SysSettingCarBodySetInProgressSet(uint8_t value)
{
	if(g_sys_setting.CarBodySetInProgress != value)
	{
		g_sys_setting.CarBodySetInProgress = value;
		g_sys_setting_up_flag = 1;
	}
}

uint8_t SysSettingCarBodySetInProgressGet(void)
{
	return g_sys_setting.CarBodySetInProgress;
}


IPC_APP_SYS_SETTING_STRU SysSettingGet(void)
{
	return g_sys_setting;
}

uint8_t SysSettingUpFlagGet(void)
{
	return g_sys_setting_up_flag;
}

void SysSettingUpFlagSet(uint8_t value)
{
	g_sys_setting_up_flag = value;
}


void SysSettingVehicleReset(void)
{
	CanAppDiagWriteRequestFlagSet(VehicleReset, 0);
}

void SysSettingFactoryReset(void)
{
	VehicleResettingSetting();
	g_sys_setting.tirePressure = 1;	//tire info display ON
	g_sys_setting.driveAssistant = 1;	//default:drive Assistant ON
	g_sys_setting.autoStopSingle = 1;	//Idle Stop Trip ON
	g_sys_setting.autoStopSum = 1;	//Idle Stop ON
	g_sys_setting.sound = 1;	
	g_sys_setting.mapType = 0;	//default: 2d
	g_sys_setting.playEffect = 1;	//dynamic effect
	g_sys_setting.alertSound = 1;	//VR alarm ON
	g_sys_setting.backLightValue = 21;	//default: backlight value 
	g_sys_setting.backLightNightValue = 20;	//default: backlight value 
	g_sys_setting.language = 1;	//Chinese
	g_sys_setting.NavStyle = 2;	//modern
	g_sys_setting.driveTimeAlert = 0;	//default: drive time alart OFF
	g_sys_setting.driveTimeSet = 30;	//default: drive time setting value 30min
	g_sys_setting.IlluminationSetting = 2;	//default: level two

	g_sys_setting.blindAreaMonitoring = 1;		//default ON
	g_sys_setting.emergencyBraking = 1;			//default ON
	g_sys_setting.vehicleDeviateAlert = 1;		//default ON
	g_sys_setting.parkAssistant = 1;			//default ON
	g_sys_setting.EAPM_Setting = 1;				//default ON

	g_sys_setting.TimeFormat = 0, //yangxl 24h 2019-12-30 09:41:27

	SettingHudReset();
	SettingMeterReset();

	//reset drive computer
	//ReSet_HAFC();
	ReSetHisAfc();//yangxl
	AVS_ResetTripAAVS();
	AVS_ResetTripBAVS();
	AFC_vResetA();
	AFC_vResetB();
	ODO_ResetTripA();
	ODO_ResetTripB();

	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
	g_sys_setting_up_flag = 1;
}

void SysSettingInit(void)
{
	BACKRAM_LANGUAGE = g_sys_setting.language;
}

/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 2019-12-23 11:00:49
*****************************************************************************/
void SettingHudReset(void)
{
	g_sys_setting.HUDSwitch = 1,			      		//yangxl 2019-9-24 14:03:45
	g_sys_setting.HUDNavigationDisplaySwitch = 1,		  
	g_sys_setting.HUDDrivingAssistanceSwitch = 1,			
	g_sys_setting.HUDTachometerSwitch = 0u,			      	
	g_sys_setting.HUDFuelConsumptionSwitch = 0,			
	g_sys_setting.HUDMusicStateSwitch = 1,			      	
	g_sys_setting.HUDMixedModeSwitch = 0,			      	
	g_sys_setting.HUDCallReminderSwitch = 1,			  

	g_sys_setting.HUDLyricSwitch = 0,			      		
	//g_sys_setting.Custommenucombinations = 0,		
	g_sys_setting.HUDRotationAngle = 12,
	g_sys_setting.HUDStyle = 0,	

	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
	g_sys_setting_up_flag = 1;

}

/**************************************************************************** 
Function	:         
Input		:          
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 2019-12-23 11:00:49
*****************************************************************************/
void SettingMeterReset(void)
{
	g_sys_setting.NavStyle = 2,
	g_sys_setting.Custommenucombinations = 0,	
	g_sys_setting.backLightValue = 21, 
    g_sys_setting.backLightNightValue = 20,

	NvM_WriteBlock(g_sys_settingNvMBlockID, (uint8_t *)&g_sys_setting);
	g_sys_setting_up_flag = 1;
}


