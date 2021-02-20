/**************************************
Author		:tanjinyi
Version		: 1.2
Timestamp	: 201126155351
***************************************/
#include"SWVersion.h"

const uint8 gIocSWVersion[IOC_SOFTWARE_VERSION_LEN] = "201126155351";
const uint8 gIocSWVersionRomHex[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] __attribute__ ((section(".InforConfig")))= {0x14,0xb,0x1a,0xf,0x35,0x33};
const uint8 gIocSWSubmitterName[IOC_SW_SUBMITTER_NAME_LEN] = "tanjinyi";

uint8 getIocSWVersion(uint8 *versionBuff,uint8 bufLen)
{
	uint8 i = 0;
	if(versionBuff!=NULL && bufLen>=IOC_SOFTWARE_VERSION_LEN)
	{
		for(i=0;i<IOC_SOFTWARE_VERSION_LEN;i++)
			versionBuff[i] = gIocSWVersion[i];
		
		return IOC_SOFTWARE_VERSION_LEN;
	}
	
	return 0;
}

uint8 getIocSWVersionRomHex(uint8 *versionBuff,uint8 bufLen)
{
	uint8 i = 0;
	if(versionBuff!=NULL && bufLen>=IOC_SOFTWARE_VERSION_ROM_HEX_LEN)
	{
		for(i=0;i<IOC_SOFTWARE_VERSION_ROM_HEX_LEN;i++)
			versionBuff[i] = gIocSWVersionRomHex[i];
		
		return IOC_SOFTWARE_VERSION_ROM_HEX_LEN;
	}
	
	return 0;
}


