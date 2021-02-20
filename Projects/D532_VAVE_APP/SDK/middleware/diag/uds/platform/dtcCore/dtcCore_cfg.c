/********************************************************************
Author: Li Qi
Version: V1.0
Timestamp: 2019-01-23 21:34:03
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
#include "dtcCore_cfg.h"
#include "MemMap.h"
#include "NvM_Cfg.h"

#define ENABLE_DTC_DATA
#define DTC_AGING_COUNTER_LIMIT 39

const tDtcCoreCfgInfor gDtcCoreCfgInfor = {
	.logLevel = DTC_LOG_LEVEL_1,
	.udsChMaxNum = UDS_CH_MAX_NUM,
	.bitStatusMask = 0x9,	//bit 3 and bit 0
	.agingCounterLimitValue = DTC_AGING_COUNTER_LIMIT,
	.agingCounterInitValue = 0xFF,
	.enable1901 = FALSE,
	.enable1902 = TRUE,
	.enable1904 = TRUE,
	.enable1906 = FALSE,
	.enable190a = FALSE,
};

tDiagMutex gDtcMutex1[UDS_CH_MAX_NUM] = {};


#ifdef UDS_IVI
const tDtcMixInfor gDtcMixInfor_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {

	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },

};


const uint8 gNvmBlockIdListForRuntimeSnapshot_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {

	gRuntimeSnapshot_IVI_0_NvMBlockID,
	gRuntimeSnapshot_IVI_1_NvMBlockID,
	gRuntimeSnapshot_IVI_2_NvMBlockID,
	gRuntimeSnapshot_IVI_3_NvMBlockID,
	gRuntimeSnapshot_IVI_4_NvMBlockID,
	gRuntimeSnapshot_IVI_5_NvMBlockID,
	gRuntimeSnapshot_IVI_6_NvMBlockID,
	gRuntimeSnapshot_IVI_7_NvMBlockID,
	gRuntimeSnapshot_IVI_8_NvMBlockID,
	gRuntimeSnapshot_IVI_9_NvMBlockID,
	gRuntimeSnapshot_IVI_10_NvMBlockID,
	gRuntimeSnapshot_IVI_11_NvMBlockID,
	gRuntimeSnapshot_IVI_12_NvMBlockID,
	gRuntimeSnapshot_IVI_13_NvMBlockID,
	gRuntimeSnapshot_IVI_14_NvMBlockID,
	gRuntimeSnapshot_IVI_15_NvMBlockID,

	};


const uint8 gNvmBlockIdListForDtcExtInfor_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {

	gDtcStatusAndExtInfor_IVI_0_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_1_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_2_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_3_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_4_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_5_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_6_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_7_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_8_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_9_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_10_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_11_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_12_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_13_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_14_NvMBlockID,
	gDtcStatusAndExtInfor_IVI_15_NvMBlockID,

	};


NVM_SEC_VAR_UNSPECIFIED tDtcSnapshot_IVI gRuntimeSnapshot_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},
	{0,0,0,0,},

};

NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},

};


tDtcVmInfor gDtcVmInfor_IVI[IVI_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{IVI_DTC_ID_CAN_COMMUNICATION_SYSTEM,              TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*1*/{IVI_DTC_ID_CAN_CONTROLLER_ERR,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*2*/{IVI_DTC_ID_ECU_SOC,                               TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*3*/{IVI_DTC_ID_ECU_GYRO,                              TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*4*/{IVI_DTC_ID_FL_LOUDSPEAKER_SHORT_TO_GND,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*5*/{IVI_DTC_ID_FL_LOUDSPEAKER_SHORT_TO_BAT,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*6*/{IVI_DTC_ID_FR_LOUDSPEAKER_SHORT_TO_GND,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*7*/{IVI_DTC_ID_FR_LOUDSPEAKER_SHORT_TO_BAT,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*8*/{IVI_DTC_ID_RL_LOUDSPEAKER_SHORT_TO_GND,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*9*/{IVI_DTC_ID_RL_LOUDSPEAKER_SHORT_TO_BAT,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*10*/{IVI_DTC_ID_RR_LOUDSPEAKER_SHORT_TO_GND,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*11*/{IVI_DTC_ID_RR_LOUDSPEAKER_SHORT_TO_BAT,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*12*/{IVI_DTC_ID_GPS_ANTENNA_SHORT_TO_GND,              TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*13*/{IVI_DTC_ID_GPS_ANTENNA_OPEN,                      TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*14*/{IVI_DTC_ID_AIR_CONDITIONER_COM_FAILURE,           TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*15*/{IVI_DTC_ID_PKI_CERTIFICATE_ERROR,			 	 TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},

};


NVM_SEC_VAR_UNSPECIFIED uint8 gIVI_PKI_CERTIFICATE[1] ={0}; //default 0

#endif


#ifdef UDS_IC
const tDtcMixInfor gDtcMixInfor_IC[IC_DTC_ID_INDEX_MAX_NUM] = {

	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
};


const uint8 gNvmBlockIdListForRuntimeSnapshot_IC[IC_DTC_ID_INDEX_MAX_NUM] = {

	gRuntimeSnapshot_IC_0_NvMBlockID,
	gRuntimeSnapshot_IC_1_NvMBlockID,
	gRuntimeSnapshot_IC_2_NvMBlockID,
	gRuntimeSnapshot_IC_3_NvMBlockID,
	gRuntimeSnapshot_IC_4_NvMBlockID,
	gRuntimeSnapshot_IC_5_NvMBlockID,
	gRuntimeSnapshot_IC_6_NvMBlockID,
	gRuntimeSnapshot_IC_7_NvMBlockID,
	gRuntimeSnapshot_IC_8_NvMBlockID,
	gRuntimeSnapshot_IC_9_NvMBlockID,
	gRuntimeSnapshot_IC_10_NvMBlockID,
	gRuntimeSnapshot_IC_11_NvMBlockID,
	gRuntimeSnapshot_IC_12_NvMBlockID,
	gRuntimeSnapshot_IC_13_NvMBlockID,		
	gRuntimeSnapshot_IC_14_NvMBlockID,
	};


const uint8 gNvmBlockIdListForDtcExtInfor_IC[IC_DTC_ID_INDEX_MAX_NUM] = {

	gDtcStatusAndExtInfor_IC_0_NvMBlockID,
	gDtcStatusAndExtInfor_IC_1_NvMBlockID,
	gDtcStatusAndExtInfor_IC_2_NvMBlockID,
	gDtcStatusAndExtInfor_IC_3_NvMBlockID,
	gDtcStatusAndExtInfor_IC_4_NvMBlockID,
	gDtcStatusAndExtInfor_IC_5_NvMBlockID,
	gDtcStatusAndExtInfor_IC_6_NvMBlockID,
	gDtcStatusAndExtInfor_IC_7_NvMBlockID,
	gDtcStatusAndExtInfor_IC_8_NvMBlockID,
	gDtcStatusAndExtInfor_IC_9_NvMBlockID,
	gDtcStatusAndExtInfor_IC_10_NvMBlockID,
	gDtcStatusAndExtInfor_IC_11_NvMBlockID,
	gDtcStatusAndExtInfor_IC_12_NvMBlockID,
	gDtcStatusAndExtInfor_IC_13_NvMBlockID,
	gDtcStatusAndExtInfor_IC_14_NvMBlockID,
	};


NVM_SEC_VAR_UNSPECIFIED tDtcSnapshot_IC gRuntimeSnapshot_IC[IC_DTC_ID_INDEX_MAX_NUM] = {
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
	{0,0,0,0,0,0,0,},
};

NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_IC[IC_DTC_ID_INDEX_MAX_NUM] = {
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
};


tDtcVmInfor gDtcVmInfor_IC[IC_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{IC_DTC_ID_CAN_COMMUNICATION_SYSTEM,               TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*1*/{IC_DTC_ID_CAN_CONTROLLER_ERR,                     TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*2*/{IC_DTC_ID_SPEEDOMETER_NO_RESP,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*3*/{IC_DTC_ID_SPEEDOMETER_ABNORMAL,                   TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*4*/{IC_DTC_ID_TACHO_METER_NO_RESP,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*5*/{IC_DTC_ID_TACHO_METER_ABNORMAL,                   TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*6*/{IC_DTC_ID_WATER_TEMP_GAUGE_NO_RESP,               TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*7*/{IC_DTC_ID_WATER_TEMP_GAUGE_ABNORMAL,              TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,60000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*8*/{IC_DTC_ID_ROM_DATA_FAILURE,                       TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*9*/{IC_DTC_ID_WDT_FAILURE,                            TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*10*/{IC_DTC_ID_VOLTAGE_TOO_LOW,                        TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*11*/{IC_DTC_ID_VOLTAGE_TOO_HIGH,                       TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*12*/{IC_DTC_ID_FUEL_SENS_OPEN,                         TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*13*/{IC_DTC_ID_FUEL_SENS_SHORT,                        TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*14*/{IC_DTC_ID_OAT_SENS_SHORT,                         TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
};


#endif


#ifdef UDS_AVM

const tDtcMixInfor gDtcMixInfor_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {

	/*0*/{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*1*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*2*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	/*3*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*4*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*5*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*6*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*7*/{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*8*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*9*/{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },

};


const uint8 gNvmBlockIdListForRuntimeSnapshot_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/gRuntimeSnapshot_AVM_0_NvMBlockID,
	/*1*/gRuntimeSnapshot_AVM_1_NvMBlockID,
	/*2*/gRuntimeSnapshot_AVM_2_NvMBlockID,
	/*3*/gRuntimeSnapshot_AVM_3_NvMBlockID,
	/*4*/gRuntimeSnapshot_AVM_4_NvMBlockID,
	/*5*/gRuntimeSnapshot_AVM_5_NvMBlockID,
	/*6*/gRuntimeSnapshot_AVM_6_NvMBlockID,
	/*7*/gRuntimeSnapshot_AVM_7_NvMBlockID,
	/*8*/gRuntimeSnapshot_AVM_8_NvMBlockID,
	/*9*/gRuntimeSnapshot_AVM_9_NvMBlockID,

	};


const uint8 gNvmBlockIdListForDtcExtInfor_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/gDtcStatusAndExtInfor_AVM_0_NvMBlockID,
	/*1*/gDtcStatusAndExtInfor_AVM_1_NvMBlockID,
	/*2*/gDtcStatusAndExtInfor_AVM_2_NvMBlockID,
	/*3*/gDtcStatusAndExtInfor_AVM_3_NvMBlockID,
	/*4*/gDtcStatusAndExtInfor_AVM_4_NvMBlockID,
	/*5*/gDtcStatusAndExtInfor_AVM_5_NvMBlockID,
	/*6*/gDtcStatusAndExtInfor_AVM_6_NvMBlockID,
	/*7*/gDtcStatusAndExtInfor_AVM_7_NvMBlockID,
	/*8*/gDtcStatusAndExtInfor_AVM_8_NvMBlockID,
	/*9*/gDtcStatusAndExtInfor_AVM_9_NvMBlockID,
};


NVM_SEC_VAR_UNSPECIFIED tDtcSnapshot_AVM gRuntimeSnapshot_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{0,0,0,},
	/*1*/{0,0,0,},
	/*2*/{0,0,0,},
	/*3*/{0,0,0,},
	/*4*/{0,0,0,},
	/*5*/{0,0,0,},
	/*6*/{0,0,0,},
	/*7*/{0,0,0,},
	/*8*/{0,0,0,},
	/*9*/{0,0,0,},

};

NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*1*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*2*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*3*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*4*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*5*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*6*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*7*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*8*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*9*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},

};


tDtcVmInfor gDtcVmInfor_AVM[AVM_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{AVM_DTC_ID_CAN_COMMUNICATION_SYSTEM,               TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*1*/{AVM_DTC_ID_CAN_CONTROLLER_ERR,                     TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*2*/{AVM_DTC_ID_POWER_INPUT_ERROR,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*3*/{AVM_DTC_ID_REAR_CAMERA_VIDEO_INPUT,                   TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*4*/{AVM_DTC_ID_RIGHT_CAMERA_VIDEO_INPUT,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*5*/{AVM_DTC_ID_FRONT_CAMERA_VIDEO_INPUT,                   TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*6*/{AVM_DTC_ID_LEFT_CAMERA_VIDEO_INPUT,               TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*7*/{AVM_DTC_ID_NEUTRAL_POINT_REGISTRATION_CAL_DISABALE,              TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*8*/{AVM_DTC_ID_AVM_CALIB_IMPLEMENT_JUDGMENT,                       TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*9*/{AVM_DTC_ID_STRG_FAIL,                            TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
};

	
#endif

#ifdef UDS_HUD

const tDtcMixInfor gDtcMixInfor_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {

	/*0*/{DTC_OPERATION_CLCLIC_BY_IGN_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*1*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*2*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	/*3*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	/*4*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*5*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*6*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*7*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	/*8*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NONE },
	/*9*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*10*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*11*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*12*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },
	/*13*/{DTC_OPERATION_CLCLIC_BY_POWER_ON,TRUE,TRUE,DTC_GROUP_NONE },

};


const uint8 gNvmBlockIdListForRuntimeSnapshot_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/gRuntimeSnapshot_HUD_0_NvMBlockID,
	/*1*/gRuntimeSnapshot_HUD_1_NvMBlockID,
	/*2*/gRuntimeSnapshot_HUD_2_NvMBlockID,
	/*3*/gRuntimeSnapshot_HUD_3_NvMBlockID,
	/*4*/gRuntimeSnapshot_HUD_4_NvMBlockID,
	/*5*/gRuntimeSnapshot_HUD_5_NvMBlockID,
	/*6*/gRuntimeSnapshot_HUD_6_NvMBlockID,
	/*7*/gRuntimeSnapshot_HUD_7_NvMBlockID,
	/*8*/gRuntimeSnapshot_HUD_8_NvMBlockID,
	/*9*/gRuntimeSnapshot_HUD_9_NvMBlockID,
	/*10*/gRuntimeSnapshot_HUD_10_NvMBlockID,
	/*11*/gRuntimeSnapshot_HUD_11_NvMBlockID,
	/*12*/gRuntimeSnapshot_HUD_12_NvMBlockID,
	/*13*/gRuntimeSnapshot_HUD_13_NvMBlockID,
	
	};


const uint8 gNvmBlockIdListForDtcExtInfor_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/gDtcStatusAndExtInfor_HUD_0_NvMBlockID,
	/*1*/gDtcStatusAndExtInfor_HUD_1_NvMBlockID,
	/*2*/gDtcStatusAndExtInfor_HUD_2_NvMBlockID,
	/*3*/gDtcStatusAndExtInfor_HUD_3_NvMBlockID,
	/*4*/gDtcStatusAndExtInfor_HUD_4_NvMBlockID,
	/*5*/gDtcStatusAndExtInfor_HUD_5_NvMBlockID,
	/*6*/gDtcStatusAndExtInfor_HUD_6_NvMBlockID,
	/*7*/gDtcStatusAndExtInfor_HUD_7_NvMBlockID,
	/*8*/gDtcStatusAndExtInfor_HUD_8_NvMBlockID,
	/*9*/gDtcStatusAndExtInfor_HUD_9_NvMBlockID,
	/*10*/gDtcStatusAndExtInfor_HUD_10_NvMBlockID,
	/*11*/gDtcStatusAndExtInfor_HUD_11_NvMBlockID,
	/*12*/gDtcStatusAndExtInfor_HUD_12_NvMBlockID,
	/*13*/gDtcStatusAndExtInfor_HUD_13_NvMBlockID,
};


NVM_SEC_VAR_UNSPECIFIED tDtcSnapshot_HUD gRuntimeSnapshot_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{0,0,0,},
	/*1*/{0,0,0,},
	/*2*/{0,0,0,},
	/*3*/{0,0,0,},
	/*4*/{0,0,0,},
	/*5*/{0,0,0,},
	/*6*/{0,0,0,},
	/*7*/{0,0,0,},
	/*8*/{0,0,0,},
	/*9*/{0,0,0,},
	/*10*/{0,0,0,},
	/*11*/{0,0,0,},
	/*12*/{0,0,0,},
	/*13*/{0,0,0,},

};

NVM_SEC_VAR_UNSPECIFIED tDtcStatusAndExtInfor gDtcStatusAndExtInfor_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*1*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*2*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*3*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*4*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*5*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*6*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*7*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*8*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*9*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*10*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*11*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*12*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	/*13*/{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},

};


tDtcVmInfor gDtcVmInfor_HUD[HUD_DTC_ID_INDEX_MAX_NUM] = {
	/*0*/{HUD_DTC_ID_CAN_COMMUNICATION_ERROR,               	TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1700,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*1*/{HUD_DTC_ID_CAN_CONTROLLER_ERR,                     	TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*2*/{HUD_DTC_ID_VOLTAGE_BELOW_THRESHOLD_ERROR,             TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*3*/{HUD_DTC_ID_VOLTAGE_ABOVE_THRESHOLD_ERROR,             TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*4*/{HUD_DTC_ID_NOT_CALIBRATION_YET,                    	TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*5*/{HUD_DTC_ID_CALIBRATION_COMMUNICATION_FAILURE,         TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*6*/{HUD_DTC_ID_CALIBRATION_DATA_RECORDING_FAILURE,        TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*7*/{HUD_DTC_ID_CURRENT_BELOW_THRESHOLD_ERROR,             TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*8*/{HUD_DTC_ID_CURRENT_ABOVE_THRESHOLD_ERROR,             TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*9*/{HUD_DTC_ID_EEPROM_FAILURE_ERROR,                      TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*10*/{HUD_DTC_ID_WDT_REASON_ERROR,                         TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*11*/{HUD_DTC_ID_CAN_CONTROLLER_ERROR,                     TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*12*/{HUD_DTC_ID_SOFTWARE_REASON_ERROR,                    TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	/*13*/{HUD_DTC_ID_SW_ON_STUCK,                            	TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
					
};

	
#endif



//===========================BELOW IS PUBLIC PARA======================
const tDtcMixInfor *gDtcMixInforArray[UDS_CH_MAX_NUM] = {
	gDtcMixInfor_IVI,
	gDtcMixInfor_IC,
	gDtcMixInfor_AVM,
	gDtcMixInfor_HUD,
};


const uint8 *gNvmBlockIdListForRuntimeSnapshotArray[UDS_CH_MAX_NUM] = {
	(uint8*)&gNvmBlockIdListForRuntimeSnapshot_IVI[0],
	(uint8*)&gNvmBlockIdListForRuntimeSnapshot_IC[0],
	(uint8*)&gNvmBlockIdListForRuntimeSnapshot_AVM[0],
	(uint8*)&gNvmBlockIdListForRuntimeSnapshot_HUD[0],
};


const uint8 *gNvmBlockIdListForDtcExtInforArray[UDS_CH_MAX_NUM] = {
	(uint8*)&gNvmBlockIdListForDtcExtInfor_IVI[0],
	(uint8*)&gNvmBlockIdListForDtcExtInfor_IC[0],
	(uint8*)&gNvmBlockIdListForDtcExtInfor_AVM[0],
	(uint8*)&gNvmBlockIdListForDtcExtInfor_HUD[0],
	
};


uint8 *gRuntimeSnapshotArray[UDS_CH_MAX_NUM] = {
	(uint8*)&gRuntimeSnapshot_IVI[0],
	(uint8*)&gRuntimeSnapshot_IC[0],
	(uint8*)&gRuntimeSnapshot_AVM[0],
	(uint8*)&gRuntimeSnapshot_HUD[0],
};


tDtcStatusAndExtInfor *gDtcStatusAndExtInforArray[UDS_CH_MAX_NUM] = {
	gDtcStatusAndExtInfor_IVI,
	gDtcStatusAndExtInfor_IC,
	gDtcStatusAndExtInfor_AVM,
	gDtcStatusAndExtInfor_HUD,
};


tDtcVmInfor *gDtcVmInforArray[UDS_CH_MAX_NUM] = {
	gDtcVmInfor_IVI,
	gDtcVmInfor_IC,
	gDtcVmInfor_AVM,
	gDtcVmInfor_HUD,
};


uint16 gDtcTotalNum[UDS_CH_MAX_NUM] = {0,0,0,0};

uint8 gDtcSnapshotTypeLen[UDS_CH_MAX_NUM] = {0,0,0,0};


void dtcCorePartInit()
{

	gDtcTotalNum[0] = sizeof(gDtcVmInfor_IVI)/sizeof(tDtcVmInfor);// ivi
	gDtcSnapshotTypeLen[0] = sizeof(tDtcSnapshot_IVI);

	gDtcTotalNum[1] = sizeof(gDtcVmInfor_IC)/sizeof(tDtcVmInfor);//ic
	gDtcSnapshotTypeLen[1] = sizeof(tDtcSnapshot_IC);

	gDtcTotalNum[2] = sizeof(gDtcVmInfor_AVM)/sizeof(tDtcVmInfor);//AVM
	gDtcSnapshotTypeLen[2] = sizeof(tDtcSnapshot_AVM);

	gDtcTotalNum[3] = sizeof(gDtcVmInfor_HUD)/sizeof(tDtcVmInfor);//HUD
	gDtcSnapshotTypeLen[3] = sizeof(tDtcSnapshot_HUD);

	//1TODO: cpy localSnapshot if supported from nvm to ram
}

