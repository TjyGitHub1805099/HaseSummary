/**************************
description: public general process APIs for sid 0x85 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/4
**************************/
	
#include"sid0x85_if.h"
#include"dtcCore_if.h"


extern const tSid0x85CfgInfor gSid0x85CfgInfor;

tDtcSettingStatus gDtcSettingStatus = DTC_SETTING_STATUS_ON;

void sid0x85SetDTCSettingStatus(tDtcSettingStatus status)
{
	gDtcSettingStatus = status;

	if(status == DTC_SETTING_STATUS_OFF)// it's recommended only disable update DTC status,not mean to diable detecting DTC
		dtcCoreChangeUpdateDtcStatusFlag(FALSE);
	else
		dtcCoreChangeUpdateDtcStatusFlag(TRUE);

	if(NULL!=gSid0x85CfgInfor.statusSwitchCbk)
		gSid0x85CfgInfor.statusSwitchCbk(&status,sizeof(&status));

}

tDtcSettingStatus sid0x85GetDTCSettingStatus()
{
	return gDtcSettingStatus;
}


void sid0x85ResetStatus()
{
	sid0x85SetDTCSettingStatus(DTC_SETTING_STATUS_ON);
}