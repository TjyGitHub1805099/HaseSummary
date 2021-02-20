/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_General.c
Author: Stephen Du
Version: V1.0
Timestamp: 2018-01-09 09:16:33
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
#include "Task_General_Cfg.h"
#include "adc_wrapper_if.h"
#include "battVoltage.h"
#include "adc_pal.h"
//#if (STD_ON == TASK_GENERAL_DEBUG_TRACE)
#include "trace.h"
//#endif
/*********** START: INCLUDE FILES ************/

/************ END: INCLUDE FILES *************/

void Task_General(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
	uint16 adc_ampTemp = 0;
	uint16 adc_ubatVol = 0;
	uint16 adc_sysVol = 0;
#if (STD_ON == TASK_GENERAL_STACK_MONITOR)
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
	
	System_Signal_Subscribe(SYSTEM_SIG_ID_10MS, SYSTEM_TASK_ID_GENERAL);
	//System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_GENERAL, X);
	
	
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_GENERAL, &l_msgInfo, FALSE, OS_WAITFOREVER);
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
				if(AdcWrapper_ConvLoop(0) == FALSE)
					continue;
				AdcWrapper_Read(0,ADC_CH_BOARD_TEMPERATURE,&adc_ubatVol);
				AdcWrapper_Read(0,ADC_CH_AMP_TEMPERATURE,&adc_ampTemp);
				BattVolt_Scan();//check System Voltage

				//TracePrintf(GENERAL_TRACE,TRACE_INFO,"INFOR:ADC:ubatVol:%d,ampTemp:%d,sysVol:%d\r\n",adc_ubatVol,adc_ampTemp,adc_sysVol);
				//TracePrintf(GENERAL_TRACE,TRACE_INFO,"INFOR:[Task_General-I]\r\n");
				//TracePrintf(GENERAL_TRACE,TRACE_WARNING,"WARNING:[Task_General-I]\r\n");
				#if (STD_ON == TASK_GENERAL_DEBUG_TRACE)
					TracePrintf(GENERAL_TRACE,TRACE_INFO, "[Task_General-I]:TRACE,failPushByte:%d,failTxBytes:%d\r\n",TraceGetFailPushBytes(),TraceGetFailTxBytes());
				#endif
			}

			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_GENERAL_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_GENERAL_DEBUG_TRACE)
			TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Task_General-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_General.c ===========================*/
