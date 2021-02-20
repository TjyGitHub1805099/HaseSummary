#include "SpiIpc_cfg.h"
#include "SPI_CircularQueue.h"
#include "spiMutex.h"
#include "SpiIpc.h"
#include "null.h"

extern const SpiIpcChCfg SpiIpcUserCfg[];
extern SPI_CQueue_ManagerType *SPI_TxQueueM_Group[];
extern SPI_CQueue_ManagerType *SPI_RxQueueM_Group[];
#if (SPI_IPC_QUEUE_DYNAMIC == 0)
extern uint8 *SPI_TxQueueGroup[];
extern uint8 *SPI_RxQueueGroup[];
#else
extern SPI_QueueSizeGroupType SPI_QueueSizeGroup[HW_SPI_IPC_MAX_NUM];
#endif

extern const SpiIpcGlobalStruct gSpiIpcGlobalCfg;
extern SpiCqMutex gSpiIpcCQueueMutexIndex[];
/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 SPI_CQueue_Init(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 size)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (NULL != buffer) && (0 != size))
	{
		SPI_CQueue_Manager[channel].head = 0;
		SPI_CQueue_Manager[channel].tail = 0;
		SPI_CQueue_Manager[channel].FrameCount = 0;
		SPI_CQueue_Manager[channel].size = size;
		SPI_CQueue_Manager[channel].buf = buffer;
		SPI_CQueue_Manager[channel].init = TRUE;
		l_return = E_OK;
	}
	return (l_return);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
//uint8 SPI_CQueue_Push(uint8 channel, uint8 *buffer, uint16 len)
uint8 SPI_CQueue_Push(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 len)
{
	uint8 l_return = E_NOT_OK;
	uint16 l_loop = 0;
	uint32 remain_len = 0;
	uint16 require_len = 0;

	if(len > SPI_MAX_DATA_SIZE)
	{
		return E_NOT_OK;
	}

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (TRUE == SPI_CQueue_Manager[channel].init)
	        && (NULL != buffer) && (0 != len))
	{
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}

		if(SPI_CQueue_Manager[channel].tail >= SPI_CQueue_Manager[channel].head)
		{
			remain_len = SPI_CQueue_Manager[channel].size - SPI_CQueue_Manager[channel].tail + SPI_CQueue_Manager[channel].head;
		}
		else
		{
			remain_len =SPI_CQueue_Manager[channel].head - SPI_CQueue_Manager[channel].tail;
		}

		require_len = (len+2);	//keep one byte for data length and one byte for margin
		if(remain_len < require_len)	//if remain space not enough to push current frame, just remove before data
		{
			while(remain_len < require_len)
			{
				remain_len += *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head) + 1;
				if(SPI_CQueue_Manager[channel].FrameCount > 0)
				{
					SPI_CQueue_Manager[channel].FrameCount--;
				}
				SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head 
					+*(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head) + 1)%SPI_CQueue_Manager[channel].size;
			}
		}
		
		*(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].tail) = len;
		SPI_CQueue_Manager[channel].tail = (SPI_CQueue_Manager[channel].tail + 1) % SPI_CQueue_Manager[channel].size;
		for (l_loop = 0; l_loop < len; l_loop++)
		{
			*(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].tail) = *(buffer + l_loop);
			SPI_CQueue_Manager[channel].tail = (SPI_CQueue_Manager[channel].tail + 1) % SPI_CQueue_Manager[channel].size;
		}
		SPI_CQueue_Manager[channel].FrameCount++;
		l_return = E_OK;
		
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}
	}
	return (l_return);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 SPI_CQueue_PushFront(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 len)
{
	uint8 l_return = E_NOT_OK;
	uint16 l_loop = 0;
	uint32 remain_len = 0;
	uint16 head_inc = 0;
	uint16 l_tail;

	if(len > SPI_MAX_DATA_SIZE)
	{
		return E_NOT_OK;
	}

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (TRUE == SPI_CQueue_Manager[channel].init)
	        && (NULL != buffer) && (0 != len))
	{
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}
		if(SPI_CQueue_Manager[channel].tail >= SPI_CQueue_Manager[channel].head)
		{
			remain_len = SPI_CQueue_Manager[channel].size - SPI_CQueue_Manager[channel].tail + SPI_CQueue_Manager[channel].head;
		}
		else
		{
			remain_len = SPI_CQueue_Manager[channel].head - SPI_CQueue_Manager[channel].tail;
		}
		
		remain_len = remain_len - 2; //keep one byte for data length and one byte for margin
		if(remain_len < len)	//if remain space not enough to push current frame, just remove before data
		{
			while((remain_len+head_inc) < len)
			{
				head_inc += *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head) + 1;
				if(SPI_CQueue_Manager[channel].FrameCount > 0)
				{
					SPI_CQueue_Manager[channel].FrameCount--;
				}
			}
			SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + head_inc) % SPI_CQueue_Manager[channel].size;
		}

		if(SPI_CQueue_Manager[channel].head  < (len+1))
		{
			SPI_CQueue_Manager[channel].head = SPI_CQueue_Manager[channel].size - ((len+1)-SPI_CQueue_Manager[channel].head);
		}
		else
		{
			SPI_CQueue_Manager[channel].head -= (len+1);
		}

		l_tail = SPI_CQueue_Manager[channel].head;
		
		*(SPI_CQueue_Manager[channel].buf + l_tail) = len;
		l_tail = (l_tail + 1) % SPI_CQueue_Manager[channel].size;
		for (l_loop = 0; l_loop < len; l_loop++)
		{
			*(SPI_CQueue_Manager[channel].buf + l_tail) = *(buffer + l_loop);
			l_tail = (l_tail + 1) % SPI_CQueue_Manager[channel].size;
		}
		SPI_CQueue_Manager[channel].FrameCount++;
		l_return = E_OK;
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}
	}
	return (l_return);
}


//len=0:pop all data
/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
//uint16 SPI_CQueue_Pop(uint8 channel, uint8 *buffer, uint16 len, uint8 fromISR)
uint16 SPI_CQueue_Pop(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 buffer_size)
{
	uint16 l_return = 0;
	uint16 l_loop = 0;
	uint16 l_count = 0;

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (TRUE == SPI_CQueue_Manager[channel].init)
	        && (NULL != buffer))
	{
		if (0 < SPI_CQueue_Manager[channel].FrameCount)
		{
			if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
			{
				spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
			}
			else
			{
				spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
			}
			l_count = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
			if(l_count > buffer_size)
			{
				return 0;
			}
			if(l_count > 0)
			{
				SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
				for (l_loop = 0; l_loop < l_count; l_loop++)
				{
					*(buffer + l_loop) = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
					SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
				}
				SPI_CQueue_Manager[channel].FrameCount--;
			}
			if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
			{
				spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
			}
			else
			{
				spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
			}
			l_return = l_count;
		}
	}
	return (l_return);
}


/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint16 SPI_CQueue_PopMulti(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 buffer_size)
{
	uint16 l_return = 0;
	uint8 l_loop = 0;
	uint8 l_count = 0;

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (TRUE == SPI_CQueue_Manager[channel].init)
	        && (NULL != buffer))
	{
		if (0 < SPI_CQueue_Manager[channel].FrameCount)
		{
			if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
			{
				spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
			}
			else
			{
				spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
			}
			l_count = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
			if(l_count > buffer_size)
			{
				return 0;
			}
			
			while(1)
			{		
				l_count = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
				if((l_return + l_count + 2) >= buffer_size)	//additions 2 bytes, 1 channel byte +1 count byte
				{
					break;
				}
				if(l_count > 0)
				{		
					*(buffer + l_return) = channel;
					l_return += 1;
					*(buffer + l_return) = l_count;
					l_return += 1;
					SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
					for (l_loop = 0; l_loop < l_count; l_loop++)
					{
						*(buffer + l_loop + l_return) = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
						SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
					}
					l_return += l_count;
					
					SPI_CQueue_Manager[channel].FrameCount--;
					if(SPI_CQueue_Manager[channel].FrameCount == 0)
					{
						break;
					}
				}
				else
				{
					break;
				}

			}
			if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
			{
				spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
			}
			else
			{
				spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
			}
		}
	}
	return (l_return);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint16 SPI_CQueue_PopMultiChannel(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 *buffer, uint16 buffer_size)
{
	uint16 l_return = 0;
	uint8 l_loop = 0;
	uint8 l_count = 0;
	uint8 channel = 0;

	for(channel = 0; channel < SpiIpcUserCfg[HwChannel].ChannelNum; channel++)
	{
		if ((TRUE == SPI_CQueue_Manager[channel].init) && (NULL != buffer))
		{
			if (0 < SPI_CQueue_Manager[channel].FrameCount)
			{
				if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
				{
					spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
				}
				else
				{
					spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
				}
				l_count = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
				if(l_count > buffer_size)
				{
					return 0;
				}
				
				while(1)
				{		
					l_count = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
					if((l_return + l_count + 2) >= buffer_size)	//additions 2 bytes, 1 channel byte +1 count byte
					{
						break;
					}
					if(l_count > 0)
					{		
						*(buffer + l_return) = channel;
						l_return += 1;
						*(buffer + l_return) = l_count;
						l_return += 1;
						SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
						for (l_loop = 0; l_loop < l_count; l_loop++)
						{
							*(buffer + l_loop + l_return) = *(SPI_CQueue_Manager[channel].buf + SPI_CQueue_Manager[channel].head);
							SPI_CQueue_Manager[channel].head = (SPI_CQueue_Manager[channel].head + 1) % SPI_CQueue_Manager[channel].size;
						}
						l_return += l_count;
						
						SPI_CQueue_Manager[channel].FrameCount--;
						if(SPI_CQueue_Manager[channel].FrameCount == 0)
						{
							break;
						}
					}
					else
					{
						break;
					}

				}
				if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
				{
					spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
				}
				else
				{
					spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
				}
			}
		}
	}
	return (l_return);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 SPI_CQueue_GetLen(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint16 *len)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (NULL != len))
	{
		*len = SPI_CQueue_Manager[channel].FrameCount;
		l_return = E_OK;
	}
	return (l_return);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 SPI_CQueue_GetTotalLen(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint16 *len)
{
	uint8 l_return = E_NOT_OK;
	uint8 l_cnt;

	if (NULL != len)
	{
		*len = 0;
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}
		for(l_cnt = 0; l_cnt < SpiIpcUserCfg[HwChannel].ChannelNum; l_cnt++)
		{
			*len += SPI_CQueue_Manager[l_cnt].FrameCount;
		}
		if(SPI_CQueue_Manager == SPI_CQueueGetTxManagerHandle(HwChannel))
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Tx);
		}
		else
		{
			spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Rx);
		}
		l_return = E_OK;
	}
	return (l_return);
}


/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 SPI_CQueue_IsEmpty(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *isEmpty)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < SpiIpcUserCfg[HwChannel].ChannelNum) && (NULL != isEmpty))
	{
		*isEmpty = (uint8)((SPI_CQueue_Manager[channel].FrameCount == 0) ? TRUE : FALSE);
	}
	return (l_return);
}

SPI_CQueue_ManagerType * SPI_CQueueGetTxManagerHandle(uint8 HwChannel)
{
	return  SPI_TxQueueM_Group[HwChannel];
}


SPI_CQueue_ManagerType * SPI_CQueueGetRxManagerHandle(uint8 HwChannel)
{
	return  SPI_RxQueueM_Group[HwChannel];
}

void SPI_CQueueGlobalLock(uint8 HwChannel)
{
	spiMutexLock(gSpiIpcCQueueMutexIndex[HwChannel].Global);
}

void SPI_CQueueGlobalUnLock(uint8 HwChannel)
{
	spiMutexUnlock(gSpiIpcCQueueMutexIndex[HwChannel].Global);
}

uint8 SPI_RxTxQueue_Init(void)
{
	uint8 ch = 0;
	uint8 i;
#if !(SPI_IPC_QUEUE_DYNAMIC == 0)
	uint8 *addr;
	uint16 buf_size;
#endif
	for(i = 0; i < gSpiIpcGlobalCfg.spiHwMaxCh; i++)
	{
		spiMutexCreate(&gSpiIpcCQueueMutexIndex[i].Tx);
		spiMutexCreate(&gSpiIpcCQueueMutexIndex[i].Rx);
		spiMutexCreate(&gSpiIpcCQueueMutexIndex[i].Global);
		for(ch = 0; ch < SpiIpcUserCfg[i].ChannelNum; ch++)
		{
		#if (SPI_IPC_QUEUE_DYNAMIC == 0)
			SPI_CQueue_Init(SpiIpcUserCfg[i].Num,SPI_TxQueueM_Group[i], ch, SPI_TxQueueGroup[i]+ch*SPI_CQUEUE_TXCH_SIZE,SPI_CQUEUE_TXCH_SIZE);
			SPI_CQueue_Init(SpiIpcUserCfg[i].Num,SPI_RxQueueM_Group[i], ch, SPI_RxQueueGroup[i]+ch*SPI_CQUEUE_RXCH_SIZE,SPI_CQUEUE_RXCH_SIZE);
		#else
			buf_size = SPI_QueueSizeGroup[i][ch][0];
			if((buf_size != 0) && (buf_size < SPI_IPC_QUEUE_MIN_SIZE))
			{
				buf_size = SPI_IPC_QUEUE_MIN_SIZE;
			}
			if(buf_size > 0)
			{
				addr = (uint8 *)pvPortMalloc(buf_size);
				if(addr != NULL)
				{
					SPI_CQueue_Init(SpiIpcUserCfg[i].Num, SPI_TxQueueM_Group[i], ch, addr, buf_size);
				}
				else
				{
					return E_NOT_OK; //for tmp debug
				}
			}
			
			buf_size = SPI_QueueSizeGroup[i][ch][1];
			if((buf_size != 0) && (buf_size < SPI_IPC_QUEUE_MIN_SIZE))
			{
				buf_size = SPI_IPC_QUEUE_MIN_SIZE;
			}
			if(buf_size > 0)
			{
				addr = (uint8 *)pvPortMalloc(buf_size);
				if(addr != NULL)
				{
					SPI_CQueue_Init(SpiIpcUserCfg[i].Num, SPI_RxQueueM_Group[i], ch, addr, buf_size);
				}
				else
				{
					return E_NOT_OK; //for tmp debug
				}
			}
		#endif
		}
	}
	return E_OK;
}



/*=========================== END OF FILE: CircularQueue.c ===========================*/
