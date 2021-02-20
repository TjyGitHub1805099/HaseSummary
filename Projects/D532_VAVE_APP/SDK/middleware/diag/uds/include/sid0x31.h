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


#define ROUTINUE_CTRL_BUFF_LEN 64

typedef struct{
	uint8 len;
	uint8 buffer[ROUTINUE_CTRL_BUFF_LEN];
}tRoutineCtrlBuffer;

typedef struct{
	uint16 did;	//tRoutineCtrlID
	tRoutineCtrlStatus status;
}tRoutineCtrlStatusInfor;


typedef boolean (*tRoutineCtrlVerifyDataCbk)(const uint8*data,uint16 len);
typedef boolean (*tRoutineCtrlCheckPreconditionCbk)(const uint8*data,uint16 len);
typedef tUdsNRCType (*tRoutineCtrlStartCbk)(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
typedef tUdsNRCType (*tRoutineCtrlStopCbk)(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
typedef tUdsNRCType (*tRoutineCtrlReqResultCbk)(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);


typedef struct{
	uint16 routineId;//tRoutineCtrlID
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
