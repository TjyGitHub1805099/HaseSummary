/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X10_IF_H
#define _SID_0X10_IF_H
#include"sid0x10.h"

extern void sid0x10Init();
extern tUdsNRCType sid0x10CheckPreCondition();
extern tUdsSessionInnerType sid0x10GetCurSession(uint32 ch);
extern tUdsNRCType sid0x10SetSession(uint32 ch,tUdsSessionInnerType sessionType);
extern tUdsNRCType sid0x10CheckSesstionType(uint8 sessionType,uint32 *innerSessionType);
extern tUdsNRCType sid0x10ChangeSessionStrategy(uint32 ch,tUdsSessionInnerType newSession);
#endif
