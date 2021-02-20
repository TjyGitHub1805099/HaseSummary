/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _UDS_H
#define _UDS_H
#include"diagCommon.h"

typedef enum{
	UDS_LOG_LEVEL_1,
	UDS_LOG_LEVEL_2,
	UDS_LOG_LEVEL_3
}tUdsLogLevel;


typedef enum{
	UDS_CH_STATUS_IDLE,
	UDS_CH_STATUS_RXING_MULTIPLE_FRAME,
	UDS_CH_STATUS_NEW_DATA,
	UDS_CH_STATUS_PENDING,
	UDS_CH_STATUS_RESP_FINAL_RESULT,
	UDS_CH_STATUS_WAIT_TXCFM,
	UDS_CH_STATUS_WAIT_NRC_TXCFM,
	UDS_CH_STATUS_GETTEN_TXCFM,
	UDS_CH_STATUS_WAIT_NRC_0X78_TXCFM,
	//CH_STATUS_WAIT_NORMAL_TXCFM
}tUdsChStatus;

#if 0
typedef enum{
	ACTIVE_CH_STATUS_IDLE,
	ACTIVE_CH_STATUS_NEW_DATA,
	ACTIVE_CH_STATUS_BUSY,
	ACTIVE_CH_STATUS_RESP_POSITIVE,
	ACTIVE_CH_STATUS_RESP_NRC,
	//ACTIVE_CH_STATUS_RESP_NRC_0x78,
	ACTIVE_CH_STATUS_WAIT_FOR_TXCFM,
	//ACTIVE_CH_STATUS_WAIT_FOR_0X78_TXCFM,
	ACTIVE_CH_STATUS_RESEND_NRC_DATA,
	ACTIVE_CH_STATUS_RESEND_POSITIVE_DATA,
}tActiveChStatus;

typedef struct _tActiveChStatus{
	tUDSAddrType addrType;
	uint32 chId;
	uint8 priority;
	tActiveChStatus chStatus;
	uint16 dataLen;
	uint8* buffer;
}tActiveChInfor;
#endif

typedef enum{
	UDS_TIMER_TYPE_NONE,
	UDS_TIMER_TYPE_P2_SERVER_MAX,
	UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED,
	UDS_TIMER_TYPE_S3_SERVER
}tUdsTimerType;

typedef struct _tUDSTimer{
	tUdsTimerType type;
	boolean timeoutFlag;
	uint32 interval;
	uint32 preTick;
}tUdsTimer;

typedef struct _tUdsChInfor{
	uint32 udsChType;
	boolean reqFromFuncAddrFlag;
	tUdsChStatus chStatus;
	boolean tmp_resp_flag;
	uint8 chPrio;//channel priority,the value is bigger, the prio is higher
	uint16 dataLen;
	uint16 bufferMaxLen;
	uint8 *buffer;
}tUdsChInfor;

typedef enum{
	UDS_WORK_MODE_IN_APP,
	UDS_WORK_MODE_IN_BOOTLOADER,
}tUdsWorkMode;


typedef struct{
	uint32 maxChNum;
	tUdsWorkMode udsWorkMode;
	uint16 udsRxTxBufferLen;
	boolean enableBroadcastUdsCnnStatus;
	uint32 sigId;
	boolean enableRespNrcIfChBusy;
	boolean enableRespNrcSNSIASandSFNSIASforFuncAddrReq;
	boolean enableRespNrc0x78;
	uint16 p2ServerMaxTimerValue;
	uint16 p2ServerMaxEnhancedTimerValue;
	uint16 s3ServerTimerValue;
	uint8 udsResendCounter;
	uint32 diagAppTaskId;
	uint32 dtcCoreTaskId;
	tUdsLogLevel udsLogLevel;
	tUdsChInfor *udsChInfor;
}tUdsCfgInfor;

#endif
