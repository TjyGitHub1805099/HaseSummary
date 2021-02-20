/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_General.c
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
#include "Task_General_Cfg.h"
#include "adc_wrapper_if.h"
#include "BattVoltage.h"
#include "adc_pal.h"
//#if (STD_ON == TASK_GENERAL_DEBUG_TRACE)
#include "trace.h"
//#endif
#include "BinSig_Acc_If.h"
/*********** START: INCLUDE FILES ************/
#include "Eeprom.h"
#include "Memif.h"
#include "Ea_if.h" 
#include "NvM_if.h"

#include "Key.h"
//#include "BoardDefines.h"
#include "PowerSupply.h"
#include "BinSig_Tcu_Acc_If.h"
#include "BinSig_Acc_If.h"
#include "BinSig_Ign_If.h"
#include "BinSig_Reverse_If.h"
#include "pins_driver.h"
#include "system_S32K148.h"
#include "pin_wrapper_if.h"
#include "download_if.h"
#include "WaterTempWarn.h"
#include "SpeedOutput.h"
#include "dtcCore_if.h"
#include "ExternRTC.h"
#include "AC_CTL.h"
#include "pmStateMachine_If.h"
#include "Ex_Wdg.h"
#include "ipc_hb_with_ivi_if.h"
#include "CanIf.h"
#include "CanIf_Cfg.h"

#include "TraceStore.h"



/************ END: INCLUDE FILES *************/
extern uint8 gCurrentDuty;
extern uint8 gPreDuty;
extern boolean gSystemInitDoneFlag;
extern uint8 TestID;
extern boolean gIgn_Off_UpdateInProgressFlag;
extern boolean gRunInMainFlag;

extern const CanIf_RxPduCfgType CanIf_RxPduCfg_0[];
extern uint8 * const Com_IpduBufPtr[];

uint16 gSocReqResetSysReason = 0xFFFF;
uint8 buff_w[10]={77,2,3,4,5,6,7,8,9,10};
uint8 buff_r[10]={0};

uint8 l_BattVoltageTrace = FALSE ;
uint8 g_PowerModingTrace = FALSE ;


//len = strlen(string)
boolean convertStr2Num(uint8* string,uint8 len,sint32 *outputNum)
{
	uint8 j = 0;
	sint8 num = 0;
	sint32 cmbNum = 0;
	boolean validNumFlag = TRUE;
	uint8 str = 0;
	boolean negativeNum = FALSE;
	boolean hexFlag = FALSE;
	
	if(string == NULL || len<1 || len>8 || outputNum == NULL)
		validNumFlag = FALSE;
	else
	{
		if(j==0 && string[j] == '-')
		{
			negativeNum = TRUE;
			j = 1;
		}
		
		if(len>2)
		{
			if(string[j] == '0' &&(
				string[j+1] == 'x'||string[j+1] == 'X'))
			{
				hexFlag = TRUE;
				j+=2;
			}
		}
		
		for(;j<len;j++)
		{

			if(hexFlag == TRUE)
			{
				cmbNum <<= 4;
				str = string[j];
				if(str >= '0' && str <= '9')
					num = str-'0';
				else if(str >= 'a' && str <= 'f')
					num = str-87;
				else if(str >= 'A' && str <= 'F')
					num = str-55;
				else
				{
					validNumFlag = FALSE;
					break;
				}
				cmbNum+= num;
			}
			else
			{
				cmbNum *= 10;
				str = string[j];
				if(str >= '0' && str <= '9')
					num = str-'0';
				else
				{
					validNumFlag = FALSE;
					break;
				}
				cmbNum+= num;
			}
		}
	}

	if(validNumFlag == TRUE)
	{
		if(negativeNum == TRUE)
			cmbNum *= -1;

		*outputNum = cmbNum;
	}
	return validNumFlag;
}


void TestSetUpdateSysFlag()
{
	Std_ReturnType ret = E_NOT_OK;
	//static uint32 updateFlag = USB_UPDATE_ENABLE_FLAG;
	//ret = Eep_Write(0, (uint8 *)&updateFlag, sizeof(updateFlag));
	//if(ret == E_OK)
	//{
		//SysResetWrapper(GENERAL_TEST);
		SocReqResetSys(NTF_UPDATE);
	//}
}

void NvmTestCbk(tStrArrayPtr strArrayPtr, uint8 arrayNum)
{

	//static uint16 freq = 2;
	//uint8_t  l_strArrayPtr[2] ={0};
	uint16_t temp = 0u;
	uint8 i = 0;
	uint8 dataLen = 0;
	sint32 outputNum = 0;
	uint16 pduId = 0;
	uint16 k = 0;
	sint32 canId = 0;
	uint16 volValue = 0;

	#if 1
	switch(strArrayPtr[0][0])
	{
	
	case '0':
		//waterTempWarnOutput(NORMAL);
		//Eep_Write(16373,buff_w,10);
		break;
	case '1':
		//NvM_WriteBlock(2, buff_w);
		NvM_WriteBlock(2, &TestID);
	   // NvM_MainFunction();
		//Eep_Write(4096,buff_w,10);
		//waterTempWarnOutput(WARNING_A);
		break;
	case '2':
		//NvM_ReadBlock(2, buff_r);
		NvM_ReadBlock(2, &TestID);
		//NvM_MainFunction();
		TracePrintf(TRACE_TRACE,0,"TestID = %d \r\n",TestID);
		//NvM_ReadBlock(2, NULL);
		//Eep_Read(16373,buff_r,10);
		//waterTempWarnOutput(WARNING_B);
		break;
	case '3':
		TestID++;
		NvM_SetRamBlockStatus(TestIDNvMBlockID, STD_ON);
		waterTempWarnOutput(WARNING_C);
		break;
	case '4':
		NvM_ResetForWriteAll();
		NvM_WriteAll();
		//NvM_MainFunction();
		break;
	case '5':
		//NvM_ReadAll();
		PM_AMP_OnOff(STD_ON);
		break;
	case '6':
		//SetUpdateSysFlag();
		PM_AMP_OnOff(STD_OFF);
		break;
	case '7':
		Soc_Power_Onoff(STD_OFF);
		break;

	case '8':
		Soc_Power_Onoff(STD_ON);
		break;

	case '9':
		if(arrayNum == 4)
		{
			#define NUM_OFF_SET_START 48
			#define NUM_OFF_SET_END 57
			uint8 ch = strArrayPtr[1][0]-NUM_OFF_SET_START;
			uint8 dtcInnertIndex = 0;
			uint8 dtcIndexStrLeN = strlen(strArrayPtr[2]);
			uint8 i = 0;
			boolean activeFlag = (strArrayPtr[3][0]-NUM_OFF_SET_START)==1?DTC_PRE_PASSIVE:DTC_PRE_ACTIVE;
			
			if(ch >=0 && ch<=9)
			{
				for(i=0;i<dtcIndexStrLeN;i++)
					if(strArrayPtr[2][i]>=NUM_OFF_SET_START && strArrayPtr[2][i]<=NUM_OFF_SET_END)
						dtcInnertIndex += (strArrayPtr[2][i]-NUM_OFF_SET_START)<<((dtcIndexStrLeN-1)*8);
				
				dtcCoreSetDtcStatus(ch,dtcInnertIndex,activeFlag,NULL,0);
			}
		}
		break;

	case 'a':
		TestSetUpdateSysFlag();
		break;
	case 'b':
		{
			//l_strArrayPtr[0] = atoi(strArrayPtr[0][1]);
			//l_strArrayPtr[1] = atoi(strArrayPtr[0][2]);
			//l_strArrayPtr[0] = tolower(strArrayPtr[0][1]);
			//l_strArrayPtr[1] = tolower(strArrayPtr[0][2]);
			//l_strArrayPtr[0] = strArrayPtr[0][1] - '0';
			//TracePrintf(TRACE_TRACE,0,"TestID = %d \r\n",l_strArrayPtr[0]);
			//temp = (l_strArrayPtr[0]<<8) + l_strArrayPtr[1];
			temp = str_to_hex(&strArrayPtr[0][1]);
			//TracePrintf(TRACE_TRACE,0,"TestID = %x \r\n",temp);
			Diag_DidDataTest(temp);
		}
	case 'c':
		if(( strArrayPtr[0][1] == '-')&&( strArrayPtr[0][2] == 'o'))
		{
			l_BattVoltageTrace = TRUE;
			g_PowerModingTrace = TRUE;
		}
		else
			l_BattVoltageTrace = FALSE;
		break;
	case 'd':
		CtiTest();
		break;
	case 'e':
		HisAfcDataTest();
		break;
	case 'f':
		
	#if 1
		
		for(i=1;i<arrayNum;i++)
		{
			dataLen = strlen(strArrayPtr[i]);
			if(dataLen>0)
			{
				if(convertStr2Num(strArrayPtr[i],dataLen,&outputNum) == TRUE)
					TracePrintf(GENERAL_TRACE,TRACE_WARNING,"[UartRxCbk]:num:%d\r\n",outputNum);
				else
					TracePrintf(GENERAL_TRACE,TRACE_WARNING,"[UartRxCbk]:string:%s\r\n",strArrayPtr[i]);
			}
			
		}
#endif
		break;
	case 'g':

		for(i=1;i<arrayNum;i++)
		{
			dataLen = strlen(strArrayPtr[i]);
			if(dataLen>0)
			{
				if(convertStr2Num(strArrayPtr[i],dataLen,&canId) == TRUE)
				{
					for(k=0;k<CANIF_RX_PDU_NUM_0;k++)
					{
						if(CanIf_RxPduCfg_0[k].id == canId)
						{
							pduId = CanIf_RxPduCfg_0[k].cbkId;

							TracePrintf(GENERAL_TRACE,TRACE_TRACE,"%x %x %x %x %x %x %x %x\r\n",
							Com_IpduBufPtr[pduId][0],Com_IpduBufPtr[pduId][1],Com_IpduBufPtr[pduId][2],Com_IpduBufPtr[pduId][3],
							Com_IpduBufPtr[pduId][4],Com_IpduBufPtr[pduId][5],Com_IpduBufPtr[pduId][6],Com_IpduBufPtr[pduId][7]
							);
						}    
					}
				}
			}
		}
  
    break;
	case 'h':
		BattVolt_GetRawValue(&volValue);
		TracePrintf(GENERAL_TRACE,TRACE_WARNING,"voltage value :%d\r\n",volValue);
		break;
	default:
		break;
	}
	#endif
}

void SocReqResetSys(uint16 reason)
{
	gSocReqResetSysReason = reason;
}

void Task_General(void * pvParameters)
{
	boolean l_returnValue = E_NOT_OK;
	SystemTaskMsgInfoType l_msgInfo = {0XFFFF, 0U, NULL};
	//uint16 adc_ampTemp = 0;
	//uint16 adc_ubatVol = 0;
	//uint16 adc_sysVol = 0;
	uint8 counter = 0;
	uint8 l_nvmCounter = 0;
	ExternSTDType stdData;
	
#if (STD_ON == TASK_GENERAL_STACK_MONITOR)
	uint16 l_stackSize = 0U;
#endif
	static uint16 batteryVolt[2]={0};
	uint8 can_reverse_status = 0;
	uint8 curReverseStatus = 0;
	uint8 stableReverseStatus = 0xFF;

	/*===============================================================================*/
	/*=============================== START: USER CODE ==============================*/
	/*===============================================================================*/
	/***************************** START: VARIABLE DEFINE ****************************/
	/*Warning: Change the following array size according to your real length.*/
	uint8 l_msgDataBuf[4] = {0U, 0U, 0U, 0U};

	/****************************** END: VARIABLE DEFINE *****************************/
	
	/********************* START: SOMETHING ONLY NEED EXEC ONCE **********************/
	l_msgInfo.parBufPtr = &l_msgDataBuf[0];
	
	TraceRegisterCbk(GENERAL_TRACE,NvmTestCbk);

	/********************** END: SOMETHING ONLY NEED EXEC ONCE ***********************/
	
	/**************************** START: SIGNAL SUBSCRIBE ****************************/
	
	System_Signal_Subscribe(SYSTEM_SIG_ID_10MS, SYSTEM_TASK_ID_GENERAL);
	//System_Signal_Subscribe(SYSTEM_SIG_ID_5MS, SYSTEM_TASK_ID_GENERAL);
	System_Signal_Subscribe(SYSTEM_SIG_ID_SOCUPDATARTC, SYSTEM_TASK_ID_GENERAL);
	System_Signal_Subscribe(SYSTEM_SIG_ID_BC_READRTC, SYSTEM_TASK_ID_GENERAL);
	//System_Signal_SubSpecValue(SYSTEM_SIG_ID_XXX, SYSTEM_TASK_ID_GENERAL, X);
	//Ea_Init();
	//NvM_Init();
	#if (STD_ON == TRACE_STORE_USING )
		TraceStore_Init();//add by tjy @2019-12-17
	#endif
	gRunInMainFlag = FALSE;
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
				batteryVolt[0]=BattVolt_Scan();//check System Voltage
				if((l_BattVoltageTrace == TRUE) && (( batteryVolt[0] > (batteryVolt[1]+10) )||( (batteryVolt[0]+10) < batteryVolt[1] )))//div 0.1V printf
				{
					batteryVolt[1] = batteryVolt[0];
					TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Batt-I]:volt = %d\r\n",batteryVolt[1]);
				}	

				//key scan
				KeyScanCyc();
				if(gIgn_Off_UpdateInProgressFlag == FALSE)
				{
					BinSig_Tcu_Acc_Scan();
					BinSig_Ign_Scan();
					BinSig_Acc_Scan();
					BinSig_Reverse_Scan();
				}
				//Ex_Wdg_Refresh();

				TraceLoopRxByTask();
				#if 1
				if(++l_nvmCounter == 10)
				{
					l_nvmCounter = 0;
					
					//liqi: DNTC req to clear all the DTC if update sys
					//when DTC module is ACTIVE, uncomment this block
					if(gSocReqResetSysReason == NTF_UPDATE)
					{
						dtcCoreSetMonitorFlag(FALSE);
						dtcCoreClearAll();
						gSystemInitDoneFlag = FALSE;
						vTaskSuspendAll();
						NvM_WriteAll();
					}

					//use mutex,avoid 'Re-enter' issue if Powermoding enter standby,
					Nvm_LockMainMutex();
					NvM_MainFunction();
					Nvm_UnlockMainMutex();

					if(gSocReqResetSysReason == NTF_UPDATE)//liqi:don't move this block to upper
						SysResetWrapper(NTF_UPDATE);
				}
				#endif
				if(gCurrentDuty != gPreDuty)
				{
					SetFanSpeed(gCurrentDuty);
					gPreDuty = gCurrentDuty;
				
				}
				//if(++counter > 10)
				//{
				//	counter = 0;
					//TracePrintf(GENERAL_TRACE,TRACE_WARNING,"INFOR:ADC:ubatVol:%d,ampTemp:%d\r\n",adc_ubatVol,adc_ampTemp);
					//TracePrintf(GENERAL_TRACE,TRACE_WARNING,"INFOR:[Task_General-I]\r\n");
				//}

				//==========================fast notification SOC open AVM:start===============================
				//add by tjy copy from N591 @2019-8-2
				can_reverse_status = 0;
				Com_RxSig_GetIND_RNG(&can_reverse_status);
				curReverseStatus = (PM_GetReverseStatus() == STD_ON || can_reverse_status == 2);
				if(stableReverseStatus!=curReverseStatus)
				{
					stableReverseStatus = curReverseStatus;
					if(stableReverseStatus==1)
					{
						TracePrintf(GENERAL_TRACE,TRACE_WARNING,"enable AVM_PIN\r\n");
						Port_SetOutLevel(IXM8,PORT_HIGH);
					}
					else
					{
						TracePrintf(GENERAL_TRACE,TRACE_WARNING,"disable AVM_PIN\r\n");
						Port_SetOutLevel(IXM8,PORT_LOW);
					}
				}
				//==========================fast notification SOC open AVM:end===============================

			}

			if(SYSTEM_SIG_ID_SOCUPDATARTC == l_msgInfo.msgId)
			{
				if(ExtrRtc_StdTimeGet(&stdData) == E_OK)
				{
					SysTimeUpdatSoc(stdData.year+2000, stdData.month, stdData.day, stdData.hour, stdData.minutes, stdData.second, 0);
				}
				else
				{
					//System_Signal_Send(SYSTEM_SIG_ID_SOCUPDATARTC, STD_ON, STD_OFF);//re trigger
				}
			}

			if(SYSTEM_SIG_ID_BC_READRTC == l_msgInfo.msgId)
			{
				if(ExtrRtc_StdTimeGet(&stdData) == E_OK)
				{
					//SysTimeUpdatSoc(stdData.year+2000, stdData.month, stdData.day, stdData.hour, stdData.minutes, stdData.second, 0);
					DEA_SetRtcTime(&stdData);
					OAT_SetRequestTimeUpdate(1);
				}
			}
			
			#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)
				ipc_hb_with_ivi();
			#endif

				#if (STD_ON == TRACE_STORE_USING )
					TraceStore_MainFunction(TRACESTORE_IPC_HW_CHANNEL,TRACESTORE_IPC_SW_CHANNEL);//add by tjy @2019-12-17
				#endif
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
