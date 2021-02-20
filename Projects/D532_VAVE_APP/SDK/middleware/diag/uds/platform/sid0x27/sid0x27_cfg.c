/**************************
description: public general process APIs for sid 0x27 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include "sid0x27_cfg.h"

//Functional addr not support SID 0X27

uint8  gKeyAuthFailCounter[UDS_CH_MAX_NUM] = {0,0,0};
tUdsSecurityStatus gUdsGlobalSecurityStatus[UDS_CH_MAX_NUM] = {UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE};
tUdsSecurityLevel gUdsSecuLevel[UDS_CH_MAX_NUM] = {UDS_SECURITY_LEVEL_0,UDS_SECURITY_LEVEL_0,UDS_SECURITY_LEVEL_0};
tSid0x27SeedKeyPairInfor gSid0x27SeedKeyPairInfor[UDS_CH_MAX_NUM][UDS_SECURITY_INDEX_MAX];
uint32 gLockTimePreTick[UDS_CH_MAX_NUM] = {0,0,0};
tUdsSecurityStatus gSecurityStatusArray[UDS_CH_MAX_NUM][UDS_SECURITY_INDEX_MAX] = {
	{UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE},
	{UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE},
	{UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE},

};

const tSid0x27CfgInfor gSid0x27CfgInfor = {
	.maxChNum = UDS_CH_MAX_NUM,
	.enableSecurityAccessType1 = TRUE,
	.enableSecurityAccessType3 = TRUE,
	.enableSecurityAccessType5 = TRUE,
	.enableSecurityAccessType7 = TRUE,
	.keyLen = SID_0X27_CFG_SECURITY_SEED_LEN,
	.seedLen = SID_0X27_CFG_SECURITY_KEY_LEN,
	.authFailedMaxCount = SID_0X27_KEY_COUNTER_LIMIT,
	.delayTimerForReAuth = SID_0X27_DELAY_TIMER_MAX,
	.needCheckSessionMatchAccessType = TRUE,
	.continueDelayTimerAfterSysReset = FALSE,
	.nvmBlockIdForLockTimer = 0,//1TODO:if .continueDelayTimerAfterSysReset == TRUE,get this value from Nvm Module
};

//if securityAccess is lock due to auth fail exceeds the max retry num,and Customer request that
//after sysem reset,the lock timer must contine to calculate value according to the value before sys reset,
//here should store this value to EEPROM
//2TODO:NVRAM_START_FLAG
uint32 gSecurityLockTimer[UDS_CH_MAX_NUM] = {0,0,0};
//2TODO:NVRAM_END_FLAG

const tSessionMapAccessTypeArray gSessionMapAccessTypeArray[UDS_CH_MAX_NUM] = {
	{	// IVI
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//after sales: l1
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//Manufacturing: l2
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5,UDS_EXTENDED_DIAG_SESSION|UDS_CODING_SESSION,//ECU_coding	:l3
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7,UDS_PROGRAMMING_SESSION,//ECU_programming l4
	},
	{	// IC
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//after sales: l1
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//Manufacturing: l2
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5,UDS_EXTENDED_DIAG_SESSION|UDS_CODING_SESSION,//ECU_coding	:l3
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7,UDS_PROGRAMMING_SESSION,//ECU_programming l4
	},
	{	// AVM
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//after sales: l1
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION,//Manufacturing: l2
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5,UDS_EXTENDED_DIAG_SESSION|UDS_CODING_SESSION,//ECU_coding	:l3
		UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7,UDS_PROGRAMMING_SESSION,//ECU_programming l4
	},
};

