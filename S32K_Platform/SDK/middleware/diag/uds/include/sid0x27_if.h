/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X27_IF_H
#define _SID_0X27_IF_H
#include"sid0x27.h"

extern void sid0x27Init();
extern tUdsNRCType sid0x27MainProcess(uint8 *data,uint16 len);
extern tUdsNRCType sid0x27CheckMainPreCondition(const uint8*data,uint16 len);
extern void sid0x27ResetStatus();
extern tInnerSecurityLevel sid0x27GetInnerSecurityLevel();
extern void sid0x27SecurityLockTimerTick();
extern uint8 sid0x27GenSeed(tUdsSecurityAccessType securityAccessType,uint8 *seedBuff,uint8 seedBuffMaxLen);
extern uint8 sid0x27GenKey(tUdsSecurityAccessType securityAccessType,const uint8 *seed,uint8 seedLen,uint8*keyBuff,uint8 keyBuffMaxLen);

#endif
