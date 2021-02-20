#include"sid0x27_if.h"
#include"sid0x27_cfg.h"
#include"sid0x10_if.h"
#include "SecurityAlgorithm.h"


extern const tSid0x27CfgInfor gSid0x27CfgInfor;
extern uint8 gECUSerialNumbe[];


tUdsNRCType sid0x27CheckMainPreCondition(const uint8*data,uint16 len)
{
	if(diagCheckPreCondtionGroup2() == FALSE)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}



uint8 sid0x27GenSeed(uint32 ch,tUdsSecurityAccessType securityAccessType,uint8 *seedBuff,uint8 seedBuffMaxLen)
{
	uint32 seed = 0;
	uint8 seedLen = 0;
	
	if(NULL!=seedBuff && SID_0X27_CFG_SECURITY_SEED_LEN<=seedBuffMaxLen)
	{
		seed = diagGetRandomNum();
		seedBuff[0] = (uint8)((seed&0xFF000000)>>24);
		seedBuff[1] = (uint8)((seed&0xFF0000)>>16);
		seedBuff[2] = (uint8)((seed&0xFF00)>>8);
		seedBuff[3] = (uint8)((seed&0xFF));
		seedLen = SID_0X27_CFG_SECURITY_SEED_LEN;
	}

	return seedLen;
}


uint8 sid0x27GenKey(uint32 ch,tUdsSecurityAccessType securityAccessType,
						const uint8 *seed,
						uint8 seedLen,
						uint8*keyBuff,
						uint8 keyBuffMaxLen)
{
	uint8 seedBuff[SID_0X27_CFG_SECURITY_SEED_LEN] = {0};
	uint8 key[SID_0X27_CFG_SECURITY_KEY_LEN] = {1,2,3,4};
	uint8 keyLen = 0;

	if(seed && (SID_0X27_CFG_SECURITY_SEED_LEN==seedLen) && keyBuff && SID_0X27_CFG_SECURITY_KEY_LEN<=keyBuffMaxLen)
	{
		diagCpyData(seedBuff,seed, SID_0X27_CFG_SECURITY_SEED_LEN);
		
		//1TODO: here is project dependently,chang it accorind to CRS
		//CreateKey(gECUSerialNumbe,10,seed,securityAccessType+1,key);
		
		diagCpyData(keyBuff,key,SID_0X27_CFG_SECURITY_KEY_LEN);
		keyLen = SID_0X27_CFG_SECURITY_KEY_LEN;
	}
	
	return keyLen;
}


