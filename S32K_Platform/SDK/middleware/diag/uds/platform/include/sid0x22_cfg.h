#ifndef _SID_0X22_CFG_H
#define _SID_0X22_CFG_H
#include"std_types.h"
#include"sid0x22_if.h"

#define DID_INNER_TEST 0x1357
#define SUPPLIER_SPECIFIC_PART_NUM_LEN 20 // VARILABLE LENGTH
#define ECU_PART_NUM_LEN 6
#define CALIBRATION_SW_PART_NUM_LEN 7
#define IMP_SID_0X14_DATE_LEN 3

//N536
#define DID_VEHICLE_MANUFACTURER_PART_NUM_LEN       12
#define DID_SYSTEM_SUPPLIER_DID_LEN                 10
#define DID_SYSTEM_NAME_LEN                         6
#define DID_SYSTEM_SUPPLIER_ECU_HARDWARE_NUM_LEN	2
#define DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM_LEN	2
#define DID_ECU_SER_NUM_LEN	 					    10 
#define DID_VIN_DATA_ID_LEN	 				        17
#define DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE_LEN   4
#define DID_CURRENT_ACTIVE_SESSION_LEN				1
#define DID_SUPPLIER_HARDWARE_REFERENCE_GUIDE_LEN   10
#define DID_DIAG_SER_NUM_LEN						3
#define DID_DOWNLOAD_SOFTWARE_INTEGRITY_LEN		1
#define DID_DOWNLOAD_SOFTWARE_COMPATIBILITY_LEN	1
#define DID_SOFTWARE_VALIDITY_LEN					1
#define DID_RESET_RECORD_LEN						2 
#define DID_BODY_CFG_INF_LEN						40
#define DID_PPK_VALUE_LEN							3
#define DID_SOFTWARE_PROGRAM_DATE_LEN				4
#define DID_ECU_INSTALL_DATE_LEN					4


#define PROGRAMMING_FINGERPRINT_BACKUP_MAX 5

typedef enum{
	//N536
	DID_VEHICLE_MANUFACTURER_PART_NUM    = 0xf187,
	DID_SYSTEM_SUPPLIER_DID              = 0xf18a,
	DID_SYSTEM_NAME                      = 0xf197,
	DID_SYSTEM_SUPPLIER_ECU_HARDWARE_NUM = 0xf193,
	DID_SYSTEM_SUPPLIER_ECU_SOFTWARE_NUM = 0xf195,
	DID_ECU_SER_NUM                      = 0xf18c, 
	DID_VIN_DATA_ID                      = 0xf190,
	DID_SUPPLIER_SOFTWARE_REFERENCE_GUIDE= 0xf183,  //供应商ECU引导软件参考
	DID_CURRENT_ACTIVE_SESSION           = 0xf186,
	DID_SUPPLIER_HARDWARE_REFERENCE_GUIDE= 0xf192,  //供应商ECU硬件参考
	DID_DIAG_SER_NUM                     = 0xf198, //诊断仪序列号
	DID_DOWNLOAD_SOFTWARE_INTEGRITY      = 0xf1fa, //下载软件完整性状态
	DID_DOWNLOAD_SOFTWARE_COMPATIBILITY  = 0xf1fb, //下载软件兼容性状态
	DID_SOFTWARE_VALIDITY                = 0xf1fc, //软件有效标志
	DID_RESET_RECORD                     = 0xb303, //复位记录
	DID_BODY_CFG_INF                     = 0xf1a8, //整车配置信息
	DID_PPK_VALUE                        = 0x5003, //PPK值
	DID_SOFTWARE_PROGRAM_DATE            = 0xf199, //software program date
	DID_ECU_INSTALL_DATE 			     = 0xf19d  //ECU install date
	
}t0x22DidNum;

//===============================================================

uint16 sid0x22ReadActiveDiagSession(uint8 *p);
tUdsNRCType verifyDate(uint16 didNum,const uint8*date,uint16 len);
uint16 sid0x22ReadVehicleManufacturerSparePartNum(uint8*respData);
tUdsNRCType sid0x2eVerifygVehicleManufacturerSparePartNum(uint16 didNum,const uint8*didValue,uint16 len);
uint16 sid0x22ReadSupplierDIDNum(uint8*respData);
uint16 sid0x22ReadSysName(uint8*respData);
uint16 sid0x22ReadEcuSertNum(uint8*respData);
uint16 sid0x22ReadSupplierSWRefGuide(uint8*respData);
uint16 sid0x22ReadDownloadSoftwareIntegrity(uint8*respData);
uint16 sid0x22ReadDownloadSoftwareCompatibility(uint8*respData);
uint16 sid0x22ReadSoftwareValidity(uint8*respData);
uint16 sid0x22ReadSupplierEcuSwNum(uint8*respData);
boolean sid0x2eWriteEcuSerialNum(uint16 didNum,const uint8 *value,uint16 len);
tUdsNRCType sid0x2eVerifyVINDataDIDValue(uint16 didNum,const uint8*didValue,uint16 len);

#endif