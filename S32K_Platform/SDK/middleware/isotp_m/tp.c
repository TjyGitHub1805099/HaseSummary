/*********************************
*		TP MODULE
*
*	version: v1.0
*	author:  liqi@hangsheng.com.cn
*	date:	 2016/09/20
*
*
**********************************/

#include"tpTypes.h"
#include"tp_if.h"

//#include"CanIf.h"
#include "ComStack_types.h"

extern uint8 CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr);

static boolean Tp_CpyData(uint8*dst,const uint8*src, uint16 srcLen);
static boolean Tp_Memset(uint8 *dst,uint16 len, uint8 value);
static void Tp_InitSQueue(uint8 ch);
static void Tp_ClearSQueue(uint8 ch);
static uint8 Tp_QueueLength(uint8 ch);
static boolean Tp_InsertSQueue(uint8 ch,uint8* data, uint8 len);
static boolean Tp_PopSQueue(uint8 ch,uint8*buffer,uint8 len);
static boolean Tp_isSQueueEmpty(uint8 ch);

static boolean Tp_ParseFrame(uint8 ch,const uint8*data,uint16 len);
static boolean Tp_SetTimer(uint8 ch,tTpTimerType type);
static void Tp_CancelTimer(uint8 i);
static void Tp_ResetParas(uint8 ch);
static void Tp_TimerProcess(uint8 i);
static void Tp_RxProcess(uint8 i);
static void Tp_TxProcess(uint8 i);
static boolean Tp_ProcessFirstFrame(uint8 i);
static void Tp_ProcessConsecutiveFrame(uint8 i);
static boolean Tp_ProcessFlowControlFrame(uint8 i);
static void Tp_DisableSTminTimer(uint8 ch);
static void Tp_EnableSTminTimer(uint8 ch);


#define RX_NEW_FRAME_FLAG(i)	gTpChInfor[i].tpRxNewFrameFlag
#define RX_CUR_FRAME_TYPE(i)	gTpChInfor[i].tpRxCurFrameType
#define RX_ALLOWED_FRAME_TYPE(i) gTpChInfor[i].tpRxAllowedFrameType
#define PRE_ACTION(i) 			gTpChInfor[i].tpPreAction
#define NEXT_ACTION(i)			gTpChInfor[i].tpNextAction
#define WAIT_TX_CFM(i)			gTpChInfor[i].tpWaitTxCfmFlag
#define TP_TIMER(i)				gTpChInfor[i].tpTimerValue
#define TP_TIMER_PRE_TICK(i)	gTpChInfor[i].tpTimerPreTick
#define TP_TIMER_TYPE(i)		gTpChInfor[i].tpTimerType
#define TP_TIMEOUT_FLAG(i)		gTpChInfor[i].tpTimeoutFlag
#define RX_FF_DL(i)				gTpChInfor[i].tpRxFFDL
#define RX_CF_SEQ(i)			gTpChInfor[i].tpRxCurCFSeqNum
#define RX_EPT_CF_SEQ(i) 		gTpChInfor[i].tpRxExpectedCFSeqNum
#define RX_CF_COUNTER(i)		gTpChInfor[i].tpRxCFCounter
#define RX_CF_SUM(i)			gTpChInfor[i].tpRxCFSum
#define RX_BS_COUNTER(i)		gTpChInfor[i].tpRxBSCounter
#define RX_FC_BS(i)				gTpChInfor[i].tpRxFcBs
#define RX_FC_ST_MIN(i)			gTpChInfor[i].tpRxFcSTmin

#define TX_NEW_FRAME_FLAG(i)	gTpChInfor[i].tpTxNewFrameFlag
#define TX_BS(i)				gTpChInfor[i].tpTxBS
#define TX_CF_COUNTER(i)		gTpChInfor[i].tpTxCFCounter
#define TX_STMIN(i)				gTpChInfor[i].tpTxSTmin
#define TX_STMIN_COUNTER(i)		gTpChInfor[i].tpTxSTminCounter
#define TX_SEQ(i)				gTpChInfor[i].tpTxSeq


#define TP_TRACE_LEVEL (gTpCfgInfor.traceLevel)
#define TP_ELE_BUF_LEN (gTpCfgInfor.tpEleBuffLen)
#define TP_RX_QUEUE_NUM (gTpCfgInfor.tpRxQueueNum)
#define TP_PDU_GROUP_NUM (gTpCfgInfor.pduIdGroupNum)
#define TP_PDU_ID_FOR_RX_ADDR(index) (gTpCfgInfor.pduIdGroupInfor[index].pduIdForRxAddr)
#define TP_PDU_ID_FOR_TX_ADDR(index) (gTpCfgInfor.pduIdGroupInfor[index].pduIdForTxAddr)
#define TP_DIAG_CH_TYPE(index)	(gTpCfgInfor.pduIdGroupInfor[index].diagChType)
#define TP_RX_BUF_ARRAY_BASE_PTR ((TBuffEle)(gTpCfgInfor.buffBasePtr))
#define TP_WRAPPER_PADDING_DATA (gTpCfgInfor.tpPaddingData)
#define	TP_WRAPPER_TIMER_VALUE_N_AR (gTpCfgInfor.timerValuesCfg->timerValue_N_AR) //[Time for the transmission of a PDU on the receiver side],
#define TP_WRAPPER_TIMER_VALUE_N_BR (gTpCfgInfor.timerValuesCfg->timerValue_N_BR) //[Time until the transmission of the next FC PDU]
#define TP_WRAPPER_TIMER_VALUE_N_CR (gTpCfgInfor.timerValuesCfg->timerValue_N_CR) //[Time until the next CF PDU is received]
		
#define	TP_WRAPPER_TIMER_VALUE_N_AS (gTpCfgInfor.timerValuesCfg->timerValue_N_AS) // [Time for the transmission of a PDU on the sender side],
#define TP_WRAPPER_TIMER_VALUE_N_BS (gTpCfgInfor.timerValuesCfg->timerValue_N_BS) //[Time until the next FC PDU is received]
#define TP_WRAPPER_TIMER_VALUE_N_CS (gTpCfgInfor.timerValuesCfg->timerValue_N_CS) //[Time until the transmission of the next CF PDU]

#define TP_UDS_INIT_CBK gTpCfgInfor.udsInitCbk
#define TP_AME_INIT_CBK gTpCfgInfor.ameInitCbk
#define TP_UDS_LOOP_CBK gTpCfgInfor.udsLoopCbk
#define TP_AME_LOOP_CBK gTpCfgInfor.ameLoopCbk
#define TP_WRAPPER_MAX_CF_SEQ gTpCfgInfor.maxCfSeq
#define TP_WRAPPER_BS gTpCfgInfor.blockSize
#define TP_WRAPPER_ST_MIN gTpCfgInfor.stMin


extern SqQueue tpSQueue[];
extern const tTpCfgInfor gTpCfgInfor;
extern tSessionLayerWrapper gUpperLayerWrapper[];
extern uint8 *tpRxBuffer[];
extern uint8 *tpTxBuffer[];
extern uint8 *gUpperRxBufHeader[];
extern uint8 *gUpperRxBufPtr[];
extern tTpTxDataInfor gUpperTxDataInfor[];
extern tTpChInfor gTpChInfor[];
extern tTpTimerSTmin gTimerSTmin[];

static boolean gTpInitFinishedFlag = FALSE;
//static PduInfoType txCanData[TP_CHANNEL_MAX];
extern uint8 gTxcfmCh[];

typedef struct{
	uint8 len;
	uint8 data[8];
}tpBufferType;


tpBufferType gTpRxBuffer[4];


void Tp_InitSQueue(uint8 ch)
{
	tpSQueue[ch].base = TP_RX_BUF_ARRAY_BASE_PTR;
	tpSQueue[ch].front = 0;
	tpSQueue[ch].rear = 0;
}

void Tp_ClearSQueue(uint8 ch)
{
	tpSQueue[ch].front = 0;
	tpSQueue[ch].rear = 0;
}

uint8 Tp_QueueLength(uint8 ch)
{
	return (tpSQueue[ch].rear - tpSQueue[ch].front + TP_RX_QUEUE_NUM) % TP_RX_QUEUE_NUM;
}

boolean Tp_InsertSQueue(uint8 ch,uint8* data, uint8 len)
{
	if ((tpSQueue[ch].rear + 1) % TP_RX_QUEUE_NUM == tpSQueue[ch].front)
	{
		return FALSE;
	}
	Tp_Memset(tpSQueue[ch].base[tpSQueue[ch].rear],TP_BUF_LEN,0);
	Tp_CpyData(tpSQueue[ch].base[tpSQueue[ch].rear],data,len);
	tpSQueue[ch].rear = (tpSQueue[ch].rear + 1) % TP_RX_QUEUE_NUM;
	return TRUE;
}


boolean Tp_PopSQueue(uint8 ch,uint8*buffer,uint8 len)
{
	if (tpSQueue[ch].front == tpSQueue[ch].rear)
		return FALSE;
	if(buffer!=NULL && len==TP_BUF_LEN)
	{
		Tp_CpyData(buffer,tpSQueue[ch].base[tpSQueue[ch].front],TP_BUF_LEN);
		Tp_Memset(tpSQueue[ch].base[tpSQueue[ch].front],TP_BUF_LEN,0);
	}
	tpSQueue[ch].front = (tpSQueue[ch].front + 1) % TP_RX_QUEUE_NUM;
	return TRUE;
}


boolean Tp_isSQueueEmpty(uint8 ch)
{
	if (tpSQueue[ch].front == tpSQueue[ch].rear)
		return TRUE;
	else
		return FALSE;
}


//chang to big endian or little endian byte order
boolean Tp_SwapByteOrder(uint8 *src, uint16 len)
{
    uint16 i;
    uint8 c;

	if(!src || len<2)
		return FALSE;
	
    for(i=0; i<len/2; i++)
    {
        c = src[i];
        src[i] = src[len-1-i];
        src[len-1-i] = c;
    }

	return TRUE;
}

boolean Tp_Memset(uint8 *dst,uint16 len, uint8 value)
{
	uint16 i=0;
	
	if(dst== NULL || len<=0 || len >4096)// set the len limitation to 4096 bytes for security
		return FALSE;

	while(i<len)
	{
		*dst++ = value;
		i++;
	}

	return TRUE;
}


boolean Tp_CpyData(uint8*dst,const uint8*src, uint16 srcLen)
{
	uint8 i = 0;
	
	if(!(dst && src) || (dst == src))
	{
		return FALSE;
	}
	
	while(i<srcLen)
	{
		*dst++ = *src++;
		i++;
	}
	
	return TRUE;
}


boolean Tp_ParseFrame(uint8 ch,const uint8*data,uint16 len)
{
	tTpRealFrameType pciType = TP_REAL_FRAME_TYPE_NONE;
	uint16 pduLen = 0;
	tTpFlowStatus fs = TP_FS_RES;
	
	if(data == NULL)
		return FALSE;
	if(len > TP_BUF_LEN || len <TP_VALID_MINI_DATA_LEN)
		return FALSE;

	pciType = data[0] & 0xF0;

	switch(pciType)
	{
	case TP_REAL_FRAME_TYPE_SF:
		pduLen = (uint16)(data[0] & 0x0F);
		if(pduLen >7)
		{
			TP_TRACE_TXT(TP_TRACE,TRACE_ERRO,"TP: Tp_ParseFrame():invalid pduLen in SF frame\n\r");
			return FALSE;
		}
		RX_CUR_FRAME_TYPE(ch) = TP_FRAME_TYPE_SF;
		break;
	case TP_REAL_FRAME_TYPE_FF:
		pduLen = (uint16)((data[0]&0x0f)<<8 | data[1]);
		if(pduLen <8)
		{
			TP_TRACE_TXT(TP_TRACE,TRACE_ERRO,"TP: Tp_ParseFrame():invalid pduLen in FF frame\n\r");
			return FALSE;
		}
		RX_CUR_FRAME_TYPE(ch) = TP_FRAME_TYPE_FF;
		break;
	case TP_REAL_FRAME_TYPE_CF:
		RX_CUR_FRAME_TYPE(ch) = TP_FRAME_TYPE_CF;
		RX_CF_SEQ(ch) = (uint8)(data[0] & 0xF);
		break;
	case TP_REAL_FRAME_TYPE_FC:
		fs = data[0] & 0xF;
		if(fs > TP_FS_OVFLW)
		{
			TP_TRACE_TXT(TP_TRACE,TRACE_ERRO,"TP: Tp_ParseFrame():invalid FS in FC frame\n\r");
			return FALSE;
		}

		RX_CUR_FRAME_TYPE(ch) = TP_FRAME_TYPE_FC;
		//RX_FC_BS(ch) = data[1];
		//RX_FC_ST_MIN(ch) = data[2];
		break;
	default:
		TP_TRACE_VALUE1(TP_TRACE,TRACE_ERRO,"TP: Tp_ParseFrame():invalid pci type[%d] in frame\n\r",pciType);
		return FALSE;
	}
	
	return TRUE;
}


void Tp_RxIndication(uint32 RxPduId, const void* data)
{
	uint16 dataLen = 0;
	uint8 localBuffer[TP_BUF_LEN];
	const PduInfoType* PduInfoPtr = NULL;
	uint8 i = 0;
	
	PduInfoPtr = (const PduInfoType*)data;
	if((PduInfoPtr == NULL) || (gTpInitFinishedFlag == FALSE))
		return;

	dataLen = (uint16)PduInfoPtr->SduLength;
	if(dataLen>TP_BUF_LEN || dataLen == 0)
	{
		return;
	}

	for(i=0;i<TP_PDU_GROUP_NUM;i++)
	{
		if(gTpCfgInfor.pduIdGroupInfor[i].pduIdForRxAddr == RxPduId)
		{	
			#if 0
			//Tp_CpyData(localBuffer,(uint8*)PduInfoPtr->SduDataPtr,dataLen); 
			Tp_InsertSQueue(i,(uint8*)PduInfoPtr->SduDataPtr,dataLen);
			#else
			gTpRxBuffer[i].len = dataLen;
			Tp_CpyData(gTpRxBuffer[i].data,(uint8*)PduInfoPtr->SduDataPtr,dataLen);
			#endif
			break;
		}
	}
	

}


boolean Tp_TxDataWrapper(uint8 ch,const uint8* data, uint8 len)
{
	PduInfoType	pduData;

	if(!data || len != TP_BUF_LEN)// TODO: CAUTION:CAN frame len must be 8 bytes,but for future extention, also keep this argument
		return FALSE;

	pduData.SduDataPtr = (SduDataPtrType)data;//after switch byte order
	pduData.SduLength = 8;

	WAIT_TX_CFM(ch) = TRUE;
	//TracePrintf(TP_TRACE,TRACE_INFO,"TP: TxDataWrapper\n\r");

	//requirement:if we got req from FUNC address,we resp only by Phy ADDRESS.
	
	if(CAN_TX_DATA(TP_PDU_ID_FOR_TX_ADDR(ch),&pduData) != E_OK)
	{
		TP_TRACE_TXT(TP_TRACE,TRACE_ERRO,"TP: Tp_TxDataWrapper(): CAN is busy,fail to send data\n\r");
		return FALSE;
	}

	return TRUE;
}


//in upperlayer, the data pointer points to sUDSTxBuffer or sAMETxBuffer
boolean Tp_TxDataRequest(uint32 ch, uint8*data, uint16 len)
{
	boolean ret = TRUE;
	
	if(ch >= TP_PDU_GROUP_NUM || data==NULL || len == 0)
		ret = FALSE;
	else if(TP_PDU_ID_FOR_TX_ADDR(ch) != 0) // can't tx data by uds functional ch
	{
		if(TRUE == TX_NEW_FRAME_FLAG(ch))//Tx process is busy now
			ret = FALSE;
		else
		{
			RX_ALLOWED_FRAME_TYPE(ch) = TP_FRAME_TYPE_NONE;//disable RX any frame
		
			gUpperTxDataInfor[ch].dataHeader = data;
			gUpperTxDataInfor[ch].dataPtr = data;
			gUpperTxDataInfor[ch].dataLen = len;
			gUpperTxDataInfor[ch].restDataLen = len; 

			//gUpperTxBufPtr[ch] = data;
			if(len >7)
				NEXT_ACTION(ch) = TP_ACTION_SEND_FF;
			else
				NEXT_ACTION(ch) = TP_ACTION_SEND_SF;

			
			TX_NEW_FRAME_FLAG(ch) = TRUE;

			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_3)
			{
				TP_TRACE_VALUE3(TP_TRACE,TRACE_INFO,"TP:txDataReq: ch:%d,data[0]=0x%x,len=%d\n\r",ch,data[0],len);
			}
			
		}
	}
	else
		ret = FALSE;
		
	return ret;
}


void Tp_TxConfirmation(uint32 TxPduId)
{
	uint8 i = 0;
	
	if(gTpInitFinishedFlag == FALSE)
		return;

	for(i=0;i<TP_PDU_GROUP_NUM;i++)
	{
		if(TxPduId == TP_PDU_ID_FOR_TX_ADDR(i))
		{
			gTxcfmCh[i] = i;
			break;
		}
	}

}

void Tp_CheckTxcfm(uint8 i)
{
	uint8 ch = 0;
	
	if(gTxcfmCh[i]!= TP_PDU_GROUP_NUM)
	{
		if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
		{
			TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:txCfm\n\r");
		}

		ch = gTxcfmCh[i];
		WAIT_TX_CFM(ch) = FALSE;
		
		switch(PRE_ACTION(ch))
		{
		case TP_ACTION_SEND_FC:
			if(RX_FF_DL(ch) > 0) //buffer_ok
				Tp_SetTimer(ch,TP_TIMER_N_CR);//waitting for next CF frame
			else// buffer_overflow or buffer_wait
				Tp_ResetParas(ch);
			break;
		case TP_ACTION_SEND_FF:
			RX_ALLOWED_FRAME_TYPE(ch) = TP_FRAME_TYPE_FC;
			Tp_SetTimer(ch,TP_TIMER_N_BS);//waitting for FC frame
			break;
		case TP_ACTION_SEND_CF:
			if(NEXT_ACTION(ch) == TP_ACTION_IDLE)//all CFs had been sent out
			{
				if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
				{
					TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:tx done, all CFs\n\r");
				}
				
				if(NULL != gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation(ch,TRUE,TP_ACTION_SEND_CF);
				Tp_ResetParas(ch);
			}
			else if(TX_BS(ch) == TX_CF_COUNTER(ch))
			{
				if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
				{
					TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:Wait next FC,bs=0x%x\n\r",TX_BS(ch));
				}
				TX_CF_COUNTER(ch) = 0;
				Tp_SetTimer(ch,TP_TIMER_N_BS);//waitting for FC frame
				RX_ALLOWED_FRAME_TYPE(ch) = TP_FRAME_TYPE_FC;
				Tp_DisableSTminTimer(ch);
			}
			else if(NEXT_ACTION(ch) == TP_ACTION_WAIT_TXCFM)
			{
				if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
				{
					TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:txCfm,will tx next CF\n\r");
				}
				NEXT_ACTION(ch) = TP_ACTION_SEND_CF;
				Tp_SetTimer(ch,TP_TIMER_NONE);
			}
			break;
		case TP_ACTION_SEND_SF:
			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:tx done, SF\n\r");
			}
			RX_ALLOWED_FRAME_TYPE(ch) = TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF;
			TX_NEW_FRAME_FLAG(ch) = FALSE;
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation(ch,TRUE,TP_ACTION_SEND_SF);
			Tp_ResetParas(ch);
			break;
		case TP_ACTION_IDLE:
			break;
		default:
			;//error
		}

		gTxcfmCh[i] = TP_PDU_GROUP_NUM;
	}
}


void Tp_EnableSTminTimer(uint8 ch)
{
	gTimerSTmin[ch].activeFlag = TRUE;
	gTimerSTmin[ch].timeoutFlag = FALSE;
	gTimerSTmin[ch].timer = TX_STMIN(ch);
	gTimerSTmin[ch].preTick = Tp_GetCurTick();
}

void Tp_DisableSTminTimer(uint8 ch)
{
	gTimerSTmin[ch].activeFlag = FALSE;
	gTimerSTmin[ch].timeoutFlag = FALSE;
	gTimerSTmin[ch].timer = 0;
	gTimerSTmin[ch].preTick = 0;
}

boolean Tp_SetTimer(uint8 ch,tTpTimerType type)
{
	if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_3)
	{
		TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:setTimer:%d\n\r",type);
	}
	
	switch(type)
	{
	case TP_TIMER_N_AR:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_AR;
		break;
	case TP_TIMER_N_BR:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_BR;
		break;
	case TP_TIMER_N_CR:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_CR;
		break;
	case TP_TIMER_N_AS:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_AS;
		break;
	case TP_TIMER_N_BS:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_BS;
		break;
	case TP_TIMER_N_CS:
		TP_TIMER(ch) = TP_WRAPPER_TIMER_VALUE_N_CS;
		break;
	case TP_TIMER_NONE:
		TP_TIMER(ch) = 0;
		break;
	default:
		TP_TIMER_TYPE(ch) = TP_TIMER_NONE;
		TP_TIMER(ch) = 0;
		//error
		return FALSE;
	}

	TP_TIMER_TYPE(ch) = type;
	TP_TIMEOUT_FLAG(ch) = FALSE;
	if(TP_TIMER_TYPE(ch)!=TP_TIMER_NONE)
		TP_TIMER_PRE_TICK(ch) = Tp_GetCurTick();
	return TRUE;
}

void Tp_CancelTimer(uint8 i)
{
	TP_TIMER(i) = 0;
	TP_TIMER_TYPE(i) = TP_TIMER_NONE;
}


void Tp_ResetParas(uint8 ch)
{

	if(gTpInitFinishedFlag == FALSE)
		Tp_InitSQueue(ch);
	gTpRxBuffer[ch].len = 0;
	Tp_SetTimer(ch,TP_TIMER_NONE);
	Tp_DisableSTminTimer(ch);

	RX_NEW_FRAME_FLAG(ch) = FALSE;
	RX_CUR_FRAME_TYPE(ch) = TP_FRAME_TYPE_NONE;

	if(TP_PDU_ID_FOR_TX_ADDR(ch) != 0)// not functional addr ch
		RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF;
	else
		RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF;

	PRE_ACTION(ch) = TP_ACTION_IDLE;
	NEXT_ACTION(ch) = TP_ACTION_IDLE;
	WAIT_TX_CFM(ch) = FALSE;
	//TracePrintf(TP_TRACE,TRACE_INFO,"TP:reset all para\r\n");
	RX_FF_DL(ch) = 0;
	RX_CF_SEQ(ch) = 0;
	RX_EPT_CF_SEQ(ch) = 1;
	RX_CF_COUNTER(ch) = 0;
	RX_CF_SUM(ch) = 0;
	RX_BS_COUNTER(ch) = 0;
	RX_FC_BS(ch) = 0;
	RX_FC_ST_MIN(ch) = 2;// TP_TASK_INTERVAL * 2 = 12 milliseconds
	
	TX_NEW_FRAME_FLAG(ch) = FALSE;
	TX_BS(ch) = 0;
	TX_CF_COUNTER(ch) = 0;
	TX_STMIN(ch) = 0;
	TX_STMIN_COUNTER(ch) = 0;
	TX_SEQ(ch) = 0;

	//txCanData[ch].SduDataPtr = (SduDataPtrType)tpTxBuffer[ch];
	//txCanData[ch].SduLength  = TP_BUF_LEN;
	gUpperRxBufPtr[ch] = NULL;
}

void Tp_Init()
{
	uint8 i = 0;
	
	if(gTpInitFinishedFlag == FALSE)
	{
		for(i=0;i<TP_PDU_GROUP_NUM;i++)
		{
			Tp_ResetParas(i);
			gTxcfmCh[i] = TP_PDU_GROUP_NUM;
		}
		gTpInitFinishedFlag = TRUE;
		
		if(NULL!=TP_UDS_INIT_CBK)
			TP_UDS_INIT_CBK();
		if(NULL!=TP_AME_INIT_CBK)
			TP_AME_INIT_CBK();
		
	}
}

void Tp_TimerProcess(uint8 ch)
{
		
	switch(TP_TIMER_TYPE(ch))
	{
	case TP_TIMER_N_AR://[Time for the transmission of a PDU on the receiver side],
		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData(ch,TP_TIMER_N_AR);
		break;
	case TP_TIMER_N_BR://[Time until the transmission of the next FC PDU]
		//2 TODO
		break;
	case TP_TIMER_N_CR://[Time until the next CF PDU is received]
		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData(ch,TP_TIMER_N_CR);
		break;
	case TP_TIMER_N_AS://[Time for the transmission of a PDU on the sender side],
		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation(ch,FALSE,TP_TIMER_N_AS);
		break;
	case TP_TIMER_N_BS://[Time until the next FC PDU is received]
		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnTxDataConfirmation(ch,FALSE,TP_TIMER_N_BS);
		break;
	case TP_TIMER_N_CS://[Time until the transmission of the next CF PDU]
		//2 TODO
		break;
	default:
		{
			TP_TRACE_VALUE1(TP_TRACE,TRACE_ERRO,"TP: Tp_TimerProcess(): invalid timer type[%d]\n\r",TP_TIMER_TYPE(ch));
		}
	}

	if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
	{
		TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:timeout:typ:%d\n\r",TP_TIMER_TYPE(ch));
	}

	Tp_ResetParas(ch);

}

boolean Tp_ProcessFirstFrame(uint8 ch)
{
	uint16 reqBufferLen = 0;
	tReqBufferResult ret = BUFFER_OK;
	uint16 blockNum = 0, rest = 0;
	uint8 buffer[TP_BUF_LEN];

	reqBufferLen = (uint16)((tpRxBuffer[ch][0]&0xF) << 8 | tpRxBuffer[ch][1]);

	if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
	{
		TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP: rx FF: DL=[%d],%d\n\r",reqBufferLen);
	}

	if(reqBufferLen < TP_BUF_LEN)
	{
		TP_TRACE_VALUE1(TP_TRACE,TRACE_WARNING,"TP: rx invalid FF: DL=[%d]\n\r",reqBufferLen);
		Tp_ResetParas(ch);
		return FALSE;
	}

	if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnRxFirstFrameIndication)
	{
		ret = gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnRxFirstFrameIndication(ch,reqBufferLen, &gUpperRxBufHeader[ch]);
		Tp_Memset(buffer, TP_BUF_LEN, TP_BUFF_PADDING_DATA);
		
		switch(ret)
		{
		case BUFFER_OK:
		
			//inform uppper layer to cancel current Rx
			if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData)
				gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData(ch,REASON_FF_FRAME_ARRIVED);
			
			gUpperRxBufPtr[ch] = gUpperRxBufHeader[ch];
			Tp_CpyData(gUpperRxBufPtr[ch],&tpRxBuffer[ch][2],TP_FIRST_FRAME_DATA_LEN);
			gUpperRxBufPtr[ch] += TP_FIRST_FRAME_DATA_LEN;
		
			//send FC frame
			buffer[0] = TP_REAL_FRAME_TYPE_FC | TP_FS_CTS;
			buffer[1] = TP_WRAPPER_BS;
			buffer[2] = TP_WRAPPER_ST_MIN;
		
			//reset some tp statuses
			Tp_SetTimer(ch,TP_TIMER_N_AR);
			RX_FF_DL(ch) = reqBufferLen;
			PRE_ACTION(ch) = TP_ACTION_SEND_FC;
			if(TP_PDU_ID_FOR_TX_ADDR(ch) != 0)// not functional addr ch
				RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF| TP_FRAME_TYPE_CF;
			else
				RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF;
			RX_EPT_CF_SEQ(ch) = 1;
			RX_CF_COUNTER(ch) = 0;
			RX_BS_COUNTER(ch) = 0;
		
			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP: tx FC\n\r");
			}
		
			if(Tp_TxDataWrapper(ch,buffer,TP_BUF_LEN) == FALSE)
			{
				if(NULL!= gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData(ch,REASON_FAIL_TX_FC_FRAME);
				Tp_ResetParas(ch);
				return FALSE;
			}
		
			//caculate how many CFs should be received.
			//in first frame,there is 6 bytes datas, 
			//in following CF frames,there is at most 7 bytes datas in on CF frame
			blockNum = (RX_FF_DL(ch) - TP_FIRST_FRAME_DATA_LEN)/TP_CONSECUTIVE_FRAME_DATA_LEN;
			rest = (RX_FF_DL(ch) - TP_FIRST_FRAME_DATA_LEN)%TP_CONSECUTIVE_FRAME_DATA_LEN;
			if( 0 == rest)
				RX_CF_SUM(ch) = blockNum;
			else
				RX_CF_SUM(ch) = blockNum + 1;
		
			//TracePrintf(TP_TRACE,TRACE_ERRO,"TP: Tp_ProcessFirstFrame(): RX_CF_SUM=[%d]\n\r",RX_CF_SUM(i));
		
			break;
		case BUFFER_OVERFLOW:
		case BUFFER_WAIT:
			Tp_SetTimer(ch,TP_TIMER_N_AR);
			PRE_ACTION(ch) = TP_ACTION_SEND_FC;
		
			if(TP_PDU_ID_FOR_TX_ADDR(ch) != 0)// not functional addr ch
				RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF| TP_FRAME_TYPE_CF;
			else
				RX_ALLOWED_FRAME_TYPE(ch) =	TP_FRAME_TYPE_SF;
			
			RX_FF_DL(ch) = 0;
			
			if(ret==BUFFER_OVERFLOW)
				buffer[0] = TP_REAL_FRAME_TYPE_FC | TP_FS_OVFLW;
			else
				buffer[0] = TP_REAL_FRAME_TYPE_FC | TP_FS_WT;
			if(Tp_TxDataWrapper(ch,buffer,TP_BUF_LEN) == FALSE)
			{
				if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(ch)].FnCancelRxData(ch,REASON_FAIL_TX_FC_FRAME);
				Tp_ResetParas(ch);
				return FALSE;
			}
			break;
		default:
			//error
			Tp_ResetParas(ch);
			return FALSE;
		}

	}



	return TRUE;
}

void Tp_ProcessConsecutiveFrame(uint8 i)
{
	uint8 buffer[TP_BUF_LEN];
	uint16 ff_len = 0;

	if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
	{		
		TP_TRACE_VALUE3(TP_TRACE,TRACE_INFO,"TP:rx CF:Seq:0x%x,0x%x,%d\n\r",RX_CF_SEQ(i),RX_EPT_CF_SEQ(i),RX_BS_COUNTER(i));
	}

	
	if(RX_EPT_CF_SEQ(i) == RX_CF_SEQ(i))
	{
		if(RX_EPT_CF_SEQ(i) == TP_WRAPPER_MAX_CF_SEQ)
			RX_EPT_CF_SEQ(i) = 0;
		else
			RX_EPT_CF_SEQ(i)++;
		RX_CF_COUNTER(i)++;
		RX_BS_COUNTER(i)++;
		Tp_CancelTimer(i);
	
		Tp_CpyData(gUpperRxBufPtr[i],&tpRxBuffer[i][1],TP_CONSECUTIVE_FRAME_DATA_LEN);
		gUpperRxBufPtr[i] += TP_CONSECUTIVE_FRAME_DATA_LEN;
			
		if(RX_CF_COUNTER(i) < RX_CF_SUM(i))//needs to rx more CFs
		{
			if(TP_WRAPPER_BS == 0 || RX_BS_COUNTER(i) < TP_WRAPPER_BS)//NO need to resp FC
			{
				Tp_SetTimer(i,TP_TIMER_N_CR);//trigger timer of waitting for next CF
			}
			else if(RX_BS_COUNTER(i) >= TP_WRAPPER_BS)//needs to resp FC to client
			{
				if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
				{
					TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP: Tp_PCF,resp FC\r\n");
				}

				//resp FC
				Tp_Memset(buffer,TP_BUF_LEN,TP_WRAPPER_PADDING_DATA);
				buffer[0] = TP_REAL_FRAME_TYPE_FC | TP_FS_CTS;
				buffer[1] = TP_WRAPPER_BS;
				buffer[2] = TP_WRAPPER_ST_MIN;

				Tp_SetTimer(i, TP_TIMER_N_AR);//wait for TxConfirmation() resp
				RX_BS_COUNTER(i) = 0;
				PRE_ACTION(i) = TP_ACTION_SEND_FC;

				if(Tp_TxDataWrapper(i,buffer,TP_BUF_LEN)== FALSE)
				{
					if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData)
						gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData(i,REASON_FAIL_TX_FC_FRAME);
					Tp_ResetParas(i);
					return;
				}
			}
				
		}
		else // all the CFs are received
		{
			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP: Got all CF\r\n");
			}
			ff_len = RX_FF_DL(i);
			Tp_ResetParas(i);
			if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication)
				gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication(i,gUpperRxBufHeader[i], ff_len);
		}
	}
	else
	{
		if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
		{
			TP_TRACE_VALUE2(TP_TRACE,TRACE_INFO,"TP:Err:SEQ is wrong,eptSeq:0x%x,curSeq:0x%x\r\n",RX_EPT_CF_SEQ(i),RX_CF_SEQ(i));
		}

		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData(i,REASON_FAIL_RX_CF_FRAME);
		Tp_ResetParas(i);
	}

}

boolean Tp_ProcessFlowControlFrame(uint8 i)
{
	uint8 flowStatus = tpRxBuffer[i][0] & 0x0f;
	uint8 blocksize = tpRxBuffer[i][1];
	uint8 STmin = (uint8)tpRxBuffer[i][2];
	//uint8 NSTmin = STmin/TP_TASK_INTERVAL;

	if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_1)
	{
		TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP: Rx FC\n\r");
	}
	
	switch(flowStatus)
	{
	case TP_FS_CTS:

		RX_FC_BS(i) = tpRxBuffer[i][1];
		RX_FC_ST_MIN(i) = tpRxBuffer[i][2];
		
		TX_BS(i) = blocksize;
		#if 0
		if(0 == NSTmin)
			TX_STMIN(i) = 1;
		else
		{
			if(0 == STmin%TP_TASK_INTERVAL)
				TX_STMIN(i) = NSTmin;
			else
				TX_STMIN(i) = NSTmin+TP_TASK_INTERVAL;
		}
		#else
			//TX_STMIN(i) = NSTmin+1;
			TX_STMIN(i) = STmin;
		#endif
		Tp_EnableSTminTimer(i);//trigger timer STmin for Tx next CF
		RX_ALLOWED_FRAME_TYPE(i) = TP_FRAME_TYPE_NONE;//disable RX any frame
		NEXT_ACTION(i) = TP_ACTION_SEND_CF;
		break;
	case TP_FS_WT:
		Tp_SetTimer(i,TP_TIMER_N_BS);
		break;
	case TP_FS_OVFLW:
		if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation)
			gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation(i,FALSE,REASON_FAIL_TX_CF_FRAME);
		Tp_ResetParas(i);
		break;
	default:
		//wrong FS, but we had check it in Tp_ParseFrame(),so we don't warry about this
		return FALSE;
	}

	return TRUE;
}

#if 0
void Tp_RxProcess(uint8 i)
{
	//tpList *tpDataNode = NULL;
	//tBuffElem *tpDataNode = NULL;
	boolean ret = FALSE;
	uint8 localBuffer[TP_BUF_LEN];


	if(WAIT_TX_CFM(i) == TRUE)
		return;

	Tp_Memset(localBuffer,TP_BUF_LEN,0);
	while(Tp_PopSQueue(i,localBuffer,TP_BUF_LEN)==TRUE)
	{
		ret = Tp_ParseFrame(i,localBuffer, TP_BUF_LEN);
		if(ret == TRUE)
		{
			Tp_Memset(tpRxBuffer[i],TP_BUF_LEN,0);

			if(Tp_CpyData(tpRxBuffer[i],localBuffer,TP_BUF_LEN) == TRUE)
			{

				if(RX_ALLOWED_FRAME_TYPE(i) & RX_CUR_FRAME_TYPE(i))
				{
					PRE_ACTION(i) = TP_ACTION_IDLE;
					switch(RX_CUR_FRAME_TYPE(i))
					{
					case TP_FRAME_TYPE_SF:
						if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData)
							gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData(i,REASON_SF_FRAME_ARRIVED);
						Tp_ResetParas(i);
						if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_3)
						{
							TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:hook: FnRxIndication,data[0]=0x%x\n\r",tpRxBuffer[i][1]);
						}
						if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication)
							gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication(i,&tpRxBuffer[i][1],tpRxBuffer[i][0]);
						
						break;
					case TP_FRAME_TYPE_FF:
						if(TP_PDU_ID_FOR_TX_ADDR(i) != 0) // functinal addressing (1-to-n communication) shall only be supported for single frame communication
           				{
              				Tp_ResetParas(i);
             				Tp_ProcessFirstFrame(i);
            			}
            			break;
					case TP_FRAME_TYPE_CF:
						Tp_ProcessConsecutiveFrame(i);
						break;
					case TP_FRAME_TYPE_FC:
						Tp_ProcessFlowControlFrame(i);
						break;
					default:
						//error
						;
					}
				}

				
			}
		}

	

		//RX_NEW_FRAME_FLAG(i) = FALSE;
	}
}
#else
void Tp_RxProcess(uint8 i)
{
	//tpList *tpDataNode = NULL;
	//tBuffElem *tpDataNode = NULL;
	boolean ret = FALSE;
	uint8 localBuffer[TP_BUF_LEN];


	if(WAIT_TX_CFM(i) == TRUE)
		return;

	Tp_Memset(localBuffer,TP_BUF_LEN,0);
	if(gTpRxBuffer[i].len>0 && gTpRxBuffer[i].len<=TP_BUF_LEN)
	{
		Tp_CpyData(localBuffer, gTpRxBuffer[i].data, gTpRxBuffer[i].len);
		gTpRxBuffer[i].len = 0;
		ret = Tp_ParseFrame(i,localBuffer, TP_BUF_LEN);
		if(ret == TRUE)
		{
			Tp_Memset(tpRxBuffer[i],TP_BUF_LEN,0);

			if(Tp_CpyData(tpRxBuffer[i],localBuffer,TP_BUF_LEN) == TRUE)
			{

				if(RX_ALLOWED_FRAME_TYPE(i) & RX_CUR_FRAME_TYPE(i))
				{
					PRE_ACTION(i) = TP_ACTION_IDLE;
					switch(RX_CUR_FRAME_TYPE(i))
					{
					case TP_FRAME_TYPE_SF:
						if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData)
							gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnCancelRxData(i,REASON_SF_FRAME_ARRIVED);
						Tp_ResetParas(i);
						if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_3)
						{
							TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:hook: FnRxIndication,data[0]=0x%x\n\r",tpRxBuffer[i][1]);
						}
						if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication)
							gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnRxIndication(i,&tpRxBuffer[i][1],tpRxBuffer[i][0]);
						
						break;
					case TP_FRAME_TYPE_FF:
						if(TP_PDU_ID_FOR_TX_ADDR(i) != 0) // functinal addressing (1-to-n communication) shall only be supported for single frame communication
           				{
              				Tp_ResetParas(i);
             				Tp_ProcessFirstFrame(i);
            			}
            			break;
					case TP_FRAME_TYPE_CF:
						Tp_ProcessConsecutiveFrame(i);
						break;
					case TP_FRAME_TYPE_FC:
						Tp_ProcessFlowControlFrame(i);
						break;
					default:
						//error
						;
					}
				}

				
			}
		}

	

		//RX_NEW_FRAME_FLAG(i) = FALSE;
	}
}

#endif


void Tp_TxProcess(uint8 i)
{
	uint8 buffer[TP_BUF_LEN];

	Tp_Memset(buffer,TP_BUF_LEN,TP_WRAPPER_PADDING_DATA);
	//check if need to send CF frame
	if(NEXT_ACTION(i) == TP_ACTION_SEND_CF)
	{
		//check if it's time to send next CF now
		if(gTimerSTmin[i].timeoutFlag == TRUE)//yes
		{

			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP:Tx CF,seq:0x%x\n\r",TX_SEQ(i));
			}
			
			gTimerSTmin[i].timeoutFlag = FALSE;//just set timeoutFlag to FALSE,don't set activeFlag to FALSE at here
			//gTimerSTmin[i].timer = 0;

			//send next CF			
			buffer[0] = (uint8)(TP_REAL_FRAME_TYPE_CF | TX_SEQ(i));
			
			if(TX_SEQ(i) == TP_WRAPPER_MAX_CF_SEQ)
				TX_SEQ(i) = 0;
			else
				TX_SEQ(i)++;
			TX_CF_COUNTER(i)++;
			
			if(gUpperTxDataInfor[i].restDataLen > TP_CONSECUTIVE_FRAME_DATA_LEN) //still more than 1 CF need to be sent
			{
				Tp_CpyData(&buffer[1],gUpperTxDataInfor[i].dataPtr,TP_CONSECUTIVE_FRAME_DATA_LEN);
				gUpperTxDataInfor[i].dataPtr += TP_CONSECUTIVE_FRAME_DATA_LEN;
				gUpperTxDataInfor[i].restDataLen -= TP_CONSECUTIVE_FRAME_DATA_LEN;
				NEXT_ACTION(i) = TP_ACTION_WAIT_TXCFM;
			}
			else//the last CF will be sent out
			{
				Tp_CpyData(&buffer[1],gUpperTxDataInfor[i].dataPtr,gUpperTxDataInfor[i].restDataLen);

				Tp_DisableSTminTimer(i);
				NEXT_ACTION(i) = TP_ACTION_IDLE;
				TX_NEW_FRAME_FLAG(i) = FALSE;
				
				RX_ALLOWED_FRAME_TYPE(i) = TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF;

				if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
				{
					TP_TRACE_VALUE1(TP_TRACE,TRACE_INFO,"TP: Tx all CF,seq:0x%x\n\r",TX_SEQ(i)-1);
				}
				
			}

			//TP_TRACE_VALUE5(TP_TRACE,TRACE_ERRO,"TP:Tx CF:%x %x %x %x %x\n\r",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

			Tp_SetTimer(i,TP_TIMER_N_AS);
			PRE_ACTION(i) = TP_ACTION_SEND_CF;

			if(Tp_TxDataWrapper(i,buffer,TP_BUF_LEN) == FALSE)
			{
				if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation(i,FALSE, REASON_FAIL_TX_CF_FRAME);
				Tp_ResetParas(i);
				return;
			}
		}
	}
	else if((NEXT_ACTION(i) == TP_ACTION_SEND_SF) || (NEXT_ACTION(i) == TP_ACTION_SEND_FF))//send SF or FF
	{
		#if 0
		// more this code to Tp_TxDataRequest()
		//disable RX any frame
		RX_ALLOWED_FRAME_TYPE(i) = TP_FRAME_TYPE_NONE;
		#endif
		//make sure STmin is also disable
		Tp_DisableSTminTimer(i);

		if(gUpperTxDataInfor[i].dataLen > TP_CONSECUTIVE_FRAME_DATA_LEN)//need to send FF
		{
			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:Tx FF\n\r");
			}

			//send First Frame
			//FirstFrame format: PCI_TYPE + FF_DL+ 6 BYTES DATAS
			buffer[0] = (uint8)(TP_REAL_FRAME_TYPE_FF | ((uint8)(gUpperTxDataInfor[i].dataLen>>8) & 0xF));
			buffer[1] = (uint8)(gUpperTxDataInfor[i].dataLen & 0xFF);
			Tp_CpyData(&buffer[2],gUpperTxDataInfor[i].dataHeader,TP_FIRST_FRAME_DATA_LEN);
			gUpperTxDataInfor[i].dataPtr += TP_FIRST_FRAME_DATA_LEN;
			gUpperTxDataInfor[i].restDataLen -= TP_FIRST_FRAME_DATA_LEN;

			//TP_TRACE_VALUE5(TP_TRACE,TRACE_ERRO,"TP: Tx FF:%x %x %x %x %x\n\r",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

			PRE_ACTION(i) = TP_ACTION_SEND_FF;
			#if 0
			//move this code to Tp_CheckTxcfm()
			RX_ALLOWED_FRAME_TYPE(i) = TP_FRAME_TYPE_FC;
			#endif
			TX_SEQ(i) = 1;
			TX_CF_COUNTER(i) = 0;
			Tp_SetTimer(i,TP_TIMER_N_AS);
			NEXT_ACTION(i) = TP_ACTION_IDLE;

			if(Tp_TxDataWrapper(i,buffer,TP_BUF_LEN) == FALSE)
			{
				if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation(i,FALSE, REASON_FAIL_TX_FF_FRAME);
				Tp_ResetParas(i);
				return;
			}

		}
		else//send SF
		{
			if(TP_TRACE_LEVEL > TP_TRACE_LEVEL_2)
			{
				TP_TRACE_TXT(TP_TRACE,TRACE_INFO,"TP:Tx SF\n\r");
			}

			buffer[0] = (uint8)(TP_REAL_FRAME_TYPE_SF | (uint8)(gUpperTxDataInfor[i].dataLen));
			
			Tp_CpyData(&buffer[1],gUpperTxDataInfor[i].dataHeader,gUpperTxDataInfor[i].dataLen);
			//TP_TRACE_VALUE5(TP_TRACE,TRACE_ERRO,"TP:Tx SF:%x %x %x %x %x\n\r",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);

			PRE_ACTION(i) = TP_ACTION_SEND_SF;
			Tp_SetTimer(i,TP_TIMER_N_AS);
			#if 0
			//move this code to Tp_CheckTxcfm()
			RX_ALLOWED_FRAME_TYPE(i) = TP_FRAME_TYPE_SF | TP_FRAME_TYPE_FF;
			TX_NEW_FRAME_FLAG(i) = FALSE;
			#endif
			NEXT_ACTION(i) = TP_ACTION_IDLE;
			
			if(Tp_TxDataWrapper(i,buffer,TP_BUF_LEN) == FALSE)
			{
				if(NULL!=gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation)
					gUpperLayerWrapper[TP_DIAG_CH_TYPE(i)].FnTxDataConfirmation(i,FALSE, REASON_FAIL_TX_SF_FRAME);
				Tp_ResetParas(i);
				return;
			}

			//here we can't call Tp_ResetParas(), we need to wait for the Tp_TxConfirmation() result
		}
	}
}


void Tp_MainLoop()
{
	static uint8 i = 0;
	uint32 curTick = 0;

	if(gTpInitFinishedFlag == FALSE)
		return;

	if(FALSE == TX_NEW_FRAME_FLAG(1))
	{
		Tp_Trace_Pop();
	}
	
	curTick = Tp_GetCurTick();

	for(i=0;i<TP_PDU_GROUP_NUM;i++)
	{
		Tp_CheckTxcfm(i);
		//timer for TX next CF if it had been active after we rx FC frame
		if(gTimerSTmin[i].activeFlag == TRUE)
		{
			if(gTimerSTmin[i].timeoutFlag==FALSE
				&& (curTick-gTimerSTmin[i].preTick >= gTimerSTmin[i].timer))
			{
				gTimerSTmin[i].timeoutFlag = TRUE;
				gTimerSTmin[i].preTick = curTick;
			}
			//gTimerSTmin[i].timer++;
		}
		
		if(TP_TIMER_TYPE(i) != TP_TIMER_NONE)
		{
			
			if(curTick-TP_TIMER_PRE_TICK(i) >= TP_TIMER(i))
			{
				TP_TIMER(i) = 0;
				TP_TIMEOUT_FLAG(i) = TRUE;// time out now
			}
		}

		if(TP_TIMEOUT_FLAG(i) == TRUE)
			Tp_TimerProcess(i);

		//if(RX_NEW_FRAME_FLAG(i) == TRUE)
			Tp_RxProcess(i);
		
		if(TX_NEW_FRAME_FLAG(i) == TRUE)
			Tp_TxProcess(i);

	}

	if(NULL!=TP_UDS_LOOP_CBK)
		TP_UDS_LOOP_CBK();
	if(NULL!=TP_AME_LOOP_CBK)
		TP_AME_LOOP_CBK();
}

