/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcSemFreeRTOS.c
 * @brief    ##### 
 * @details  this file ###
 * @author   
 * @version  1.0.0
 * @date     4/20/2017
 **************************************************************************/
//#include "ipcSem.h"
#include "osif.h" 
#include "Task_IpcSem.h"


static semaphore_t s_Task_IpcSemPool = NULL;


boolean Task_IpcSemCreate(void)
{
	boolean returnValue = E_NOT_OK;
	if(STATUS_SUCCESS == OSIF_SemaCreate(&s_Task_IpcSemPool, 0 ))
	{
		returnValue = E_OK;
	}
	return (returnValue);
}

void Task_IpcSemPost(void)
{
	if(s_Task_IpcSemPool != NULL)
	{
		OSIF_SemaPost(&s_Task_IpcSemPool);
	}
}

boolean Task_IpcSemWait(uint16_t timeout)
{
	boolean returnValue = E_NOT_OK;
	
	if((s_Task_IpcSemPool != NULL) && (STATUS_SUCCESS ==  OSIF_SemaWait(&s_Task_IpcSemPool, timeout)))
	{
		returnValue = E_OK;
	}
	return returnValue;
}


