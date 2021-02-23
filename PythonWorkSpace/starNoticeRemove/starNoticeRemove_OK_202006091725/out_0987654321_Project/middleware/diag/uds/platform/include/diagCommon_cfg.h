/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DIAG_COMMON_CFG_H
#define _DIAG_COMMON_CFG_H

#include"diagCommon_if.h"

typedef enum{
	//Don't change the order!!!!!
	//If you need to add a member, please add it to the back.
	DIAG_DEV_ID_IVI = 0,
	DIAG_DEV_ID_IC  = 1,
	DIAG_DEV_ID_AVM = 2,
	DIAG_DEV_ID_HUD = 3,
	//add member
	DIAG_DEV_NUM
}tDiagDevId;


#define UDS_IVI 0
#define UDS_IC	1
#define UDS_AVM	2
#define UDS_HUD	3

#define UDS_CH_MAX_NUM 4	//IVI, IC,AVM,HUD

#define DIAG_HIGH_VOLTAGE_THRESHOLD_VALUE 1600 // 16V
#define DIAG_HIGH_VOLTAGE_FILTER_THRESHOLD_VALUE 1550 // 15.5V

#define DIAG_LOW_VOLTAGE_THRESHOLD_VALUE 900 // 9V
#define DIAG_LOW_VOLTAGE_FILTER_THRESHOLD_VALUE 850 // 8.5V

#define DIAG_STATIC_VEHICLE_SPEED_VALUE 5 // 5KM

#define DIAG_ENGINE_IS_RUNNING_THRESHOLD_VALUE 500 // 500r/min
#define DIAG_ENGINE_IS_RUNNING_FILTER_THRESHOLD_VALUE 400 // 400r/min

extern uint16 diagReveCpyData(uint8* dst, uint8* src, uint16 srcLen);

#endif
