/**************************
description: public general process APIs for sid 0x27 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x27_if.h"
#include"sid0x10_if.h"

void sid0x27ResetKeyAndSeed(uint32 ch);
static boolean sid0x27ConvertAccessTypeToIndex(uint32 ch,tUdsSecurityAccessType accessType,uint8 *index);
static tSid0x27SeedKeyPairInfor* sid0x27GetSeed(uint32 ch,tUdsSecurityAccessType accessType);
static tSid0x27SeedKeyPairInfor* sid0x27GetKey(uint32 ch,tUdsSecurityAccessType accessType);
static boolean sid0x27SetSecurityLevel(uint32 ch,tUdsSecurityLevel level);
static tUdsNRCType sid0x27VerifyKey(uint32 ch,tUdsSecurityAccessType securityAccessType,const uint8* keyPtr,uint16 keyLen,uint8 index);
static tUdsNRCType sid0x27CheckDataLen(uint32 ch,uint8 securityAccessType,uint16 len);
static tUdsNRCType sid0x27CheckAccessTypeAndDataLen(uint32 ch,const uint8 *data,uint16 payloadLen);
static void sid0x27ResetKeyFailedCounter(uint32 ch);
static void sid0x27ResetLockTimer(uint32 ch);
static tUdsNRCType sid0x27ProcessAfterAuthFailed(uint32 ch,tUdsSecurityAccessType securityAccessType);
static tUdsSecurityStatus sid0x27GetSecurityStatus(uint32 ch);
static tUdsNRCType sid0x27MatchSessionAndAccessType(uint32 ch,tUdsSecurityAccessType accessType);
static boolean sid0x27SetSecurityStatus(uint32 ch,tUdsSecurityStatus securityStatus);


extern uint16 gRespDataLen[];
extern const tSid0x27CfgInfor gSid0x27CfgInfor;
extern uint32 gSecurityLockTimer[];
extern const tSessionMapAccessTypeArray gSessionMapAccessTypeArray[];

extern uint8  gKeyAuthFailCounter[];
extern tUdsSecurityStatus gUdsGlobalSecurityStatus[];
extern tUdsSecurityLevel gUdsSecuLevel[];
extern tSid0x27SeedKeyPairInfor gSid0x27SeedKeyPairInfor[][UDS_SECURITY_INDEX_MAX];
extern uint32 gLockTimePreTick[];
extern tUdsSecurityStatus gSecurityStatusArray[][UDS_SECURITY_INDEX_MAX];

static boolean gSecurityLockTimerActiveFlag = FALSE;

tInnerSecurityLevel sid0x27GetInnerSecurityLevel(uint32 ch)
{
	switch(gUdsSecuLevel[ch])
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


boolean sid0x27SetSecurityLevel(uint32 ch,tUdsSecurityLevel level)
{
	switch(level)
	{
	case UDS_SECURITY_LEVEL_0:
	case UDS_SECURITY_LEVEL_1:
	case UDS_SECURITY_LEVEL_2:
	case UDS_SECURITY_LEVEL_3:
	case UDS_SECURITY_LEVEL_4:
		gUdsSecuLevel[ch] = level;
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}


tUdsSecurityStatus sid0x27GetSecurityStatus(uint32 ch)
{
	return gUdsGlobalSecurityStatus[ch];
}

void sid0x27ResetStatus(uint32 ch)
{
	sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_NOT_ACTIVE);
}

boolean sid0x27SetSecurityStatus(uint32 ch,tUdsSecurityStatus securityStatus)
{
	if(gSecurityLockTimerActiveFlag == TRUE && gUdsGlobalSecurityStatus[ch] == UDS_SECURITY_STATUS_LOCKED)
		return FALSE;

	switch(securityStatus)
	{
	case UDS_SECURITY_STATUS_LOCKED:
		gSecurityLockTimerActiveFlag = TRUE;
		sid0x27ResetKeyAndSeed(ch);
		break;
	case UDS_SECURITY_STATUS_NOT_ACTIVE:
		sid0x27ResetKeyAndSeed(ch);
		break;
	case UDS_SECURITY_STATUS_ACTIVE:
		sid0x27ResetKeyFailedCounter(ch);
		sid0x27ResetLockTimer(ch);
		break;
	default:
		return FALSE;
	}

	gUdsGlobalSecurityStatus[ch] = securityStatus;

	return TRUE;
}


void sid0x27ResetKeyAndSeed(uint32 ch)
{
	uint8 i = 0;
	
	for(i=0;i<UDS_SECURITY_INDEX_MAX;i++)
	{
		gSecurityStatusArray[ch][i] = UDS_SECURITY_STATUS_NOT_ACTIVE;
		gSid0x27SeedKeyPairInfor[ch][i].seedLen = 0;
		gSid0x27SeedKeyPairInfor[ch][i].keyLen= 0;
	}
	sid0x27SetSecurityLevel(ch,UDS_SECURITY_LEVEL_0);
}


boolean sid0x27CreateKey(uint32 ch,uint32 accessType,tSid0x27SeedKeyPairInfor *seedKeyPairInfor)
{
	boolean ret = FALSE;
	
	if(seedKeyPairInfor)
	{
		seedKeyPairInfor->keyLen = sid0x27GenKey(ch,accessType,seedKeyPairInfor->seed,seedKeyPairInfor->seedLen,seedKeyPairInfor->key,SID_0X27_KEY_BUFF_LEN);
		if(seedKeyPairInfor->keyLen>0 && seedKeyPairInfor->keyLen<=SID_0X27_KEY_BUFF_LEN)
			ret = TRUE;
	}

	return ret;
}

boolean sid0x27ConvertAccessTypeToIndex(uint32 ch,tUdsSecurityAccessType accessType,uint8 *index)
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


tUdsNRCType sid0x27VerifyKey(uint32 ch,tUdsSecurityAccessType securityAccessType,const uint8* keyPtr,uint16 keyLen,uint8 index)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 i = 0;

	if(keyLen != gSid0x27CfgInfor.keyLen)
		ret = NRC_INCORRECT_MESSAGE_LENGTH;

	if(sid0x27CreateKey(ch,securityAccessType,&gSid0x27SeedKeyPairInfor[ch][index])==TRUE)
	{
		if(diagCmp(gSid0x27SeedKeyPairInfor[ch][index].key,keyPtr,gSid0x27CfgInfor.keyLen)==TRUE)
		{
			gRespDataLen[ch] = 1;
			sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_ACTIVE);
			gSecurityStatusArray[ch][index] = UDS_SECURITY_STATUS_ACTIVE;
			//reset other levle to NOT_ACTIVE
			for(i=0;i<UDS_SECURITY_INDEX_MAX;i++)
			{
				if(i!=index)
					gSecurityStatusArray[ch][i] = UDS_SECURITY_STATUS_NOT_ACTIVE;	
			}
			if(sid0x27SetSecurityLevel(ch,index+1) == FALSE)
				ret = NRC_GENERAL_REJECT;//inner error
		}
		else
		{
			ret = sid0x27ProcessAfterAuthFailed(ch,securityAccessType);
		}
	}
	else
		ret = NRC_GENERAL_REJECT;

	return ret;
}

tSid0x27SeedKeyPairInfor* sid0x27GetSeed(uint32 ch,tUdsSecurityAccessType accessType)
{
	uint8 index = 0;
	
	if(sid0x27ConvertAccessTypeToIndex(ch,accessType,&index) == TRUE)
	{
		if(gSid0x27SeedKeyPairInfor[ch][index].seedLen== 0)//no seed,need to generate new seed
		{
			gSid0x27SeedKeyPairInfor[ch][index].seedLen = sid0x27GenSeed(ch,accessType,gSid0x27SeedKeyPairInfor[ch][index].seed,SID_0X27_SEED_BUFF_LEN);
		}

		if(gSid0x27SeedKeyPairInfor[ch][index].seedLen >0 && gSid0x27SeedKeyPairInfor[ch][index].seedLen<=SID_0X27_SEED_BUFF_LEN)
			return  &gSid0x27SeedKeyPairInfor[ch][index];
	}

	return NULL;
}

tSid0x27SeedKeyPairInfor* sid0x27GetKey(uint32 ch,tUdsSecurityAccessType accessType)
{
	uint8 index = 0;
	
	if(sid0x27ConvertAccessTypeToIndex(ch,accessType,&index) == FALSE)
		return NULL;
	
	return &gSid0x27SeedKeyPairInfor[ch][index];
}

tUdsNRCType sid0x27CheckDataLen(uint32 ch,uint8 securityAccessType,uint16 len)
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

tUdsNRCType sid0x27CheckAccessTypeAndDataLen(uint32 ch,const uint8 *data,uint16 payloadLen)
{
	uint8 securityAccessType;
	tUdsNRCType ret = NRC_NONE;
	
	if(NULL!=data && payloadLen >=2)
	{
		securityAccessType = data[1]&0x7F;
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
			ret = sid0x27CheckDataLen(ch,securityAccessType,payloadLen);
	}
	else
		ret = NRC_INCORRECT_MESSAGE_LENGTH;
	
	return ret;
}

tUdsNRCType sid0x27MatchSessionAndAccessType(uint32 ch,tUdsSecurityAccessType accessType)
{
	tUdsSessionInnerType sessionType;
	tUdsNRCType ret = NRC_NONE;
	uint8 i = 0;

	if(gSid0x27CfgInfor.needCheckSessionMatchAccessType == TRUE)
	{
		sessionType = sid0x10GetCurSession(ch);
		for(i=0;i<4;i++)
		{
			if(accessType == gSessionMapAccessTypeArray[ch].sessionMapAccessType[i].accessType)
			{
				if((sessionType & gSessionMapAccessTypeArray[ch].sessionMapAccessType[i].innerSession)==0)
					ret = NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION;
				break;
			}
		}
	}
	
	return ret;
}


tUdsNRCType sid0x27RespSeed(uint32 ch,uint8 *data,tUdsSecurityAccessType securityAccessType,uint8 index)
{
	tUdsNRCType ret = NRC_NONE;
	tSid0x27SeedKeyPairInfor* seedKeyPairInfor = NULL;
	
	if((ret=sid0x27MatchSessionAndAccessType(ch,securityAccessType)) == NRC_NONE)
	{
		seedKeyPairInfor = sid0x27GetSeed(ch,securityAccessType);
		if(seedKeyPairInfor!=NULL)
		{
			gSecurityStatusArray[ch][index] = UDS_SECURITY_STATUS_WAIT_FOR_KEY;
			diagCpyData(data+2,seedKeyPairInfor->seed,gSid0x27CfgInfor.seedLen);
			gRespDataLen[ch] = seedKeyPairInfor->seedLen+1;
		}
		else
			ret = NRC_GENERAL_REJECT;
	}

	return ret;
}


tUdsNRCType sid0x27ProcessAfterAuthFailed(uint32 ch,tUdsSecurityAccessType securityAccessType)
{
	tUdsNRCType ret = NRC_NONE;
	
	if(gKeyAuthFailCounter[ch] < gSid0x27CfgInfor.authFailedMaxCount)
	{
		gKeyAuthFailCounter[ch]++;
	}
	
	if(gKeyAuthFailCounter[ch] >= gSid0x27CfgInfor.authFailedMaxCount)
	{
		sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_LOCKED);
		gLockTimePreTick[ch] = diagGetCurTick();
		ret = NRC_EXCEED_NUMBER_OF_ATTEMPTS;
	}
	else
		ret = NRC_INVALID_KEY;

	return ret;
}


void sid0x27ResetLockTimer(uint32 ch)
{
	gSecurityLockTimer[ch] = 0;
}

void sid0x27ResetKeyFailedCounter(uint32 ch)
{
	gKeyAuthFailCounter[ch] = 0;
}

//if security is lock,uds core will call this api periodicly
void sid0x27SecurityLockTimerTick(uint32 ch)
{
	uint32 interval = 0;
	uint32 curTick = 0;
	
	if(sid0x27GetSecurityStatus(ch) == UDS_SECURITY_STATUS_LOCKED)
	{
		curTick = diagGetCurTick();
		interval = curTick - gLockTimePreTick[ch];
		gLockTimePreTick[ch] = curTick;
		gSecurityLockTimer[ch] += interval;
		if(gSecurityLockTimer[ch] >= gSid0x27CfgInfor.delayTimerForReAuth)
		{
			gSecurityLockTimerActiveFlag = FALSE;
			sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_NOT_ACTIVE);
			sid0x27ResetLockTimer(ch);
			sid0x27ResetKeyFailedCounter(ch);
		}
	}
}


tUdsNRCType sid0x27MainProcess(tUdsReqInfor*reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	tUdsSecurityStatus	securityStatus = UDS_SECURITY_STATUS_NOT_ACTIVE;
	tUdsSecurityAccessType securityAccessType = 0;
	uint8 index = 0;
	uint32 ch = reqInfor->ch;
	uint8 *data = reqInfor->data;
	uint16 len = reqInfor->len;

	if((ret=sid0x27CheckAccessTypeAndDataLen(ch,data,len))!=NRC_NONE)
		return ret;
	
	securityAccessType = data[1]&0x7F;

	if(sid0x27ConvertAccessTypeToIndex(ch,securityAccessType,&index) == FALSE)
		return NRC_GENERAL_REJECT;
	
	securityStatus = sid0x27GetSecurityStatus(ch);
	
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
				if(gSecurityStatusArray[ch][index] == UDS_SECURITY_STATUS_ACTIVE)
				{
					gRespDataLen[ch] = gSid0x27CfgInfor.seedLen+1;
					diagResetBuffer(data+2,0,gSid0x27CfgInfor.seedLen);
				}
				else
				{
					ret = sid0x27RespSeed(ch,data,securityAccessType,index);	
				}
				break;
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
				
				if(gSecurityStatusArray[ch][index] == UDS_SECURITY_STATUS_WAIT_FOR_KEY)
				{
					ret = sid0x27VerifyKey(ch,securityAccessType,&data[2],len-2,index);
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
				ret = sid0x27RespSeed(ch,data,securityAccessType,index);	
				break;
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_2:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_4:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_6:
			case UDS_SECURITY_ACCESS_TYPE_SEND_KEY_8:
				if(gSecurityStatusArray[ch][index] == UDS_SECURITY_STATUS_WAIT_FOR_KEY)
				{
					ret = sid0x27VerifyKey(ch,securityAccessType,&data[2],len-2,index);
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
	uint8 ch = 0;

	for(ch=0;ch<gSid0x27CfgInfor.maxChNum;ch++)
	{
		sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_NOT_ACTIVE);
		
		if(gSecurityLockTimer[ch] > 0)
		{
			if(gSid0x27CfgInfor.continueDelayTimerAfterSysReset == FALSE)
				gSecurityLockTimer[ch] = 0;

			sid0x27SetSecurityStatus(ch,UDS_SECURITY_STATUS_LOCKED);
			gLockTimePreTick[ch] = diagGetCurTick();
			diagSetNvmBlockId(gSid0x27CfgInfor.nvmBlockIdForLockTimer,TRUE);
		}
	}
}

