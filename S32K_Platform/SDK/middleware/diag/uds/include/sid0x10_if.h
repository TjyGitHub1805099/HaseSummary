/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X10_IF_H
#define _SID_0X10_IF_H
#include"sid0x10.h"

extern void sid0x10Init();
extern tUdsNRCType sid0x10CheckPreCondition();
extern tUdsSessionInnerType sid0x10GetCurSession();
extern tUdsNRCType sid0x10SetSession(tUdsSessionInnerType sessionType);
extern tUdsNRCType sid0x10CheckSesstionType(uint8 sessionType,uint32 *innerSessionType);
extern tUdsNRCType sid0x10ChangeSessionStrategy(tUdsSessionInnerType newSession);
#endif
