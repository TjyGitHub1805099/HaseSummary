#include"tpConfig.h"
#include"tpTypes.h"
#include"system_tick.h"

#ifdef TP_USE_UDS
#include"uds_if.h"
#endif

#ifdef TP_USE_AME
#include"ame_if.h"
#endif

#ifdef TP_USE_AME
uint8 tpRxAMEBuffer[TP_BUF_LEN];
uint8 tpTxAMEBuffer[TP_BUF_LEN];
#endif

#ifdef TP_USE_UDS
uint8 tpRxUDSPhyBuffer[TP_BUF_LEN];
uint8 tpTxUDSPhyBuffer[TP_BUF_LEN];

uint8 tpRxUDSFuncBuffer[TP_BUF_LEN];
uint8 tpTxUDSFuncBuffer[TP_BUF_LEN];
#endif

//uint16 tpPduIdRx[TP_CHANNEL_MAX] = {PDU_ID_UDS_PHY_RX,PDU_ID_UDS_FUNC_RX,PDU_ID_AME_RX};
const uint16 tpPduIdTx[TP_CHANNEL_MAX] = {PDU_ID_UDS_PHY_TX,PDU_ID_UDS_FUNC_TX,PDU_ID_AME_TX};

const uint8 *tpRxBuffer[TP_CHANNEL_MAX]={
	#ifdef TP_USE_UDS
		tpRxUDSPhyBuffer,tpRxUDSFuncBuffer,
	#else
		NULL,NULL,
	#endif

	#ifdef TP_USE_AME
		tpRxAMEBuffer,
	#else
		NULL,
	#endif
	};


const uint8 *tpTxBuffer[TP_CHANNEL_MAX]={
	#ifdef TP_USE_UDS
		tpTxUDSPhyBuffer,tpTxUDSFuncBuffer,
	#else
		NULL,NULL,
	#endif
		
	#ifdef TP_USE_AME
		tpTxAMEBuffer,
	#else
		NULL,
	#endif
};


const tSessionLayerWrapper gUpperLayerWrapper[TP_CHANNEL_MAX] = {
	#ifdef TP_USE_UDS
	{PDU_ID_UDS_PHY_RX,PDU_ID_UDS_PHY_TX,udsRxIndication,udsRxFirstFrameIndication,udsCancelRxData,udsTxDataConfirmation},
	{PDU_ID_UDS_FUNC_RX,PDU_ID_UDS_FUNC_TX,udsRxIndication,udsRxFirstFrameIndication,udsCancelRxData,udsTxDataConfirmation},
	#else
	{PDU_ID_AME_RX,PDU_ID_AME_TX,NULL,NULL,NULL,NULL},
	{PDU_ID_AME_RX,PDU_ID_AME_TX,NULL,NULL,NULL,NULL},
	#endif
	#ifdef TP_USE_AME
	{PDU_ID_AME_RX,PDU_ID_AME_TX,AmeRxIndication,AmeRxFirstFrameIndication,AmeCancelRxData,AmeTxDataConfirmation}
	#else
	{PDU_ID_AME_RX,PDU_ID_AME_TX,NULL,NULL,NULL,NULL}
	#endif
};

uint8 gTpBuffElem[TP_CHANNEL_MAX][TP_RX_BUFFER_NUM][TP_BUF_LEN] __attribute__((section(".diagSectionData")));


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
	.pduIdForUdsPhyRxAddr = PDU_ID_UDS_PHY_RX,
	.pduIdForUdsPhyTxAddr = PDU_ID_UDS_PHY_TX,
	.pduIdForUdsFuncRxAddr = PDU_ID_UDS_FUNC_RX,
	.pduIdForAMERxAddr = PDU_ID_AME_RX,
	.pduIdForAMETxAddr = PDU_ID_AME_TX,
	.maxCfSeq = TP_MAX_CF_SEQ,
	.tpPaddingData = TP_BUFF_PADDING_DATA,
	.blockSize = TP_BS,
	.stMin = TP_ST_MIN,
	.buffBasePtr = (TBuffEle)gTpBuffElem[0][0],
	.timerValuesCfg = &gTpTimerValueCfg,
	#ifdef TP_USE_UDS
	.udsInitCbk = udsInit,
	.udsLoopCbk = udsLoop,
	#else
	.udsInitCbk = NULL,
	.udsLoopCbk = NULL,
	#endif
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

