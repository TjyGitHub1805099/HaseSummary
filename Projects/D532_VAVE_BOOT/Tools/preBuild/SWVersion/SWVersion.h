/**************************************
Author		:tanjinyi
Version		: 1.1
Timestamp	: 201126155351
***************************************/
#include"std_Types.h"

#define IOC_SOFTWARE_VERSION_ROM_HEX_LEN 6
#define IOC_SOFTWARE_VERSION_LEN 13
#define IOC_SW_SUBMITTER_NAME_LEN 32

extern uint8 getIocSWVersion(uint8 *versionBuff,uint8 bufLen);
extern uint8 getIocSWVersionRomHex(uint8 *versionBuff,uint8 bufLen);
