/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _SID_0X22_H
#define _SID_0X22_H

#include"diagCommon_if.h"

#define UDS_READ_DID_TO_SOD_MAX_LEN 40//bytes

typedef tUdsNRCType (*tVerifyFnCbk)(uint32 ch, uint16 didNum, const uint8* data, uint16 len);
typedef uint16 (*tReadFnCbk)(uint32 ch, uint8* respData);
typedef tUdsNRCType (*tWriteFnCbk)(uint32 ch, uint16 didNum, const uint8* data, uint16 len);


typedef struct{
	uint16 id;
	tRdWrFlag rdwrFlag;
	uint32 scrtLevelForRead;
	uint32 scrtLevelForWrite;
	boolean isVariousLenFlag;
	boolean fromSocFlag;
	uint16 dataLen;
	uint8 *nvmAddr;
	uint8 nvmBlockId;
	boolean privateFlag;
	tVerifyFnCbk verifyDataFnCbk;
	tReadFnCbk readFnCbk;
	tWriteFnCbk writeFnCbk;
}t0x22DidInfo;

typedef struct{
	uint8 count;
	uint16 *buffer;
}t0x22DidBuffer;

#endif
