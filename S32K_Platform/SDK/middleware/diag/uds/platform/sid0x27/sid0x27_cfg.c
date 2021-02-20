/**************************
description: public general process APIs for sid 0x27 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include "sid0x27_cfg.h"


const tSid0x27CfgInfor gSid0x27CfgInfor = {
	.enableSecurityAccessType1 = TRUE,
	.enableSecurityAccessType3 = TRUE,
	.enableSecurityAccessType5 = TRUE,
	.enableSecurityAccessType7 = TRUE,
	.keyLen = SID_0X27_CFG_SECURITY_SEED_LEN,
	.seedLen = SID_0X27_CFG_SECURITY_KEY_LEN,
	.authFailedMaxCount = SID_0X27_KEY_COUNTER_LIMIT,
	.delayTimerForReAuth = SID_0X27_DELAY_TIMER_MAX,
	.needCheckSessionMatchAccessType = TRUE,
	.continueDelayTimerAfterSysReset = TRUE,
	.nvmBlockIdForLockTimer = 0,//1TODO:if .continueDelayTimerAfterSysReset == TRUE,get this value from Nvm Module
};

