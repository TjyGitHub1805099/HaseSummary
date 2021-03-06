/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_DiagApp.c
Author: Stephen Du
Version: V1.0
Timestamp: 2019-01-03 16:47:37
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
#include "Task_DiagApp_Cfg.h"
#include "diagCommon_cfg.h"
#if (STD_ON == TASK_DIAGAPP_DEBUG_TRACE)
#include "trace.h"
#endif
/*********** START: INCLUDE FILES ************/
#include"dtcApp_if.h"
#include"BoardDefines.h"
/************ END: INCLUDE FILES *************/
uint8 gDiagAppRxMsgBuf[100] = {0};
static uint32 gDiagAppRxTick[UDS_CH_MAX_NUM] = {0};
static boolean gDiagReqSocMFGFlag = FALSE;


//CPU freq:112M
#define MAX_PERFORMANCE_1S   526654 //652839

//CPU freq:80M
//#define MAX_PERFORMANCE_1S   387100 //480900
extern uint32 ulIdleCycleCount;
extern uint8 gCalCpuPayloadSwitch;

uint32_t cpu_rate = 0;
uint32_t cpu_rate_Sum = 0;
uint32_t cpu_rate_Cnt = 0;
uint32_t cpu_rate_x = 0;
void vCPUStatistics(void)
{
	uint32 curCount = 0;

	curCount = ulIdleCycleCount;
	ulIdleCycleCount = 0;
	cpu_rate = 1000 - curCount * 1000 / MAX_PERFORMANCE_1S;

	cpu_rate_Sum += cpu_rate;
	cpu_rate_Cnt++;
	if(cpu_rate_Sum > 0x7FFFFFFF)
	{
		cpu_rate_Sum /= 2;
		cpu_rate_Cnt /= 2;
	}
	cpu_rate_x = cpu_rate_Sum/ cpu_rate_Cnt;
	//TracePrintf(0,0,0,0, "[CPULoad Usage]: %d%\n\r", 100 - curCount * 100 / MAX_PERFORMANCE_1S); // 1s
}


uint32 diagApp_getRxTick(uint8 ch)
{
	return gDiagAppRxTick[ch];
}

void diagApp_setRxTick(uint8 ch, uint32 tick)
{
	gDiagAppRxTick[ch] = tick;
}

void Task_DiagApp(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
	uint8 *data = NULL;
	uint8 s_cnt = 0;
	uint8 socMfgTimerCounter = 0;
#if (STD_ON == TASK_DIAGAPP_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	
	
	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &gDiagAppRxMsgBuf[0];
	
	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	
	System_Signal_Subscribe(SYSTEM_SIG_ID_DIAGCMD, SYSTEM_TASK_ID_DIAGAPP);  //diag command need to request SOC
	System_Signal_Subscribe(SYSTEM_SIG_ID_13MS, SYSTEM_TASK_ID_DIAGAPP);
	//System_Signal_Subscribe(SYSTEM_SIG_ID_DIAG_TRIGGER_SOC_MFG, SYSTEM_TASK_ID_DIAGAPP);
	//System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_DIAGAPP, X);
	
	
	/***************************** END: SIGNAL SUBSCRIBE *****************************/
	/*===============================================================================*/
	/*================================ END: USER CODE ===============================*/
	/*===============================================================================*/
	while (1)
	{
		l_returnValue = System_Task_ReceiveMessage(SYSTEM_TASK_ID_DIAGAPP, &l_msgInfo, FALSE, OS_WAITFOREVER);
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
#if (STD_ON == TASK_DIAGAPP_DEBUG_TRACE)
			TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "[Task_DiagApp-I]:Some infomation\r\n");
#endif

			switch(l_msgInfo.msgId)
			{
				/*
				case SYSTEM_SIG_ID_DIAG_TRIGGER_SOC_MFG:
					gDiagReqSocMFGFlag = TRUE;
					break;
					*/
				case SYSTEM_SIG_ID_13MS:
					if(gDiagReqSocMFGFlag== TRUE)
					{
						socMfgTimerCounter++;
						if(socMfgTimerCounter>=50)//650ms
						{
							socMfgTimerCounter = 0;
							gDiagReqSocMFGFlag = FALSE;
							Port_SetOutLevel(IOC_BOOT_MODE,PORT_HIGH);//SOC MFG MODE
							//power on SOC
							Soc_Power_Onoff(STD_ON);
						}
					}
					
					if(gCalCpuPayloadSwitch != 0)
					{
						if(s_cnt < 100)
						{
							s_cnt++;
						}
						else
						{
							s_cnt = 0;
							vCPUStatistics();
							//TracePrintf(TRACE_TRACE,TRACE_INFO,"%d\r\n",cpu_rate_x);
						}
					}
					
					dtcAppLoop();
					wllrMainLoop();
					ameParseNewMsg();
					break;
				case SYSTEM_SIG_ID_DIAGCMD:
					/*
					* data[0]:ch
					* data[1]~data[4]:rxTick
					* data[5]:sid
					* data[6]~rest:other datas
					*/

					data = l_msgInfo.parBufPtr;
					uint8 ch = data[0];
					if(l_msgInfo.size>=5)
					{
						if(ch <UDS_CH_MAX_NUM)
							gDiagAppRxTick[ch] = (uint32)(data[1]<<24|data[2]<<16|data[3]<<8|data[4]);

						switch(l_msgInfo.parBufPtr[5])
						{
							case SID_CLEAR_DTC:
								 dtcAppReDetect(l_msgInfo.parBufPtr,l_msgInfo.size);
								 break;
							case SID_READ_DATA_BY_DID:
								 diagApp0x22ReqHandle(l_msgInfo.parBufPtr, l_msgInfo.size);
								 break;
							case SID_IOCONTROL:
								 diagApp0x2fReqHandle(l_msgInfo.parBufPtr, l_msgInfo.size);
								 break;
							default:
								 break;
						}
					}

					break;
				default:
					break;
			}
			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_DIAGAPP_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_DIAGAPP_DEBUG_TRACE)
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "[Task_DiagApp-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
		}
	}
}
/*=========================== END OF FILE: Task_DiagApp.c ===========================*/
