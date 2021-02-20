#ifndef PMSTATEMACHINE_H
#define PMSTATEMACHINE_H

/*=============[I N T E R F A C E  W I T H  A P P L I C A T I O N  S O F T W A R E]=============*/
/************************************************************************************************/
#include "Std_Types.h"

/*============================[D E S C R I P T I O N]===========================================*/
/************************************************************************************************/

/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/
#define PM_H_AR_MAJOR_VERSION   5U
#define PM_H_AR_MINOR_VERSION   2U
#define PM_H_AR_PATCH_VERSION   0U

#define PM_H_SW_MAJOR_VERSION   2U//Power_manage_V2.5
#define PM_H_SW_MINOR_VERSION   5U
#define PM_H_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
#define PM_REF_NM_INFO STD_ON

#define POWERMODING_DEBUG

#define PORT_HIGH 1U
#define PORT_LOW 0U

#define PM_AMP_MUTE PORT_LOW
#define PM_AMP_UNMUTE PORT_HIGH

#define PM_AMP_STANDBY PORT_LOW
#define PM_AMP_WORKING PORT_HIGH

#define REMOTE_START_MASK   0xF0
#define REMOTE_START_FlAG   0xA0


#define ACCEPT_SLEEP  SC_AcceptSleep
#define KEEP_WAKEUP   SC_WakeUp


#define ACC_ON             0U
#define ACC_OFF            1U

#define PM_IGN_ON          0U
#define PM_IGN_OFF         1U

#define PM_TCU_ACC_ON      0U
#define PM_TCU_ACC_OFF     1U

#define SYS_LVI_APPEAR	   1U //battery voltage <7.8V
#define SYS_HVI_APPEAR	   0U //battery voltage >16.8V or <7.8v

/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/
typedef enum
{
	PM_MODE_FUNC_MAIN,
	PM_MODE_FUNC_ENTRY,
	PM_MODE_FUNC_EXIT,
	PM_MODE_FUNC_MAX,
} PMModeFuncType;

typedef enum
{
	PM_MODE_ALL_OFF,     //0
	PM_MODE_TEMPO_OFF,   //1
	PM_MODE_ENG_OFF,     //2 PM_MODE_TEMPO_ON
	PM_MODE_ENG_ON,      //3
	PM_MODE_CRANKING,    //4
	PM_MODE_RMT_CHK,     //5
	PM_MODE_RMT_START,   //6
	PM_MODE_OTA_UPDATE,  //7
	PM_MODE_MAX,         //8
} PMModeType;

typedef enum
{
	PM_ANIMATE_INIT,
	PM_ANIMATE_RUNNING,
	PM_ANIMATE_ENDED,
	PM_ANIMATE_MAX,
} PMAnimateType;

typedef enum
{
	PM_VOLT_STATE_NORMAL,
	PM_VOLT_STATE_WARNING,
	PM_VOLT_STATE_ERROR,
} PMVoltStateType;

typedef enum
{
	PM_MBTEMP_STATE_NORMAL,
	PM_MBTEMP_STATE_WARNING,
	PM_MBTEMP_STATE_ERROR,
} PMMbTempStateType;

typedef enum
{
	PM_AMPTEMP_STATE_NORMAL,
	PM_AMPTEMP_STATE_WARNING,
	PM_AMPTEMP_STATE_ERROR,
} PMAmpTempStateType;

//reference:<64.532 DA CAN MSG DESIGN_V1.3.3.xlsx> 'receive1'sheet, 'EngineStatus'
typedef enum
{
	PM_EngineStatus_stopped=0,
	PM_EngineStatus_stalled,
	PM_EngineStatus_running,
	PM_EngineStatus_cranking,
} PMEngineStatusType;
//reference:<64.532 DA CAN MSG DESIGN_V1.3.3.xlsx> 'receive1'sheet, 'DoorSwitchesState'
typedef enum
{
	PM_DoorSwitchesState_none=0,//xxxxx
	PM_DoorSwitchesState_left_open=1,//xxxx1
	PM_DoorSwitchesState_right_open=2,//xxx1x
	PM_DoorSwitchesState_rear_left_open=4,//xx1xx
	PM_DoorSwitchesState_rear_right_open=8,//x1xxx
	PM_DoorSwitchesState_back_open=16,//1xxxx
} PMDoorSwitchesStateType;

typedef struct
{
	PMVoltStateType volt;
	PMMbTempStateType mbTemp;
	PMAmpTempStateType ampTemp;
} PMSafeType;

typedef struct
{
	uint8 preMode;
	uint8 curMode;
	uint8 nextMode;

	PMAnimateType animateState;
	boolean daLcdUsrCtr;
	boolean icLcdUsrCtr;
	boolean ampMute;
	boolean diagActive;
	PMSafeType safeState;
	//uint8 circuitVersion;
	uint8 variantCtrl;
} PMManagerType;

typedef struct
{
	boolean Acc_status;
	boolean Tcu_Acc_status;//replace by DAwakeupRequest in canid=0x563
	boolean Ign_status;
	boolean RmtStart_Flag;//<64.532 DA CAN MSG DESIGN_V1.3.3.xlsx> sheet receive1 remote _ENG-ST_STATE value 1b=IGN on by remote ENG-ST
	uint8 RmtStart_TimeOut;
	uint8 Reverse_status;
	PMEngineStatusType EngineStatus;//<64.532 DA CAN MSG DESIGN_V1.3.3.xlsx> , sheet receive1 ,EngineStatus
	PMDoorSwitchesStateType DoorSwitchesState;//<64.532 DA CAN MSG DESIGN_V1.3.3.xlsx> , sheet receive1 ,DoorSwitchesState
	boolean CanSleepStatus;
	boolean TouchScreenStatus;
	boolean DAwakeupRequest;
	
	boolean EngOff2TempOff_OnceFlag;
	boolean ISS_ACT;

	boolean AVM_Outdoor_Photo_Request;
	boolean AVM_Outdoor_Photo_Result;
	uint32 AVM_Outdoor_Photo_Request_StartTimer;
} PMSignalStatusType;

//1.this logic add by tjy @20200319 request by liqian used for solve U1000
//2.at ALL_OFF mode and received the state of SOC power off TFT ,when wake up event occur ,reset system immediately
//3.follow DA TFT power on sequence (must delay 3000 ms after power off)
#define PM_U1000_SOLUTION STD_ON

#endif

