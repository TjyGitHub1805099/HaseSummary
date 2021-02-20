#include "Std_Types.h"
#include "AppInfo.h"
#include "MemMap.h"
extern int _start();

APPVALID_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
const uint8 AppValidNumEnd[8] = {0x5a, 0xa1, 0xca, 0xa5, 0xca, 0xe7, 0x6f, 0x2e};
APPVALID_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED


APPINFO_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
const AppinfoStructType Appinfo =
{
	(uint32)_start,
	(uint32)&AppValidNumEnd[0],
	{0x5a, 0xa1, 0xca, 0xa5, 0xca, 0xe7, 0x6f, 0x2e}
};

const SysVerInfoSType SysVersionInfo =
{
	/*compileDate[11]*/{__DATE__},
	/*compileTime[8]*/{__TIME__},
	/*SysSupplierID[5]*/{'3','G','1','2',' '},
	/*EcuName[15]*/{'I', 'P', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
	/*PartNumAndVersion[6]*/{SYS_VERINFO_PROJECTNUM, SYS_VERINFO_TEAMNUM, (uint8)(SYS_VERINFO_PARTSERIALNUM >> 8), (uint8)(SYS_VERINFO_PARTSERIALNUM), SYS_VERINFO_VERSTAGE, SYS_VERINFO_VERSUBSTAGE},
	/*VehicleEcuSwNum[7]*/{SYS_VERINFO_PROJECTNUM, SYS_VERINFO_TEAMNUM, (uint8)(SYS_VERINFO_PARTSERIALNUM >> 8), (uint8)(SYS_VERINFO_PARTSERIALNUM) + SYS_CATEGOAY_SW, SYS_VERINFO_SWHWSERIALNUM, (uint8)((SYS_VERINFO_FRS_MAINSTAGE << 4) + SYS_VERINFO_FRS_SUBSTAGE), SYS_VERINFO_SW_SUBVER},
	/*AppSwPartNum[7]*/{SYS_VERINFO_PROJECTNUM, SYS_VERINFO_TEAMNUM, (uint8)(SYS_VERINFO_PARTSERIALNUM >> 8), (uint8)(SYS_VERINFO_PARTSERIALNUM) + SYS_CATEGOAY_APP, SYS_VERINFO_SWHWSERIALNUM, (uint8)((SYS_VERINFO_FRS_MAINSTAGE << 4) + SYS_VERINFO_FRS_SUBSTAGE), SYS_VERINFO_APP_SUBVER},
	/*BasicSwPartNum[7]*/{SYS_VERINFO_PROJECTNUM, SYS_VERINFO_TEAMNUM, (uint8)(SYS_VERINFO_PARTSERIALNUM >> 8), (uint8)(SYS_VERINFO_PARTSERIALNUM) + SYS_CATEGOAY_BSC, SYS_VERINFO_SWHWSERIALNUM, (uint8)((SYS_VERINFO_FRS_MAINSTAGE << 4) + SYS_VERINFO_FRS_SUBSTAGE), SYS_VERINFO_BSC_SUBVER},
	/*HmiSwPartNum[7]*/{SYS_VERINFO_PROJECTNUM, SYS_VERINFO_TEAMNUM, (uint8)(SYS_VERINFO_PARTSERIALNUM >> 8), (uint8)(SYS_VERINFO_PARTSERIALNUM) + SYS_CATEGOAY_HMI, SYS_VERINFO_SWHWSERIALNUM, (uint8)((SYS_VERINFO_FRS_MAINSTAGE << 4) + SYS_VERINFO_FRS_SUBSTAGE), SYS_VERINFO_HMI_SUBVER},
	/*SupplierPartNum[10]*/{'M', '8', '1', '0', '5', ' ', ' ', ' ', ' ', ' '},
	/*SupplierSwNum[11]*/{'M', '8', '1', '0', '5', 'J', '0', '0', '0', '0', '0'},
	/*AdaptiveHwNum[10]*/{'M', '8', '1', '0', '5', 'S', '0', '0', '0', '0'},
	/*dbcVersion*/SYS_VERINFO_DBC,
};

#if 0
const SysVerInfoSType SysVerInfo =
{
	/*ECUName[15]*/{0x49, 0x50, 0x43, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
	/*SWHWSerialNum*/0x00,
	/*ProjectNum*/0x51,
	/*TeamNum*/0x75,
	/*PartSerialNum*/0x0030,
	/*dbcVersion*/0x9005,
	/*VersionNum*/0x4100,
	/*SWVersion*/0x8000,
	/*AppVersion*/0x8000,
	/*BasicVersion*/0x8000,
	/*HMIVersion*/0x8000,
	/*SupplierSWNum[11]*/{0x4d, 0x38, 0x31, 0x30, 0x35, 0x4a, 0x30, 0x30, 0x30, 0x30, 0x30},
	/*SupplierPartNum[10]*/{0x4d, 0x38, 0x31, 0x30, 0x35, 0x4a, 0x30, 0x30, 0x30, 0x30}
};
#endif
APPINFO_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED

uint8 Sys_GetBootloaderVersion(uint8* buff)
{
	uint8 lenLoop = 0u;

	for(lenLoop=0;lenLoop<SYS_BOOTLOADER_VER_LEN;lenLoop++)
	{
		*buff++=*(uint8*)(SYS_BOOTLOADER_VER_ADDR+lenLoop);
	}
	return(SYS_BOOTLOADER_VER_LEN);
}

