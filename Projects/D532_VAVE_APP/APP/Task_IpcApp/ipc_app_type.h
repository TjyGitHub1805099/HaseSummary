#ifndef _IPC_APP_TYPE_H_
#define _IPC_APP_TYPE_H_

typedef enum
{
    IPC_SOC_STATUS_UPDATE_IdleStopNotAvailablePopupConfirm = 1,
    IPC_SOC_STATUS_UPDATE_IdleStopNotAvailablePopupIsShowing,
    IPC_SOC_STATUS_UPDATE_RearSeatPopupIsShowing,
} IPC_SOC_STATUS_UPDATE_ENUM;

typedef enum
{
	/*01*/ IPC_SOC_SETTING_language = 1,        //[0,1]语言�?:英语 1:汉语
    /*02*/ IPC_SOC_SETTING_unit,                //[0,1]单位�?:km/h,1:mile/h
    /*03*/ IPC_SOC_SETTING_triePressure,        //[0,1]胎压报警开�?
    /*04*/ IPC_SOC_SETTING_driveAssistant,      //[0,1]驾驶辅助开�?
    /*05*/ IPC_SOC_SETTING_autoStopSingle,      //[0,1]自动启停-单次开�?
    /*06*/ IPC_SOC_SETTING_autoStopSum,         //[0,1]自动启停-累计开�?
    /*07*/ IPC_SOC_SETTING_sound,               //[0,1]音响开�?
    /*08*/ IPC_SOC_SETTING_powerCycle,          //[0,1]能量循环开�?
    /*09*/ IPC_SOC_SETTING_mapType,             //[0,1]全局地图类型(0:2d,1:3d)
    /*10*/ IPC_SOC_SETTING_playEffect,          //[0,1]动态效果设置开�?
    /*11*/ IPC_SOC_SETTING_alertSound,          //[0,1]语音辅助报警开�?
    /*12*/ IPC_SOC_SETTING_vehicleDeviateAlert, //[0,1]车辆偏离预警开�?
    /*13*/ IPC_SOC_SETTING_blindAreaMonitoring, //[0,1]盲区监测开�?
    /*14*/ IPC_SOC_SETTING_emergencyBraking,    //[0,1]前方防碰撞紧急制动开�?
    /*15*/ IPC_SOC_SETTING_parkAssistant,       //[0,1]泊车辅助开�?
    /*16*/ IPC_SOC_SETTING_driveTimeAlert,      //[0,1]驾驶时间提醒开�?
    /*17*/ IPC_SOC_SETTING_driveTimeSet,        //[30,360](分钟)驾驶时间提醒设定时间
    /*18*/ IPC_SOC_SETTING_maintenanceMileage,  //[0,10000](km)??里程保养�?关闭
    /*19*/ IPC_SOC_SETTING_maintenanceTime,     //[0,365](�???时间保养,0关闭
    /*20*/ IPC_SOC_SETTING_backLightValue,      //[0,21]仪表背光
    /*21*/ IPC_SOC_SETTING_backLightNightValue, //[0,21]仪表背光,夜间模式
    /*22*/ IPC_SOC_SETTING_speedLimit,          //[0,24](单位10km/h,0:关闭�?4:240km/h)
    /*23*/ IPC_SOC_SETTING_bestFuelTimeOut,     //[0,60]最佳燃油设定默认超�?second,ui step:5)
    /*24*/ IPC_SOC_SETTING_frontTire,           //[0,0x0f]前胎压告警阈值，定义见表�?
    /*25*/ IPC_SOC_SETTING_rearTire,            //[0,0x0f]后胎压告警阈值，定义见表�?
    /*26*/ IPC_SOC_SETTING_hmiLastPage,         //[0,0xff]ui页面代码
    /*27*/ IPC_SOC_SETTING_IlluminationSetting, //[0,7]氛围�?user set
    /*28*/ IPC_SOC_SETTING_TimeFormat,          //[0,1]0:24,1:12
    /*29*/ IPC_SOC_SETTING_NaviMode,            //[0,1]0:small 1:big,only save,donot notificatio
    /*30*/ IPC_SOC_SETTING_NavStyle,            //[0,1]0:Modern 1:Technology
  	/*31*/ IPC_SOC_SETTING_EAPM_Setting,   		//[0,1]防误踩油门系统开�?
  	/*32*/ IPC_SOC_SETTING_LightSensitivity,  	//[0,1]点灯灵敏�?
  	/*33*/ IPC_SOC_SETTING_LightOffDay,      	//[0,1]灯光关闭延时
  	/*34*/ IPC_SOC_SETTING_SelectiveUnlock,    	//[0,1]单车门解�?
  	/*35*/ IPC_SOC_SETTING_WiperWithSpeed,    	//[0,1]速度感应雨刷  

	/* yangxl */
	/*36*/ IPC_SOC_SETTING_AutomaticClosingWindow,				//0: OFF 1: ON
	/*37*/ IPC_SOC_SETTING_BrushAwayTheRain,					//0: OFF 1: ON
	/*38*/ IPC_SOC_SETTING_RearviewMirrorFoldsAutomatically,	//0: OFF 1: ON
	/*39*/ IPC_SOC_SETTING_RearviewMirrorSwitch,				//0: OFF 1: ON
	/*40*/ IPC_SOC_SETTING_AutomaticLocking,					//0: OFF 1: ON
	/*41*/ IPC_SOC_SETTING_AutomaticUnlock,						//0: OFF 1: ON
	/*42*/ IPC_SOC_SETTING_IntensityOfLight,					//0: low 1: midd 2: high 
	/*43*/ IPC_SOC_SETTING_TurnOffLightsDelay,					//0: 0s 1: 30s 2: 45s 3: 60s 4: 90s  
	/*44*/ IPC_SOC_SETTING_CarLockPrompt,						//0: OFF 1: ON
	/*45*/ IPC_SOC_SETTING_RestoreFactorySettings,				//0: OFF 1: ON
	/*46*/ IPC_SOC_SETTING_RainfallSensitivity,					//0: low 1: midd 2: high 
	/*47*/ IPC_SOC_SETTING_FEBSwitch,							//0: OFF 1: ON
	/*48*/ IPC_SOC_SETTING_LKASwitch,							//0: OFF 1: ON
	/*49*/ IPC_SOC_SETTING_BSWSwitch,							//0: OFF 1: ON
	/*50*/ IPC_SOC_SETTING_CTASwitch,							//0: OFF 1: ON
	/*51*/ IPC_SOC_SETTING_EAPMSwitch,							//0: OFF 1: ON
	/*52*/ IPC_SOC_SETTING_DOASwitch,							//0: OFF 1: ON	
	/*53*/ IPC_SOC_SETTING_LDWSwitch,							//0: OFF 1: ON
	/*54*/ IPC_SOC_SETTING_ECOSwitch,							//0: OFF 1: ON		
	/*55*/ IPC_SOC_SETTING_ISSSwitch,							//0: OFF 1: ON

	/*56*/ IPC_SOC_SETTING_HUDSwitch ,                        //0: OFF 1: ON
    /*57*/ IPC_SOC_SETTING_HUDNavigationDisplaySwitch ,       //0: OFF 1: ON
    /*58*/ IPC_SOC_SETTING_HUDDrivingAssistanceSwitch ,       //0: OFF 1: ON
    /*59*/ IPC_SOC_SETTING_HUDTachometerSwitch ,              //0: OFF 1: ON
    /*60*/ IPC_SOC_SETTING_HUDFuelConsumptionSwitch ,         //0: OFF 1: ON
    /*61*/ IPC_SOC_SETTING_HUDMusicStateSwitch ,              //0: OFF 1: ON
    /*62*/ IPC_SOC_SETTING_HUDMixedModeSwitch ,               //0: OFF 1: ON
    /*63*/ IPC_SOC_SETTING_HUDCallReminderSwitch ,            //0: OFF 1: ON
    /*64*/ IPC_SOC_SETTING_HUDLyricSwitch ,                   //0: OFF 1: ON
    /*65*/ IPC_SOC_SETTING_Custommenucombinations,            //[0,4]
    /*66*/ IPC_SOC_SETTING_HUDRotationAngle ,                 //
    /*67*/ IPC_SOC_SETTING_HUDStyle ,                         //
    /*68*/ IPC_SOC_SETTING_UPASwitch ,                         //
	/* yangxl */

	IPC_SOC_SETTING_MAX,
} IPC_SOC_SETTING_TYPE_ENUM;

typedef enum
{
    //==========================
    IPC_SOC_COMMAND_socReady = 1,
    IPC_SOC_COMMAND_lcdPower,
    IPC_SOC_COMMAND_updateReq,
    IPC_SOC_COMMAND_sysOnorOff,

    //==========================
    IPC_SOC_COMMAND_airbagStatus, //[0:off][1:on][2:fault]
    IPC_SOC_COMMAND_overSpeed,    //[1:Trip A reset][2:Trip B reset]
    IPC_SOC_COMMAND_resetTrip,    //[1:Trip A reset][2:Trip B reset]
    IPC_SOC_COMMAND_resetAverageSpeed,
    IPC_SOC_COMMAND_resetAverageFuel,
    IPC_SOC_COMMAND_resetDriveTime,
    IPC_SOC_COMMAND_resetFuelCap,

    //===========================
    IPC_SOC_COMMAND_FactoryReset,
    IPC_SOC_COMMAND_HistoryFuelReset,
    IPC_SOC_COMMAND_HistoryFuelUpdate,
    IPC_SOC_COMMAND_IdleStopCumulativeReset,
    IPC_SOC_COMMAND_IdleStopTripReset,
    IPC_SOC_COMMAND_SonarDisplayCancel,
    IPC_SOC_COMMAND_AudioOnOff,
    IPC_SOC_COMMAND_Illumination,
    IPC_SOC_COMMAND_SelfDiagModeSwitch,
    IPC_SOC_COMMAND_MeterBasicInfo,
    IPC_SOC_COMMAND_VehicleReset,

	IPC_SOC_COMMAND_HudReset,  //yangxl 2019-12-23 10:57:22
	IPC_SOC_COMMAND_MeterReset, //yangxl 2019-12-23 10:57:22

	IPC_SOC_COMMAND_MAX,
} IPC_SOC_COMMAND_TYPE_ENUM;

typedef enum
{
    //==========================
    IPC_IOC_COMMAND_CarBodySetInProgress = 1,
} IPC_IOC_COMMAND_TYPE_ENUM;


typedef enum
{
    TELLTALE_OFF = 0,
    TELLTALE_ON,
    TELLTALE_1Hz,
    TELLTALE_2Hz,
    TELLTALE_0p5Hz,
    TELLTALE_2p5Hz,
    TELLTALE_INVALID,
} TELLTALE_ENUM;

typedef enum
{
    ICON_OFF = 0,
    ICON_ON,
    ICON_INVALID,
} ICON_STAT_ENUM;

typedef enum
{
    ACOUSTIC_OFF = 0,
    ACOUSTIC_ON,
    ACOUSTIC_INVALID,
} ACOUSTIC_STAT_ENUM;

typedef enum
{
    FUN_ID_BASIC_FAST = ( 0x01 ),
    FUN_ID_BASIC_MIDDLE = ( 0x02 ),
    FUN_ID_BASIC_SLOW = ( 0x03 ),
    FUN_ID_WARN_MSG = ( 0x04 ),
    FUN_ID_NORMAL_MSG = ( 0x05 ),
    FUN_ID_SYS_CTRL_MSG = ( 0x06 ),
    FUN_ID_TPMS = ( 0x07 ),
    FUN_ID_DRIVE_TIME = ( 0x08 ),
    FUN_ID_TIME_INFO_MSG = ( 0x09 ),
    FUN_ID_SEMI_SET_MSG = ( 0x0A ),
    FUN_ID_SYS_SET_MSG = ( 0x0B ),
    FUN_ID_BUTTON_MSG = ( 0x0C ),
    FUN_ID_IDLE_STOP = ( 0x0D ),
    FUN_ID_HIS_AVG_FUEL = ( 0x0E ),
    FUN_ID_SELF_DIAG_D = ( 0x0F ),
    FUN_ID_SELF_DIAG_S = ( 0x10 ),
    FUN_ID_BASIC_INFO = ( 0x11 ),
    FUN_ID_SYSTEM_REQ = ( 0x12 ),
    FUN_ID_IOC_SET_MSG = ( 0x13 ),

    FUN_ID_SOC_SYSTEM_SET_MSG = ( 0x81 ),
    FUN_ID_SOC_SET_MSG = ( 0x82 ),
    FUN_ID_ACOUSTIC_MSG = ( 0x84 ),
    FUN_ID_IOC_STAT = ( 0xF9 ), 
    FUN_ID_DIAG_L351_SIG = ( 0xFA ), /* yangxl */
    FUN_ID_DIAG_SIG = ( 0xFB ),
    FUN_ID_MULTI_CAN_MSG = ( 0xFC ),
    FUN_ID_HARDWARE_SIG = ( 0xFD ),
    FUN_ID_CAN_TIMEOUT = ( 0xFE ),
} IPC_FUN_ID_ENUM;

typedef enum
{
	NORMAL_MSG_ID_TurnLeft = 1,
	NORMAL_MSG_ID_TurnRight,
	NORMAL_MSG_ID_TurnLeft_TurnRight,
	NORMAL_MSG_ID_HighBeam,
	NORMAL_MSG_ID_AutoHighBeamWhite,
	NORMAL_MSG_ID_AutoHighBeamYellow,
	NORMAL_MSG_ID_LowBeam,
	NORMAL_MSG_ID_FogLight,
	NORMAL_MSG_ID_RearFog,
	NORMAL_MSG_ID_Position,
	NORMAL_MSG_ID_TPMS,
	NORMAL_MSG_ID_EPSFault,
	NORMAL_MSG_ID_ESC,
	NORMAL_MSG_ID_ESCOFF,
	NORMAL_MSG_ID_HDCWarn,
	NORMAL_MSG_ID_EngineHoodOpen,
	NORMAL_MSG_ID_FrontLeftDoorOpen,
	NORMAL_MSG_ID_FrontRightDoorOpen,
	NORMAL_MSG_ID_RearLeftDoorOpen,
	NORMAL_MSG_ID_RearRightDoorOpen,
	NORMAL_MSG_ID_TrunkDoorOpen,
	NORMAL_MSG_ID_AutoholdGreen,
	NORMAL_MSG_ID_AutoholdYellow,
	NORMAL_MSG_ID_CruiseControlIndWhite,
	NORMAL_MSG_ID_CruiseControlIndGreen,
	NORMAL_MSG_ID_BatOn,
	NORMAL_MSG_ID_LowFuel,
	NORMAL_MSG_ID_Engineoverheated,
	NORMAL_MSG_ID_Enginefailure,
	NORMAL_MSG_ID_Engineoilpressure,
	NORMAL_MSG_ID_BrakeError,
	NORMAL_MSG_ID_EPB,
	NORMAL_MSG_ID_ABS,
	NORMAL_MSG_ID_AEB_OFF,
	NORMAL_MSG_ID_AEB,
	NORMAL_MSG_ID_AirbagSystemFault,
	NORMAL_MSG_ID_Frontseatbelt,
	NORMAL_MSG_ID_Security,
	NORMAL_MSG_ID_BatteryRemaining,
	NORMAL_MSG_ID_READY,
	NORMAL_MSG_ID_E_PKB_Warn,
	NORMAL_MSG_ID_ChargeConnect,
	NORMAL_MSG_ID_SlowIndicator,
	NORMAL_MSG_ID_ITS_FEB_White,
	NORMAL_MSG_ID_ITS_FEB_Yellow,
	NORMAL_MSG_ID_ITS_LDW_White,
	NORMAL_MSG_ID_ITS_LDW_Yellow,
	NORMAL_MSG_ID_ITS_BSW_White,
	NORMAL_MSG_ID_ITS_BSW_Yellow,
	NORMAL_MSG_ID_Snow,
	NORMAL_MSG_ID_AT_Check,
	NORMAL_MSG_ID_IdleStop,
	NORMAL_MSG_ID_ECOMode,
	NORMAL_MSG_ID_PKB_Warning_Display,
	NORMAL_MSG_ID_I_KEY_Complete,
	NORMAL_MSG_ID_I_KEY_Error,
	NORMAL_MSG_ID_HeadLampWarning,
	NORMAL_MSG_ID_FuelCapWarning,
	NORMAL_MSG_ID_BSWBlockageConditionDetected,
	NORMAL_MSG_ID_BSWTemporaryOFF,
	NORMAL_MSG_ID_BSWmalfunction,
	NORMAL_MSG_ID_LDWSystemError,
	NORMAL_MSG_ID_LDWCameraError,
	NORMAL_MSG_ID_FEBSystemFault,
	NORMAL_MSG_ID_FEBVDC_OFFError,
	NORMAL_MSG_ID_FEBRadarError,
	NORMAL_MSG_ID_FEBPedestrianDetectionError,
	NORMAL_MSG_ID_LightReminder,
	NORMAL_MSG_ID_BrakeEngineStart,
	NORMAL_MSG_ID_BrakeEngineStart1,
	NORMAL_MSG_ID_PushEngineStart,
	NORMAL_MSG_ID_PushEngineStart1,
	NORMAL_MSG_ID_CrutchEngineStart,
	NORMAL_MSG_ID_CrutchEngineStart1,
	NORMAL_MSG_ID_PutKeyFab,
	NORMAL_MSG_ID_SteeringRotate,
	NORMAL_MSG_ID_KeyBattery,
	NORMAL_MSG_ID_LockWarning,
	NORMAL_MSG_ID_KeyFobTakeOutWarning,
	NORMAL_MSG_ID_KEYIDNG,
	NORMAL_MSG_ID_ShiftPPosition,
	NORMAL_MSG_ID_ShiftPAndRemoteENGStart,
	NORMAL_MSG_ID_BatterySaverDisplay,
	NORMAL_MSG_ID_BatterySaverDisplayLater,
	NORMAL_MSG_ID_E_PKBBenchTestMode,
	NORMAL_MSG_ID_E_PKBPressBrakePedal,
	NORMAL_MSG_ID_E_PKBPressBrakePedalDRank,
	NORMAL_MSG_ID_E_PKBReleaseParkingBrake,
	NORMAL_MSG_ID_E_PKBReleaseParkingBrakeDRank,
	NORMAL_MSG_ID_E_PKBEmergencyBrakeActive,
	NORMAL_MSG_ID_E_PKBAutoFunctionFailed,
	NORMAL_MSG_ID_E_PKBCalibrating,
	NORMAL_MSG_ID_E_PKBSlopeIsBeyond,
	NORMAL_MSG_ID_RearSeatbeltLeft,
	NORMAL_MSG_ID_RearSeatbeltMiddle,
	NORMAL_MSG_ID_RearSeatbeltRight,
	NORMAL_MSG_ID_IdleStopON,
	NORMAL_MSG_ID_IdleStopOFF,
	NORMAL_MSG_ID_IdleStopWarning,
	NORMAL_MSG_ID_IdleStopNotAvailable,
	NORMAL_MSG_ID_IdleStopPushEngine,
	NORMAL_MSG_ID_IdleStopTurnIgn,
	NORMAL_MSG_ID_IdleStopSystemFault,
	NORMAL_MSG_ID_IdleStopISSInd,
	NORMAL_MSG_ID_DCTErrorPark,
	NORMAL_MSG_ID_DCTOverHeat,
	NORMAL_MSG_ID_DCTErrorRepair,
	NORMAL_MSG_ID_RemoteENGStart,
	NORMAL_MSG_ID_DriveTimeInterrupt,
	NORMAL_MSG_ID_LowSocWarning,
	NORMAL_MSG_ID_ChargeWarnning,
	NORMAL_MSG_ID_ShiftByWireWarning,
	NORMAL_MSG_ID_ShiftByWireWarningR,
	NORMAL_MSG_ID_ShiftByWireWarningGear,
	NORMAL_MSG_ID_ChargeFail,
	NORMAL_MSG_ID_ChargeTurnOffPower,
	NORMAL_MSG_ID_EVPSystemError,
	NORMAL_MSG_ID_VCMFailure,
	NORMAL_MSG_ID_VCMChargeConnect,
	NORMAL_MSG_ID_BatteryTempControlFailure,
	NORMAL_MSG_ID_AirConditioningLineFailure,
	NORMAL_MSG_ID_ElectricalMotorWarning,
	NORMAL_MSG_ID_DynamicBatteryWarning,
	NORMAL_MSG_ID_CVT_Sport,
	NORMAL_MSG_ID_TireLowPresureWarning,
	NORMAL_MSG_ID_FlatTireWarning,
	NORMAL_MSG_ID_ParkingSensorError,
	NORMAL_MSG_ID_TpmsResetAccept,
	NORMAL_MSG_ID_EAPM_IndicatorWhite,
	NORMAL_MSG_ID_EAPM_IndicatorYellow,
	NORMAL_MSG_ID_DoorOpenStatus,
	NORMAL_MSG_ID_EAPM_TemporaryOFF,
	NORMAL_MSG_ID_EAPM_SystemOFF,
	NORMAL_MSG_ID_EAPM_SystemFault,
	NORMAL_MSG_ID_EAPM_SystemON,
	NORMAL_MSG_ID_MasterWarning,
	
	/* yangxl */
	NORMAL_MSG_ID_DCTErrorEngTorqueDown,
	NORMAL_MSG_ID_DCTOverHeatEngTorqueDown,
	NORMAL_MSG_ID_DCTOverHeatFunctionLimited,

	NORMAL_MSG_ID_ConfirmPRangStatus,
	NORMAL_MSG_ID_ConfirmShiftLeverPosition,
	NORMAL_MSG_ID_GearshiftFaultGo4S,
	NORMAL_MSG_ID_GearshiftFaultContact4S,

	NORMAL_MSG_ID_ExhaustFilterGreen,
	NORMAL_MSG_ID_ExhaustFilterYellow,
	NORMAL_MSG_ID_ExhaustFilterRed,

	NORMAL_MSG_ID_ITS_ACC_White,
	NORMAL_MSG_ID_ITS_ACC_Green,
	NORMAL_MSG_ID_ITS_ACC_Yellow,
	NORMAL_MSG_ID_Vehicle_Distance_L1,
	NORMAL_MSG_ID_Vehicle_Distance_L2,
	NORMAL_MSG_ID_Vehicle_Distance_L3,
	NORMAL_MSG_ID_Steering_Assist_OFF,
	NORMAL_MSG_ID_Steering_Assist_ON,

	NORMAL_MSG_ID_T_Box_Register_complete,
	NORMAL_MSG_ID_SSB_Failure,
	NORMAL_MSG_ID_IGN1_Failure,
	NORMAL_MSG_ID_ESCL_Failure,
	NORMAL_MSG_ID_48V_System_Ind,
	NORMAL_MSG_ID_Shift_N_P_Eng_Start,

	NORMAL_MSG_ID_ACC_Disable,
	NORMAL_MSG_ID_ACC_Active_Set,
	NORMAL_MSG_ID_ACC_Abnormal,
	NORMAL_MSG_ID_ACC_Active_Res,
	NORMAL_MSG_ID_ACC_Failure,
	NORMAL_MSG_ID_ACC_VDC_OFF,
	NORMAL_MSG_ID_ACC_Radar_KeepOut,
	NORMAL_MSG_ID_ACC_System_Failure,
	NORMAL_MSG_ID_LKA_TimeOut_Exit,
	NORMAL_MSG_ID_LKA_Failure,
	NORMAL_MSG_ID_LKA_Camera_KeepOut,
	NORMAL_MSG_ID_LKA_Camera_OverHeat,
	NORMAL_MSG_ID_LKA_System_Failure,
	NORMAL_MSG_ID_LKA_Indicator_White,
	NORMAL_MSG_ID_LKA_Indicator_Green,
	NORMAL_MSG_ID_LKA_Indicator_Red,
	NORMAL_MSG_ID_LKA_Indicator_Yellow,
	NORMAL_MSG_ID_LDW_Camera_OverHeat,
	NORMAL_MSG_ID_LDW_Failure,
	NORMAL_MSG_ID_FEB_System_Fault2,

	NORMAL_MSG_ID_TirelowpresureWarning2,
	NORMAL_MSG_ID_Flattirewarning2,
	NORMAL_MSG_ID_48V_ASSIST,
	NORMAL_MSG_ID_48V_RETREIVE,

	NORMAL_MSG_ID_LKA_VIEW_5,
	NORMAL_MSG_ID_LKA_VIEW_4,
	NORMAL_MSG_ID_DCT_Function_Limited_Ctr_Err,

	NORMAL_MSG_ID_Push_Brake,
	
	/* yangxl */
	NORMAL_MSG_ID_MAX
} NORMAL_MSG_ID_ENUM;

typedef enum
{
	NORMAL_MSG_ID_DoorOpenBuzzer = 200,
	NORMAL_MSG_ID_BackdoorOpen,
	NORMAL_MSG_ID_LightLeftOnAlarm,
	NORMAL_MSG_ID_SmartKeyInReminderBuzzer,
	NORMAL_MSG_ID_KeyInIgnitionReminder,
	NORMAL_MSG_ID_SmartKey_OFF_PositionReminderBuzzer,
	NORMAL_MSG_ID_E_PKBWarningBuzzer,
	NORMAL_MSG_ID_SmartRemoteOutOfCarReminderBuzzer,
	NORMAL_MSG_ID_IdleStopBuzzer,
	NORMAL_MSG_ID_SeatBeltWarning,
	NORMAL_MSG_ID_TireAlarm, 
	NORMAL_MSG_ID_UserModeSwitch,
	NORMAL_MSG_ID_ParkingBrakLeftReminder,
	NORMAL_MSG_ID_IlluminationControlMaxMin,
	NORMAL_MSG_ID_MultidisplayAlarmSound,
	NORMAL_MSG_ID_AT_GearRefuseBuzzer,
	NORMAL_MSG_ID_TurnSoundTi,
	NORMAL_MSG_ID_TurnSoundTa,
	NORMAL_MSG_ID_FCW_Buzzer,
	NORMAL_MSG_ID_FEB_Buzzer,
	NORMAL_MSG_ID_BSW_Buzzer,
	NORMAL_MSG_ID_SonarLV1_Buzzer,
	NORMAL_MSG_ID_SonarLV2_Buzzer,
	NORMAL_MSG_ID_SonarLV3_Buzzer,
	NORMAL_MSG_ID_SonarLV4_Buzzer,
	NORMAL_MSG_ID_LDW_Buzzer,
	NORMAL_MSG_ID_EAPM_Buzzer,
	NORMAL_MSG_ID_MOD_Buzzer,
	NORMAL_MSG_ID_PressBrakePedalD_Rank,
	NORMAL_MSG_ID_SlopeIsTooLarge,
	NORMAL_MSG_ID_PressTheBrakePedal,
	NORMAL_MSG_ID_E_PKB_NotReleasedRepeat,
	NORMAL_MSG_ID_E_PKB_NotReleased,
	/* yangxl */
	NORMAL_MSG_ID_UPA_SelfCheck_Buzzer,
	NORMAL_MSG_ID_UPA_Fault_Buzzer,
	NORMAL_MSG_ID_FAP_FindParking_Buzzer,
	NORMAL_MSG_ID_FAP_FindParkingSlowDown_Buzzer,
	NORMAL_MSG_ID_FAP_QuitFindParking_Buzzer,
	NORMAL_MSG_ID_FAP_APAStart_Buzzer,
	NORMAL_MSG_ID_FAP_APAComplete_Buzzer,
	NORMAL_MSG_ID_FAP_APASuspended_Buzzer,
	NORMAL_MSG_ID_FAP_APAQuit_Buzzer,
	NORMAL_MSG_ID_LKA_Standby_Buzzer,
	NORMAL_MSG_ID_LKA_Controlling_Buzzer,
	NORMAL_MSG_ID_LKA_ToSell_Buzzer,
	NORMAL_MSG_ID_LKA_DrvrTkovReq_Buzzer,
	NORMAL_MSG_ID_ACC_DrvrTkov_Buzzer,

	NORMAL_MSG_ID_TrFaultContBuzzer,
	/* yangxl */
	NORMAL_BUZZER_ID_MAX,
}NORMAL_BUZZER_ID_ENUM;


typedef enum
{
	IPC_GEAR_None,
	IPC_GEAR_1,
	IPC_GEAR_2,
	IPC_GEAR_3,
	IPC_GEAR_4,
	IPC_GEAR_5,
	IPC_GEAR_6,
	IPC_GEAR_7,
	IPC_GEAR_P,
	IPC_GEAR_R,
	IPC_GEAR_N,
	IPC_GEAR_INVALID = 15, 
}IPC_GEAR_ENUM;

typedef enum
{
	NORMAL_MSG_STAT_RESET = 0,
	NORMAL_MSG_STAT_SET,
	NORMAL_MSG_STAT_1Hz,
	NORMAL_MSG_STAT_2Hz,
	NORMAL_MSG_STAT_Reserved,
}NORMAL_MSG_STAT_ENUM;

typedef enum
{
	IPC_FEATURE_IOC_NTF = 1,	/* ioc notify*/
	IPC_FEATURE_IOC_REQ,		/* ioc request */
	IPC_FEATURE_IOC_REP,		/* ioc reply */
	IPC_FRATURE_SOC_NTF,		/* soc notify */
	IPC_FEATURE_SOC_REQ,		/* soc request */
	IPC_FEATURE_SOC_REP,		/* soc reply */
}IPC_FEATURE_ENUM;

typedef enum
{
   IPC_PM_ALL_OFF,     //0
   IPC_PM_TEMPO_OFF,   //1
   IPC_PM_ENG_OFF,     //2
   IPC_PM_ENG_ON,      //3
   IPC_PM_CRANKING,    //4
   IPC_PM_RMT_CHK,     //5
   IPC_PM_RMT_START,   //6
   IPC_PM_MAX,         //7
} IPC_PM_ENUM;

typedef enum
{
	IPC_ACOUSTIC_OFF,
	IPC_ACOUSTIC_ON,
}IPC_ACOUSTIC_ENUM;

typedef enum
{
	IPC_ADAS_FEB_VIEW_OFF,
	IPC_ADAS_FEB_VIEW_ON,			//white
	IPC_ADAS_FEB_VIEW_BLINK,		//yellow blink 2.5hz
	IPC_ADAS_FEB_VIEW_ON_YELLOW,	//yellow
	IPC_ADAS_FEB_MAX,
}IPC_ADAS_FEB_ENUM;

typedef enum
{
	IPC_ADAS_BSW_VIEW_OFF,
	IPC_ADAS_BSW_VIEW_ON_LEFT,	//left white on
	IPC_ADAS_BSW_VIEW_ON_RIGHT,	//right white on
	IPC_ADAS_BSW_VIEW_ON_LEFT_RIGHT,	//left and right white on
	IPC_ADAS_BSW_TEMP_OFF_LEFT,			//left white blink 0.5hz
	IPC_ADAS_BSW_TEMP_OFF_RIGHT,		//right white blink 0.5hz
	IPC_ADAS_BSW_TEMP_OFF_LEFT_RIGHT,	//left and right white blink 0.5hz
	IPC_ADAS_BSW_VIEW_ON_YELLOW_LEFT,	//left yellow
	IPC_ADAS_BSW_VIEW_ON_YELLOW_RIGHT,	//right yellow
	IPC_ADAS_BSW_VIEW_ON_YELLOW_LEFT_RIGHT,	//left right yellow
	IPC_ADAS_BSW_VIEW_ON_BLINK_LEFT,		//left yellow blink 2.5Hz
	IPC_ADAS_BSW_VIEW_ON_BLINK_RIGHT,		//right yellow blink 2.5Hz
	IPC_ADAS_BSW_VIEW_ON_BLINK_LEFT_RIGHT,	//left right yellow blink 2.5Hz
	IPC_ADAS_BSW_VIEW_ON_BLINK_LEFT_AND_RIGHT_ON,		//left yellow blink 2.5Hz, right white on
	IPC_ADAS_BSW_VIEW_ON_BLINK_RIGHT_AND_LEFT_ON,		//right yellow blink 2.5Hz, left white on
	IPC_ADAS_BSW_MAX,
}IPC_ADAS_BSW_ENUM;

typedef enum
{
	IPC_ADAS_LDW_VIEW_OFF,
	IPC_ADAS_LDW_VIEW_ON,			//white
	IPC_ADAS_LDW_VIEW_LEFT_BLINK,	//yellow blink 2.5hz
	IPC_ADAS_LDW_VIEW_RIGHT_BLINK,	//yellow blink 2.5hz
	IPC_ADAS_LDW_VIEW_ON_YELLOW,	//yellow
	IPC_ADAS_LDW_MAX,
}IPC_ADAS_LDW_ENUM;

typedef enum
{
	IPC_ADAS_LKA_VIEW_OFF,
	IPC_ADAS_LKA_VIEW_1,
	IPC_ADAS_LKA_VIEW_2,   
	IPC_ADAS_LKA_VIEW_3,  
	IPC_ADAS_LKA_VIEW_4,
	IPC_ADAS_LKA_VIEW_5,
	IPC_ADAS_LKA_VIEW_6,   
	IPC_ADAS_LKA_VIEW_7,  
	IPC_ADAS_LKA_MAX,
}IPC_ADAS_LKA_ENUM;

typedef enum
{
	IPC_ADAS_ACC_VIEW_OFF,
	IPC_ADAS_ACC_VIEW_1,
	IPC_ADAS_ACC_VIEW_2,   
	IPC_ADAS_ACC_VIEW_3,  
	IPC_ADAS_ACC_VIEW_4,
	IPC_ADAS_ACC_VIEW_5,
	IPC_ADAS_ACC_VIEW_6,   
	IPC_ADAS_ACC_VIEW_7,  
	IPC_ADAS_ACC_VIEW_8, 
	IPC_ADAS_ACC_MAX,
}IPC_ADAS_ACC_ENUM;



#endif
