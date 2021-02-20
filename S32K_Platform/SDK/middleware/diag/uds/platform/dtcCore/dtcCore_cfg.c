/**************************
description: 
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/
#include "dtcCore_cfg.h"

#define ENABLE_DTC_DATA

const tDtcCoreCfgInfor gDtcCoreCfgInfor = {
	.logLevel = DTC_LOG_LEVEL_0,
	.totalDtcNumStoredInNvm = DTC_20_PERCENT_STORED_NUM,
};

const tDtcMixInfor gDtcMixInfor[] = {
#ifdef ENABLE_DTC_DATA	
		{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_BODY}, //INNER_DTC_ID_VOLTAGE_TOO_LOW
		{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_BODY}, /*INNER_DTC_ID_VOLTAGE_TOO_HIGH*/
		{DTC_OPERATION_CLCLIC_BY_POWER_ON,FALSE,TRUE,DTC_GROUP_NETWORK}, /*INNER_DTC_ID_CAN_BUS_OFF*/

		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_NETWORK}, /*INNER_DTC_ID_LOST_COMMUNICATION_WITH_WCT*/
		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_NETWORK}, /*INNER_DTC_ID_LOST_COMMUNICATION_WITH_BZS*/
		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_NETWORK}, /*INNER_DTC_ID_LOST_COMMUNICATION_WITH_IPC*/
		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_NETWORK}, /*INNER_DTC_ID_LOST_COMMUNICATION_WITH_TPMS*/
	
		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_BODY}, /*INNER_DTC_ID_SYSTEM_BOOT_ERROR*/
		{DTC_OPERATION_CLCLIC_BY_IGN_ON,FALSE,TRUE,DTC_GROUP_BODY}, /*INNER_DTC_ID_MP5_CALIBRATION_FILE_ERROR*/
#endif
};


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM

#ifdef DIAG_OS_UCOS
#pragma segment NVM_RAM_START
#endif

tDtcStoredInfor gDtcStoredInfor[DTC_20_PERCENT_STORED_NUM] = {
{/*innerDtcId*/DTC_NVM_SPACE_IS_EMPTY, /*dtcStatusBit*/0,  /*extInfor*/0,0,0,0,/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
{DTC_NVM_SPACE_IS_EMPTY,		0,	0,0,0,0,	0,0,0,0,0,0,		0,0,0,		0,		0,},
{DTC_NVM_SPACE_IS_EMPTY,		0,	0,0,0,0,	0,0,0,0,0,0,		0,0,0,		0,		0,},
{DTC_NVM_SPACE_IS_EMPTY,		0,	0,0,0,0,	0,0,0,0,0,0,		0,0,0,		0,		0,},
};


#ifdef DIAG_OS_UCOS
#pragma segment NVM_RAM_END
#endif


//1TODO:the nvmBlockId should be gotten from NVM module
const uint8 gNvmBlockIdListForDtcStoredInfor[DTC_20_PERCENT_STORED_NUM] = {0,0,0,0};

tDtcSnapshot gRuntimeSnapshotList[] = {
#ifdef ENABLE_DTC_DATA	

	/*DTC_ID_VOLTAGE_TOO_LOW*/	{/*time:0xf1,0x20,*/1,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_VOLTAGE_TOO_HIGH*/ {/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},

	/*DTC_ID_CAN_BUS_OFF*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	
	/*DTC_ID_LOST_COMMUNICATION_WITH_WCT*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_BZS*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_IPC*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_TPMS*/ {/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_SYSTEM_BOOT_ERROR*/			{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_MP5_CALIBRATION_FILE_ERROR*/ 	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
#endif
};


tDtcStatusAndExtInfor gDtcStatusAndExtInforArray[] = {
#ifdef ENABLE_DTC_DATA

	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},/*DTC_ID_VOLTAGE_TOO_LOW*/
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},/*DTC_ID_VOLTAGE_TOO_HIGH*/
	{FALSE,DTC_DEFAULT_STATUS,{2,0,0,0}},/*DTC_ID_CAN_BUS_OFF*/

	{FALSE,DTC_DEFAULT_STATUS,{3,0,0,0}},/*DTC_ID_LOST_COMMUNICATION_WITH_WCT*/
	{FALSE,DTC_DEFAULT_STATUS,{3,0,0,0}},/*DTC_ID_LOST_COMMUNICATION_WITH_BZS*/
	{FALSE,DTC_DEFAULT_STATUS,{3,0,0,0}},/*DTC_ID_LOST_COMMUNICATION_WITH_IPC*/
	{FALSE,DTC_DEFAULT_STATUS,{3,0,0,0}},/*DTC_ID_LOST_COMMUNICATION_WITH_TPMS*/
	{FALSE,DTC_DEFAULT_STATUS,{1,0,0,0}},/*DTC_ID_SYSTEM_BOOT_ERROR*/
	{FALSE,DTC_DEFAULT_STATUS,{7,0,0,0}},/*DTC_ID_MP5_CALIBRATION_FILE_ERROR*/
#endif
};



tDtcVmInfor gDtcVmInforList[] = {
#ifdef ENABLE_DTC_DATA

	{EXT_DTC_ID_VOLTAGE_TOO_LOW, TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,500,500,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	{EXT_DTC_ID_VOLTAGE_TOO_HIGH,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,500,500,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	{EXT_DTC_ID_CAN_BUS_OFF,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	//follow DTCs:5 times of msg cyclic has been detected by COM,here no need to detect 5 times again
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_WCT,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_BZS, TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_IPC,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_TPMS,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	{EXT_DTC_ID_SYSTEM_BOOT_ERROR,TRUE,TRUE,FALSE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_MP5_CALIBRATION_FILE_ERROR,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
#endif
};


uint16 gDtcTotalNum = 0;
uint16 gAvailableDtcSpaceNumInNvm = DTC_20_PERCENT_STORED_NUM;


void dtcCorePartInit()
{
	uint8 i = 0;
	uint8 index = 0;

	gDtcTotalNum = sizeof(gDtcVmInforList)/sizeof(tDtcVmInfor);

	//1TODO:cpy global snapshot  from nvm to ram
	for(i=0;i<DTC_20_PERCENT_STORED_NUM;i++)
	{
		if(gDtcStoredInfor[i].innerDtcId != DTC_NVM_SPACE_IS_EMPTY)
		{	
			index = gDtcStoredInfor[i].innerDtcId;
			diagCpyData((uint8*)&gDtcStatusAndExtInforArray[index],(const uint8*)&gDtcStoredInfor[i].dtcStatusAndExtInfor,sizeof(tDtcStatusAndExtInfor));
			diagCpyData((uint8*)&gRuntimeSnapshotList[index],(const uint8*)&gDtcStoredInfor[i].snapshot,sizeof(tDtcSnapshot));
			gDtcStatusAndExtInforArray[index].storedInNvmFlag = TRUE;
			gAvailableDtcSpaceNumInNvm--;
		}
	}

	//1TODO: cpy localSnapshot from nvm to ram
}


#else // STORE ALL DTC TO NVM




//1TODO:the nvmBlockId should be gotten from NVM module
const uint8 gNvmBlockIdListForRuntimeSnapshot[9] = {0}; // map to gRuntimeSnapshotList,the arraylength is equal to the actual total DTC num
const uint8 gNvmBlockIdListForDtcExtInfor[9] = {0}; // map to gDtcStatusAndExtInforArray,the arraylength is equal to the actual total DTC num

#ifdef DIAG_OS_UCOS
#pragma segment NVM_RAM_START
#endif

tDtcSnapshot gRuntimeSnapshotList[] = {
#ifdef ENABLE_DTC_DATA	

	/*DTC_ID_VOLTAGE_TOO_LOW*/	{/*time:0xf1,0x20,*/1,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_VOLTAGE_TOO_HIGH*/ {/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	
	/*DTC_ID_CAN_BUS_OFF*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},

	/*DTC_ID_LOST_COMMUNICATION_WITH_WCT*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_BZS*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_IPC*/	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_LOST_COMMUNICATION_WITH_TPMS*/ {/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_SYSTEM_BOOT_ERROR*/			{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
	/*DTC_ID_MP5_CALIBRATION_FILE_ERROR*/ 	{/*time:0xf1,0x20,*/0,0,0,0,0,0,/*odometer:0xf1,0x21,*/0,0,0,/*supplyVoltage:0xf1,0x22,*/0,/*ignitionStatus:0xf1,0x23,*/0,},
#endif
};


tDtcStatusAndExtInfor gDtcStatusAndExtInforArray[] = {
#ifdef ENABLE_DTC_DATA

	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},

	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
	{FALSE,DTC_DEFAULT_STATUS,{0,0,0,0}},
#endif
};

#ifdef DIAG_OS_UCOS
#pragma segment NVM_RAM_END
#endif

tDtcVmInfor gDtcVmInforList[] = {
#ifdef ENABLE_DTC_DATA

	{EXT_DTC_ID_VOLTAGE_TOO_LOW, TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,500,500,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT},
	{EXT_DTC_ID_VOLTAGE_TOO_HIGH,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,500,500,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	{EXT_DTC_ID_CAN_BUS_OFF,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	//follow DTCs:5 times of msg cyclic has been detected by COM,here no need to detect 5 times again
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_WCT,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_BZS, TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,2000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_IPC,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,500,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_LOST_COMMUNICATION_WITH_TPMS,TRUE,TRUE,TRUE,TRUE,DTC_RESET_BY_INIT,0,1000,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },

	{EXT_DTC_ID_SYSTEM_BOOT_ERROR,TRUE,TRUE,FALSE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
	{EXT_DTC_ID_MP5_CALIBRATION_FILE_ERROR,TRUE,TRUE,TRUE,FALSE,DTC_RESET_BY_INIT,0,0,0,{DTC_TIMER_NONE,FALSE,0},DTC_PRE_INIT },
#endif
};


uint16 gDtcTotalNum = 0;

void dtcCorePartInit()
{

	gDtcTotalNum = sizeof(gDtcVmInforList)/sizeof(tDtcVmInfor);


	//1TODO: cpy localSnapshot if supported from nvm to ram
}

#endif
