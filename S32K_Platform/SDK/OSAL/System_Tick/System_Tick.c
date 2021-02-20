/********************************************************************
File name: System_Tick.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-04-05 11:59:28
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
#include "System_Task.h"
#include "System_Signal.h"
#include "Std_Types.h"

uint32 g_ticks = 0;

extern SystemTaskListType SystemTaskList[];


/* A tick hook is used by the "Full" build configuration.  The Full and blinky
build configurations share a FreeRTOSConfig.h header file, so this simple build
configuration also has to define a tick hook - even though it does not actually
use it for anything. */
void vApplicationTickHook(void)
{
	BaseType_t ret = errQUEUE_FULL;
	static uint32 l_count = 1;
	SystemTaskMsgInfoType l_message;

	l_message.msgId = SYSTEM_SIG_ID_1MS;
	l_message.size = 0; //Warning! never ref this in your code, you should set the real size value.
	l_message.parBufPtr = (uint8*)l_count;

	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		g_ticks++;
		//ret = System_Task_SendMessage(SYSTEM_TASK_ID_ALARM, &l_message, TRUE);

		ret = xQueueSendToBackFromISR(SystemTaskList[SYSTEM_TASK_ID_ALARM].queueHandle, &l_message, NULL);
		
	if (pdTRUE != ret)
	{
		l_count++;
	}
	else
	{
		l_count = 1;
	}
}

}

//this will be removed,please use System_GetTickValue
Std_ReturnType GetCounterValue(uint8 CounterID,uint32* Value)
{
	CounterID = CounterID;
	*Value = g_ticks;
	return E_OK;
}

Std_ReturnType GetElapsedCounterValue(uint8 CounterID,uint32* Value,uint32* ElapsedValue)
{
	CounterID = CounterID;

	*ElapsedValue = (g_ticks - *Value);
	*Value = g_ticks;
	return E_OK;
}

uint32 System_GetTickValue(void)
{
	return(g_ticks);
}

/*=========================== END OF FILE: System_Tick.c ===========================*/

