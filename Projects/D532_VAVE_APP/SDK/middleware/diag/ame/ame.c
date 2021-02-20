/**
* @file:    ame.c
* @brief:   Add your description here for this file.
* @author:  qiweifeng
* @date:    2018-09-29 17:40:29
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  qiweifeng  2018-09-29 17:40:29  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/

#include "ame.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "msg_def.h"
#include "tpTypes.h"
#include "Task_IpcApp_Cfg.h"
#include "tpConfig.h"
/*------------------------------ END: USER INCLUDE ------------------------------*/

#if 1 // Example Comments


extern uint8 gUnlockAmeKey[];

extern const tAmeSubServiceFnCbk gAmeSubServiceFnCbk[];
extern uint16 gAmeSubServiceFnCbkTotalNum;

tAmeTraceLevel gAmeTraceLevel = AME_TRACE_LEVEL_0;

uint8    gAmeRespNrcBuffer[8] = {0};
uint8    gAmeRxBuffer[AME_RX_BUFF_LEN] = {0};
uint16   gAmeRxDataLen = 0;
boolean  gAmeBusyFlag = FALSE;
boolean  gAmeRespRepeatMsgFlag = FALSE;
uint8    gAmeCmd = 0; //used to store the new cmd we will process, currently there is no another cmd processed
uint8    gAmeTempCmd = 0; //used to store the new cmd but currently we are processing another cmd
static boolean  gAmeNewCmdFlag = FALSE;
static uint8 gAmeDevCh = UDS_IVI;


tAmeTxBuffer gAmeTxBuffer;


void    ameResetParas();


tAmeTxBuffer* ameGetTxBuffer()
{
	return &gAmeTxBuffer;
}


/******************************************************************************************************************
* check if the cmd is supported
*
******************************************************************************************************************/
#if 0
boolean ameCheckCmd(uint8* dataPtr, uint16 len)
{
	uint8 cmd = 0;
	if(dataPtr == NULL || len<2)
		return FALSE;

	cmd = dataPtr[0];
	switch(cmd)
	{
		case SID_ECU_RESET:            //AME_CMD_RESET_MODULE:
		case SID_READ_DATA_BY_DID:     //AME_CMD_READ_DATA:
		case SID_WRITE_DATA_BY_DID:    //AME_CMD_WRITE_DATA:
		case SID_SECURITY_ACCESS:      //AME_CMD_SECURITE_ACCESS:
		case SID_IOCONTROL:            //AME_CMD_IO_CONTROL:
		case SID_ROUTINE_CONTROL:      //AME_CMD_ROUTE_CONTROL:
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

/****************************************************************************************************************
* for TP layer
*
****************************************************************************************************************/
#if 1
boolean ameRxIndication(uint32 ch, uint8* newDataPtr, uint16 len)
{
	static boolean ameActiveFlag = FALSE;
	uint8 i = 0;

	
#ifdef SW_PHASE_APPLICATION

	if(newDataPtr == NULL || len<2)
		return FALSE;
	
#if 1
	if(ameActiveFlag == FALSE)   // need to check if ame is active
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
	else if(gAmeBusyFlag == TRUE)//AME is processing another CMD now
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
			diagCpyData(gAmeRxBuffer, newDataPtr, len);
		}
		else
		{
			//in tp layer, datas had been stored in sAMERxBuffer
		}
		
		gAmeBusyFlag = TRUE;
		gAmeRxDataLen = len;
		gAmeNewCmdFlag = TRUE;
	}
	
#endif
	return TRUE;
}
#endif



/******************************************************************************************************************
* for TP layer
*
******************************************************************************************************************/
tReqBufferResult ameRxFirstFrameIndication(uint32 ch, uint16 reqBufferLen, uint8** RxBufferPtr)
{	
	if(reqBufferLen > AME_RX_BUFF_LEN-7)//dec 7, to make sure there is no data overflow
		return BUFFER_OVERFLOW;

	if(gAmeBusyFlag == TRUE)
		return BUFFER_WAIT;
	
	*RxBufferPtr = gAmeRxBuffer;
	return BUFFER_OK;
}


/******************************************************************************************************************
* for TP layer
*
******************************************************************************************************************/
void ameCancelRxData(uint32 ch, uint8 reason)
{
	//reset all upper layer status&datas
	//AmeResetParas();
	
}


/******************************************************************************************************************
* for TP layer
*
******************************************************************************************************************/
void ameTxDataConfirmation(uint32 ch, boolean result, uint8 errId)
{
	if(result == FALSE)
	{
		//reset all upper layer status&datas
		
	}
	else
	{

	}
}


/******************************************************************************************************************
*
*
******************************************************************************************************************/
void ameAppResetTxBuffer()
{
	gAmeTxBuffer.curDataLen = 0;
}


/******************************************************************************************************************
*
******************************************************************************************************************/
uint16 ameAppFillTxBuffer(uint8* data, uint16 len)
{
	if(data == NULL || len>(AME_TX_BUFF_LEN-1) || len == 0)
		return 0;
	
	if((gAmeTxBuffer.curDataLen+len+1) > AME_TX_BUFF_LEN)
		return 0;
	else
		gAmeTxBuffer.curDataLen += diagCpyData(&(gAmeTxBuffer.buffer[1]) + gAmeTxBuffer.curDataLen, data, len);

	return gAmeTxBuffer.curDataLen;
}


/******************************************************************************************************************
*
*
******************************************************************************************************************/
boolean ameTxDataReq(uint32 Ch, uint8* data, uint16 len)
{
#ifdef SW_PHASE_APPLICATION

	if(data == NULL || len > AME_TX_BUFF_LEN)
		return FALSE;

	gAmeBusyFlag = FALSE;
	if(gAmeTraceLevel>AME_TRACE_LEVEL_1)
		AME_TRACE_VALUE1(AME_TRACE, TRACE_INFO, "AME:TX data,sid=0x%0x\r\n", data[0]);
	return Tp_TxDataRequest(TP_DIAG_DEV_ID_AME, data, len);    //if TX is busy, FALSE will be returned
#else
	return FALSE;
#endif
}


/******************************************************************************************************************
* in fact, it's no necessary to keep @para data and @para len, just for extention in future.
*
******************************************************************************************************************/
boolean ameAppTxData(uint32 Ch, uint8* data, uint16 len)
{
	//a small bug:
	//step1:rx one ame cmd
	//step2:before app resp,rx AME_CMD_RESET_MODULE from remote,reset all, at once,rx same ame cmd as step1
	//step3:when app resp,here can't distinguish new same cmd is rxed.
	if(gAmeCmd==0 || gAmeCmd!=gAmeRxBuffer[0] || gAmeTxBuffer.curDataLen==0)
		return FALSE;

	gAmeTxBuffer.buffer[0] = gAmeCmd+0x40;
	return ameTxDataReq(TP_DIAG_DEV_ID_AME, gAmeTxBuffer.buffer, gAmeTxBuffer.curDataLen+1);
}


/******************************************************************************************************************
* All data in gAmeTxBuffer will be TXed.
******************************************************************************************************************/
error_type ameAppTxDataToSoc(request_context_t* ctx, uint16 did, uint8* data, uint16 len)
{
	error_type ret = 0;

#if 0
	gAmeTxBuffer.buffer[0] = (uint8)(didNum>>8);
	gAmeTxBuffer.buffer[1] = (uint8)(didNum);
	diagCpyData(&gAmeTxBuffer.buffer[2], data, len);
	gAmeTxBuffer.curDataLen = len+2;
#else
	diagCpyData(&gAmeTxBuffer.buffer[0], data, len);
	gAmeTxBuffer.curDataLen = len;
#endif

	ret = ipc_gw_send_reply(0, ctx, &gAmeTxBuffer.buffer[0], gAmeTxBuffer.curDataLen);
	
	return ret;
}
/******************************************************************************************************************
*devch:AME ch
*This function is used to set the AME channel
******************************************************************************************************************/
void ameSetChannelInfor(uint8 devch)
{
	gAmeDevCh = devch;
}


/******************************************************************************************************************
* gAmeRxBuffer[0]:ch
* gAmeRxBuffer[1]:sid
* On tp layer, chNum is: func 0, IVI 1, IC 2, AVM 3, AME 4
* On app layer, chNum is: IVI 0, IC 1, AVM 2
* Inside the ame commond, chNum according to tp layer
******************************************************************************************************************/
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


/******************************************************************************************************************
*
*
******************************************************************************************************************/
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

/*========================================= END OF FILE: ame.c ======================================*/
