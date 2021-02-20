/**************************
description: UDS session id process
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/
#include"uds_sid_if.h"

#include"tp_if.h"

#ifdef UDS_WITH_SID0X10
#include"sid0x10_if.h"
#endif

#ifdef UDS_WITH_SID0X11
#include"sid0x11_if.h"
#endif

#ifdef UDS_WITH_SID0X28
#include"sid0x28_if.h"
#endif

#ifdef UDS_WITH_DOWNLOAD
#include"download_if.h"
#endif

#ifdef UDS_WITH_SID0X27
#include"sid0x27_if.h"
#endif

#ifdef UDS_WITH_SID0X31
#include"sid0x31_if.h"
#endif

#ifdef UDS_WITH_SID0X22
#include"sid0x22_if.h"
#endif


void diagForwardReqToApplayer(uint32 taskId,uint8 ch,uint8* rawDataPtr,uint8 *data,uint16 len);

extern uint16 gRespDataLen;
extern boolean gForwardReqToDiagAppFlag;
extern const tUdsCfgInfor gUdsCfgInfor;
extern uint32 gDownloadMaxNumOfBlock;


//only gForwardReqToDiagAppFlag==TRUE, then diagApp can set gGetRespFromDiagAppFlag = TRUE,then sidHook can return NRC_NONE
boolean gGetRespFromDiagAppFlag = FALSE;
uint8 gDiagRespCode = 0;
uint8 *gUDSBufferPtr = NULL;

static tTpChanType gCurChId = TP_CHANNEL_MAX;

boolean diagSaveChId(uint8 ch)
{
	switch(ch)
	{
	case TP_CHANNEL_UDS_FUNC:
	case TP_CHANNEL_UDS_PHY:
		gCurChId = ch;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void diagForwardReqToApplayer(uint32 taskId,uint8 ch,uint8* rawDataPtr,uint8 *data,uint16 len)
{	
	gForwardReqToDiagAppFlag = TRUE;
	gGetRespFromDiagAppFlag = FALSE;
	gUDSBufferPtr = rawDataPtr+1;
	diagSaveChId(ch);

	udsSendMsg(taskId,data,len);
	//udsSendSignal(SYSTEM_SIG_ID_DIAGCMD,0x1902);
}

tUdsNRCType diagSid0x99InnerTest(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret;

	ret = sid0x99MainProcess(ch,data,len);

	return ret;
}

#ifdef UDS_WITH_SID0X10
tUdsNRCType diagSid0x10SessionControl(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsSessionInnerType innerSessionType;
	uint16 timer = 0;
	tUdsNRCType ret;
	
	if(data == NULL)
		return NRC_GENERAL_REJECT;
	data[1] &= 0x7F;
	//check minimum length
	if(len!=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if((ret = sid0x10CheckSesstionType(data[1],(uint32*)&innerSessionType))!=NRC_NONE)
		return ret;
	
	if((ret=sid0x10CheckPreCondition(data,len))!=NRC_NONE)
		return ret;
	
	if((ret=sid0x10SetSession(innerSessionType)) != NRC_NONE)
		return ret;
	
	
	//P2server_max resolution is 1ms
	timer = gUdsCfgInfor.p2ServerMaxTimerValue;
	data[2] = (uint8)((timer&0xFF00)>>8);
	data[3] = (uint8)(timer&0xFF);
	//P2*server_max resolution is 10ms
	timer = gUdsCfgInfor.p2ServerMaxEnhancedTimerValue/10;
	
	data[4] = (uint8)((timer&0xFF00)>>8);
	data[5] = (uint8)(timer&0xFF);

	gRespDataLen = 5;
	return NRC_NONE;
}
#endif

#ifdef UDS_WITH_SID0X11
//if(0x11 02) then sid0x27SecurityLevelInit()
tUdsNRCType diagSid0x11EcuReset(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 resetType = 0;

	if(data == NULL || len !=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	data[1] &= 0x7F;

	resetType = data[1];

	if((ret=sid0x11CheckPreCondition())!=NRC_NONE)
		return ret;
	
	if((ret=sid0x11CheckResetType(resetType))!= NRC_NONE)
		return ret;

	udsSetResetSysType(resetType & 0x7F);
	gRespDataLen = 1;
	ret = NRC_NONE;

	return ret;

}
#endif

tUdsNRCType diagSid0x21ReadDataByLocalDID(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_SERVICE_NOT_SUPPORTED;

	return ret;
}


#ifdef UDS_WITH_SID0X22
tUdsNRCType diagSid0x22ReadDataByDID(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	const t0x22DidBuffer *didList = NULL;
	uint16 payloadLen = 0;
	uint8 i = 0;
	const t0x22DidInfo * didInfo = NULL;
	uint8* p = NULL;
	uint16 sumLen = 0;
	uint16 cpyDataLen = 0;
	uint8 toSocDidCount = 0;
	uint8 toSocDidNumList[UDS_READ_DID_TO_SOD_MAX_LEN] = {0};
	
	if(gForwardReqToDiagAppFlag == FALSE)
	{
		gRespDataLen = 0;
		gGetRespFromDiagAppFlag = FALSE;
		if((ret = sid0x22CheckPreCondition(data,len))!=NRC_NONE)
			return ret;
		
		if(data==NULL|| len<3)
			return NRC_INCORRECT_MESSAGE_LENGTH;

		payloadLen = len -1;
		if(payloadLen%2 != 0)
			return NRC_INCORRECT_MESSAGE_LENGTH;
		
		p = &data[1];

		if( (ret=sid0x22ParseDidList(p,payloadLen,&didList)) != NRC_NONE)
			return ret;

		if(didList!=NULL)
		{

			for(i=0;i<didList->count;i++)
			{	
				didInfo = sid0x22GetDidInfo(didList->buffer[i]);
				if(didInfo!=NULL)
				{
					sumLen += 2;
					sumLen += didInfo->dataLen;
				}
				else
					return NRC_REQUEST_OUT_OF_RANGE; // DID is not supported
			}

			sumLen += 1;// sid length
			if(sumLen >gUdsCfgInfor.udsRxTxBufferLen)
				return NRC_RESPONSE_TOO_LONG;

			sumLen = 0;

			toSocDidNumList[0] = 0x22;
			for(i=0;i<didList->count;i++)
			{	
				didInfo = sid0x22GetDidInfo(didList->buffer[i]);
				if(didInfo!=NULL)
				{
					if(didInfo->fromSocFlag == TRUE)
					{
						toSocDidCount++;
						toSocDidNumList[1] = toSocDidCount;
						toSocDidNumList[(toSocDidCount-1)*2+2] = (uint8)((didList->buffer[i]&0xFF00)>>8);
						toSocDidNumList[(toSocDidCount-1)*2+3] = (uint8)((didList->buffer[i]&0xFF));
						if((toSocDidCount+2) > UDS_READ_DID_TO_SOD_MAX_LEN)
							return NRC_GENERAL_REJECT;
					}
					else
					{
						sumLen += 2;// didNum len
						*p = (uint8)((didList->buffer[i]&0xFF00)>>8);
						*(p+1) = (uint8)((didList->buffer[i]&0xFF));
						p += 2;
						
						if(didInfo->readFnCbk != NULL)
						{
							cpyDataLen = didInfo->readFnCbk(p);
							sumLen += cpyDataLen;
							p += cpyDataLen;
							
						}
						else if(didInfo->nvmAddr != NULL)
						{
							sumLen += didInfo->dataLen; // did DATA len
							p += diagCpyData(p,didInfo->nvmAddr,didInfo->dataLen);
						}
						else
							return NRC_GENERAL_REJECT; //didList cfg error
					
					}
				}
			}

			
			gRespDataLen = sumLen;

			if(toSocDidCount == 0)
			{
				return NRC_NONE;
			}
			else
			{
				return NRC_REQUEST_OUT_OF_RANGE;
			}
			
		}
	}
	else
	{
		if(gGetRespFromDiagAppFlag == FALSE)
			return NRC_RESPONSE_PENDING;
		else
		{
			gForwardReqToDiagAppFlag = FALSE;
			gGetRespFromDiagAppFlag = FALSE;
			return gDiagRespCode;
		}
	}

	return ret;
}


tUdsNRCType diagSid0x2eWriteDataByDID(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	
	if(data==NULL || len < 4)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((ret=sid0x2eCheckPreCondition(data,len))!=NRC_NONE)
		return ret;
	
	ret = sid0x2eWriteDID(data+1,len-1);
	
	diagResetBuffer(&data[3],0,len-3);
	gRespDataLen = 2;
	
	return ret;
}
#endif

#ifdef UDS_WITH_SID0X27
tUdsNRCType diagSid0x27SecurityAccess(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;

	if(data==NULL )
		return NRC_GENERAL_REJECT;
	if((ret=sid0x27CheckMainPreCondition(data,len))!=NRC_NONE)
			return ret;

	ret = sid0x27MainProcess(data,len);
	return ret;
}
#endif

#ifdef UDS_WITH_SID0X28
tUdsNRCType diagSid0x28CommunicationControl(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_SERVICE_NOT_SUPPORTED;

	if(data == NULL || len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	data[1] &= 0x7f;
	if((ret = sid0x28CheckPreCondition(data,len))!=NRC_NONE)
		return ret;

	ret = sid0x28ParseReq(data, len);
	gRespDataLen = 1;

	return ret;
}
#endif


#ifdef UDS_WITH_SID0X31

/*
data[0]:0x31
data[1]:routineCtrlType:1 :startRoutine;2:stopRoutine;3:reqRoutinueResults
data[2]~data[3]:routineCtrlId
data[4]~: routineCtrlOptionRecord
*/
tUdsNRCType diagSid0x31RoutineControl(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	
	
	if(gForwardReqToDiagAppFlag == FALSE)
	{
		gRespDataLen = 0;
		
		if(data==NULL || len<4)
			return NRC_INCORRECT_MESSAGE_LENGTH;	
		
		ret = sid0x31MainProcess(data,len);
		
		if(ret == NRC_RESPONSE_PENDING)
		{
			gForwardReqToDiagAppFlag = TRUE;
			gGetRespFromDiagAppFlag = FALSE;
			gUDSBufferPtr = data+1;
			diagSaveChId(ch);

		}
		
	}
	else
	{
		if(FALSE == gGetRespFromDiagAppFlag)
			ret = NRC_RESPONSE_PENDING;
		else
		{
			gForwardReqToDiagAppFlag = FALSE;
			gGetRespFromDiagAppFlag = FALSE;
			ret = gDiagRespCode;
		}
	}

	return ret;

}
#endif

#ifdef UDS_WITH_DOWNLOAD

//req:
//byte 1:DataFormatIdentifier
//byte 2:AddressAndLengthFormatID
//byte 3:MemoryAddress
//byte 4~:UnCompressedMemorySize
tUdsNRCType diagSid0x34RequestDownload(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;

	if((ret = sid0x34CheckMainPreCondition()) != NRC_NONE)
		return ret;

	if((ret =sid0x34VerifyData(data,len)) != NRC_NONE)
		return ret;
	diagResetBuffer(data+1,0,7);
	//resp: byte 1:LengthFormatIdentifier,byte 2~:MaxNumberOfBlockLength
	data[1] = 0x20;
	data[2] = (uint8)((gDownloadMaxNumOfBlock&0xFF00)>>8);
	data[3] = (uint8)((gDownloadMaxNumOfBlock&0xFF));
	gRespDataLen = 3;
	
	return NRC_NONE;
}



tUdsNRCType diagSid0x35RequestUpload(uint8 ch,  uint8 *data,uint16 len)
{
	// TODO:
	return NRC_SERVICE_NOT_SUPPORTED;
}


tUdsNRCType diagSid0x36TransferData(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;

	if(data == NULL || len<3)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if((ret=sid0x36MainProcess(data,len))!= NRC_NONE)
	{
		return ret;
	}
	else
	{
		diagResetBuffer(data+1,0,8);
		gRespDataLen = 1;
	}
	return NRC_NONE;
}


tUdsNRCType diagSid0x37RequestTransferExit(uint8 ch,  uint8 *data,uint16 len)
{
	tUdsNRCType	ret = NRC_NONE;

	if((ret = sid0x37MainProcess(data, len))!= NRC_NONE)
		return ret;

	diagResetBuffer(data+1,0,7);
	gRespDataLen = 0;

	return NRC_NONE;
}
#endif

tUdsNRCType diagSid0x3eTesterPresent(uint8 ch,  uint8 *data,uint16 len)
{
	if(data == NULL || len!=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((data[1] | 0x80) != 0x80)
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	
	gRespDataLen = 1;

	return NRC_NONE;
}



boolean diagSidResp(boolean dataFromSocFlag,boolean finishFlag,uint8 respCode,  const uint8 *respData, uint16 len)
{
	static uint16 sum_len = 0;
	static boolean bufferIsClearFlag = FALSE;
	uint16 newTotalLen = 0;
	uint16 offSet = 0;
	
	if(gForwardReqToDiagAppFlag == TRUE && gCurChId != TP_CHANNEL_MAX)
	{
		gDiagRespCode = respCode;

		if(respData!=NULL && len!=0)
		{
			if(dataFromSocFlag == FALSE)
				newTotalLen = sum_len+len;
			else
				newTotalLen = sum_len+len+gRespDataLen;// soc resp data len and ioc resp data len 
			
			if(newTotalLen <(gUdsCfgInfor.udsRxTxBufferLen-1))
			{
				if(respData!=NULL)
				{
					offSet = newTotalLen - len;
					if(bufferIsClearFlag == FALSE)
					{
						bufferIsClearFlag = TRUE;
						diagResetBuffer(gUDSBufferPtr,0,gUdsCfgInfor.udsRxTxBufferLen-1);
					}
					diagCpyData(gUDSBufferPtr+offSet,respData,len);
					sum_len = newTotalLen;
				}
			}
			else
			{
				//if buffer exceeds,application will stop resp data,and drop the datas
				bufferIsClearFlag = FALSE;
				sum_len = 0;
				gCurChId = TP_CHANNEL_MAX;
				gDiagRespCode = NRC_RESPONSE_TOO_LONG;//exceed the buffer len
				gRespDataLen = 0;
				gGetRespFromDiagAppFlag = TRUE;
			
				return FALSE;
			}

		}


		if(finishFlag == TRUE)
		{
			bufferIsClearFlag = FALSE;
			if(sum_len!=0)
			{
				gRespDataLen = sum_len;
				sum_len = 0;
			}
			gCurChId = TP_CHANNEL_MAX;
			gGetRespFromDiagAppFlag = TRUE;
		}
		return TRUE;
	}

	sum_len = 0;
	bufferIsClearFlag = FALSE;

	return FALSE;
}


