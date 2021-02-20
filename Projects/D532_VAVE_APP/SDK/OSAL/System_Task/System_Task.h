/********************************************************************
File name: System_Task.h
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
#ifndef _SYSTEM_TASK_H_
#define _SYSTEM_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "System_Task_Cfg.h"
#include "System_Portable.h"
#if (SYS_OS_TYPE == SYS_OS_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#define OS_WAITFOREVER portMAX_DELAY
#endif


typedef struct
{
	uint8 flagGroup;
	TaskHandle_t taskHandle;
	QueueHandle_t queueHandle;
} SystemTaskListType;

typedef struct
{
	uint16 msgId;
	uint16 size;
	uint8 * parBufPtr;
} SystemTaskMsgInfoType;

typedef struct
{
	const char * const taskName;
	uint16 priority;
	uint16 stackSize;
	void (*taskMainFuncPtr)(void *);
	void * taskParameter;
	uint8 active;
	uint8 queueSize;
} SystemTaskConfigType;
	
extern void System_Task_Init(void);
extern void System_Task_Active(uint8 taskId);
extern void System_Task_Deactivate(uint8 taskId);
extern boolean System_Task_IsActive(uint8 taskId);
extern void System_Task_Delay(uint32 timeMs);
extern boolean System_Task_SendMessage(uint8 taskId, SystemTaskMsgInfoType* msgBufPtr, boolean fromISR);
extern boolean System_Task_ReceiveMessage(uint8 taskId, SystemTaskMsgInfoType* msgBufPtr, boolean fromISR, uint32 timeoutMs);
extern TaskHandle_t System_Task_GetHandle(uint8 taskId);
extern const sint8 * System_Task_GetName(uint8 taskId);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: System_Task.h ===========================*/
