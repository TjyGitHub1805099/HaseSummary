/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X27_IF_H
#define _SID_0X27_IF_H
#include"sid0x27.h"

extern void sid0x27Init();
extern tUdsNRCType sid0x27MainProcess(tUdsReqInfor*reqInfor);
extern tUdsNRCType sid0x27CheckMainPreCondition(const uint8*data,uint16 len);
extern void sid0x27ResetStatus(uint32 ch);
extern tInnerSecurityLevel sid0x27GetInnerSecurityLevel(uint32 ch);
extern void sid0x27SecurityLockTimerTick(uint32 ch);
extern uint8 sid0x27GenSeed(uint32 ch,tUdsSecurityAccessType securityAccessType,uint8 *seedBuff,uint8 seedBuffMaxLen);
extern uint8 sid0x27GenKey(uint32 ch,tUdsSecurityAccessType securityAccessType,const uint8 *seed,uint8 seedLen,uint8*keyBuff,uint8 keyBuffMaxLen);

#endif
