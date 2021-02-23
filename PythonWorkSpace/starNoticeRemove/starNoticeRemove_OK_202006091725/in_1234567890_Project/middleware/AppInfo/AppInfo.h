#ifndef _APPINFO_H_
#define _APPINFO_H_

#include "MemMap.h"

/************************** Don't change the following value:START **************************/
#define SYS_CATEGOAY_HW 0
#define SYS_CATEGOAY_SW 1
#define SYS_CATEGOAY_APP 2
#define SYS_CATEGOAY_BSC 3
#define SYS_CATEGOAY_CAL 4
#define SYS_CATEGOAY_HMI 5
#define SYS_CATEGOAY_COD 6
#define SYS_CATEGOAY_FLS 7

/*SYS_VERINFO_FRS_MAINSTAGE*/
#define SYS_MAINSTAGE_BEFOREREL 0
#define SYS_MAINSTAGE_FRS_TA1 1
#define SYS_MAINSTAGE_FRS_TA2 2
#define SYS_MAINSTAGE_FRS_PT1 3
#define SYS_MAINSTAGE_FRS_PT2 4
#define SYS_MAINSTAGE_FRS_WI1 5
#define SYS_MAINSTAGE_FRS_FG1 6
#define SYS_MAINSTAGE_FRS_FC 7
#define SYS_MAINSTAGE_FRS_100 8
#define SYS_MAINSTAGE_FRS_PP 9
#define SYS_MAINSTAGE_FRS_D0 0X0A
#define SYS_MAINSTAGE_FRS_SOP 0X0B

/*SYS_VERINFO_VERSTAGE*/
#define SYS_VERSTAGE_PVS 'A'
#define SYS_VERSTAGE_OTS 'S'
#define SYS_VERSTAGE_PT 'P'

#define SYS_LEN_SUPPLIER_ID 5
#define SYS_LEN_ECU_NAME 15
#define SYS_LEN_PARTNUM_VERSION 6
#define SYS_LEN_VEHICLE_ECU_SW  7
#define SYS_LEN_APP_SW 7
#define SYS_LEN_BSC_SW 7
#define SYS_LEN_HMI_SW 7
#define SYS_LEN_SUPPLIER_PART 10
#define SYS_LEN_SUPPLIER_SW 11
#define SYS_LEN_ADAPTIVE_HW 10
/*************************************** END ****************************************/

/************************** User Configuration area:START **************************/
#define SYS_VERINFO_PROJECTNUM 0x51
#define SYS_VERINFO_TEAMNUM 0x75
#define SYS_VERINFO_PARTSERIALNUM 0x0030
#define SYS_VERINFO_SWHWSERIALNUM 0X00
#define SYS_VERINFO_FRS_MAINSTAGE SYS_MAINSTAGE_FRS_100
/*The following value must between 0 and 9*/
#define SYS_VERINFO_FRS_SUBSTAGE 0
#define SYS_VERINFO_SW_SUBVER 0X00
#define SYS_VERINFO_APP_SUBVER 0X00
#define SYS_VERINFO_BSC_SUBVER 0X00
#define SYS_VERINFO_HMI_SUBVER 0X00

#define SYS_VERINFO_VERSTAGE SYS_VERSTAGE_PVS
#define SYS_VERINFO_VERSUBSTAGE 2

#define SYS_VERINFO_DBC 0xA001

#define SYS_BOOTLOADER_VER_LEN MEM_BOOTLOADER_VER_LEN
#define SYS_BOOTLOADER_VER_ADDR MEM_BOOTLOADER_VER_ADDR

/************************** User Configuration area:END **************************/

typedef struct
{
	uint32 AppStartAddr;
	uint32 AppValidAddr;
	uint8  AppValidNum[8];
}AppinfoStructType;

typedef struct
{
	uint8 compileDate[11];
	uint8 compileTime[8];
	uint8 SysSupplierID[SYS_LEN_SUPPLIER_ID];
	uint8 EcuName[SYS_LEN_ECU_NAME];
	uint8 PartNumAndVersion[SYS_LEN_PARTNUM_VERSION];
	uint8 VehicleEcuSwNum[SYS_LEN_VEHICLE_ECU_SW];
	uint8 AppSwPartNum[SYS_LEN_APP_SW];
	uint8 BasicSwPartNum[SYS_LEN_BSC_SW];
	uint8 HmiSwPartNum[SYS_LEN_HMI_SW];
	uint8 SupplierPartNum[SYS_LEN_SUPPLIER_PART];
	uint8 SupplierSwNum[SYS_LEN_SUPPLIER_SW];
	uint8 AdaptiveHwNum[SYS_LEN_ADAPTIVE_HW];
	uint16 dbcVersion;
}SysVerInfoSType;

extern uint8 Sys_GetBootloaderVersion(uint8* buff);

#endif

