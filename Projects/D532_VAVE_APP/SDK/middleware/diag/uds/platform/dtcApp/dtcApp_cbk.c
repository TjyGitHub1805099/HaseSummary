#include"diagCommon_if.h"
#include"dtcCore_if.h"
#include"dtcCore_cfg.h"
#include "Alarm.h"

#include"dtcApp_if.h"
#include"dtcApp_cfg.h"

#include"System_Task_Cfg.h"
#include "System_Signal.h"

#include"NvM.h"
#include"NvM_if.h"
#include"NvM_Cfg.h"
#include"NvM_Types.h"
#include"MemMap.h"
#include"string.h"
#include"trace.h"
#include"Com_If.h"
#include"FuelProc.h"
#include"i2c_mems.h"
#include"adc_wrapper_if.h"

#include"Task_M_IpcApp_Cfg.h" 
#include"Spi_Ipc_gw_api.h"
#include"diagAppIpc_cbk.h"
#include "amp_control_api.h"
#include "pmStateMachine.h"



extern tDtcAppCanFaultInfor * gDtcAppCanFaultInforPtr[];
extern uint16 gIcCanFaultBitFlagArray[];

extern uint16 *gDtcAppCanFaultBitArray[];
extern uint16 gDtcAppNvmBlockIdPtr[];
extern uint16 gIVICanFaultBitFlagArray[];
extern uint16 gIcCanFaultBitFlagArray[];
extern uint16 gAvmCanFaultBitFlagArray[];

extern uint8 gIVICanFaultRealtimeFlag[];
extern uint8 gIcCanFaultRealtimeFlag[];
extern uint8 gAvmCanFaultRealtimeFlag[];
extern uint8 *gDtcAppCanFaultBitRealtimeArray[];

extern boolean gCanMsgRxSTRGTimeoutFlag;

boolean gDiagMuxOnCurState = FALSE;
static boolean gVehicleRpmTimeoutFlag = TRUE;

static boolean gVehicleSpeedTimeoutFlag = TRUE;


uint8 gDtcAppCanFaultTotalNum[UDS_CH_MAX_NUM] = {0,0,0,0};
boolean gDtcAppTraceSwitchOn = FALSE;

uint8 gAvmCanMsgState[AVM_CAN_MSG_STATE_LEN] = {0,0,0,0};	//to SOC:0x57a,cfm flag,0x665,cfm flag

extern void diagAppWllrClear();
uint8 gIviCanMsgSemiFlag[IVI_FAULT_INDEX_MAX] = {0};
uint8 gIcCanMsgSemiFlag[IC_FAULT_INDEX_MAX] = {0};
uint8 gAvmCanMsgSemiFlag[AVM_FAULT_INDEX_MAX] = {0};


extern tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IVI[];
extern uint8 gIVI_PKI_CERTIFICATE[];
/****************************************************************************************************
* This function will be called by COM_layer
* to inform DTCAPP which msg was missed
*****************************************************************************************************/
void IVI_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag)
{

	if(semiFlag == FALSE)
	{
		if(timeoutFlag == TRUE)
			gIVICanFaultRealtimeFlag[ecuId] |= canMsgMask;
		else
		{
			gIVICanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
		}
	}
	else
	{
		if(timeoutFlag==FALSE)//if rx
			gIviCanMsgSemiFlag[ecuId] |= canMsgMask;//mark this ecu as Equipped,only redetected until power on again
		
		if(gIviCanMsgSemiFlag[ecuId] & canMsgMask)//with rx record
		{
			if(timeoutFlag == TRUE)
				gIVICanFaultRealtimeFlag[ecuId] |= canMsgMask;
			else
			{
				gIVICanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
			}
		}
		else//no rx record
			gIVICanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
	}
}


/****************************************************************************************************
* This function will be called by COM_layer
* to inform DTCAPP which msg was missed
*****************************************************************************************************/

void IC_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag)
{

	if(semiFlag == FALSE)
	{
		if(timeoutFlag == TRUE)
			gIcCanFaultRealtimeFlag[ecuId] |= canMsgMask;
		else
		{
			gIcCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
		}
	}
	else
	{
		if(timeoutFlag==FALSE)
			gIcCanMsgSemiFlag[ecuId] |= canMsgMask;//if rx,this means the ecu is equipped,only redetected until power on again
		
		if(gIcCanMsgSemiFlag[ecuId] & canMsgMask)//with rx record
		{
			if(timeoutFlag == TRUE)
				gIcCanFaultRealtimeFlag[ecuId] |= canMsgMask;
			else
			{
				gIcCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
			}
		}
		else//no rx record,always mark with no fault
			gIcCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
	}
}


/****************************************************************************************************
* This function will be called by COM_layer
* to inform DTCAPP which msg was missed
*****************************************************************************************************/

void AVM_dtcAppReportCanCommuFault(boolean timeoutFlag, uint16 ecuId,uint8 canMsgMask,boolean semiFlag)
{
	if(semiFlag == FALSE)
	{
		if(timeoutFlag == TRUE)
			gAvmCanFaultRealtimeFlag[ecuId] |= canMsgMask;
		else
		{
			gAvmCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
		}
	}
	else
	{
		if(timeoutFlag==FALSE)
			gAvmCanMsgSemiFlag[ecuId] |= canMsgMask;//if rx,this means the ecu is equipped,only redetected until power on again
		
		if(gAvmCanMsgSemiFlag[ecuId] & canMsgMask)//with rx record
		{
			if(timeoutFlag == TRUE)
				gAvmCanFaultRealtimeFlag[ecuId] |= canMsgMask;
			else
			{
				gAvmCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
			}
		}
		else//no rx record,always mark with no fault
			gAvmCanFaultRealtimeFlag[ecuId] &= ~canMsgMask;
	}
}



/****************************************************************************************************
* If lowVol and highVol about DTC are same as lowVol and highVol about PM, this func is unnecessary
****************************************************************************************************/
tDiagSysVoltageStatus diagCheckSysVoltageStatusForDtc()
{
	static boolean lowVol = FALSE;
	static boolean highVol = FALSE;
	static uint16 sysVoltage = 0;

	sysVoltage = 1200;
	diagGetSysVoltage(&sysVoltage);

	//1TODO:
	
	if(sysVoltage < DTC_LOW_VOLTAGE_THRESHOLD_VALUE)
		lowVol = TRUE;
	else if(sysVoltage >= DTC_LOW_VOLTAGE_FILTER_THRESHOLD_VALUE)
		lowVol = FALSE;	

	if(sysVoltage > DTC_HIGH_VOLTAGE_THRESHOLD_VALUE)
		highVol = TRUE;
	else if(sysVoltage <= DTC_HIGH_VOLTAGE_FILTER_THRESHOLD_VALUE)
		highVol = FALSE;

	if(lowVol== TRUE)
		return DTC_SYS_VOLTAGE_STATUS_LOW;
	else if(highVol == TRUE)
		return DTC_SYS_VOLTAGE_STATUS_HIGH;
	else
		return DTC_SYS_VOLTAGE_STATUS_NORMAL;
}


boolean dtcCheckVoltage()
{
	tDiagSysVoltageStatus sysVolStatus;
	
	sysVolStatus = diagCheckSysVoltageStatusForDtc();

	if(sysVolStatus == DTC_SYS_VOLTAGE_STATUS_LOW)
	{
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_VOLTAGE_TOO_LOW, DTC_PRE_ACTIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_POWER_INPUT_ERROR, DTC_PRE_ACTIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_VOLTAGE_BELOW_THRESHOLD_ERROR, DTC_PRE_ACTIVE, NULL_PTR, 0);
	}

	if(sysVolStatus == DTC_SYS_VOLTAGE_STATUS_HIGH)
	{
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_VOLTAGE_TOO_HIGH, DTC_PRE_ACTIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_POWER_INPUT_ERROR, DTC_PRE_ACTIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_VOLTAGE_ABOVE_THRESHOLD_ERROR, DTC_PRE_ACTIVE, NULL_PTR, 0);
	}

	if(sysVolStatus == DTC_SYS_VOLTAGE_STATUS_NORMAL)
	{
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_VOLTAGE_TOO_LOW, DTC_PRE_PASSIVE, NULL_PTR, 0);
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_VOLTAGE_TOO_HIGH, DTC_PRE_PASSIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_POWER_INPUT_ERROR, DTC_PRE_PASSIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_VOLTAGE_BELOW_THRESHOLD_ERROR, DTC_PRE_PASSIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_VOLTAGE_ABOVE_THRESHOLD_ERROR, DTC_PRE_PASSIVE, NULL_PTR, 0);
	}
	//else
		//dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_POWER_INPUT_ERROR, DTC_PRE_ACTIVE, NULL_PTR, 0);

}



#if 0
/****************************************************************************************************
*
*****************************************************************************************************/
void dtcAppRestartDetectDtc(uint8 *data, uint16 len)
{
	uint32 dtcGroup = 0;
	uint8 i = 0;
	

	if(len == 4)
	{
		dtcGroup = (uint32)(data[1]<<16|data[2]<<8|data[3]);
		if(dtcGroup == DTC_GROUP_ALL)//means all dtc
		{
			DIAG_TRACE_VALUE1(DIAGAPP_TRACE, TRACE_INFO, "restart to detect all dtc,T:%d\r\n", diagGetCurTick());
			for(i=0; i<gDtcGroupCbkCounter; i++)
				gDtcGroupCbkList[i].hook((uint8*)gDtcMsgMixInfor, sizeof(tDtcMsgMixInfor));
		}
		else
		{
			#if 1
			DIAG_TRACE_VALUE2(DIAGAPP_TRACE, TRACE_INFO, "restart to detect dtc grp:0x%x,T:%d\r\n", dtcGroup, diagGetCurTick());
			switch(dtcGroup)
			{
			case DTC_GROUP_BODY:
			case DTC_GROUP_CHASSIS:
			case DTC_GROUP_EMISSIONS_RELATED:
			case DTC_GROUP_NETWORK:
			case DTC_GROUP_POWERTRAIN:
			
				for(i=0; i<gDtcGroupCbkCounter; i++)
				{
					if(gDtcGroupCbkList[i].group == dtcGroup)
						gDtcGroupCbkList[i].hook((uint8*)gDtcMsgMixInfor, sizeof(tDtcMsgMixInfor));
				}
				break;
			
			default:
				// specify single DTC
				for(i=0; i<gDtcGroupCbkCounter; i++)
				{
					if(gDtcGroupCbkList[i].dtcId == dtcGroup)
					{
						gDtcGroupCbkList[i].hook((uint8*)gDtcMsgMixInfor, sizeof(tDtcMsgMixInfor));
						break;
					}
				}
			}
			#endif
		}
	}
}
#endif


boolean dtcApp_getVehicleSpeedTimeoutFlag()
{	
	return gVehicleSpeedTimeoutFlag;
}

boolean dtcApp_SetVehicleSpeedTimeoutFlag(boolean flag)
{	
	gVehicleSpeedTimeoutFlag = flag; 
}


boolean dtcApp_getVehicleRpmTimeoutFlag()
{	
	return gVehicleRpmTimeoutFlag;
}

void dtcApp_setVehicleRpmTimeoutFlag(boolean flag)
{
	gVehicleRpmTimeoutFlag = flag;
}

//CAUTION:
//DON'T ADD ANY TRACE IN THIS FUNCTION,
//THIS FUNCTION WILL BE CALLED BY CAN DRIVER DIRECTLY
void dtcAppReportBusOffState(boolean isBusOff)
{
	uint8 ch = 0;
	static boolean sBusOffFlag = FALSE;

	if(sBusOffFlag != isBusOff)
	{
		sBusOffFlag = isBusOff;
		for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
		{
			if(gDtcAppCanFaultBitRealtimeArray[ch]!=NULL)
				gDtcAppCanFaultBitRealtimeArray[ch][DTC_APP_FAULT_INDEX_CAN_BUS_OF] = isBusOff;
		}
	}
}


void dtcAppReDetect(uint8 *data,uint16 len)
{
	uint8 ch = 0;
	uint8 i = 0;
	uint32 curTick = 0;
	uint8 uReqData[1];
	
	DIAG_TRACE_TXT(DIAGAPP_TRACE,TRACE_WARNING, "dtcApp,re-detect\r\n");
	
	diagResetBuffer((uint8*)gIVICanFaultBitFlagArray,0,2*IVI_FAULT_INDEX_MAX);
	diagResetBuffer((uint8*)gIcCanFaultBitFlagArray,0,2*IC_FAULT_INDEX_MAX);
	diagResetBuffer((uint8*)gAvmCanFaultBitFlagArray,0,2*AVM_FAULT_INDEX_MAX);

	curTick = diagGetCurTick();
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
	{
		for(i=0; i<gDtcAppCanFaultTotalNum[ch]; i++)
		{
			if((ch==UDS_IVI && i == IVI_FAULT_INDEX_RX_CAN_MSG_VCM))
				continue;
			//gDtcAppCanFaultBitRealtimeArray[ch][i] = 0;
			gDtcAppCanFaultBitArray[ch][i] = 0x80;
			gDtcAppCanFaultInforPtr[ch][i].faultActive = TRUE;
			gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = TRUE;
			gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = FALSE;
			gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = FALSE;
			gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;
			gDtcAppCanFaultInforPtr[ch][i].preTick = curTick;
			diagSetNvmBlockId(gDtcAppNvmBlockIdPtr[ch], TRUE);
		}
	}


	//////////////////////////////////////////////
	//TODO:RE-TRIGGER OTHER FAULTS DETECTION EXCEPT CAN MSG

	//Trigger AVM to resend the fault status.
	uReqData[0] = 2;
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 1); //len = 1

	//////////////////////////////////////////////
	diagAppWllrClear();

}

#if 1

boolean dtcAppGetAvmMsgState(uint8 *status,uint8 len)
{
	boolean ret = FALSE;
	
	if(status && len== AVM_CAN_MSG_STATE_LEN)
	{
		diagCpyData(status,gAvmCanMsgState,len);
		ret = TRUE;
	}

	return ret;
}


void dtcAppReportAvmMsgToSoc(uint8 ignStatus,uint32 curSysTick)
{
	static uint8 avmMsgStatus[2] = {0,0};
	static boolean timerActiveFlag[2] = {0,0};
	static boolean timeoutFlag[2] = {0,0};
	static uint32 timerStartTick[2] = {0,0};
	static boolean txedByTimeoutFlag[2] = {0,0};
	static boolean txedByNewRx[2] = {0,0};
	static uint8 l_ignStatus = 0xFF;
	
	uint8 i = 0;

	if(l_ignStatus != ignStatus)
	{
		//reset all
		l_ignStatus = ignStatus;
		for(i=0;i<2;i++)
		{
			timerActiveFlag[i] = 0;
			timeoutFlag[i] = 0;
			txedByTimeoutFlag[i] = 0;
			txedByNewRx[i] = 0;
			gAvmCanMsgState[2*i] = 0;
			gAvmCanMsgState[2*i+1] = 0;
		}
	}


	if(ignStatus == DIAG_KL15_OFF)
		return;


	avmMsgStatus[0] = gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_BSW]&AVM_RX_CAN_MSG_MASK_BSW_0X57A ? 0:1;	// 0: lost, 1:rx
	//avmMsgStatus[1] = gDtcAppCanFaultBitRealtimeArray[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_BSW]&AVM_RX_CAN_MSG_MASK_BSW_0X665 ? 0:1;  //532 not support 665 check

	for(i=0;i<2;i++)
	{
		if(avmMsgStatus[i] == 0 )//lost
		{
			if(timerActiveFlag[i] == FALSE)
			{
				txedByNewRx[i] = FALSE;
				timerActiveFlag[i] = TRUE;
				timerStartTick[i] = curSysTick;
				timeoutFlag[i] = FALSE;
				txedByTimeoutFlag[i] = FALSE;
				gAvmCanMsgState[2*i] = avmMsgStatus[i];//cur status
				gAvmCanMsgState[2*i+1] = FALSE;	//not cfm
			}
			else
			{
				if(timeoutFlag[i] != TRUE)
				{
					if(curSysTick - timerStartTick[i] >= 1700)
					{
						timeoutFlag[i] = TRUE;
						txedByTimeoutFlag[i] = FALSE;
						gAvmCanMsgState[2*i+1] = TRUE;//cfm
					}
				}
			}
		}
		else
		{
			timerActiveFlag[i] = FALSE;
			timeoutFlag[i] = FALSE;
			gAvmCanMsgState[2*i] = avmMsgStatus[i];
			gAvmCanMsgState[2*i+1] = TRUE;
		}

		
	}

	for(i=0;i<2;i++)
	{
		if(timeoutFlag[i] == TRUE && txedByTimeoutFlag[i] == FALSE)
		{
			txedByTimeoutFlag[0] = TRUE;
			txedByTimeoutFlag[1] = TRUE;
			TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"1:AVM 0x57A:%d\r\n",avmMsgStatus[0]);
			ipc_send_notification(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CAN_MSG_STATE_RESP, gAvmCanMsgState, 4);
			break;
		}
		else if(txedByNewRx[i] == FALSE && avmMsgStatus[i] == 1)
		{
			txedByNewRx[i] = TRUE;
			TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"2:AVM 0x57A:%d\r\n",avmMsgStatus[0]);
			ipc_send_notification(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CAN_MSG_STATE_RESP, gAvmCanMsgState, 4);
		}
	}
	
			
}
#else

boolean dtcAppGetAvmMsgState(uint8 *status,uint8 len)
{
	boolean ret = FALSE;
	
	if(status && len== AVM_CAN_MSG_STATE_LEN)
	{
		memcpy(status,gAvmCanMsgState,len);
		ret = TRUE;
	}

	return ret;
}

void dtcAppReportAvmMsgToSoc(uint8 ignStatus,uint32 curSysTick)
{
	static uint8 avmMsgStableStatusCounter[2] = {0,0};
	static uint8 avmMsgStatus[2] = {0,0};
	static uint8 avmMsgPreStatus[2] = {0,0};
	static boolean avmMsgStateReverseFlag[2] = {0,0};	
	static boolean avmMsgStatusCfmFlag[2] = {0,0};
	static uint8 l_ignStatus = 0;
	
	uint8 i = 0;

	if(ignStatus == DIAG_KL15_OFF)
	{
		if(l_ignStatus != ignStatus)
		{
			l_ignStatus = ignStatus;
			for(i=0;i<2;i++)
			{
				avmMsgStableStatusCounter[i] = 0;
				avmMsgStatus[i] = 0;
				avmMsgPreStatus[i] = 0;
				avmMsgStateReverseFlag[i] = FALSE;
				avmMsgStatusCfmFlag[i] = FALSE;
				gAvmCanMsgState[2*i] = 0;
				gAvmCanMsgState[2*i-1] = 0;
			}
		}

		return;
	}

	avmMsgStatus[0] = gDtcAppCanFaultBitRealtimeArray[UDS_IVI][IVI_FAULT_INDEX_RX_CAN_MSG_BSW]&IVI_RX_CAN_MSG_MASK_BSW_0X57A ? 0:1;	// 0: lost, 1:rx
	avmMsgStatus[1] = gDtcAppCanFaultBitRealtimeArray[UDS_IVI][IVI_FAULT_INDEX_RX_CAN_MSG_BSW]&IVI_RX_CAN_MSG_MASK_BSW_0X665?0:1;

	for(i=0;i<2;i++)
	{
		if(avmMsgPreStatus[i] != avmMsgStatus[i])
		{
			avmMsgPreStatus[i] = avmMsgStatus[i];
			avmMsgStateReverseFlag[i] = TRUE;
			avmMsgStableStatusCounter[i] = curSysTick;
			avmMsgStatusCfmFlag[i] = FALSE;
			gAvmCanMsgState[2*i] = avmMsgStatus[i];
			gAvmCanMsgState[2*i-1] = FALSE;
			//TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"i:%d,tick:%d\r\n",i,curSysTick);
		}

		if(avmMsgStateReverseFlag[i] == TRUE)
		{
			if(avmMsgStatus[i] == 0)//lost
			{
				if(curSysTick - avmMsgStableStatusCounter[i] >= 1700)//300+1700= 2s
				{
					avmMsgStatusCfmFlag[i] = TRUE;//cfm lost
					gAvmCanMsgState[2*i-1] = TRUE;
					//TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"cfm,lost i:%d,tick:%d\r\n",i,curSysTick);
				}
			}
			else
			{
				avmMsgStatusCfmFlag[i] = TRUE;//cfm rx				
				gAvmCanMsgState[2*i-1] = TRUE;
				//TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"cfm rx i:%d,tick:%d\r\n",i,curSysTick);
			}
		}
	}
	
	if((avmMsgStateReverseFlag[0] == TRUE && avmMsgStatusCfmFlag[0] == TRUE)
		|| (avmMsgStateReverseFlag[1] == TRUE && avmMsgStatusCfmFlag[1] == TRUE))
	{
		avmMsgStateReverseFlag[0] = FALSE;
		avmMsgStateReverseFlag[1] = FALSE;
		TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"AVM 0x57A:%d,0x665:%d\r\n",avmMsgStatus[0],avmMsgStatus[1]);
		ipc_send_notification(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CAN_MSG_STATE_RESP, gAvmCanMsgState, 4);
	}
	
			
}

#endif


void dtcAppInit()
{
	uint8 i = 0;
	uint8 ch = 0;
	uint8 faultCounter = 0;
	
	dtcCoreRegAppTaskId(SYSTEM_TASK_ID_DIAGAPP);
	
	gDtcAppCanFaultTotalNum[UDS_IVI] = IVI_FAULT_INDEX_MAX;	//ivi
	gDtcAppCanFaultTotalNum[UDS_IC] = IC_FAULT_INDEX_MAX;	//ic
	gDtcAppCanFaultTotalNum[UDS_AVM] = AVM_FAULT_INDEX_MAX;	//avm 

	if(CanIf_GetDriverInitResult(0) == FALSE || CanIf_GetDriverInitResult(1) == FALSE)
	{
		dtcCoreSetDtcStatus(UDS_IVI,IVI_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_ACTIVE, NULL_PTR, 0);
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_ACTIVE, NULL_PTR, 0);
		dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_ACTIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_ACTIVE, NULL_PTR, 0);
	}
	else
	{
		dtcCoreSetDtcStatus(UDS_IVI,IVI_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_PASSIVE, NULL_PTR, 0);
		dtcCoreSetDtcStatus(UDS_IC,IC_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_PASSIVE, NULL_PTR, 0);
		dtcCoreSetDtcStatus(UDS_AVM,AVM_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_PASSIVE, NULL_PTR, 0);
		//dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CAN_CONTROLLER_ERR, DTC_PRE_PASSIVE, NULL_PTR, 0);
	}
	
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
	{
		for(i=0; i<gDtcAppCanFaultTotalNum[ch]; i++)
		{
			if(gDtcAppTraceSwitchOn == TRUE)
				TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"init:%x\r\n",gDtcAppCanFaultBitArray[ch][i]);
			if(i== DTC_APP_FAULT_INDEX_CAN_BUS_OF)
			{
				gDtcAppCanFaultBitRealtimeArray[ch][i] = 0;
				gDtcAppCanFaultBitArray[ch][i] &= 0xff7f; //default,no fault
				gDtcAppCanFaultInforPtr[ch][i].faultActive = FALSE;
				gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = FALSE;
			}
			else
			{
				if((ch==UDS_IVI && i == IVI_FAULT_INDEX_RX_CAN_MSG_VCM)
				||(ch==UDS_AVM && i == AVM_FAULT_INDEX_TX_MUTE)
				||(ch==UDS_IC && i == IC_FAULT_INDEX_RX_CAN_MSG_HVAC))
				{
					gDtcAppCanFaultBitRealtimeArray[ch][i] = 0;
					gDtcAppCanFaultBitArray[ch][i] = 0x00; //default,tx/rx with fault
					gDtcAppCanFaultInforPtr[ch][i].faultActive = FALSE;
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = FALSE;
				}
				else
				{
					gDtcAppCanFaultBitRealtimeArray[ch][i] = 1;
					gDtcAppCanFaultBitArray[ch][i] |= 0x80; //default,tx/rx with fault
					gDtcAppCanFaultInforPtr[ch][i].faultActive = TRUE;
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = TRUE;
				}
			}
			
			gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = FALSE;
			gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;
			gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = FALSE;
			
			if((gDtcAppCanFaultBitArray[ch][i]&DTC_APP_THIS_CYCLE_TEST_FAILED) == 0)//no fault in last IGN ON->IGN OFF
			{
				if((gDtcAppCanFaultInforPtr[ch][i].opt_cond_level & ECU_OPT_COND_LEVEL_3) == 0)//the other faults will  be check after IGN status reverses from off->on
				{
					//dec faultCounter
					faultCounter = (uint8)(gDtcAppCanFaultBitArray[ch][i]&0x3F);
					if( faultCounter>1)//in last last cycle,fault confirmed
					{
						gDtcAppCanFaultBitArray[ch][i] &= 0xFFC0;
						gDtcAppCanFaultBitArray[ch][i] |= (faultCounter-1);
						diagSetNvmBlockId(gDtcAppNvmBlockIdPtr[ch], TRUE);
					}
				}
			}
		}
	}
}


void dtcAppLoop()
{

	uint16 i = 0;
	uint16 k = 0;
	uint8 ch =0;
	static uint8 l_ignStableState = 0;
	uint8 l_ignCurState = 0;
	boolean ignStateReverseFlag = FALSE;
	uint8 l_accState = 0;
	static boolean firstBootFlag = TRUE;
	static uint32 firstBootTimer = 0;
	uint32 curTick = 0;
	static boolean sDiagMuxOnStableState = FALSE;
	uint32 l_busOffState = 0;
	uint32 l_sysVoltageState = 0;
	boolean canMsgErrFlag[UDS_CH_MAX_NUM] = {0};
	boolean curFaultStatus = FALSE;
	boolean preFaultStatus = FALSE;
	boolean atLeastOneTestDoneFlag[UDS_CH_MAX_NUM] = {0};
	//static uint32 diagMuxOnLostCounter = 0;
	uint16 l_u16Value = 0;
	boolean l_u8Value = 0;
	uint8 nvmStatus = 0;
	uint32 fuelSensorStatus = 0;
	uint32 curSysTick = 0;
	uint8 faultCounter = 0;
	static boolean spiIpcReadyFlag = FALSE;
	uint8 reqData[4];
	boolean diagMuxOnMsgRxTimeoutFlag = FALSE;
	static request_context_t ctx;

	static boolean delay1secondFlag = FALSE;
	static boolean delay50secondFlag = FALSE;
	static boolean checkLoudSpeakerFlag = TRUE;
	uint8 LoudSpeakerStatus[8] ={0};
	uint8 testIndex = 2;
	uint16 OATStatus = 0;
	uint32	SleepStatus = 0;

	#if 0
	if(spiIpcReadyFlag == FALSE)
	{
		if(GetM_IpcComStatus() == TRUE)
		{
			spiIpcReadyFlag = TRUE;
			ctx.function_id = AVM_IPC_PROG_DATE_REQ;
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "diagApp req AVM program date!\r\n");
			reqData[0] = 1;
			ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_PROG_DATE_REQ, reqData, 1, &ctx, diagApp_AVM_IpcResp);

			ctx.function_id = AVM_IPC_SWVERSION_REQ;
			reqData[0] = 1;
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "diagApp req AVM SW VERSION!\r\n");
			ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_SWVERSION_REQ, reqData, 1, &ctx, diagApp_AVM_IpcResp);
		}
	}
	#endif
	curSysTick = diagGetCurTick();
	if(firstBootFlag == TRUE)
	{
		firstBootFlag = FALSE;
		firstBootTimer = curSysTick;
		//TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"dtcApp set firstBoottimer\r\n");
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_ECU_SOC, DTC_PRE_PASSIVE, NULL, 0);

	}

	l_sysVoltageState = diagCheckSysVoltageStatus();
	
	
	diagGetAccStatus(&l_accState);
	diagGetKL15Status(&l_ignCurState);
	
	dtcCheckVoltage();

	if(delay1secondFlag == FALSE)
	{
		if(curSysTick >1000)
		{
			delay1secondFlag = TRUE;
		}
	}

	if(delay1secondFlag == TRUE)
	{
		if(l_sysVoltageState ==DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		{	
			dtcAppReportAvmMsgToSoc(l_ignCurState,curSysTick);
		}
	}
	
	if(curSysTick - firstBootTimer <DTC_APP_DELAY_TIMER)
		return;

	if(PM_Get_Current_Mode() == PM_MODE_ALL_OFF)
		return;
	

	ignStateReverseFlag = FALSE;
	if(l_ignStableState != l_ignCurState)
	{
		l_ignStableState = l_ignCurState;
		ignStateReverseFlag = TRUE;
	}
	
	diagMuxOnMsgRxTimeoutFlag = gDtcAppCanFaultBitRealtimeArray[UDS_IVI][IVI_FAULT_INDEX_RX_CAN_MSG_ECM]&IVI_RX_CAN_MSG_MASK_ECM_0X551?TRUE:FALSE;
	Com_RxSig_GetCANMASK(&gDiagMuxOnCurState);
	if(sDiagMuxOnStableState != gDiagMuxOnCurState)
	{
		if(gDtcAppTraceSwitchOn == TRUE)
			TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"curDiagMuxOn :%s\r\n",gDiagMuxOnCurState==TRUE?"ON":"OFF");
		if(gDiagMuxOnCurState == TRUE)
		{
			//if(gDtcAppTraceSwitchOn == TRUE)
				//TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"rx 551,reset A,D\r\n");
			//DiagMuxOn: if 0-->1, then reset all the fault state if node A and D
			for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
			{
				for(k=DTC_APP_RX_CAN_MSG_FAULT_START_INDEX; k<gDtcAppCanFaultTotalNum[ch]; k++)
				{
					if((gDtcAppCanFaultInforPtr[ch][k].ecu_category & (ECU_CATEGORY_A|ECU_CATEGORY_D))||
						( (ch==UDS_IVI && k == IVI_FAULT_INDEX_RX_CAN_MSG_ECM)
						|| (ch==UDS_IC && k == IC_FAULT_INDEX_RX_CAN_MSG_ECM)
						|| (ch==UDS_AVM && k == AVM_FAULT_INDEX_RX_CAN_MSG_HCM)))
					{
						//gDtcAppCanFaultBitRealtimeArray[ch][k] = 0xFF;
						//gDtcAppCanFaultBitArray[ch][k] &= 0xFF7F;//set bit 7 to 0
						gDtcAppCanFaultBitArray[ch][k] |= 0x80;//set bit 7 to 1
						gDtcAppCanFaultInforPtr[ch][k].faultActive = TRUE;
						gDtcAppCanFaultInforPtr[ch][k].JT1_timerActiveFlag = TRUE;
						gDtcAppCanFaultInforPtr[ch][k].JT1_timeoutFlag = FALSE;
						gDtcAppCanFaultInforPtr[ch][k].JT2_timerActiveFlag = FALSE;
						gDtcAppCanFaultInforPtr[ch][k].JT2_timeoutFlag = FALSE;
						gDtcAppCanFaultInforPtr[ch][k].preTick = diagGetCurTick();
					}
				}
			}
		}

		#if 0
		if(gDiagMuxOnCurState == TRUE)
		{
			//0x551 period is 100ms
			//diagMuxOnLostCounter++;
			//if(diagMuxOnLostCounter>=5)//if com report timeout[300ms], and dtcapp wait agin after 50ms, still lost,then update preState[sum time is 350ms]
				sDiagMuxOnStableState = gDiagMuxOnCurState;
		}
		else
		{
			sDiagMuxOnStableState = gDiagMuxOnCurState;
			diagMuxOnLostCounter = 0;
		}
		#else
		sDiagMuxOnStableState = gDiagMuxOnCurState;
		#endif
	}

	//check Can Fault
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &SleepStatus); //Check sleeping 2s count down status.

	
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
	{
		canMsgErrFlag[ch] = FALSE;
		atLeastOneTestDoneFlag[ch] = FALSE;
		for(i=DTC_APP_RX_CAN_MSG_FAULT_START_INDEX; i<gDtcAppCanFaultTotalNum[ch]; i++)
		{
			if( l_sysVoltageState!= DIAG_SYS_VOLTAGE_STATUS_NORMAL 
				&& (gDtcAppCanFaultInforPtr[ch][i].ecu_category & (ECU_CATEGORY_A|ECU_CATEGORY_D)) )
				continue;
			if(l_accState== DIAG_ACC_OFF && gDtcAppCanFaultInforPtr[ch][i].opt_cond_level & ECU_OPT_COND_LEVEL_2)
				continue;
			if((l_ignCurState== DIAG_KL15_OFF) && (gDtcAppCanFaultInforPtr[ch][i].opt_cond_level & ECU_OPT_COND_LEVEL_3))
				continue;
			if(gDtcAppCanFaultInforPtr[ch][i].referToDiagMuxOn == TRUE && sDiagMuxOnStableState == FALSE)
				continue;
			if(i==DTC_APP_FAULT_INDEX_CAN_TX_MUTE && (Com_GetStatus()&COM_TX_ENABLE)==0 )
				continue;

			if(ignStateReverseFlag == TRUE && l_ignCurState ==  DIAG_KL15_ON && (gDtcAppCanFaultInforPtr[ch][i].opt_cond_level & ECU_OPT_COND_LEVEL_3))
			{
				if((gDtcAppCanFaultBitArray[ch][i]&DTC_APP_THIS_CYCLE_TEST_FAILED) == 0)//no fault in last cycle
				{
					faultCounter = (uint8)(gDtcAppCanFaultBitArray[ch][i]&0x3F);
					if( faultCounter>1)//in last last cycle,fault confirmed
					{
						gDtcAppCanFaultBitArray[ch][i] &= 0xFFC0;
						gDtcAppCanFaultBitArray[ch][i] |= (faultCounter-1);
						diagSetNvmBlockId(gDtcAppNvmBlockIdPtr[ch], TRUE);
					}
				}
			}
			
			atLeastOneTestDoneFlag[ch] = TRUE;
			curFaultStatus = gDtcAppCanFaultBitRealtimeArray[ch][i]?TRUE:FALSE;
			preFaultStatus = gDtcAppCanFaultInforPtr[ch][i].faultActive;

			if(gDtcAppCanFaultInforPtr[ch][i].faultActive !=  curFaultStatus)
			{
				//if(ch==UDS_IC  && i==testIndex &&gDtcAppTraceSwitchOn == TRUE)
				if(gDtcAppTraceSwitchOn == TRUE)
					TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,Status:%d\r\n",ch,i,curFaultStatus);
				if(curFaultStatus == TRUE)
				{
					//if(ch==UDS_IC  && i==testIndex&& gDtcAppTraceSwitchOn == TRUE)
					if(gDtcAppTraceSwitchOn == TRUE)
						TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,ACTIVE JT1\r\n",ch,i);
					gDtcAppCanFaultInforPtr[ch][i].preTick = diagGetCurTick();
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = TRUE;
				}
				else
				{
					//if(ch==UDS_IC  && i==testIndex&& gDtcAppTraceSwitchOn == TRUE)
					if(gDtcAppTraceSwitchOn == TRUE)
						TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,reset JT1,JT2\r\n",ch,i);
					gDtcAppCanFaultBitArray[ch][i] &= 0x7F3F;//set bit 7 ,6 to 0,set bit23 to 0
					gDtcAppCanFaultInforPtr[ch][i].preTick = diagGetCurTick();
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = FALSE;
				}

				gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = FALSE;
				gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = FALSE;
				gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;

				gDtcAppCanFaultInforPtr[ch][i].faultActive = curFaultStatus;
			}
			
			
			if(gDtcAppCanFaultInforPtr[ch][i].faultActive == TRUE)//p_absent is true
			{
				canMsgErrFlag[ch] = TRUE;
				curTick = diagGetCurTick();
				#if 0
				if(gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag == TRUE
					&& gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag == FALSE)
				{
					if((curTick - gDtcAppCanFaultInforPtr[ch][i].preTick) >= gDtcAppCanFaultInforPtr[ch][i].JT1_timerValue)
					{
						if(ch==UDS_IC  && i==2&& gDtcAppTraceSwitchOn == TRUE)
							TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,JT1 timeout\r\n",ch,i);
						gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = TRUE;
						gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = TRUE;
						gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;
						gDtcAppCanFaultBitArray[ch][i] |= (DTC_APP_RX_CANMSG_TIMEOUT_BIT | DTC_APP_THIS_CYCLE_TEST_FAILED);
						dtcCoreSetDtcStatus(ch, DTC_CORE_CAN_FAULT_INDEX, DTC_PRE_ACTIVE, NULL,0);
					}
				}
				#else
				if(gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag == TRUE)
				{
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = FALSE;
					//if(ch==UDS_IC  && i==testIndex&& gDtcAppTraceSwitchOn == TRUE)
					if(gDtcAppTraceSwitchOn == TRUE)
						TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,JT1 timeout\r\n",ch,i);
					gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = TRUE;
					gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = TRUE;
					gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;
					gDtcAppCanFaultBitArray[ch][i] |= (DTC_APP_RX_CANMSG_TIMEOUT_BIT | DTC_APP_THIS_CYCLE_TEST_FAILED);
					dtcCoreSetDtcStatus(ch, DTC_CORE_CAN_FAULT_INDEX, DTC_PRE_ACTIVE, NULL,0);
				}

				#endif
					
				if(gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag == TRUE
					&& gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag == FALSE)
				{
					if((curTick - gDtcAppCanFaultInforPtr[ch][i].preTick) >= gDtcAppCanFaultInforPtr[ch][i].JT2_timerValue)
					{
						if(gDtcAppTraceSwitchOn == TRUE)
							TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,JT2 timeout\r\n",ch,i);
						gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = TRUE;
						gDtcAppCanFaultBitArray[ch][i] |= DTC_APP_RX_CANMSG_TIMEOUT_CFM_BIT;
						gDtcAppCanFaultBitArray[ch][i] &= 0xFFC0;	//reset fault counter to 0
						gDtcAppCanFaultBitArray[ch][i] |= VALMAX_VALUE;//reset fault counter to 40
	
						diagSetNvmBlockId(gDtcAppNvmBlockIdPtr[ch], TRUE);

					}
				}
				
			}
			#if 0
			else
			{
				if(preFaultStatus == TRUE)
				{
					if(gDtcAppTraceSwitchOn == TRUE)
						TracePrintf(DIAGAPP_TRACE,TRACE_WARNING,"ch:%d,i:%d,reset JT1,JT2\r\n",ch,i);
					gDtcAppCanFaultInforPtr[ch][i].JT1_timerActiveFlag = FALSE;
					gDtcAppCanFaultInforPtr[ch][i].JT1_timeoutFlag = FALSE;
					gDtcAppCanFaultInforPtr[ch][i].JT2_timerActiveFlag = FALSE;
					gDtcAppCanFaultInforPtr[ch][i].JT2_timeoutFlag = FALSE;
				}
			}
			#endif
			
		}
		
		if((atLeastOneTestDoneFlag[ch] == TRUE && canMsgErrFlag[ch] == FALSE) || (ALARM_STATE_RUNNING == SleepStatus))
		{
			if(gDtcAppCanFaultTotalNum[ch]>0)
				dtcCoreSetDtcStatus(ch, DTC_CORE_CAN_FAULT_INDEX, DTC_PRE_PASSIVE, NULL, 0);
		}

	}



#ifdef UDS_IC
	
	//if(sDiagMuxOnStableState == TRUE)// does here need this preconditon ????
	if(l_ignCurState ==  DIAG_KL15_ON)
	{		
		//speed meter
		if((gDtcAppCanFaultBitRealtimeArray[UDS_IC][IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC]&IC_RX_CAN_MSG_MASK_ABS_0X284) ==0)
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_SPEEDOMETER_NO_RESP, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_SPEEDOMETER_NO_RESP, DTC_PRE_ACTIVE, NULL, 0);

		l_u16Value = 0;
		Com_ReceiveSignal(SIG_HND_VSO, &l_u16Value);// CAN ID:284 from ABS
		if(l_u16Value != 0xFFFF)
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_SPEEDOMETER_ABNORMAL, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_SPEEDOMETER_ABNORMAL, DTC_PRE_ACTIVE, NULL, 0);

		//TACHO meter, RPM ?	// CAN ID:180 from ECM
		if((gDtcAppCanFaultBitRealtimeArray[UDS_IC][IC_FAULT_INDEX_RX_CAN_MSG_ECM]&IC_RX_CAN_MSG_MASK_ECM_0X180) ==0)
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_TACHO_METER_NO_RESP, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_TACHO_METER_NO_RESP, DTC_PRE_ACTIVE, NULL, 0);

		l_u16Value = 0;
		Com_ReceiveSignal(SIG_HND_TACHO, &l_u16Value); // CAN ID:180 from ECM
		if(l_u16Value != 0xFFFF)
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_TACHO_METER_ABNORMAL, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_TACHO_METER_ABNORMAL, DTC_PRE_ACTIVE, NULL, 0);



		if(diagMuxOnMsgRxTimeoutFlag == FALSE)// CAN ID:551 from ECM
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_NO_RESP, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_NO_RESP, DTC_PRE_ACTIVE, NULL, 0);

		l_u16Value = 0;
		Com_ReceiveSignal(SIG_HND_TWN, &l_u16Value);// CAN ID:551 from ECM
		if(l_u16Value != 0xFF)
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_ABNORMAL, DTC_PRE_PASSIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_ABNORMAL, DTC_PRE_ACTIVE, NULL, 0);

	}


	//RAM failure
	NvM_GetErrorStatus(TestIDNvMBlockID, &nvmStatus);
	if(nvmStatus == NVM_REQ_OK)
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_ROM_DATA_FAILURE, DTC_PRE_PASSIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_ROM_DATA_FAILURE, DTC_PRE_ACTIVE, NULL, 0);


	//check fuel sensor status
	fuelSensorStatus = FUEL_GetFuelSigStatus();

	if(fuelSensorStatus == FuelSigOpen)
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_FUEL_SENS_OPEN, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_FUEL_SENS_OPEN, DTC_PRE_PASSIVE, NULL, 0);

	if(fuelSensorStatus == FuelSigShort)
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_FUEL_SENS_SHORT, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_FUEL_SENS_SHORT, DTC_PRE_PASSIVE, NULL, 0);

#if 0    //not required in 532 according to the document update.
	//OUTSIDE TEMPERATURE SENSOR STATUS
	OATStatus = DEA_GetOAT_Valid();
	if(OATStatus == 0)
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_OAT_SENS_SHORT, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IC, IC_DTC_ID_INDEX_OAT_SENS_SHORT, DTC_PRE_PASSIVE, NULL, 0);
#endif

#endif


#ifdef UDS_IVI

	if(mems_i2c_getErrStatus() == TRUE)
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_ECU_GYRO, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_ECU_GYRO, DTC_PRE_PASSIVE, NULL, 0);

	//CONNECTION WITH SOC
	if(delay50secondFlag == FALSE)
	{
		if(curSysTick >60000)
		{
			delay50secondFlag = TRUE;
		}
	}

	if(delay50secondFlag == TRUE)
	{
		if(PM_Get_Current_Mode() != PM_MODE_ALL_OFF)
		{
			if(get_ipc_hb_with_ivi_status() == TRUE)
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_ECU_SOC, DTC_PRE_PASSIVE, NULL, 0);
			else
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_ECU_SOC, DTC_PRE_ACTIVE, NULL, 0);
		}
	}


	//CHECK LOADERSPEADER STATUS
	if(checkLoudSpeakerFlag)
	{
		if(api_amp_get_diag_result(LoudSpeakerStatus) == 0)
		{
			if(LoudSpeakerStatus[0] == DCLoadCondition_ShortSupply)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_PASSIVE, NULL, 0);
			}
			else if(LoudSpeakerStatus[0] == DCLoadCondition_ShortGround)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_PASSIVE, NULL, 0);
			}

			if(LoudSpeakerStatus[1] == DCLoadCondition_ShortSupply)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_PASSIVE, NULL, 0);
			}
			else if(LoudSpeakerStatus[1] == DCLoadCondition_ShortGround)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_PASSIVE, NULL, 0);
			}


			if(LoudSpeakerStatus[2] == DCLoadCondition_ShortSupply)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_PASSIVE, NULL, 0);
			}
			else if(LoudSpeakerStatus[2] == DCLoadCondition_ShortGround)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_PASSIVE, NULL, 0);
			}

			if(LoudSpeakerStatus[3] == DCLoadCondition_ShortSupply)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_PASSIVE, NULL, 0);
			}
			else if(LoudSpeakerStatus[3] == DCLoadCondition_ShortGround)
			{
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_GND, DTC_PRE_ACTIVE, NULL, 0);
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_BAT, DTC_PRE_PASSIVE, NULL, 0);
			}


			checkLoudSpeakerFlag = FALSE;
		}

	}

	//CHECK GPS ANTENNA STATUS:
	//SHORT TO GND, OPEN
	AdcWrapper_Read(ADC_LOGIC_CH_GPS_ANT_PWR, &l_u16Value);      //adc0, 10bit, 3.3V, tolerance is 0.05V. 
	if(l_u16Value < (uint16)(1024*0.05/3.3))
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_GPS_ANTENNA_OPEN, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_GPS_ANTENNA_OPEN, DTC_PRE_PASSIVE, NULL, 0);

	if(Port_GetPinLevel(ANT_PWR_ERR) == 0)
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_GPS_ANTENNA_SHORT_TO_GND, DTC_PRE_ACTIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_GPS_ANTENNA_SHORT_TO_GND, DTC_PRE_PASSIVE, NULL, 0);
	
	
	//TODO:
	//AIR CONDITIONER CONNECTION
	if(l_ignCurState ==  DIAG_KL15_ON)
	{
		if((gDtcAppCanFaultBitRealtimeArray[UDS_IVI][IVI_FAULT_INDEX_RX_CAN_MSG_HVAC]&IVI_RX_CAN_MSG_MASK_HVAC_0X5CB) != 0)
			dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_AIR_CONDITIONER_COM_FAILURE, DTC_PRE_ACTIVE, NULL, 0);
		else
			dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_AIR_CONDITIONER_COM_FAILURE, DTC_PRE_PASSIVE, NULL, 0);
	}

	//PKI certificate checking
	#if 0
	if(gIVI_PKI_CERTIFICATE[0] == 20)
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_PKI_CERTIFICATE_ERROR, DTC_PRE_PASSIVE, NULL, 0);
	else
		dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_PKI_CERTIFICATE_ERROR, DTC_PRE_ACTIVE, NULL, 0);
	#endif
	
#endif

	static uint8 preStateLWSOK = 0xff;
	static uint8 preStateLWSCAL = 0xff;

#ifdef UDS_AVM
	if(l_ignCurState ==  DIAG_KL15_ON)
	{
		#if 0
		if((gDtcAppCanFaultInforPtr[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_STRG].JT1_timerActiveFlag == FALSE)
		   && (gDtcAppCanFaultInforPtr[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_STRG].JT1_timeoutFlag == FALSE)
		   && (gDtcAppCanFaultInforPtr[UDS_AVM][AVM_FAULT_INDEX_RX_CAN_MSG_STRG].JT2_timeoutFlag == FALSE))
	   #else
		if(FALSE == gCanMsgRxSTRGTimeoutFlag)
	   #endif
		{
			l_u8Value = 0;
			Com_RxSig_GetLWSOK(&l_u8Value);
			if(l_u8Value != preStateLWSOK)
			{
				preStateLWSOK = l_u8Value;
				TracePrintf(UDS_TRACE,TRACE_WARNING,"LWSOK=%d\r\n",preStateLWSOK);
			}

			if(l_u8Value == 0)//abnormal
				dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_STRG_FAIL, DTC_PRE_ACTIVE, NULL, 0);
			else
				dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_STRG_FAIL, DTC_PRE_PASSIVE, NULL, 0);

			l_u8Value = 0;
			Com_RxSig_GetLWSCAL(&l_u8Value);

			if(l_u8Value != preStateLWSCAL)
			{
				preStateLWSCAL = l_u8Value;
				TracePrintf(UDS_TRACE,TRACE_WARNING,"LWSCAL=%d\r\n",preStateLWSCAL);
			}


			if(l_u8Value == 0)//abnormal
				dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_NEUTRAL_POINT_REGISTRATION_CAL_DISABALE, DTC_PRE_ACTIVE, NULL, 0);
			else
				dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_NEUTRAL_POINT_REGISTRATION_CAL_DISABALE, DTC_PRE_PASSIVE, NULL, 0);



		}
	}
#endif
}



