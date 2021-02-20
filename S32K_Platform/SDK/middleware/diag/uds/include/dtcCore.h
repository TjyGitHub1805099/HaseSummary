/**************************
description: DTC Core
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/08
**************************/


#ifndef _DTC_CORE_H
#define _DTC_CORE_H
#include"diagCommon_if.h"

#undef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
#define DTC_NVM_SPACE_IS_EMPTY 0xff

#if 0
	#undef ONCE_EVENT_DTC
#else
	#define ONCE_EVENT_DTC
#endif

#define DTC_AGING_COUNTER_LIMIT 40
#define DTC_APP_TASKID_LIST_LEN 5

typedef enum{
	DTC_LOG_LEVEL_0,
	DTC_LOG_LEVEL_1,
	DTC_LOG_LEVEL_2,
}tDtcLogLevel;

typedef enum{
	REPORT_NUM_OF_DTC_BY_STATUS_MASK  = 1,
	REPORT_DTC_BY_STATUS_MASK = 2,
	REPORT_DTC_SNAPSHOT_RECORD_BY_DTC_NUM = 4,
	REPORT_DTC_SNAPSHOT_RECORD_BY_RECORD_NUM = 5,//not supported
	REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUM = 6,
	REPORT_SUPPORTED_DTC = 0xa,
	
}tReportDTCInforType;

typedef enum{
	DTC_RESET_BY_INIT,
	DTC_RESET_BY_SID_0X14,
	DTC_RESET_BY_AGING_COUNTER_EXCEED,
	DTC_RESET_BY_NOT_CODED,
	DTC_RESET_BY_VALID_CODING,
	DTC_RESET_BY_NOT_CHECK_CODING
}tDTCResetReason;

typedef enum{
	DTC_GROUP_NONE = 0x00,
	DTC_GROUP_EMISSIONS_RELATED = 0xFFFF33,
	DTC_GROUP_POWERTRAIN = 0x100000,
	DTC_GROUP_CHASSIS = 0x400000,
	DTC_GROUP_BODY = 0x800000,
	DTC_GROUP_NETWORK = 0xC00000,
	DTC_GROUP_ALL = 0xFFFFFF
}tDtcGroup;

typedef struct{
	uint32 extDtcId;//tExtDTCId
	uint16 innerDtcId;//tInnerDTCId
}tDTCIdMap;

typedef enum{
	STD_DTC_INFOR = 1,
	STD_FREEZE_FRAME = 2,
	STD_FREEZE_FRAME_DYNAMIC=0x71,
	ALL_DTC_EXT_DATA_RECORD_NUMBERS= 0xFF
}tDtcSubType;

#define DTC_STATUS_AVAILABILITY_MASK 0x7f

typedef enum{
	DTC_STATUS_BIT_NONE = 0x0,
	DTC_STATUS_BIT_TEST_FAILED = 0x1,	//reset from 1 to 0 if addition test is ok or sid 0x14
	DTC_STATUS_BIT_TEST_FAILED_THIS_OPERATION_CYCLE = 0x2,//reset from 1 to 0 if new operation cycle starts or sid 0x14. and if bitConfirmed is 1,this bit must be 1 until next operation cycle
	DTC_STATUS_BIT_PENDING = 0x4, // in last operation cyclic,if test completed, and no failed, then in this operation cyclic, set this bit to 0
	DTC_STATUS_BIT_CONFIRMED = 0x8,
	DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR = 0x10, // reset to 1 after SID 0x14, then if test can be completed at least once, set this bit to 0
	DTC_STATUS_BIT_TEST_FAILED_SINCE_LAST_CLEAR = 0x20, // if testFailed once after SId 0x14, set this bit to 1
	DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE = 0x40 // if test can be running once, then set this bit to 0

}tDtcStatusBit;

//#define DTC_DEFAULT_STATUS (DTC_STATUS_BIT_TEST_NOT_COMPLETE_SINCE_LAST_CLEAR|DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)
#define DTC_DEFAULT_STATUS (DTC_STATUS_BIT_TEST_NOT_COMPLETE_THIS_OPERATION_CYCLE)

typedef enum{
	DTC_PRE_RECORD_NONE = 0x0,
	DTC_PRE_RECORD_TEST_NOT_COMPLETED_LAST_OPERATION_CYCLE = 0x1,
	DTC_PRE_RECORD_TEST_FAILED_IN_LAST_OPERATION_CYCLE = 0x2,
}tDtcPreRecord;

typedef enum{
	DTC_ENABLE,
	DTC_DISABLE
}tDtcEnableFlag;

typedef enum{
	DIAG_IGN_STATUS_OFF = 0,	// 0, ALL OFF
	DIAG_IGN_STATUS_ACC,	// 1, ACC on, kl15 off,kl30 not start
	DIAG_IGN_STATUS_ON,		// 2, acc on, kl15 on, kl 30 not start
	DIAG_IGN_STATUS_START,	// 3, acc on, kl15 on, KL30 START
	DIAG_IGN_STATUS_RESV_4,
	DIAG_IGN_STATUS_RESV_5,
	DIAG_IGN_STATUS_RESV_6,
	DIAG_IGN_STATUS_INVALID
}tDiagIgnitionStatus;

/*
//pg132
typedef struct _tDtcStdInfor{
	uint8 priority;
	uint8 occCounter;//occurrence counter
}tDtcStdInfor;


//pg133
typedef struct _tDtcFreezeFrame{
	uint8 ageCounter;
	uint16 km_high;
	uint8 km_low;
	uint8 time[6];
}tDtcFreezeFrame;
*/

typedef struct _tDtcExtData{
	uint8 priority;// 4 bits
	uint8 occCounter;
	uint8 agedCounter;//if Dtc is removed due to agingCounter >40,then agedCounter+= 1,it can be reset to 0 only by SID 0x14
	uint8 agingCounter;//if DTC is confirmed,after one KL15 off->on cyclic,if there is no testFailed,this value will be added 1,the max value is 40,if there is any testFailed,reset to 0 at once
}tDtcExtData;

typedef enum{
	CODING_TYPE_NONE,
	CODING_TYPE_CDS,
	CODING_TYPE_VOD
}tCodingType;

typedef enum{
	DTC_PRE_INIT,
	DTC_PRE_PASSIVE,
	DTC_PRE_ACTIVE
}tDtcPassOrActive;

typedef enum{
	DTC_TIMER_NONE,
	DTC_TIMER_DISQUALIFY,
	DTC_TIMER_QUALIFY
}tDtcTimerType;

typedef struct{
	uint16 type;
	boolean timeoutFlag;
	uint32 preTick;
}tDtcTimer;

typedef enum{
	DTC_OPERATION_CLCLIC_BY_POWER_ON,
	DTC_OPERATION_CLCLIC_BY_IGN_ON,
	DTC_OPERATION_CLCLIC_BY_MIX
}tDtcOperationCyclic;

typedef struct _tDtcMixInfor{
	//tInnerDTCId innerId;
	//uint8 nvmBlockId;
	//tDtcOperationCyclic operCycType;
	uint8 operCycType;
	boolean isStoppedByHiLowSysVoltage;
	uint8 traceLogEnableFlag;
	uint32 dtcGroup;
}tDtcMixInfor;


typedef struct _tDtcVmInfor{
	uint32 dtc_id;
	//uint8 codingType;//this DTC is related to coding
	uint8 enableDtc:1;// type:tDtcEnableFlag
	uint8 continueToDetectFlag:1;
	uint8 unlearningByAgingCounterFlag:1;
	uint8 timerCalculatedByUpperLayerFlag:1;
	uint8 preResetReason:3;
	uint8 reserved:1;
	uint32 qual_timer;
	uint32 disqual_timer;
	tDtcTimer curTimer;
	tDtcPassOrActive passiveOrActive;
}tDtcVmInfor;

typedef struct {
	boolean storedInNvmFlag;
	uint8 statusBit;//tDtcStatusBit statusBit;
	tDtcExtData extData;
}tDtcStatusAndExtInfor;

typedef struct _tDtcInfor{
	tDtcVmInfor*  vmInfor;
	tDtcStatusAndExtInfor* nvmInfor;
}tDtcInfor;

typedef struct{
	tDtcLogLevel logLevel;
	uint8 totalDtcNumStoredInNvm;//this para is only effected after ENABLE_STORE_20_PERCENT_DTC_TO_NVM is defined
}tDtcCoreCfgInfor;

typedef struct{
	uint16 len;
	uint8 buffer[64];
}tDtcDataBuffer;


typedef struct _tDtcGlobalSnapshotRecord{
	uint8 timeStamp[6];// ID: 0xf120, year,month,day,hour,minute,second
	uint8 odometer[3];	// ID:0xf121
	uint8 supplyVoltage; // ID:0xf122
	uint8 ignitionStatus;	// ID: 0xf123	
}tDtcGlobalSnapshotRecord;


typedef struct{
	tDtcGlobalSnapshotRecord globalSnapshot;
	//tDtcLocalSnapshotRecord localSnapshot;
}tDtcSnapshot;

typedef struct{
	uint8 innerDtcId;//0xff means this dtc has not stored in nvm
	tDtcStatusAndExtInfor dtcStatusAndExtInfor;
	tDtcSnapshot snapshot;
}tDtcStoredInfor;


#endif
