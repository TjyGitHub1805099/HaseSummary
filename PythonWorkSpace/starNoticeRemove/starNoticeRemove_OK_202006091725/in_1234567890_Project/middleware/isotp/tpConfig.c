#include"tpTypes.h"
#include"tpConfig.h"
#include"System_Tick.h"
#include"CanIf_Cfg.h"
#include"ComStack_Types.h" 
#include"CircularQueue.h"
#include"tp_if.h"
#include"sid0x99_if.h"
#include"CircleList.h"

#ifdef TP_USE_UDS
#include"uds_if.h"
#endif

#ifdef TP_USE_AME
#include"ame_if.h"
#endif


tTpPduIdGroupInfor tpPduIdGroupInforArray[TP_PDU_ID_GROUP_NUM_MAX] = {
	//CAUTION:Functional CH must be the 1th,this can't be changed
	{TP_DIAG_CH_TYPE_UDS,TP_PDU_ID_FUNC_RX,       0},//Functional addr has no tx addr
	{TP_DIAG_CH_TYPE_UDS,TP_IVI_PDU_ID_UDS_PHY_RX,TP_IVI_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_UDS,TP_IC_PDU_ID_UDS_PHY_RX, TP_IC_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_UDS,TP_AVM_PDU_ID_UDS_PHY_RX,TP_AVM_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_UDS,TP_HUD_PDU_ID_UDS_PHY_RX,TP_HUD_PDU_ID_UDS_PHY_TX},
	{TP_DIAG_CH_TYPE_AME,TP_AME_PDU_ID_UDS_PHY_RX,TP_AME_PDU_ID_UDS_PHY_TX},

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

uint8 tpRxUDSPhyBufferAVM[TP_BUF_LEN];
uint8 tpTxUDSPhyBufferAVM[TP_BUF_LEN];

uint8 tpRxUDSPhyBufferHUD[TP_BUF_LEN];
uint8 tpTxUDSPhyBufferHUD[TP_BUF_LEN];

const uint8 *tpRxBuffer[TP_PDU_ID_GROUP_NUM_MAX]={
		tpRxUDSFuncBuffer,
		tpRxUDSPhyBufferIVI,
		tpRxUDSPhyBufferIC,
		tpRxUDSPhyBufferAVM,
		tpRxUDSPhyBufferHUD,
#ifdef TP_USE_AME
		tpRxAMEBuffer,
#else
		NULL,
#endif


};


#define RX_CIRCLE_LIST_ITEM_NUM_FUNC_ADDR 4
#define RX_CIRCLE_LIST_ITEM_NUM_IVI 4
#define RX_CIRCLE_LIST_ITEM_NUM_IC 4
#define RX_CIRCLE_LIST_ITEM_NUM_AVM 4
#define RX_CIRCLE_LIST_ITEM_NUM_HUD 4
#define RX_CIRCLE_LIST_ITEM_NUM_AME 3


tCircleListItem gTpRxCircleListItem_funAddr[RX_CIRCLE_LIST_ITEM_NUM_FUNC_ADDR];
tCircleListItem gTpRxCircleListItem_ivi[RX_CIRCLE_LIST_ITEM_NUM_IVI];
tCircleListItem gTpRxCircleListItem_ic[RX_CIRCLE_LIST_ITEM_NUM_IC];
tCircleListItem gTpRxCircleListItem_avm[RX_CIRCLE_LIST_ITEM_NUM_AVM];
tCircleListItem gTpRxCircleListItem_hud[RX_CIRCLE_LIST_ITEM_NUM_HUD];
tCircleListItem gTpRxCircleListItem_ame[RX_CIRCLE_LIST_ITEM_NUM_AME];


tCircleListInfor gTpRxCirlceListInfor[TP_PDU_ID_GROUP_NUM_MAX] = {
	{NULL,NULL,gTpRxCircleListItem_funAddr,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_FUNC_ADDR},
	{NULL,NULL,gTpRxCircleListItem_ivi,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_IVI},
	{NULL,NULL,gTpRxCircleListItem_ic,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_IC},
	{NULL,NULL,gTpRxCircleListItem_avm,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_AVM},
	{NULL,NULL,gTpRxCircleListItem_hud,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_HUD},
#ifdef TP_USE_AME
	{NULL,NULL,gTpRxCircleListItem_ame,TP_BUF_LEN,RX_CIRCLE_LIST_ITEM_NUM_AME},
#endif
	};


const uint8 *tpTxBuffer[TP_PDU_ID_GROUP_NUM_MAX]={
		tpTxUDSFuncBuffer,
		tpTxUDSPhyBufferIVI,
		tpTxUDSPhyBufferIC,
		tpTxUDSPhyBufferAVM,
		tpTxUDSPhyBufferHUD,
	#ifdef TP_USE_AME
		tpTxAMEBuffer,
	#else
		NULL,
	#endif

};


const tSessionLayerWrapper gUpperLayerWrapper[] = {
	{udsRxIndication,udsRxFirstFrameIndication,udsCancelRxData,udsTxDataConfirmation},
	#ifdef TP_USE_AME
	{ameRxIndication,(tFnRxFirstFrameIndication)ameRxFirstFrameIndication,ameCancelRxData,ameTxDataConfirmation}
	#else
	{NULL,NULL,NULL,NULL}
	#endif
};

//uint8 gTpBuffElem[TP_PDU_ID_GROUP_NUM_MAX][TP_RX_BUFFER_NUM][TP_BUF_LEN] __attribute__((section(".diagSectionData")));

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
	.traceLevel = TP_TRACE_LEVEL_4,
	.tpEleBuffLen = TP_BUF_LEN,
	.tpRxQueueNum = TP_RX_BUFFER_NUM,
	.maxCfSeq = TP_MAX_CF_SEQ,
	.tpPaddingData = TP_BUFF_PADDING_DATA,
	.pduIdGroupNum = TP_PDU_ID_GROUP_NUM_MAX,
	.blockSize = TP_RX_BS,
	.stMin = TP_RX_ST_MIN,
	.buffBasePtr = NULL,//(TBuffEle)gTpBuffElem[0][0],
	.timerValuesCfg = (tTpTimerValues *)&gTpTimerValueCfg,
	.pduIdGroupInfor = (tTpPduIdGroupInfor *)&tpPduIdGroupInforArray,

	.udsInitCbk = udsInit,
	.udsLoopCbk = udsLoop,

	#ifdef TP_USE_AME
		.ameInitCbk = NULL,
		.ameLoopCbk = NULL,
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
	//PduInfoType PduInfoPtr;
	//boolean errFlag = FALSE;
	//uint32 txByCanFailCounter = 0;

	if(len == 0 || NULL == dataPtr || gTpTraceByCanFlag== FALSE || sid0x99_GetEnableTpTraceByCanFlag() == FALSE)
		return;
	txDataLen = (uint8)len;
	if(CQueue_Push(CQUEUE_TRACE_LEN_BY_CAN,&txDataLen,1,STD_OFF) == CQUEUE_ERR_PUSHPOP)
	{
		gTxTraceLenByCanFailCounter++;
		//errFlag = TRUE;
	}
	
	if(CQueue_Push(CQUEUE_TRACE_DATA_BY_CAN,dataPtr,len,STD_OFF) == CQUEUE_ERR_PUSHPOP)
	{
		gTxTraceDataByCanFailCounter++;
		//errFlag = TRUE;
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


