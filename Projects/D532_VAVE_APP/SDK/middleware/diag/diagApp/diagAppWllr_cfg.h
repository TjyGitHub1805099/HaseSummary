#ifndef _DIAGAPP_WLLR_CFG_H
#define _DIAGAPP_WLLR_CFG_H


#include "Std_Types.h"

#define WLLR_AGING_COUNTER_LIMIT  40
#define WLLR_INIT_VALUE  0xFF
#define MILEAGE_INIT_VALUE  0xFFFFFFFF



typedef enum{
	//DON'T CHANGE THE ORDER

	INNER_WL_ID_ABS                            = 0,
	INNER_WL_ID_VDC_TCS_OFF                    = 1,
	INNER_WL_ID_SLIP_IND                       = 2,
	INNER_WL_ID_BRAKE                          = 3,
	INNER_WL_ID_DOOR                           = 4,
	INNER_WL_ID_TRUCK_HATCHBACK                = 5,
	INNER_WL_ID_OIL                            = 6,
	INNER_WL_ID_C_ENG                          = 7,
	INNER_WL_ID_C_ENG2                         = 8,
	INNER_WL_ID_CRUISE_IND                     = 9,
	INNER_WL_ID_SET_IND                        = 10,
	INNER_WL_ID_CRUISE                         = 11,
	INNER_WL_ID_BA                             = 12,
	INNER_WL_ID_O_D_OFF                        = 13,
	INNER_WL_ID_ATC_T_AMT                      = 14,
	INNER_WL_ID_AT_OIL_TMP                     = 15,
	INNER_WL_ID_CVT_IND                        = 16,
	INNER_WL_ID_SPORT_IND                      = 17,
	INNER_WL_ID_4WD                            = 18,
	INNER_WL_ID_FUEL                           = 19,
	INNER_WL_ID_WASHER                         = 20,
	INNER_WL_ID_TIRE_PRESSURE                  = 21,
	INNER_WL_ID_KEY_GREEN_YELLOW               = 22,
	INNER_WL_ID_KEY_RED                        = 23,
	INNER_WL_ID_KEY_KNOB                       = 24,
	INNER_WL_ID_EPS                            = 25,
	INNER_WL_ID_E_4WD                          = 26,
	INNER_WL_ID_AFS_OFF_IND                    = 27,
	INNER_WL_ID_4WAS_RAS                       = 28,
	INNER_WL_ID_HDC                            = 29,
	INNER_WL_ID_SYS_FAIL                       = 30,
	INNER_WL_ID_SFT_POSI                       = 31,
	INNER_WL_ID_HV_BAT                         = 32,
	INNER_WL_ID_HEV_BRAKE                      = 33,
	INNER_WL_ID_SFT_OPERATION                  = 34,
	INNER_WL_ID_L_K_MAS                        = 35,
	INNER_WL_ID_CHARGE                         = 36,
	INNER_WL_ID_CHARGE_IND                     = 37,
	INNER_WL_ID_OIL_LEV                        = 38,
	INNER_WL_ID_DPF                            = 39,
	INNER_WL_ID_TRAILER                        = 40,
	INNER_WL_ID_RUN_FLAT                       = 41,
	INNER_WL_ID_E_SUS                          = 42,
	INNER_WL_ID_ROUNCH_CNT                     = 43,
	INNER_WL_ID_SOW_O_IND                      = 44,
	INNER_WL_ID_FILTER                         = 45,
	INNER_WL_ID_BRAKE_PAD                      = 46,
	INNER_WL_ID_SCR                            = 47,
	INNER_WL_ID_E_PKB_IND                      = 48,
	INNER_WL_ID_E_PKB                          = 49,
	INNER_WL_ID_LED_HEADLAMP                   = 50,
	INNER_WL_ID_ISS_IND                        = 51,
	INNER_WL_ID_FUEL_CAP                       = 52,
	INNER_WL_ID_ENG_REMOTE_IND                 = 53,
	INNER_WL_ID_BATTERY_REMAINING              = 54,
	INNER_WL_ID_READY_IND                      = 55,
	INNER_WL_ID_SLOW_IND                       = 56,
	INNER_WL_ID_CAUTION                        = 57,
	INNER_WL_ID_BATTERY_BRAKEDOWN              = 58,
	
	INNER_WL_ID_MAX
	
}tInnerWlId;


typedef enum{
	WL_OFF,
	WL_ON,
}tWlStatus;


typedef struct{
	//uint8 wl_id;
	uint32 mileage;
	uint8 ignCounter;
}tWlInfor;


typedef struct{
	//uint8 wl_id;
	uint8 wlStatus:1;
	uint8 recordedFlag:1;
}tWlStatusRecord;




#endif

