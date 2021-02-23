/**************************
description: public general process APIs for sid 0x22 and 0x2e of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2018/9/13
**************************/

#include"sid0x22_cfg.h"
#include"NvM.h"
#include"sid0x27_if.h"
#include"HardwareVersion.h"
#include "main.h"
//============================================================================================================
uint8 g_IVI_UdsInnerTestDid[IVI_DID_UDS_INNER_TEST_LEN] = {0x55,0x66,0x77,0x88};
//NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_SparePartsNumNissan[IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN] = {'2','5','9','1','5','2','G','S','1','A'};
//NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_SparePartsNumRenault[IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN] = {'0','1','2','3','4','5','6','7','8','9'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_SysSupplierNum[IVI_DID_SYS_SUPPLIER_NUM_LEN] = {'5','A','M','0'};

//DON'T USE THIS VARIANT[please use socSerialNum],BUT ALSO DONT' REMOVE IT EXCEPT ALSO DELETE IT IN NVM CFG
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_EcuSerialNum[IVI_DID_ECU_SERIAL_NUM_LEN] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_VIN[IVI_DID_VIN_LEN] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_VehicleManufEcuHwNum[IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN] = {'2','5','9','1','5','2','G','N','0','A'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_SysSupplierEcuSwNum[IVI_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN] = {0};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_SysSupplierEcuSwVersionNum[IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN] = {0};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_DiagIdCode[IVI_DID_DIAG_ID_CODE_LEN] = {0x50};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_OperationalReference[IVI_DID_OPERATIONAL_REFERENCE_LEN] = {'2','5','9','1','5','2','G','N','0','A'};
//============================================================================================================
uint8 g_IC_UdsInnerTestDid[IC_DID_UDS_INNER_TEST_LEN] = {0x55,0x66,0x77,0x88};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_SparePartsNumNissan[IC_DID_SPARE_PARTS_NUM_NISSAN_LEN] = {'2','4','8','1','0','2','G','N','0','A'};
//NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_SparePartsNumRenault[IC_DID_SPARE_PARTS_NUM_RENAULT_LEN] = {'0','1','2','3','4','5','6','7','8','9'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_DiagIdCode[IC_DID_DIAG_ID_CODE_LEN] = {0x43};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_SysSupplierNum[IC_DID_SYS_SUPPLIER_NUM_LEN] = {'B','0','9','1'};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_SysSupplierEcuSwNum[IC_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN] = {0};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_SysSupplierEcuSwVersionNum[IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN] = {0};
NVM_SEC_VAR_UNSPECIFIED uint8 g_IC_LatestSparePartsNum[IC_DID_LATEST_SPARE_PARTS_NUM_LEN] = {'2','5','9','1','5','2','G','N','0','B'};
//============================================================================================================
uint8 g_AVM_UdsInnerTestDid[AVM_DID_UDS_INNER_TEST_LEN] = {0x55,0x66,0x77,0x88};
NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_ProductionLine[AVM_DID_PRODUCTION_LINE_LEN] = {0}; //default 0
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_SparePartsNumNissan[AVM_DID_SPARE_PARTS_NUM_NISSAN_LEN] = {'0','1','2','3','4','5','6','7','8','9'}; //use the same as DA
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_VehicleManufEcuHwNum[AVM_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN] = {'0','1','2','3','4','5','6','7','8','9'}; //same as spare parts num
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_OperationalReference[AVM_DID_OPERATIONAL_REFERENCE_LEN] = {'0','0','0','0','0','0','0','0','0','0'}; //No CAN upgrade, all 0
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_SysSupplierNum[AVM_DID_SYS_SUPPLIER_NUM_LEN] = {0x42,0x30,0x39,0x31};  //ASCII: B091
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_EcuSerialNum[AVM_DID_ECU_SERIAL_NUM_LEN] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t'}; //USE DA sn
//NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_DiagIdCode[AVM_DID_DIAG_ID_CODE_LEN] = {0}; //default 0, not specified yet

//=====================these values can be changed=================
uint8 g_IVI_SparePartsNumNissan[IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN] = {'2','5','9','1','5','2','G','N','0','A'};
uint8 g_HUD_SparePartsNumNissan[HUD_DID_SPARE_PARTS_NUM_NISSAN_LEN] = {'2','4','9','4','1','2','G','N','0','A'};
//=============these are const value==============================
const uint8 g_AVM_BootVersion[AVM_DID_BOOT_VERSION_LEN] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
const uint8 g_AVM_OperationalReference[AVM_DID_OPERATIONAL_REFERENCE_LEN] = {'2','8','4','A','1','0','0','0','0','0'};
const uint8 g_AVM_SysSupplierNum[AVM_DID_SYS_SUPPLIER_NUM_LEN] = {0x42,0x30,0x39,0x31};  //ASCII: B091
const uint8 g_AVM_DiagIdCode[AVM_DID_DIAG_ID_CODE_LEN] = {0xF0};
//============================================================================================================
/*
uint8 g_HUD_
uint8 g_HUD_
uint8 g_HUD_
uint8 g_HUD_
uint8 g_HUD_
*/

//============================================================================================================

extern boolean gUdsChSupportFlag[];


uint16 g0x22DidTotalNum[UDS_CH_MAX_NUM] = {0};



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                    IVI                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const t0x22DidInfo g0x22DidInfor_IVI[] = {
#if 0
	// 1234 
	{IVI_DID_UDS_INNER_TEST,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_UDS_INNER_TEST_LEN,	                     NULL_PTR,
	 0, 												 NULL_PTR,
	 IVI_sid0x22ReadUdsInnerTestDid,		             NULL_PTR},
#endif
	// F1A1 
	{IVI_DID_SPARE_PARTS_NUM_NISSAN,					 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN,				 g_IVI_SparePartsNumNissan,
	 0, 												 FALSE, 						 
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

#if 0
	// F187 
	{IVI_DID_SPARE_PARTS_NUM_RENAULT,					 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN,				 NULL_PTR,
	 0, 												 NULL_PTR,
	 IVI_sid0x22ReadSparePartsNumRenault,				 IVI_sid0x2eWriteSparePartsNumRenault},
#endif

	// F18A 
	{IVI_DID_SYS_SUPPLIER_NUM,							 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SYS_SUPPLIER_NUM_LEN,						 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IVI_sid0x22ReadSysSupplierNum,
	 IVI_sid0x2eWriteSysSupplierNum},

	// F18C
	{IVI_DID_ECU_SERIAL_NUM,		                     DID_READ|DID_AME_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_ECU_SERIAL_NUM_LEN,	                     socSerialNum,
	 0, 												 FALSE, 						 
	 NULL_PTR,                                           NULL_PTR,		                 
	 IVI_sid0x2eWriteEcuSerialNum},

#if 1

	// F190
	{IVI_DID_VIN,		                                 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_VIN_LEN,	                                 g_IVI_VIN,
	 0, 												 FALSE, 						 NULL_PTR,
	 NULL,		                 						 IVI_sid0x2eWriteVin},
#endif

	// F191 
	{IVI_DID_VEHICLE_MANUF_ECU_HW_NUM,		             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN,	             g_IVI_VehicleManufEcuHwNum,
	 0, 												 FALSE, 						 NULL_PTR,
	 NULL_PTR,		         NULL_PTR},

	// F194 
	{IVI_DID_SYS_SUPPLIER_ECU_SW_NUM,		             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN,	             NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadSysSupplierEcuSwNum,		         NULL_PTR},

	// F195 
	{IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM,		     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN,	     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadSysSupplierEcuSwVersionNum,		     NULL_PTR},

	// F1A0 
	{IVI_DID_DIAG_ID_CODE,		                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_DIAG_ID_CODE_LEN,	                         NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadDiagIdCode,		                     NULL_PTR},

	// F012 
	{IVI_DID_OPERATIONAL_REFERENCE,		                 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_OPERATIONAL_REFERENCE_LEN,	                 NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadOperationalReference,		         NULL_PTR},

	// EF82 
	{IVI_DID_CAN_DIAG_MUX_STATE,					     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_CAN_DIAG_MUX_STATE_LEN,				     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadCanDiagMuxState,				     NULL_PTR},

	// 0101 
	{IVI_DID_INPUT_LINES,								 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_INPUT_LINES_LEN,							 NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadInputLines, 						 NULL_PTR},

	// 0102 
	{IVI_DID_TCU_ACC, 							         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_TCU_ACC_LEN, 						         NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadTcuAcc,						         NULL_PTR},
	
	// 0200 
	{IVI_DID_BATTERY_LEVEL, 						     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_BATTERY_LEVEL_LEN, 					     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadBatteryLevel,					     NULL_PTR},

	// 0201 
	{IVI_DID_SPEED_INFORMATION,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IVI_DID_SPEED_INFORMATION_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IVI_sid0x22ReadSpeedInformation,		             NULL_PTR},
	
};


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                     IC                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const t0x22DidInfo g0x22DidInfor_IC[] = {
#if 0
	// 1234 
	{IC_DID_UDS_INNER_TEST,		                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_UDS_INNER_TEST_LEN,	                         NULL_PTR,
	 0, 												 NULL_PTR,
	 IC_sid0x22ReadUdsInnerTestDid,		                 NULL_PTR},
#endif

	// F1A1 
	{IC_DID_SPARE_PARTS_NUM_NISSAN,		                 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SPARE_PARTS_NUM_NISSAN_LEN,	                 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSparePartsNumNissan,
	 IC_sid0x2eWriteSparePartsNumNissan},

#if 0
	// F187 
	{IC_DID_SPARE_PARTS_NUM_RENAULT,		             DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SPARE_PARTS_NUM_RENAULT_LEN,	             NULL_PTR,
	 0, 												 NULL_PTR,
	 IC_sid0x22ReadSparePartsNumRenault,		         IC_sid0x2eWriteSparePartsNumRenault},
#endif

	// F1A0 
	{IC_DID_DIAG_ID_CODE,		                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_DIAG_ID_CODE_LEN,	                         NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDiagIdCode,
	 NULL_PTR},

	// F18A
	{IC_DID_SYS_SUPPLIER_NUM,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SYS_SUPPLIER_NUM_LEN,	                     NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierNum,
	 IC_sid0x2eWriteSysSupplierNum},
	
	// F194 
	{IC_DID_SYS_SUPPLIER_ECU_SW_NUM,		             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN,	             NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierSwNum,
	 NULL_PTR},
	
	// F195 
	{IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM,		     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN,	     NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierSwVersionNum,
	 NULL_PTR},

	// F012 
	{IC_DID_LATEST_SPARE_PARTS_NUM,		                 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_LATEST_SPARE_PARTS_NUM_LEN,	                 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadLatestSparePartsNum,
	 IC_sid0x2eWriteLatestSparePartsNum},
	
	// F40D 
	{IC_DID_SPEED_OMETER,		                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SPEED_OMETER_LEN,	                         NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSpeedOmeter,
	 NULL_PTR},
	
	// 0E00 
	{IC_DID_SPEED_OUTPUT,                                DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE,                                              FALSE,                 
	 IC_DID_SPEED_OUTPUT_LEN,                            NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSpeedOutput,
	 NULL_PTR},

	// 0E01 
	{IC_DID_ODOMETER_OUTPUT,                             DID_READ, 
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE,    
	 IC_DID_ODOMETER_OUTPUT_LEN,                         NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadOdometerOutput,
	 NULL_PTR},
	
	// 0E02  
	{IC_DID_ODOMETER,                                    DID_READ, 
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE,    
	 IC_DID_ODOMETER_LEN,                                NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadOdometer,
	 NULL_PTR},
	
	// 0E03 
	{IC_DID_FUEL_METER,                                  DID_READ, 
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE,    
	 IC_DID_FUEL_METER_LEN,                              NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadFuelMeter,
	 NULL_PTR},

	// 0E04 
	{IC_DID_WATER_TEMPERATURE,                           DID_READ, 
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE,    
	 IC_DID_WATER_TEMPERATURE_LEN,                       NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadWaterTemperature,
	 NULL_PTR},

	 // 0E05 ,reserved
	 // 0E06 ,reserved

	// 0E07 
	{IC_DID_0E07_INFO,                                   DID_READ, 
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              TRUE,  
	 IC_DID_0E07_INFO_LEN,                               NULL_PTR,     
	 0,                                                  FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},
	 
	// 0E08  
	{IC_DID_DISTANCE_TO_EMPTY,                           DID_READ,          
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE,                        
	 IC_DID_DISTANCE_TO_EMPTY_LEN,                       NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDistanceToEmpty,
	 NULL_PTR},
		
	// 0E09 
	{IC_DID_AMBIENT_TEMPERATURE,                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_AMBIENT_TEMPERATURE_LEN,                     NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadAmbientTemperature,
	 NULL_PTR},

	// 0E0A
	{IC_DID_0E0A_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E0A_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E0AInfo,
	 NULL_PTR},

#if 0
	// 0E0B 
	{IC_DID_GENERAL_POWER_METER,                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_GENERAL_POWER_METER_LEN,                     NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadGeneralPowerMeter,                    NULL_PTR},

	// 0E0C 
	{IC_DID_SOC_METER,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_SOC_METER_LEN,                               NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadSocMeter,                             NULL_PTR},

	// 0E0D 
	{IC_DID_VOLT_METER,                                  DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_VOLT_METER_LEN,                              NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadVoltMeter,                            NULL_PTR},

	// 0E0E 
	{IC_DID_BRAKE_PAD,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_BRAKE_PAD_LEN,                               NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadBrakePad,                             NULL_PTR},
#endif

	// 0E0F 
	{IC_DID_0E0F_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E0F_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E0FInfo,
	 NULL_PTR},

#if 0
	// 0E10 
	{IC_DID_ENG_SHUT_TIMER,                              DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_ENG_SHUT_TIMER_LEN,                          NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadEngShutTimer,                         NULL_PTR},

	// 0E11 
	{IC_DID_TORQUE_DISTRIBUTION,                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_TORQUE_DISTRIBUTION_LEN,                     NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadTorqueDistribution,                   NULL_PTR},
#endif

	// 0E12 
	{IC_DID_0E12_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E12_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E12Info,
	 NULL_PTR},

	// 0E13 
	{IC_DID_CC_SL_REQ,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_CC_SL_REQ_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadCcSlReq,
	 NULL_PTR},

#if 0
	// 0E14 
	{IC_DID_SCR_REMAINING,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_SCR_REMAINING_LEN,                           NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadScrRemaining,                         NULL_PTR},

	// 0E15 
	{IC_DID_SCR_DISTANCE,                                DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_SCR_DISTANCE_LEN,                            NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadScrDistance,                          NULL_PTR},

	// 0E16 
	{IC_DID_OIL_DISTANCE,                                DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_OIL_DISTANCE_LEN,                            NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadOilDistance,                          NULL_PTR},

	// 0E17 
	{IC_DID_OIL_RESET,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_OIL_RESET_LEN,                               NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,              
	 IC_sid0x22ReadOilReset,                             NULL_PTR},

	// 0E18 
	{IC_DID_ABSOLUTE_TIME,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_ABSOLUTE_TIME_LEN,                           NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadAbsoluteTime,                         NULL_PTR},
#endif

	// 0E19 
	{IC_DID_0E19_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E19_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E19Info,
	 NULL_PTR},

#if 0
	// 0E1A 
	{IC_DID_ASST_CHG,                                    DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_ASST_CHG_LEN,                                NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadAsstChg,                              NULL_PTR},

	// 0E1B 
	{IC_DID_EV_INDICATOR,                                DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_EV_INDICATOR_LEN,                            NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadEvIndicator,                          NULL_PTR},

	// 0E1C 
	{IC_DID_APO_DISPLAY,                                 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_APO_DISPLAY_LEN,                             NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadApoDisplay,                           NULL_PTR},

	// 0E1D 
	{IC_DID_SOC_DISPLAY,                                 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_SOC_DISPLAY_LEN,                             NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSocDisplay,                           NULL_PTR},

	// 0E1E 
	{IC_DID_ECO_APO,                                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_ECO_APO_LEN,                                 NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadEcoApo,                               NULL_PTR},

	// 0E1F 
	{IC_DID_REGEN_BRAKE_TRQ,                             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_REGEN_BRAKE_TRQ_LEN,                         NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadRegenBrakeTrq,                        NULL_PTR},

	// 0E20 
	{IC_DID_MECHA_BRAKE_TRQ,                             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_MECHA_BRAKE_TRQ_LEN,                         NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadMechaBrakeTrq,                        NULL_PTR},

	// 0E21 
	{IC_DID_ENERGY_FLOW,                                 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_ENERGY_FLOW_LEN,                             NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadEnergyFlow,                           NULL_PTR},
#endif

#if 0
	// 0E22 
	{IC_DID_0E22_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E22_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E22Info,
	 NULL_PTR},
#endif


#if 0
	// 0E23 
	{IC_DID_OIL_DIST_K9K,                                DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_OIL_DIST_K9K_LEN,                            NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadOilDistK9k,                           NULL_PTR},
#endif

	// 0E24 
	{IC_DID_0E24_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E24_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E24Info,
	 NULL_PTR},

	// 0E25 
	{IC_DID_TIRE_PRESS_FR,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_TIRE_PRESS_FR_LEN,                           NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadTirePressFr,
	 NULL_PTR},
	 
	// 0E26 
	{IC_DID_TIRE_PRESS_FL,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_TIRE_PRESS_FL_LEN,                           NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadTirePressFl,
	 NULL_PTR},

	// 0E27 
	{IC_DID_TIRE_PRESS_RR,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_TIRE_PRESS_RR_LEN,                           NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadTirePressRr,
	 NULL_PTR},

	// 0E28 
	{IC_DID_TIRE_PRESS_RL,                               DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_TIRE_PRESS_RL_LEN,                           NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadTirePressRl,
	 NULL_PTR},

	// 0E29 
	{IC_DID_0E29_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E29_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDid0E29Info,
	 NULL_PTR},

#if 0
	// 0E2A 
	{IC_DID_RAM_ERROR_DATA,                              DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_RAM_ERROR_DATA_LEN,                          NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadRamErrorData,                         NULL_PTR},

	// 0E2B 
	{IC_DID_0E2B_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E2B_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadDid0E2BInfo,                          NULL_PTR},

	// 0E2C 
	{IC_DID_0E2C_INFO,                                   DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,                         UDS_INNER_SECURITY_LEVEL_0,
	 FALSE,                                              FALSE, 
	 IC_DID_0E2C_INFO_LEN,                               NULL_PTR,
	 0,                                                  FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadDid0E2CInfo,                          NULL_PTR},
#endif

	// 0E80 
	{IC_DID_DISPLAY_NONDISPLAY_INFO, 					 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE, 
	 IC_DID_DISPLAY_NONDISPLAY_INFO_LEN, 				 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDisplayNondisplayInfo,
	 NULL_PTR},

	// 0E90 
	{IC_DID_WARNING_LAMP_LIGHTING_RECORDS, 				 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE, 
	 IC_DID_WARNING_LAMP_LIGHTING_RECORDS_LEN, 			 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadWarningLampLightingRecords,
	 NULL_PTR},

#if 0
	// DA70
	{IC_DID_WARNING_LAMP_LIGHTING_MILEAGE, 				 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 IC_DID_WARNING_LAMP_LIGHTING_MILEAGE_LEN, 			 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadWarningLampLightingMileage,
	 NULL_PTR},
#endif


	// EF82 
	{IC_DID_CAN_COMMUNICATION_SUPPORT_INFO, 			 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE, 
	 IC_DID_CAN_COMMUNICATION_SUPPORT_INFO_LEN, 		 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadCanCommunicationSupportInfo,
	 NULL_PTR},

#if 0
	// F0D0 
	{IC_DID_SSD_TOTAL_MILEAGE,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SSD_TOTAL_MILEAGE_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSsdTotalMileage,		                 NULL_PTR},

	// F0D1 
	{IC_DID_SSD_BAT_VOLTAGE,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SSD_BAT_VOLTAGE_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSsdBatVoltage,		                 NULL_PTR},

	// F0D2 
	{IC_DID_SSD_IGN_VOLTAGE,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SSD_IGN_VOLTAGE_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSsdIgnVoltage,		                 NULL_PTR},

	// F0D3 
	{IC_DID_SSD_AGING_COUNTER,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SSD_AGING_COUNTER_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSsdAgingCounter,		                 NULL_PTR},

	// F0D4 
	{IC_DID_SSD_VEHICLE_SPEED,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 IC_DID_SSD_VEHICLE_SPEED_LEN,	                     NULL_PTR,
	 0, 												 FALSE, 						 NULL_PTR,
	 IC_sid0x22ReadSsdVehicleSpeed,		                 NULL_PTR},
#endif


	  // 0104
	 {IC_DID_SSD_AGING_RESULT,							  DID_READ,
	  UDS_INNER_SECURITY_LEVEL_0,						  UDS_INNER_SECURITY_LEVEL_0,
	  FALSE,											  TRUE,
	  IC_DID_SSD_AGING_RESULT_LEN,						  NULL_PTR,
	  0,												  TRUE,
	  NULL_PTR, 										  NULL_PTR,
	  NULL_PTR},
	  

	 // 0A01
 	{IC_DID_SSD_MEDIA_SOURCE,		                     DID_READ,
 	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
 	 FALSE, 											 TRUE,
 	 IC_DID_SSD_MEDIA_SOURCE_LEN,	                     NULL_PTR,
 	 0, 												 TRUE,
 	 NULL_PTR,                                           NULL_PTR,
 	 NULL_PTR},


	// 0A05
    {IC_DID_SSD_CURRENT_SONG_NAME,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_CURRENT_SONG_NAME_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0404
    {IC_DID_SSD_BT_ADDRESS,								DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_BT_ADDRESS_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},
	
	// 0405
    {IC_DID_SSD_BT_CONNECT_STATE,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_BT_CONNECT_STATE_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0406
    {IC_DID_SSD_BT_CALL_STATE,						    DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_BT_CALL_STATE_LEN,					    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0304
    {IC_DID_SSD_WIFI_SIGNAL_NUM,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_SIGNAL_NUM_LEN,					    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0307
    {IC_DID_SSD_WIFI_SIGNAL_INTENSITY,				    DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_SIGNAL_INTENSITY_LEN,				NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0308
    {IC_DID_SSD_WIFI_CONNECT_STATE,				        DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_CONNECT_STATE_LEN,				    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 030C
    {IC_DID_SSD_WIFI_CONNECT_NUM,				        DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_CONNECT_NUM_LEN,				    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0603
    {IC_DID_SSD_WIFI_GPS_INFO,				            DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_GPS_INFO_LEN,				        NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 060D
    {IC_DID_SSD_WIFI_GPS_DETAIL,				        DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_WIFI_GPS_DETAIL_LEN,				        NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0701
    {IC_DID_SSD_SYS_VERSION,				            DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_VERSION_LEN,				            NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0704
    {IC_DID_SSD_SYS_AUDIO_VOLUME,				        DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_AUDIO_VOLUME_LEN,				    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0705
    {IC_DID_SSD_SYS_UUID,				        		DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												FALSE,
 	IC_DID_SSD_SYS_UUID_LEN,				    		NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           IC_sid0x22ReadUUID,
	NULL_PTR},

	// 0708
    {IC_DID_SSD_SYS_WIRELESS_CHARGE_STATE,				DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_WIRELESS_CHARGE_STATE_LEN,			NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0709
    {IC_DID_SSD_SYS_STOP_SIGNAL,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_STOP_SIGNAL_LEN,						NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 070A
    {IC_DID_SSD_SYS_STEER_SIGNAL,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_STEER_SIGNAL_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 070B
    {IC_DID_SSD_SYS_REVERSE_SIGNAL,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_SYS_REVERSE_SIGNAL_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},


	// 0711
    {IC_DID_SSD_SYS_EQPIN,								DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												FALSE,
 	IC_DID_SSD_SYS_EQPINL_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           IC_sid0x22ReadEQPIN,
 	NULL_PTR},

	// 0F01
    {IC_DID_SSD_VOICE_ASSIS_PAGE,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_VOICE_ASSIS_PAGE_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0501
    {IC_DID_SSD_TCU_CONNECT_STATE,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_TCU_CONNECT_STATE_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0502
    {IC_DID_SSD_TCU_NET_CONNECT_STATE,					DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_TCU_NET_CONNECT_STATE_LEN,			    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0503
    {IC_DID_SSD_TCU_4G_SIGNAL_INTENSITY,				DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
 	IC_DID_SSD_TCU_4G_SIGNAL_INTENSITY_LEN,			    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},
 	
	// 0001
    {IC_DID_SSD_MAIN_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_MAIN_VERSION_LEN,					    NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0002
    {IC_DID_SSD_HARDWARE_VERSION,						DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_HARDWARE_VERSION_LEN,					NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0003
    {IC_DID_SSD_MCU_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_MCU_VERSION_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0004
    {IC_DID_SSD_SOC_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_SOC_VERSION_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0005
    {IC_DID_SSD_MCAN_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_MCAN_VERSION_LEN,						NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0006
    {IC_DID_SSD_BT_SW_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_BT_SW_VERSION_LEN,						NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0007
    {IC_DID_SSD_GPS_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_GPS_VERSION_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0008
    {IC_DID_SSD_VR_VERSION,							    DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_VR_VERSION_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

	// 0009
    {IC_DID_SSD_AVM_VERSION,							DID_READ,
 	UDS_INNER_SECURITY_LEVEL_0, 						UDS_INNER_SECURITY_LEVEL_0,
 	FALSE,												TRUE,
	IC_DID_SSD_AVM_VERSION_LEN,							NULL_PTR,
 	0,													TRUE,
 	NULL_PTR,                                           NULL_PTR,
 	NULL_PTR},

};



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                    AVM                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const t0x22DidInfo g0x22DidInfor_AVM[] = {
#if 0
	// 1234 
	{AVM_DID_UDS_INNER_TEST,							 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_UDS_INNER_TEST_LEN,						 NULL_PTR,
	 0, 												 NULL_PTR,
	 AVM_sid0x22ReadUdsInnerTestDid,					 NULL_PTR},
#endif
	 //0101
	{AVM_DID_PRODUCTION_LINE,				 			 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 AVM_DID_PRODUCTION_LINE_LEN,			 			 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadProductionLine,
	 AVM_sid0x2eWriteProductionLine},

	 //0102
	{AVM_DID_DATA_MONITOR_SERVICE_REQUEST,				 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE,
	 AVM_DID_DATA_MONITOR_SERVICE_REQUEST_LEN,			 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadDataMonitorServiceRequest,
	 NULL_PTR},

	//0110
	{AVM_DID_CALIBRATION_STATE,				             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE,
	 AVM_DID_CALIBRATION_STATE_LEN,			             NULL,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadCalibrationState,
	 NULL_PTR},

	//EF82
	{AVM_DID_CAN_DIAG_MUX_STATE,				         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 AVM_DID_CAN_DIAG_MUX_STATE_LEN,			         NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadCanDiagMuxState,
	 NULL_PTR},


	//F199
	{AVM_DID_PROGRAMING_DATE,				             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE,
	 AVM_DID_PROGRAMING_DATE_LEN,			             NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadProgramingDate,
	 NULL_PTR},


	//F1A1
	{AVM_DID_SPARE_PARTS_NUM_NISSAN,				     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_SPARE_PARTS_NUM_NISSAN_LEN,			     g_IVI_SparePartsNumNissan,
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

	//F191
	{AVM_DID_VEHICLE_MANUF_ECU_HW_NUM,					 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN,				 g_IVI_VehicleManufEcuHwNum,/*same as IVI*/
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

#if 0
	//F012
	{AVM_DID_OPERATIONAL_REFERENCE,					     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_OPERATIONAL_REFERENCE_LEN,				     g_AVM_OperationalReference,
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},
#endif

	// F18A
	{AVM_DID_SYS_SUPPLIER_NUM,					         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_SYS_SUPPLIER_NUM_LEN,				         (uint8 *)g_AVM_SysSupplierNum,
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

	// F18C
	{AVM_DID_ECU_SERIAL_NUM,					         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_ECU_SERIAL_NUM_LEN,				         socSerialNum,/*g_IVI_EcuSerialNum,*/
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

	// F195
	{AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM,			 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE,
	 AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN,		 NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           AVM_sid0x22ReadSysSupplierEcuSwVersionNum,
	 NULL_PTR},

	// F1A0
	{AVM_DID_DIAG_ID_CODE,			                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_DIAG_ID_CODE_LEN,		                     (uint8 *)g_AVM_DiagIdCode,
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},

#if 0
	// F180
	{AVM_DID_BOOT_VERSION,			                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 AVM_DID_BOOT_VERSION_LEN,		                     g_AVM_BootVersion,
	 0, 												 FALSE,
	 NULL_PTR,                                           NULL_PTR,
	 NULL_PTR},
#endif

	 // FD04
	 {AVM_DID_ENCRYPT_IC_STATUS,			             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 AVM_DID_ENCRYPT_IC_STATUS_LEN,		                 NULL_PTR,
	 0, 												 TRUE,
	 NULL_PTR,                                           AVM_sid0x22ReadEncryptICStatus,
	 NULL_PTR},

	// FD05
	{AVM_DID_ORIGINAL_STATUS, 							 DID_READ,
	UDS_INNER_SECURITY_LEVEL_0, 						 UDS_INNER_SECURITY_LEVEL_0,
	FALSE,												 FALSE,
	AVM_DID_ORIGINAL_STATUS_LEN,						 NULL_PTR,
	0,													 TRUE,
	NULL_PTR,											 AVM_sid0x22ReadOriginalStatus,
	NULL_PTR},

};


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@                                                    HUD                                                         @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
const t0x22DidInfo g0x22DidInfor_HUD[] = {

	//F1A1
	{HUD_DID_SPARE_PARTS_NUM_NISSAN,				     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 HUD_DID_SPARE_PARTS_NUM_NISSAN_LEN,			     g_HUD_SparePartsNumNissan,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 NULL_PTR},

	// F1A0 
	{HUD_DID_DIAG_ID_CODE,		                         DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 HUD_DID_DIAG_ID_CODE_LEN,	                         NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadDiagIdCode,
	 NULL_PTR},

	// F18A
	{HUD_DID_SYS_SUPPLIER_NUM,		                     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 HUD_DID_SYS_SUPPLIER_NUM_LEN,	                     NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierNum,
	 IC_sid0x2eWriteSysSupplierNum},
	
	// F194 
	{HUD_DID_SYS_SUPPLIER_ECU_SW_NUM,		             DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 HUD_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN,	             NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierSwNum,
	 NULL_PTR},
	
	// F195 
	{HUD_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM,		     DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0, 
	 FALSE, 											 FALSE, 				
	 HUD_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN,	     NULL_PTR,
	 0, 												 FALSE,
	 NULL_PTR,                                           IC_sid0x22ReadSysSupplierSwVersionNum,
	 NULL_PTR},

	//F012
	{HUD_DID_OPERATIONAL_REFERENCE,				     	 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 FALSE,
	 HUD_DID_OPERATIONAL_REFERENCE_LEN,			         g_HUD_SparePartsNumNissan,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 NULL_PTR},
	 

	//01A1
	{HUD_DID_INI_CALIBRATION_DATA,				     	 DID_READ,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_INI_CALIBRATION_DATA_LEN,			     	 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 NULL_PTR},

	//01A2
	{HUD_DID_NEW_CALIBRATION_DATA,				     	 DID_READ|DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 TRUE, 											 	 TRUE,
	 HUD_DID_NEW_CALIBRATION_DATA_LEN,			     	 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteCalibrationData},
		
	//0111
	{HUD_DID_MAIN_SW_ONOFF_USER1,						 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USER1_LEN,					 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},

	//0121
	{HUD_DID_MAIN_SW_ONOFF_USER2,						 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USER2_LEN,					 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},

	//0131
	{HUD_DID_MAIN_SW_ONOFF_USER3,						 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USER3_LEN,					 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},

	//0141
	{HUD_DID_MAIN_SW_ONOFF_USER4,						 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USER4_LEN,					 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},

	//0151
	{HUD_DID_MAIN_SW_ONOFF_USER5,						 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USER5_LEN,					 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},
	 
	//0161
	{HUD_DID_MAIN_SW_ONOFF_USERDEFAULT,					 DID_WRITE,
	 UDS_INNER_SECURITY_LEVEL_0,						 UDS_INNER_SECURITY_LEVEL_0,
	 FALSE, 											 TRUE,
	 HUD_DID_MAIN_SW_ONOFF_USERDEFAULT_LEN,				 NULL_PTR,
	 0, 												 FALSE, 						 
	 NULL_PTR,											 NULL_PTR,
	 HUD_sid0x2eWriteMainSWONOFF},

};


const t0x22DidInfo* g0x22DidInforArray[UDS_CH_MAX_NUM] = {
	//Don't change the order!!!!!
	g0x22DidInfor_IVI,
	g0x22DidInfor_IC,
	g0x22DidInfor_AVM,
	g0x22DidInfor_HUD,
};


void sid0x22Init()
{	
	uint8 ampVersion = 0;
	g0x22DidTotalNum[0] = sizeof(g0x22DidInfor_IVI)/sizeof(t0x22DidInfo);
	g0x22DidTotalNum[1] = sizeof(g0x22DidInfor_IC)/sizeof(t0x22DidInfo);
	g0x22DidTotalNum[2] = sizeof(g0x22DidInfor_AVM)/sizeof(t0x22DidInfo);
	g0x22DidTotalNum[3] = sizeof(g0x22DidInfor_HUD)/sizeof(t0x22DidInfo);

	ampVersion = getITMasterCtrlVerion();

	switch (ampVersion)
	{
	#if 0
		case HSAE_CA004:

		
			gUdsChSupportFlag[UDS_AVM] = FALSE;
			gUdsChSupportFlag[UDS_HUD] = FALSE;
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GN0A", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GN0A", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GN0A", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
		
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0A", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			break;
		case HSAE_CA005:
		
			gUdsChSupportFlag[UDS_AVM] = FALSE;
			gUdsChSupportFlag[UDS_HUD] = FALSE;
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GN0A", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GN0A", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GN0A", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
		
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0A", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			break;			
		case HSAE_CA002:
		
			gUdsChSupportFlag[UDS_AVM] = FALSE;
			gUdsChSupportFlag[UDS_HUD] = FALSE;
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GN0B", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GN0B", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GN0B", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
			
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0B", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA			
			break;
		case HSAE_CA003:
			gUdsChSupportFlag[UDS_HUD] = FALSE;
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GN0C", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GN0C", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GN0C", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
		
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0B", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA			
			break;
	#endif
	
		case HSAE_CA008:
			
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GU0C", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GU0C", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GU0C", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
			
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0B", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA			
			break;

		default:
			diagCpyData(g_IVI_SparePartsNumNissan, (const uint8*)"259152GU0B", IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_VehicleManufEcuHwNum, (const uint8*)"259152GU0B", IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN); //AVM use the same value as DA
			diagCpyData(g_IVI_OperationalReference, (const uint8*)"259152GU0B", IVI_DID_OPERATIONAL_REFERENCE_LEN); //AVM use the same value as DA
			
			diagCpyData(g_IC_SparePartsNumNissan, (const uint8*)"248102GN0B", IC_DID_SPARE_PARTS_NUM_NISSAN_LEN); //AVM use the same value as DA	

			break;
	}
	
}


