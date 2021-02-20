#ifndef _TP_TYPES_H
#define _TP_TYPES_H
#include"Std_Types.h"
#include "tpConfig.h"

#define TP_BUF_LEN 8
#define TP_RX_TMP_BUF_LEN 9
#define TP_VALID_MINI_DATA_LEN 3
#define TP_FIRST_FRAME_DATA_LEN (TP_BUF_LEN-2)
#define TP_CONSECUTIVE_FRAME_DATA_LEN (TP_BUF_LEN-1)

typedef  void (*tTpFuncPtr)();

typedef enum{
	TP_CHANNEL_UDS_FUNC = 0,
	TP_CHANNEL_UDS_PHY,
	TP_CHANNEL_AME,
	TP_CHANNEL_MAX
}tTpChanType;

typedef enum{
	BUFFER_OK = 0,
	BUFFER_WAIT= 1,
	BUFFER_OVERFLOW = 2,
	BUFFER_ERROR = 0xFF,
}tReqBufferResult;

typedef enum{
	CH_UDS_FUNC=0,
	CH_UDS_PHY=1,
	CH_AME=2,
}tChId;


typedef boolean (*tFnRxIndication)(uint32 ch,uint8* dataPtr, uint16 len);
//return value type,in fact is tReqBufferResult
typedef uint32 (*tFnRxFirstFrameIndication)(uint32 ch,uint16 reqBufferLen,uint8 ** bufferPtr);
typedef void (*tFnCancelRxData)(uint32 ch,uint8 reason);
typedef void (*tFnTxDataConfirmation)(uint32 ch,boolean result,uint8 errId);


typedef struct _tplist{
	struct _tplist *next;
	boolean available;
	uint8 data[8];
}tpList;

typedef struct _tpDataQueue{
	tpList *head;
	tpList *tail;
}tpDataQueue;


typedef struct{
	uint16 pduIdRx;//CAN ID
	uint16 pduIdTx;//CAN ID
	tFnRxIndication FnRxIndication;
	tFnRxFirstFrameIndication FnRxFirstFrameIndication;
	tFnCancelRxData FnCancelRxData;
	tFnTxDataConfirmation FnTxDataConfirmation;
}tSessionLayerWrapper;

typedef enum{
	REASON_SF_FRAME_ARRIVED,
	REASON_FF_FRAME_ARRIVED,
	REASON_FAIL_RX_CF_FRAME,
	REASON_FAIL_RX_FC_FRAME,

	REASON_FAIL_TX_FC_FRAME,
	REASON_FAIL_TX_SF_FRAME,
	REASON_FAIL_TX_FF_FRAME,
	REASON_FAIL_TX_CF_FRAME
}tCancelRxReason;

/*
typedef enum{
	TP_CHANNEL_UDS_PHY = 0,
	TP_CHANNEL_UDS_FUNC = 1,		
	TP_CHANNEL_AME = 2,
	TP_CHANNEL_MAX = 3
}tTpChanType;
*/


typedef enum{
	TP_TIMER_NONE = 0,
	TP_TIMER_N_AR = 1,//[Time for the transmission of a PDU on the receiver side],
	TP_TIMER_N_BR = 2,//[Time until the transmission of the next FC PDU]
	TP_TIMER_N_CR = 3,//[Time until the next CF PDU is received]

	TP_TIMER_N_AS = 4,//[Time for the transmission of a PDU on the sender side],
	TP_TIMER_N_BS = 5,//[Time until the next FC PDU is received]
	TP_TIMER_N_CS = 6//[Time until the transmission of the next CF PDU]
}tTpTimerType;


typedef struct{
	uint8 timerValue_N_AR;
	uint8 timerValue_N_BR;
	uint8 timerValue_N_CR;
	uint8 timerValue_N_AS;
	uint8 timerValue_N_BS;
	uint8 timerValue_N_CS;
}tTpTimerValues;


typedef enum{
	TP_FRAME_TYPE_NONE = 0x0,
	TP_FRAME_TYPE_SF = 0x1,
	TP_FRAME_TYPE_FF = 0x2,
	TP_FRAME_TYPE_CF = 0x4,
	TP_FRAME_TYPE_FC = 0x8

}tTpFrameType;

typedef enum{
	TP_REAL_FRAME_TYPE_NONE = 0x80,
	TP_REAL_FRAME_TYPE_SF = 0,
	TP_REAL_FRAME_TYPE_FF = 0x10,
	TP_REAL_FRAME_TYPE_CF = 0x20,
	TP_REAL_FRAME_TYPE_FC = 0x30,

}tTpRealFrameType;


typedef enum{
	TP_ACTION_IDLE,
	TP_ACTION_SEND_SF,
	TP_ACTION_SEND_FF,
	TP_ACTION_SEND_CF,
	TP_ACTION_SEND_FC,
	TP_ACTION_WAIT_TXCFM,
}tTpActionType;


typedef enum{
	TP_FS_CTS=0,//ContinueToSend
	TP_FS_WT = 1,//Wait
	TP_FS_OVFLW = 2,//OverFlow
	TP_FS_RES = 3 // reserved
}tTpFlowStatus;

//tp channel information
typedef struct _tpChInfor{
	boolean tpRxNewFrameFlag;
	tTpFrameType tpRxCurFrameType;
	tTpFrameType tpRxAllowedFrameType;
	tTpActionType tpPreAction;
	tTpActionType tpNextAction;
	boolean tpWaitTxCfmFlag;
	sint16 tpTimerValue;
	uint32 tpTimerPreTick;
	tTpTimerType tpTimerType;
	boolean tpTimeoutFlag;
	uint16 tpRxFFDL;//data len in first frame
	uint8 tpRxCurCFSeqNum;
	uint8 tpRxExpectedCFSeqNum;
	uint16 tpRxCFCounter;
	uint16 tpRxCFSum;
	uint8 tpRxBSCounter;
	uint8 tpRxFcBs;	// block size
	uint8 tpRxFcSTmin; //STmin
	
	boolean tpTxNewFrameFlag;
	uint8 tpTxBS;
	uint8 tpTxCFCounter;
	uint16 tpTxSTmin;
	uint16 tpTxSTminCounter;
	uint8 tpTxSeq;

}tTpChInfor;

typedef struct{
	boolean activeFlag;
	boolean timeoutFlag;
	uint8 timer;
	uint32 preTick;
}tTpTimerSTmin;

typedef struct{
	uint16 dataLen;
	uint8* dataHeader;
	uint8* dataPtr;
	uint16 restDataLen;
}tTpTxDataInfor;

typedef uint8 (*TBuffEle)[TP_RX_TMP_BUF_LEN];
typedef struct
{
	//QElemType *base;
	TBuffEle base;
	uint8 front;
	uint8 rear;
}SqQueue;


typedef struct{
	tpTraceLevel traceLevel;
	uint16 tpEleBuffLen;
	uint8 tpRxQueueNum;
	uint16 pduIdForUdsPhyRxAddr;
	uint16 pduIdForUdsPhyTxAddr;
	uint16 pduIdForUdsFuncRxAddr;
	uint16 pduIdForAMERxAddr;
	uint16 pduIdForAMETxAddr;
	TBuffEle buffBasePtr;
	uint8 tpPaddingData;
	tTpTimerValues *timerValuesCfg;
	uint8 maxCfSeq;
	uint8 blockSize;
	uint8 stMin;
	void(*udsInitCbk)();
	void(*udsLoopCbk)();
	void(*ameInitCbk)();
	void(*ameLoopCbk)();
}tTpCfgInfor;


#endif
