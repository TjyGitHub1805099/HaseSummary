/********************************************************************
File name: System_Task.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-18 09:54:28
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#include <string.h>
#include "System_Task.h"
#if (STD_ON == SYSTEM_TASK_DEBUG_TRACE)
#include "trace.h"
#endif

extern SystemTaskListType SystemTaskList[];
extern SystemTaskConfigType SystemTaskConfig[];
extern const uint16 System_TaskMaxId;

#define SYSTEM_TASK_MODULE_UNINIT 0X00
#define SYSTEM_TASK_MODULE_INIT 0X01

#define SYSTEM_TASK_FLAG_VALID 0X01
#define SYSTEM_TASK_FLAG_ACTIVE 0X02
#define SYSTEM_TASK_FLAG_EVENTVALID 0X04
#define SYSTEM_TASK_FLAG_EVENTBUSY 0X08

typedef struct
{
	uint8 init;
} SystemTaskManagerType;

SystemTaskManagerType SystemTaskManager = {SYSTEM_TASK_MODULE_UNINIT};


void System_Task_Init(void)
{
	BaseType_t  l_returnValue = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
	QueueHandle_t l_queueHandle = NULL;
	uint8 l_taskId = 0U;

	if (SYSTEM_TASK_MODULE_INIT == SystemTaskManager.init)
	{
		return;
	}
	for (l_taskId = 0; l_taskId < System_TaskMaxId; l_taskId++)
	{
		SystemTaskList[l_taskId].flagGroup = 0;
		l_returnValue = xTaskCreate(SystemTaskConfig[l_taskId].taskMainFuncPtr,
		                            SystemTaskConfig[l_taskId].taskName,
		                            SystemTaskConfig[l_taskId].stackSize,
		                            SystemTaskConfig[l_taskId].taskParameter,
		                            SystemTaskConfig[l_taskId].priority,
		                            &SystemTaskList[l_taskId].taskHandle);

		if (pdPASS == l_returnValue)
		{
			SystemTaskList[l_taskId].flagGroup |= SYSTEM_TASK_FLAG_VALID;
			if (STD_ON != SystemTaskConfig[l_taskId].active)
			{
				vTaskSuspend(SystemTaskList[l_taskId].taskHandle);
				SystemTaskList[l_taskId].flagGroup &= (~SYSTEM_TASK_FLAG_ACTIVE);
			}
			else
			{
				SystemTaskList[l_taskId].flagGroup |= SYSTEM_TASK_FLAG_ACTIVE;
			}

			l_queueHandle = xQueueCreate(SystemTaskConfig[l_taskId].queueSize, sizeof(SystemTaskMsgInfoType));
			if (NULL != l_queueHandle)
			{
				SystemTaskList[l_taskId].queueHandle = l_queueHandle;
				SystemTaskList[l_taskId].flagGroup |= SYSTEM_TASK_FLAG_EVENTVALID;
			}
			else
			{
#if(STD_ON==SYSTEM_TASK_DEBUG_TRACE)
				TracePrintf(SYSTEM_TRACE, TRACE_ERRO, "[Sys-E]>Task:%d Creat dataQueue failed. Err:Mem\n\r", l_taskId);
#endif
			}

		}
		else
		{
#if(STD_ON==SYSTEM_TASK_DEBUG_TRACE)
			TracePrintf(SYSTEM_TRACE, TRACE_ERRO, "[Sys-E]>Creat task failed Err:Mem\n\r");
#endif
		}
	}
	SystemTaskManager.init = SYSTEM_TASK_MODULE_INIT;
}

void System_Task_Active(uint8 taskId)
{
	if ((SYSTEM_TASK_MODULE_INIT == SystemTaskManager.init)
	        && (0 != (SYSTEM_TASK_FLAG_VALID & SystemTaskList[taskId].flagGroup))
	        && (0 == (SYSTEM_TASK_FLAG_ACTIVE & SystemTaskList[taskId].flagGroup))
	        && (taskId < System_TaskMaxId))
	{
		vTaskResume(SystemTaskList[taskId].taskHandle);
		SystemTaskList[taskId].flagGroup |= SYSTEM_TASK_FLAG_ACTIVE;
	}
}

void System_Task_Deactivate(uint8 taskId)
{
	if ((SYSTEM_TASK_MODULE_INIT == SystemTaskManager.init)
	        && (0 != (SYSTEM_TASK_FLAG_VALID & SystemTaskList[taskId].flagGroup))
	        && (0 != (SYSTEM_TASK_FLAG_ACTIVE & SystemTaskList[taskId].flagGroup))
	        && (taskId < System_TaskMaxId))
	{
		SystemTaskList[taskId].flagGroup &= (~SYSTEM_TASK_FLAG_ACTIVE);
		vTaskSuspend(SystemTaskList[taskId].taskHandle);
	}
}

boolean System_Task_IsActive(uint8 taskId)
{
	boolean returnValue = STD_OFF;

	if(taskId < System_TaskMaxId)
	{
		returnValue = (SystemTaskList[taskId].flagGroup & SYSTEM_TASK_FLAG_ACTIVE)>>1;
	}
	return(returnValue);
}

void System_Task_Delay(uint32 timeMs)
{
    vTaskDelay(pdMS_TO_TICKS(timeMs));
}

boolean System_Task_SendMessage(uint8 taskId, SystemTaskMsgInfoType* msgBufPtr, boolean fromISR)
{
	boolean l_returnValue = E_OK;
	boolean l_memErrFlag = E_OK;
	BaseType_t l_ercd = errQUEUE_FULL;
	uint8 * l_msgAddress = NULL;

	if ((SYSTEM_TASK_MODULE_INIT == SystemTaskManager.init)
	        && (0 != (SYSTEM_TASK_FLAG_EVENTVALID & SystemTaskList[taskId].flagGroup))
	        && (taskId < System_TaskMaxId))
	{
		if (0 != msgBufPtr->size)
		{
			l_msgAddress = (uint8 *)pvPortMalloc(msgBufPtr->size);
			if (NULL == l_msgAddress)
			{
#if(STD_ON==SYSTEM_TASK_DEBUG_TRACE)
				TracePrintf(SYSTEM_TRACE, TRACE_ERRO, "[Sys-E]>Get msg memory failed\n\r");
#endif
				l_memErrFlag = E_NOT_OK;
				l_returnValue = E_NOT_OK;
			}
			else
			{
				memcpy(l_msgAddress, msgBufPtr->parBufPtr, msgBufPtr->size);
				msgBufPtr->parBufPtr = l_msgAddress;
			}
		}
		if (E_OK == l_memErrFlag)
		{
			if ( TRUE == fromISR )
			{
				l_ercd = xQueueSendToBackFromISR(SystemTaskList[taskId].queueHandle, msgBufPtr, NULL);
				if (pdTRUE != l_ercd)
				{
#if(STD_ON==SYSTEM_TASK_DEBUG_TRACE)
					TracePrintf(SYSTEM_TRACE, TRACE_ERRO, "[Sys-E]>Non-task ctx send msg to TaskId:%d Err:QueueFull\n\r", taskId);
#endif
					if (l_msgAddress != NULL)
					{
					vPortFree(l_msgAddress);
					}
					l_returnValue = E_NOT_OK;
				}
			}
			else
			{
				l_ercd = xQueueSendToBack(SystemTaskList[taskId].queueHandle, msgBufPtr, 0);
				if (pdPASS != l_ercd)
				{
#if(STD_ON==SYSTEM_TASK_DEBUG_TRACE)
					TracePrintf(SYSTEM_TRACE, TRACE_ERRO, "[Sys-E]>Send msg to Task:%d Err:QueueFull\n\r", taskId);
#endif
					if (l_msgAddress != NULL)
					{
					vPortFree(l_msgAddress);
					}
					l_returnValue = E_NOT_OK;
				}
			}
		}
	}
	else
	{
		l_returnValue = E_NOT_OK;
	}
	return (l_returnValue);
}

boolean System_Task_ReceiveMessage(uint8 taskId, SystemTaskMsgInfoType* msgBufPtr, boolean fromISR, uint32 timeoutMs)
{
	boolean l_returnValue = E_OK;
	BaseType_t l_ercd = pdPASS;
	SystemTaskMsgInfoType tmpMsgBufPtr = {0,0,NULL};
	uint32 tick_counter = 0;

	if ((SYSTEM_TASK_MODULE_INIT == SystemTaskManager.init)
	        && (NULL != msgBufPtr)
	        && (taskId < System_TaskMaxId))
	{
		if (OS_WAITFOREVER != timeoutMs)
		{
			timeoutMs = pdMS_TO_TICKS(timeoutMs);
		}
		if (TRUE != fromISR)
		{
			l_ercd = xQueueReceive(SystemTaskList[taskId].queueHandle, &tmpMsgBufPtr, timeoutMs);
		}
		else
		{
			l_ercd = xQueueReceiveFromISR(SystemTaskList[taskId].queueHandle, &tmpMsgBufPtr, NULL);
		}
		if (pdPASS != l_ercd)
		{
			if(NULL != tmpMsgBufPtr.parBufPtr)
			{
				vPortFree(tmpMsgBufPtr.parBufPtr);
			}

			l_returnValue = E_NOT_OK;
		}
		else
		{
			if (0 != tmpMsgBufPtr.size)
			{
				if(NULL != msgBufPtr->parBufPtr)
				{
					msgBufPtr->msgId = tmpMsgBufPtr.msgId;
					msgBufPtr->size = tmpMsgBufPtr.size;
					memcpy(msgBufPtr->parBufPtr, tmpMsgBufPtr.parBufPtr, tmpMsgBufPtr.size);
				}
				vPortFree(tmpMsgBufPtr.parBufPtr);
			}
			else//for alarm only
			{
				msgBufPtr->msgId = tmpMsgBufPtr.msgId;
				if(msgBufPtr->parBufPtr!=NULL)
				{
					tick_counter = (uint32)tmpMsgBufPtr.parBufPtr;
					memcpy(msgBufPtr->parBufPtr, (uint8*)(&tick_counter),4);
				}
			}
		}
	}
	return (l_returnValue);
}

TaskHandle_t System_Task_GetHandle(uint8 taskId)
{
	TaskHandle_t returnValue = NULL_PTR;
	
	if(taskId < System_TaskMaxId)
	{
		returnValue = SystemTaskList[taskId].taskHandle;
	}
	return(returnValue);
}

const sint8 * System_Task_GetName(uint8 taskId)
{
	const sint8 * returnValue = NULL_PTR;
	
	if(taskId < System_TaskMaxId)
	{
		returnValue = SystemTaskConfig[taskId].taskName;
	}
	return(returnValue);
}

/*=========================== END OF FILE: System_Task.c ===========================*/

