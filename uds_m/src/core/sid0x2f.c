/**************************
description: public general process APIs for sid 0x2f of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/


#include"sid0x2f_if.h"
#include"sid0x27_if.h"
#include"uds_sid.h"

#ifndef NULL
#define NULL 0
#endif

static sint8 sid0x2fGetIndex(uint32 ch,uint16 did);
const tIOCtrlInfo* sid0x2fGetIoCtrlInfo(uint32 ch,uint16 did);
static boolean sid0x2fSetStatus(uint32 ch,uint16 did,tIOControlParameterType status);
static tUdsNRCType sid0x2fCheckIoCtrlParameter(uint32 ch,const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo);
const tIOCtrlStatus*sid0x2fGetIoCtrlStatus(uint32 ch,uint16 did);

extern void diagForwardReqToApplayer(uint32 taskId,tUdsReqInfor *reqInfor,uint8 *data,uint16 len);

extern const tUdsCfgInfor gUdsCfgInfor;
extern uint16 gRespDataLen[];
extern uint8 gIOCtrlDidNum[];
extern const tIOCtrlInfo *gIOControlInfoArray[];
extern tIOCtrlStatus *gIOCtrlStatusArray[];

void sid0x2fStopAllTest(uint32 ch)
{
	uint8 i=0;
	tUdsReqInfor reqInfor;

	reqInfor.ch = ch;
	reqInfor.data = NULL;
	reqInfor.len = 0;
	reqInfor.reqFromFuncAddrFlag = FALSE;
	reqInfor.reqTick = 0;
	for(i=0;i<gIOCtrlDidNum[ch];i++)
	{
		if( IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU != gIOCtrlStatusArray[ch][i].status)
		{
			if(NULL != gIOControlInfoArray[ch][i].stopTestCbk)
				gIOControlInfoArray[ch][i].stopTestCbk(&reqInfor,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
			gIOCtrlStatusArray[ch][i].status = IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU;
		}
	}
	
}


void sid0x2fStopTest(uint32 ch,tIOCtrlInfo* ioctrlInfo)
{
	sint8 index = 0;
	tUdsReqInfor reqInfor;

	reqInfor.ch = ch;
	reqInfor.data = NULL;
	reqInfor.len = 0;
	reqInfor.reqFromFuncAddrFlag = FALSE;
	reqInfor.reqTick = 0;

	if(ioctrlInfo!=NULL)
	{
		index = sid0x2fGetIndex(ch,ioctrlInfo->did);
		if(index>=0)
		{
			if(gIOCtrlStatusArray[ch][index].status != IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU)
			{
				if(ioctrlInfo->stopTestCbk!=NULL)
					ioctrlInfo->stopTestCbk(&reqInfor,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
				gIOCtrlStatusArray[ch][index].status = IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU;
			}
		}
	}
}



// data: byte0~1: DID, byte2:ioCtrlPara, byte3~more:ioCtrlState[if appear]
tUdsNRCType sid0x2fCheckIoCtrlParameter(uint32 ch,const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo)
{
	tIOControlParameterType io_ctrl_parameter;
	tUdsNRCType ret = NRC_NONE;
	
	if(data!=NULL && len >=3)
	{
		io_ctrl_parameter = data[2];
		
		switch(io_ctrl_parameter)
		{
		case IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU:
		case IOCONTROL_PARA_TYPE_RESET_TO_DEFAULT_OR_INIT_VALUE:
			if(len!=3)
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			break;
		case IOCONTROL_PARA_TYPE_FREEZE_CURRENT_STATE:
			if(len!=3)
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			break;
		case IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT:

			//1TODO: should here to check if currently this actuator had beed started??
			
			// DID[2]+controlPara[1]+controlState[1~8]+controlMask[0~8]
			//check controlState#1~#n in diagApp layer
			if(sid0x2fCheckSpecificPrecondition(ch,ioctrlInfo) == FALSE)
				ret = NRC_CONDITION_NOT_CORRECT;
			else
			{
				if(ioctrlInfo->verifyParaCbk!=NULL)
				{
					if(ioctrlInfo->verifyParaCbk(data,len)== FALSE)
						ret = NRC_REQUEST_OUT_OF_RANGE;
				}
				else
					ret = NRC_GENERAL_REJECT;
			}

			break;
		default:
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}

	}
	else
		ret = NRC_INCORRECT_MESSAGE_LENGTH;
	
	return ret;

}


tUdsNRCType sid0x2fCheckExtracDataOrState(uint32 ch,const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo)
{
	tUdsNRCType ret = NRC_NONE;
	if(NULL!=ioctrlInfo->checkExtraDataOrState)
		return ioctrlInfo->checkExtraDataOrState(data,len); 

	return ret;
}

boolean sid0x2fSetStatus(uint32 ch,uint16 did,tIOControlParameterType status)
{
	sint8 index = 0;
	boolean ret = FALSE;
	
	index = sid0x2fGetIndex(ch,did);
	if(index>=0)
	{
		gIOCtrlStatusArray[ch][index].status = status;
		ret = TRUE;
	}

	return ret;
}

sint8 sid0x2fGetIndex(uint32 ch,uint16 did)
{
	sint8 i = 0;
	for(i=0;i<gIOCtrlDidNum[ch];i++)
	{
		if(gIOControlInfoArray[ch][i].did == did)
			return i;
	}

	return -1;
}

const tIOCtrlInfo* sid0x2fGetIoCtrlInfo(uint32 ch,uint16 did)
{
	uint8 i=0;
	
	for(i=0;i<gIOCtrlDidNum[ch];i++)
	{
		if(gIOControlInfoArray[ch][i].did == did)
		{
			return &gIOControlInfoArray[ch][i];
		}
	}
	
	return NULL;
}

const tIOCtrlStatus*sid0x2fGetIoCtrlStatus(uint32 ch,uint16 did)
{
	sint8 index = 0;
	index = sid0x2fGetIndex(ch,did);
	if(index>=0)
	{
		return &gIOCtrlStatusArray[ch][index];
	}

	return NULL;
}

tUdsNRCType sid0x2fMainProcess(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *payloadPtr = NULL;
	uint16 io_did = 0;
	const tIOCtrlInfo* ioctrlInfo = NULL;
	const tIOCtrlBuffer* respDataBuffer = NULL;
	uint8 ioCtrlPara;
	uint32 ch = reqInfor->ch;
	uint8*data = reqInfor->data;
	uint16 len = reqInfor->len;

	//check min message len
	if(len < 4)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if(sid0x2fCheckMainPreCondition() == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	
	//check did
	io_did = data[1] <<8 | data[2];
	if((ioctrlInfo=sid0x2fGetIoCtrlInfo(ch,io_did)) == NULL)
		return NRC_REQUEST_OUT_OF_RANGE;
	
	//check security
	if((ioctrlInfo->securLevelFilter & sid0x27GetInnerSecurityLevel(ch)) == 0)
		return NRC_SECURITY_ACCESS_REQUEST;
	
	//check session
	//if((ioctrlInfo->sesstionFilter & sid0x10GetCurSession()) ==0)
		//return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
	
	payloadPtr = data+1;
	//check ioCtrl parameter and state
	//data[0]:0x2f,data[1~2]:DID,data[3]:ioCtrlPara,data[4~more]:ioCtrlState
	if((ret=sid0x2fCheckIoCtrlParameter(ch,payloadPtr,len-1,ioctrlInfo)) != NRC_NONE)
		return ret;

	if((ret=sid0x2fCheckExtracDataOrState(ch,payloadPtr,len-1,ioctrlInfo)) != NRC_NONE)
		return ret;

	ioCtrlPara = data[3];

	

	if(ioCtrlPara == IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT)
	{
		if(NULL != ioctrlInfo->startTestCbk)
			respDataBuffer = ioctrlInfo->startTestCbk(reqInfor,payloadPtr,len-1);
		else
			return NRC_GENERAL_REJECT;
	}
	else
	{
		if((NULL != ioctrlInfo->stopTestCbk))
			respDataBuffer = ioctrlInfo->stopTestCbk(reqInfor,ioCtrlPara);
		else
			return NRC_GENERAL_REJECT;
	}

	if(respDataBuffer!=NULL && respDataBuffer->len<(gUdsCfgInfor.udsRxTxBufferLen-1))
	{
		diagCpyData(payloadPtr,respDataBuffer->buffer,respDataBuffer->len);
		gRespDataLen[ch] = respDataBuffer->len;
	}
	else
	{
		if(ioctrlInfo->getRespSyncFlag == FALSE)  //to SOC
		{
			//diagForwardReqToApplayer(gUdsCfgInfor.diagAppTaskId, reqInfor, data, len);
			ret = NRC_RESPONSE_PENDING;
		}
		else
		{
			sid0x2fSetStatus(ch,io_did, IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
			return NRC_GENERAL_REJECT;
		}
	}


	if(sid0x2fSetStatus(ch,ioctrlInfo->did,ioCtrlPara)!= TRUE)
		return NRC_GENERAL_REJECT;

	
	return ret;
}
