/********************************************************************
Author: Li Qi
Version: V1.0
Timestamp: 2019-01-23 21:31:11
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _DTC_CORE_CFG_H
#define _DTC_CORE_CFG_H

#include"dtcCore.h"
#include"diagCommon_cfg.h"
#include "MemMap.h"

#define DTC_20_PERCENT_STORED_NUM_IVI 4
#define DTC_20_PERCENT_STORED_NUM_IC 6

typedef enum _tSnapshortType{
	GLOBAL_SNAPSHORT_RECORED_NUM = 1,
	LOCAL_SNAPSHORT_RECORD_NUM = 2,
	ALL_SNAPSHORT_RECORD_NUM = 0xff,
}tSnapshortType;

typedef enum {
	GLOBAL_EXTENDED_DATA_RECORED_NUM = 1,
	LOCAL_EXTENDED_DATA_RECORD_NUM = 2,
	ALL_EXTENDED_DATA_RECORD_NUM = 0xff,
}tDtcExtDataRecordNum;

typedef struct {
	uint8 odometer[3];	// ID:0xf0d0
	//uint8 Occurrence;	// ID:0xf0d1
	uint8 supplyVoltage; // ID:0xf0d2
}tDtcGlobalSnapshotRecord_IVI;


typedef struct{
	tDtcGlobalSnapshotRecord_IVI globalSnapshot;
	//tDtcLocalSnapshotRecord localSnapshot;
}tDtcSnapshot_IVI;


typedef struct {
	uint8 odometer[3];	// ID:0xf0d0
	uint8 batVoltage; // ID:0xf0d1
	uint8 ignVoltage; // ID:0xf0d2
	//uint8 agingCounter;	//ID:0xf0d3
	uint8 vehicleSpeed[2];	// ID: 0xf0d4
}tDtcGlobalSnapshotRecord_IC;


typedef struct{
	tDtcGlobalSnapshotRecord_IC globalSnapshot;
	//tDtcLocalSnapshotRecord localSnapshot;
}tDtcSnapshot_IC;

typedef struct {
	uint8 odometer[3];	// ID:0xf1d1
}tDtcGlobalSnapshotRecord_AVM;


typedef struct{
	tDtcGlobalSnapshotRecord_AVM globalSnapshot;
	//tDtcLocalSnapshotRecord localSnapshot;
}tDtcSnapshot_AVM;

typedef struct {
	uint8 odometer[3];	// ID:0xf1d1
}tDtcGlobalSnapshotRecord_HUD;


typedef struct{
	tDtcGlobalSnapshotRecord_HUD globalSnapshot;
	//tDtcLocalSnapshotRecord localSnapshot;
}tDtcSnapshot_HUD;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	IVI_DTC_ID_INDEX_CAN_COMMUNICATION_SYSTEM         = 0,
	IVI_DTC_ID_INDEX_CAN_CONTROLLER_ERR               = 1,
	IVI_DTC_ID_INDEX_ECU_SOC                          = 2,
	IVI_DTC_ID_INDEX_ECU_GYRO                         = 3,
	IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_GND      = 4,
	IVI_DTC_ID_INDEX_FL_LOUDSPEAKER_SHORT_TO_BAT      = 5,
	IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_GND      = 6,
	IVI_DTC_ID_INDEX_FR_LOUDSPEAKER_SHORT_TO_BAT      = 7,
	IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_GND      = 8,
	IVI_DTC_ID_INDEX_RL_LOUDSPEAKER_SHORT_TO_BAT      = 9,
	IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_GND      = 10,
	IVI_DTC_ID_INDEX_RR_LOUDSPEAKER_SHORT_TO_BAT      = 11,
	IVI_DTC_ID_INDEX_GPS_ANTENNA_SHORT_TO_GND         = 12,
	IVI_DTC_ID_INDEX_GPS_ANTENNA_OPEN                 = 13,
	IVI_DTC_ID_INDEX_AIR_CONDITIONER_COM_FAILURE      = 14,
	IVI_DTC_ID_INDEX_PKI_CERTIFICATE_ERROR      	  = 15,
	IVI_DTC_ID_INDEX_MAX_NUM                          = 16,

}tIVIInnerDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL

	IVI_DTC_ID_CAN_COMMUNICATION_SYSTEM               = 0Xd00001,
	IVI_DTC_ID_CAN_CONTROLLER_ERR                     = 0Xd01001,
	IVI_DTC_ID_ECU_SOC                                = 0X935049,
	IVI_DTC_ID_ECU_GYRO                               = 0X934549,
	IVI_DTC_ID_FL_LOUDSPEAKER_SHORT_TO_GND            = 0X930F11,
	IVI_DTC_ID_FL_LOUDSPEAKER_SHORT_TO_BAT            = 0X930F12,
	IVI_DTC_ID_FR_LOUDSPEAKER_SHORT_TO_GND            = 0X930D11,
	IVI_DTC_ID_FR_LOUDSPEAKER_SHORT_TO_BAT            = 0X930D12,
	IVI_DTC_ID_RL_LOUDSPEAKER_SHORT_TO_GND            = 0X931111,
	IVI_DTC_ID_RL_LOUDSPEAKER_SHORT_TO_BAT            = 0X931112,
	IVI_DTC_ID_RR_LOUDSPEAKER_SHORT_TO_GND            = 0X930B11,
	IVI_DTC_ID_RR_LOUDSPEAKER_SHORT_TO_BAT            = 0X930B12,
	IVI_DTC_ID_GPS_ANTENNA_SHORT_TO_GND               = 0X934611,
	IVI_DTC_ID_GPS_ANTENNA_OPEN                       = 0X934613,
	IVI_DTC_ID_AIR_CONDITIONER_COM_FAILURE            = 0X937202,
	IVI_DTC_ID_PKI_CERTIFICATE_ERROR				  = 0XAE3E06,

}tIVIExtDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	IC_DTC_ID_INDEX_CAN_COMMUNICATION_SYSTEM          = 0,
	IC_DTC_ID_INDEX_CAN_CONTROLLER_ERR                = 1,
	IC_DTC_ID_INDEX_SPEEDOMETER_NO_RESP               = 2,
	IC_DTC_ID_INDEX_SPEEDOMETER_ABNORMAL              = 3,
	IC_DTC_ID_INDEX_TACHO_METER_NO_RESP               = 4,
	IC_DTC_ID_INDEX_TACHO_METER_ABNORMAL              = 5,
	IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_NO_RESP          = 6,
	IC_DTC_ID_INDEX_WATER_TEMP_GAUGE_ABNORMAL         = 7,
	IC_DTC_ID_INDEX_ROM_DATA_FAILURE                  = 8,
	IC_DTC_ID_INDEX_WDT_FAILURE                       = 9,
	IC_DTC_ID_INDEX_VOLTAGE_TOO_LOW                   = 10,
	IC_DTC_ID_INDEX_VOLTAGE_TOO_HIGH                  = 11,
	IC_DTC_ID_INDEX_FUEL_SENS_OPEN                    = 12,
	IC_DTC_ID_INDEX_FUEL_SENS_SHORT                   = 13,
	IC_DTC_ID_INDEX_OAT_SENS_SHORT                    = 14,
	IC_DTC_ID_INDEX_MAX_NUM                           = 15,

}tICInnerDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL

	IC_DTC_ID_CAN_COMMUNICATION_SYSTEM                = 0Xd00001,
	IC_DTC_ID_CAN_CONTROLLER_ERR                      = 0Xd01049,
	IC_DTC_ID_SPEEDOMETER_NO_RESP                     = 0Xa20587,
	IC_DTC_ID_SPEEDOMETER_ABNORMAL                    = 0Xa20529,
	IC_DTC_ID_TACHO_METER_NO_RESP                     = 0Xa26787,
	IC_DTC_ID_TACHO_METER_ABNORMAL                    = 0Xa26729,
	IC_DTC_ID_WATER_TEMP_GAUGE_NO_RESP                = 0Xa26887,
	IC_DTC_ID_WATER_TEMP_GAUGE_ABNORMAL               = 0Xa26829,
	IC_DTC_ID_ROM_DATA_FAILURE                        = 0Xa32345,
	IC_DTC_ID_WDT_FAILURE                             = 0Xa32347,
	IC_DTC_ID_VOLTAGE_TOO_LOW                         = 0Xa33116,
	IC_DTC_ID_VOLTAGE_TOO_HIGH                        = 0Xa33117,
	IC_DTC_ID_FUEL_SENS_OPEN                          = 0X940213,
	IC_DTC_ID_FUEL_SENS_SHORT                         = 0X940211,
	IC_DTC_ID_OAT_SENS_SHORT                          = 0Xa21011,

}tICExtDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	AVM_DTC_ID_INDEX_CAN_COMMUNICATION_SYSTEM          			= 0,
	AVM_DTC_ID_INDEX_CAN_CONTROLLER_ERR                			= 1,
	AVM_DTC_ID_INDEX_POWER_INPUT_ERROR               			= 2,
	AVM_DTC_ID_INDEX_REAR_CAMERA_VIDEO_INPUT              		= 3,
	AVM_DTC_ID_INDEX_RIGHT_CAMERA_VIDEO_INPUT               	= 4,
	AVM_DTC_ID_INDEX_FRONT_CAMERA_VIDEO_INPUT              		= 5,
	AVM_DTC_ID_INDEX_LEFT_CAMERA_VIDEO_INPUT          			= 6,
	AVM_DTC_ID_INDEX_NEUTRAL_POINT_REGISTRATION_CAL_DISABALE	= 7,
	AVM_DTC_ID_INDEX_AVM_CALIB_IMPLEMENT_JUDGMENT				= 8,
	AVM_DTC_ID_INDEX_STRG_FAIL		  							= 9,
	AVM_DTC_ID_INDEX_MAX_NUM									= 10,

}tAVMInnerDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	AVM_DTC_ID_CAN_COMMUNICATION_SYSTEM					= 0xd00001,
	AVM_DTC_ID_CAN_CONTROLLER_ERR 						= 0xd01049,
	AVM_DTC_ID_POWER_INPUT_ERROR						= 0xf0031c,
	AVM_DTC_ID_REAR_CAMERA_VIDEO_INPUT					= 0xd11a96,
	AVM_DTC_ID_RIGHT_CAMERA_VIDEO_INPUT					= 0xd11b96,
	AVM_DTC_ID_FRONT_CAMERA_VIDEO_INPUT					= 0xd11c96,
	AVM_DTC_ID_LEFT_CAMERA_VIDEO_INPUT					= 0xd11d96,
	AVM_DTC_ID_NEUTRAL_POINT_REGISTRATION_CAL_DISABALE	= 0xd23254,
	AVM_DTC_ID_AVM_CALIB_IMPLEMENT_JUDGMENT				= 0xd30454,
	AVM_DTC_ID_STRG_FAIL								= 0x5a3941,

}tAVMExtDTCId;

typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	HUD_DTC_ID_INDEX_CAN_COMMUNICATION_ERROR					= 0,
	HUD_DTC_ID_INDEX_CAN_CONTROLLER_ERR							= 1,
	HUD_DTC_ID_INDEX_VOLTAGE_BELOW_THRESHOLD_ERROR				= 2,
	HUD_DTC_ID_INDEX_VOLTAGE_ABOVE_THRESHOLD_ERROR				= 3,
	HUD_DTC_ID_INDEX_NOT_CALIBRATION_YET						= 4,
	HUD_DTC_ID_INDEX_CALIBRATION_COMMUNICATION_FAILURE			= 5,
	HUD_DTC_ID_INDEX_CALIBRATION_DATA_RECORDING_FAILURE			= 6,
	HUD_DTC_ID_INDEX_CURRENT_BELOW_THRESHOLD_ERROR				= 7,
	HUD_DTC_ID_INDEX_CURRENT_ABOVE_THRESHOLD_ERROR 				= 8,
	HUD_DTC_ID_INDEX_EEPROM_FAILURE_ERROR						= 9,
	HUD_DTC_ID_INDEX_WDT_REASON_ERROR							= 10,
	HUD_DTC_ID_INDEX_CAN_CONTROLLER_ERROR						= 11,
	HUD_DTC_ID_INDEX_SOFTWARE_REASON_ERROR						= 12,
	HUD_DTC_ID_INDEX_SW_ON_STUCK								= 13,
	HUD_DTC_ID_INDEX_MAX_NUM									= 14,

}tHUDInnerDTCId;


typedef enum{
	// CAUTION:DON'T CHANGE THE ORDER BY MANUAL
	HUD_DTC_ID_CAN_COMMUNICATION_ERROR					= 0xD00001,
	HUD_DTC_ID_CAN_CONTROLLER_ERR						= 0xD01049,
	HUD_DTC_ID_VOLTAGE_BELOW_THRESHOLD_ERROR			= 0xD00316,
	HUD_DTC_ID_VOLTAGE_ABOVE_THRESHOLD_ERROR			= 0xD00317,
	HUD_DTC_ID_NOT_CALIBRATION_YET						= 0xD33554,
	HUD_DTC_ID_CALIBRATION_COMMUNICATION_FAILURE		= 0xD33608,
	HUD_DTC_ID_CALIBRATION_DATA_RECORDING_FAILURE		= 0xD33656,
	HUD_DTC_ID_CURRENT_BELOW_THRESHOLD_ERROR			= 0xA2A018,
	HUD_DTC_ID_CURRENT_ABOVE_THRESHOLD_ERROR 			= 0xA2A019,
	HUD_DTC_ID_EEPROM_FAILURE_ERROR						= 0xA2A046,
	HUD_DTC_ID_WDT_REASON_ERROR							= 0xA2A047,
	HUD_DTC_ID_CAN_CONTROLLER_ERROR						= 0xA2A049,
	HUD_DTC_ID_SOFTWARE_REASON_ERROR					= 0xA2A053,
	HUD_DTC_ID_SW_ON_STUCK								= 0xA2A123,

}tHUDExtDTCId;

extern NVM_SEC_VAR_UNSPECIFIED uint8 gIVI_PKI_CERTIFICATE[1] ;						 //define in dtcCore_cfg.c  line 166
extern tDtcSnapshot_AVM gRuntimeSnapshot_AVM[AVM_DTC_ID_INDEX_MAX_NUM];				 //define in dtcCore_cfg.c  line 338  
extern tDtcSnapshot_HUD gRuntimeSnapshot_HUD[HUD_DTC_ID_INDEX_MAX_NUM];				 //define in dtcCore_cfg.c  line 441
extern tDtcSnapshot_IC gRuntimeSnapshot_IC[IC_DTC_ID_INDEX_MAX_NUM] ;				 //define in dtcCore_cfg.c  line 232 
extern tDtcSnapshot_IVI gRuntimeSnapshot_IVI[IVI_DTC_ID_INDEX_MAX_NUM]; 			 //define in dtcCore_cfg.c  line 105
extern const tDtcCoreCfgInfor gDtcCoreCfgInfor;										 //define in dtcCore_cfg.c  line 22	
extern tDtcStatusAndExtInfor *gDtcStatusAndExtInforArray[];							 //define in dtcCore_cfg.c  line 32	
extern NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_AVM[];  	 //define in dtcCore_cfg.c  line 351	
extern NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IC[];	 //define in dtcCore_cfg.c  line 250	
extern NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IVI[];	 //define in dtcCore_cfg.c  line 124	
extern uint16 gDtcTotalNum[];														 //define in dtcCore_cfg.c  line 550
extern  tDtcStatusAndExtInfor gDtcStatusAndExtInfor_HUD[];	
#endif																		
		
