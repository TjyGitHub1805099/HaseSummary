/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DIAG_COMMON_H
#define _DIAG_COMMON_H

#include"Std_Types.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#define SW_PHASE_APPLICATION

#define UDS_WITH_SID0X10
#define UDS_WITH_SID0X11
#define UDS_WITH_SID0X28

#define UDS_WITH_DTC
#define UDS_WITH_SID0X22
#define UDS_WITH_SID0X23
#define UDS_WITH_SID0X2F
#define UDS_WITH_SID0X31
#define UDS_WITH_SID0X27
#define UDS_WITH_DOWNLOAD

typedef  void*  tDiagMutex;

#if 1
extern void TracePrintf (uint16 moduleId, uint8 level,const char *fmt, ...);

#define DIAG_TRACE_TXT(module,trace_level,txt) TracePrintf(module,trace_level,txt)
#define DIAG_TRACE_VALUE1(module,trace_level,txt,value) 		TracePrintf(module,trace_level,txt,value)
#define DIAG_TRACE_VALUE2(module,trace_level,txt,value1,value2) 	TracePrintf(module,trace_level,txt,value1,value2)
#define DIAG_TRACE_VALUE3(module,trace_level,txt,value1,value2,value3) 	TracePrintf(module,trace_level,txt,value1,value2,value3)
#define DIAG_TRACE_VALUE4(module,trace_level,txt,value1,value2,value3,value4) TracePrintf(module,trace_level,txt,value1,value2,value3,value4)
#define DIAG_TRACE_VALUE5(module,trace_level,txt,value1,value2,value3,value4,value5) TracePrintf(module,trace_level,txt,value1,value2,value3,value4,value5)
#else
#define DIAG_TRACE_TXT(module,trace_level,txt)
#define DIAG_TRACE_VALUE1(module,trace_level,txt,value)
#define DIAG_TRACE_VALUE2(module,trace_level,txt,value1,value2)
#define DIAG_TRACE_VALUE3(module,trace_level,txt,value1,value2,value3)
#define DIAG_TRACE_VALUE4(module,trace_level,txt,value1,value2,value3,value4)
#define DIAG_TRACE_VALUE5(module,trace_level,txt,value1,value2,value3,value4,value5)

#endif

#define DIAG_CMD_DELAY_DETECT_DTC_REQ 0x56
#define DIAG_CMD_RESTART_DETECTING_DTC_REQ 0x57
#define DIAG_CMD_STOP_ALL_ROUNTE_CTRL_TEST 0x58
#define DIAG_CMD_STOP_ALL_IO_CTRL_TEST 0x59


#define DIAG_ACC_ON 1
#define DIAG_ACC_OFF 0

#define DIAG_KL15_ON 1	//IGN ON
#define DIAG_KL15_OFF 0 // IGN OFF

#define DIAG_KL30_START_CRANK 1 // engine start,this is a Instantaneous status, about 2 seconds
#define DIAG_KL30_STOP_CRANK 0 


typedef enum{
	UDS_ADDR_TYPE_FUNC = 1,
	UDS_ADDR_TYPE_PHY = 2,
}tUDSAddrType;


typedef struct{
	boolean enable;
	boolean timeoutFlag;
	uint16  tick;
}tDiagTimer;

typedef enum{
	DIAG_STOP_REASON_NONE,
	DIAG_STOP_REASON_LOW_VOLT,
	DIAG_STOP_REASON_HIGH_VOLT,
	DIAG_STOP_REASON_NM_OFF,
	DIAG_STOP_REASON_KL15_OFF_OR_LOW_VOL,
	DIAG_STOP_REASON_OTHER
}tDiagStopReason;

typedef enum{
	UDS_DEFAULT_SESSION = 0x1,	// EXTERNAL: 1
	UDS_PROGRAMMING_SESSION = 0x2,	// EXTERNAL: 2
	UDS_EXTENDED_DIAG_SESSION = 0x4,	//EXTERNAL: 3
	UDS_SAFETY_SYS_DIAG_SESSION = 0x8,	//EXTERNAL: 4
	UDS_CODING_SESSION = 0x10,			//EXTERNAL:41		
}tUdsSessionInnerType;

typedef enum{
	UDS_EXTERNAL_DEFAULT_SESSION = 0x1,
	UDS_EXTERNAL_DEFAULT_SESSION_SUPPRESS_POS_RESP = 0x81,
	UDS_EXTERNAL_PROGRAMMING_SESSION = 0x2,
	UDS_EXTERNAL_PROGRAMMING_SESSION_SUPPRESS_POS_RESP = 0x82,
	UDS_EXTERNAL_EXTENDED_DIAG_SESSION = 0x3,
	UDS_EXTERNAL_EXTENDED_DIAG_SESSION_SUPPRESS_POS_RESP = 0x83,
	UDS_EXTERNAL_SAFETY_SYSTEM_DIAG_SESSION = 0x4,
	UDS_EXTERNAL_SAFETY_SYSTEM_DIAG_SESSION_SUPPRESS_POS_RESP = 0x84,
	UDS_EXTERNAL_CODING_SESSION = 0x41,
	UDS_EXTERNAL_CODING_SESSION_SUPPRESS_POS_RESP = 0xc1,
}tUdsSessionExternalType;


typedef enum{
	NRC_GENERAL_REJECT = 0x10,
	NRC_SERVICE_NOT_SUPPORTED = 0x11,
	NRC_SUB_FUNCTION_NOT_SUPPORTED = 0x12,	
	NRC_INCORRECT_MESSAGE_LENGTH = 0x13,
	NRC_RESPONSE_TOO_LONG = 0x14,
	NRC_BUSY_REPEAT_REQ = 0x21,
	NRC_CONDITION_NOT_CORRECT = 0x22,
	NRC_REQUEST_SEQUENCE_ERROR = 0x24,
	NRC_REQUEST_OUT_OF_RANGE = 0x31,
	NRC_SECURITY_ACCESS_REQUEST = 0x33,
	NRC_INVALID_KEY = 0x35,
	NRC_EXCEED_NUMBER_OF_ATTEMPTS = 0x36,
	NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED = 0x37,
	NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED = 0x70,
	NRC_TRANSFER_DATA_SUSPENDED = 0x71,
	NRC_GENERAL_PROGRAMMING_FAILURE = 0x72,
	NRC_WRONG_BLOCK_SEQ_COUNTER = 0x73,
	NRC_RESPONSE_PENDING = 0x78,
	NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION = 0x7E,
	NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION = 0x7F,
	NRC_VOLTAGE_TOO_HIGH = 0x92,
	NRC_VOLTAGE_TOO_LOW = 0x93,
	NRC_STOP_PROCESS = 0xfe,//due to internl error,process needs to be stopped
	NRC_NONE = 0xff//positive resp
}tUdsNRCType;



typedef enum{
	SID_SESSION_CONTROL = 0x10,
	SID_ECU_RESET = 0x11,
	SID_CLEAR_DTC = 0x14,
	SID_READ_DTC = 0x19,
	SID_READ_DATA_BY_DID = 0x22,
	SID_READ_MEMORY_BY_ADDRESS	= 0x23,
	SID_READ_SCALING_DATA_BY_DID = 0x24,
	SID_SECURITY_ACCESS = 0x27,
	SID_COMMUNICATION_CONTROL = 0x28,
	SID_READ_DATA_BY_PERIODIC_DID = 0x2A,
	SID_DYNAMICALLY_DEFINE_DATA_DID = 0x2C,
	SID_WRITE_DATA_BY_DID = 0x2E,
	SID_IOCONTROL = 0x2F,
	SID_WRITE_MEMORY_BY_ADDRESS = 0x3D,
	SID_ROUTINE_CONTROL = 0x31,
	SID_REQUEST_DOWNLOAD = 0x34,
	SID_REQUEST_UPLOAD = 0x35,
	SID_TRANSFER_DATA = 0x36,
	SID_REQUEST_TRANSFER_EXIT = 0x37,
	SID_REQUEST_FILE_TRANSFER = 0x38,
	SID_TESTER_PRESENT = 0x3E,
	SID_ACCESS_TIMING_PARAMETER = 0x83,
	SID_SECURED_DATA_TRANSMISSION = 0x84,
	SID_CONTROL_DTC_SETTING = 0x85,
	SID_RESPONSE_ON_EVENT = 0x86,
	SID_LINK_CONTROL = 0x87,
	SID_INNER_TEST = 0x99,
}tSidType;

typedef enum{
	DID_READ = 1,
	DID_WRITE = 2,
	DID_READ_WRITE = 3,
	DID_AME_WRITE = 8 // it means this DID value can only be written by AME.
}tRdWrFlag;

typedef enum{
	UDS_NONE_RESET_TYPE = 0,
	UDS_HARD_RESET = 0x1,
	UDS_HARD_RESET_SUPPRESS_POS_RESP = 0x81,
	UDS_KEY_OFF_ON_RESET = 0x2,
	UDS_KEY_OFF_ON_RESET_SUPPRESS_POS_RESP =0x82,
	UDS_SOFT_RESET = 0x3,
}tUdsResetSysType;

typedef struct{
	uint8 Year;
	uint8 Month;
	uint8 Day;
	uint8 Hour;
	uint8 Min;
	uint8 Sec;
}tDiagCANTime;

typedef struct{
	uint8 ch;
	boolean reqFromFuncAddrFlag;
	uint16 len;
	uint8* data;
	uint32 reqTick;
}tUdsReqInfor;

typedef struct{
	uint8 ch;
	boolean dataFromSocFlag;
	boolean finishFlag;
	uint32 respCode;
	uint16 len;
	uint8* data;
	uint32 recordTick;
}tUdsRespInfor;


typedef void (*tDiagFnCbkWithPara)(uint32 ch,void *data,uint16 len);
typedef void (*tDiagFnCbkNoPara)(void);

typedef enum{
	DIAG_SYS_VOLTAGE_STATUS_NORMAL,
	DIAG_SYS_VOLTAGE_STATUS_HIGH,
	DIAG_SYS_VOLTAGE_STATUS_LOW,
}tDiagSysVoltageStatus;


#endif
