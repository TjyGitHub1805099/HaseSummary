/**************************
description: public general process APIs for sid 0x27 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x27_if.h"
#include"sid0x10_if.h"

void sid0x27ResetKeyAndSeed();
static boolean sid0x27ConvertAccessTypeToIndex(tUdsSecurityAccessType accessType,uint8 *index);
static tSid0x27SeedKeyPairInfor* sid0x27GetSeed(tUdsSecurityAccessType accessType);
static tSid0x27SeedKeyPairInfor* sid0x27GetKey(tUdsSecurityAccessType accessType);
static boolean sid0x27SetSecurityLevel(tUdsSecurityLevel level);
static tUdsNRCType sid0x27VerifyKey(tUdsSecurityAccessType securityAccessType,const uint8* keyPtr,uint16 keyLen,uint8 index);
static tUdsNRCType sid0x27CheckDataLen(uint8 securityAccessType,uint16 len);
static tUdsNRCType sid0x27CheckAccessTypeAndDataLen(const uint8 *data,uint16 payloadLen);
static void sid0x27ResetKeyFailedCounter();
static void sid0x27ResetLockTimer();
static tUdsNRCType sid0x27AddAuthFailedCounter(tUdsSecurityAccessType securityAccessType);
static tUdsSecurityStatus sid0x27GetSecurityStatus();
static tUdsNRCType sid0x27MatchSessionAndAccessType(tUdsSecurityAccessType accessType);
static boolean sid0x27SetSecurityStatus(tUdsSecurityStatus securityStatus);


extern uint16 gRespDataLen;
extern const tSid0x27CfgInfor gSid0x27CfgInfor;
extern uint32 gSecurityLockTimer;
extern const uint32 gSessionMapAccessType[][2];
extern uint8  gKeyAuthFailCounter;

static tUdsSecurityStatus gUdsGlobalSecurityStatus = UDS_SECURITY_STATUS_NOT_ACTIVE;
static tUdsSecurityLevel gUdsSecuLevel = UDS_SECURITY_LEVEL_0;
static tSid0x27SeedKeyPairInfor gSid0x27SeedKeyPairInfor[UDS_SECURITY_INDEX_MAX];
static uint32 gLockTimePreTick = 0;
static tUdsSecurityStatus gSecurityStatusArray[UDS_SECURITY_INDEX_MAX] = {UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE,UDS_SECURITY_STATUS_NOT_ACTIVE};
static uint8 gSid0x27ReqSeedCounter = 0;

static boolean gSecurityLockTimerActiveFlag = FALSE;
tInnerSecurityLevel sid0x27GetInnerSecurityLevel()
{
	switch(gUdsSecuLevel)
	{
	case UDS_SECURITY_LEVEL_0:
		return UDS_INNER_SECURITY_LEVEL_0;
	case UDS_SECURITY_LEVEL_1:
		return UDS_INNER_SECURITY_LEVEL_1;
	case UDS_SECURITY_LEVEL_2:
		return UDS_INNER_SECURITY_LEVEL_2;
	case UDS_SECURITY_LEVEL_3:
		return UDS_INNER_SECURITY_LEVEL_3;
	case UDS_SECURITY_LEVEL_4:
		return UDS_INNER_SECURITY_LEVEL_4;
	}

	return UDS_INNER_SECURITY_LEVEL_0;
}


boolean sid0x27SetSecurityLevel(tUdsSecurityLevel level)
{
	switch(level)
	{
	case UDS_SECURITY_LEVEL_0:
	case UDS_SECURITY_LEVEL_1:
	case UDS_SECURITY_LEVEL_2:
	case UDS_SECURITY_LEVEL_3:
	case UDS_SECURITY_LEVEL_4:
		gUdsSecuLevel = level;
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}


tUdsSecurityStatus sid0x27GetSecurityStatus()
{
	return gUdsGlobalSecurityStatus;
}

void sid0x27ResetStatus()
{
	sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_NOT_ACTIVE);
}

boolean sid0x27SetSecurityStatus(tUdsSecurityStatus securityStatus)
{
	if(gSecurityLockTimerActiveFlag == TRUE && gUdsGlobalSecurityStatus == UDS_SECURITY_STATUS_LOCKED)
		return FALSE;

	switch(securityStatus)
	{
	case UDS_SECURITY_STATUS_LOCKED:
		gSecurityLockTimerActiveFlag = TRUE;
		sid0x27ResetKeyAndSeed();
		break;
	case UDS_SECURITY_STATUS_NOT_ACTIVE:
		sid0x27ResetKeyAndSeed();
		break;
	case UDS_SECURITY_STATUS_ACTIVE:
		sid0x27ResetKeyFailedCounter();
		sid0x27ResetLockTimer();
		break;
	default:
		return FALSE;
	}

	gUdsGlobalSecurityStatus = securityStatus;

	return TRUE;
}


void sid0x27ResetKeyAndSeed()
{
	uint8 i = 0;
	
	for(i=0;i<UDS_SECURITY_INDEX_MAX;i++)
	{
		gSecurityStatusArray[i] = UDS_SECURITY_STATUS_NOT_ACTIVE;
		gSid0x27SeedKeyPairInfor[i].seedLen = 0;
		gSid0x27SeedKeyPairInfor[i].keyLen= 0;
	}
	sid0x27SetSecurityLevel(UDS_SECURITY_LEVEL_0);
}


boolean sid0x27CreateKey(uint32 accessType,tSid0x27SeedKeyPairInfor *seedKeyPairInfor)
{
	boolean ret = FALSE;
	
	if(seedKeyPairInfor)
	{
		seedKeyPairInfor->keyLen = sid0x27GenKey(accessType,seedKeyPairInfor->seed,seedKeyPairInfor->seedLen,seedKeyPairInfor->key,SID_0X27_KEY_BUFF_LEN);
		if(seedKeyPairInfor->keyLen>0 && seedKeyPairInfor->keyLen<=SID_0X27_KEY_BUFF_LEN)
			ret = TRUE;
	}

	return ret;
}

boolean sid0x27ConvertAccessTypeToIndex(tUdsSecurityAccessType accessType,uint8 *index)
{
	if(index == NULL)
		return FALSE;
	
	switch(accessType)
	{
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
		*index = UDS_SECURITY_INDEX_0;
		break;
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
		*index = UDS_SECURITY_INDEX_1;
		break;
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
		*index = UDS_SECURITY_INDEX_2;
		break;
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
		*index = UDS_SECURITY_INDEX_3;
		break;
	default:
		return FALSE;
	}

	return TRUE;
}


tUdsNRCType sid0x27VerifyKey(tUdsSecurityAccessType securityAccessType,const uint8* keyPtr,uint16 keyLen,uint8 index)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 i = 0;

	if(keyLen != gSid0x27CfgInfor.keyLen)
		ret = NRC_INCORRECT_MESSAGE_LENGTH;

	if(sid0x27CreateKey(securityAccessType,&gSid0x27SeedKeyPairInfor[index])==TRUE)
	{
		if(diagCmp(gSid0x27SeedKeyPairInfor[index].key,keyPtr,gSid0x27CfgInfor.keyLen)==TRUE)
		{
			gRespDataLen = 1;
			sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_ACTIVE);
			gSecurityStatusArray[index] = UDS_SECURITY_STATUS_ACTIVE;
			//reset other levle to NOT_ACTIVE
			for(i=0;i<UDS_SECURITY_INDEX_MAX;i++)
			{
				if(i!=index)
					gSecurityStatusArray[i] = UDS_SECURITY_STATUS_NOT_ACTIVE;	
			}
			if(sid0x27SetSecurityLevel(index+1) == FALSE)
				ret = NRC_GENERAL_REJECT;//inner error
		}
		else
		{
			ret = sid0x27AddAuthFailedCounter(securityAccessType);
		}
	}
	else
		ret = NRC_GENERAL_REJECT;

	return ret;
}

tSid0x27SeedKeyPairInfor* sid0x27GetSeed(tUdsSecurityAccessType accessType)
{
	uint8 index = 0;
	
	if(sid0x27ConvertAccessTypeToIndex(accessType,&index) == TRUE)
	{
		if(gSid0x27SeedKeyPairInfor[index].seedLen== 0)//no seed,need to generate new seed
		{
			gSid0x27SeedKeyPairInfor[index].seedLen = sid0x27GenSeed(accessType,gSid0x27SeedKeyPairInfor[index].seed,SID_0X27_SEED_BUFF_LEN);
		}

		if(gSid0x27SeedKeyPairInfor[index].seedLen >0 && gSid0x27SeedKeyPairInfor[index].seedLen<=SID_0X27_SEED_BUFF_LEN)
			return  &gSid0x27SeedKeyPairInfor[index];
	}

	return NULL;
}

tSid0x27SeedKeyPairInfor* sid0x27GetKey(tUdsSecurityAccessType accessType)
{
	uint8 index = 0;
	
	if(sid0x27ConvertAccessTypeToIndex(accessType,&index) == FALSE)
		return NULL;
	
	return &gSid0x27SeedKeyPairInfor[index];
}

tUdsNRCType sid0x27CheckDataLen(uint8 securityAccessType,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	
	switch(securityAccessType)
	{
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:			
	case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
		if(len!=2)
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
	case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
		if(len!=(gSid0x27CfgInfor.keyLen+2)) // sid_len + accessType_len + keyLen
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;
	default:
		ret = NRC_REQUEST_OUT_OF_RANGE;
	}

	return ret;
}

tUdsNRCType sid0x27CheckAccessTypeAndDataLen(const uint8 *data,uint16 payloadLen)
{
	uint8 securityAccessType;
	tUdsNRCType ret = NRC_NONE;
	
	if(NULL!=data && payloadLen >=2)
	{
		securityAccessType = data[1];
		switch(securityAccessType)
		{
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
			if(gSid0x27CfgInfor.enableSecurityAccessType1==FALSE)
				ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
			break;
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
			if(gSid0x27CfgInfor.enableSecurityAccessType3==FALSE)
				ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
			break;
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
			if(gSid0x27CfgInfor.enableSecurityAccessType5==FALSE)
				ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
			break;
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
			if(gSid0x27CfgInfor.enableSecurityAccessType7==FALSE)
				ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
			break;
		default:
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
		}

		if(ret == NRC_NONE)
			ret = sid0x27CheckDataLen(securityAccessType,payloadLen);
	}
	else
		ret = NRC_INCORRECT_MESSAGE_LENGTH;
	
	return ret;
}

tUdsNRCType sid0x27MatchSessionAndAccessType(tUdsSecurityAccessType accessType)
{
	tUdsSessionInnerType sessionType;
	tUdsNRCType ret = NRC_NONE;
	uint8 i = 0;

	if(gSid0x27CfgInfor.needCheckSessionMatchAccessType == TRUE)
	{
		sessionType = sid0x10GetCurSession();
		for(i=0;i<4;i++)
		{
			if(accessType == gSessionMapAccessType[i][0])
			{
				if((sessionType & gSessionMapAccessType[i][1])==0)
					ret = NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
				break;
			}
		}
	}
	
	return ret;
}


tUdsNRCType sid0x27RespSeed(uint8 *data,tUdsSecurityAccessType securityAccessType,uint8 index)
{
	tUdsNRCType ret = NRC_NONE;
	tSid0x27SeedKeyPairInfor* seedKeyPairInfor = NULL;

	if(sid0x27AddAuthFailedCounter(securityAccessType) != NRC_EXCEED_NUMBER_OF_ATTEMPTS)
	{
		if((ret=sid0x27MatchSessionAndAccessType(securityAccessType)) == NRC_NONE)
		{
			seedKeyPairInfor = sid0x27GetSeed(securityAccessType);
			if(seedKeyPairInfor!=NULL)
			{
				gSecurityStatusArray[index] = UDS_SECURITY_STATUS_WAIT_FOR_KEY;
				diagCpyData(data+2,seedKeyPairInfor->seed,gSid0x27CfgInfor.seedLen);
				gRespDataLen = seedKeyPairInfor->seedLen+1;
			}
			else
				ret = NRC_GENERAL_REJECT;
		}
	}
	else
		ret = NRC_EXCEED_NUMBER_OF_ATTEMPTS;
	
	return ret;
}


void sid0x27ReTriggerLock()
{
	gKeyAuthFailCounter = gSid0x27CfgInfor.authFailedMaxCount;
	gSid0x27ReqSeedCounter = gKeyAuthFailCounter;
	sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_LOCKED);
	gLockTimePreTick = diagGetCurTick();
}

tUdsNRCType sid0x27AddAuthFailedCounter(tUdsSecurityAccessType securityAccessType)
{
	tUdsNRCType ret = NRC_NONE;
	//boolean exceedFlag = FALSE;
	
	if(securityAccessType%2 != 0)//req seed
	{
		gSid0x27ReqSeedCounter++;
		if(gSid0x27ReqSeedCounter >= 2)
			gKeyAuthFailCounter++;
		if(gSid0x27ReqSeedCounter >= (gSid0x27CfgInfor.authFailedMaxCount +1))
		{
			gSid0x27ReqSeedCounter = gSid0x27CfgInfor.authFailedMaxCount +1;
		}
	}
	else	// req to cmp Key
	{
		gSid0x27ReqSeedCounter = 0;
		gKeyAuthFailCounter++;
	}

	
	if(gKeyAuthFailCounter >= gSid0x27CfgInfor.authFailedMaxCount)
	{
		gKeyAuthFailCounter = gSid0x27CfgInfor.authFailedMaxCount;
		sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_LOCKED);
		gLockTimePreTick = diagGetCurTick();
		ret = NRC_EXCEED_NUMBER_OF_ATTEMPTS;
	}
	else
		ret = NRC_INVALID_KEY;

	return ret;
}


void sid0x27ResetLockTimer()
{
	gSecurityLockTimer = 0;
}

void sid0x27DecreaseKeyFailedCounter()
{
	if(gSid0x27CfgInfor.authFailedMaxCount > 0)
	{
		if(gKeyAuthFailCounter >0)
		{
			gKeyAuthFailCounter -= 1;
			gSid0x27ReqSeedCounter = 0;
		}
	}
	
}

void sid0x27ResetKeyFailedCounter()
{
	gKeyAuthFailCounter = 0;
	gSid0x27ReqSeedCounter = 0;
}

//if security is lock,uds core will call this api periodicly
void sid0x27SecurityLockTimerTick()
{
	uint32 interval = 0;
	uint32 curTick = 0;
	
	if(sid0x27GetSecurityStatus() == UDS_SECURITY_STATUS_LOCKED)
	{
		curTick = diagGetCurTick();
		interval = curTick - gLockTimePreTick;
		gLockTimePreTick = curTick;
		gSecurityLockTimer += interval;
		if(gSecurityLockTimer >= gSid0x27CfgInfor.delayTimerForReAuth)
		{
			gSecurityLockTimerActiveFlag = FALSE;
			sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_NOT_ACTIVE);
			sid0x27ResetLockTimer();
			sid0x27DecreaseKeyFailedCounter();
		}
	}
}


tUdsNRCType sid0x27MainProcess(uint8 *data,uint16 len)
{
	tUdsNRCType ret = NRC_NONE;
	tUdsSecurityStatus	securityStatus = UDS_SECURITY_STATUS_NOT_ACTIVE;
	tUdsSecurityAccessType securityAccessType = 0;
	uint8 index = 0;

	if((ret=sid0x27CheckAccessTypeAndDataLen(data,len))!=NRC_NONE)
		return ret;
	
	securityAccessType = data[1]&0x7F;

	if(sid0x27ConvertAccessTypeToIndex(securityAccessType,&index) == FALSE)
		return NRC_GENERAL_REJECT;
	
	securityStatus = sid0x27GetSecurityStatus();
	
	switch(securityStatus)
	{
	case UDS_SECURITY_STATUS_LOCKED:
		switch(securityAccessType)
		{
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:
		case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
			ret = NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED;
			break;
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
		case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
			ret = NRC_REQUEST_SEQUENCE_ERROR;
			break;
		default:
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;//error,not happened
		}
		break;
	case UDS_SECURITY_STATUS_NOT_ACTIVE:
	case UDS_SECURITY_STATUS_ACTIVE:
		if(securityStatus == UDS_SECURITY_STATUS_ACTIVE)
		{
			
			switch(securityAccessType)
			{
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
				//if cur this level is already ACTIVE,resp seed with all 0
				if(gSecurityStatusArray[index] == UDS_SECURITY_STATUS_ACTIVE)
				{
					gRespDataLen = gSid0x27CfgInfor.seedLen+1;
					diagResetBuffer(data+2,0,gSid0x27CfgInfor.seedLen);
				}
				else
				{
					ret = sid0x27RespSeed(data,securityAccessType,index);	
				}
				break;
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
				
				if(gSecurityStatusArray[index] == UDS_SECURITY_STATUS_WAIT_FOR_KEY)
				{
					ret = sid0x27VerifyKey(securityAccessType,&data[2],len-2,index);
				}
				else
					ret = NRC_REQUEST_SEQUENCE_ERROR;
				break;
				
			default:
				ret = NRC_GENERAL_REJECT;
			}

		}
		else if(securityStatus == UDS_SECURITY_STATUS_NOT_ACTIVE)
		{
			switch(securityAccessType)
			{
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_1:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_3:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_5:
			case UDS_SECURITY_ACCESS_TYPE_REQ_SEED_7:
				ret = sid0x27RespSeed(data,securityAccessType,index);	
				break;
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
				if(gSecurityStatusArray[index] == UDS_SECURITY_STATUS_WAIT_FOR_KEY)
				{
					ret = sid0x27VerifyKey(securityAccessType,&data[2],len-2,index);
				}
				else
					ret = NRC_REQUEST_SEQUENCE_ERROR;
				break;
			default:
				ret = NRC_GENERAL_REJECT;
			}
			
		}
		break;
	default:
		//error
		ret= NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}


void sid0x27Init()
{
	
	sid0x27SetSecurityStatus(UDS_SECURITY_STATUS_NOT_ACTIVE);

	if(gSid0x27CfgInfor.relockByFailCounterIfSysReset == TRUE)
	{
		gSecurityLockTimer = 0;
		
		if(gKeyAuthFailCounter >= gSid0x27CfgInfor.authFailedMaxCount)
			sid0x27ReTriggerLock();
	}
	else if(gSid0x27CfgInfor.relockByDelayTimerIfSysReset == TRUE)
	{
		if(gSecurityLockTimer > 0)
		{
			sid0x27ReTriggerLock();
		}
	}
	else
	{
		gKeyAuthFailCounter = 0;
		gSecurityLockTimer = 0;
	}
}

