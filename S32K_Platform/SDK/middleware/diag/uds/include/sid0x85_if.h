/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X85_IF_H
#define _SID_0X85_IF_H
#include"sid0x85.h"

extern void sid0x85Init();
extern void sid0x85SetDTCSettingStatus(tDtcSettingStatus status);
extern tUdsNRCType sid0x85CheckPreCondition();
extern tDtcSettingStatus sid0x85GetDTCSettingStatus();
extern void sid0x85ResetStatus();

#endif
