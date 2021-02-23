

#include "ame.h"

#include "msg_def.h"
#include "tpTypes.h"
#include "Task_IpcApp_Cfg.h"
#include "tpConfig.h"
#include"tp_if.h"



#if 1 

tAmeTraceLevel gAmeTraceLevel = AME_TRACE_LEVEL_0;

uint8    gAmeRespNrcBuffer[8] = {0};
uint8    gAmeRxBuffer[AME_RX_BUFF_LEN] = {0};
uint16   gAmeRxDataLen = 0;
boolean  gAmeBusyFlag = FALSE;
boolean  gAmeRespRepeatMsgFlag = FALSE;
uint8    gAmeCmd = 0; 
uint8    gAmeTempCmd = 0; 
static boolean  gAmeNewCmdFlag = FALSE;
static uint8 gAmeDevCh = UDS_IVI;


tAmeTxBuffer gAmeTxBuffer;


void    ameResetParas();


tAmeTxBuffer* ameGetTxBuffer()
{
	return &gAmeTxBuffer;
}



#if 0
boolean ameCheckCmd(uint8* dataPtr, uint16 len)
{
	uint8 cmd = 0;
	if(dataPtr == NULL || len<2)
		return FALSE;

	cmd = dataPtr[0];
	switch(cmd)
	{
		case SID_ECU_RESET:            
		case SID_READ_DATA_BY_DID:     
		case SID_WRITE_DATA_BY_DID:    
		case SID_SECURITY_ACCESS:      
		case SID_IOCONTROL:            
		case SID_ROUTINE_CONTROL:      
		{
			gAmeCmd = cmd;
			break;
		}
		default:
		{
			return FALSE;
		}
	}

	return TRUE;
}
#endif


#if 1
boolean ameRxIndication(uint32 ch, uint8* newDataPtr, uint16 len)
{
	static boolean ameActiveFlag = FALSE;
	uint8 i = 0;

	
#ifdef SW_PHASE_APPLICATION

	if(newDataPtr == NULL || len<2)
		return FALSE;
	
#if 1
	if(ameActiveFlag == FALSE)   
	{
		if(len == UNLOCK_AME_DATA_LEN)
		{
			for(i=0; i<UNLOCK_AME_DATA_LEN; i++)
			{
				if(newDataPtr[i] != gUnlockAmeKey[i])
				{
					return FALSE;
				}
			}
			ameActiveFlag = TRUE;
		}
		return FALSE;
	}
#endif

	if(gAmeTraceLevel>AME_TRACE_LEVEL_1)
		AME_TRACE_VALUE2(AME_TRACE,TRACE_INFO, "AME:Rx msg[id:0x%x,len:%d]\n\r", newDataPtr[0], len);
	
	if(newDataPtr[0] == AME_CMD_RESET_MODULE)
	{
		ameResetParas();
		gAmeRespNrcBuffer[0] = AME_CMD_RESET_MODULE+0x40;
		Tp_TxDataRequest(TP_DIAG_DEV_ID_AME, gAmeRespNrcBuffer, 1);
	}
	else if(gAmeBusyFlag == TRUE)
	{
		AME_TRACE_TXT(AME_TRACE,TRACE_INFO,"AME: busy now,ignore new msg\n\r");

		gAmeRespNrcBuffer[0] = NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION;
		gAmeRespNrcBuffer[1] = newDataPtr[0];
		gAmeRespNrcBuffer[2] = NRC_BUSY_REPEAT_REQ;

		Tp_TxDataRequest(TP_DIAG_DEV_ID_AME, gAmeRespNrcBuffer, 3);
	}
	else
	{
		if(len<8)
		{
			diagCpyData(gAmeRxBuffer, (const uint8*)newDataPtr, len);
		}
		else
		{
			
		}
		
		gAmeBusyFlag = TRUE;
		gAmeRxDataLen = len;
		gAmeNewCmdFlag = TRUE;
	}
	
#endif
	return TRUE;
}
#endif




tReqBufferResult ameRxFirstFrameIndication(uint32 ch, uint16 reqBufferLen, uint8** RxBufferPtr)
{	
	if(reqBufferLen > AME_RX_BUFF_LEN-7)
		return BUFFER_OVERFLOW;

	if(gAmeBusyFlag == TRUE)
		return BUFFER_WAIT;
	
	*RxBufferPtr = gAmeRxBuffer;
	return BUFFER_OK;
}



void ameCancelRxData(uint32 ch, uint8 reason)
{
	
	
	
}



void ameTxDataConfirmation(uint32 ch, boolean result, uint8 errId)
{
	if(result == FALSE)
	{
		
		
	}
	else
	{

	}
}



void ameAppResetTxBuffer()
{
	gAmeTxBuffer.curDataLen = 0;
}



uint16 ameAppFillTxBuffer(uint8* data, uint16 len)
{
	if(data == NULL || len>(AME_TX_BUFF_LEN-1) || len == 0)
		return 0;
	
	if((gAmeTxBuffer.curDataLen+len+1) > AME_TX_BUFF_LEN)
		return 0;
	else
		gAmeTxBuffer.curDataLen += diagCpyData((&(gAmeTxBuffer.buffer[1]) + gAmeTxBuffer.curDataLen), (const uint8*)data, len);

	return gAmeTxBuffer.curDataLen;
}



boolean ameTxDataReq(uint32 Ch, uint8* data, uint16 len)
{
#ifdef SW_PHASE_APPLICATION

	if(data == NULL || len > AME_TX_BUFF_LEN)
		return FALSE;

	gAmeBusyFlag = FALSE;
	if(gAmeTraceLevel>AME_TRACE_LEVEL_1)
		AME_TRACE_VALUE1(AME_TRACE, TRACE_INFO, "AME:TX data,sid=0x%0x\r\n", data[0]);
	return Tp_TxDataRequest(TP_DIAG_DEV_ID_AME, data, len);    
#else
	return FALSE;
#endif
}



boolean ameAppTxData(uint32 Ch, uint8* data, uint16 len)
{
	
	
	
	
	if(gAmeCmd==0 || gAmeCmd!=gAmeRxBuffer[0] || gAmeTxBuffer.curDataLen==0)
		return FALSE;

	gAmeTxBuffer.buffer[0] = gAmeCmd+0x40;
	return ameTxDataReq(TP_DIAG_DEV_ID_AME, gAmeTxBuffer.buffer, gAmeTxBuffer.curDataLen+1);
}



error_type ameAppTxDataToSoc(request_context_t* ctx, uint16 did, uint8* data, uint16 len)
{
	error_type ret = 0;

#if 0
	gAmeTxBuffer.buffer[0] = (uint8)(didNum>>8);
	gAmeTxBuffer.buffer[1] = (uint8)(didNum);
	diagCpyData(&gAmeTxBuffer.buffer[2], data, len);
	gAmeTxBuffer.curDataLen = len+2;
#else
	diagCpyData(&gAmeTxBuffer.buffer[0], (const uint8*)data, len);
	gAmeTxBuffer.curDataLen = len;
#endif

	ret = ipc_gw_send_reply(0, ctx, &gAmeTxBuffer.buffer[0], gAmeTxBuffer.curDataLen);
	
	return ret;
}

void ameSetChannelInfor(uint8 devch)
{
	gAmeDevCh = devch;
}



boolean ameParseNewMsg()
{
	uint8 i = 0;
	boolean validSidFlag = FALSE;
	uint8 respCode = 0;
	uint8 devCh = 0;

 	if(gAmeNewCmdFlag == FALSE)
 		return TRUE;


	devCh = gAmeDevCh;

	for(i=0; i<gAmeSubServiceFnCbkTotalNum; i++)
	{
		if(gAmeRxBuffer[0] == gAmeSubServiceFnCbk[i].sid)
		{
			validSidFlag = TRUE;
			gAmeCmd = gAmeRxBuffer[0];
			if(gAmeTraceLevel>AME_TRACE_LEVEL_2)
				AME_TRACE_VALUE1(AME_TRACE, TRACE_INFO, "AME:RX SID 0x%X\r\n", gAmeRxBuffer[0]);
			respCode = gAmeSubServiceFnCbk[i].FnCbk(devCh, &gAmeRxBuffer[0], gAmeRxDataLen);
			break;
		}
	}
	
	if(validSidFlag == FALSE)
	{
		gAmeRespNrcBuffer[0] = NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION;
		gAmeRespNrcBuffer[1] = gAmeCmd;
		gAmeRespNrcBuffer[2] = NRC_SERVICE_NOT_SUPPORTED;
		ameTxDataReq(TP_DIAG_DEV_ID_AME, gAmeRespNrcBuffer, 3);
	}
	else if(respCode != NRC_NONE)
	{
		AME_TRACE_VALUE1(AME_TRACE,TRACE_INFO, "AME:respCode=0x%0x\r\n", respCode);
		gAmeRespNrcBuffer[0] = NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION;
		gAmeRespNrcBuffer[1] = gAmeCmd;
		gAmeRespNrcBuffer[2] = respCode;
		ameTxDataReq(TP_DIAG_DEV_ID_AME, gAmeRespNrcBuffer, 3);
	}

	gAmeNewCmdFlag = FALSE;
	
	return validSidFlag;
}



void ameResetParas()
{
	gAmeBusyFlag = FALSE;
	gAmeNewCmdFlag = FALSE;
	gAmeRespRepeatMsgFlag = FALSE;
	gAmeRxDataLen = 0;
	gAmeCmd = 0;
	gAmeTempCmd = 0;
	diagResetBuffer(gAmeRxBuffer, 0, AME_RX_BUFF_LEN);
	diagResetBuffer((uint8*)&gAmeTxBuffer, 0, sizeof(tAmeTxBuffer));
	diagResetBuffer(gAmeRespNrcBuffer, 0, 8);
}




#endif


