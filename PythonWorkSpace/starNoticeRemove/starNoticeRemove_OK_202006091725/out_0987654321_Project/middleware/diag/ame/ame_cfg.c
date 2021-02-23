
#include "Std_Types.h"

#include "ame_cbk.h"
#include "ame_cfg.h"




uint8 gUnlockAmeKey[UNLOCK_AME_DATA_LEN] = {0xB0,0xC1,0xDF,0x2E,0x5A,0x46,0x73,0x15,0x69,0xA8};



#if 0

const uint8 array1[] = {0x31,0x32,0x33,0x34};   
const uint8 array2[] = {0x35,0x36,0x37,0x38};   
const uint8 array3[] = {0x39,0x40,0x41,0x42};


uint16 gArray1len = sizeof(array1);
uint16 gArray2len = sizeof(array2);
uint16 gArray3len = sizeof(array3);

#endif



const tAmeSubServiceFnCbk gAmeSubServiceFnCbk[] = {
	{SID_READ_DATA_BY_DID,  ameReadData},
	{SID_WRITE_DATA_BY_DID, ameWriteData},
	{SID_IOCONTROL,         ameIOControl},
	{SID_SECURITY_ACCESS,   ameSecurityAccess},
};


const t0x22DidInfo gAmeRWDidInfo[] = {
	
};






uint16 gAmeSubServiceFnCbkTotalNum = sizeof(gAmeSubServiceFnCbk)/sizeof(tAmeSubServiceFnCbk);










