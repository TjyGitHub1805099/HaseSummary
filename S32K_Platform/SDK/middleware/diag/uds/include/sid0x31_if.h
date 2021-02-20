/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X31_IF_H
#define _SID_0X31_IF_H
#include"sid0x31.h"

extern void sid0x31Init();
extern boolean sid0x31CheckRoutineCtrlType(tRoutineCtrlType routineType);
extern void sid0x31StopAllTest();
extern tUdsNRCType sid0x31MainProcess(uint8*data,uint16 len);
extern boolean sid0x31CheckMainPreCondition();
#endif
