/**************************
description: public general process APIs for sid 0x85 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include "sid0x85_cfg.h"

extern void sid0x85StatusSwitchCbk(uint32 ch,void *data,uint16 len);

tDtcSettingStatus gDtcSettingStatus[UDS_CH_MAX_NUM] = {DTC_SETTING_STATUS_ON,DTC_SETTING_STATUS_ON,DTC_SETTING_STATUS_ON};

const tSid0x85CfgInfor gSid0x85CfgInfor = {
	.statusSwitchCbk = sid0x85StatusSwitchCbk,
};