/*************************************************************************************************************
* author: liqi@hangsheng.com.cn
* 
*************************************************************************************************************/

#include"diagCommon.h"
#if 0






#include "diagAppSid0x31.h"
#include"gsc0x31.h"
#include"gsc0x22.h"
#include"gsc0x85.h"
#ifdef DIAG_OS_UCOS
#include"HwCrc.h"
#endif
#ifdef SW_PHASE_APPLICATION
#include"dtcCore.h"
#ifdef DIAG_OS_UCOS
#include"dea_diag.h"
#endif
#include"sysStandby.h"
#endif

#ifdef SW_PHASE_BOOTLOADER
#ifdef DIAG_OS_UCOS
#include"CodeFlash.h"
#include"swdl_flag.h"
#endif
#endif


static boolean diagRoutineCtrlDummyTest(uint8*data,uint16 len);


boolean gIpcAutoTestStarted = FALSE;
boolean gIpcDisplayTestIsStarted = FALSE;


boolean diagApp0x31StopAllTest(uint8*data, uint16 len)
{

	#if 0// dummy code
	#ifdef SW_PHASE_APPLICATION
	if(TRUE == gIpcAutoTestStarted)
	{
		gIpcAutoTestStarted = FALSE;
		//DEA_DiagIPCAutoTest(DEA_DIAG_EXIT,0);
		diagIPCAutoTest(DEA_DIAG_EXIT,0);
	}
	
	if(TRUE == gIpcDisplayTestIsStarted)
	{
		gIpcDisplayTestIsStarted = FALSE;
		//DEA_DiagIPCDisplayTest(DEA_DIAG_EXIT,0);
		diagIPCDisplayTest(DEA_DIAG_EXIT,0);
	}
	#endif
	#endif
	
	return TRUE;
}

boolean diagApp0x31ReqHandle(uint8*data, uint16 len)
{
	uint16 routineId = 0;
	tRoutineCtlType routineType;
	boolean ret = FALSE;
	#ifdef SW_PHASE_BOOTLOADER
	uint8 buffer[40];
	#else
	uint8 *buffer = data;
	#endif

	//data[0] : sid 0x31
	//data[1] :routineCtrlType
	//data[2] data[3]:ID
	//data[4] :option
	
	if(data == NULL || len<4)
	{
		sid0x31RespData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		return FALSE;
	}

	#ifdef SW_PHASE_BOOTLOADER
	if(len<=40)
		diagCpyData(buffer,data,len);
	else
	{
		sid0x31RespData(TRUE,NRC_INCORRECT_MESSAGE_LENGTH,NULL,0);
		return FALSE;
	}
	#endif
	
	routineType = buffer[1];
	routineId = (uint16)(buffer[2]<<8 | buffer[3]);
	
	switch(routineId)
	{
	case ROUTINE_CTL_DUMMY_REQ:
		ret = diagRoutineCtrlDummyTest(buffer,len);
		break;

	default:
		return FALSE;
	}
	
	return ret;
}

boolean diagRoutineCtrlDummyTest(uint8* data,uint16 len)
{
	uint8 respData[7];
	uint8 kl30_status = DIAG_KL30_START_CRANK;
	tRoutineCtlType routineCtrlType;
	boolean preConditionIsOk = TRUE;

	//data[0] : sid 0x31
	//data[1] :routineCtrlType
	//data[2] data[3]:ID
	//data[4] :option

	routineCtrlType = data[1];
	switch(routineCtrlType)
	{
	case ROUTINE_CTL_TYPE_START_ROUTINE:
		respData[0] = data[1];//routineCtrlType
		respData[1] = data[2];//routineId	high
		respData[2] = data[3];//routineId low

		//if(diagGetEngineRPM(&rpm) == TRUE && (rpm>0))
		if(diagCheckIfEngineIsRunning() == TRUE)
		{
			respData[3] = ENGINE_SPEED_IS_NOT_ZERO;
			preConditionIsOk = FALSE;
		}
		else
			respData[3] = ENGINE_SPEED_CONDITIONS_ARE_MET;

		if(diagCheckIfVehicleIsStatic() == FALSE)
		{
			respData[4] = VEHICLE_SPEED_IS_NOT_STANDING;
			preConditionIsOk = FALSE;
		}
		else
			respData[4] = VEHICLE_CONDITIONS_ARE_MET;

		diagGetKL30Status(&kl30_status);
		if(kl30_status==DIAG_KL30_START_CRANK)
		{
			respData[5] = IGNITION_IS_IN_START_POSITION;
			preConditionIsOk = FALSE;
		}
		else
			respData[5] = IGNITION_CONDTIONS_ARE_MET;

		if(diagCheckSysVoltageStatus() == DIAG_SYS_VOLTAGE_STATUS_NORMAL)
			respData[6] = VOLTAGE_CONDITIONS_ARE_MET;
		else
		{
			respData[6] = UNDER_OR_ABOVE_PROGRAMMING_VOLTAGE;
			preConditionIsOk = FALSE;
		}
		
		sid0x31RespData(TRUE,NRC_NONE,respData,7);
		break;
	//case ROUTINE_CTL_TYPE_REQ_ROUTINE_RESULTS:
	//case ROUTINE_CTL_TYPE_STOP_ROUTINE:
	default:
		sid0x31RespData(TRUE,NRC_SUB_FUNCTION_NOT_SUPPORTED,NULL,0);
		return FALSE;
	}
	
	return TRUE;
}






#endif
#if 0
extern tUdsReqInfor gUdsTmpReqInfor;
/***************************************************************************************************
* When the result come back from SOC AVM, this function will be called.
* The rx_data include DID and the result, no SID.
***************************************************************************************************/
void diagApp0x31IpcCallback(uint16 function, uint8 sid, uint8 *rx_data, uint32 len) //AVM Specific
{
	tUdsRespInfor *respInfor;
	respInfor->recordTick = gUdsTmpReqInfor.reqTick;
	respInfor->ch = 2; //AVM
	respInfor->dataFromSocFlag = TRUE;
	respInfor->finishFlag = TRUE;
	respInfor->len = len;
	respInfor->data = rx_data; //2, 05/06, status, error code;
	respInfor->respCode = NRC_NONE;

	diagSidResp(respInfor);

}

#endif

void diagApp0x31IpcCallback(uint8 ch,uint8* rx_data, uint32 len)
{
	#if 1
	tUdsRespInfor respInfor;

	respInfor.recordTick = diagApp_getRxTick(ch);
	respInfor.ch = ch;
	respInfor.dataFromSocFlag = TRUE;
	respInfor.finishFlag = TRUE;
	respInfor.len = len;
	respInfor.data = rx_data;
	respInfor.respCode = NRC_NONE;

	diagSidResp(&respInfor);

	#else
	diagCpyData(gUDSBufferPtr,rx_data,len);
	gGetRespFromDiagAppFlag = TRUE;
	gDiagRespCode = 0xFF;
	gRespDataLen = 3;
	#endif
}







