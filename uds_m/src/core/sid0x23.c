/**************************
description: public general process APIs for sid 0x23 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2019/02/15
**************************/
	
#include"sid0x23_if.h"


extern const tSid0x23CfgInfor gSid0x23CfgInfor;
extern const tUdsCfgInfor gUdsCfgInfor;
extern uint16 gRespDataLen[];

tUdsNRCType sid0x23MainProcess(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 *data;
	uint16 len;
	uint32 memAddr;
	uint16 memSize;
	
	ret = sid0x23CheckPreCondition();
	if(ret != NRC_NONE)
		return ret;

	data = reqInfor->data;
	len = reqInfor->len;
	
	memAddr = (uint32)(data[1]<<24|data[2]<<16|data[3]<<8|data[4]);
	memSize = (uint16)(data[5]<<8|data[6]);

	if( memAddr<gSid0x23CfgInfor.memAddrMini || (memAddr+memSize)>gSid0x23CfgInfor.memAddrMax)
		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	if(memSize >(gUdsCfgInfor.udsRxTxBufferLen-7))
		ret = NRC_REQUEST_OUT_OF_RANGE;
	
	if(ret == NRC_NONE)
	{
		diagResetBuffer(&data[1],0,16);
		diagCpyData(&data[1],(uint8*)memAddr,memSize);
		gRespDataLen[reqInfor->ch] = memSize;
	}
	
		
	return ret;

}
