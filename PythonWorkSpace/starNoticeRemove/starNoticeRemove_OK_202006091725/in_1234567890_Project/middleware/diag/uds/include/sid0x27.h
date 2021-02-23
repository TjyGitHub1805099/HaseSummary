/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X27_H
#define _SID_0X27_H

#include"diagCommon_if.h"


//security state machine
typedef enum{
	//UDS_SECURITY_STATUS_INIT,
	UDS_SECURITY_STATUS_LOCKED,
	UDS_SECURITY_STATUS_NOT_ACTIVE,
	UDS_SECURITY_STATUS_WAIT_FOR_KEY,
	UDS_SECURITY_STATUS_ACTIVE
}tUdsSecurityStatus;


typedef enum{
	UDS_SECURITY_LEVEL_0 = 0,
	UDS_SECURITY_LEVEL_1 = 1,//req_seed:0X01 send_key:0X02; After Sales (Service)
	UDS_SECURITY_LEVEL_2 = 2,//req_seed:0X03 send_key:0X04; EOL (Manufacturing )
	UDS_SECURITY_LEVEL_3 = 3,//req_seed:0X05 send_key:0X06; ECU-Coding
	UDS_SECURITY_LEVEL_4 = 4 //req_seed:0X07 send_key:0X08; ECU-Programming (Flash)
}tUdsSecurityLevel;

typedef enum{
	UDS_INNER_SECURITY_LEVEL_0 = 1,
	UDS_INNER_SECURITY_LEVEL_1 = 2,
	UDS_INNER_SECURITY_LEVEL_2 = 4,
	UDS_INNER_SECURITY_LEVEL_3 = 8,
	UDS_INNER_SECURITY_LEVEL_4 = 16
}tInnerSecurityLevel;


typedef enum{
	UDS_SECURITY_INDEX_0 = 0,
	UDS_SECURITY_INDEX_1 = 1,
	UDS_SECURITY_INDEX_2 = 2,
	UDS_SECURITY_INDEX_3 = 3,
	UDS_SECURITY_INDEX_MAX
}tUdsSecurityIndex;

typedef enum{
	UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1 = 0x1,//after sale
	UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2 = 0x2,
	UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3 = 0x3,//EOL
	UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4 = 0x4,
	UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5 = 0x5,//coding
	UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6 = 0x6,
	UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7 = 0x7,//flashing
	UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8 = 0x8,
	//DIAG_SECURITY_ACCESS_TYPE_REQ_SEED_FOR_ISO26021 = 0x5f,
	//DIAG_SECURITY_ACCESS_TYPE_SEND_KEY_FOR_ISO26021 = 0x60,
}tUdsSecurityAccessType;

typedef struct{
	tUdsSecurityAccessType accessType;
	uint32 innerSession;
}tSessionMapAccessType;

typedef struct{
	tSessionMapAccessType sessionMapAccessType[4]; 
}tSessionMapAccessTypeArray;


typedef struct{
	uint32 maxChNum;
	boolean enableSecurityAccessType1;
	boolean enableSecurityAccessType3;
	boolean enableSecurityAccessType5;
	boolean enableSecurityAccessType7;
	uint8 keyLen;
	uint8 seedLen;
	uint8 authFailedMaxCount;
	uint16 delayTimerForReAuth;
	boolean needCheckSessionMatchAccessType;
	boolean continueDelayTimerAfterSysReset;
	uint8 nvmBlockIdForLockTimer;//get this value from Nvm Module
}tSid0x27CfgInfor;

#define SID_0X27_SEED_BUFF_LEN 64
#define SID_0X27_KEY_BUFF_LEN 64

typedef struct{
	uint8 seed[SID_0X27_SEED_BUFF_LEN];
	uint8 seedLen;
	uint8 key[SID_0X27_KEY_BUFF_LEN];
	uint8 keyLen;
}tSid0x27SeedKeyPairInfor;


#endif
