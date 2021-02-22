/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_SWDL.c
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
#include "Task_SWDL_Cfg.h"
#if (STD_ON == TASK_SWDL_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/
#include "Eeprom.h"
#include "flash_Cfg.h"
#include "download.h"
//#include "BoardDefines.h"
//#include "pin_mux.h"

/************ END: INCLUDE FILES *************/

void Task_SWDL(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
#if (STD_ON == TASK_SWDL_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};
	boolean firstFlag = FALSE;
	uint8 counter = 0;
	
	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	/*
	System_Signal_Subscribe(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_SWDL);
	System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_SWDL, X);
	*/
	System_Signal_Subscribe(SYSTEM_SIG_ID_5MS, SYSTEM_TASK_ID_SWDL);
	TraceRxTrigger();
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_SWDL, &l_msgInfo, FALSE, OS_WAITFOREVER);
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
			if (SYSTEM_SIG_ID_5MS == l_msgInfo.msgId)
			{
#if (STD_ON == TASK_SWDL_DEBUG_TRACE)
				//TracePrintf(SWDL_TRACE, TRACE_INFO, "[Task_SWDL-I]:Some infomation\r\n");
#endif
				if(firstFlag == FALSE)
				{
					firstFlag = TRUE;
					TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: OS UP DONE\r\n");
				}
				Download_Loop();

				TraceLoopRxByTask();
			}

			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_SWDL_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_SWDL_DEBUG_TRACE)
			TracePrintf(SWDL_TRACE, TRACE_WARNING, "[Task_SWDL-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_SWDL.c ===========================*/