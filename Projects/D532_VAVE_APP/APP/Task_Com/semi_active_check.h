#ifndef _SEMI_ACTIVE_CHECK_H_
#define _SEMI_ACTIVE_CHECK_H_

#include "dea.h"
#include "Task_IpcApp_Cfg.h"
#include "Std_Types.h"

typedef enum
{
	semiActiveDisplayUnitIndex = 0,
	semiActiveLanguageIndex,
	semiActiveTPMSWithIndex1,
	semiActiveTPMSWithIndex2,
	semiActiveServiceIndex,
	semiActiveOATWithIndex,
	semiActiveClockWithoutNavi_AudioIndex,
	semiActiveNAVI_AUDIOIndex,
	semiActiveITS_BSWIndex,
	semiActiveITS_LDWIndex,
	semiActiveITS_FEBIndex,
	semiActiveAT_MTIndex,
	semiActiveIdleStopFunctionIndex,
	semiActiveE_PKB_And_AVHIndex,
	semiActiveIBA_OFFIndex,
	semiActiveIndirectIlluminationIndex,
	semiActiveRearSeatIndex,
	semiActiveSLIPIndex,
	semiActiveI_KEYIndex,
	semiActiveOAT_WithIndex,
	semiSonarIndex,
	semiLED_HeadlampIndex,
	semiEAPMIndex,
	semiSelectiveUnlockIndex,
	semiWiperWithSpeedIndex,
	semiLightSensitivityIndex,
	semiLightOffDayIndex,
	semiActiveITS_ACCIndex, //yangxl
	semiActiveITS_LKAIndex, //yangxl
	semiActiveShiftByWireIndex, //yangxl
	semiMaxIndex,
}SEMI_INDEX_ENUM;


extern void SemiActiveCheckInit(void);
extern void SemiActiveCheckProc(void);

extern uint8_t semiActiveDisplayUnitGet(void);
extern uint8_t semiActiveLanguageGet(void);
	
extern uint8_t semiActiveTPMSWithGet(void);

extern uint8_t semiActiveServiceGet(void);

extern uint8_t semiActiveOATWithGet(void);

extern uint8_t semiActiveClockWithoutNavi_AudioGet(void);

extern uint8_t semiActiveNAVI_AUDIOGet(void);

extern uint8_t semiActiveITS_BSWGet(void);

extern uint8_t semiActiveITS_LDWGet(void);

extern uint8_t semiActiveITS_FEBGet(void);

extern uint8_t semiActiveAT_MTGet(void);

extern uint8_t semiActiveIdleStopFunctionGet(void);

extern uint8_t semiActiveE_PKB_And_AVHGet(void);

extern uint8_t semiActiveIBA_OFFGet(void);

extern uint8_t semiActiveIndirectIlluminationGet(void);

extern uint8_t semiActiveRearSeatGet(void);

extern uint8_t semiActiveSLIPGet(void);

extern uint8_t semiActiveI_KEYGet(void);

extern uint8_t semiActiveOAT_WithGet(void);

extern uint8_t semiSonarGet(void);

extern uint8_t semiLED_HeadlampFlagGet(void);

extern uint8_t semiEAPMFlagGet(void);

extern uint8_t semiSelectiveUnlockFlagGet(void);

extern uint8_t semiWiperWithSpeedFlagGet(void);

extern uint8_t semiLightSensitivityFlagGet(void);

extern uint8_t semiLightOffDayFlagGet(void);

extern void semiSelectiveUnlockFlagSet(uint8_t value);

extern void semiWiperWithSpeedFlagSet(uint8_t value);

extern void semiLightSensitivityFlagSet(uint8_t value);

extern void semiLightOffDayFlagSet(uint8_t value);

extern void semiCarAssistSettingReset(void);

extern uint8_t semiActiveITS_TJPGet(void); /*yangxl */
extern uint8_t semiActiveITS_ICCGet(void);

extern uint8_t semiActiveUpdateFlagGet(void); 

extern void semiActiveUpdateFlagSet(uint8_t value);


extern uint8_t semiAutoFoldFunctionFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiAutoLockFunctionCustomizeFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiLockAutoWindowUpFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiAutoUnlockFunctionCustomizeFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiAnswerBackFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiRainSensorFlagGet(void); //yangxl 2019-7-22 16:18:37
extern uint8_t semiWiperWithWipingDripFlagGet(void); //yangxl 2019-7-22 16:18:37

extern void semiAutoFoldFunctionFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiAutoLockFunctionCustomizeFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiLockAutoWindowUpFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiAutoUnlockFunctionCustomizeFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiAnswerBackFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiRainSensorFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37
extern void semiWiperWithWipingDripFlagSet(uint8_t value); //yangxl 2019-7-22 16:18:37

extern uint8_t semiActiveITS_LKAGet(void); //yangxl 2019-7-25 10:52:00
extern uint8_t semiActiveITS_ACCGet(void); //yangxl 2019-7-25 10:51:55
extern uint8_t semiActiveShiftByWireGet(void); //yangxl 2019-11-26 15:11:25







#endif
