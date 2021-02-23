/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X28_IF_H
#define _SID_0X28_IF_H
#include"sid0x28.h"

extern void sid0x28Init();
extern tUdsNRCType sid0x28CheckPreCondition(const uint8*data,uint16 len);
extern tUdsNRCType sid0x28MainProcess(tUdsReqInfor *reqInfor);
extern void sid0x28ComCtrlEnableAll();
extern void sid0x28ResetStatus(uint32 ch);
#endif
