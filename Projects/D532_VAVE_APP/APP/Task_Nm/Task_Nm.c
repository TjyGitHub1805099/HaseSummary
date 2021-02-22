/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_Nm.c
Author: Stephen Du
Version: V1.0
Timestamp: 2018-09-06 16:36:07
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
#include "Task_Nm_Cfg.h"
#if (STD_ON == TASK_NM_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/
#include "System_Tick.h"
#include "Can_Nm.h"
#include "AC_CTL.h"
#include "Gps_Parse.h"
#include "Eeprom.h"
/************ END: INCLUDE FILES *************/
void Task_Nm(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
#if (STD_ON == TASK_NM_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};
	uint32 counter = 1;
	/****************************** END: VARIABLE DEFINE *****************************/
	uint8 buff_rn[16];
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	static uint32 sNMTickcounter = 1;
	static uint32 sNMTickValu[2] = {0,0} ;
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/

	TraceRegisterCbk(NM_TRACE,CanNm_TestByTrace);

	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	/*
	System_Signal_Subscribe(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_NM);
	System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_NM, X);
	*/
	System_Signal_Subscribe(SYSTEM_SIG_ID_10MS, SYSTEM_TASK_ID_NM);
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_NM, &l_msgInfo, FALSE, OS_WAITFOREVER);
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
				CanNm_MainFunction(0);
				
				IC_WakeUpFrameCtrl();
#if 0
				ExtrRtc_MainFunction();
#endif

#if 0
				#if (STD_ON == TASK_NM_DEBUG_TRACE)
					if( counter++ % 100 == 0 )
					{
						//Eep_Read(4096,buff_rn,16);
						//TracePrintf(NM_TRACE, TRACE_WARNING, "[Task_Nm-I]:%d\r\n",counter/100);
						//CanNm_SendWakeUpFrame();
					}
				#endif

				//const sint8 * buff="$GBGSV,3,1,09,402,38,230,,403,54,193,29,404,32,119,,405,17,251,*63\r\n$GNRMC,030951.00,A,3144.95902,N,11714.15143,E,0.000,273.21,280618,,,A*7B\r\n" ;
				//Gps_InfoAnalysis(buff,strlen(buff));

				#if (STD_ON == TASK_NM_DEBUG_TRACE)
					sNMTickcounter++;
					sNMTickValu[0] = System_GetTickValue();
					if(( sNMTickValu[0] - sNMTickValu[1])>=20)
					{
						//TracePrintf(NM_TRACE, TRACE_WARNING, "[NM-CYCLE-ERR]:%d %d\r\n",sNMTickcounter/100,( sNMTickValu[0] - sNMTickValu[1]));
					}
					sNMTickValu[1] = sNMTickValu[0];
				#endif
#endif
			}

			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_NM_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_NM_DEBUG_TRACE)
			TracePrintf(NM_TRACE, TRACE_WARNING, "[Task_Nm-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_Nm.c ===========================*/