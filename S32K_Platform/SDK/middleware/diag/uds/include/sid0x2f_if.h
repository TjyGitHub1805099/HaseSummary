/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X2F_IF_H
#define _SID_0X2F_IF_H
#include"sid0x2f.h"

extern void sid0x2fInit();
extern void sid0x2fStopAllTest();
extern tUdsNRCType sid0x2fMainProcess(uint8 ch,uint8*data,uint16 len);
extern boolean sid0x2fCheckMainPreCondition();
extern boolean sid0x2fCheckSpecificPrecondition(const tIOCtrlInfo* ioctrlInfor);

#endif
