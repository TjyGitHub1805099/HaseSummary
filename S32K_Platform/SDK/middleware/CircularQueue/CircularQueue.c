/**
*@file  CircularQueue.c
*@brief Containing the entire or parts of Circular Queue code.
*@date    2017-10-26 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-10-26  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "CircularQueue.h"
#include "CircularQueue_Cfg.h"
#include "System_Mutex.h"
#include "System_Portable.h"

typedef struct {
	uint8 init;
	uint16 head;
	uint16 tail;
	uint16 size;
	uint16 count;
	SystemMutexType lock;
	uint8 *buf;
} CQueue_ManagerType;

CQueue_ManagerType CQueue_Manager[CQUEUE_CHANNEL_MAX];

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
uint8 CQueue_Init(uint8 channel, uint8 *buffer, uint16 size)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != buffer) && (0 != size))
	{
		EnterCriticalCode();
		CQueue_Manager[channel].head = 0;
		CQueue_Manager[channel].tail = 0;
		CQueue_Manager[channel].count = 0;
		CQueue_Manager[channel].lock = System_Mutex_Create();
		CQueue_Manager[channel].size = size;
		CQueue_Manager[channel].buf = buffer;
		CQueue_Manager[channel].init = TRUE;
		ExitCriticalCode();
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
uint8 CQueue_Push(uint8 channel, uint8 *buffer, uint16 len, uint8 fromISR)
//uint8 CQueue_Push(uint8 channel, uint8 *buffer, uint16 len)
{
	uint8 l_return = E_NOT_OK;
	uint16 l_loop = 0;

	if ((channel < CQUEUE_CHANNEL_MAX) && (TRUE == CQueue_Manager[channel].init)
	        && (NULL != buffer) && (0 != len))
	{
		System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
		if ((CQueue_Manager[channel].count + len) <= CQueue_Manager[channel].size)
		{
			for (l_loop = 0; l_loop < len; l_loop++)
			{
				*(CQueue_Manager[channel].buf + CQueue_Manager[channel].tail) = *(buffer + l_loop);
				CQueue_Manager[channel].tail = (CQueue_Manager[channel].tail + 1) % CQueue_Manager[channel].size;
				CQueue_Manager[channel].count++;
			}
			l_return = E_OK;
		}
		System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
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
uint16 CQueue_Pop(uint8 channel, uint8 *buffer, uint16 len, uint8 fromISR)
//uint16 CQueue_Pop(uint8 channel, uint8 *buffer, uint16 len)
{
	uint16 l_return = 0;
	uint16 l_loop = 0;
	uint16 l_count = 0;

	if ((channel < CQUEUE_CHANNEL_MAX) && (TRUE == CQueue_Manager[channel].init)
	        && (NULL != buffer))
	{
		if (0 != len)
		{
			if(len <= CQueue_Manager[channel].count)
			{
				l_count = len;
			}	
		}
		else
		{
			l_count = CQueue_Manager[channel].count;
		}
		for (l_loop = 0; l_loop < l_count; l_loop++)
		{	
			System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
			*(buffer + l_loop) = *(CQueue_Manager[channel].buf + CQueue_Manager[channel].head);
			CQueue_Manager[channel].head = (CQueue_Manager[channel].head + 1) % CQueue_Manager[channel].size;
			CQueue_Manager[channel].count--;
			System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
		}
		l_return = l_count;
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
uint8 CQueue_Clear(uint8 channel,uint8 fromISR)
{
	uint8 l_return = E_NOT_OK;

	if (channel < CQUEUE_CHANNEL_MAX)
	{
		System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
		CQueue_Manager[channel].head = 0;
		CQueue_Manager[channel].tail = 0;
		CQueue_Manager[channel].count = 0;
		System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
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
uint8 CQueue_GetLen(uint8 channel, uint16 *len, uint8 fromISR)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != len))
	{
		System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
		*len = CQueue_Manager[channel].count;
		System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
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
uint8 CQueue_IsEmpty(uint8 channel, uint8 *isEmpty, uint8 fromISR)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != isEmpty))
	{
		System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
		*isEmpty = (uint8)((CQueue_Manager[channel].count == 0) ? TRUE : FALSE);
		System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
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
uint8 CQueue_IsFull(uint8 channel, uint8 *isFull, uint8 fromISR)
{
	uint8 l_return = E_NOT_OK;

	if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != isFull))
	{
		System_Mutex_Lock(CQueue_Manager[channel].lock,fromISR);
		*isFull = (uint8)((CQueue_Manager[channel].count == CQueue_Manager[channel].size) ? TRUE : FALSE);
		System_Mutex_Unlock(CQueue_Manager[channel].lock,fromISR);
	}
	return (l_return);
}
/*=========================== END OF FILE: CircularQueue.c ===========================*/
