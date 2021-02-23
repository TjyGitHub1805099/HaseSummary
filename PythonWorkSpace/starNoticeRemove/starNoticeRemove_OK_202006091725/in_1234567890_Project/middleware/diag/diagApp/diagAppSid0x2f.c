/*************************************************************************************************************
* author: liqi@hangsheng.com.cn
* 
*************************************************************************************************************/

#if 1

#include "diagAppSid0x2f.h"
#include "sid0x2f_cfg.h"
#include "diagAppIpc_cbk.h"
#include "msg_def.h"
#include "trace.h"
#include "Task_M_IpcApp_Cfg.h"
#include "uds_sid_if.h"
#include "Spi_Ipc_gw_api.h"

#ifdef DIAG_OS_UCOS
#include "dea_diag.h"
#endif

#if 0
static uint8 gIOCtrlStateBuff[IOCTRL_BUFFER_LEN];
static uint8 gIOCtrlMaskBuff[IOCTRL_BUFFER_LEN];

#endif


extern uint32 diagApp_getRxTick(uint8 ch);


# if 0
tIOCtrlInfo gIOControlInfo[] = {
	{IOCTRL_ID_DUMMY_TEST,UDS_INNER_SECURITY_LEVEL_0 | UDS_INNER_SECURITY_LEVEL_2,FALSE,IOCTRL_REQ_NONE,1,gIOCtrlStateBuff, gIOCtrlMaskBuff},
};
# endif


uint8 diagApp0x2fGetIoCtrlCaseNum()
{
	//return sizeof(gIOControlInfo)/sizeof(tIOCtrlInfo);
	return 0;
}


boolean diagApp0x2fCheckPrecondition(const tIOCtrlInfo* ioctrlInfor)
{
	if(ioctrlInfor!= NULL)
	{
		switch(ioctrlInfor->did)
		{
		case IOCTRL_ID_DUMMY_TEST:
			// TODO: if need, add here
			return TRUE;
		default:
			return FALSE;
		}
	}

	return FALSE;
}

tUdsNRCType diagApp0x2fChkCtrlStateOrCtrlMask(const tIOCtrlInfo* ioctrlInfo,uint8*data, uint16 len)
{
	//uint16 speed = 0;
	//uint16 rpm = 0;
	//uint8 fuelLed = 0;
	
	if(ioctrlInfo==NULL || data==NULL)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	switch(ioctrlInfo->did)
	{
	case IOCTRL_ID_DUMMY_TEST:
		//parse ctrlState or ctrlMask according to CRS,
		//if invalid, return FALSE;
		//diagCpyData(ioctrlInfo->ctrlState,data+4,ioctrlInfo->ctrlStateLen);
		//if more datas, coninue to check and cpy

		break;
/*
	case IOCTRL_ID_DISPLAY_TEST:
		//data[4] : 0-100% Control Backlight brightness of IPC Display
		//data[5] : 0-100% Control Backlight brightness of IPC LED's backlight
		ioctrlInfo->ctrlState[0] = data[4];
		ioctrlInfo->ctrlState[1] = data[5];
		break;
*/
	default:
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}
		
	
	return NRC_NONE;
}

boolean diagApp0x2fStopTest(const tIOCtrlInfo* ioctrlInfo)
{

	return TRUE;	
}

boolean diagApp0x2fReqHandle(uint8*data, uint16 len)
{
	uint8 ch = data[0];
	//uint16 did = 0;
	uint8 reqData[32];
	static request_context_t ctx;

	if(data == NULL)
		return FALSE;

	//did = (data[6]<<8)|data[7];
	if(ch == UDS_IVI)
	{
		//ctx.channel = IPC_CHANNEL_DIAG;
		//ctx.function_id = IVI_IPC_UDS_READ;
		//ipc_gw_send_request(1, IPC_CHANNEL_DIAG, IVI_IPC_DIAG_FUNCTION_UDS_READ, &data[5], len-1, &ctx, diagApp_AVM_IpcResp);
		return TRUE;
	}
	else if(ch == UDS_IC)
	{
	# if 0
		switch(did)
		{
			case IC_DID_UDS_SWITCH_MEDIA_SOURCE:
				ctx.function_id = IC_IPC_UDS_CTRL;   //not confirmed
				ctx.channel = IPC_GW_CHANNEL_SYSTEM; //not confirmed
				reqData[0] = data[];						 //not confirmed
				ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_UDS_CTRL, reqData, 1, &ctx, diagAppIpcCbk_AutoTestMcuRequest);
				break;
		 	default:
				break;
		}
	# endif
		/* for auto test use  */
		ctx.function_id = IC_IPC_AUTOTEST_CTRL;
		ctx.channel = IPC_GW_CHANNEL_SYSTEM;
		diagCpyData(reqData, data+6, len-6);
		ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_AUTOTEST_CTRL, reqData, len-6, &ctx, diagAppIpcCbk_AutoTestMcuRequest);
		/* for auto test use  */
		return TRUE;
	}
	else if(ch == UDS_AVM)
	{
		return TRUE;
	}
	else
		return FALSE;

# if 0
	uint16 did = 0;
	uint8 ioCtrlPara = 0;
	tIOCtrlInfo* ioctrlInfo = NULL;
	tUdsNRCType respCode = NRC_NONE;
	uint8 *ioctlStateMask = NULL;
	uint16 realDataLen = 0;
	
	if(data == NULL || len <4)
	{
		diagSidResp(FALSE, TRUE, NRC_INCORRECT_MESSAGE_LENGTH, NULL, 0);
		return FALSE;
	}
	
	realDataLen = len -4;
	
	did = (uint16)(data[6]<<8 | data[7]);
	
	ioctrlInfo = sid0x2fGetIoCtrlInfo(did);
	if(ioctrlInfo != NULL)
	{
		if(ioctrlInfo->status == IOCTRL_REQ_START && (realDataLen != ioctrlInfo->ctrlStateLen))
		{
			diagSidResp(FALSE, TRUE, NRC_INCORRECT_MESSAGE_LENGTH, NULL, 0);
			return FALSE;
		}
		
		if(ioctrlInfo->ctrlState!=NULL)
			diagCpyData(ioctrlInfo->ctrlState,data+4,ioctrlInfo->ctrlStateLen);
		
		switch(ioctrlInfo->did)
		{
		case IOCTRL_ID_DUMMY_TEST:
			
			if(ioctrlInfo->status == IOCTRL_REQ_STOP)
				;
			else if(ioctrlInfo->status == IOCTRL_REQ_START)
				;
			
			break;
		default:
			//1 ERROR
			respCode = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}

		// we can resp directly, or after get resp from other modules
		///sid0x2fResp(respCode,ioctrlInfo);
		diagSidResp(FALSE, TRUE, respCode, data+1, len-1);
		return TRUE;
	}

	return FALSE;
# endif
	
}
#endif



void diagApp0x2fIpcCallback(uint8 ch, uint8* rx_data, uint32 len)
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


