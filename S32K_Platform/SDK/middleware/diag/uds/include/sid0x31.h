/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X31_H
#define _SID_0X31_H

#include"diagCommon_if.h"


typedef enum{
	ROUTINE_CTRL_TYPE_START_ROUTINE = 1,
	ROUTINE_CTRL_TYPE_STOP_ROUTINE = 2,
	ROUTINE_CTRL_TYPE_REQ_ROUTINE_RESULTS = 3,
}tRoutineCtrlType;

typedef enum{
	ROUTINE_CTRL_STATUS_STOP,
	ROUTINE_CTRL_STATUS_IN_PROGRESS
}tRoutineCtrlStatus;

typedef enum{
	ROUTINE_CTL_DUMMY_REQ = 0x0101,
}tRoutineCtrlID;

#define ROUTINUE_CTRL_BUFF_LEN 64

typedef struct{
	uint8 len;
	uint8 buffer[ROUTINUE_CTRL_BUFF_LEN];
}tRoutineCtrlBuffer;

typedef struct{
	tRoutineCtrlID did;
	tRoutineCtrlStatus status;
}tRoutineCtrlStatusArray;


typedef boolean (*tRoutineCtrlVerifyDataCbk)(const uint8*data,uint16 len);
typedef boolean (*tRoutineCtrlCheckPreconditionCbk)(const uint8*data,uint16 len);
typedef tRoutineCtrlBuffer* (*tRoutineCtrlStartCbk)(const uint8*data,uint16 len);
typedef tRoutineCtrlBuffer* (*tRoutineCtrlStopCbk)(const uint8*data,uint16 len);
typedef tRoutineCtrlBuffer* (*tRoutineCtrlReqResultCbk)(const uint8*data,uint16 len);


typedef struct{
	tRoutineCtrlID routineId;
	uint32 sessionFilter;//tUdsSessionInnerType
	uint32 securityLevelFilter;//tInnerSecurityLevel
	boolean getRespSyncFlag;
	tRoutineCtrlVerifyDataCbk verifyDataCbk;
	tRoutineCtrlCheckPreconditionCbk checkPreCondCbk;
	tRoutineCtrlStartCbk startCbk;
	tRoutineCtrlStopCbk stopCbk;
	tRoutineCtrlReqResultCbk reqResultCbk;
}tRoutineCtlInfor;

typedef struct{

}tSid0x31CfgInfor;


#endif
