/********************************************************************
File name: Alarm.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-01-18 12:07:19
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _ALARM_H_
#define _ALARM_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "Alarm_Cfg.h"

#define ALARM_STATE_STOPPED (0U)
#define ALARM_STATE_RUNNING (1U)
#define ALARM_STATE_PAUSE (2U)
#define ALARM_STATE_OVERFLOW (3U)

typedef enum
{
	ALARM_TYPE_NONE,
	ALARM_TYPE_SINGLE,
	ALARM_TYPE_CYCLE,
} AlarmTypeEType;

typedef struct
{
	uint8 state;
	uint32 counter;
	uint8 regCount;
} Alarm_RuntimeInfoType;

typedef struct
{
	uint8 start;
	AlarmTypeEType type;
	uint32 time;
	uint8 regMaxNum;
} Alarm_ConfigInfoType;
	
extern void Alarm_Init(uint16 moduleOffset);
extern void Alarm_Main(uint32 count);
extern boolean Alarm_Start(uint16 alarmId);
extern boolean Alarm_Stop(uint16 alarmId);
extern boolean Alarm_Pause(uint16 alarmId);
extern boolean Alarm_Resume(uint16 alarmId);
extern boolean Alarm_Subscribe(uint16 alarmId, uint16 taskId);
/*
*stauts
*0: ALARM_STATE_STOPPED
*1: ALARM_STATE_RUNNING
*2: ALARM_STATE_PAUSE
*3: ALARM_STATE_OVERFLOW
*/
extern boolean Alarm_GetStatus(uint16 alarmId, uint32* status);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: Alarm.h ===========================*/

