/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X99_IF_H
#define _SID_0X99_IF_H
#include"diagCommon_if.h"


extern void sid0x99Init();
extern tUdsNRCType sid0x99MainProcess(tUdsReqInfor *reqInfor);
extern tUdsNRCType sid0x99CheckPreCondition();
extern boolean sid0x99_GetEnableTpTraceByCanFlag();
extern void sid0x99_SetEnableTpTraceByCanFlag(boolean flag);
extern boolean sid0x99_Get_InnerTestFlag();
#endif
