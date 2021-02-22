/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_Com.c
Author: Stephen Du
Version: V1.0
Timestamp: 2018-08-14 17:42:35
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
#include "Task_Com_Cfg.h"
#if (STD_ON == TASK_COM_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/
#include "Com_If.h"
#include "CanIf.h"
#include "CanApp_PduMsgProc.h"
#include "CanApp_TxProc.h"
#include "dte.h"
#include"CanApp.h"


/************ END: INCLUDE FILES *************/

void Task_Com(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
#if (STD_ON == TASK_COM_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};
	uint8 Can_BusOff;
	//uint8 value;
	//uint8 value1;
	//float32 value2;
	//uint16 value3;
	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	
	System_Signal_Subscribe(SYSTEM_SIG_ID_10MS, SYSTEM_TASK_ID_COM);
	System_Signal_Subscribe(SYSTEM_SIG_ID_SWC_PRESS, SYSTEM_TASK_ID_COM);
	System_Signal_Subscribe(SYSTEM_SIG_ID_SWC_REL, SYSTEM_TASK_ID_COM);
	System_Signal_Subscribe(SYSTEM_SIG_ID_SWC_HOLD, SYSTEM_TASK_ID_COM);
	System_Signal_Subscribe(SYSTEM_SIG_ID_SWC_HOLD_REL, SYSTEM_TASK_ID_COM);
	System_Signal_Subscribe(SYSTEM_SIG_ID_CAN_SEND_ENABLE, SYSTEM_TASK_ID_COM);
	
	System_Signal_Subscribe(SYSTEM_SIG_ID_ECALL_SPK_STATUS, SYSTEM_TASK_ID_COM);
	
	//System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_COM, X);
	
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	SemiActiveCheckInit();
	//Com_TxStart(); //For Test
	Com_TxStop();
	CanApp_Init();
	CanApp_TxMsgInit();
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_COM, &l_msgInfo, FALSE, OS_WAITFOREVER);
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
			if (SYSTEM_SIG_ID_10MS == l_msgInfo.msgId)
			{
#if (STD_ON == TASK_COM_DEBUG_TRACE)
				TracePrintf(COM_TRACE, TRACE_INFO, "[Task_Com-I]:Some infomation\r\n");

#endif
/*!Warning!:Please do not change the order of these functions, let alone add other functions to them!*/
				Com_MainFunctionTx();
				Com_MainFunctionRx();
				CanApp_RxPduChanged();
				AllPduMsgProc();
				CanApp_Main();
				SemiActiveCheckProc();
				CanApp_TxMsgProc();
				//Com_MainFunctionTx();//ahead
/*!==================================END MAIN FUNC ================================================!*/
			}
			else if((SYSTEM_SIG_ID_SWC_REL == l_msgInfo.msgId)||(SYSTEM_SIG_ID_SWC_HOLD == l_msgInfo.msgId)
				||(SYSTEM_SIG_ID_SWC_HOLD_REL == l_msgInfo.msgId)||(SYSTEM_SIG_ID_SWC_PRESS == l_msgInfo.msgId))
			{
				Botton_Proc((uint8)(*l_msgInfo.parBufPtr), l_msgInfo.msgId);	//button cheaked
			}
			else if(SYSTEM_SIG_ID_CAN_SEND_ENABLE == l_msgInfo.msgId)
			{
				if (STD_ON == *(uint32*)(l_msgInfo.parBufPtr))
				{
					Com_TxStart();
				}
				else
				{
					Com_TxStop();
				}
			}
			else if(SYSTEM_SIG_ID_ECALL_SPK_STATUS == l_msgInfo.msgId)
			{
				CanApp_sendSpeakerReadysignal();
			}
			
			ShortPressRelease();	//short pressed release
			
			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_COM_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_COM_DEBUG_TRACE)
			TracePrintf(COM_TRACE, TRACE_WARNING, "[Task_Com-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_Com.c ===========================*/