/**************************************
Author		: android-build
Version		: 1.4
Timestamp	: 201230114008
***************************************/
#include"SWVersion.h"

const uint8 gIocSWVersion[IOC_SOFTWARE_VERSION_LEN] = "201230114008";
const uint8 gIocSWVersionRomHex[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] __attribute__ ((section(".InforConfig")))= {0x14,0xc,0x1e,0xb,0x28,0x8};
const uint8 gIocSWSubmitterName[IOC_SW_SUBMITTER_NAME_LEN] = "android-build";

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


