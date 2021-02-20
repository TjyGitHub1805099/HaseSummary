/**************************
description: public general process APIs for sid 0x31 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/02
**************************/
#include"sid0x31_if.h"
#include"sid0x27_if.h"
#include"sid0x10_if.h"
#include"uds_sid.h"

static sint8 sid0x31GetIndex(uint16 did);
static const tRoutineCtlInfor* sid0x31GetRoutineCtrlInfor(uint16 routineId);
static boolean sid0x31CheckSecurityLevel(const tRoutineCtlInfor* routineCtlInfor);
static tUdsNRCType sid0x31CheckIfStatusConflicted(uint16 did,tRoutineCtrlType routineType);
static boolean sid0x31SetStatus(uint16 did,tRoutineCtrlType status);

extern const tUdsCfgInfor gUdsCfgInfor;
extern uint16 gRespDataLen;

extern uint8 gRoutineCtrlTotalNum;
extern const tRoutineCtlInfor gRoutineCtlInfors[];
extern tRoutineCtrlStatusArray gRoutinueCtrlStatus[];

boolean sid0x31CheckRoutineCtrlType(tRoutineCtrlType routineType)
{	
	boolean ret = TRUE;
	
	switch(routineType)
	{
	case ROUTINE_CTRL_TYPE_START_ROUTINE:
	case ROUTINE_CTRL_TYPE_STOP_ROUTINE:
	case ROUTINE_CTRL_TYPE_REQ_ROUTINE_RESULTS:
		break;
	default:
		ret = FALSE;
	}

	return ret;
}

tUdsNRCType sid0x31CheckIfStatusConflicted(uint16 did,tRoutineCtrlType reqRoutineType)
{
	sint8 index = 0;
	boolean ret = NRC_NONE;
	
	index = sid0x31GetIndex(did);
	if(index >=0)
	{
		if(((reqRoutineType == ROUTINE_CTRL_TYPE_START_ROUTINE) && (ROUTINE_CTRL_STATUS_IN_PROGRESS == gRoutinueCtrlStatus[index].status))
			||((reqRoutineType == ROUTINE_CTRL_TYPE_STOP_ROUTINE) && (ROUTINE_CTRL_STATUS_STOP == gRoutinueCtrlStatus[index].status))
			||((reqRoutineType == ROUTINE_CTRL_TYPE_REQ_ROUTINE_RESULTS) && (ROUTINE_CTRL_STATUS_STOP == gRoutinueCtrlStatus[index].status)))
			ret = NRC_REQUEST_SEQUENCE_ERROR;
	}
	else
		ret = NRC_GENERAL_REJECT;
	
	return ret;
}

const tRoutineCtlInfor* sid0x31GetRoutineCtrlInfor(uint16 routineId)
{
	uint8 i = 0;
	
	while(i<gRoutineCtrlTotalNum)
	{
		if(gRoutineCtlInfors[i].routineId==routineId)
			return &gRoutineCtlInfors[i];
		i++;
	}
	
	return NULL;
}

sint8 sid0x31GetIndex(uint16 did)
{
	sint8 i = 0;
	
	for(i=0;i<gRoutineCtrlTotalNum;i++)
	{
		if(gRoutineCtlInfors[i].routineId == did)
			return i;
	}

	return -1;
}

boolean sid0x31SetStatus(uint16 did,tRoutineCtrlType status)
{
	sint8 index = 0;
	boolean ret = FALSE;
	
	index = sid0x31GetIndex(did);
	if(index>=0)
	{
		if(status != ROUTINE_CTRL_TYPE_STOP_ROUTINE)
			gRoutinueCtrlStatus[index].status = ROUTINE_CTRL_STATUS_IN_PROGRESS;
		else
			gRoutinueCtrlStatus[index].status = ROUTINE_CTRL_STATUS_STOP;
		ret = TRUE;
	}

	return ret;
}

boolean sid0x31CheckSecurityLevel(const tRoutineCtlInfor* routineCtlInfor)
{
	if(routineCtlInfor == NULL)
		return FALSE;

	if(routineCtlInfor->securityLevelFilter != UDS_INNER_SECURITY_LEVEL_0)
	{
		if((routineCtlInfor->securityLevelFilter & sid0x27GetInnerSecurityLevel()) == 0)
			return FALSE;
		else
			return TRUE;
	}

	return TRUE;
}


void sid0x31StopAllTest()
{

	uint8 i = 0;
	for(i=0;i<gRoutineCtrlTotalNum;i++)
	{
		if(gRoutinueCtrlStatus[i].status == ROUTINE_CTRL_STATUS_IN_PROGRESS)
		{
			if(NULL!=gRoutineCtlInfors[i].stopCbk)
				gRoutineCtlInfors[i].stopCbk(NULL,0);
			gRoutinueCtrlStatus[i].status = ROUTINE_CTRL_STATUS_STOP;
		}
	}
}

//data:byte0:0x31;byte1:routineCtrlType;byte2~3:routineDID;byte4~more:routineCtrlOptionRecord
tUdsNRCType sid0x31MainProcess(uint8 ch,uint8*data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	tRoutineCtrlType routineCtrlType;
	uint16 routineId;
	const tRoutineCtlInfor *routineCtlInfor = NULL;
	uint8 *payLoadDataPtr = NULL;
	uint16 payLoadDataLen = 0;
	tRoutineCtrlBuffer *respBuffer = NULL;
	
	if(sid0x31CheckMainPreCondition()!=TRUE)
		return NRC_CONDITION_NOT_CORRECT;

	routineCtrlType = data[1];
	if(sid0x31CheckRoutineCtrlType(routineCtrlType) != TRUE)
		return NRC_REQUEST_OUT_OF_RANGE;

	routineId = (uint16)(data[2]<<8 | data[3]);

	if((routineCtlInfor = sid0x31GetRoutineCtrlInfor(routineId)) == NULL)
		return NRC_REQUEST_OUT_OF_RANGE;

	if((routineCtlInfor->sessionFilter & sid0x10GetCurSession()) == 0)
		return NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;

	if(sid0x31CheckSecurityLevel(routineCtlInfor) == FALSE)
		return NRC_SECURITY_ACCESS_REQUEST;

	if((ret=sid0x31CheckIfStatusConflicted(routineId,routineCtrlType))!=NRC_NONE)
		return ret;

	payLoadDataPtr = data+1;
	payLoadDataLen = len -1;
	
	switch(routineCtrlType)
	{
	case ROUTINE_CTRL_TYPE_START_ROUTINE:
		if(routineCtlInfor->checkPreCondCbk!=NULL)
		{
			if(routineCtlInfor->checkPreCondCbk(payLoadDataPtr,payLoadDataLen) == FALSE)
				return NRC_CONDITION_NOT_CORRECT;
		}

		if(routineCtlInfor->verifyDataCbk != NULL)
		{
			if(routineCtlInfor->verifyDataCbk(payLoadDataPtr,payLoadDataLen)== FALSE)
				return NRC_REQUEST_OUT_OF_RANGE;
		}
		
		if(routineCtlInfor->startCbk!=NULL)
		{
			respBuffer = routineCtlInfor->startCbk(payLoadDataPtr,payLoadDataLen);
		}
		else
			return NRC_GENERAL_REJECT;
		break;
	case ROUTINE_CTRL_TYPE_STOP_ROUTINE:
		if(payLoadDataLen!=3)
			return NRC_INCORRECT_MESSAGE_LENGTH;
		if(routineCtlInfor->stopCbk!=NULL)
		{
			respBuffer = routineCtlInfor->stopCbk(payLoadDataPtr,payLoadDataLen);
		}
		else
			return NRC_GENERAL_REJECT;

		break;
	case ROUTINE_CTRL_TYPE_REQ_ROUTINE_RESULTS:
		if(payLoadDataLen!=3)
			return NRC_INCORRECT_MESSAGE_LENGTH;

		if(routineCtlInfor->reqResultCbk!=NULL)
		{
			respBuffer = routineCtlInfor->reqResultCbk(payLoadDataPtr,payLoadDataLen);
		}
		else
			return NRC_GENERAL_REJECT;
		break;
	default:
		return NRC_REQUEST_OUT_OF_RANGE;
	}

	if(routineCtlInfor->getRespSyncFlag == TRUE)
	{
		if(respBuffer!=NULL && (respBuffer->len < gUdsCfgInfor.udsRxTxBufferLen-1))
		{
			diagCpyData(payLoadDataPtr,respBuffer->buffer,respBuffer->len);
			gRespDataLen = respBuffer->len;
		}
		else
		{
			sid0x31SetStatus(routineId,ROUTINE_CTRL_TYPE_STOP_ROUTINE);
			ret = NRC_GENERAL_REJECT;
		}
	}
	else
		ret = NRC_RESPONSE_PENDING;

	sid0x31SetStatus(routineId,routineCtrlType);

	return ret;
}
