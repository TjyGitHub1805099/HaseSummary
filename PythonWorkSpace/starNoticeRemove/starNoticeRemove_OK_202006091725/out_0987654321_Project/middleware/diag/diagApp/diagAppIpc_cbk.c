#include"diagAppIpc_cbk.h"

#include"msg_def.h"
#include"diagAppSid0x22.h"
#include"diagAppSid0x31.h"
#include"diagAppSid0x2f.h"
#include "pmStateMachine_If.h"
#include "Spi_Ipc_gw_api.h"

#include"sid0x22_if.h"
#include "trace.h"
#include"dtcCore_if.h"
#include"dtcCore_cfg.h"
#include"dtcApp_if.h"
#include"BoardDefines.h"
#include"diagCommon_cfg.h"
#include "NvM_Cfg.h" 
#include"uds_sid_if.h"
#include "dtcCore_cfg.h"
#include "main.h"



uint8 g_AVM_ProgramingDate[AVM_DID_PROGRAMING_DATE_LEN] = {0};
uint8 g_AVM_SysSupplierEcuSwVersionNum[AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN] = {0};
uint8 g_AVM_EncryptedIC_Status[AVM_DID_ENCRYPT_IC_STATUS_LEN] = {0};
uint8 g_AVM_Original_Status[AVM_DID_ORIGINAL_STATUS_LEN] = {0};
uint8 g_AVM_Photo_Result[1] ={0};  
uint8 g_AVM_Capture_State[1] ={0}; 


extern void PM_CheckAVMPhotoResult(uint8 result);
extern uint8 PM_GetAVM_Outdoor_Photo_RequestSignal(void);
extern uint32 diagApp_getRxTick(uint8 ch);

void diagAppIpcCbk_SocNotify_IVI(uint16 function, uint8* rx_data, uint32 len)
{
	DIAG_TRACE_TXT(UDS_TRACE, TRACE_INFO, "diagAppIpcCbk_SocNotify()\r\n");

	uint8 subFunctionId = 0;
	uint8 uUserId;
	
	if(NULL == rx_data)
		return;

	switch(function)
	{

		case IVI_IPC_UDS_SETUP_CERTIFICATE_STATE:
			IpcCbk_SocWriteMcuReply(function, rx_data, len);
			break;
		case IVI_IPC_UDS_GET_VIN:
			IpcCbk_SocReadMcuReply(function, rx_data, len);
			break;
		
		case HUD_IPC_CALIBRATION_OR_RESET_REQ:	
			subFunctionId = rx_data[3];
			if((subFunctionId >= 1) && (subFunctionId <= 6))   
			{
				if(len == 5)
				{
					TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "RX HUD Reset status:%d\r\n",rx_data[4]);
					uUserId = rx_data[3] -1;
					gHUD_ResetFinish[uUserId] = TRUE;
					gHUD_ResetState[uUserId] = rx_data[4];  
					
				}
			}
			break;

		case HUD_IPC_READ_CAL_DATA_RESP:
			DIAG_TRACE_TXT(UDS_TRACE, TRACE_WARNING, "HUD_22_A1/A2 feedback notify received!\r\n");
			diagApp0x22IpcCallback(UDS_HUD, rx_data+1, len-1);

			break;
		case HUD_IPC_REPORT_CALIBRATION_STATUS:
			if (0 == rx_data[0])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_NOT_CALIBRATION_YET, DTC_PRE_PASSIVE, NULL_PTR, 0);
			else if(1 == rx_data[0])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_NOT_CALIBRATION_YET, DTC_PRE_ACTIVE, NULL_PTR, 0);

			if (0 == rx_data[1])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CALIBRATION_COMMUNICATION_FAILURE, DTC_PRE_PASSIVE, NULL_PTR, 0);
			else if(1 == rx_data[1])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CALIBRATION_COMMUNICATION_FAILURE, DTC_PRE_ACTIVE, NULL_PTR, 0);

			if (0 == rx_data[2])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CALIBRATION_DATA_RECORDING_FAILURE, DTC_PRE_PASSIVE, NULL_PTR, 0);
			else if(1 == rx_data[2])
			dtcCoreSetDtcStatus(UDS_HUD,HUD_DTC_ID_INDEX_CALIBRATION_DATA_RECORDING_FAILURE, DTC_PRE_ACTIVE, NULL_PTR, 0);

			break;
#if 0
		case IVI_IPC_DIAG_FUNCTION_UDS_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_CTRL:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_READ:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_CTRL:
			
			break;
#endif

		default:
			break;
	}




	
	switch (function)
	{
	
	}


	

}



void diagAppIpcCbk_SocRequest_IVI(const void* ctx, uint8* rx_data, uint32 len)
{

	DIAG_TRACE_TXT(UDS_TRACE, TRACE_INFO, "diagAppIpcCbk_SocRequest()\r\n");

#if 0
	request_context_t* context = (request_context_t*)ctx;

	if ((context == NULL) || (rx_data == NULL) || (ctx == NULL))
	{
		return;
	}
	
	switch(context->function_id)
	{

		case IVI_IPC_UDS_SETUP_CERTIFICATE_STATE:
			IpcCbk_SocWriteMcuReply(context->function_id, rx_data, len);
			break;
		case IVI_IPC_UDS_GET_VIN:
			IpcCbk_SocReadMcuReply(context->function_id, rx_data, len);
			break;
		default:
			break;
	}
#endif
}


void IpcCbk_SocReadMcuReply(uint16 function, uint8* rx_data, uint16 len)
{
	const t0x22DidInfo* didInfo = NULL;
	uint16 did = 0;

	switch(function)
	{
		case IVI_IPC_UDS_GET_VIN:
			did = 0xF190;
			break;
		default:
			break;
	}

	didInfo = sid0x22GetDidInfo(DIAG_DEV_ID_IVI, did);
	if(didInfo)
	{
		if(didInfo->nvmAddr != NULL)
		{
			ipc_send_notification(1, IPC_GW_CHANNEL_DIAG_IVI, IVI_IPC_UDS_GET_VIN, didInfo->nvmAddr, didInfo->dataLen);
		}
	}
}


void IpcCbk_SocWriteMcuReply(uint16 function, uint8* rx_data, uint16 len)
{
	
	
	
	
	switch(function)
	{
		case IVI_IPC_UDS_SETUP_CERTIFICATE_STATE:
			
			
			gIVI_PKI_CERTIFICATE[0] = rx_data[0];

		    TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "RX PKI certificate:%d\r\n",rx_data[0]);
			diagNvmWriteSync(gIVI_PKI_CERTIFICATENvMBlockID,gIVI_PKI_CERTIFICATE);
			#if 0
			if (gIVI_PKI_CERTIFICATE[0] == 20)
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_PKI_CERTIFICATE_ERROR, DTC_PRE_PASSIVE, NULL, 0);
			else
				dtcCoreSetDtcStatus(UDS_IVI, IVI_DTC_ID_INDEX_PKI_CERTIFICATE_ERROR, DTC_PRE_ACTIVE, NULL, 0);
			#endif
			
			break;
	#if 0
		case IPC_DIAG_FUNCTION_AME_WRITE_MODULE_CONFIG_ITEM:
			did = 0xF010;
			break;
		case IPC_DIAG_FUNCTION_AME_WRITE_IOC_SW_VERSION_NUM:
			did = 0xF195;
			break;
	#endif
		default:
			break;
	}
#if 0
	didInfo = sid0x22GetDidInfo(DIAG_DEV_ID_IVI, did);
	if(didInfo && (len==didInfo->dataLen))
	{
		if(didInfo->nvmAddr != NULL)
		{
			diagCpyData(didInfo->nvmAddr, rx_data, len);
			diagNvmWriteSync(didInfo->nvmBlockId, didInfo->nvmAddr);
			ret = 0;
		}
	}
#endif	
	ipc_send_notification(1, IPC_GW_CHANNEL_DIAG_IVI, IVI_IPC_UDS_SETUP_CERTIFICATE_STATE, (uint8*)gIVI_PKI_CERTIFICATE, 1);
}






void diagAppIpcCbk_IVI_RequestSoc(const void* ctx, uint16 function, uint8* rx_data, uint32 len)
{
	request_context_t* context = (request_context_t*)ctx;
	uint8* tempPtr = rx_data;
	
	if((context == NULL) || (rx_data == NULL) || (ctx == NULL))
	{
		return;
	}

	switch(context->function_id)
	{

# if 0
		case IVI_IPC_DIAG_FUNCTION_UDS_READ:
			diagApp0x22IpcCallback(IVI_IPC_DIAG_FUNCTION_UDS_READ, rx_data[0], tempPtr+1, len-1);
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_CTRL:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_READ:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_CTRL:
			
			break;
# endif
		case IC_IPC_UDS_READ:
			diagApp0x22IpcCallback(0, tempPtr, len);
			break;
		case IC_IPC_UDS_CTRL:
			diagApp0x2fIpcCallback(0, tempPtr, len);
			break;
			
		default:
			break;
	}

}


void diagAppIpcCbk_AutoTestMcuRequest(const void* ctx, uint16 function, uint8* rx_data, uint32 len)
{

	DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:AutoTest_Result_Callback\r\n");
	request_context_t* context = (request_context_t*)ctx;
	uint8* tempPtr = rx_data;
	
	if((context == NULL) || (rx_data == NULL) || (ctx == NULL) || (len == 0))
	{
		return;
	}

	switch(context->function_id)
	{

# if 0
		case IVI_IPC_DIAG_FUNCTION_UDS_READ:
			diagApp0x22IpcCallback(IVI_IPC_DIAG_FUNCTION_UDS_READ, rx_data[0], tempPtr+1, len-1);
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_CTRL:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_READ:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_CTRL:
			
			break;
# endif
		case IC_IPC_AUTOTEST_READ:
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:AutoTest_22_serviceResultReceived\r\n");
			diagApp0x22IpcCallback(1,tempPtr, len);
			break;
		case IC_IPC_AUTOTEST_CTRL:
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:AutoTest_2F_serviceResultReceived\r\n");
			diagApp0x2fIpcCallback(1, tempPtr, len);
			break;

		default:
			break;
	}

}



#ifdef UDS_AVM

void diagApp_AVM_IpcNotifyInd(uint16 function, uint8* rx_data, uint32 len)
{
	uint8 subFunctionId = 0;
	boolean innerErrorFlag = FALSE;
	uint8 calibrationType = 0;
	uint8 cameraStatus = 0;
	
	uint8 uReqData[1];

	if(NULL == rx_data)
		return;
	
	switch (function)
	{
	case AVM_IPC_CALIBRATION_OR_ERR_RESP:   
		
		subFunctionId = rx_data[0];
		if(subFunctionId == AVM_CALIBRATION_ERR_INFOR)
		{
			if(len == 3)
			{
				TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "RX AVM err status:%d,%d\r\n",rx_data[1],rx_data[2]);
				
				cameraStatus = rx_data[1]; 
				if(cameraStatus&1)
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_FRONT_CAMERA_VIDEO_INPUT, DTC_PRE_ACTIVE, NULL, 0);
				else
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_FRONT_CAMERA_VIDEO_INPUT, DTC_PRE_PASSIVE, NULL, 0);

				if(cameraStatus&2)
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_REAR_CAMERA_VIDEO_INPUT, DTC_PRE_ACTIVE, NULL, 0);
				else
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_REAR_CAMERA_VIDEO_INPUT, DTC_PRE_PASSIVE, NULL, 0);

				if(cameraStatus&4)
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_LEFT_CAMERA_VIDEO_INPUT, DTC_PRE_ACTIVE, NULL, 0);
				else
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_LEFT_CAMERA_VIDEO_INPUT, DTC_PRE_PASSIVE, NULL, 0);

				if(cameraStatus&8)
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_RIGHT_CAMERA_VIDEO_INPUT, DTC_PRE_ACTIVE, NULL, 0);
				else
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_RIGHT_CAMERA_VIDEO_INPUT, DTC_PRE_PASSIVE, NULL, 0);

				
				if(rx_data[2] == 1)
				{
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_AVM_CALIB_IMPLEMENT_JUDGMENT, DTC_PRE_ACTIVE, NULL, 0);
				}
				else if(rx_data[2] == 0)
				{
					dtcCoreSetDtcStatus(UDS_AVM, AVM_DTC_ID_INDEX_AVM_CALIB_IMPLEMENT_JUDGMENT, DTC_PRE_PASSIVE, NULL, 0);
				}

				
				uReqData[0] = 1;
				ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 1); 

				
			}
		}
		else if(subFunctionId == AVM_CALIBRATION_RESULT)
		{
			if(len == 4)
			{
				
				calibrationType = rx_data[1];
				
				
				gAVM_CalibrationState[0] = rx_data[2];

				
				gAVM_CalibrationState[1] = rx_data[3];
				
				
				if(rx_data[2] ==  AVM_CALIBRATION_STATUS_FAILED || rx_data[2] == AVM_CALIBRATION_STATUS_SUCCESS)
				{
					gAVM_CalibrationFinish = TRUE;  
					gAVM_CalibrationInProgress = FALSE;
				}
				TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "Rx AVM calib result:%d,%d!\r\n",rx_data[2],rx_data[3]);
				
			}
			else
				innerErrorFlag = TRUE;
		}
		else
			innerErrorFlag = TRUE;

		if(innerErrorFlag == TRUE)
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "RX AVM calibration result,but inner err!!\r\n");
		break;


	case AVM_IPC_SWVERSION_RESP:  
		TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "rx notify:AVM SW version!\r\n");
		#if 0
		uRespData[0] = (uint8)((AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM&0xFF00)>>8);
		uRespData[1] = (uint8)((AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM&0xFF));
		uRespData[2] = rx_data[0];
		uRespData[3] = rx_data[1];
		uRespData[4] = rx_data[2];
		uRespData[5] = rx_data[3];
		diagApp_txData(UDS_AVM,NRC_NONE,TRUE,uRespData,6);
		#else
		if(len >= 4)
		{
			g_AVM_SysSupplierEcuSwVersionNum[0] = rx_data[0]+0x30;
			g_AVM_SysSupplierEcuSwVersionNum[1] = rx_data[1]+0x30;
			g_AVM_SysSupplierEcuSwVersionNum[2] = rx_data[2]+0x30;
			g_AVM_SysSupplierEcuSwVersionNum[3] = rx_data[3]+0x30;
		}
		#endif

		break;

	case AVM_IPC_PROG_DATE_RESP:  
		TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "rx notify:AVM program date!\r\n");
		#if 0
		uRespData[0] = (uint8)((AVM_DID_PROGRAMING_DATE&0xFF00)>>8);
		uRespData[1] = (uint8)((AVM_DID_PROGRAMING_DATE&0xFF));

		uRespData[2] = rx_data[1];	
		uRespData[3] = rx_data[2];	
		uRespData[4] = rx_data[3];	
		uRespData[5] = rx_data[4];	
		diagApp_txData(UDS_AVM,NRC_NONE,TRUE,uRespData,6);
		#else
		if(len >= 4)
		{
			g_AVM_ProgramingDate[0] = rx_data[0];	
			g_AVM_ProgramingDate[1] = rx_data[1];	
			g_AVM_ProgramingDate[2] = rx_data[2];	
			g_AVM_ProgramingDate[3] = rx_data[3];	
		}
	#endif
		break;
		
	case AVM_IPC_ORIGINAL_STATUS_RESP_1:
		if(len >= 2)
		{
			g_AVM_EncryptedIC_Status[0] = rx_data[0];	
			g_AVM_EncryptedIC_Status[1] = rx_data[1];	
		}
		break;

	case AVM_IPC_ORIGINAL_STATUS_RESP_2:
		if(len >= 3)
		{
			g_AVM_Original_Status[0] = rx_data[0];	
			g_AVM_Original_Status[1] = rx_data[1];	
			g_AVM_Original_Status[2] = rx_data[2];	
		}

		break;

	case AVM_IPC_CAPTURE_STATE_NOTIFY:

		if(len == 1)
		{
			g_AVM_Capture_State[0] = rx_data[0]; 
			
			
			if( 0 == g_AVM_Capture_State[0])
			{
				TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "rx AVM_Capture_State = %d,close Rearview Mirror\r\n",g_AVM_Capture_State[0]);
				
				
				
			}
		}

		break;
		
	case AVM_IPC_PHOTO_RESULT_NOTIFY:

		if(len == 1)
		{
			g_AVM_Photo_Result[0] = rx_data[0];	

			
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "rx AVM_Photo_Result = %d\r\n",g_AVM_Photo_Result[0]);
			PM_CheckAVMPhotoResult(g_AVM_Photo_Result[0]);
		}

		break;

	default:
		break;
	}
	(void)calibrationType;
}


void diagApp_AVM_IpcRequestInd(const void *ctx, uint8 *rx_data, uint32 len)
{
	request_context_t* context = (request_context_t*)ctx;
	uint8 subFunctionId;
	uint8 uRespData[16] = {0};
	

	if(( NULL == ctx ) || ( NULL == rx_data ))
	
	{
		return;
	}

	TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "rx SOC AVM req id[%d]\r\n",context->function_id);

	switch(context->function_id)
	{
	#if 1
	case AVM_IPC_CAN_MSG_STATE_REQ:
		subFunctionId = rx_data[0];
		switch(subFunctionId)
		{
		case AVM_IPC_QUERY_CAN_MSG_STATUS_REQ:
			dtcAppGetAvmMsgState(uRespData,AVM_CAN_MSG_STATE_LEN);
			TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "soc req AVM CAN state\r\n");
			ipc_gw_send_reply(1, context, uRespData, AVM_CAN_MSG_STATE_LEN);
			break;
		default:
			break;
		}
		break;
	#endif
	
	case AVM_IPC_POWER_CTRL_REQ:
		if(rx_data[0] == 0)
		{
			Port_SetOutLevel(CAMMERA1_ON,PORT_LOW);
			Port_SetOutLevel(CAMMERA2_ON,PORT_LOW);
			Port_SetOutLevel(CAMMERA3_ON,PORT_LOW);
			Port_SetOutLevel(CAMMERA4_ON,PORT_LOW);
		}
		else
		{
			Port_SetOutLevel(CAMMERA1_ON,PORT_HIGH);
			Port_SetOutLevel(CAMMERA2_ON,PORT_HIGH);
			Port_SetOutLevel(CAMMERA3_ON,PORT_HIGH);
			Port_SetOutLevel(CAMMERA4_ON,PORT_HIGH);
		}
		ipc_gw_send_reply(1, context, rx_data, 1);
		break;
	case AVM_IPC_GET_PHOTO_STATE_REQ:
		
		
		rx_data[0]= PM_GetAVM_Outdoor_Photo_RequestSignal();
		TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "soc get AVM photo req state = %d \r\n",rx_data[0]);
		ipc_gw_send_reply(1, context, rx_data, 1);
		break;
	default:
		break;
	}
}


void diagApp_AVM_IpcResp(const void* ctx, uint16 function, uint8* rx_data, uint32 len) 
{
	request_context_t* context = (request_context_t*)ctx;
	
	
	
	
	if(( NULL == ctx ) || ( NULL == rx_data ))
		return;

	switch(context->function_id)
	{
		case AVM_IPC_SWVERSION_RESP:  
			TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "rx resp:AVM SW version!\r\n");
			#if 0
			uRespData[0] = (uint8)((AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM&0xFF00)>>8);
			uRespData[1] = (uint8)((AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM&0xFF));
			uRespData[2] = rx_data[0];
			uRespData[3] = rx_data[1];
			uRespData[4] = rx_data[2];
			uRespData[5] = rx_data[3];
			diagApp_txData(UDS_AVM,NRC_NONE,TRUE,uRespData,6);
			#else
			if (len >= 4)
			{
				g_AVM_SysSupplierEcuSwVersionNum[0] = rx_data[0];
				g_AVM_SysSupplierEcuSwVersionNum[1] = rx_data[1];
				g_AVM_SysSupplierEcuSwVersionNum[2] = rx_data[2];
				g_AVM_SysSupplierEcuSwVersionNum[3] = rx_data[3];
			}
			#endif

		case AVM_IPC_PROG_DATE_RESP:  
			TracePrintf(DIAGAPP_TRACE, TRACE_INFO, "rx resp:AVM program date!\r\n");
			#if 0
			uRespData[0] = (uint8)((AVM_DID_PROGRAMING_DATE&0xFF00)>>8);
			uRespData[1] = (uint8)((AVM_DID_PROGRAMING_DATE&0xFF));

			uRespData[2] = rx_data[1];	
			uRespData[3] = rx_data[2];	
			uRespData[4] = rx_data[3];	
			uRespData[5] = rx_data[4];	
			diagApp_txData(UDS_AVM,NRC_NONE,TRUE,uRespData,6);
			#else
			if(len >= 4)
			{
				g_AVM_ProgramingDate[0] = rx_data[0];	
				g_AVM_ProgramingDate[1] = rx_data[1];	
				g_AVM_ProgramingDate[2] = rx_data[2];	
				g_AVM_ProgramingDate[3] = rx_data[3];	
			}
#endif
			break;
		default:
			break;
	}


}

#endif

void diagApp_txData(uint8 ch,uint8 respCode,boolean finishFlag,uint8 *txData,uint16 len)
{
	tUdsRespInfor respInfor;

	respInfor.recordTick = diagApp_getRxTick(ch);
	respInfor.ch = ch; 
	respInfor.dataFromSocFlag = TRUE;
	respInfor.finishFlag = finishFlag;
	respInfor.len = len;
	respInfor.data = txData; 
	respInfor.respCode = respCode;

	diagSidResp(&respInfor);
}

#ifdef UDS_HUD
void diagApp_HUD_IpcNotifyInd(uint16 function, uint8* rx_data, uint32 len)
{

	uint8 subFunctionId = 0;
	uint8 uUserId;
	
	if(NULL == rx_data)
		return;
	
	switch (function)
	{
	case HUD_IPC_CALIBRATION_OR_RESET_REQ:	
		
		subFunctionId = rx_data[3];
		if((subFunctionId >= 1) && (subFunctionId <= 6))   
		{
			if(len == 5)
			{
				
				uUserId = rx_data[3] -1;
				gHUD_ResetFinish[uUserId] = TRUE;
				gHUD_ResetState[uUserId] = rx_data[4];  
				
			}
		}
		break;

	default:
		break;
	}


}

void diagAppIpcCbk_HUDCalibrationMcuRequest(const void* ctx, uint16 function, uint8* rx_data, uint32 len)
{
	
	request_context_t* context = (request_context_t*)ctx;
	
	
	if((context == NULL) || (rx_data == NULL) || (ctx == NULL) || (len == 0))
	{
		return;
	}

	switch(context->function_id)
	{

# if 0
		case IVI_IPC_DIAG_FUNCTION_UDS_READ:
			diagApp0x22IpcCallback(IVI_IPC_DIAG_FUNCTION_UDS_READ, rx_data[0], tempPtr+1, len-1);
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_UDS_CTRL:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_READ:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_WRITE:
			
			break;
		case IVI_IPC_DIAG_FUNCTION_AME_CTRL:
			
			break;
# endif
		case HUD_IPC_READ_CAL_DATA_RESP:
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:HUD_Calibration_func_22_serviceResultReceived\r\n");
			diagApp0x22IpcCallback(UDS_HUD, rx_data+1, len-1);
			break;
		
		case HUD_IPC_CALIBRATION_OR_RESET_RESP:
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:HUD_Calibration_func_31_serviceResultReceived\r\n");
			diagApp0x31IpcCallback(UDS_HUD, rx_data+1, len-1);
			break;
			
		default:
			break;
	}


}



#endif

