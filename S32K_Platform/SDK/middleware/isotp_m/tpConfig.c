#include"tpConfig.h"
#include"tpTypes.h"
#include"system_tick.h"
#include"CanIf_Cfg.h"
#include"ComStack_types.h"
#include"circularQueue.h"
#include"tp_if.h"

#ifdef TP_USE_UDS
#include"uds_if.h"
#endif

#ifdef TP_USE_AME
#include"ame_if.h"
#endif

typedef enum{
	TP_DIAG_DEV_ID_FUNC,
	TP_DIAG_DEV_ID_IVI,
	TP_DIAG_DEV_ID_IC,
	TP_DIAG_DEV_ID_AME,
	TP_DIAG_DEV_ID_MAX,
}tTpDiagDevId;


#define TP_PDU_ID_GROUP_NUM_MAX TP_DIAG_DEV_ID_MAX	//the first is for UDS functional ch,the last one be AME

#define TP_AME_PDU_ID_RX 70 //get this value from canIf module
#define TP_AME_PDU_ID_TX 60 //get this value from canIf module

#define TP_PDU_ID_FUNC_RX           CANIF_RX_PDU_CFG_CBK_ID_0

#define TP_IVI_PDU_ID_UDS_PHY_RX    CANIF_RX_PDU_CFG_CBK_ID_2//get this value from canIf module
#define TP_IVI_PDU_ID_UDS_FUNC_RX   TP_PDU_ID_FUNC_RX//get this value from canIf module
#define TP_IVI_PDU_ID_UDS_PHY_TX    CANIF_TX_PDU_CFG_CBK_ID_2 //get this value from canIf module

#define TP_IC_PDU_ID_UDS_PHY_RX     CANIF_RX_PDU_CFG_CBK_ID_1//get this value from canIf module
#define TP_IC_PDU_ID_UDS_FUNC_RX    TP_PDU_ID_FUNC_RX//get this value from canIf module
#define TP_IC_PDU_ID_UDS_PHY_TX     CANIF_TX_PDU_CFG_CBK_ID_1 //get this value from canIf module


tTpPduIdGroupInfor tpPduIdGroupInforArray[TP_PDU_ID_GROUP_NUM_MAX] = {
	//CAUTION:Functional CH must be the 1th,this can't be changed
	{TP_DIAG_CH_TYPE_UDS,TP_PDU_ID_FUNC_RX,       0},//Functional addr has no tx addr
	{TP_DIAG_CH_TYPE_UDS,TP_IVI_PDU_ID_UDS_PHY_RX,TP_IVI_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_UDS,TP_IC_PDU_ID_UDS_PHY_RX, TP_IC_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_AME,TP_AME_PDU_ID_RX,        TP_AME_PDU_ID_TX},	
};


SqQueue tpSQueue[TP_PDU_ID_GROUP_NUM_MAX];

uint8 *gUpperRxBufHeader[TP_PDU_ID_GROUP_NUM_MAX];
uint8 *gUpperRxBufPtr[TP_PDU_ID_GROUP_NUM_MAX];
tTpTxDataInfor gUpperTxDataInfor[TP_PDU_ID_GROUP_NUM_MAX];
tTpTimerSTmin gTimerSTmin[TP_PDU_ID_GROUP_NUM_MAX];


#ifdef TP_USE_AME

uint8 tpRxAMEBuffer[TP_BUF_LEN];
uint8 tpTxAMEBuffer[TP_BUF_LEN];
#endif

uint8 tpRxUDSFuncBuffer[TP_BUF_LEN];
uint8 tpTxUDSFuncBuffer[TP_BUF_LEN];

uint8 tpRxUDSPhyBufferIVI[TP_BUF_LEN];
uint8 tpTxUDSPhyBufferIVI[TP_BUF_LEN];

uint8 tpRxUDSPhyBufferIC[TP_BUF_LEN];
uint8 tpTxUDSPhyBufferIC[TP_BUF_LEN];


const uint8 *tpRxBuffer[TP_PDU_ID_GROUP_NUM_MAX]={
		tpRxUDSFuncBuffer,
		tpRxUDSPhyBufferIVI,
		tpRxUDSPhyBufferIC,
#ifdef TP_USE_AME
		tpRxAMEBuffer,
#else
		NULL,
#endif

};


const uint8 *tpTxBuffer[TP_PDU_ID_GROUP_NUM_MAX]={
		tpTxUDSFuncBuffer,
		tpTxUDSPhyBufferIVI,
		tpTxUDSPhyBufferIC,
	#ifdef TP_USE_AME
		tpTxAMEBuffer,
	#else
		NULL,
	#endif
};


const tSessionLayerWrapper gUpperLayerWrapper[] = {
	{udsRxIndication,udsRxFirstFrameIndication,udsCancelRxData,udsTxDataConfirmation},
	#ifdef TP_USE_AME
	{AmeRxIndication,AmeRxFirstFrameIndication,AmeCancelRxData,AmeTxDataConfirmation}
	#else
	{NULL,NULL,NULL,NULL}
	#endif
};

uint8 gTpBuffElem[TP_PDU_ID_GROUP_NUM_MAX][TP_RX_BUFFER_NUM][TP_BUF_LEN] __attribute__((section(".diagSectionData")));

tTpChInfor gTpChInfor[TP_PDU_ID_GROUP_NUM_MAX];
uint8 gTxcfmCh[TP_PDU_ID_GROUP_NUM_MAX];

const tTpTimerValues gTpTimerValueCfg ={
	.timerValue_N_AR = TP_TIMER_VALUE_N_AR,
	.timerValue_N_BR = TP_TIMER_VALUE_N_BR,
	.timerValue_N_CR = TP_TIMER_VALUE_N_CR,
	.timerValue_N_AS = TP_TIMER_VALUE_N_AS,
	.timerValue_N_BS = TP_TIMER_VALUE_N_BS,
	.timerValue_N_CS = TP_TIMER_VALUE_N_CS
	};


const tTpCfgInfor gTpCfgInfor = {
	.traceLevel = TP_TRACE_LEVEL_0,
	.tpEleBuffLen = TP_BUF_LEN,
	.tpRxQueueNum = TP_RX_BUFFER_NUM,
	.maxCfSeq = TP_MAX_CF_SEQ,
	.tpPaddingData = TP_BUFF_PADDING_DATA,
	.pduIdGroupNum = TP_PDU_ID_GROUP_NUM_MAX,
	.blockSize = TP_BS,
	.stMin = TP_ST_MIN,
	.buffBasePtr = (TBuffEle)gTpBuffElem[0][0],
	.timerValuesCfg = &gTpTimerValueCfg,
	.pduIdGroupInfor = &tpPduIdGroupInforArray,

	.udsInitCbk = udsInit,
	.udsLoopCbk = udsLoop,

	#ifdef TP_USE_AEM
		.ameInitCbk = ameInit,
		.ameLoopCbk = ameLoop,
	#else
		.ameInitCbk = NULL,
		.ameLoopCbk = NULL
	#endif
};


uint32 Tp_GetCurTick()
{
	return System_GetTickValue();
}

uint8 CQUEUE_TRACE_LEN_BY_CAN = 0xFF;
uint8 CQUEUE_TRACE_DATA_BY_CAN = 0xFF;
uint16 gTxTraceDataByCanFailCounter = 0;
uint16 gTxTraceLenByCanFailCounter = 0;

uint8 gTraceLenTxByCanQueueBuffer[256] = {1};
uint8 gTraceDataTxByCanQueueBuffer[4096] = {1};
uint8 gTpTxTraceDataByCanBuffer[128] = {1};
boolean gTpTraceByCanFlag = TRUE;

void Tp_Trace_Init()
{
	CQUEUE_TRACE_LEN_BY_CAN = CQueue_Init(CQUEUE_BUFTYPE_UINT8,gTraceLenTxByCanQueueBuffer,256);
	CQUEUE_TRACE_DATA_BY_CAN = CQueue_Init(CQUEUE_BUFTYPE_UINT8,gTraceDataTxByCanQueueBuffer,4096);
}

void Tp_SetTraceFlag(boolean flag)
{
	static boolean firstFlag = TRUE;

	if(firstFlag == FALSE)
		gTpTraceByCanFlag = flag;
	
	if(firstFlag == TRUE)
		firstFlag = FALSE;
	
}

void Tp_Trace_Push(uint16 len, uint8* dataPtr)
{
	uint8 txDataLen = 0;
	PduInfoType PduInfoPtr;
	boolean errFlag = FALSE;
	uint32 txByCanFailCounter = 0;

	if(len == 0 || NULL == dataPtr || gTpTraceByCanFlag== FALSE || sid0x99_GetEnableTpTraceByCanFlag() == FALSE)
		return;
	txDataLen = (uint8)len;
	if(CQueue_Push(CQUEUE_TRACE_LEN_BY_CAN,&txDataLen,1,STD_OFF) == CQUEUE_ERR_PUSHPOP)
	{
		gTxTraceLenByCanFailCounter++;
		errFlag = TRUE;
	}
	
	if(CQueue_Push(CQUEUE_TRACE_DATA_BY_CAN,dataPtr,len,STD_OFF) == CQUEUE_ERR_PUSHPOP)
	{
		gTxTraceDataByCanFailCounter++;
		errFlag = TRUE;
	}
	/*
	if(TRUE == errFlag)
	{
		txByCanFailCounter = (uint32)(gTxTraceLenByCanFailCounter<<16 | gTxTraceDataByCanFailCounter);
		PduInfoPtr.SduDataPtr = (uint8*)&txByCanFailCounter;
		PduInfoPtr.SduLength = 4;
		CanIf_Transmit(CANIF_TX_PDU_CFG_CBK_ID_1, &PduInfoPtr);
	}
	*/

}

void Tp_Trace_Pop()
{
	uint8 txTraceLenByCan = 0;
	uint16 dataLen = 0;

	if(gTpTraceByCanFlag== FALSE || sid0x99_GetEnableTpTraceByCanFlag() == FALSE)
	{
		if(CQueue_GetLen(CQUEUE_TRACE_LEN_BY_CAN,&dataLen,STD_OFF)==E_OK)
		{
			if(dataLen >0)
			{
				CQueue_Clear(CQUEUE_TRACE_LEN_BY_CAN,STD_OFF);
				CQueue_Clear(CQUEUE_TRACE_DATA_BY_CAN,STD_OFF);
			}
		}
		return;
	}

	if(CQueue_Pop(CQUEUE_TRACE_LEN_BY_CAN,&txTraceLenByCan,1,STD_OFF)!=CQUEUE_ERR_PUSHPOP)
	{
		if(txTraceLenByCan>0)
		{
			if(CQueue_Pop(CQUEUE_TRACE_DATA_BY_CAN,gTpTxTraceDataByCanBuffer,txTraceLenByCan,STD_OFF)!=CQUEUE_ERR_PUSHPOP)
			{
				Tp_TxDataRequest(1,gTpTxTraceDataByCanBuffer,txTraceLenByCan);
			}
		}
	}
}


