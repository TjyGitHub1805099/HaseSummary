/**************************
description: public general process APIs for sid 0x22 and 0x2e of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x22_cfg.h"
#include"nvm.h"
#include"sid0x27_if.h"

//REALLOCATE_VAR2NVRAM_START
uint8  UdsTestDid[4]={0x11,0x22,0x33,0x44};
uint16 gSystemSupplierEcuHardwareNum=0x0101;
uint8  gECUSerialNumbe[DID_ECU_SER_NUM_LEN]={ '1','6','0','4','1','9','0','0','0','1'};
uint8  gVINDataID[DID_VIN_DATA_ID_LEN] = {'A','B','C','D','E','F','G','0','1','2','3','4','5','6','7','8','9'};
uint8  gSupplierHWRefGuide[DID_SUPPLIER_HARDWARE_REFERENCE_GUIDE_LEN]={'E','6','1','3','7','U','A','I','1','0'};
uint8  gDiagSerNum[DID_DIAG_SER_NUM_LEN]={'H',0x00,0x01};
uint8  gResetRecord[DID_RESET_RECORD_LEN]={0,1};
uint8  gBodyCfgInf[DID_BODY_CFG_INF_LEN] __attribute__((section(".diagSectionData")));
uint8  gPPKValue[DID_PPK_VALUE_LEN]={1,0,0};
uint8  gSoftwareProgramDate[DID_SOFTWARE_PROGRAM_DATE_LEN]={0x20,0x17,0x01,0x23};
uint8  gEcuInstallDate[DID_ECU_INSTALL_DATE_LEN]={0x20,0x17,0x01,0x23};
//REALLOCATE_VAR2NVRAM_END

uint16  g0x22DidTotalNum = 0;

const t0x22DidInfo g0x22DidInfor[] = {	
	/* 1 vehicleManufacturerSparePartNumberDataIdentifier - ASCII*/
	{DID_VEHICLE_MANUFACTURER_PART_NUM, DID_READ,              UDS_INNER_SECURITY_LEVEL_0,                UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE,                             FALSE,                 DID_VEHICLE_MANUFACTURER_PART_NUM_LEN,      NULL,
	 0,     sid0x2eVerifygVehicleManufacturerSparePartNum,     sid0x22ReadVehicleManufacturerSparePartNum, NULL},
	
	/* 2 systemSupplierIdentifierDataIdentifier  - ASCII + BCD*/
	{DID_SYSTEM_SUPPLIER_DID,           DID_READ, UDS_INNER_SECURITY_LEVEL_0,            UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                             FALSE,    DID_SYSTEM_SUPPLIER_DID_LEN,           NULL,
	 0,                                 NULL,     sid0x22ReadSupplierDIDNum,             NULL},
	
	/* 3 SystemName  - ASCII */
	{DID_SYSTEM_NAME,                   DID_READ, UDS_INNER_SECURITY_LEVEL_0,            UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                             FALSE,    DID_SYSTEM_NAME_LEN,                   NULL,
	 0,                                 NULL,     sid0x22ReadSysName,                    NULL},
	
	/* 4 systemSupplierECUHardwareVersionNumberDataIdentifier  - BCD*/
	{DID_SYSTEM_SUPPLIER_ECU_HARDWARE_NUM,     DID_READ, UDS_INNER_SECURITY_LEVEL_0,               UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                    FALSE,    DID_SYSTEM_SUPPLIER_ECU_HARDWARE_NUM_LEN, (uint8*)&gSystemSupplierEcuHardwareNum,
	 0, NULL,     NULL,                                     NULL},

	/* 5 SystemSupplierECUSoftwareVersionNumberDataIdentifier  - BCD*/
	{DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM, DID_READ, UDS_INNER_SECURITY_LEVEL_0,               UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                FALSE,    DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM_LEN, NULL,
	 0,                                    NULL,     sid0x22ReadSupplierEcuSwNum,              NULL},
	 
	/* 6 ECUSerialNumberDataIdentifier  - ASCII*/
	{DID_ECU_SER_NUM,            DID_READ|DID_AME_WRITE, UDS_INNER_SECURITY_LEVEL_0, UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                      FALSE,                  DID_ECU_SER_NUM_LEN,        gECUSerialNumbe,     
	 0, NULL,                   NULL,                       sid0x2eWriteEcuSerialNum},
	 
	/* 7 VIN DataIdentifier -ASCII */
	{DID_VIN_DATA_ID,              DID_READ|DID_WRITE,           UDS_INNER_SECURITY_LEVEL_0, UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                        FALSE,                        DID_VIN_DATA_ID_LEN,        gVINDataID,
	 0,        sid0x2eVerifyVINDataDIDValue, NULL,                       NULL},
	 
	/* 8 供应商ECU引导软件参考  - ASCII*/
	{DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE, DID_READ,                       UDS_INNER_SECURITY_LEVEL_0,                UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                 FALSE,                          DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE_LEN, NULL,
	 0,                                     NULL,                           sid0x22ReadSupplierSWRefGuide,             NULL},
	 
	/* 9 these DIDs are running time states  - Hex(Unsigned)*/
	{DID_CURRENT_ACTIVE_SESSION, DID_READ, UDS_INNER_SECURITY_LEVEL_0,     UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                      FALSE,    DID_CURRENT_ACTIVE_SESSION_LEN, NULL,
	 0,                          NULL,     sid0x22ReadActiveDiagSession,   NULL},
	 
	/* 10 供应商ECU硬件参考 - ASCII*/
	{DID_SUPPLIER_HARDWARE_REFERENCE_GUIDE, DID_READ|DID_WRITE, UDS_INNER_SECURITY_LEVEL_0,                UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                                 FALSE,              DID_SUPPLIER_HARDWARE_REFERENCE_GUIDE_LEN, gSupplierHWRefGuide,
	 0,        NULL,               NULL,                                      NULL},
	 
	/* 11 诊断仪序列号 - ASCII+BCD*/
	{DID_DIAG_SER_NUM,       DID_READ|DID_WRITE, UDS_INNER_SECURITY_LEVEL_0,  UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                  FALSE,              DID_DIAG_SER_NUM_LEN,        gDiagSerNum, 
	 0, NULL,               NULL,                        NULL},
	
	/* 13 下载软件完整性状态 -Hex(Unsigned)*/	
	{DID_DOWNLOAD_SOFTWARE_INTEGRITY, DID_READ,  UDS_INNER_SECURITY_LEVEL_0,            UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                            FALSE,    DID_DOWNLOAD_SOFTWARE_INTEGRITY_LEN,   NULL,
	 0,                                NULL,     sid0x22ReadDownloadSoftwareIntegrity,  NULL},
	
	/* 14 下载软件兼容性状态 - Hex(Unsigned)*/
	{DID_DOWNLOAD_SOFTWARE_COMPATIBILITY, DID_READ, UDS_INNER_SECURITY_LEVEL_0,               UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                FALSE,    DID_DOWNLOAD_SOFTWARE_COMPATIBILITY_LEN, NULL,
	 0,                                    NULL,     sid0x22ReadDownloadSoftwareCompatibility, NULL},
	
	/* 15 软件有效标志 - Hex(Unsigned)*/
	{DID_SOFTWARE_VALIDITY, DID_READ, UDS_INNER_SECURITY_LEVEL_0,  UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                 FALSE,    DID_SOFTWARE_VALIDITY_LEN,   NULL,
	 0,                     NULL,     sid0x22ReadSoftwareValidity, NULL},
	
	/* 16 复位记录 - Hex(Unsigned)*/
	{DID_RESET_RECORD,        DID_READ, UDS_INNER_SECURITY_LEVEL_0,   UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                   FALSE,    DID_RESET_RECORD_LEN,         gResetRecord,
	 0, NULL,     NULL,                         NULL},
	
	/* 17 整车配置信息 - Hex(Unsigned)*/
	{DID_BODY_CFG_INF,       DID_READ|DID_WRITE|DID_AME_WRITE,  UDS_INNER_SECURITY_LEVEL_0,   UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                  FALSE,                             DID_BODY_CFG_INF_LEN,         gBodyCfgInf,
	 0, NULL,                              NULL,                         NULL},
	
	/* 18 PPK值 - Hex(Unsigned)*/
	{DID_PPK_VALUE,        DID_READ|DID_WRITE, UDS_INNER_SECURITY_LEVEL_0,   UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                FALSE,              DID_PPK_VALUE_LEN,            gPPKValue,
	 0, NULL,               NULL,                         NULL},
	
	/* 19 software program date - BCD*/
	{DID_SOFTWARE_PROGRAM_DATE,       DID_READ|DID_WRITE, UDS_INNER_SECURITY_LEVEL_0,    UDS_INNER_SECURITY_LEVEL_1,
	 FALSE,                           FALSE,              DID_SOFTWARE_PROGRAM_DATE_LEN, gSoftwareProgramDate,
	 0, NULL,               NULL,                          NULL},
};


void sid0x22Init()
{	
	g0x22DidTotalNum = sizeof(g0x22DidInfor)/sizeof(t0x22DidInfo);
}


