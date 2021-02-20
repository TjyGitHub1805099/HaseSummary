/*============================[R E V I S I O N   H I S T O R Y]=================================*/
/************************************************************************************************/
/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * The integer value 1 is being added or subtracted from a pointer.
 */

/* MISRA-C:2004 Rule 19.1,
 * #include statements in a file should only be preceded by other preprocessor directives or comments.
 */

/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/


/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/
#include "Std_Types.h"
#include "TraceStore.h"
#include "TraceStore_Cfg.h"

#if (STD_ON == TRACE_STORE_USING )

#include "CircularQueue.h"
#include "SpiIpc_cfg.h"
#include "SPI_CircularQueue.h"
/*========================[E X T E R N A L  V A R I A B L E]====================================*/
/************************************************************************************************/
extern boolean socOnFlag;
extern SpiCqMutex gSpiIpcCQueueMutexIndex[HW_SPI_IPC_MAX_NUM];
extern boolean gSystemInitDoneFlag;
/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
uint16 TraceStoreChannel = CQUEUE_ERR_CHANNEL;
uint16 TraceStoreCountChannel = CQUEUE_ERR_CHANNEL;

uint8 lTraceStoreBuff[TRACE_STORE_BUFF_LEN];

TraceStoreType gTraceStoreCtrl = 
{
	CQUEUE_ERR_CHANNEL,//channel
	CQUEUE_ERR_CHANNEL,//countChannel

	0,//dataLen
	(TRACE_STORE_MAIN_START_LOC),//dataLenInMain
	STD_OFF,//traceStart
	STD_ON,//traceInmain
	{0},//dataBuf
};


/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/
/* pointer sou  copy to point des */
static void TraceStore_MemCpy(const uint8* sou,uint8* des,uint16 len)
{	
	uint16 l_loop=0;
	while(l_loop<len)
	{	
		*(des + l_loop) = *(sou + l_loop);
		l_loop++;
	}
}

//this function must replay on free rtos
void TraceStore_Init(void)
{	
	gTraceStoreCtrl.channel = CQueue_Init(CQUEUE_BUFTYPE_UINT8, &lTraceStoreBuff[0], TRACE_STORE_BUFF_LEN);
	gTraceStoreCtrl.traceInmain = STD_OFF;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
void TraceStore_Push(uint8 *data, uint32 len, uint8 fromISR)
#else
void TraceStore_Push(uint8 *data, uint32 len)
#endif
{	
	static uint8 l_copyMainTrace = STD_ON;
	uint16 l_queueIndex = CQUEUE_ERR_PUSHPOP;

	if(STD_ON == gTraceStoreCtrl.traceInmain)
	{
		if( ( gTraceStoreCtrl.dataLenInMain + len ) < (TRACE_STORE_BUFF_LEN ))
		{
			TraceStore_MemCpy(data,&lTraceStoreBuff[gTraceStoreCtrl.dataLenInMain%TRACE_STORE_BUFF_LEN],len);
			gTraceStoreCtrl.dataLenInMain += len;
		}
	}
	else
	{
		//push main trace data
		if(STD_ON == l_copyMainTrace)
		{
			l_copyMainTrace = STD_OFF;

			#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
				l_queueIndex = CQueue_Push(gTraceStoreCtrl.channel, &lTraceStoreBuff[(TRACE_STORE_MAIN_START_LOC)], (gTraceStoreCtrl.dataLenInMain-(TRACE_STORE_MAIN_START_LOC)), STD_OFF);
			#else
				l_queueIndex = CQueue_Push(gTraceStoreCtrl.channel, &lTraceStoreBuff[(TRACE_STORE_MAIN_START_LOC)], (gTraceStoreCtrl.dataLenInMain-(TRACE_STORE_MAIN_START_LOC));
			#endif
		}

	
		#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
			l_queueIndex = CQueue_Push(gTraceStoreCtrl.channel, data, len, STD_OFF);
		#else
			l_queueIndex = CQueue_Push(gTraceStoreCtrl.channel, data, len);
		#endif
	}
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
void TraceStore_Pop(uint8 *data, uint32 len, uint8 fromISR)
#else
void TraceStore_Pop(uint8 *data, uint32 len)
#endif
{
	uint16 l_queueIndex = CQUEUE_ERR_PUSHPOP;

	#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
		l_queueIndex = CQueue_Pop(gTraceStoreCtrl.channel, data, len, STD_OFF);
	#else
		l_queueIndex = CQueue_Pop(gTraceStoreCtrl.channel, data, len);
	#endif
}

void TraceStore_traceStartSet(uint8 enable)
{
	gTraceStoreCtrl.traceStart = enable;
}

uint16 TraceStore_GetRemainLen(uint8 HwChannel,uint8 channel)
{
	uint16 remain_len = 0;
	SPI_CQueue_ManagerType *SPI_CQueue_Manager;
	SPI_CQueue_Manager = SPI_CQueueGetTxManagerHandle(HwChannel);
	if(( HwChannel < HW_SPI_IPC_MAX_NUM ) && ( channel <= SpiIpcUserCfg[HwChannel].ChannelNum ) )
	{
		//Lock
		SPI_CQueueGlobalLock(HwChannel);//SPI global lock
		spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);//SPI chanel lock

		//get remain length
		if(SPI_CQueue_Manager[channel].tail >= SPI_CQueue_Manager[channel].head)
		{
			remain_len = SPI_CQueue_Manager[channel].size - SPI_CQueue_Manager[channel].tail + SPI_CQueue_Manager[channel].head;
		}
		else
		{
			remain_len =SPI_CQueue_Manager[channel].head - SPI_CQueue_Manager[channel].tail;
		}

		//Unlock
		spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);//SPI chanel unlock
		SPI_CQueueGlobalUnLock(HwChannel);//SPI global unlock
	}
	else
	{
		remain_len = 0 ;
	}

	return remain_len;
}
void TraceStore_MainFunction(uint8 HwChannel,uint8 chanel)
{
	uint16 len = 0 ;
	static uint32 delayCnt = 0 ;

//1.when SOC is already (IPC conected and can to save)
//2.delay 1000ms avoid IPC crowd after IPC conected ,and every 20ms send once
//3.get chanel remain length ,if lager than (SPI_CQUEUE_TXCH_SIZE - SPI_MAX_DATA_SIZE) can be send
//  used to ensure KEYSERVICE data not be affected
	if(( TRUE == socOnFlag ) && ( STD_ON == gTraceStoreCtrl.traceStart ) && (TRUE == gSystemInitDoneFlag) )
	{
		if( (++delayCnt >= 100) && ( (delayCnt%2) == 0) )
		{
			if( TraceStore_GetRemainLen(HwChannel,chanel) > (SPI_CQUEUE_TXCH_SIZE - SPI_MAX_DATA_SIZE) )
			{
				//get data buffer cache len
				#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
					CQueue_GetLen(gTraceStoreCtrl.channel,&gTraceStoreCtrl.dataLen,STD_OFF);
				#else
					CQueue_GetLen(gTraceStoreCtrl.channel,&gTraceStoreCtrl.dataLen);
				#endif
	
				if( 0 != gTraceStoreCtrl.dataLen )
				{
					if( gTraceStoreCtrl.dataLen >= TRACESTORE_IPC_SW_SEND_LEN )//max pop out TRACESTORE_IPC_SW_SEND_LEN
						len = TRACESTORE_IPC_SW_SEND_LEN ;
					else
						len = gTraceStoreCtrl.dataLen;
					
				#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
					TraceStore_Pop(&gTraceStoreCtrl.dataBuf[0],len,1);
				#else
					TraceStore_Pop(&gTraceStoreCtrl.dataBuf[0],len);
				#endif
					
				ipc_send_notification(TRACESTORE_IPC_HW_CHANNEL,TRACESTORE_IPC_SW_CHANNEL,TRACESTORE_IPC_SW_FUNCTION_ID,&gTraceStoreCtrl.dataBuf[0],len);
				}	
			}
		}
	}
}

#endif//#if (STD_ON == TRACE_STORE_USING )
