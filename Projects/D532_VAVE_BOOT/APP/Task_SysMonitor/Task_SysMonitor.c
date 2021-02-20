/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_SysMonitor.c
Author: Stephen Du
Version: V1.0
Timestamp: 2018-08-14 10:21:02
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
#include "Task_SysMonitor_Cfg.h"
#if (STD_ON == TASK_SYSMONITOR_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/

/************ END: INCLUDE FILES *************/

void Task_SysMonitor(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
#if (STD_ON == TASK_SYSMONITOR_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};
	
	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	/*
	System_Signal_Subscribe(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_SYSMONITOR);
	System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_SYSMONITOR, X);
	*/
	
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_SYSMONITOR, &l_msgInfo, FALSE, OS_WAITFOREVER);
		if (E_OK == l_returnValue)
		{
			/*
			You can use: "l_msgInfo.msgId" to get the message id.
			You can use: "l_msgInfo.size" to get the message size.
			You can use: "l_msgInfo.parBufPtr[]" or "l_msgDataBuf[]"
			             to get the message content if the message size not equal 0.
			 */
			/*===============================================================================*/
			/*=============================== START: USER CODE ==============================*/
			/*===============================================================================*/
			/*if (SYSTEM_SIG_ID_XXX == l_msgInfo.msgId)
			{
#if (STD_ON == TASK_SYSMONITOR_DEBUG_TRACE)
				TracePrintf(SYSMONITOR_TRACE, TRACE_INFO, "[Task_SysMonitor-I]:Some infomation\r\n");
#endif
			}*/

			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_SYSMONITOR_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_SYSMONITOR_DEBUG_TRACE)
			TracePrintf(SYSMONITOR_TRACE, TRACE_WARNING, "[Task_SysMonitor-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_SysMonitor.c ===========================*/
