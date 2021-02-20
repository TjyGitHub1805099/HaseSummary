#ifndef _SYS_SETTING_H_
#define _SYS_SETTING_H_
#include "NvM.h"
#include "MemMap.h"
#include "NvM_if.h"
#include "Task_IpcApp_Cfg.h"

#define WiperWithSpeedOn 	0
#define WiperWithSpeedOff 	1

#define TransLightOffDayFromHmiToCan(x) ((x==0)?1:((x==1)?2:(x==2?0:x)))
#define TransLightOffDayFromCanToHmi(x) ((x==0)?2:((x==1)?0:(x==2?1:x)))


typedef void (*SysSettingFuncType)(uint16_t);

extern NVM_SEC_VAR_UNSPECIFIED IPC_APP_SYS_SETTING_STRU g_sys_setting;

void SysSettingLanguageSet(uint16_t value);

void SysSettingUnitSet(uint16_t value);

void SysSettingTirePressureSet(uint16_t value);

void SysSettingDriveAssistantSet(uint16_t value);
void SysSettingAutoStopSingleSet(uint16_t value);

void SysSettingAutoStopSumSet(uint16_t value);

void SysSettingSoundSet(uint16_t value);

void SysSettingPowerCycleSet(uint16_t value);

void SysSettingMapTypeSet(uint16_t value);

void SysSettingPlayEffectSet(uint16_t value);

void SysSettingAlertSoundSet(uint16_t value);

void SysSettingVehicleDeviateAlertSet(uint16_t value);
void SysSettingBlindAreaMonitoringUpdate(uint16_t value);
uint8_t SysSettingVehicleDeviateAlertGet(void);

void SysSettingBlindAreaMonitoringSet(uint16_t value);
void SysSettingVehicleDeviateAlertUpdate(uint16_t value);
uint8_t SysSettingVehicleDeviateAlertGet(void);


void SysSettingEmergencyBrakingSet(uint16_t value);
void SysSettingEmergencyBrakingUpdate(uint16_t value);
uint8_t SysSettingEmergencyBrakingGet(void);


void SysSettingParkAssistantSet(uint16_t value);

uint8_t SysSettingParkAssistantGet(void);

void SysSettingParkAssistantUpdate(uint16_t value);

uint16_t SysSettingAutoStopSingleGet(void);

uint16_t SysSettingAutoStopSumGet(void);

void SysSettingDriveTimeAlertSet(uint16_t value);

void SysSettingDriveTimeSetSet(uint16_t value);

void SysSettingMaintenanceMileageSet(uint16_t value);

void SysSettingMaintenanceTimeSet(uint16_t value);

void SysSettingbackLightValueSet(uint16_t value);

void SysSettingSpeedLimitSet(uint16_t value);

void SysSettingbestFuelTimeOutSet(uint16_t value);

void SysSettingFrontTireSet(uint16_t value);

void SysSettingRearTireSet(uint16_t value);

void SysSettingHmiLastPageSet(uint16_t value);

void SysSettingIlluminationSettingSet(uint16_t value);

void SysSettingTimeFormatSet(uint16_t value);

void SysSettingNaviModeSet(uint16_t value);

void SysSettingNavStyleSet(uint16_t value);

void SysSettingEAPM_SettingSet(uint16_t value);

uint8_t SysSettingEAPMGet(void);

void SysSettingLightSensitivitySet(uint16_t value);

void SysSettingLightSensitivityUpdate(uint16_t value);

void SysSettingLightOffDaySet(uint16_t value);

void SysSettingLightOffDayUpdate(uint16_t value);

void SysSettingSelectiveUnlockSet(uint16_t value);

void SysSettingSelectiveUnlockUpdate(uint16_t value);

void SysSettingWiperWithSpeedSet(uint16_t value);

void SysSettingWiperWithSpeedUpdate(uint16_t value);

uint8_t SysSettingLightSensitivityGet(void);

uint8_t SysSettingLightOffDayGet(void);

uint8_t SysSettingSelectiveUnlockGet(void);

uint8_t SysSettingWiperWithSpeedGet(void);

void SysSettingCarBodySetInProgressSet(uint8_t value);

uint8_t SysSettingCarBodySetInProgressGet(void);

IPC_APP_SYS_SETTING_STRU SysSettingGet(void);

uint8_t SysSettingUpFlagGet(void);

void SysSettingUpFlagSet(uint8_t value);

void SysSettingFactoryReset(void);

void SysSettingVehicleReset(void);
/* yangxl */
void SysSettingAutomaticClosingWindowSet(uint16_t value);
void SysSettingBrushAwayTheRainSet(uint16_t value);
void SysSettingRearviewMirrorFoldsAutomaticallySet(uint16_t value);
void SysSettingRearviewMirrorSwitchSet(uint16_t value);
void SysSettingAutomaticLockingSet(uint16_t value);
void SysSettingAutomaticUnlockSet(uint16_t value);
void SysSettingIntensityOfLightSet(uint16_t value);
void SysSettingTurnOffLightsDelaySet(uint16_t value);
void SysSettingCarLockPromptSet(uint16_t value);
void SysSettingRestoreFactorySettingsSet(uint16_t value);
void SysSettingRainfallSensitivitySet(uint16_t value);
void SysSettingFEBSwitchSet(uint16_t value);
void SysSettingLKASwitchSet(uint16_t value);
void SysSettingBSWSwitchSet(uint16_t value);
void SysSettingCTASwitchSet(uint16_t value);
void SysSettingEAPMSwitchSet(uint16_t value);
void SysSettingDOASwitchSet(uint16_t value);
void SysSettingLDWSwitchSet(uint16_t value);
void SysSettingECOSwitchSet(uint16_t value);
void SysSettingISSSwitchSet(uint16_t value);
void SysSettingUPASwitchSet(uint16_t value); //yangxl 2019-11-14 17:08:19
extern uint8_t SysSettingECOSwitchGet(void);

void SysSettingFEBSwitchUpdate(uint16_t value);
void SysSettingLKASwitchUpdate(uint16_t value);
void SysSettingBSWSwitchUpdate(uint16_t value);
void SysSettingEAPMSwitchUpdate(uint16_t value);
void SysSettingLDWSwitchUpdate(uint16_t value);
void SysSettingISSSwitchUpdate(uint16_t value);
void SysSettingECOSwitchUpdate(uint16_t value);
void SysSettingUPASwitchUpdate(uint16_t value); //yangxl 2019-11-14 17:08:19


void SysSettingHUDSwitchSet(uint16_t value);
void SysSettingHUDNavigationDisplaySwitchSet(uint16_t value);
void SysSettingHUDDrivingAssistanceSwitchSet(uint16_t value);
void SysSettingHUDTachometerSwitchSet(uint16_t value);
void SysSettingHUDFuelConsumptionSwitchSet(uint16_t value);
void SysSettingHUDMusicStateSwitchSet(uint16_t value);
void SysSettingHUDMixedModeSwitchSet(uint16_t value);
void SysSettingHUDCallReminderSwitchSet(uint16_t value);
void SysSettingHUDLyricSwitchSet(uint16_t value);
void SysSettingCustommenucombinationsSet(uint16_t value);
void SysSettingHUDRotationAngleSet(uint16_t value);
void SysSettingHUDStyleSet(uint16_t value);



uint8_t SysSettingFEBSwitchGet(void);
uint8_t SysSettingLKASwitchGet(void);
uint8_t SysSettingBSWSwitchGet(void);
uint8_t SysSettingEAPMSwitchGet(void);
uint8_t SysSettingLDWSwitchGet(void);
uint8_t SysSettingISSSwitchGet(void);
uint8_t SysSettingUPASwitchGet(void); //yangxl 2019-11-14 17:08:19


void SysSettingbackLightNightValueSet(uint16_t value); //yangxl2019-7-15 14:13:37

uint8_t SysSettingRearviewMirrorFoldsAutomaticallyGet(void);//yangxl 2019-7-22 16:56:29
uint8_t SysSettingAutomaticLockingGet(void); //yangxl 2019-7-22 16:56:29
uint8_t SysSettingAutomaticClosingWindowGet(void);//yangxl 2019-7-22 16:56:29
uint8_t SysSettingAutomaticUnlockGet(void);//yangxl 2019-7-22 16:56:29
uint8_t SysSettingCarLockPromptGet(void); //yangxl 2019-7-22 16:56:29
uint8_t SysSettingBrushAwayTheRainGet(void); //yangxl 2019-7-22 16:56:29
uint8_t SysSettingIntensityOfLightGet(void);
uint8_t SysSettingTurnOffLightsDelayGet(void);



void SysSettingAutomaticClosingWindowUpdate(uint16_t value);
void SysSettingRearviewMirrorFoldsAutomaticallyUpdate(uint16_t value); //yangxl 2019-7-22 16:56:29
void SysSettingAutomaticLockingUpdate(uint16_t value);
void SysSettingAutomaticUnlockUpdate(uint16_t value);
void SysSettingCarLockPromptUpdate(uint16_t value);
void SysSettingBrushAwayTheRainUpdate(uint16_t value);
void SysSettingIntensityOfLightUpdate(uint16_t value);
void SysSettingTurnOffLightsDelayUpdate(uint16_t value);




void SysSettingInit(void);



void SettingMeterReset(void); //yangxl 2019-12-23 11:07:16
void SettingHudReset(void); //yangxl 2019-12-23 11:07:21










/* yangxl */

extern const SysSettingFuncType SysSettingFuncList[];

#endif
