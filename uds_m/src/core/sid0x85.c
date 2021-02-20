/**************************
description: public general process APIs for sid 0x85 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/4
**************************/
	
#include"sid0x85_if.h"
#include"dtcCore_if.h"


extern const tSid0x85CfgInfor gSid0x85CfgInfor;

extern tDtcSettingStatus gDtcSettingStatus[];

tUdsNRCType sid0x85MainProcess(tUdsReqInfor *reqInfor)
{
	uint8 status;
	tUdsNRCType ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	boolean enableFlag;

	ret = sid0x85CheckPreCondition();
	if(ret != NRC_NONE)
		return ret;

	status = reqInfor->data[1] & 0x7F;
	switch(status)
	{
	case DTC_SETTING_STATUS_ON:
	case DTC_SETTING_STATUS_OFF:
		enableFlag = (status == DTC_SETTING_STATUS_ON)? TRUE:FALSE;
		break;
	default:
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	// it's recommended only disable update DTC status,not mean to diable detecting DTC
	dtcCoreChangeUpdateDtcStatusFlag(reqInfor->ch,enableFlag);
	
	if(NULL!=gSid0x85CfgInfor.statusSwitchCbk)
		gSid0x85CfgInfor.statusSwitchCbk(reqInfor->ch,&status,sizeof(&status));

	gDtcSettingStatus[reqInfor->ch] = status;
	return ret;

}

tDtcSettingStatus sid0x85GetDTCSettingStatus(uint32 ch)
{
	return gDtcSettingStatus[ch];
}


void sid0x85ResetStatus(uint32 ch)
{
	dtcCoreChangeUpdateDtcStatusFlag(ch,TRUE);
	gDtcSettingStatus[ch] = DTC_SETTING_STATUS_ON;
	
}
