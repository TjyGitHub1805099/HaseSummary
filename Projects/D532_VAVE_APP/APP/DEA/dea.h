/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : DataPool.h
**          Author: xiaobo
**
**          Date  : 2017-05-24
**          
**          
******************************************************************************/
#ifndef __DEA_H_
#define __DEA_H_

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "Std_Types.h"
#include "stdio.h"
#include "Com_If.h"
#include "Task_IpcApp_Cfg.h"
#include "ExternRTC.h"


//#include "CanService_If.h"


/******************************************************************************
**    MACROS
******************************************************************************/
#define DEA_CALL_CHAR_BUF_SIZE	(200)
#define DEA_MEDIA_CHAR_BUF_SIZE	(200)
#define RESET_MAGIC_VALUE		(0x55AAAA55)
#define IPC_MAGIC_VALUE			(0x12344321)
/******************************************************************************
**    BACKRAM FIX ADDRESS VAR
******************************************************************************/
#define BACKRAM_END 		*((uint32_t*)(0x2001E7FC))	//0x2001E800-4
#define BACKRAM_LANGUAGE 	*((uint8_t*)(0x2001E7FB))	//0x2001E800-5	value: 0-english not 0:chinese
#define BACKRAM_MAGIC2 		*((uint32_t*)(0x2001E7F4))	//0x2001E800-12
#define BACKRAM_IPC_FLAG 	*((uint32_t*)(0x2001E7F0))	//0x2001E800-16



/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/
typedef struct
{
	
	uint16_t value_one;
	uint16_t value_two;
	uint16_t value_three;
	uint16_t value_four;
	uint16_t value_five;
	uint16_t value_six;
		
}HIS_AFC;




typedef enum
{
	RST_IRRST = 0,	/* Irregular reset */
	RST_ERST,		/* External reset */
	RST_WDG1,		/* Watchdog Reset 1 */
	RST_WDG0,		/* Watchdog Reset 0 */
	RST_SCRT,		/* Flash security violation reset */
	RST_SRST,		/* Software reset */
	RST_UNKNOW		/* Unknow reset */
}RST_TYPE_ENUM;

typedef enum
{
	IGN_OFF = 0,
	IGN_ON
}IGN_STAT_ENUM;

typedef enum
{
	DEA_ACC_OFF = 0,
	DEA_ACC_ON
}ACC_STAT_ENUM;


typedef enum
{
	ENGINE_START_OFF = 0,
	ENGINE_START_ON
}ENGINE_START_STAT_ENUM;

typedef enum
{
	CC_OFF = 0,
	CC_STANDBY,
	CC_ACTIVE_RESUME_FROM_ABOVE,
	CC_ACTIVE_RESUME_FROM_BELOW,
	CC_ACTIVE_TRIP_DOWN,
	CC_ACTIVE_TRIP_UP,
	CC_ACTIVE_DECELERATION,
	CC_ACTIVE_ACCELERATION,
	CC_ACTIVE_CRUISE,
	CC_OVERRIDE,
	CC_RESERVED,
	CC_INVALID,
}CRUISE_CONTROL_ENUM;


typedef struct
{
	uint8_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
}TIME_STRU;

typedef struct
{
	uint16_t Year;
	uint8_t Month;
	uint8_t Day;
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
}MHU_TIME_STRU;


typedef enum
{
	TIME_24H = 0,
	TIME_12H
}TIME_FORMAT_ENUM;

typedef struct
{
	uint8_t DIMMING_MHU_DARK_ON;
	uint8_t DIMMING_MHU_DARK_OFF;
}DIMMING_MHU_STRU;

typedef struct
{
	uint8_t DIMMING_OFFSET_IPC_DARK_ON;
	uint8_t DIMMING_OFFSET_IPC_DARK_OFF;
}DIMMING_OFFSET_IPC_STRU;

typedef struct
{
	uint8_t IC_DIMMING_DARK_OFF;
	uint8_t IC_DIMMING_DARK_ON;
	uint8_t IC_BRIGHTNESS_OFFSET_DARK_OFF;
	uint8_t IC_BRIGHTNESS_OFFSET_DARK_ON;
	uint8_t DIMMING_VEHICLE_BRIGHTNESS;
}IC_DIMMING_STRU;

typedef enum
{
	POSITION_LIGHT_OFF = 0,
	POSITION_LIGHT_ON
}STAT_POS_LIGHT_ENUM;

typedef enum
{
	BUTTON_NONE = 0,
	BUTTON_DOWN,
	BUTTON_OK,
	BUTTON_UP,
	BUTTON_BACK,
}BOARD_BUTTON_ENUM;

typedef enum
{
	BUTTON_RELEASE,
	BUTTON_SHORT_PRESSED,
	BUTTON_LONG_PRESSED,
	BUTTON_RESERVED,
}BUTTON_STAT_ENUM;

typedef enum
{
	LANGUAGE_CHINESE = 0,
	LANGUAGE_ENGLISH
}LANGUAGE_ENUM;


typedef struct
{
#if 0
	uint8_t TempWarnFL_Valid	:1;	/* 0-Invalid 1-Valid*/
	uint8_t TempWarnFR_Valid	:1;	
	uint8_t TempWarnRL_Valid	:1;
	uint8_t TempWarnRR_Valid	:1;
	uint8_t TireTempFL_Valid	:1;
	uint8_t TireTempFR_Valid	:1;
	uint8_t TireTempRL_Valid	:1;
	uint8_t TireTempRR_Valid	:1;

	uint8_t PosWLampFL_Valid		:1;
	uint8_t PosWLampFR_Valid		:1;
	uint8_t PosWLampRL_Valid		:1;
	uint8_t PosWLampRR_Valid		:1;
	uint8_t PressureTyreFL_Valid	:1;	
	uint8_t PressureTyreFR_Valid	:1;
	uint8_t PressureTyreRL_Valid	:1;
	uint8_t PressureTyreRR_Valid	:1;
#endif
	
	uint8_t TempWarnFL				:2;
	uint8_t TempWarnFR				:2;
	uint8_t TempWarnRL				:2;
	uint8_t TempWarnRR				:2;
	uint8_t PosWLampFL				:4;
	uint8_t PosWLampFR				:4;
	uint8_t PosWLampRL				:4;
	uint8_t PosWLampRR				:4;
	uint8_t TireTempFL;
	uint8_t TireTempFR;
	uint8_t TireTempRL;
	uint8_t TireTempRR;
	uint8_t PressureTyreFL;
	uint8_t PressureTyreFR;
	uint8_t PressureTyreRL;
	uint8_t PressureTyreRR;
}TIRE_STAT_STRU;


typedef enum
{
	 GEAR_NONE = 0,
	 GEAR_P,
	 GEAR_R,
	 GEAR_N,
	 GEAR_D1,
	 GEAR_D2,
	 GEAR_D3,
	 GEAR_D4,
	 GEAR_D5,
	 GEAR_D6,
	 GEAR_D7,
	 GEAR_M1,
	 GEAR_M2,
	 GEAR_M3,
	 GEAR_M4,
	 GEAR_M5,
	 GEAR_M6,
	 GEAR_M7
}GEAR_ENUM;

typedef enum
{
	GearShift_None = 0,
	GearShiftUp,
	GearShiftDown
}GEAR_SHIFT_ENUM;

typedef enum
{
	TURN_OFF,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_LEFT_RIGHT,
	TRUN_INVALID,
}TURN_IND_STAT_ENUM;

typedef enum
{
	DEA_BUTTON_UP,
	DEA_BUTTON_DOWN,
	DEA_BUTTON_OK,
	DEA_BUTTON_BACK,
	DEA_BUTTON_NUM,
}DEA_BUTTON_ENUM;


typedef struct
{
	uint16_t EBAStateValid			:1; /* 0-Invalid 1-Valid*/
	uint16_t EBAState				:3; // 7 "Signal invalid" 6 "Reserved" 5 "Reserved" 4 "Error " 3 "Intervention activ" 2 "OFF Temporary error " 1 "OK; Stand by" 0 "Init" ;
	uint16_t FCW_RequestValid		:1; /* 0-Invalid 1-Valid*/
	uint16_t FCW_Request			:2; // 3 "Signal invalid" 2 "Reserved" 1 "Warning requested" 0 "No warning requested" ;
	uint16_t RadarFaultInfoValid	:1; /* 0-Invalid 1-Valid*/
	uint16_t RadarFaultInfo			:2; // 3 "Radar sensor functions temporarily not available " 2 "Radar sensor functions not available, please visit repair shop "
	uint16_t STAT_EBAActivationValid:1; /* 0-Invalid 1-Valid*/
	uint16_t STAT_EBAActivation		:2; // 3 "Signal invalid" 2 "Reserved" 1 "ON" 0 "OFF" ;
	uint16_t STAT_FCWValid			:1; /* 0-Invalid 1-Valid*/
	uint16_t STAT_FCW				:3;	// 7 "Signal invalid" 6 "Reserved" 5 "Reserved" 4 "Error" 3 "Intervention activ" 2 "OFF Temporary not available" 1 "OK; Stand by" 0 "Init" ;
	uint16_t STAT_FCWActivationValid:1; /* 0-Invalid 1-Valid*/
	uint16_t STAT_FCWActivation		:2;	// 3 "Signal invalid" 2 "Reserved" 1 "ON" 0 "OFF" ;
	uint16_t Reserved				:12;
}RADAR_STRU;

typedef struct
{
	uint16_t LDWSwitchStatusValid		:1;	/* 0-Invalid 1-Valid*/
	uint16_t LDWSwitchStatus			:4;	// 3 "Signal invalid" 2 "Reserved" 1 "ON" 0 "OFF" ;
	uint16_t LDWLeftDepartureWarnValid	:1;	/* 0-Invalid 1-Valid*/
	uint16_t LDWLeftDepartureWarn		:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Warning" 0 "No warning" ;
	uint16_t LDWSystemStatusValid		:1;	/* 0-Invalid 1-Valid*/
	uint16_t LDWSystemStatus			:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Fault" 0 "Normal" ;
	uint16_t LDWStatusSignalValid		:1; /* 0-Invalid 1-Valid*/
	uint16_t LDWStatusSignal			:2;	// 3 "Signal invalid" 2 "On (function on , and reach active speed)" 1 "Ready (function on, but doesn't reach active speed)" 0 "OFF" ;
	uint16_t Reserved					:1;
	uint16_t LDWRightDepartureWarnValid	:1;	/* 0-Invalid 1-Valid*/
	uint16_t LDWRightDepartureWarn		:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Warning" 0 "No warning" ;
	uint16_t Reserved1					:13;
}LDWS_STRU;

typedef struct
{
	uint8_t DoorTrunkAjarValid		:1;	/* 0-Invalid 1-Valid*/
	uint8_t DoorTrunkAjar			:1;	/* 0-Close 1-Open*/
	uint8_t DoorAjarFLValid			:1;
	uint8_t DoorAjarFL				:1;
	uint8_t DoorAjarFRValid			:1;
	uint8_t DoorAjarFR				:1;
	uint8_t DoorAjarRLValid			:1;
	uint8_t DoorAjarRL				:1;
	uint8_t DoorAjarRRValid			:1;
	uint8_t DoorAjarRR				:1;
	uint8_t BonnetAjarValid			:1;
	uint8_t BonnetAjar				:1;
	uint8_t Resserved				:4;
}DOOR_STAT_STRU;


typedef enum
{
	SEAT_MEMORY_OFF = 0,
	SEAT_MEMORY_REQUEST,
	SEAT_MEMORY_DONE,
	SEAT_MEMORY_INVALID,
}SEAT_MEMORY_ENUM;

typedef struct
{
	uint8_t ETGRemind_CentrolLockStateValid	:2;	/* 0-Invalid 1-Valid*/
	uint8_t ETGRemind_CentrolLockState		:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Disallow" 0 "Allow" ;
	uint8_t ETGRemind_ETGStateValid			:2;	/* 0-Invalid 1-Valid*/
	uint8_t ETGRemind_ETGState				:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Disallow" 0 "Allow" ;
	uint8_t ETGRemind_ShiftStateValid		:2; /* 0-Invalid 1-Valid*/
	uint8_t ETGRemind_ShiftState			:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Disallow" 0 "Allow" ;
	uint8_t ETGRemind_VehicleStateValid		:2; /* 0-Invalid 1-Valid*/
	uint8_t ETGRemind_VehicleState			:2;	// 3 "Signal invalid" 2 "Reserved" 1 "Disallow" 0 "Allow" ;
}ETG_STATUS_STRU;

typedef enum
{
	PDC_Disable = 0,
	PDC_IndividualTone_1Hz,
	PDC_IndividualTone_2Hz,
	PDC_IndividualTone_4Hz,
	PDC_ContinousTone,
	PDC_SelfTestTone,
	PDC_INVALID,
}PDC_ENUM;

typedef struct
{
	uint8_t WLC_W_FODValid		:1;		/* 0-Invalid 1-Valid*/
	uint8_t WLC_W_FOD			:3; 	// 7 "Signal invalid" 6 "Reserved" 5 "Reserved" 4 "Reserved" 3 "FOD warning 3rd" 2 "FOD warning 2nd" 1 "FOD warning 1st" 0 "No obstruction detected" ;
	uint8_t Reserved			:4;
	uint8_t	WLC_St_SysInfoValid	:1;
	uint8_t WLC_St_SysInfo		:4; 	/*15 "Signal invalid" 14 "Reserved" 13 "Reserved" 12 "Reserved" 11 "Reserved" 10 "Reserved" 9 "Reserved" 
									   8 "Reserved" 7 "Repeat charge finish" 6 "Repeat charge after full" 5 "charge pause because of SESAM" 4 "Temperature protection" 
									   3 "Cell phone token away" 2 "Charge finished" 1 "Charge start" 0 "No information" */
	uint8_t Reserved1			:3;
}WLC_STRU;

typedef struct
{
	uint8_t WarnMsgId;
	uint8_t BitStatus	:2;
	uint8_t Reserved	:4;
	uint8_t BitBlinking	:2;
}WARN_MSG_STRU;

typedef struct
{
	uint8_t DISP_RQ_GR_GRB;
	uint8_t ALIV_DISP_DATA_PWTR;
	uint8_t DISP_PO_GRB;
	uint8_t DISP_PRG_GRB;
}TCU_DISP_DATA_PWTR_RAW_STRU;


typedef struct
{
	uint8_t TotalMessagesLC		:4;
	uint8_t MessageNumber_LC	:4;
	uint8_t MHU_IPC_Amount_LC	:4;
	uint8_t MHU_IPC_ID_LC		:4;
	uint8_t MHU_IPC_ID_LC_LAST;
	uint8_t dataLen;
	uint8_t data[DEA_CALL_CHAR_BUF_SIZE];
}DEA_MHU_IPC_LASTCALLS_STRU;

typedef struct
{
	uint8_t TotalMessage;
	uint8_t MessageNumber;
	uint8_t Type;
	uint8_t dataLen;
	uint8_t data[DEA_MEDIA_CHAR_BUF_SIZE];
}DEA_MHU_IPC_MEDIA_STRU;

typedef struct
{
	uint8_t TELStatus;
	uint8_t MUTEState;
}DEA_MHU_IPC_TEL_STRU;

typedef enum
{
	DEA_PhoneNotPaired = 0,
	DEA_PhonePaired,
	DEA_PhoneNotPairedAndCallBtnPressed,
	DEA_PhonePairedAndCallBtnPressed,
	DEA_PhoneReserved,
}DEA_MHU_IPC_PAIR_STATUS_ENUM;

typedef struct
{
	uint8_t Hour;
	uint8_t Min;
	uint8_t Sec;
}DEA_CALLING_TIME_STRU;

typedef enum
{
	TEL_Accept = 0,
	TEL_Decline,
	TEL_Calling,
	TEL_MuteON,
	TEL_MuteOFF,
	TEL_CancelOrEnd,
	TEL_PairingRequest,
	TEL_Invalid,
}DEA_IC_PHONE_STATE_ENUM;

typedef struct
{
	DEA_IC_PHONE_STATE_ENUM Telphone;
	uint8_t CalId;
}DEA_IC_MHU_TEL_STRU;

typedef struct
{
	uint8_t Route;
	uint8_t Arrow;
	uint16_t Distance;
	uint8_t LvdsReady;
	uint8_t LvdsSigRecvFlag;
}DEA_NAVI_STRU;

typedef struct
{
	uint8_t NaviStatus;
	uint8_t Arrow;
	uint16_t Distance;
}DEA_NAVI_FOR_IPC_STRU;

typedef struct
{
	uint8_t CompassState;
	uint16_t CompassData;
}DEA_COMPASS_FOR_IPC_STRU;

typedef enum
{
	DEA_FUEL_SIG_NORMAL,
	DEA_FUEL_SIG_SHORT,	//sensor short
	DEA_FUEL_SIG_OPEN,	//sensor open
	DEA_FUEL_SIG_SHORT_TO_BAT,	//short to bat
	DEA_FUEL_SIG_SHORT_TO_GND,	//short to ground
}DEA_FUEL_SIG_ENUM;

typedef enum
{
	DEA_OAT_SIG_NORMAL,
	DEA_OAT_SIG_SHORT,	//sensor short
	DEA_OAT_SIG_OPEN,	//sensor open
	DEA_OAT_SIG_SHORT_TO_BAT,	//short to bat
	DEA_OAT_SIG_SHORT_TO_GND,	//short to ground
}DEA_OAT_SIG_ENUM;

typedef enum
{
	DEA_LVDS_NORMAL,
	DEA_LVDS_ERROR,
}DEA_LVDS_STAT_ENUM;


	

/******************************************************************************
**    FUNCTION DEFINITIONS
******************************************************************************/
#if 0
/**591 project require**/

//stering A
void DEA_SetSRCBottonValue(uint8_t value);
uint8_t DEA_GetSRCBottonValue();

void DEA_SetMENU_UPBottonValue(uint8_t value);
uint8_t DEA_GetMENU_UPBottonValue();

void DEA_SetMENU_DOWNBottonValue(uint8_t value);
uint8_t DEA_GetMENU_DOWNBottonValue();

void DEA_SetTEL_ONBottonValue(uint8_t value);
uint8_t DEA_GetTEL_ONBottonValue();

void DEA_SetSW_OKBottonValue(uint8_t value);
uint8_t DEA_GetSW_OKBottonValue();

//stering B

void DEA_SetVOL_DOWNBottonValue(uint8_t value);
uint8_t DEA_GetVOL_DOWNBottonValue();

void DEA_SetVOL_UPBottonValue(uint8_t value);
uint8_t DEA_GetVOL_UPBottonValue();

void DEA_SetCAMERABottonValue(uint8_t value);
uint8_t DEA_GetCAMERABottonValue();

void DEA_SetMODEBottonValue(uint8_t value);
uint8_t DEA_GetMODEBottonValue();

void DEA_SetDISPBottonValue(uint8_t value);
uint8_t DEA_GetDISPBottonValue();
#else

void DEA_SetKeyStrgUpValue(uint8_t value);
uint8_t DEA_GetKeyStrgUpValue(void);

void DEA_SetKeyStrgDownValue(uint8_t value);
uint8_t DEA_GetKeyStrgDownValue(void);

void DEA_SetKeyStrgLeftValue(uint8_t value);
uint8_t DEA_GetKeyStrgLeftValue(void);

void DEA_SetKeyStrgRightValue(uint8_t value);
uint8_t DEA_GetKeyStrgRightValue(void);

void DEA_SetKeyStrgOkValue(uint8_t value);
uint8_t DEA_GetKeyStrgOkValue(void);

void DEA_SetKeyStrgDispValue(uint8_t value);
uint8_t DEA_GetKeyStrgDispValue(void);

void DEA_SetKeyStrgTelOnValue(uint8_t value);
uint8_t DEA_GetKeyStrgTelOnValue(void);

void DEA_SetKeyStrgDvrValue(uint8_t value);
uint8_t DEA_GetKeyStrgDvrValue(void);

void DEA_SetKeyStrgLkaValue(uint8_t value);
uint8_t DEA_GetKeyStrgLkaValue(void);








#endif





//获取小计和本次里程时间
uint32_t DEA_GetTripTimeA();
void DEA_SetTripTimeA(uint32_t value);
uint8_t DEA_GetTripTimeAValid();
void DEA_SetTripTimeAValid(uint8_t value);
uint32_t DEA_GetTripTimeB();
void DEA_SetTripTimeB(uint32_t value);
uint8_t DEA_GetTripTimeBValid();
void DEA_SetTripTimeBValid(uint8_t value);

//续航里程

uint32_t DEA_GetRawReMilgeValue(void);

void DEA_SetRawgReMilgeValue(uint32_t value);

uint32_t DEA_GetFilterReMilgeValue(void);

void DEA_SetFilterReMilgeValue(uint32_t value);

void DEA_SetReMilgeErrorValueStat(uint8_t stat);

uint8_t DEA_GetReMilgeErrorValueStat(void);

void DEA_SetReMilgeValueValid(uint8_t valid);

uint8_t DEA_GetReMilgeValueValid(void);

//电流表
int16_t DEA_GetRawIpackValue(void);

void DEA_SetRawIpackValue(int16_t value);

int16_t DEA_GetFilterIpackValue(void);

void DEA_SetFilterIpackValue(int16_t value);

void DEA_SetIpackErrorValueStat(uint8_t stat);

uint8_t DEA_GetIpackErrorValueStat(void);

void DEA_SetIpackValueValid(uint8_t valid);

uint8_t DEA_GetIpackValueValid(void);

//电压表
uint16_t DEA_GetRawVpackValue(void);


void DEA_SetRawVpackValue(uint16_t value);


uint16_t DEA_GetFilterVpackValue(void);


void DEA_SetFilterVpackValue(uint16_t value);



void DEA_SetVpackErrorValueStat(uint8_t stat);


uint8_t DEA_GetVpackErrorValueStat(void);


void DEA_SetVpackValueValid(uint8_t valid);


uint8_t DEA_GetVpackValueValid(void);

//电机转速
int32_t DEA_GetRawMspValue(void);

void DEA_SetRawMspValue(int32_t value);

int32_t DEA_GetFilterMspValue(void);

void DEA_SetFilterMspValue(int32_t value);

void DEA_SetMspErrorValueStat(uint8_t stat);

uint8_t DEA_GetMspErrorValueStat(void);

void DEA_SetMspValueValid(uint8_t valid);

uint8_t DEA_GetMspValueValid(void);

//电量表接口
uint8_t DEA_GetRawSocValue(void);

void DEA_SetRawSocValue(uint8_t value);

uint8_t DEA_GetFilterBmsSocValue(void);

void DEA_SetFilterBmsSocValue(uint8_t value);

void DEA_SetSocErrorValueStat(uint8_t stat);

uint8_t DEA_GetSocErrorValueStat(void);

void DEA_SetSocValueValid(uint8_t valid);

uint8_t DEA_GetSocValueValid(void);

//功率表接口
int16_t DEA_GetRawPpackValue(void);

void DEA_SetRawPpackValue(int16_t value);

int16_t DEA_GetFilterPpackValue(void);

void DEA_SetFilterPpackValue(int16_t value);

void DEA_SetPrmErrorValueStat(uint8_t stat);

uint8_t DEA_GetPrmErrorValueStat(void);

void DEA_SetPrmValueValid(uint8_t valid);

uint8_t DEA_GetPrmValueValid(void);


int16_t DEA_GetAvpPackAValue(); /**平均功率A**/

void DEA_SetAvpPackAValue(int16_t value);

void DEA_SetAvpPackAValueValid(uint8_t valid);
uint8_t DEA_GetAvpPackAValueValid(void);

int16_t DEA_GetAvpPackBValue(); /**平均功率B**/

void DEA_SetAvpPackBValue(int16_t value);

void DEA_SetAvpPackBValueValid(uint8_t valid);
uint8_t DEA_GetAvpPackBValueValid(void);



//金康协议gtw_vcu_ready信号接口

void DEA_SetRawVcuReadyValue(uint8_t value);

uint8_t DEA_GetRawVcuReadyValue(void);

void DEA_SetRawVcuReadyErrStat(uint8_t stat);

uint8_t DEA_GetRawVcuReadyErrStat(void);

//金康协议车速信号的valid信号

void DEA_SetRawVchSpValidValue(uint8_t value);

uint8_t DEA_GetRawVchSpValidValue();

void DEA_SetVchSpValidErrStat(uint8_t value);

uint8_t DEA_GetVchSpValidErrStat();


/**档位值**/	
uint8_t DEA_GetRawGearValue(void);		//591 project require gear value
void DEA_SetRawGearValue(uint8_t value);

uint8_t DEA_GetFilterGearValue(void);
void DEA_SetFilterGearValue(uint8_t value);

void DEA_SetGearErrorValueStat(uint8_t stat);
uint8_t DEA_GetGearErrorValueStat(void);

void DEA_SetFilterGearValueValid(uint8_t valid);
uint8_t DEA_GetFilterGearValueValid(void);

void DEA_SetRawGearATCheakValue(uint8_t value);	//cheak	AT
uint8_t DEA_GetRawGearATCheakValue(void);

void DEA_SetRawGearIdicateAllow(uint8_t value);	//indicate allow
uint8_t DEA_GetRawGearIdicateAllow(void);

uint8_t DEA_GetRawGearBlinkValue(void);
void DEA_SetRawGearBlinkValue(uint8_t value);

uint8_t DEA_GetGearRawSensorValue(void);	//RawSensor value
void DEA_SetGearRawSensorValue(uint8_t value);

uint8_t DEA_GetGearDataRecive(void); //judge if ever once recived the ID:0x421
void DEA_SetGearDataRecive(uint8_t value);




/**********/
//Travel Time
void DEA_SetFilterTravelTime_H(uint8_t time);	//591 project require
uint8_t DEA_GetFilterTravelTime_H(void);

void DEA_SetFilterTravelTime_M(uint8_t time);	//591 project require
uint8_t DEA_GetFilterTravelTime_M(void);

void DEA_SetFilterTravelTime_S(uint8_t time);	//591 project require
uint8_t DEA_GetFilterTravelTime_S(void);


void DEA_SetFilterTravelTimeValid(uint8_t valid);	//591 project require: Travel time valid
uint8_t DEA_GetFilterTravelTimeValid(void);

//Accel Guide

void DEA_SetRawAccelGuide_X(uint8_t temp);	//591 project require
uint8_t DEA_GetRawAccelGuide_X(void);

void DEA_SetFilterAccelGuide_Y(uint8_t temp);	//591 project require
uint8_t DEA_GetFilterAccelGuide_Y(void);

void DEA_SetFilterAccelGuideValue(uint8_t temp);//591 project require
uint8_t DEA_GetFilterAccelGuideValue(void);

void DEA_SetFilterAccelGuideValValid(uint8_t valid);//591 project require
uint8_t DEA_GetFilterAccelGuideValValid(void);

//ECO_rank

void DEA_SetFilterEcoRankValue(uint8_t temp);	//591 project require
uint8_t DEA_GetFilterEcoRankValue(void);

void DEA_SetFilterEcoRankValid(uint8_t temp);	//591 project require
uint8_t DEA_GetSFilterEcoRankValid(void);

void DEA_SetFilterEcoConddition(uint8_t temp);//591 project require
uint8_t DEA_GetSFilterEcoCondition(void);

void DEA_SetRawEcoShiftPosition(uint8_t temp);	//591 project require
uint8_t DEA_GetRawEcoShiftPosition(void);

void DEA_SetRawEcoMode(uint8_t temp);	 //591 project require
uint8_t DEA_GetRawEcoMode(void);

void DEA_SetVehicleSpeedOutPut(uint16_t temp);
uint16_t DEA_GetVehicleSpeedOutPut(void);


//best  History AFC
void DEA_SetFilterBestHistoryAfcValue(uint16_t temp);	//591 project require: Best History afc 
uint16_t DEA_GetFilterBestHistoryAfcValue(void);
	
//History afc show valid
void DEA_SetFilterHistoryAfcShowValid(uint8_t temp);	//591 project require: History afc show valid
uint8_t DEA_GetFilterHistoryAfcShowValid(void);

//History afc
void DEA_SetFilterHistoryAfcValue(HIS_AFC temp);	//591 project require: History afc
HIS_AFC DEA_GetFilterHistoryAfcValue(void);
uint16_t DEA_GetLastAfcValue(void);


//Idle Stop Status
void DEA_SetRawIdleStopStatusValue(uint8_t temp);	//591 project require: Idle Stop Status
uint8_t DEA_GetRawIdleStopStatusValue(void);

//Idle passing CumulativeValue
void DEA_SetFilterCumulativeValue_H(uint8_t temp);	//591 project require: Idle passing CumulativeValue
uint8_t DEA_GetFilterCumulativeValue_H(void);

void DEA_SetFilterCumulativeValue_M(uint8_t temp);	//591 project require: Idle passing CumulativeValue
uint8_t DEA_GetFilterCumulativeValue_M(void);

void DEA_SetFilterCumulativeValue_S(uint8_t temp);	//591 project require: Idle passing CumulativeValue
uint8_t DEA_GetFilterCumulativeValue_S(void);


//Idle passing lTripValue
void DEA_SetFilterlTripValue_H(uint8_t temp);	//591 project require: Idle passing lTripValue
uint8_t DEA_GetFilterlTripValue_H(void);

void DEA_SetFilterlTripValue_M(uint8_t temp);	//591 project require: Idle passing lTripValue
uint8_t DEA_GetFilterlTripValue_M(void);

void DEA_SetFilterlTripValue_S(uint8_t temp);	//591 project require: Idle passing lTripValue
uint8_t DEA_GetFilterlTripValue_S(void);

//idle fuelvalue
void DEA_SetFilterfuelCumulativeValue(uint32_t temp);	//591 project require: Idle fuel lTripValue
uint32_t DEA_GetFilterfuelCumulativeValue(void);

void DEA_SetFilterfuelLtripValue(uint32_t temp);	//591 project require: Idle fuel lTripValue
uint32_t DEA_GetFilterfuelLtripValue(void);


void DEA_SetResetMagicWord(uint32_t value);
uint32_t DEA_GetResetMagicWord(void);


void DEA_SetSysRstType(RST_TYPE_ENUM stat);
RST_TYPE_ENUM DEA_GetSysRstType(void);


void DEA_SetIgnStat(IGN_STAT_ENUM stat);
IGN_STAT_ENUM DEA_GetIgnStat(void);

void DEA_SetAccStat(ACC_STAT_ENUM stat);
ACC_STAT_ENUM DEA_GetAccStat(void);

void DEA_SetTerminalErrorStat(uint8_t stat);
uint8_t DEA_GetTerminalErrorStat(void);

void DEA_SetTerminalErrorTimerExpire(uint8_t stat);
uint8_t DEA_GetTerminalErrorTimerExpire(void);



void DEA_SetEngineStartStat(ENGINE_START_STAT_ENUM stat);
ENGINE_START_STAT_ENUM DEA_GetEngineStartStat(void);


void DEA_SetRawVehicleSpeedDiag(uint16_t speed);
uint16_t DEA_GetRawVehicleSpeedDiag(void);

void DEA_SetFilterVehicleSpeedDiag(uint16_t speed);
uint16_t DEA_GetFilterVehicleSpeedDiag(void);



void DEA_SetRawVehicleSpeedErrorStat(uint8_t stat);
uint8_t DEA_GetRawVehicleSpeedErrorStat(void);


void DEA_SetVehicleSpeedDiagMode(uint8_t mode);
uint8_t DEA_GetVehicleSpeedDiagMode(void);


void DEA_SetRawVehicleSpeed(uint16_t speed);
uint16_t DEA_GetRawVehicleSpeed(void);

void DEA_SetFilterVehicleSpeed(uint16_t speed);
uint16_t DEA_GetFilterVehicleSpeed(void);

uint16_t DEA_GetRawVehicleSpeedCanOutput(void);
void DEA_SetRawVehicleSpeedCanOutput(uint16_t speed);


void DEA_SetFilterVehicleSpeedValid(uint8_t valid);
uint8_t DEA_GetFilterVehicleSpeedValid(void);

void DEA_SetSpeedLimitAlarm(uint8_t stat);
uint8_t DEA_GetSpeedLimitAlarm(void);

void DEA_SetSpeedLimitValue(uint16_t stat);
uint16_t DEA_GetSpeedLimitValue(void);

void DEA_SetInstFuelConsmpRate(uint16_t value);
uint16_t DEA_GetInstFuelConsmpRate(void);

//Revolution 
void DEA_SetRawEngineRevolutionErrorStat(uint8_t stat);
uint8_t DEA_GetRawEngineRevolutionErrorStat(void);

void DEA_SetRawEngineRevolutionErrorStat_3EC(uint8_t stat);		//591 project require
uint8_t DEA_GetRawEngineRevolutionErrorStat_3EC(void);		//591 project require

void DEA_SetRawRevolutionIdleStat(uint8_t stat);	//591 project require
uint8_t DEA_GetRawRevolutionIdleStat(void);		//591 project require

void DEA_SetEngineRevolutionDiagMode(uint8_t mode);
uint8_t DEA_GetEngineRevolutionDiagMode(void);

void DEA_SetRawEngineRevolutionDiag(uint16_t engine);
uint16_t DEA_GetRawEngineRevolutionDiag(void);


uint8_t DEA_GetEngineRunningStat(void);
void DEA_SetEngineRunningStat(uint8_t stat);

void DEA_SetButtonStatus(DEA_BUTTON_ENUM index, BUTTON_STAT_ENUM stat);
BUTTON_STAT_ENUM DEA_GetButtonStatus(DEA_BUTTON_ENUM index);



void DEA_SetRawEngineRevolution(uint16_t engine);
uint16_t DEA_GetRawEngineRevolution(void);

void DEA_SetRawEngineRevolution_3EC(uint16_t engine);	//591 project require
uint16_t DEA_GetRawEngineRevolution_3EC(void);	//591 project require


void DEA_SetFilterEngineRevolution(uint16_t engine);
uint16_t DEA_GetFilterEngineRevolution(void);

void DEA_SetFilterEngineRevolutionValid(uint8_t valid);
uint8_t DEA_GetFilterEngineRevolutionValid(void);

uint8_t DEA_GetRawEngine3ECStat(void);		//591 project require
void DEA_SetRawEngine3ECStat(uint8_t stat);		//591 project require


//coolant
void DEA_SetCoolantTempRaw(uint16_t CoolTemp);
uint16_t DEA_GetCoolantTempRaw(void);

void DEA_SetCoolantTempRawValid(uint8_t stat);
uint8_t DEA_GetCoolantTempRawValid(void);


void DEA_SetCoolantTempValid(uint8_t valid);
uint8_t DEA_GetCoolantTempValid(void);


void DEA_SetCoolantTemp(int32_t CoolTemp);
int32_t DEA_GetCoolantTemp(void);

void DEA_SetCoolantIndAngle(uint16_t angle);	//591 project require
uint16_t DEA_GetCoolantIndAngle(void);

void DEA_SetCoolantSeg(uint8_t CoolTemp);
uint8_t DEA_GetCoolantSeg(void);

void DEA_SetIFC_Valid(uint8_t valid);
uint8_t DEA_GetIFC_Valid(void);

void DEA_SetIFC_Unit(uint8_t unit);
uint8_t DEA_GetIFC_Unit(void);	/*! 0:l/km 1:l/h */


void DEA_SetIFC_Vaule(uint16_t value);
uint16_t DEA_GetIFC_Value(void);

void DEA_SetAFC_A_Valid(uint8_t valid);
uint8_t DEA_GetAFC_A_Valid(void);


void DEA_SetAFC_A_Vaule(uint16_t value);
uint16_t DEA_GetAFC_A_Value(void);

void DEA_SetAFC_B_Valid(uint8_t valid);
uint8_t DEA_GetAFC_B_Valid(void);


void DEA_SetAFC_B_Vaule(uint16_t value);
uint16_t DEA_GetAFC_B_Value(void);


void DEA_SetCanMileage(uint16_t millage);
uint16_t DEA_GetCanMillage(void);

void DEA_SetIcMileage(uint32_t millage);
uint32_t DEA_GetIcMillage(void);

uint32_t DEA_GetOdo(void);
uint8_t DEA_GetOdoValid(void);


uint8_t DEA_GetTripA_Status(void);

uint8_t DEA_GetTripB_Status(void);


void DEA_SetBcmOdo(uint32_t odo);
uint32_t DEA_GetBcmOdo(void);

void DEA_SetBCM_ODO_CrcMatch(uint8_t stat);
uint8_t DEA_GetBCM_ODO_CrcMatch(void);



//void DEA_SetTripA(uint32_t Trip);
uint32_t DEA_GetTripA(void);


//void DEA_SetTripB(uint32_t Trip);
uint32_t DEA_GetTripB(void);

void DEA_SetOAT_RawValid(uint8_t valid);
int16_t DEA_GetOAT_RawValid(void);


void DEA_SetOAT_RawValue(int16_t value);
int16_t DEA_GetOAT_RawValue(void);

void DEA_SetOAT_RawResValid(uint8_t valid);
uint8_t DEA_GetOAT_RawResValid(void);

void DEA_SetOAT_CanRawValue(uint8_t value);
uint8_t DEA_GetOAT_CanRawValue(void);

void DEA_SetOAT_RawResValue(uint16_t value);	//0.1ohm
uint16_t DEA_GetOAT_RawResValue(void);


void DEA_SetOAT_Valid(int16_t value);
int16_t DEA_GetOAT_Valid(void);

void DEA_SetMHUTimeUpdatedInd(uint8_t stat);
uint8_t DEA_GetMHUTimeUpdatedInd(void);


void DEA_SetOAT_FilterValue(int16_t value);
int16_t DEA_GetOAT_FilterValue(void);

void DEA_SetMHUTimeValid(uint8_t flag);
uint8_t DEA_GetMHUTimeValid(void);


void DEA_SetMHUTimeValue(MHU_TIME_STRU time);
void DEA_GetMHUTimeValue(MHU_TIME_STRU *pTime);


void DEA_SetTimeValue(TIME_STRU time);
void DEA_GetTimeValue(TIME_STRU *pTime);

void DEA_SetTimeFormat(TIME_FORMAT_ENUM TimeFormat);
TIME_FORMAT_ENUM DEA_GetTimeFormat(void);

void DEA_SetMHUTimeFormat(TIME_FORMAT_ENUM TimeFormat);
TIME_FORMAT_ENUM DEA_GetMHUTimeFormat(void);


void DEA_SetDimmingSettingOkFlag(uint8_t stat);
uint8_t DEA_GetDimmingSettingOkFlag(void);

void DEA_SetDimmingFromMHURspInd(uint8_t stat);
uint8_t DEA_GetDimmingFromMHURspInd(void);

void DEA_SetDimmingMsgToMHUFlag(uint8_t stat);
uint8_t DEA_GetDimmingMsgToMHUFlag(void);



void DEA_SetDimmingVehicleMHU(DIMMING_MHU_STRU dimming);
void DEA_GetDimmingVehicleMHU(DIMMING_MHU_STRU *pDimming);

void DEA_SetDimmingOffsetIPC(DIMMING_OFFSET_IPC_STRU DimmingOffset);
void DEA_GetDimmingOffsetIPC(DIMMING_OFFSET_IPC_STRU *pDimmingOffset);


void DEA_SetPositionLightStat(STAT_POS_LIGHT_ENUM PositionStat);
STAT_POS_LIGHT_ENUM DEA_GetPositionLightStat(void);

void DEA_SetIC_Dimming(IC_DIMMING_STRU ic_dimming);
void DEA_GetIC_Dimming(IC_DIMMING_STRU *pIc_dimming);

uint8_t DEA_GetIC_DimmingDarkON(void);
uint8_t DEA_GetIC_DimmingDarkOFF(void);


void DEA_SetPhotoSensorValue(int16_t sensor_value);
int16_t DEA_GetPhotoSensorValue(void);

void DEA_SetIC_TempEx(uint16_t ic_temp);
uint16_t DEA_GetIC_TempEx(void);

void DEA_SetIC_TempExUnfilt(uint16_t ic_temp_unfilt);
uint16_t DEA_GetIC_TempExUnfilt(void);

void DEA_SetBoardButtonStatus(BOARD_BUTTON_ENUM button);
BOARD_BUTTON_ENUM DEA_GetBoardButtonStatus(void);

void DEA_SetDriveModeButton(uint8_t stat);
uint8_t DEA_GetDriveModeButton(void);
/*
void DEA_SetBordComputerButton(uint8_t stat);
uint8_t DEA_GetBordComputerButton(void);
*/

void DEA_SetPhoneButton(uint8_t stat);
uint8_t DEA_GetPhoneButton(void);


void DEA_SetFuelInjectionValid(uint16_t stat);
uint8_t DEA_GetFuelInjectionValid(void);

void DEA_SetRawFuelInjectionErrStat(uint8_t stat);	//591 project require: fuel injection stat
uint8_t DEA_GetRawFuelInjectionErrStat(void);		//591 project require: fuel injection stat

void DEA_SetFuelInjection(uint16_t fuel_injection);
uint16_t DEA_GetFuelInjection(void);

uint8_t DEA_GetTII_Valid(void);

void DEA_SetTII_Valid(uint8_t valid);

uint16_t DEA_GetTII_Value(void);

void DEA_SetTII_Value(uint16_t value);

void DEA_SetDTE_Valid(uint8_t stat);
uint8_t DEA_GetDTE_Valid(void);

void DEA_SetDTE(uint16_t dte);
uint16_t DEA_GetDTE(void);

void DEA_SetLanguage(LANGUAGE_ENUM language);
LANGUAGE_ENUM DEA_GetLanguage(void);

void DEA_SetMHULanguage(LANGUAGE_ENUM language);
LANGUAGE_ENUM DEA_GetMHULanguage(void);

void DEA_SetUnitChangedFlagForCan(uint8_t stat);
uint8_t DEA_GetUnitChangedFlagForCan(void);



void DEA_SetServiceInterval(uint16_t service_interval);
uint16_t DEA_GetServiceInterval(void);

TIRE_STAT_STRU *DEA_GetTireStatPtr(void);

void DEA_SetTireFalut(uint8_t stat);
uint8_t DEA_GetTireFault(void);


void DEA_SetDISP_GR_GRB_Sig(uint8_t stat);
uint8_t DEA_GetDISP_GR_GRB_Sig(void);

void DEA_SetDISP_RQ_GR_GRB_Sig(uint8_t stat);
uint8_t DEA_GetDISP_RQ_GR_GRB_Sig(void);

void DEA_SetDISP_PO_GRB_Sig(uint8_t stat);
uint8_t DEA_GetDISP_PO_GRB_Sig(void);

void DEA_SetDISP_PRG_GRB_Sig(uint8_t stat);
uint8_t DEA_GetDISP_PRG_GRB_Sig(void);

void DEA_SetDISP_GR_PT_Sig(uint8_t stat);
uint8_t DEA_GetDISP_GR_PT_Sig(void);

void DEA_SetDISP_GSI_DRV_Sig(uint8_t stat);
uint8_t DEA_GetDISP_GSI_DRV_Sig(void);

void DEA_SetDISP_TARG_DRV_Sig(uint8_t stat);
uint8_t DEA_GetDISP_TARG_DRV_Sig(void);

void DEA_SetGear(GEAR_ENUM gear);
GEAR_ENUM DEA_GetGear(void);


void DEA_SetTargetGear(GEAR_ENUM target_gear);
GEAR_ENUM DEA_GetTargetGear(void);

void DEA_SetGearShiftStat(GEAR_SHIFT_ENUM gear_shift);
GEAR_SHIFT_ENUM DEA_GetGearShiftStat(void);

void DEA_SetTransGearIO(uint8_t value);
uint8_t DEA_GetTransGearIO(void);

void DEA_SetTransGearValid(uint8_t value);
uint8_t DEA_GetTransGearValid(void);

void DEA_SetTransCntrlValid(uint8_t value);
uint8_t DEA_GetTransCntrlValid(void);

void DEA_SetTransCntrl(uint8_t value);
uint8_t DEA_GetTransCntrl(void);

void DEA_SetTransType(uint8_t value);
uint8_t DEA_GetTransType(void);


void DEA_SetTurnIndicator(TURN_IND_STAT_ENUM turn_indicator_stat);
TURN_IND_STAT_ENUM DEA_GetTurnIndicator(void);


void DEA_SetHighBeamStat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetHighBeamStat(void);

void DEA_SetLowBeamStat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetLowBeamStat(void);

void DEA_SetFogLampFront(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetFogLampFront(void);


void DEA_SetFogLampRear(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetFogLampRear(void);


void DEA_SetPositionLight(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetPositionLight(void);

void DEA_SetAutolight(ICON_STAT_ENUM stat);
ICON_STAT_ENUM DEA_GetAutolight(void);

void DEA_SetAutoWiping(ICON_STAT_ENUM stat);
ICON_STAT_ENUM DEA_GetAutoWiping(void);

void DEA_SetEPS_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetEPS_Stat(void);

void DEA_SetESC_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetESC_Stat(void);

void DEA_SetESC_OFF_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetESC_OFF_Stat(void);

void DEA_SetHDC_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetHDC_Stat(void);

void DEA_SetAirBag_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetAirBag_Stat(void);

void DEA_SetABS_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetABS_Stat(void);


void DEA_SetEBD_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetEBD_Stat(void);


void DEA_SetDriverSeatbelt_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetDriverSeatbelt_Stat(void);

void DEA_SetPassSeatbelt_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetPassSeatbelt_Stat(void);


void DEA_SetRadarStat(RADAR_STRU radar_value);
void DEA_GetRadarStat(RADAR_STRU *pRadar_value);

void DEA_SetLDWS_Stat(LDWS_STRU ldws);
void DEA_GetLDWS_Stat(LDWS_STRU *pLdws);

void DEA_SetLDWSOnOffStat(uint8_t stat);
uint8_t DEA_GetLDWSOnOffStat(void);


void DEA_SetDoorStat(DOOR_STAT_STRU door_stat);
void DEA_GetDoorStat(DOOR_STAT_STRU *pDoorStat);


void DEA_SetSeatMemoryStore(SEAT_MEMORY_ENUM seat);
SEAT_MEMORY_ENUM DEA_GetSeatMemoryStore(void);

void DEA_SetETG_Status(ETG_STATUS_STRU stat);
void DEA_GetETG_Status(ETG_STATUS_STRU *pStat);

void DEA_SetVehicleStopStat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetVehicleStopStat(void);

void DEA_SetESC_ASR_Stat(TELLTALE_ENUM stat);
TELLTALE_ENUM DEA_GetESC_ASR_Stat(void);


void DEA_SetBCM_PDC_Stat(PDC_ENUM stat);
PDC_ENUM DEA_GetBCM_PDC_Stat(void);

void DEA_SetPDC_Stat(PDC_ENUM stat);
PDC_ENUM DEA_GetPDC_Stat(void);

void DEA_SetBSW_Stat(uint8_t stat);
uint8_t DEA_GetBSW_Stat(void);

void DEA_SetCruiseControl_ST_CC(CRUISE_CONTROL_ENUM cruise);
CRUISE_CONTROL_ENUM DEA_GetCruiseControl_ST_CC(void);

void DEA_SetCruiseControl_TAR_V_CC(uint8_t cc);
uint8_t DEA_GetCruiseControl_TAR_V_CC(void);

int32_t DEA_GetMaintainDistance(void);
uint8_t DEA_GetDistanceValid(void);




void DEA_SetWLC_Stat(WLC_STRU stat);
void DEA_GetWLC_Stat(WLC_STRU *pStat);


void DEA_SetFuelLevelPercentValid(uint8_t value);
uint8_t DEA_GetFuelLevelPercentValid(void);

void DEA_SetFuelLevelPercent(uint16_t value);
uint16_t DEA_GetFuelLevelPercent(void);


void DEA_SetWarnMsg(WARN_MSG_STRU warn);
void DEA_GetWarnMsg(WARN_MSG_STRU *pWarnMsg);

void DEA_SetCompass(uint16_t value);
uint16_t DEA_GetCompass(void);

void DEA_SetTravelTimeValue(uint32_t time);
uint32_t DEA_GetTravelTimeValue(void);


void DEA_SetAVSA_Valid(uint8_t stat);
uint8_t DEA_GetAVSA_valid(void);

void DEA_SetAVSA(uint32_t avs);
uint32_t DEA_GetAVSA(void);

void DEA_SetAVSB_Valid(uint8_t stat);
uint8_t DEA_GetAVSB_valid(void);

void DEA_SetAVSB(uint32_t avs);
uint32_t DEA_GetAVSB(void);


void DEA_SetBCM_VinValid(uint8_t stat);
uint8_t DEA_GetBCM_VinValid(void);

void DEA_SetBCM_Vin(uint8_t *pVin);

void DEA_GetBCM_Vin(uint8_t *pVin);

void DEA_SetIPC_Vin(uint8_t *pVin);
void DEA_GetIPC_Vin(uint8_t *pVin);

void DEA_PutICWarnMsg(WARN_MSG_STRU warnMsg);
WARN_MSG_STRU DEA_GetICWarnMsg(void);
uint8_t DEA_GetICWarnMsgCnt(void);


void DEA_SetSBRSoundRequest(uint8_t stat);
uint8_t DEA_GetSBRSoundRequest(void);


void DEA_SetCanTimeOutStat(PduIdType pduId, uint8_t stat);	/* stat:1-Ture 0-False */
uint8_t DEA_GetCanTimeOutStat(PduIdType pduId);

DEA_MHU_IPC_LASTCALLS_STRU *DEA_GetLastcallPtr(void);
DEA_MHU_IPC_MEDIA_STRU *DEA_GetMediaPtr(void);


void DEA_SetPhonePairStatus(DEA_MHU_IPC_PAIR_STATUS_ENUM pairStat);
DEA_MHU_IPC_PAIR_STATUS_ENUM DEA_GetPhonePairStatus(void);

void DEA_SetTELStatus(DEA_MHU_IPC_TEL_STRU telStat);
DEA_MHU_IPC_TEL_STRU DEA_GetTELStatus(void);

void DEA_SetCallingTime(DEA_CALLING_TIME_STRU callingTime);
DEA_CALLING_TIME_STRU DEA_GetCallingTime(void);

void DEA_SetNaviChangedFlag(uint8_t stat);
uint8_t  DEA_GetNaviChangedFlag(void);

void DEA_SetNaviStatus(DEA_NAVI_STRU navi);
DEA_NAVI_STRU DEA_GetNaviStatus(void);



void DEA_SetNaviForIPCStatus(DEA_NAVI_FOR_IPC_STRU navi);
DEA_NAVI_FOR_IPC_STRU DEA_GetNaviForIPCStatus(void);

void DEA_SetCompassForIPC(DEA_COMPASS_FOR_IPC_STRU stat);
DEA_COMPASS_FOR_IPC_STRU DEA_GetCompassForIPC(void);

void DEA_SetDME_CrcMatch(uint8_t stat);
uint8_t DEA_GetDME_CrcMatch(void);

void DEA_SetEPBI_CrcMatch(uint8_t stat);
uint8_t DEA_GetEPBI_CrcMatch(void);

void  DEA_SetFuelSigStat(DEA_FUEL_SIG_ENUM stat);
DEA_FUEL_SIG_ENUM DEA_GetFuelSigStat(void);

void DEA_SetOATSigStat(DEA_OAT_SIG_ENUM stat);
DEA_OAT_SIG_ENUM DEA_GetOATSigStat(void);

void DEA_SetLVDSStat(DEA_LVDS_STAT_ENUM stat);
DEA_LVDS_STAT_ENUM DEA_GetLVDSStat(void);

void DEA_SetICTelStatusUpdatedFlag(uint8_t stat);
uint8_t DEA_GetICTelStatusUpdatedFlag(void);

void DEA_SetICTelStatus(DEA_IC_MHU_TEL_STRU stat);
DEA_IC_MHU_TEL_STRU DEA_GetICTelStatus(void);

void DEA_SetTTSecond(uint32_t tt);
uint32_t DEA_GetTTSecond(void);

void DEA_SetBmsRequiredChargingTime(uint16_t value);
uint16_t DEA_GetBmsRequiredChargingTime(void);

void DEA_SetBmsRequiredChargingTimeValid(uint8_t value);
uint8_t DEA_GetBmsRequiredChargingTimeValid(void);


uint8_t DEA_GetBmsChargingState(void);
void DEA_SetBmsChargingState(uint8_t value);

void IPC_SetUpdatedStat(uint8_t id, uint8_t stat);
uint8_t IPC_GetUpdatedStat(uint8_t id);

void DEA_SetCanReceiveIndStat(uint8_t id, uint8_t stat);
uint8_t DEA_GetCanReceiveIndStat(uint8_t id);

void DEA_SetIDSTSTS(uint8_t value);
uint8_t DEA_GetIDSTSTS(void);

void DEA_SetHardwareStatus(uint32_t value);
uint8_t DEA_GetHardwareStatus(uint32_t mask);

void DEA_SetISMSKSW(uint8_t value);
uint8_t DEA_GetISMSKSW(void);


void DEA_SetEPB_WORKMODE(uint8_t value);
uint8_t DEA_GetEPB_WORKMODE(void);


void DEA_SetEPB_STAT(uint8_t value);
uint8_t DEA_GetEPB_STAT(void);

void DEA_SetDynamicBrakingStatus(uint8_t value);
uint8_t DEA_GetDynamicBrakingStatus(void);


void DEA_SetTempOutputMode(uint8_t value);
uint8_t DEA_GetTempOutputMode(void);	//0:None 1:Toa 195ms 2:Tob 130ms 3:Toc 65ms


void DEA_SetFuelCapReset(uint8_t value);
uint8_t DEA_GetFuelCapReset(void);

/************Sig VDC_LAMP*****************/
void DEA_SetVDC_LAMP(uint8_t value);
uint8_t DEA_GetVDC_LAMP(void);

void DEA_SetCrankingStatus(uint8_t value);
uint8_t DEA_GetCrankingStatus(void);


void DEA_SetTACHO(uint16_t value);
uint16_t DEA_GetTACHO(void);

void DEA_SetRawTACHO(uint16_t value);
uint16_t DEA_GetRawTACHO(void);

void DEA_SetCanMsgtoIpcInitFlag(uint8_t value);

uint8_t DEA_GetCanMsgtoIpcInitFlag(void);

void DEA_SetFGAUGE(uint16_t value);
uint16_t DEA_GetFGAUGE(void);

void DEA_SetFMAD(uint16_t value);
uint16_t DEA_GetFMAD(void);

void DEA_SetVoltageState(uint8_t value);
uint8_t DEA_GetVoltageState(void);

void DEA_SetTPD_FR(uint8_t value);
uint8_t DEA_GetTPD_FR(void);

void DEA_SetTPD_FL(uint8_t value);
uint8_t DEA_GetTPD_FL(void);

void DEA_SetTPD_RR(uint8_t value);
uint8_t DEA_GetTPD_RR(void);

void DEA_SetTPD_RL(uint8_t value);
uint8_t DEA_GetTPD_RL(void);

void DEA_SetACCSPDREQ(uint8_t value); /*yangxl */
uint8_t DEA_GetACCSPDREQ(void);

void DEA_SetTS_FR(uint8_t value); /*yangxl */
uint8_t DEA_GetTS_FR(void);

void DEA_SetTS_FL(uint8_t value); /*yangxl */
uint8_t DEA_GetTS_FL(void);

void DEA_SetTS_RR(uint8_t value); /*yangxl */
uint8_t DEA_GetTS_RR(void);

void DEA_SetTS_RL(uint8_t value); /*yangxl */
uint8_t DEA_GetTS_RL(void);

void DEA_SetD_STYLE(uint8_t value); /*yangxl */
uint8_t DEA_GetD_STYLE(void);

void DEA_SetREQ_DISP_MET(uint8_t value); /*yangxl */
uint8_t DEA_GetREQ_DISP_MET(void);

void DEA_SetSONAR_S_REQ(uint8_t value); /*yangxl */
uint8_t DEA_GetSONAR_S_REQ(void);


void DEA_SetRR_CEN(uint8_t value); /*yangxl */
uint8_t DEA_GetRR_CEN(void);

void DEA_SetA_RR_CEN(uint8_t value); /*yangxl */
uint8_t DEA_GetA_RR_CEN(void);

void DEA_SetRR_L(uint8_t value); /*yangxl */
uint8_t DEA_GetRR_L(void);

void DEA_SetA_RR_L(uint8_t value); /*yangxl */
uint8_t DEA_GetA_RR_L(void);

void DEA_SetRR_R(uint8_t value); /*yangxl */
uint8_t DEA_GetRR_R(void);

void DEA_SetA_RR_R(uint8_t value); /*yangxl */
uint8_t DEA_GetA_RR_R(void);

void DEA_SetSTS_DISTANCE(uint8_t value); /*yangxl */
uint8_t DEA_GetSTS_DISTANCE(void);


void DEA_SetrangeStatus_FR_Center(uint8_t value);/*yangxl */
uint8_t DEA_GetrangeStatus_FR_Center(void);

void DEA_SetrangeStatus_FR_Left(uint8_t value);/*yangxl */
uint8_t DEA_GetrangeStatus_FR_Left(void);

void DEA_SetrangeStatus_FR_Right(uint8_t value);/*yangxl */
uint8_t DEA_GetrangeStatus_FR_Right(void);

void DEA_SetA_FR_Center(uint8_t value);/*yangxl */
uint8_t DEA_GetA_FR_Center(void);

void DEA_SetA_FR_CorLeft(uint8_t value);/*yangxl */
uint8_t DEA_GetA_FR_CorLeft(void);

void DEA_SetA_FR_ConrRight(uint8_t value);/*yangxl */
uint8_t DEA_GetA_FR_ConrRight(void);

void DEA_SetDetectDisplayRequest(uint8_t value); /*yangxl */
uint8_t DEA_GetDetectDisplayRequest(void);

void DEA_SetECO_DISP_RQ(uint8_t value); /*yangxl */
uint8_t DEA_GetECO_DISP_RQ(void);

void DEA_SetGSINXTGP(uint8_t value); /*yangxl */
uint8_t DEA_GetGSINXTGP(void);


void DEA_SetPEBS_INFO(uint8_t value); /*yangxl */
uint8_t DEA_GetPEBS_INFO(void);

void DEA_SetWARN_VRU(uint8_t value); /*yangxl */
uint8_t DEA_GetWARN_VRU(void);

void DEA_SetMSG_VDC(uint8_t value); /*yangxl */
uint8_t DEA_GetMSG_VDC(void);

void DEA_SetPRE_WARN_FR(uint8_t value);  /*yangxl */
uint8_t DEA_GetPRE_WARN_FR(void);

void DEA_SetAEB_ACTIVE(uint8_t value); /*yangxl */
uint8_t DEA_GetAEB_ACTIVE(void);

void DEA_SetWARN_LANE_L(uint8_t value); /*yangxl */
uint8_t DEA_GetWARN_LANE_L(void);

void DEA_SetWARN_LANE_R(uint8_t value); /*yangxl */
uint8_t DEA_GetWARN_LANE_R(void);

void DEA_SetLDW_INFO(uint8_t value); /*yangxl */
uint8_t DEA_GetLDW_INFO(void);

void DEA_SetLTP_FR(uint8_t value); /*yangxl */
uint8_t DEA_GetLTP_FR(void);

void DEA_SetLTP_FL(uint8_t value); /*yangxl */
uint8_t DEA_GetLTP_FL(void);

void DEA_SetLTP_RR(uint8_t value); /*yangxl */
uint8_t DEA_GetLTP_RR(void);

void DEA_SetLTP_RL(uint8_t value); /*yangxl */
uint8_t DEA_GetLTP_RL(void);

void DEA_SetSPDBLREQ(uint8_t value); /*yangxl */
uint8_t DEA_GetSPDBLREQ(void);

void DEA_SetASCDON_DISP(uint8_t value); /*yangxl */
uint8_t DEA_GetASCDON_DISP(void);

void DEA_SetFT_FR(uint8_t value); /*yangxl */
uint8_t DEA_GetFT_FR(void);

void DEA_SetFT_FL(uint8_t value); /*yangxl */
uint8_t DEA_GetFT_FL(void);

void DEA_SetFT_RL(uint8_t value); /*yangxl */
uint8_t DEA_GetFT_RL(void);

void DEA_SetFT_RR(uint8_t value); /*yangxl */
uint8_t DEA_GetFT_RR(void);

void DEA_SetSRRL_SET(uint8_t value);/*yangxl */
uint8_t DEA_GetSRRL_SET(void);

void DEA_SetFAILD_DT(uint8_t value);/*yangxl */
uint8_t DEA_GetFAILD_DT(void);

void DEA_SetALT_ST(uint8_t value);/*yangxl */
uint8_t DEA_GetALT_ST(void);

void DEA_SetBLOCK_DT(uint8_t value);/*yangxl */
uint8_t DEA_GetBLOCK_DT(void);

void DEA_SetALT_IND(uint8_t value);/*yangxl */
uint8_t DEA_GetALT_IND(void);

void DEA_SetSRRR_SET_R(uint8_t value);/*yangxl */
uint8_t DEA_GetSRRR_SET_R(void);

void DEA_SetFAILD_DT_R(uint8_t value);/*yangxl */
uint8_t DEA_GetFAILD_DT_R(void);

void DEA_SetALT_ST_R(uint8_t value);/*yangxl */
uint8_t DEA_GetALT_ST_R(void);

void DEA_SetBLOCK_DT_R(uint8_t value);/*yangxl */
uint8_t DEA_GetBLOCK_DT_R(void);

void DEA_SetALT_IND_R(uint8_t value);/*yangxl */
uint8_t DEA_GetALT_IND_R(void);

void DEA_SetCAN_MASK(uint8_t value);/*yangxl 2020-4-28 16:39:55 */
uint8_t DEA_GetCAN_MASK(void);





//void DEA_SetTPMS_POSI_SYNC(uint8_t value); /*yangxl */
//uint8_t DEA_GetTPMS_POSI_SYNC(void);


void DEA_SetRtcTime(ExternSTDType *pRtcTime);

ExternSTDType DEA_GetRtcTime(void);

void DEA_SetIpcRecoverFlag(uint8_t value);
uint8_t DEA_GetIpcRecoverFlag(void);





#endif

