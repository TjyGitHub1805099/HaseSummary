/**************************
description: public general process APIs for sid 0x2f of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/


#include"sid0x2f_if.h"
#include"sid0x27_if.h"
#include"uds_sid.h"

static sint8 sid0x2fGetIndex(uint16 did);
const tIOCtrlInfo* sid0x2fGetIoCtrlInfo(uint16 did);
static boolean sid0x2fSetStatus(uint16 did,tIOControlParameterType status);
static tUdsNRCType sid0x2fCheckIoCtrlParameter(const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo);
const tIOCtrlStatus*sid0x2fGetIoCtrlStatus(uint16 did);

extern void diagForwardReqToApplayer(uint32 taskId,uint8 ch,uint8* rawDataPtr,uint8 *data,uint16 len);

extern const tUdsCfgInfor gUdsCfgInfor;
extern uint16 gRespDataLen;
extern uint8 gIOCtrlDidNum;
extern const tIOCtrlInfo gIOControlInfo[];
extern tIOCtrlStatus gIOCtrlStatus[];

void sid0x2fStopAllTest()
{
	uint8 i=0;

	for(i=0;i<gIOCtrlDidNum;i++)
	{
		if( IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU != gIOCtrlStatus[i].status)
		{
			if(NULL != gIOControlInfo[i].stopTestCbk)
				gIOControlInfo[i].stopTestCbk(IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
			gIOCtrlStatus[i].status = IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU;
		}
	}
	
}


void sid0x2fStopTest(tIOCtrlInfo* ioctrlInfo)
{
	sint8 index = 0;
	
	if(ioctrlInfo!=NULL)
	{
		index = sid0x2fGetIndex(ioctrlInfo->did);
		if(index>=0)
		{
			if(gIOCtrlStatus[index].status != IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU)
			{
				if(ioctrlInfo->stopTestCbk!=NULL)
					ioctrlInfo->stopTestCbk(IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
				gIOCtrlStatus[index].status = IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU;
			}
		}
	}
}



// data: byte0~1: DID, byte2:ioCtrlPara, byte3~more:ioCtrlState[if appear]
tUdsNRCType sid0x2fCheckIoCtrlParameter(const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo)
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
			if(sid0x2fCheckSpecificPrecondition(ioctrlInfo) == FALSE)
				ret = NRC_CONDITION_NOT_CORRECT;
			else
			{
				if(ioctrlInfo->verifyParaCbk!=NULL)
				{
					ret = ioctrlInfo->verifyParaCbk(data,len);
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


tUdsNRCType sid0x2fCheckExtracDataOrState(const uint8 *data,uint16 len,const tIOCtrlInfo* ioctrlInfo)
{
	tUdsNRCType ret = NRC_NONE;
	if(NULL!=ioctrlInfo->checkExtraDataOrState)
		return ioctrlInfo->checkExtraDataOrState(data,len); 

	return ret;
}

boolean sid0x2fSetStatus(uint16 did,tIOControlParameterType status)
{
	sint8 index = 0;
	boolean ret = FALSE;
	
	index = sid0x2fGetIndex(did);
	if(index>=0)
	{
		gIOCtrlStatus[index].status = status;
		ret = TRUE;
	}

	return ret;
}

sint8 sid0x2fGetIndex(uint16 did)
{
	sint8 i = 0;
	for(i=0;i<gIOCtrlDidNum;i++)
	{
		if(gIOControlInfo[i].did == did)
			return i;
	}

	return -1;
}

const tIOCtrlInfo* sid0x2fGetIoCtrlInfo(uint16 did)
{
	uint8 i=0;
	
	for(i=0;i<gIOCtrlDidNum;i++)
	{
		if(gIOControlInfo[i].did == did)
		{
			return &gIOControlInfo[i];
		}
	}
	
	return NULL;
}

const tIOCtrlStatus*sid0x2fGetIoCtrlStatus(uint16 did)
{
	sint8 index = 0;
	index = sid0x2fGetIndex(did);
	if(index>=0)
	{
		return &gIOCtrlStatus[index];
	}

	return NULL;
}

tUdsNRCType sid0x2fMainProcess(uint8 ch,uint8*data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *payloadPtr = NULL;
	uint16 io_did = 0;
	const tIOCtrlInfo* ioctrlInfo = NULL;
	const tIOCtrlBuffer* respDataBuffer = NULL;
	uint8 ioCtrlPara;

	//check min message len
	if(len < 4)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	if(sid0x2fCheckMainPreCondition() == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	
	//check did
	io_did = data[1] <<8 | data[2];
	if((ioctrlInfo=sid0x2fGetIoCtrlInfo(io_did)) == NULL)
		return NRC_REQUEST_OUT_OF_RANGE;
	
	//check security
	if((ioctrlInfo->securLevelFilter & sid0x27GetInnerSecurityLevel()) == 0)
		return NRC_SECURITY_ACCESS_REQUEST;
	
	//check session
	//if((ioctrlInfo->sesstionFilter & sid0x10GetCurSession()) ==0)
		//return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
	
	payloadPtr = data+1;
	//check ioCtrl parameter and state
	//data[0]:0x2f,data[1~2]:DID,data[3]:ioCtrlPara,data[4~more]:ioCtrlState
	if((ret=sid0x2fCheckIoCtrlParameter(payloadPtr,len-1,ioctrlInfo)) != NRC_NONE)
		return ret;

	if((ret=sid0x2fCheckExtracDataOrState(payloadPtr,len-1,ioctrlInfo)) != NRC_NONE)
		return ret;

	ioCtrlPara = data[3];

	if(ioctrlInfo->getRespSyncFlag == FALSE)  //to SOC
	{
		diagForwardReqToApplayer(gUdsCfgInfor.diagAppTaskId, ch, data, data, len);
		ret = NRC_RESPONSE_PENDING;
	}
	else //IOC
	{
		if(ioCtrlPara == IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT)
		{
			if(NULL != ioctrlInfo->startTestCbk)
				respDataBuffer = ioctrlInfo->startTestCbk(payloadPtr,len-1);
			else
				return NRC_GENERAL_REJECT;
		}
		else
		{
			if((NULL != ioctrlInfo->stopTestCbk))
				respDataBuffer = ioctrlInfo->stopTestCbk(ioCtrlPara);
			else
				return NRC_GENERAL_REJECT;
		}

		if(respDataBuffer!=NULL && respDataBuffer->len<(gUdsCfgInfor.udsRxTxBufferLen-1))
		{
			diagCpyData(payloadPtr,respDataBuffer->buffer,respDataBuffer->len);
			gRespDataLen = respDataBuffer->len;
		}
		else
		{
			sid0x2fSetStatus(io_did, IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU);
			return NRC_GENERAL_REJECT;
		}
	}

	if(sid0x2fSetStatus(ioctrlInfo->did,ioCtrlPara)!= TRUE)
		return NRC_GENERAL_REJECT;

	
	return ret;
}
