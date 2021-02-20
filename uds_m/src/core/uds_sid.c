/**************************
description: UDS session id process
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/
#include"uds_sid_if.h"

#include"tp_if.h"
#include"sid0x99_if.h"

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

#ifdef UDS_WITH_SID0X2F
#include"sid0x2f_if.h"
#endif

#ifdef UDS_WITH_SID0X31
#include"sid0x31_if.h"
#endif

#ifdef UDS_WITH_SID0X22
#include"sid0x22_if.h"
#endif

#ifdef UDS_WITH_SID0X23
#include"sid0x23_if.h"
#endif

#ifdef UDS_WITH_DTC
#include"dtcCore_if.h"
#include"sid0x85_if.h"
#endif

void diagForwardReqToApplayer(uint32 taskId,tUdsReqInfor*reqInfor,uint8 *data,uint16 len);

extern uint16 gRespDataLen[];
extern const tUdsCfgInfor gUdsCfgInfor;
extern uint32 gDownloadMaxNumOfBlock;
extern uint8 gDiagRespCode[];
extern uint32 gUdsRxNewDataTick[];
extern tDiagMutex gUdsSidMutex[];
extern uint8 *gUDSBufferPtr[];

static uint8 gUdsSidBuffer[100];

void diagForwardReqToApplayer(uint32 taskId,tUdsReqInfor*reqInfor,uint8 *data,uint16 len)
{	
	gUDSBufferPtr[reqInfor->ch] = &reqInfor->data[1];
	gUdsSidBuffer[0] = (uint8)(reqInfor->ch&0xFF);
	gUdsSidBuffer[1] = (uint8)((reqInfor->reqTick&0xFF000000)>>24);
	gUdsSidBuffer[2] = (uint8)((reqInfor->reqTick&0xFF0000)>>16);
	gUdsSidBuffer[3] = (uint8)((reqInfor->reqTick&0xFF00)>>8);
	gUdsSidBuffer[4] = (uint8)(reqInfor->reqTick&0xFF);
	//gUdsSidBuffer[5] = (uint8)(reqInfor->data[0]);//sid
	
	if(len>90)
		len = 90;
	
	diagCpyData(gUdsSidBuffer+5, data,len);
	udsSendMsg(taskId,gUdsSidBuffer,len+5);
	
}

tUdsNRCType diagSid0x99InnerTest(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret;
	
	ret = sid0x99MainProcess(reqInfor);
	
	return ret;
}


#ifdef UDS_WITH_SID0X10
tUdsNRCType diagSid0x10SessionControl(tUdsReqInfor*reqInfor)
{
	tUdsSessionInnerType innerSessionType;
	uint16 timer = 0;
	tUdsNRCType ret;
	
	if(reqInfor->data == NULL)
		return NRC_GENERAL_REJECT;
	
	//check minimum length
	if(reqInfor->len!=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if((ret = sid0x10CheckSesstionType(reqInfor->data[1],(uint32*)&innerSessionType))!=NRC_NONE)
		return ret;
	
	if((ret=sid0x10CheckPreCondition(reqInfor->data,reqInfor->len))!=NRC_NONE)
		return ret;
	
	if((ret=sid0x10SetSession(reqInfor->ch,innerSessionType)) != NRC_NONE)
		return ret;
	
	
	//P2server_max resolution is 1ms
	timer = gUdsCfgInfor.p2ServerMaxTimerValue;
	reqInfor->data[2] = (uint8)((timer&0xFF00)>>8);
	reqInfor->data[3] = (uint8)(timer&0xFF);
	//P2*server_max resolution is 10ms
	timer = gUdsCfgInfor.p2ServerMaxEnhancedTimerValue/10;
	
	reqInfor->data[4] = (uint8)((timer&0xFF00)>>8);
	reqInfor->data[5] = (uint8)(timer&0xFF);

	gRespDataLen[reqInfor->ch] = 5;
	return NRC_NONE;
}
#endif

#ifdef UDS_WITH_SID0X11
//if(0x11 02) then sid0x27SecurityLevelInit()
tUdsNRCType diagSid0x11EcuReset(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 resetType = 0;

	if(reqInfor->data == NULL || reqInfor->len !=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	resetType = reqInfor->data[1];

	if((ret=sid0x11CheckPreCondition())!=NRC_NONE)
		return ret;
	
	if(((ret=sid0x11CheckResetType(resetType)) != NRC_NONE))
		return ret;

	udsSetResetSysType(resetType & 0x7F);
	gRespDataLen[reqInfor->ch] = 1;
	ret = NRC_NONE;

	return ret;

}
#endif

tUdsNRCType diagSid0x21ReadDataByLocalDID(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_SERVICE_NOT_SUPPORTED;
	
	return ret;
}


#ifdef UDS_WITH_SID0X22
tUdsNRCType diagSid0x22ReadDataByDID(tUdsReqInfor*reqInfor)
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
	uint32 ch = 0;
	
	if((ret = sid0x22CheckPreCondition(reqInfor->data,reqInfor->len))!=NRC_NONE)
		return ret;
	
	if(reqInfor->data==NULL|| reqInfor->len<3)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	payloadLen = reqInfor->len -1;
	if(payloadLen%2 != 0)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if(sid0x22Check0xDF(reqInfor) == NRC_NONE)
		return NRC_NONE;
	
	ch = reqInfor->ch;
	p = &reqInfor->data[1];

	if( (ret=sid0x22ParseDidList(ch,p,payloadLen,&didList)) != NRC_NONE)
		return ret;

	if(didList!=NULL)
	{

		for(i=0;i<didList->count;i++)
		{	
			didInfo = sid0x22GetDidInfo(ch,didList->buffer[i]);
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
			didInfo = sid0x22GetDidInfo(ch,didList->buffer[i]);
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
						cpyDataLen = didInfo->readFnCbk(ch, p);
						sumLen += cpyDataLen;
						p += cpyDataLen;
						
					}
					else if(didInfo->nvmAddr != NULL)
					{
						sumLen += didInfo->dataLen; // did DATA len
						p += diagCpyData(p, didInfo->nvmAddr, didInfo->dataLen);
					}
					else
						return NRC_GENERAL_REJECT; //didList cfg error
				
				}
			}
		}

		
		gRespDataLen[ch] = sumLen;

		if(toSocDidCount == 0)
		{
			return NRC_NONE;
		}
		else
		{
			if(UDS_WORK_MODE_IN_APP == gUdsCfgInfor.udsWorkMode)
			{
				diagForwardReqToApplayer(gUdsCfgInfor.diagAppTaskId,reqInfor,toSocDidNumList,toSocDidCount*2+2);
				return NRC_RESPONSE_PENDING;
			}
			else
				return NRC_REQUEST_OUT_OF_RANGE;
		}
		
	}
	

	return ret;
}


tUdsNRCType diagSid0x2eWriteDataByDID(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *data;
	uint16 len;

	data = reqInfor->data;
	len = reqInfor->len;
	if(data==NULL || len < 4)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((ret=sid0x2eCheckPreCondition(data,len))!=NRC_NONE)
		return ret;

	if(sid0x2eCheck0xDF(reqInfor) == NRC_NONE)
		return NRC_NONE;
	
	ret = sid0x2eWriteDID(reqInfor->ch,data+1,len-1);
	
	diagResetBuffer(&data[3],0,len-3);
	gRespDataLen[reqInfor->ch] = 2;
	
	return ret;
}
#endif

#ifdef UDS_WITH_SID0X23
tUdsNRCType diagSid0x23ReadMemByAddr(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *data;
	uint16 len;

	data = reqInfor->data;
	len = reqInfor->len;
	if(data==NULL || len <7)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	ret=sid0x23MainProcess(reqInfor);
		
	return ret;
}
#endif


#ifdef UDS_WITH_SID0X27
tUdsNRCType diagSid0x27SecurityAccess(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;

	if(reqInfor->data==NULL )
		return NRC_GENERAL_REJECT;
	if((ret=sid0x27CheckMainPreCondition(reqInfor->data,reqInfor->len))!=NRC_NONE)
			return ret;

	ret = sid0x27MainProcess(reqInfor);
	return ret;
}
#endif

#ifdef UDS_WITH_SID0X28
tUdsNRCType diagSid0x28CommunicationControl(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_SERVICE_NOT_SUPPORTED;

	if(reqInfor->data == NULL || reqInfor->len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((ret = sid0x28CheckPreCondition(reqInfor->data,reqInfor->len))!=NRC_NONE)
		return ret;

	ret = sid0x28MainProcess(reqInfor);
	gRespDataLen[reqInfor->ch] = 1;

	return ret;
}
#endif


#ifdef UDS_WITH_SID0X2F

/*
byte 0: 2F
byte 1~2: iocontrol DID
byte 3: iocontrol parameter: returnControlToECU[0],ResetToDefaultOrInitValue[1],FreezeCurrentState[2],ShortTermAdjustment[3]
byte 4: control state1{used to control test timer: 01-0xFA Time, 0xFF:Infinite time}
byte 5~7: other common Control state if necessary
*/
tUdsNRCType diagSid0x2fIOControl(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;	

	if(sid0x2fCheck0xDF(reqInfor) == NRC_NONE)
		return NRC_NONE;

	ret = sid0x2fMainProcess(reqInfor);

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
tUdsNRCType diagSid0x31RoutineControl(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	
	if(reqInfor->data==NULL || reqInfor->len<4)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if(sid0x31Check0xDF(reqInfor)!=NRC_NONE)
		ret = sid0x31MainProcess(reqInfor);

	return ret;

}
#endif

#ifdef UDS_WITH_DOWNLOAD

//req:
//byte 1:DataFormatIdentifier
//byte 2:AddressAndLengthFormatID
//byte 3:MemoryAddress
//byte 4~:UnCompressedMemorySize
tUdsNRCType diagSid0x34RequestDownload(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *data = reqInfor->data;
	
	if((ret = sid0x34CheckMainPreCondition()) != NRC_NONE)
		return ret;

	if((ret =sid0x34VerifyData(reqInfor)) != NRC_NONE)
		return ret;
	diagResetBuffer(reqInfor->data+1,0,7);
	//resp: byte 1:LengthFormatIdentifier,byte 2~:MaxNumberOfBlockLength
	data[1] = 0x20;
	data[2] = (uint8)((gDownloadMaxNumOfBlock&0xFF00)>>8);
	data[3] = (uint8)((gDownloadMaxNumOfBlock&0xFF));
	gRespDataLen[reqInfor->ch] = 3;
	
	return NRC_NONE;
}



tUdsNRCType diagSid0x35RequestUpload(tUdsReqInfor *reqInfor)
{
	// TODO:
	return NRC_SERVICE_NOT_SUPPORTED;
}


tUdsNRCType diagSid0x36TransferData(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	

	if(reqInfor->data == NULL || reqInfor->len<3)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if((ret=sid0x36MainProcess(reqInfor))!= NRC_NONE)
	{
		return ret;
	}
	else
	{
		diagResetBuffer(reqInfor->data+1,0,8);
		gRespDataLen[reqInfor->ch] = 1;
	}
	return NRC_NONE;
}


tUdsNRCType diagSid0x37RequestTransferExit(tUdsReqInfor*reqInfor)
{
	tUdsNRCType	ret = NRC_NONE;

	if((ret = sid0x37MainProcess(reqInfor))!= NRC_NONE)
		return ret;

	diagResetBuffer(reqInfor->data+1,0,7);
	gRespDataLen[reqInfor->ch] = 0;

	return NRC_NONE;
}
#endif

tUdsNRCType diagSid0x3eTesterPresent(tUdsReqInfor*reqInfor)
{
	if(reqInfor->data == NULL || reqInfor->len!=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((reqInfor->data[1] | 0x80) != 0x80)
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	
	gRespDataLen[reqInfor->ch] = 1;

	return NRC_NONE;
}


#ifdef UDS_WITH_DTC
tUdsNRCType diagSid0x14ClearDTC(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;

#ifdef UDS_WITH_DTC
	if(reqInfor->data == NULL || reqInfor->len != 4)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	ret = dtcCoreClearDtc(reqInfor);
	if(ret == NRC_NONE)
		gRespDataLen[reqInfor->ch] = 0;
#endif
	return ret;
}


tUdsNRCType diagSid0x85DTCSetting(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;

	if(reqInfor->data == NULL || reqInfor->len !=2)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	if((ret=sid0x85MainProcess(reqInfor)) != NRC_NONE);
		return ret;
	
	gRespDataLen[reqInfor->ch] = 1;

	return ret;
}


tUdsNRCType diagSid0x19ReadDtc(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;

#ifdef SW_PHASE_APPLICATION

	if(reqInfor->data==NULL || reqInfor->len < 2)
		return NRC_INCORRECT_MESSAGE_LENGTH;

	if((ret=sid0x19CheckPreCondition(reqInfor->data,reqInfor->len)) != NRC_NONE)
		return ret;
	gRespDataLen[reqInfor->ch] = 0;
	ret=dtcCoreReadDtcInfor(reqInfor);
#endif
	return ret;
}

#endif


tUdsNRCType diagSidResp(tUdsRespInfor *respInfor)
{
	static uint16 sum_len[10] = {0};
	static boolean bufferIsClearFlag[10] = {0};
	uint16 newTotalLen = 0;
	uint16 offSet = 0;
	tUdsChStatus udsChStatus;
	uint32 ch = respInfor->ch;
	boolean dataFromSocFlag = respInfor->dataFromSocFlag;
	boolean finishFlag = respInfor->finishFlag;
	uint32 respCode = respInfor->respCode;
	const uint8 *respData = respInfor->data;
	uint16 len = respInfor->len;

	if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
	{
		diagMutexLock(gUdsSidMutex[ch]);
		if(udsGetChStatus(ch,&udsChStatus) == FALSE || udsChStatus != UDS_CH_STATUS_PENDING || respInfor->recordTick != gUdsRxNewDataTick[ch])
		{
			sum_len[ch] = 0;
			bufferIsClearFlag[ch] = FALSE;
			gDiagRespCode[ch] = NRC_STOP_PROCESS;
		}
		else
		{
			gDiagRespCode[ch] = respCode;

			if(respData!=NULL && len!=0)
			{
				if(dataFromSocFlag == FALSE)
					newTotalLen = sum_len[ch]+len;
				else
					newTotalLen = sum_len[ch]+len+gRespDataLen[ch];// soc resp data len and ioc resp data len 
				
				if(newTotalLen <(gUdsCfgInfor.udsChInfor->bufferMaxLen-1))
				{
					offSet = newTotalLen - len;
					if(bufferIsClearFlag[ch] == FALSE)
					{
						bufferIsClearFlag[ch] = TRUE;
						diagResetBuffer(&gUDSBufferPtr[ch][0],0,gUdsCfgInfor.udsChInfor->bufferMaxLen-1);
					}
					diagCpyData(&gUDSBufferPtr[ch][0]+offSet,respData,len);
					sum_len[ch] = newTotalLen;
				}
				else
				{
					//if buffer exceeds,application will stop resp data,and drop the datas
					bufferIsClearFlag[ch] = FALSE;
					sum_len[ch] = 0;
					gDiagRespCode[ch] = NRC_RESPONSE_TOO_LONG;//exceed the buffer len
					gRespDataLen[ch] = 0;
				}

			}


			if(finishFlag == TRUE)
			{
				bufferIsClearFlag[ch] = FALSE;
				if(sum_len[ch]!=0)
				{
					gRespDataLen[ch] = sum_len[ch];
					sum_len[ch] = 0;
					gUdsRxNewDataTick[ch] = 0;
				}
			}
		}

		diagMutexUnlock(gUdsSidMutex[ch]);
	}
	
	//sum_len[ch] = 0;
	//bufferIsClearFlag[ch] = FALSE;

	return gDiagRespCode[ch];
}


