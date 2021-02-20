#ifndef _CAN_APP_DIAG_CLIENT_H_
#define _CAN_APP_DIAG_CLIENT_H_

#include "Std_Types.h"

#define CAN_APP_DIAG_TASK_PERIOD	(10)
#define CAN_APP_DIAG_TOUT			(2000)	//2S

typedef enum
{
	CAN_APP_DIAG_RET_OK = 0,
	CAN_APP_DIAG_RET_OFT,	//out of times
	CAN_APP_DIAG_RET_NOFT,	//negative response out of times
}CAN_APP_DIAG_RET_ENUM;	

#if 0
typedef enum
{
	CAN_APP_DIAG_CODING_READ_INIT = 0,
	CAN_APP_DIAG_CODING_READ_SW_SESSION,
	CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING,
	CAN_APP_DIAG_CODING_READ_DELAY_TIMER,
	CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK,
	CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED,
	CAN_APP_DIAG_CODING_READ_COMPLETE,
}CAN_APP_DIAG_CODING_READ_ENUM;

typedef enum
{
	DiagWriteNone = 0,
	SwitchSession,			//10:switch session
	LightSensitivity,		//[0,1]点灯灵敏�?
	LightOffDay,			//[0,1]灯光关闭延时
	SelectiveUnlock,		//[0,1]单车门解�?
	WiperWithSpeed,			//[0,1]速度感应雨刷
	VehicleReset,			//vehicle reset
}CAN_APP_DIAG_CODING_WRITE_ENUM;
#else
typedef enum
{
	CAN_APP_DIAG_CODING_READ_INIT = 0,
	CAN_APP_DIAG_CODING_READ_SW_SESSION,
	CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING,
	CAN_APP_DIAG_CODING_READ_DELAY_TIMER,
	//CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK,
	CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION,
	CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION,
	CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP,
	CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION,
	CAN_APP_DIAG_CODING_READ_ANSWER_BACK,
	//CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED,
	CAN_APP_DIAG_CODING_READ_RAIN_SENSOR,
	//CAN_APP_DIAG_CODING_READ_REAR_WIPER_LINKEDTOREVERSE,
	CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP,
	//CAN_APP_DIAG_CODING_READ_REMOTE_ENGINE_START,
	CAN_APP_DIAG_CODING_READ_COMPLETE,
}CAN_APP_DIAG_CODING_READ_ENUM;

typedef enum
{
	DiagWriteNone = 0,
	SwitchSession,			//10:switch session
	LightSensitivity,		//[0,1]点灯灵敏�?
	LightOffDay,			//[0,1]灯光关闭延时
	//SelectiveUnlock,		//[0,1]单车门解�?
	//WiperWithSpeed,			//[0,1]速度感应雨刷
	AutoFoldFunction,
	AutoLockFunctionCustomize,
	LockAutoWindowUp,
	AutoUnlockFunctionCustomize,
	AnswerBack,
	RainSensor,
	WiperWithWipingDrip,
	VehicleReset,			//vehicle reset
}CAN_APP_DIAG_CODING_WRITE_ENUM;
#endif

extern void CanAppDiagRxProc(uint8_t *data, uint16_t len);
extern void CanAppDiagClientProc(void);
extern void CanAppDiagWriteRequestFlagSet(uint8_t falg, uint8_t value);

#endif