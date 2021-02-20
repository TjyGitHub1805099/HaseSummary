/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X11_IF_H
#define _SID_0X11_IF_H
#include"sid0x11.h"

extern void sid0x11Init();
extern tUdsNRCType sid0x11CheckPreCondition();
extern tUdsNRCType sid0x11CheckResetType(uint8 resetType);
extern void sid0x11ResetSys(uint8 resetType);

#endif
