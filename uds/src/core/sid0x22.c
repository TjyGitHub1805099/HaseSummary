/**************************
description: public general process APIs for sid 0x22 and 0x2e of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x22_if.h"
#include"sid0x27_if.h"

#define DID_MAX_NUM 40

extern uint8  g0x22DidTotalNum;
extern const t0x22DidInfo g0x22DidInfor[];

static uint16 s0x22DidBuffer[DID_MAX_NUM];
static t0x22DidBuffer    g0x22DidBuffer   = {0,s0x22DidBuffer};


const t0x22DidInfo* sid0x22GetDidInfo(uint16 id)
{
	uint8 i = 0;

	for(i=0;i<g0x22DidTotalNum;i++)
	{
		if(g0x22DidInfor[i].id == id)
			return &g0x22DidInfor[i];
	}

	return NULL;
}

tUdsNRCType sid0x22ParseDidList(const uint8*data,uint16 len,const t0x22DidBuffer** dids)
{
	uint16 did = 0;
	uint16 i = 0;
	uint16 m = 0;
	
	if(data ==NULL || len %2 != 0)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	g0x22DidBuffer.count = 0;
	
	while(i<len)
	{
		//did = ((uint16*)data)[i];
		did = (uint16)(data[i]<<8|data[i+1]);
		i+=2;
		//check did. [maybe it's better to use binary search algorithm]
		for(m=0;m<g0x22DidTotalNum;m++)
		{
			if(g0x22DidInfor[m].id == did)
			{
				if((g0x22DidInfor[m].rdwrFlag & DID_READ)==0)
					return NRC_REQUEST_OUT_OF_RANGE;

				if(g0x22DidInfor[m].scrtLevelForRead != UDS_INNER_SECURITY_LEVEL_0)
				{
					if((g0x22DidInfor[m].scrtLevelForRead & sid0x27GetInnerSecurityLevel()) == 0)
						return NRC_SECURITY_ACCESS_REQUEST;
				}
				
				g0x22DidBuffer.buffer[g0x22DidBuffer.count++] = did;
				if((g0x22DidBuffer.count+1) >DID_MAX_NUM)
				{
					//drop the rest DIDs,exceed the buffer
					//g0x22DidBuffer.count = 0;
					//return NRC_RESPONSE_TOO_LONG;
					if(dids!= NULL)
						*dids = &g0x22DidBuffer;
					return NRC_NONE;
				}
				
				break;
			}
		}
		
	}

	if(g0x22DidBuffer.count == 0)//none of the DIDs is supported
		return NRC_REQUEST_OUT_OF_RANGE;

	if(dids!= NULL)
		*dids = &g0x22DidBuffer;
	
	return NRC_NONE;
}



tUdsNRCType sid0x2eWriteDID(uint8*data,uint16 len)
{
	uint16 did = 0;
	uint16 m = 0;
	tUdsNRCType ret = NRC_NONE;
	uint16 didValueLen = 0;
	
	if(data ==NULL || len==0)
		return NRC_GENERAL_REJECT;
	
	did = (uint16)(data[0]<<8|data[1]);
	didValueLen = len-2;
	for(m=0;m<g0x22DidTotalNum;m++)
	{
		if(g0x22DidInfor[m].id == did)
		{
			if((g0x22DidInfor[m].rdwrFlag & DID_WRITE)==0)
				return NRC_REQUEST_OUT_OF_RANGE;
			
			if(g0x22DidInfor[m].scrtLevelForWrite != UDS_INNER_SECURITY_LEVEL_0)
			{
				if((g0x22DidInfor[m].scrtLevelForWrite & sid0x27GetInnerSecurityLevel()) == 0)
					return NRC_SECURITY_ACCESS_REQUEST;
			}
			
			//CHECK precondition,return NRC_CONDITION_NOT_CORRECT

			//check length
			if(g0x22DidInfor[m].isVariousLenFlag == TRUE)
			{
				if(didValueLen > g0x22DidInfor[m].dataLen)
					return NRC_INCORRECT_MESSAGE_LENGTH;
			}
			else
			{
				if(didValueLen != g0x22DidInfor[m].dataLen)
					return NRC_INCORRECT_MESSAGE_LENGTH;
			}
			
			//verify data value
			if(g0x22DidInfor[m].verifyDataFnCbk!=NULL)
			{
				if((ret = g0x22DidInfor[m].verifyDataFnCbk(did,&data[2],didValueLen))!=NRC_NONE)
					return ret;
			}

			if(g0x22DidInfor[m].writeFnCbk != NULL)
			{
				ret = g0x22DidInfor[m].writeFnCbk(did,&data[2],didValueLen);
				if(ret != NRC_NONE)
					return ret;
			}
			else if(g0x22DidInfor[m].nvmAddr != NULL)
				diagCpyData(g0x22DidInfor[m].nvmAddr,&data[2],didValueLen);
			else
				return NRC_GENERAL_REJECT;
			
			if(g0x22DidInfor[m].nvmBlockId != 0)
				diagNvmWriteSync(g0x22DidInfor[m].nvmBlockId, NULL);
			return NRC_NONE;
		}
	}
	
	return NRC_REQUEST_OUT_OF_RANGE;
}


