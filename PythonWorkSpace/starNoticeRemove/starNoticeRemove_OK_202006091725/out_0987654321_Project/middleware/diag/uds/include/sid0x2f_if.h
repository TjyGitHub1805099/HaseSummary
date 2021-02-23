/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X2F_IF_H
#define _SID_0X2F_IF_H
#include"sid0x2f.h"

extern void sid0x2fInit();
extern void sid0x2fStopAllTest(uint32 ch);
extern tUdsNRCType sid0x2fMainProcess(tUdsReqInfor *reqInfor);
extern boolean sid0x2fCheckMainPreCondition();
extern boolean sid0x2fCheckSpecificPrecondition(uint32 ch,const tIOCtrlInfo* ioctrlInfor);
extern tUdsNRCType sid0x2fCheck0xDF(tUdsReqInfor *reqInfor);
extern const tIOCtrlInfo *sid0x2fGetIoCtrlInfo(uint32 ch,uint16 did);
#endif
