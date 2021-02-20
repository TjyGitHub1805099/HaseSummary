/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X22_IF_H
#define _SID_0X22_IF_H

#include"sid0x22.h"

extern void sid0x22Init();
extern const t0x22DidInfo* sid0x22GetDidInfo(uint16 id);
extern tUdsNRCType sid0x22ParseDidList(const uint8*data,uint16 len,const t0x22DidBuffer** dids);
extern tUdsNRCType sid0x22CheckPreCondition(const uint8*data,uint16 len);
extern tUdsNRCType sid0x2eCheckPreCondition(const uint8*data,uint16 len);
extern tUdsNRCType sid0x2eWriteDID(uint8*data,uint16 len);

#endif
