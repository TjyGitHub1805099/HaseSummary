/**
* @file:    ame_Cbk.c
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
#include "diagAppIpc_cbk.h"
#include "Task_IpcApp_Cfg.h"
#include "sid0x2f_if.h"

/*------------------------------ END: USER INCLUDE ------------------------------*/


boolean gSecurityPassFlag = FALSE;
boolean gAMEGuard1 = FALSE;
boolean gAMEGuard2 = FALSE;
boolean gAMEGuard3 = FALSE;


extern uint16 gArray1len;
extern uint16 gArray2len;
extern uint16 gArray3len;
extern const uint8 array1[];
extern const uint8 array2[];
extern const uint8 array3[];

//extern const tAmeIOCtrlDidInfo gAmeIOCtrlDidInfo[];
//extern uint16 gAmeIOCtrlDidTotalNum;



/****************************************************************************************************************
*
****************************************************************************************************************/
tUdsNRCType ameCmpData(uint8* req, const uint8* array, uint8 len)
{
	uint8 i = 0;
	for(i=0; i<len; i++)
	{
		if(req[i] != array[i])
			return NRC_REQUEST_OUT_OF_RANGE;
	}
	return NRC_NONE;
}

#if 0
/****************************************************************************************************************
* Before writing, you need to verify conditions.
****************************************************************************************************************/
tUdsNRCType ameVerifyGuard(uint8* didValue, uint16 len)
{
	uint16 i = 0;
	tUdsNRCType ret = NRC_NONE;

	
	if(gAMEGuard1 == FALSE)
	{
		if(len == gArray1len)
		{
			if((ret=ameCmpData(didValue, array1, gArray1len)) != NRC_NONE)
				return ret;
			gAMEGuard1 = TRUE;
		}
		else
			return NRC_REQUEST_OUT_OF_RANGE;
	}
	else if(gAMEGuard2 == FALSE)
	{
		if(len == gArray2len)
		{
			if((ret=ameCmpData(didValue, array2, gArray2len)) != NRC_NONE)
			{
				gAMEGuard1 = FALSE;
				return ret;
			}

			gAMEGuard2 = TRUE;
		}
		else
		{
			gAMEGuard1 = FALSE;
			return NRC_REQUEST_OUT_OF_RANGE;
		}
	}
	else if(gAMEGuard3 == FALSE)
	{
		if(len == gArray3len)
		{
			if((ret=ameCmpData(didValue, array3, gArray3len))!= NRC_NONE)
			{
				gAMEGuard1 = FALSE;
				gAMEGuard2 = FALSE;
				return ret;
			}
			
			gAMEGuard3 = TRUE;
		}			
		else
		{
			gAMEGuard1 = FALSE;
			gAMEGuard2 = FALSE;				
			return NRC_REQUEST_OUT_OF_RANGE;
		}
	}

	gSecurityPassFlag = TRUE;
	return NRC_NONE;
}

#endif

/****************************************************************************************************************
* 
****************************************************************************************************************/
uint32 ameParseDevChannel(uint32 function)
{
	uint32 ret = 0;
	switch(function)
	{
		case IVI_IPC_UDS_READ:
		case IVI_IPC_UDS_WRITE:
		case IVI_IPC_UDS_CTRL:
			ret = UDS_IVI;
			break;
		case IC_IPC_UDS_READ:
		case IC_IPC_UDS_WRITE:
		case IC_IPC_UDS_CTRL:
			ret = UDS_IC;
			break;
		case AVM_IPC_UDS_READ:
			ret = UDS_AVM;
			break;
		default:
			break;
	}
	return ret;
}


/****************************************************************************************************************
* make uint8 array just like '0xFF 0xFF 0xEE 0xEE 0xDD 0xDD 0xCC 0xCC'
* to uint16 array just like '0xFFFF 0xEEEE 0xDDDD 0xCCCC' (0xFFFF represents a DID)
****************************************************************************************************************/
tUdsNRCType ameRWParseDidList(uint32 ch, const uint8* data, uint16 len, const t0x22DidBuffer** dids)
{
	return sid0x22ParseDidList(ch, data, len, dids);
}


/****************************************************************************************************************
* 
****************************************************************************************************************/
t0x22DidInfo* ameRWGetDidInfo(uint32 devCh, uint16 did)
{
	return sid0x22GetDidInfo(devCh, did);
}


/****************************************************************************************************************
* 
****************************************************************************************************************/
/*
tAmeIOCtrlDidInfo* ameIOControlGetDidInfo(uint16 did)
{
	uint8 i = 0;
	
	for(i=0; i<gAmeIOCtrlDidTotalNum; i++)
	{
		if(gAmeIOCtrlDidInfo[i].id == did)
		{
			return &gAmeIOCtrlDidInfo[i];
		}
	}
	
	return NULL;
}
*/

tIOCtrlInfo* ameIOControlGetDidInfo(uint32 ch, uint16 did)
{
	return sid0x2fGetIoCtrlInfo(ch, did);
}


/****************************************************************************************************************
* data[0] = SID
* data[1] = DID_H
* data[2] = DID_L
****************************************************************************************************************/
tUdsNRCType ameReadData(uint32 devCh, uint8* data, uint16 len)
{
	tUdsNRCType respCode = NRC_NONE;
	static request_context_t ctx;
	uint16 i = 0;
	const t0x22DidBuffer* didList = NULL;   //The memory space is already there, it is g0x22DidBuffer, 40 bytes.
    const t0x22DidInfo* ameDidInfo = NULL;
	uint16 sumLen = 0;   //Not participating in the sending process, just for Verifying total length.
	uint16 cpyDataLen = 0;
	uint8  toSocDidCount = 0;
	uint8  toSocDidNumListWithSid[UDS_READ_DID_TO_SOD_MAX_LEN] = {0};
	uint16 dataLenInTxBuf = 0;
	tAmeTxBuffer* ameTxBufferPtr = NULL;
	
	if(data==NULL || len<3 || ((len-1)%2!=0))
		return NRC_INCORRECT_MESSAGE_LENGTH;

	ameTxBufferPtr = ameGetTxBuffer();
	ameAppResetTxBuffer();

	if((respCode = ameRWParseDidList(devCh, data+1, len-1, &didList)) != NRC_NONE)
		return respCode;

	if(didList != NULL)
	{
//---------------Compare total length and AME_TX_BUF length----------------------------
		for(i=0; i< didList->count; i++)
		{	
			ameDidInfo = ameRWGetDidInfo(devCh, didList->buffer[i]);
			if(ameDidInfo != NULL)
			{
				sumLen += 2;
				sumLen += ameDidInfo->dataLen;
			}
			else
				return NRC_REQUEST_OUT_OF_RANGE;
		}

		sumLen += 1;     // sid length
		if(sumLen >AME_TX_BUFF_LEN)
			return NRC_RESPONSE_TOO_LONG;
//---------------------------------------------------------------------------------------
		
		toSocDidNumListWithSid[0] = SID_READ_DATA_BY_DID;    // sid
		
		for(i=0; i< didList->count; i++)
		{	
			ameDidInfo = ameRWGetDidInfo(devCh, didList->buffer[i]);
			if(ameDidInfo != NULL)
			{
				if(ameDidInfo->fromSocFlag == TRUE)
				{
					toSocDidCount++;
					toSocDidNumListWithSid[1] = toSocDidCount;
					diagCpyData(&toSocDidNumListWithSid[(toSocDidCount-1)*2+2], &data[i*2+1], 2);
					if((toSocDidCount+2) > UDS_READ_DID_TO_SOD_MAX_LEN)
						return NRC_GENERAL_REJECT;
				}
				else
				{
					//ameAppReveFillTxBuffer((uint8*)&didList->buffer[i], 2);
					ameAppFillTxBuffer(&data[i*2+1], 2);
					if(ameDidInfo->readFnCbk != NULL)
					{
						cpyDataLen = ameDidInfo->readFnCbk(devCh, ameTxBufferPtr->buffer+(1+ameTxBufferPtr->curDataLen));
						ameTxBufferPtr->curDataLen += cpyDataLen;
					}
					else if(ameDidInfo->nvmAddr != NULL)
					{
						dataLenInTxBuf = ameAppFillTxBuffer(ameDidInfo->nvmAddr, ameDidInfo->dataLen);
					}
				}
			}
		}

//---------------------------------------------------------------------------------------
		if(toSocDidCount == 0)
		{
			ameAppTxData(TP_DIAG_DEV_ID_AME, ameTxBufferPtr->buffer, ameTxBufferPtr->curDataLen);
			return NRC_NONE;
		}
		else
		{			
			ctx.channel = IPC_CHANNEL_DIAG;
			ctx.function_id = IVI_IPC_AME_READ;
			ctx.sequence_number = 0;
			ipc_gw_send_request(0, IPC_CHANNEL_DIAG, IVI_IPC_AME_READ, toSocDidNumListWithSid, toSocDidCount*2+2, &ctx, ameIpcCbk_McuRequestSocReply);
			return NRC_RESPONSE_PENDING;
		}
		
	}	

	return NRC_NONE;
}


/*****************************************************************************************************************
* dataInfo[0] = SID
* dataInfo[1] = DID_H
* dataInfo[2] = DID_L
*****************************************************************************************************************/
tUdsNRCType ameWriteData(uint32 devCh, uint8* dataInfo, uint16 len)
{
	uint8* didValue = NULL;
	uint16 didValueLen = 0;
	uint16 didNum;
	const t0x22DidInfo* didInfor = NULL;
	tUdsNRCType ret = NRC_NONE;
	static request_context_t ctx;

	if(!dataInfo || len<3 || len>AME_RX_BUFF_LEN)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	//if(gSecurityPassFlag == FALSE)
		//return NRC_SECURITY_ACCESS_REQUEST;
	
	//DIAG_TRACE_TXT(AME_TRACE, TRACE_INFO, "AME:SID 0x2e\r\n");
	didValueLen = len - 3;
	didValue = &dataInfo[3];
	didNum = (uint16)(dataInfo[1]<<8 | dataInfo[2]);


#if 0
//--------------------------------------- Security verification ----------------------------------------
	if(didNum == 0xffff)
	{
		ret = ameVerifyGuard(didValue, didValueLen);
		if(ret== NRC_NONE)
		{
			ameAppResetTxBuffer();
			ameAppFillTxBuffer(&dataInfo[1], 2);
			ameAppTxData(&dataInfo[1], 2);
			return NRC_NONE;
		}
		else
			return ret;
	}
	else if(gSecurityPassFlag == FALSE)
		return NRC_SECURITY_ACCESS_REQUEST;
//------------------------------------------------------------------------------------------------------
#endif


	didInfor = ameRWGetDidInfo(devCh, didNum);
	if(didInfor != NULL)
	{	
		if((didInfor->rdwrFlag & DID_AME_WRITE) == 0)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		
		if(didInfor->isVariousLenFlag == TRUE)
		{
			if(didValueLen > didInfor->dataLen)
				return NRC_INCORRECT_MESSAGE_LENGTH;
		}
		else
		{
			if(didValueLen != didInfor->dataLen)
				return NRC_INCORRECT_MESSAGE_LENGTH;	
		}

		if(didInfor->verifyDataFnCbk != NULL)
		{
			if((ret = didInfor->verifyDataFnCbk(devCh, didNum, didValue, didValueLen)) != NRC_NONE)
				return ret;
		}
		
		if(didInfor->fromSocFlag == TRUE)
		{
			ctx.channel = IPC_CHANNEL_DIAG;
			ctx.function_id = IVI_IPC_AME_WRITE;
			ctx.sequence_number = 0;
			ipc_gw_send_request(0, IPC_CHANNEL_DIAG, IVI_IPC_AME_WRITE, &dataInfo[0], len, &ctx, ameIpcCbk_McuRequestSocReply);
		
			return NRC_RESPONSE_PENDING;
		}
		
		if(didInfor->writeFnCbk != NULL)
		{
			if(didInfor->writeFnCbk(devCh, didNum, didValue, didValueLen) != NRC_NONE)
				return NRC_REQUEST_OUT_OF_RANGE;
		}
		else if(didInfor->nvmAddr != NULL)
		{
			if(didInfor->nvmBlockId != 0)
			{
				diagCpyData(didInfor->nvmAddr, didValue, didValueLen);
				diagNvmWriteSync(didInfor->nvmBlockId, didInfor->nvmAddr);
			}
		}
		else
			return NRC_GENERAL_REJECT;

		ameAppResetTxBuffer();
		ameAppFillTxBuffer(&dataInfo[1], 2);
		ameAppTxData(TP_DIAG_DEV_ID_AME, &dataInfo[1], 2);
	}
	else
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;

    return NRC_NONE;
}


/*****************************************************************************************************************
* req[0] = SID
* req[1] = DID_H
* req[2] = DID_L
*****************************************************************************************************************/
tUdsNRCType ameIOControl(uint32 devCh, uint8* req, uint16 len)
{
#if 0
	uint8* didValue = NULL;
	uint16 didValueLen = 0;
	uint16 didNum;
	const tIOCtrlInfo* didInfor = NULL;
	tUdsNRCType ret = NRC_NONE;
	static request_context_t ctx;

	if(!req ||len <3|| len > AME_RX_BUFF_LEN)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	didValueLen = len -3;
	didValue = &req[3];
	didNum = (uint16)(req[1]<<8|req[2]);
	
	didInfor = ameIOControlGetDidInfo(ch, didNum);
	if(didInfor != NULL)
	{	
		if(didInfor->getRespSyncFlag == FALSE)
		{
			ctx.channel = IPC_CHANNEL_DIAG;
			ctx.function_id = IVI_IPC_AME_CTRL;
			ctx.sequence_number = 0;
			ipc_gw_send_request(0, IPC_CHANNEL_DIAG, IVI_IPC_AME_CTRL, &req[0], len, &ctx, ameIpcCbk_McuRequestSocReply);

			return NRC_RESPONSE_PENDING;
		}
		else
		{
			if(didInfor->startTestCbk != NULL)
			{
				if(didInfor->startTestCbk(req, len) != NRC_NONE)
					return NRC_REQUEST_OUT_OF_RANGE;
			}
			else
				return NRC_GENERAL_REJECT;
		}
		
		ameAppResetTxBuffer();
		ameAppFillTxBuffer(&req[1], 2);
		ameAppTxData(&req[1], 2);
	}
	else
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;

    return NRC_NONE;
#endif
}


/*****************************************************************************************************************
* data[0] = SID
* data[1] = sub function
*****************************************************************************************************************/
tUdsNRCType ameSecurityAccess(uint32 devCh, uint8* data, uint16 len)
{
#if 0
	uint8 subFunc = data[1];
	static uint8 seedBuff[AME_SEED_LEN] = {0};
	uint8 retKey[AME_KEY_LEN] = {0};
	uint8 retBuf[AME_KEY_LEN+1] = {0};
	uint32 seed = 0;
	tUdsNRCType ret = NRC_NONE;

	if(!data ||len <2)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(subFunc == 0x01)
	{
		if(len != 2)
			return NRC_INCORRECT_MESSAGE_LENGTH;
		
		seed = diagGetRandomNum();
		seedBuff[0] = (uint8)((seed&0xFF000000)>>24);
		seedBuff[1] = (uint8)((seed&0xFF0000)>>16);
		seedBuff[2] = (uint8)((seed&0xFF00)>>8);
		seedBuff[3] = (uint8)((seed&0xFF));

		retBuf[0] = subFunc;
		diagCpyData(&retBuf[1], seedBuff, AME_SEED_LEN);
		
		ameAppResetTxBuffer();
		ameAppFillTxBuffer(retBuf, AME_SEED_LEN+1);
		ameAppTxData(retBuf, AME_SEED_LEN+1);

		return NRC_NONE;
	}
	else if(subFunc == 0x02)
	{
		if(len != 6)
			return NRC_INCORRECT_MESSAGE_LENGTH;
		
		computeAmeKey(seedBuff, retKey);
		if(ameCmpData(retKey, &data[2], AME_KEY_LEN) != NRC_NONE)
		{
			return NRC_INVALID_KEY;
		}
		else
		{
			gSecurityPassFlag = TRUE;
			retBuf[0] = subFunc;
			ameAppResetTxBuffer();
			ameAppFillTxBuffer(retBuf, 1);
			ameAppTxData(retBuf, 1);
		}
	}
	else
	{
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}
	
	return NRC_NONE;
#endif
}


/*****************************************************************************************************************
* rx_data[0] = SID
* rx_data[1] = DID_H
* rx_data[2] = DID_L
* When response return from SOC, this function will be called automatically
*****************************************************************************************************************/
void ameIpcCbk_McuRequestSocReply(request_context_t* ctx, uint16 function, uint8* rx_data, uint32 len)
{
	uint32 devCh = 0;

	tAmeTxBuffer* ameTxBufferPtr = NULL;
	
	ameTxBufferPtr = ameGetTxBuffer();    // get buf
	
	ameAppResetTxBuffer();       // clear buf
	
	ameAppFillTxBuffer(&rx_data[1], len-1);      // fill buf

	devCh = ameParseDevChannel(function);
	ameAppTxData(devCh, ameTxBufferPtr->buffer, ameTxBufferPtr->curDataLen);     // Tx buf data
	
	return;
}


/*****************************************************************************************************************
* rx_data[0] = SID
* rx_data[1] = DID_H
* rx_data[2] = DID_L
*****************************************************************************************************************/
void ameIpcCbk_SocReadMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len)
{
	uint32 devCh = 0;
	t0x22DidInfo* ameDidInfo = NULL;
	uint16 did = 0;
	
	if((len<3)||(rx_data==NULL))
	{
		return;
	}

#if 0
	switch(ctx->function_id)
	{
		case IPC_DIAG_FUNCTION_AME_READ_ECU_SERIAL_NUM:
			did = DID_ECU_SER_NUM;
			break;
		case IPC_DIAG_FUNCTION_AME_READ_MODULE_CONFIG_ITEM:
			did = DID_MODULE_CONFIGURABLE_ITEM;
			break;
		case IPC_DIAG_FUNCTION_AME_READ_ECU_SW_VERSION_NUM:
			did = DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM;
			break;
		default:
			break;
	}
#endif
	
	devCh = ameParseDevChannel(ctx->function_id);
	ameDidInfo = ameRWGetDidInfo(devCh, did);
	if(ameDidInfo && ameDidInfo->nvmAddr)
		ameAppTxDataToSoc(ctx, did, ameDidInfo->nvmAddr, ameDidInfo->dataLen);
}


/****************************************************************************************************************
* rx_data[0] = SID
* rx_data[1] = DID_H
* rx_data[2] = DID_L
****************************************************************************************************************/
void ameIpcCbk_SocCtrlMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len)
{


}





#if 0

/****************************************************************************************************************
* 
****************************************************************************************************************/

void ameFlushDataToEEPROM()
{
	uint8 i = 0;
	uint16 crc = 0;
	uint8 tmpBuffer[100];

	for(i=0;i<gDidTotalNum;i++)
	{
		if((gDidInfo[i].changedBySocFlag == TRUE) && (gDidInfo[i].nvmAddr!=NULL))
		{
			gDidInfo[i].changedBySocFlag = FALSE;

			diagCpyData(tmpBuffer, gDidInfo[i].nvmAddr, gDidInfo[i].dataLen);
			crc = Crc16_Calculate(tmpBuffer, gDidInfo[i].dataLen);
			tmpBuffer[gDidInfo[i].dataLen] = (uint8)((crc&0xFF00)>>8);
			tmpBuffer[gDidInfo[i].dataLen+1] = (uint8)((crc&0xFF));
			
			switch(gDidInfo[i].id)
			{
			case	DID_ECU_SER_NUM:
				eeprom_write(MEM_ADDR_ECUSerialNumber_1,tmpBuffer,MEM_SIZE_ECUSerialNumber_1);
				eeprom_write(MEM_ADDR_ECUSerialNumber_2,tmpBuffer,MEM_SIZE_ECUSerialNumber_2);
				break;
			default:
				break;
			}
		}
	}
}
#endif








/*========================================= END OF FILE: ame_Cbk.c ========================================*/
