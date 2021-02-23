
#include "diagAppSid0x22.h"
#include "sid0x22_cfg.h"
#include "uds_sid_if.h"
#include "diagAppIpc_cbk.h"
#include "msg_def.h"
#include "trace.h"
#include "Spi_Ipc_gw_api.h"
#include "DeaDiag.h"
#include "uds_cfg.h"

extern uint32 diagApp_getRxTick(uint8 ch);

extern tUdsReqInfor gUdsTmpReqInfor;

uint8 g_HUD_message_counter =0;
uint8 g_HUD_message[182];
uint8 g_HUD_message_len = 0;


boolean diagApp0x22ReqHandle(const uint8* data, uint16 len)
{
	uint8 ch = data[0];
	uint16 did = 0;
	did = did;
	
	
	
	
	uint8 reqData[16];
	

	static request_context_t ctx;

	if(data == NULL)
		return FALSE;

	did = (data[7]<<8)|data[8];

	if(ch == UDS_IVI)
	{
		
		
		
		return TRUE;
	}
	else if(ch == UDS_IC)
	{
	# if 0
		switch(did)
		{
			case IC_DID_0E07_INFO:
				DEA_Diag_GetDid_0E07_Info(NULL_PTR);
				break;

			case IC_DID_SSD_MEDIA_SOURCE:
				ctx.function_id = IC_IPC_UDS_READ;
				ctx.channel = IPC_GW_CHANNEL_SYSTEM;
				reqData[0] = 0;
				ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_UDS_READ, reqData, 1, &ctx, diagAppIpcCbk_AutoTestMcuRequest);
				break;
		  	case IC_DID_SSD_CURRENT_SONG_NAME:
				ctx.function_id = IC_IPC_UDS_READ;
				ctx.channel = IPC_GW_CHANNEL_SYSTEM;
				reqData[0] = 1;
				ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_UDS_READ, reqData, 1, &ctx, diagAppIpcCbk_AutoTestMcuRequest);
				break;
					
		}
	#endif

	if(did == IC_DID_0E07_INFO)
		DEA_Diag_GetDid_0E07_Info(NULL_PTR);

	else   
	{
		ctx.function_id = IC_IPC_AUTOTEST_READ;
		ctx.channel = IPC_GW_CHANNEL_SYSTEM;
		diagCpyData(reqData, data+7, len-7);
		ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_AUTOTEST_READ, reqData, len-7, &ctx, diagAppIpcCbk_AutoTestMcuRequest);


		
		
#if 0
		respData[0] = 0x0A;
		respData[1] = 0x01;
		respData[2] = 0x03;
		
		diagAppIpcCbk_AutoTestMcuRequest(&ctx, 13, respData, 3);
#endif
		
	}

		return TRUE;
	}
	else if(ch == UDS_AVM)
	{
		#if 0
		ctx.channel = IPC_CHANNEL_DIAG;
		switch (did)
		{
		case AVM_DID_PROGRAMING_DATE: 
			ctx.function_id = AVM_IPC_PROG_DATE_REQ;
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "diagApp req AVM program date!\r\n");
			reqData[0] = 1;
			ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_PROG_DATE_REQ, reqData, 1, &ctx, diagApp_AVM_IpcResp);
			break;
		case AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM: 
			ctx.function_id = AVM_IPC_SWVERSION_REQ;
			reqData[0] = 1;
			TracePrintf(DIAGAPP_TRACE, TRACE_WARNING, "diagApp req AVM SW VERSION!\r\n");
			ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_SWVERSION_REQ, reqData, 1, &ctx, diagApp_AVM_IpcResp);
			break;
		default:
			break;

		}
		#endif
		return TRUE;
	}

	else if(ch == UDS_HUD)
	{

		ctx.function_id = HUD_IPC_READ_CAL_DATA_REQ;
		ctx.channel = IPC_GW_CHANNEL_DIAG_IVI;
		reqData[0] = 0x22;
		diagCpyData(reqData+1, data+7, len-7);
		


		ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_READ_CAL_DATA_REQ, reqData, 3);

		
		
		
		return TRUE;

	}
	else
		return FALSE;
}



#if 0
extern boolean gGetRespFromDiagAppFlag;
extern uint8 gDiagRespCode;
#endif

void diagApp0x22IpcCallback(uint8 ch,uint8* rx_data, uint32 len)
{
	#if 1
	tUdsRespInfor respInfor;
	if(ch == UDS_HUD) 
	{
		if(g_HUD_message_counter ==0)
		{
		
			g_HUD_message_len = len;
			diagCpyData(g_HUD_message, rx_data, len);
			g_HUD_message_counter += 1;
		}
		else if(g_HUD_message_counter ==1)
		{
			respInfor.recordTick = diagApp_getRxTick(ch);
			respInfor.ch = ch;
			respInfor.dataFromSocFlag = TRUE;
			respInfor.finishFlag = TRUE;
			diagCpyData(g_HUD_message+g_HUD_message_len, rx_data+2, len-2);
			respInfor.data = g_HUD_message;
			respInfor.len = g_HUD_message_len+len-2;
			respInfor.respCode = NRC_NONE;
			g_HUD_message_counter = 0;
			diagSidResp(&respInfor);
		}
	}
	else
	{
		respInfor.recordTick = diagApp_getRxTick(ch);
		respInfor.ch = ch;
		respInfor.dataFromSocFlag = TRUE;
		respInfor.finishFlag = TRUE;
		respInfor.len = len;
		respInfor.data = rx_data;
		respInfor.respCode = NRC_NONE;

		diagSidResp(&respInfor);

	}
	#else
	diagCpyData(gUDSBufferPtr,rx_data,len);
	gGetRespFromDiagAppFlag = TRUE;
	gDiagRespCode = 0xFF;
	gRespDataLen = 3;
	#endif
}







