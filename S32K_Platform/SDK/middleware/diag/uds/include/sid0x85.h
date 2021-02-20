/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _SID_0X85_H
#define _SID_0X85_H

#include"diagCommon_if.h"


typedef enum{
	DTC_SETTING_STATUS_ON = 1,
	DTC_SETTING_STATUS_OFF = 2
}tDtcSettingStatus;


typedef struct{
	tDiagFnCbkWithPara statusSwitchCbk;
}tSid0x85CfgInfor;

#endif
