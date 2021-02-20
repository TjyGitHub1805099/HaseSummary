/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : DataPool.c
**
**          Date  : 2017-05-24
**          
**          
******************************************************************************/


/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "dea.h"
#include "odo.h"
#include "FreeRTOS.h"
//#include "SysReset.h"
//#include "DimmControlCfg.h"
#include "string.h"
#include "NvM.h"
#include "MemMap.h"




/******************************************************************************
**    MACROS
******************************************************************************/


/******************************************************************************
**    VARIABLE DEFINITIONS
******************************************************************************/
extern uint32_t g_BootLoaderJumpCmd;
extern uint32_t g_DeaResetMagicWord;

RST_TYPE_ENUM g_SysRestType = RST_IRRST;
IGN_STAT_ENUM g_IgnStat = IGN_OFF;
ACC_STAT_ENUM g_AccStat = DEA_ACC_ON;
uint8_t g_DEA_TerminalErrorStat = 0;
uint8_t g_DEA_TerminalErrorTimerExpire = 0;
ENGINE_START_STAT_ENUM g_EngineStartStat = ENGINE_START_OFF;
uint8_t g_RawVehicleSpeedStat = 0U;
uint16_t g_RawVehicleSpeed 	    = 0U;
uint16_t g_RawVehicleSpeedCanOutput 	= 0U;
uint16_t g_RawVehicleSpeedDiag 	= 0U;
uint8_t g_VehicleSpeedDiagMode = 0U;
uint16_t g_FilterVehicleSpeedDiag 	= 0U;
uint16_t g_FilterVehicleSpeed 	= 0U;
uint8_t g_FilterVehicleSpeedValid = 0U;
//Revolution
uint16_t g_RawEngineRevolutionDiag = 0U;
uint8_t g_EngineRevolutionDiagMode = 0U;
uint8_t g_RawEngineRevolutionErrorStat = 0U;
uint8_t g_RawEngineRevolutionErrorStat_3EC = 0U;	//591 project require
uint16_t g_RawEngineRevolution = 0U;
uint16_t g_RawEngineRevolution_3EC = 0U;	//591 project require
uint16_t g_FilterEngineRevolution = 0U;
uint8_t g_FilterEngineRevolutionValid = 0U;
uint8_t g_EngineRunningStat = 0U;
uint8_t g_RawEngine3EC_Flag = 0U;	//591 project require
uint8_t g_RawRevolutionIdleStat = 0U;	//591 project require,idle status
uint8_t g_RawRevolutionIdleRequest = 0U;	//591 project require,idle request
uint8_t g_RawIdleStopStatus = 0U;	//591 project require,idle stop status -> idle stop time

/**591 project require idle passage & idlefuel**/
uint8_t g_FilterCumulativeValue_H = 0U;
uint8_t g_FilterCumulativeValue_M = 0U;
uint8_t g_FilterCumulativeValue_S = 0U;
uint8_t g_FilterlTripValue_H = 0U;
uint8_t g_FilterlTripValue_M = 0U;
uint8_t g_FilterlTripValue_S = 0U;

uint32_t g_FilterfuelCumulativeVal = 0U;
uint32_t g_FilterfuelLtripVal = 0U;


//travel time
/**hour**/
uint8_t g_FilterTravelTime_H = 0U;	//591 project require
/**minume**/
uint8_t g_FilterTravelTime_M = 0U;	//591 project require
/**second**/
uint8_t g_FilterTravelTime_S = 0U;	//591 project require
/**timevalid**/
uint8_t g_FilterTravelTimeValid = 0U;	//591 project require

//Accel Guide
uint8_t g_RawAccelGuide_X = 0U;		//591 project require
uint8_t g_FilterAccelGuideValue = 0U;	//591 project require
uint8_t g_FilterAccelGuideValValid = 0U;
uint8_t g_FilterAccelGuide_Y = 0U;	//591 project require(Threshold Y(V))

//ECO_rank
uint8_t g_FilterEcoRankValue = 0U;	//591 project require
uint8_t g_FilterEcoRankValid = 0U;	//591 project require
uint8_t g_FilterEcoContion = 0U;	//591 project require
uint8_t g_RawEcoShiftPosition = 0U;//591 project require
uint8_t g_RawEcoModeValue = 0U;	//591 project require

//H_AFC
HIS_AFC	g_FilterHistoryAfc = {0};
uint8_t g_FilterShowFlag = 0U;
uint16_t g_FilterBestAfc = 0U;

uint16_t  g_CoolantTempRaw	= 0U;
uint8_t  g_CoolantTempRawValid	= 0U;
uint8_t  g_IFC_Valid = 0U;
uint8_t  g_IFC_Unit = 0U;
uint16_t g_IFC_Value = 0U;
uint8_t  g_AFC_A_Valid = 0U;
uint16_t g_AFC_A_Value = 0U;
uint8_t  g_AFC_B_Valid = 0U;
uint16_t g_AFC_B_Value = 0U;
uint8_t  g_CoolantTempValid	= 0U;
int32_t  g_CoolantTemp	= 0U;
uint8_t  g_CoolantSeg	= 0U;
uint16_t g_CoolantIndAngle = 0U;	//591 project require   Cti indication Angle
uint16_t g_Millage 			= 0U;
uint32_t g_IcMillage 		= 0U;
uint32_t g_BCM_ODO			= 0xFFFF;
uint32_t g_BCM_ODO_CrcMatch = 0U;
uint32_t g_TripA 			= 0U;
uint32_t g_TripB 			= 0U;
uint8_t	 g_OAT_Valid		= 0U;
uint8_t	 g_OAT_RawValid		= 0U;
int16_t	 g_OAT_RawValue		= 0U;
uint8_t	 g_OAT_CanRawValue	= 0U;
uint8_t	 g_OAT_RawResValid	= 0U;
uint16_t g_OAT_RawResValue	= 0U;
int16_t	 g_OAT_FilterValue	= 0U;
uint8_t  g_SpeedLimitAlarm	= 0U;
uint16_t  g_SpeedLimitValue	= 0U;
uint16_t g_DEA_InstFuelConsmpRate = 0U;

BUTTON_STAT_ENUM  g_DEA_ButtonStat[DEA_BUTTON_NUM] = {0};
uint8_t g_DEA_BCM_VINValid = 0;
uint8_t g_DEA_BCM_VIN[7] = {0};
uint8_t g_DEA_IPC_VIN[7] = {0};

WARN_MSG_STRU g_ICWarnMsgBuf[10] = {0};
uint8_t g_ICWarnMsgCnt = 0;
uint8_t g_DME_CrcMatch = 1;
uint8_t g_EPBI_CrcMatch = 1;
uint8_t g_DEA_SBRSoundRequestStat = 0;

DEA_FUEL_SIG_ENUM g_DEA_FuelSigStat = DEA_FUEL_SIG_NORMAL;
DEA_OAT_SIG_ENUM g_DEA_OatSigStat = DEA_OAT_SIG_NORMAL;
DEA_LVDS_STAT_ENUM g_DEA_LvdsStat = DEA_LVDS_NORMAL;


uint8_t					g_CruiseControl_TAR_V_CC = 0U;
uint8_t 				DEA_MHU_TimeUpdatedInd = 0;
uint8_t 				DEA_MHU_TimeValid = 0;
MHU_TIME_STRU			DEA_MHU_Time = {0};
TIME_STRU				g_Time	= {0};
TIME_FORMAT_ENUM		g_TimeFormat = TIME_24H;
TIME_FORMAT_ENUM		g_MHUTimeFormat = TIME_24H;
uint8_t					g_DEA_DimmingSettingOkFlag = 0;
uint8_t 				g_DEA_DimmingFromMHURspInd = 0;
uint8_t 				g_DEA_DimmingMsgToMHUFlag = 0;
DIMMING_MHU_STRU 		g_DimmingVehicleMUH = {0};
DIMMING_OFFSET_IPC_STRU	g_DimmingOffsetIPC = {0};
STAT_POS_LIGHT_ENUM		g_PosLight = POSITION_LIGHT_OFF;
IC_DIMMING_STRU 		g_IC_Dimming = {0};
int16_t 				g_PhotoSensorValue = 0U;
uint16_t 				g_IC_TempEx = 0U;
uint16_t 				g_IC_TempExUnfilt = 0U;
BOARD_BUTTON_ENUM 		g_ButtonStat = BUTTON_NONE;
uint8_t 				g_DriveModeButtonStat = 0U;
uint8_t 				g_BordComputerButtonStat = 0U;
uint8_t 				g_PhoneButton = 0;
uint8_t 				g_FuelInjectionValid = 0U;
uint8_t					g_FuelInjectionstat = 0U;
uint16_t 				g_FuelInjection = 0U;
uint16_t 				g_DEA_DTE = 0U;
uint8_t					g_DEA_DTE_Valid = 0U;
uint8_t					g_DEA_TII_Valid = 0U;
uint16_t				g_DEA_TII_Value = 0U;
LANGUAGE_ENUM 			g_LanguageSet = LANGUAGE_ENGLISH;
LANGUAGE_ENUM 			g_MHULanguageSet = LANGUAGE_ENGLISH;
uint8_t 				g_DEA_UnitChangedFlagForCan = 0;
//uint16_t 				g_ServiceInterval = 0U;
TIRE_STAT_STRU 			g_TireStat = {0};
uint8_t 				g_TireFault = 0;
uint8_t					g_DISP_GR_GRB_Sig = 0U;
uint8_t					g_DISP_RQ_GR_GRB_Sig = 0U;
uint8_t					g_DISP_PO_GRB_Sig = 0U;
uint8_t					g_DISP_PRG_GRB_Sig = 0U;
uint8_t					g_DISP_GR_PT_Sig = 0U;
uint8_t					g_DISP_GSI_DRV_Sig = 0U;
uint8_t					g_DISP_TARG_DRV_Sig = 0U;
GEAR_ENUM 				g_Gear = GEAR_NONE;
GEAR_ENUM 				g_TargetGear = GEAR_NONE;
GEAR_SHIFT_ENUM 		g_GearShiftStat = GearShift_None;


uint8_t 				g_DEA_TransGearValid = 0;
uint8_t 				g_DEA_TransGearIO = 0;
uint8_t 				g_DEA_TransCntrlValid = 0;
uint8_t 				g_DEA_TransCntrlMd = 0;
uint8_t 				g_DEA_TransCntlType = 0;

TURN_IND_STAT_ENUM		g_TurnIndStat = TURN_OFF;
TELLTALE_ENUM			g_HighBeamStat = TELLTALE_OFF;
TELLTALE_ENUM			g_LowBeamStat = TELLTALE_OFF;
TELLTALE_ENUM			g_FogLampFront = TELLTALE_OFF;
TELLTALE_ENUM 			g_FogLampRear  = TELLTALE_OFF;
TELLTALE_ENUM			g_PositionLight	= TELLTALE_OFF;
ICON_STAT_ENUM			g_Autolight		= ICON_OFF;
ICON_STAT_ENUM 			g_AutoWiping	= ICON_OFF;
TELLTALE_ENUM 			g_EPS_Stat		= TELLTALE_OFF;
TELLTALE_ENUM			g_ESC_Stat 		= TELLTALE_OFF;
TELLTALE_ENUM			g_ESC_OFF_Stat	= TELLTALE_OFF;
TELLTALE_ENUM 			g_HDC_Stat		= TELLTALE_OFF;
TELLTALE_ENUM			g_AirBagStat    = TELLTALE_OFF;
TELLTALE_ENUM			g_DSbeltStat    = TELLTALE_OFF;
TELLTALE_ENUM			g_PSbeltStat    = TELLTALE_OFF;
TELLTALE_ENUM			g_ABSStat    	= TELLTALE_OFF;
TELLTALE_ENUM			g_EBDStat    	= TELLTALE_OFF;
RADAR_STRU				g_RadarStat		= {0};
LDWS_STRU				g_LDWS_Stat		= {0};
uint8_t 				g_DEA_LDWSOnOffStat = 0;
DOOR_STAT_STRU			g_DoorStat		= {0};
SEAT_MEMORY_ENUM 		g_SeatMemory	= SEAT_MEMORY_OFF;
ETG_STATUS_STRU			g_ETG_Status	= {0};
TELLTALE_ENUM 			g_VehicleStopStat = TELLTALE_OFF;
TELLTALE_ENUM			g_ESC_ASR_Stat	= TELLTALE_OFF;
PDC_ENUM				g_BCM_PDC_Stat		= PDC_Disable;
PDC_ENUM				g_PDC_Stat		= PDC_Disable;
uint8_t					g_BSW_Stat		= 0U;
CRUISE_CONTROL_ENUM 	g_CruiseControl_ST_CC = CC_OFF;
WLC_STRU				g_WLC_Stat		= {0};

uint8_t 				g_DEA_FuelLevelPercentValid = 0;
uint16_t 				g_DEA_FuelLevelPercent = 0;

WARN_MSG_STRU			g_WarnMsg		= {0};
uint16_t 				g_compass		= 0U;
uint32_t                g_Display_Tt    = 0;
uint8_t                	g_AVS_A_Valid   = 0;
uint32_t                g_AVS_A         = 0;
uint8_t                	g_AVS_B_valid   = 0;
uint32_t                g_AVS_B         = 0;
uint8_t					g_CanTimeoutBuf[COM_IPDU_MAX_NUM] = {0};

DEA_MHU_IPC_LASTCALLS_STRU g_DEA_LastCall = {0};
DEA_MHU_IPC_MEDIA_STRU g_DEA_Media = {0};
DEA_MHU_IPC_PAIR_STATUS_ENUM g_DEA_PhonePairStatus = DEA_PhoneNotPaired;
DEA_MHU_IPC_TEL_STRU g_DEA_TELStatus = {0};
DEA_CALLING_TIME_STRU g_DEA_CallingTime = {0};
uint8_t g_DEA_NaviChangedFlag =  0;
DEA_NAVI_STRU g_DEA_NaviStatus = {0};
DEA_NAVI_FOR_IPC_STRU g_DEA_NaviForIPCStatus = {0};
DEA_COMPASS_FOR_IPC_STRU g_DEA_Compass = {0};
uint8_t g_IC_TelStatusUpdatedFlag = 0;
DEA_IC_MHU_TEL_STRU g_IC_TelStatus = {0};
uint32_t g_DEA_TTSecond = 0;

/********金康项目自加DEA接口****/
//电量表
uint8_t	                g_SocStat = 0;
uint8_t					g_SocValid = 0; //as return
uint8_t	            	g_RawSocValue = 0;
uint8_t	            	g_FilterSocValue = 0;

//功率表
int16_t					g_RawPpackPrValue = 0; //瞬时功耗
int16_t					g_FilterPpackPrValue = 0;
uint8_t	                g_PrmStat = 0;
uint8_t					g_PrmValid = 0; //as return

int16_t					g_AvpPackAValue = 0; //平均功耗A
uint8_t					g_AvpPackAValueValid = 0;

int16_t					g_AvpPackBValue = 0; //平均功耗B
uint8_t					g_AvpPackBValueValid = 0;


//VCU_Ready信号
uint8_t                 g_RawVcuReadyValue = 0;
uint8_t	                g_VcuReadyStat = 0;

//车速Valid信号
uint8_t					g_RawVchSpValidValue = 0;
uint8_t					g_VchSpValidStat = 0;

/**行车电脑数据信息**/
//行程时间-精度为1分钟
uint32_t				g_TripTimeA = 0;
uint32_t				g_TripTimeB = 0;
uint8_t					g_TripTimeAValid = 0;
uint8_t					g_TripTimeBValid = 0;


//续航里程
uint32_t				g_RawReMilgeValue = 0;
uint32_t				g_FilterReMilgeValue = 0;
uint8_t					g_ReMilgeValid = 0; //as return
uint8_t					g_ReMilgeStat = 0;

//电流表
int16_t 				g_RawIpackValue = 0;
int16_t 				g_FilterIpackValue = 0;
uint8_t					g_IpackValid = 0; //as return
uint8_t					g_IpackStat = 0;

//电压表
uint16_t 				g_RawVpackValue = 0;
uint16_t 				g_FilterVpackValue = 0;
uint8_t					g_VpackValid = 0; //as return
uint8_t					g_VpackStat = 0;

//电机转速
int32_t					g_RawMspValue = 0;
int32_t					g_FilterMspValue = 0;
uint8_t					g_MspValid = 0; //as return
uint8_t					g_MspStat = 0;

//Gear 											//591 project require gear value
uint8_t					g_RawGearValue = 0;
uint8_t					g_FilterGearValue = 0;
uint8_t					g_GearValueValid = 0; //as return(给上层判断传上去的数据是否有效)
uint8_t					g_GearStat = 0;

uint8_t					g_GearATCheakValue = 0;
uint8_t					g_GearIdicateValue = 0;
uint8_t					g_GearBlinkValue = 0;

uint8_t 				g_GearRawSensor = 0;
uint8_t 				g_GearDataRecive = 0;


//按钮按键
/**按键状态-长按/短按/释放**/
#if 0
uint8_t					g_KEY_SW_SRCStat = 0;
uint8_t					g_KEY_SW_MENU_UPStat = 0;
uint8_t					g_KEY_SW_MENU_DOWNStat = 0;
uint8_t					g_KEY_SW_TEL_ONStat = 0;
uint8_t					g_KEY_SW_OKStat = 0;

uint8_t					g_KEY_SW_VOL_DOWNStat = 0;
uint8_t					g_KEY_SW_VOL_UPStat = 0;
uint8_t					g_KEY_SW_CAMERAStat = 0;
uint8_t					g_KEY_SW_MODEStat = 0;
uint8_t					g_KEY_SW_DISPStat = 0;
#else
uint8_t					g_KEY_StrgUpStat = 0; //yangxl
uint8_t					g_KEY_StrgDownStat = 0;
uint8_t					g_KEY_StrgLeftStat = 0;
uint8_t					g_KEY_StrgRightStat = 0;
uint8_t					g_KEY_StrgOkStat = 0;
uint8_t					g_KEY_StrgDispStat = 0;
uint8_t					g_KEY_StrgTelOnStat = 0;
uint8_t					g_KEY_StrgDvrStat = 0;
uint8_t					g_KEY_StrgLkaStat = 0;

#endif




uint8_t 				g_BmsChargingState = 0;
uint8_t 				g_BmsRequiredChargingTimeValid = 0;
uint16_t 				g_BmsRequiredChargingTime = 0;
uint8_t 				DeaIDSTSTS = 0;
uint32_t 				g_DeaHardwareStatus = 0;

static uint8_t s_CanUpdatedStat[COM_RX_IPDU_MAX_NUM/8+1] = {0};
static uint8_t s_CanReceivedIndStat[COM_RX_IPDU_MAX_NUM/8+1] = {0};
uint8_t 				g_DeaNightDimmerFlag = 0;
uint8_t 				g_DeaCLUSTER_ILL_REQ = 0;
uint8_t 				g_DeaEngineStatusCopy = 0;
uint8_t 				g_DeaStaterControlStatus = 0;
uint8_t 				DeaEPB_WORKMODE = 0;
uint8_t 				DeaEPB_STAT = 0;
uint8_t 				DeaDynamicBrakingStatus = 0;
uint8_t					DeaISMSKSW = 0;
uint8_t 				DeaTempOutputMode = 0;
uint8_t 				DeaFuelCapReset = 0;
uint8_t 				DeaVDC_LAMP = 0;
uint8_t 				DeaCrankingStatus = 0;
uint16_t				DeaTACHO = 0;
uint16_t				DeaRawTACHO = 0;
uint8_t 				CanMsgtoIpcInitFlag = 0;
uint8_t 				DeaSHIFT_IND = 0;

uint16_t 				DeaFGAUGE = 0;
uint16_t				DeaFMad = 0;
uint8_t					DeaVoltageState = 0;

uint16_t g_VehicleSpeedOutPut 	    = 0U;

uint8_t DeaTPD_FR = 0u; /*yangxl */
uint8_t DeaTPD_FL = 0u; /*yangxl */
uint8_t DeaTPD_RR = 0u; /*yangxl */
uint8_t DeaTPD_RL = 0u; /*yangxl */
uint8_t Dea_ACCSPDREQ = 0u; /*yangxl */

uint8_t DeaTS_FR = 0; /*yangxl */
uint8_t DeaTS_FL = 0; /*yangxl */
uint8_t DeaTS_RR = 0; /*yangxl */
uint8_t DeaTS_RL = 0; /*yangxl */
uint8_t DeaD_STYLE = 0; /*yangxl */

uint8_t DeaREQ_DISP_MET = 0; /*yangxl */
uint8_t DeaSONAR_S_REQ = 0; /*yangxl */
uint8_t DeaRR_CEN = 0; /*yangxl */
uint8_t	DeaA_RR_CEN = 0; /*yangxl */
uint8_t	DeaRR_L = 0; /*yangxl */
uint8_t	DeaA_RR_L = 0; /*yangxl */
uint8_t	DeaRR_R = 0; /*yangxl */
uint8_t	DeaA_RR_R = 0; /*yangxl */
uint8_t	DeaSTS_DISTANCE = 0; /*yangxl */
	
uint8_t DearangeStatus_FR_Center = 0; /*yangxl */
uint8_t DearangeStatus_FR_Left = 0;/*yangxl */
uint8_t DearangeStatus_FR_Right = 0;/*yangxl */
uint8_t DeaA_FR_Center = 0;/*yangxl */
uint8_t DeaA_FR_CorLeft = 0;/*yangxl */
uint8_t DeaA_FR_ConrRight = 0;/*yangxl */
uint8_t DeaDetectDisplayRequest = 0;/*yangxl */
uint8_t DeaECO_DISP_RQ = 0;/*yangxl */
uint8_t DeaGSINXTGP = 0;/*yangxl */

uint8_t DeaPEBS_INFO = 0;/*yangxl */
uint8_t DeaWARN_VRU = 0;/*yangxl */
uint8_t DeaMSG_VDC = 0;/*yangxl */
uint8_t DeaPRE_WARN_FR = 0;/*yangxl */
uint8_t DeaAEB_ACTIVE = 0;/*yangxl */

uint8_t DeaWARN_LANE_L = 0;/*yangxl */
uint8_t DeaWARN_LANE_R = 0;/*yangxl */
uint8_t DeaLDW_INFO = 0;/*yangxl */

uint8_t DeaLTP_FR = 0;/*yangxl */
uint8_t DeaLTP_FL = 0;/*yangxl */
uint8_t DeaLTP_RR = 0;/*yangxl */
uint8_t DeaLTP_RL = 0;/*yangxl */

uint8_t DeaSPDBLREQ = 0;/*yangxl */
uint8_t DeaASCDON_DISP = 0;/*yangxl */

uint8_t DeaFT_FR = 0;/*yangxl */
uint8_t DeaFT_FL = 0;/*yangxl */
uint8_t DeaFT_RL = 0;/*yangxl */
uint8_t DeaFT_RR = 0;/*yangxl */

uint8_t DeaSRRL_SET = 0;/*yangxl */
uint8_t DeaFAILD_DT = 0;/*yangxl */
uint8_t DeaALT_ST = 0;/*yangxl */
uint8_t DeaBLOCK_DT = 0;/*yangxl */
uint8_t DeaALT_IND = 0;/*yangxl */
uint8_t DeaSRRR_SET_R = 0;/*yangxl */
uint8_t DeaFAILD_DT_R = 0;/*yangxl */
uint8_t DeaALT_ST_R = 0;/*yangxl */
uint8_t DeaBLOCK_DT_R = 0;/*yangxl */
uint8_t DeaALT_IND_R = 0;/*yangxl */
uint8_t DeaIpcRecoverFlag = 0;/*yangxl */

uint8_t DeaCAN_MASK = 0;/*yangxl */



ExternSTDType Dea_RtcTime = {0};



void DEA_SetVehicleSpeedOutPut(uint16_t temp)
{
	g_VehicleSpeedOutPut = temp;
}

uint16_t DEA_GetVehicleSpeedOutPut(void)
{
	return g_VehicleSpeedOutPut;
}

//best history afc

void DEA_SetFilterBestHistoryAfcValue(uint16_t temp)	//591 project require: Best History afc 
{ 
	g_FilterBestAfc = temp;
}
uint16_t DEA_GetFilterBestHistoryAfcValue(void)
{
	return g_FilterBestAfc;
}	


//History afc show valid

void DEA_SetFilterHistoryAfcShowValid(uint8_t temp)	//591 project require: History afc show valid
{ 
	g_FilterShowFlag = temp;
}
uint8_t DEA_GetFilterHistoryAfcShowValid(void)
{
	return g_FilterShowFlag;
}

//History afc

void DEA_SetFilterHistoryAfcValue(HIS_AFC temp)	//591 project require: History afc latest six
{
	g_FilterHistoryAfc = temp;
}
HIS_AFC DEA_GetFilterHistoryAfcValue(void)
{
	return g_FilterHistoryAfc;
}

uint16_t DEA_GetLastAfcValue(void)
{
	return g_FilterHistoryAfc.value_one;
}


//Idle Stop Status

void DEA_SetRawIdleStopStatusValue(uint8_t temp)	//591 project require: Idle Stop Status
{
	g_RawIdleStopStatus = temp;
}
uint8_t DEA_GetRawIdleStopStatusValue(void)
{
	return g_RawIdleStopStatus;
}

//Idle passing CumulativeValue
void DEA_SetFilterCumulativeValue_H(uint8_t temp)	//591 project require: Idle passing CumulativeValue
{
	g_FilterCumulativeValue_H = temp;
}
uint8_t DEA_GetFilterCumulativeValue_H(void)
{
	return g_FilterCumulativeValue_H;
}

void DEA_SetFilterCumulativeValue_M(uint8_t temp)	//591 project require: Idle passing CumulativeValue
{
	g_FilterCumulativeValue_M = temp;
}
uint8_t DEA_GetFilterCumulativeValue_M(void)
{
	return g_FilterCumulativeValue_M;
}

void DEA_SetFilterCumulativeValue_S(uint8_t temp)	//591 project require: Idle passing CumulativeValue
{
	g_FilterCumulativeValue_S = temp;
}
uint8_t DEA_GetFilterCumulativeValue_S(void)
{
	return g_FilterCumulativeValue_S;
}


//Idle passing lTripValue
void DEA_SetFilterlTripValue_H(uint8_t temp)	//591 project require: Idle passing lTripValue
{
	g_FilterlTripValue_H = temp;
}
uint8_t DEA_GetFilterlTripValue_H(void)
{
	return g_FilterlTripValue_H;
}

void DEA_SetFilterlTripValue_M(uint8_t temp)	//591 project require: Idle passing lTripValue
{
	g_FilterlTripValue_M = temp;
}
uint8_t DEA_GetFilterlTripValue_M(void)
{
	return g_FilterlTripValue_M;
}

void DEA_SetFilterlTripValue_S(uint8_t temp)	//591 project require: Idle passing lTripValue
{
	g_FilterlTripValue_S = temp;
}
uint8_t DEA_GetFilterlTripValue_S(void)
{
	return g_FilterlTripValue_S;
}


//idle fuelvalue
void DEA_SetFilterfuelCumulativeValue(uint32_t temp)	//591 project require: Idle fuel lTripValue
{
	g_FilterfuelCumulativeVal = temp;
}
uint32_t DEA_GetFilterfuelCumulativeValue(void)
{
	return g_FilterfuelCumulativeVal;
}

void DEA_SetFilterfuelLtripValue(uint32_t temp)	//591 project require: Idle fuel lTripValue
{
	g_FilterfuelLtripVal = temp;
}
uint32_t DEA_GetFilterfuelLtripValue(void)
{
	return g_FilterfuelLtripVal;
}


//ECO_rank 591

void DEA_SetFilterEcoRankValue(uint8_t temp)	//591 project require: ECO rate
{
	g_FilterEcoRankValue = temp;
}
uint8_t DEA_GetFilterEcoRankValue(void)
{
	return g_FilterEcoRankValue;
}

void DEA_SetFilterEcoRankValid(uint8_t temp)	
{
	g_FilterEcoRankValid = temp;
}
uint8_t DEA_GetSFilterEcoRankValid(void)
{
	return g_FilterEcoRankValid;
}

void DEA_SetFilterEcoConddition(uint8_t temp)	//591 project require: ECO condition 
{
	g_FilterEcoContion = temp;
}
uint8_t DEA_GetSFilterEcoCondition(void)
{
	return g_FilterEcoContion;
}

void DEA_SetRawEcoShiftPosition(uint8_t temp)	//591 project require: Eco Shift Position
{
	g_RawEcoShiftPosition = temp;
}
uint8_t DEA_GetRawEcoShiftPosition(void)
{
	return g_RawEcoShiftPosition;
}

void DEA_SetRawEcoMode(uint8_t temp)	 //591 project require: Eco Mode
{
	g_RawEcoModeValue = temp;
}
uint8_t DEA_GetRawEcoMode(void)
{
	return g_RawEcoModeValue;
}


//Accel Guide 591

void DEA_SetRawAccelGuide_X(uint8_t temp)	//591 project require
{
	g_RawAccelGuide_X = temp;
}
uint8_t DEA_GetRawAccelGuide_X(void)
{
	return g_RawAccelGuide_X;
}

void DEA_SetFilterAccelGuide_Y(uint8_t temp)	//591 project require
{
	g_FilterAccelGuide_Y = temp;
}
uint8_t DEA_GetFilterAccelGuide_Y(void)
{
	return g_FilterAccelGuide_Y;
}

void DEA_SetFilterAccelGuideValue(uint8_t temp)	//591 project require
{
	g_FilterAccelGuideValue = temp;
}
uint8_t DEA_GetFilterAccelGuideValue(void)
{
	return g_FilterAccelGuideValue;
}

void DEA_SetFilterAccelGuideValValid(uint8_t valid) //591 project require
{
	g_FilterAccelGuideValValid = valid;
}
uint8_t DEA_GetFilterAccelGuideValValid(void)
{
	return g_FilterAccelGuideValValid;
}


//Travel Time 591

void DEA_SetFilterTravelTime_H(uint8_t time)	//591 project require: hour
{
	g_FilterTravelTime_H = time;
}
uint8_t DEA_GetFilterTravelTime_H(void)
{
	return g_FilterTravelTime_H;
}
void DEA_SetFilterTravelTime_M(uint8_t time)	//591 project require: minume
{
	g_FilterTravelTime_M = time;
}
uint8_t DEA_GetFilterTravelTime_M(void)
{
	return g_FilterTravelTime_M;
}

void DEA_SetFilterTravelTime_S(uint8_t time)	//591 project require: second
{
	g_FilterTravelTime_S = time;
}
uint8_t DEA_GetFilterTravelTime_S(void)
{
	return g_FilterTravelTime_S;
}

void DEA_SetFilterTravelTimeValid(uint8_t valid)	//591 project require: Travel time valid
{
	g_FilterTravelTimeValid = valid;
}
uint8_t DEA_GetFilterTravelTimeValid(void)
{
	return g_FilterTravelTimeValid;
}


#if 0

/**591 project require**/

//stering A
void DEA_SetSRCBottonValue(uint8_t value)
{
	g_KEY_SW_SRCStat = value;
}
uint8_t DEA_GetSRCBottonValue()
{
	return	g_KEY_SW_SRCStat;
}


void DEA_SetMENU_UPBottonValue(uint8_t value)
{
	g_KEY_SW_MENU_UPStat = value;
}
uint8_t DEA_GetMENU_UPBottonValue()
{
	return	g_KEY_SW_MENU_UPStat;
}


void DEA_SetMENU_DOWNBottonValue(uint8_t value)
{
	g_KEY_SW_MENU_DOWNStat = value;
}
uint8_t DEA_GetMENU_DOWNBottonValue()
{
	return	g_KEY_SW_MENU_DOWNStat;
}


void DEA_SetTEL_ONBottonValue(uint8_t value)
{
	g_KEY_SW_TEL_ONStat = value;
}
uint8_t DEA_GetTEL_ONBottonValue()
{
	return	g_KEY_SW_TEL_ONStat;
}


void DEA_SetSW_OKBottonValue(uint8_t value)
{
	g_KEY_SW_OKStat = value;
}
uint8_t DEA_GetSW_OKBottonValue()
{
	return	g_KEY_SW_OKStat;
}


//stering B

void DEA_SetVOL_DOWNBottonValue(uint8_t value)
{
	g_KEY_SW_VOL_DOWNStat = value;
}
uint8_t DEA_GetVOL_DOWNBottonValue()
{
	return	g_KEY_SW_VOL_DOWNStat;
}

void DEA_SetVOL_UPBottonValue(uint8_t value)
{
	g_KEY_SW_VOL_UPStat = value;
}
uint8_t DEA_GetVOL_UPBottonValue()
{
	return	g_KEY_SW_VOL_UPStat;
}

void DEA_SetCAMERABottonValue(uint8_t value)
{
	g_KEY_SW_CAMERAStat = value;
}
uint8_t DEA_GetCAMERABottonValue()
{
	return	g_KEY_SW_CAMERAStat;
}

void DEA_SetMODEBottonValue(uint8_t value)
{
	g_KEY_SW_MODEStat = value;
}
uint8_t DEA_GetMODEBottonValue()
{
	return	g_KEY_SW_MODEStat;
}

void DEA_SetDISPBottonValue(uint8_t value)
{
	g_KEY_SW_DISPStat = value;
}
uint8_t DEA_GetDISPBottonValue()
{
	return	g_KEY_SW_DISPStat;
}
#else
void DEA_SetKeyStrgUpValue(uint8_t value)
{
	g_KEY_StrgUpStat = value;
}
uint8_t DEA_GetKeyStrgUpValue(void)
{
	return	g_KEY_StrgUpStat;
}

void DEA_SetKeyStrgDownValue(uint8_t value)
{
	g_KEY_StrgDownStat = value;
}
uint8_t DEA_GetKeyStrgDownValue(void)
{
	return	g_KEY_StrgDownStat;
}

void DEA_SetKeyStrgLeftValue(uint8_t value)
{
	g_KEY_StrgLeftStat = value;
}
uint8_t DEA_GetKeyStrgLeftValue(void)
{
	return	g_KEY_StrgLeftStat;
}

void DEA_SetKeyStrgRightValue(uint8_t value)
{
	g_KEY_StrgRightStat = value;
}
uint8_t DEA_GetKeyStrgRightValue(void)
{
	return	g_KEY_StrgRightStat;
}

void DEA_SetKeyStrgOkValue(uint8_t value)
{
	g_KEY_StrgOkStat = value;
}
uint8_t DEA_GetKeyStrgOkValue(void)
{
	return	g_KEY_StrgOkStat;
}

void DEA_SetKeyStrgDispValue(uint8_t value)
{
	g_KEY_StrgDispStat = value;
}
uint8_t DEA_GetKeyStrgDispValue(void)
{
	return	g_KEY_StrgDispStat;
}

void DEA_SetKeyStrgTelOnValue(uint8_t value)
{
	g_KEY_StrgTelOnStat = value;
}
uint8_t DEA_GetKeyStrgTelOnValue(void)
{
	return	g_KEY_StrgTelOnStat;
}

void DEA_SetKeyStrgDvrValue(uint8_t value)
{
	g_KEY_StrgDvrStat = value;
}
uint8_t DEA_GetKeyStrgDvrValue(void)
{
	return	g_KEY_StrgDvrStat;
}

void DEA_SetKeyStrgLkaValue(uint8_t value)
{
	g_KEY_StrgLkaStat = value;
}
uint8_t DEA_GetKeyStrgLkaValue(void)
{
	return	g_KEY_StrgLkaStat;
}






#endif



//行程时间

uint32_t DEA_GetTripTimeA()
{
	return g_TripTimeA;
}

void DEA_SetTripTimeA(uint32_t value)
{
	g_TripTimeA = value;
}

uint8_t DEA_GetTripTimeAValid()
{
	return g_TripTimeAValid;
}

void DEA_SetTripTimeAValid(uint8_t value)
{
	g_TripTimeAValid = value;
}

uint32_t DEA_GetTripTimeB()
{
	return g_TripTimeB;
}

void DEA_SetTripTimeB(uint32_t value)
{
	g_TripTimeB = value;
}

uint8_t DEA_GetTripTimeBValid()
{
	return g_TripTimeBValid;
}

void DEA_SetTripTimeBValid(uint8_t value)
{
	g_TripTimeBValid = value;
}



//续航里程

uint32_t DEA_GetRawReMilgeValue(void)
{
	return g_RawReMilgeValue;
}

void DEA_SetRawgReMilgeValue(uint32_t value)
{
	 g_RawReMilgeValue = value;
}

uint32_t DEA_GetFilterReMilgeValue(void)
{
	return g_FilterReMilgeValue;
}

void DEA_SetFilterReMilgeValue(uint32_t value)
{
	 g_FilterReMilgeValue = value;
}


void DEA_SetReMilgeErrorValueStat(uint8_t stat)
{
	g_ReMilgeStat = stat;
}

uint8_t DEA_GetReMilgeErrorValueStat(void)
{
	return g_ReMilgeStat;
}

void DEA_SetReMilgeValueValid(uint8_t valid)
{
	g_ReMilgeValid = valid;
}

uint8_t DEA_GetReMilgeValueValid(void)
{
	return g_ReMilgeValid;
}

//电流表
int16_t DEA_GetRawIpackValue(void)
{
	return g_RawIpackValue;
}

void DEA_SetRawIpackValue(int16_t value)
{
	 g_RawIpackValue = value;
}

int16_t DEA_GetFilterIpackValue(void)
{
	return g_FilterIpackValue;
}

void DEA_SetFilterIpackValue(int16_t value)
{
	 g_FilterIpackValue = value;
}


void DEA_SetIpackErrorValueStat(uint8_t stat)
{
	g_IpackStat = stat;
}

uint8_t DEA_GetIpackErrorValueStat(void)
{
	return g_IpackStat;
}

void DEA_SetIpackValueValid(uint8_t valid)
{
	g_IpackValid = valid;
}

uint8_t DEA_GetIpackValueValid(void)
{
	return g_IpackValid;
}

//电压表
uint16_t DEA_GetRawVpackValue(void)
{
	return g_RawVpackValue;
}

void DEA_SetRawVpackValue(uint16_t value)
{
	 g_RawVpackValue = value;
}

uint16_t DEA_GetFilterVpackValue(void)
{
	return g_FilterVpackValue;
}

void DEA_SetFilterVpackValue(uint16_t value)
{
	 g_FilterVpackValue = value;
}


void DEA_SetVpackErrorValueStat(uint8_t stat)
{
	g_VpackStat = stat;
}

uint8_t DEA_GetVpackErrorValueStat(void)
{
	return g_VpackStat;
}

void DEA_SetVpackValueValid(uint8_t valid)
{
	g_VpackValid = valid;
}

uint8_t DEA_GetVpackValueValid(void)
{
	return g_VpackValid;
}

 
//电机转速
int32_t DEA_GetRawMspValue(void)
{
	return g_RawMspValue;
}

void DEA_SetRawMspValue(int32_t value)
{
	 g_RawMspValue = value;
}

int32_t DEA_GetFilterMspValue(void)
{
	return g_FilterMspValue;
}

void DEA_SetFilterMspValue(int32_t value)
{
	 g_FilterMspValue = value;
}


void DEA_SetMspErrorValueStat(uint8_t stat)
{
	g_MspStat = stat;
}

uint8_t DEA_GetMspErrorValueStat(void)
{
	return g_MspStat;
}

void DEA_SetMspValueValid(uint8_t valid)
{
	g_MspValid = valid;
}

uint8_t DEA_GetMspValueValid(void)
{
	return g_MspValid;
}


//电量表接口
uint8_t DEA_GetRawSocValue(void)
{
	return g_RawSocValue;
}

void DEA_SetRawSocValue(uint8_t value)
{
	 g_RawSocValue = value;
}


uint8_t DEA_GetFilterBmsSocValue(void)
{
	return g_FilterSocValue;
}

void DEA_SetFilterBmsSocValue(uint8_t value)
{
	g_FilterSocValue = value;
}


void DEA_SetSocErrorValueStat(uint8_t stat)
{
	g_SocStat = stat;
}

uint8_t DEA_GetSocErrorValueStat(void)
{
	return g_SocStat;
}

void DEA_SetSocValueValid(uint8_t valid)
{
	g_SocValid = valid;
}

uint8_t DEA_GetSocValueValid(void)
{
	return g_SocValid;
}


//功率表接口
int16_t DEA_GetRawPpackValue(void)
{
	return g_RawPpackPrValue;
}

void DEA_SetRawPpackValue(int16_t value)
{
	 g_RawPpackPrValue = value;
}


int16_t DEA_GetFilterPpackValue(void)
{
	 return g_FilterPpackPrValue;
}

void DEA_SetFilterPpackValue(int16_t value)
{
	 g_FilterPpackPrValue = value;
}


void DEA_SetPrmErrorValueStat(uint8_t stat)
{
	g_PrmStat = stat;
}

uint8_t DEA_GetPrmErrorValueStat(void)
{
	return g_PrmStat;
}

void DEA_SetPrmValueValid(uint8_t valid)
{
	g_PrmValid = valid;
}

uint8_t DEA_GetPrmValueValid(void)
{
	return g_PrmValid;
}

int16_t DEA_GetAvpPackAValue() /**平均功率A**/
{
	return g_AvpPackAValue;
}

void DEA_SetAvpPackAValue(int16_t value)
{
	g_AvpPackAValue = value;
}

void DEA_SetAvpPackAValueValid(uint8_t valid)
{
	g_AvpPackAValueValid = valid;
}

uint8_t DEA_GetAvpPackAValueValid(void)
{
	return g_AvpPackAValueValid;
}
int16_t DEA_GetAvpPackBValue() /**平均功率B**/
{
	return g_AvpPackBValue;
}

void DEA_SetAvpPackBValue(int16_t value)
{
	g_AvpPackBValue = value;
}

void DEA_SetAvpPackBValueValid(uint8_t valid)
{
	g_AvpPackBValueValid = valid;
}

uint8_t DEA_GetAvpPackBValueValid(void)
{
	return g_AvpPackBValueValid;
}



//skon_s106 VCU_ready signal

void DEA_SetRawVcuReadyValue(uint8_t value)
{
	g_RawVcuReadyValue = value;
}

uint8_t DEA_GetRawVcuReadyValue(void)
{
	return g_RawVcuReadyValue;
}

void DEA_SetRawVcuReadyErrStat(uint8_t stat) 
{
	g_VcuReadyStat = stat;
}

uint8_t DEA_GetRawVcuReadyErrStat(void)
{
	return g_VcuReadyStat;
}

//skon_s106 project vehcile speed value valid
void DEA_SetRawVchSpValidValue(uint8_t value)
{
	g_RawVchSpValidValue = value;

}
uint8_t DEA_GetRawVchSpValidValue()
{
	return g_RawVchSpValidValue;
}

void DEA_SetVchSpValidErrStat(uint8_t value)
{
	g_VchSpValidStat = value;
}
uint8_t DEA_GetVchSpValidErrStat()
{
	return g_VchSpValidStat;
}



/**geer value**/
uint8_t DEA_GetRawGearValue(void)
{
	return g_RawGearValue;
}

void DEA_SetRawGearValue(uint8_t value)
{
	 g_RawGearValue = value;
}


uint8_t DEA_GetFilterGearValue(void)
{
	return g_FilterGearValue;
}

void DEA_SetFilterGearValue(uint8_t value)
{
	g_FilterGearValue = value;
}


void DEA_SetGearErrorValueStat(uint8_t stat)
{
	g_GearStat = stat;
}

uint8_t DEA_GetGearErrorValueStat(void)
{
	return g_GearStat;
}

void DEA_SetFilterGearValueValid(uint8_t valid)
{
	g_GearValueValid = valid;
}

uint8_t DEA_GetFilterGearValueValid(void)
{
	return g_GearValueValid;
}


void DEA_SetRawGearATCheakValue(uint8_t value)	//AT cheak
{
	g_GearATCheakValue = value;
}

uint8_t DEA_GetRawGearATCheakValue(void)
{
	return g_GearATCheakValue;
}


void DEA_SetRawGearIdicateAllow(uint8_t value)	//AT indicate allow
{
	g_GearIdicateValue = value;
}

uint8_t DEA_GetRawGearIdicateAllow(void)
{
	return g_GearIdicateValue;
}


uint8_t DEA_GetRawGearBlinkValue(void)	//AT blink allow
{
	return g_GearBlinkValue;
}

void DEA_SetRawGearBlinkValue(uint8_t value)
{
	g_GearBlinkValue = value;
}

uint8_t DEA_GetGearRawSensorValue(void)	//RawSensor value
{
	return g_GearRawSensor;
}

void DEA_SetGearRawSensorValue(uint8_t value)
{
	g_GearRawSensor = value;
}


uint8_t DEA_GetGearDataRecive(void) //judge if ever once recived the ID:0x421
{
	return g_GearDataRecive;
}
	
void DEA_SetGearDataRecive(uint8_t value)
{
	g_GearDataRecive = value;
}


/**********************************************************/
void DEA_SetBootLoaderJumpCmd(uint32_t value)
{
	g_BootLoaderJumpCmd = value;
}

uint32_t DEA_GetBootLoaderJumpCmd(void)
{
	return g_BootLoaderJumpCmd;
}

void DEA_SetResetMagicWord(uint32_t value)
{
	g_DeaResetMagicWord = value;
}

uint32_t DEA_GetResetMagicWord(void)
{
	return g_DeaResetMagicWord;
}


void DEA_SetSysRstType(RST_TYPE_ENUM stat)
{
    g_SysRestType = stat;
}


RST_TYPE_ENUM DEA_GetSysRstType(void)
{
	//g_SysRestType = Sys_GetResetType();
    return g_SysRestType;
}

void DEA_SetIgnStat(IGN_STAT_ENUM stat)
{
	g_IgnStat = stat;
}

IGN_STAT_ENUM DEA_GetIgnStat(void)
{
	return g_IgnStat;
}

void DEA_SetAccStat(ACC_STAT_ENUM stat)
{
	g_AccStat = stat;
}

ACC_STAT_ENUM DEA_GetAccStat(void)
{
	return g_AccStat;
}

void DEA_SetTerminalErrorStat(uint8_t stat)
{
	g_DEA_TerminalErrorStat = stat;
}

uint8_t DEA_GetTerminalErrorStat(void)
{
	return g_DEA_TerminalErrorStat;
}

void DEA_SetTerminalErrorTimerExpire(uint8_t stat)
{
	g_DEA_TerminalErrorTimerExpire = stat;
}

uint8_t DEA_GetTerminalErrorTimerExpire(void)
{
	return g_DEA_TerminalErrorTimerExpire;
}


void DEA_SetEngineStartStat(ENGINE_START_STAT_ENUM stat)
{
	g_EngineStartStat = stat;
}

ENGINE_START_STAT_ENUM DEA_GetEngineStartStat(void)
{
	return g_EngineStartStat;
}


void DEA_SetRawVehicleSpeedErrorStat(uint8_t stat)
{
	g_RawVehicleSpeedStat = stat;
}


uint8_t DEA_GetRawVehicleSpeedErrorStat(void)
{
	return g_RawVehicleSpeedStat;
}

void DEA_SetRawVehicleSpeedDiag(uint16_t speed)
{
	g_RawVehicleSpeedDiag = speed;
}
uint16_t DEA_GetRawVehicleSpeedDiag(void)
{
	return g_RawVehicleSpeedDiag;
}


void DEA_SetVehicleSpeedDiagMode(uint8_t mode)
{
	g_VehicleSpeedDiagMode = mode;
}

uint8_t DEA_GetVehicleSpeedDiagMode(void)
{
	return g_VehicleSpeedDiagMode;
}

void DEA_SetRawVehicleSpeed(uint16_t speed)
{
	g_RawVehicleSpeed = speed;
}
uint16_t DEA_GetRawVehicleSpeed(void)
{
	return g_RawVehicleSpeed;
}
//vehicle speed value 
void DEA_SetFilterVehicleSpeed(uint16_t speed)
{
	g_FilterVehicleSpeed = speed;
}
uint16_t DEA_GetRawVehicleSpeedCanOutput(void)
{
	return g_RawVehicleSpeedCanOutput;
}
//vehicle speed value 
void DEA_SetRawVehicleSpeedCanOutput(uint16_t speed)
{
	g_RawVehicleSpeedCanOutput = speed;
}

uint16_t DEA_GetFilterVehicleSpeed(void)
{
	return g_FilterVehicleSpeed;
}
//vehicle speed value valid
void DEA_SetFilterVehicleSpeedValid(uint8_t valid)
{
	g_FilterVehicleSpeedValid = valid;
}
uint8_t DEA_GetFilterVehicleSpeedValid(void)
{
	return g_FilterVehicleSpeedValid;
}

void DEA_SetFilterVehicleSpeedDiag(uint16_t speed)
{
	g_FilterVehicleSpeedDiag = speed;
}
uint16_t DEA_GetFilterVehicleSpeedDiag(void)
{
	return g_FilterVehicleSpeedDiag;
}


void DEA_SetSpeedLimitAlarm(uint8_t stat)
{
	g_SpeedLimitAlarm = stat;
}

uint8_t DEA_GetSpeedLimitAlarm(void)
{
	return g_SpeedLimitAlarm;
}

void DEA_SetSpeedLimitValue(uint16_t stat)
{
	g_SpeedLimitValue = stat;
}

uint16_t DEA_GetSpeedLimitValue(void)
{
	return g_SpeedLimitValue;
}

void DEA_SetInstFuelConsmpRate(uint16_t value)
{
	g_DEA_InstFuelConsmpRate = value;
}

uint16_t DEA_GetInstFuelConsmpRate(void)
{
	return g_DEA_InstFuelConsmpRate;
}

//Revolution
void DEA_SetRawEngineRevolutionErrorStat(uint8_t stat)
{
	g_RawEngineRevolutionErrorStat = stat;
}

uint8_t DEA_GetRawEngineRevolutionErrorStat(void)
{
	return g_RawEngineRevolutionErrorStat;
}

void DEA_SetRawEngineRevolutionErrorStat_3EC(uint8_t stat)	//591 project require
{
	g_RawEngineRevolutionErrorStat_3EC = stat;
}

uint8_t DEA_GetRawEngineRevolutionErrorStat_3EC(void)	//591 project require
{
	return g_RawEngineRevolutionErrorStat_3EC;
}

void DEA_SetRawRevolutionIdleStat(uint8_t stat)	//591 project require
{
	g_RawRevolutionIdleStat = stat;
}

uint8_t DEA_GetRawRevolutionIdleStat(void)	//591 project require
{
	return g_RawRevolutionIdleStat;
}

void DEA_SetRawRevolutionIdleRequest(uint8_t stat)	//591 project require
{
	g_RawRevolutionIdleRequest = stat;
}

uint8_t DEA_GetRawRevolutionIdleRequest(void)	//591 project require
{
	return g_RawRevolutionIdleRequest;
}


void DEA_SetEngineRevolutionDiagMode(uint8_t mode)
{
	g_EngineRevolutionDiagMode = mode;
}

uint8_t DEA_GetEngineRevolutionDiagMode(void)
{
	return g_EngineRevolutionDiagMode;
}


void DEA_SetRawEngineRevolutionDiag(uint16_t engine)
{
	g_RawEngineRevolutionDiag = engine;
}

uint16_t DEA_GetRawEngineRevolutionDiag(void)
{
	return g_RawEngineRevolutionDiag;
}


void DEA_SetRawEngineRevolution(uint16_t engine)
{
	g_RawEngineRevolution = engine;
}

uint16_t DEA_GetRawEngineRevolution(void)
{
	return g_RawEngineRevolution;
}

void DEA_SetFilterEngineRevolution(uint16_t engine)
{
	g_FilterEngineRevolution = engine;
}

uint16_t DEA_GetFilterEngineRevolution(void)
{
	return g_FilterEngineRevolution;
}
void DEA_SetRawEngineRevolution_3EC(uint16_t engine)	//591 project require
{
	g_RawEngineRevolution_3EC = engine;
}

uint16_t DEA_GetRawEngineRevolution_3EC(void)	//591 project require
{
	return g_RawEngineRevolution_3EC;
}

void DEA_SetFilterEngineRevolutionValid(uint8_t valid)
{
	g_FilterEngineRevolutionValid = valid;
}

uint8_t DEA_GetFilterEngineRevolutionValid(void)
{
	return g_FilterEngineRevolutionValid;
}


uint8_t DEA_GetEngineRunningStat(void)
{
	return g_EngineRunningStat;
}

void DEA_SetEngineRunningStat(uint8_t stat)
{
	g_EngineRunningStat = stat;
}

uint8_t DEA_GetRawEngine3ECStat(void)		//591 project require
{
	return g_RawEngine3EC_Flag;
}

void DEA_SetRawEngine3ECStat(uint8_t stat)		//591 project require
{
	g_RawEngine3EC_Flag = stat;
}


//CoolLant
void DEA_SetCoolantTempRaw(uint16_t CoolTemp)
{
	g_CoolantTempRaw = CoolTemp;
}

uint16_t DEA_GetCoolantTempRaw(void)
{
	return g_CoolantTempRaw;
}

//CoolLant
void DEA_SetCoolantTempRawValid(uint8_t stat)
{
	g_CoolantTempRawValid = stat;
}

uint8_t DEA_GetCoolantTempRawValid(void)
{
	return g_CoolantTempRawValid;
}

void DEA_SetButtonStatus(DEA_BUTTON_ENUM index, BUTTON_STAT_ENUM stat)
{
	g_DEA_ButtonStat[index] = stat;
}


BUTTON_STAT_ENUM DEA_GetButtonStatus(DEA_BUTTON_ENUM index)
{
	return g_DEA_ButtonStat[index];
}


void DEA_SetCoolantTempValid(uint8_t valid)
{
	g_CoolantTempValid = valid;
}

uint8_t DEA_GetCoolantTempValid(void)
{
	return g_CoolantTempValid;
}


void DEA_SetCoolantTemp(int32_t CoolTemp)
{
	g_CoolantTemp = CoolTemp;
}

int32_t DEA_GetCoolantTemp(void)
{
	return g_CoolantTemp;
}

void DEA_SetCoolantIndAngle(uint16_t angle)	//591 project require
{
	g_CoolantIndAngle = angle;
}

uint16_t DEA_GetCoolantIndAngle(void)
{
	return g_CoolantIndAngle;
}


void DEA_SetCoolantSeg(uint8_t CoolTemp)
{
	g_CoolantSeg= CoolTemp;
}

uint8_t DEA_GetCoolantSeg(void)
{
	return g_CoolantSeg;
}

//AFC
void DEA_SetIFC_Valid(uint8_t valid)
{
	g_IFC_Valid = valid;
}

uint8_t DEA_GetIFC_Valid(void)
{
	return g_IFC_Valid;
}

void DEA_SetIFC_Unit(uint8_t unit)
{
	g_IFC_Unit = unit;
}

uint8_t DEA_GetIFC_Unit(void)
{
	return g_IFC_Unit;
}

void DEA_SetIFC_Vaule(uint16_t value)
{
	g_IFC_Value = value;
}

uint16_t DEA_GetIFC_Value(void)
{
	return g_IFC_Value;
}


void DEA_SetAFC_A_Valid(uint8_t valid)
{
	g_AFC_A_Valid = valid;
}

uint8_t DEA_GetAFC_A_Valid(void)
{
	return g_AFC_A_Valid;
}

void DEA_SetAFC_A_Vaule(uint16_t value)
{
	g_AFC_A_Value = value;
}

uint16_t DEA_GetAFC_A_Value(void)
{
	return g_AFC_A_Value;
}

void DEA_SetAFC_B_Valid(uint8_t valid)
{
	g_AFC_B_Valid = valid;
}

uint8_t DEA_GetAFC_B_Valid(void)
{
	return g_AFC_B_Valid;
}

void DEA_SetAFC_B_Vaule(uint16_t value)
{
	g_AFC_B_Value = value;
}

uint16_t DEA_GetAFC_B_Value(void)
{
	return g_AFC_B_Value;
}

//ODO
void DEA_SetCanMileage(uint16_t millage)
{
	g_Millage = millage;
}

uint16_t DEA_GetCanMillage(void)
{
	return g_Millage;
}

void DEA_SetBcmOdo(uint32_t odo)
{
	g_BCM_ODO = odo;
}

uint32_t DEA_GetBcmOdo(void)
{
	return g_BCM_ODO;
}

void DEA_SetBCM_ODO_CrcMatch(uint8_t stat)
{
	g_BCM_ODO_CrcMatch = stat;
}


uint8_t DEA_GetBCM_ODO_CrcMatch(void)
{
	return g_BCM_ODO_CrcMatch;
}


void DEA_SetIcMileage(uint32_t millage)
{
	g_IcMillage = millage;
}


uint32_t DEA_GetIcMillage(void)
{
	return g_IcMillage;
}

uint32_t DEA_GetTripA(void)
{
	return ODO_GetTripA();
}

uint32_t DEA_GetTripB(void)
{
	return ODO_GetTripB();
}

uint32_t DEA_GetOdo(void)
{
	return ODO_GetOdo();
}

uint8_t DEA_GetOdoValid(void)
{
	return ODO_GetOdoValid();
}

uint8_t DEA_GetTripA_Status(void)
{
	return ODO_GetTripA_Status();
}


uint8_t DEA_GetTripB_Status(void)
{
	return ODO_GetTripB_Status();
}

//OAT
void DEA_SetOAT_Valid(int16_t value)
{
	g_OAT_Valid = value;
}

int16_t DEA_GetOAT_Valid(void)
{
	return g_OAT_Valid;
}

void DEA_SetOAT_RawValid(uint8_t valid)
{
	g_OAT_RawValid = valid;
}

int16_t DEA_GetOAT_RawValid(void)
{
	return g_OAT_RawValid;
}


void DEA_SetOAT_RawValue(int16_t value)
{
	g_OAT_RawValue = value;
}

int16_t DEA_GetOAT_RawValue(void)
{
	return g_OAT_RawValue;
}


void DEA_SetOAT_CanRawValue(uint8_t value)
{
	g_OAT_CanRawValue = value;
}

uint8_t DEA_GetOAT_CanRawValue(void)
{
	return g_OAT_CanRawValue;
}

void DEA_SetOAT_RawResValid(uint8_t valid)
{
	g_OAT_RawResValid = valid;
}

uint8_t DEA_GetOAT_RawResValid(void)
{
	return g_OAT_RawResValid;
}


void DEA_SetOAT_RawResValue(uint16_t value)
{
	g_OAT_RawResValue = value;
}

uint16_t DEA_GetOAT_RawResValue(void)
{
	return g_OAT_RawResValue;
}


void DEA_SetOAT_FilterValue(int16_t value)
{
	g_OAT_FilterValue = value;
}

int16_t DEA_GetOAT_FilterValue(void)
{
	return g_OAT_FilterValue;
}

//CruiseControl
void DEA_SetCruiseControl_ST_CC(CRUISE_CONTROL_ENUM cruise)
{
	g_CruiseControl_ST_CC = cruise;
}

CRUISE_CONTROL_ENUM DEA_GetCruiseControl_ST_CC(void)
{
	return g_CruiseControl_ST_CC;
}

void DEA_SetCruiseControl_TAR_V_CC(uint8_t cc)
{
	g_CruiseControl_TAR_V_CC= cc;
}

uint8_t DEA_GetCruiseControl_TAR_V_CC(void)
{
	return g_CruiseControl_TAR_V_CC;
}

int32_t DEA_GetMaintainDistance(void)
{
	return SERV_GetDistance();
}


uint8_t DEA_GetDistanceValid(void)
{
	return SERV_GetDistanceValid();
}

void DEA_SetMHUTimeUpdatedInd(uint8_t stat)
{
	DEA_MHU_TimeUpdatedInd = stat;
}

uint8_t DEA_GetMHUTimeUpdatedInd(void)
{
	return DEA_MHU_TimeUpdatedInd;
}

void DEA_SetMHUTimeValid(uint8_t flag)
{
	DEA_MHU_TimeValid = flag;
}

uint8_t DEA_GetMHUTimeValid(void)
{
	return DEA_MHU_TimeValid;
}

void DEA_SetMHUTimeValue(MHU_TIME_STRU time)
{
	DEA_MHU_Time = time;
}

void DEA_GetMHUTimeValue(MHU_TIME_STRU *pTime)
{
	*pTime = DEA_MHU_Time;
}

void DEA_SetTimeValue(TIME_STRU time)
{
	//portENTER_CRITICAL();
	g_Time = time;
	//portEXIT_CRITICAL();
}

void DEA_GetTimeValue(TIME_STRU *pTime)
{
	//portENTER_CRITICAL();
	*pTime = g_Time;
	//portEXIT_CRITICAL();
}


void DEA_SetTimeFormat(TIME_FORMAT_ENUM TimeFormat)
{
	g_TimeFormat = TimeFormat;
}

TIME_FORMAT_ENUM DEA_GetTimeFormat(void)
{
	return g_TimeFormat;
}

void DEA_SetMHUTimeFormat(TIME_FORMAT_ENUM TimeFormat)
{
	g_MHUTimeFormat = TimeFormat;
}

TIME_FORMAT_ENUM DEA_GetMHUTimeFormat(void)
{
	return g_MHUTimeFormat;
}

void DEA_SetPositionLightStat(STAT_POS_LIGHT_ENUM PositionStat)
{
	g_PosLight = PositionStat;
}

STAT_POS_LIGHT_ENUM DEA_GetPositionLightStat(void)
{
	return g_PosLight;
}

void DEA_SetIC_Dimming(IC_DIMMING_STRU ic_dimming)
{
	portENTER_CRITICAL();
	g_IC_Dimming = ic_dimming;
	portEXIT_CRITICAL();
	DEA_SetDimmingMsgToMHUFlag(1);
}

void DEA_GetIC_Dimming(IC_DIMMING_STRU *pIc_dimming)
{
	portENTER_CRITICAL();
	*pIc_dimming = g_IC_Dimming;
	portEXIT_CRITICAL();
}

uint8_t DEA_GetIC_DimmingDarkON(void)
{
	return g_IC_Dimming.IC_DIMMING_DARK_ON;
}

uint8_t DEA_GetIC_DimmingDarkOFF(void)
{
	return g_IC_Dimming.IC_DIMMING_DARK_OFF;
}


void DEA_SetPhotoSensorValue(int16_t sensor_value)
{
	g_PhotoSensorValue = sensor_value;
}

int16_t DEA_GetPhotoSensorValue(void)
{
	return g_PhotoSensorValue;
}


void DEA_SetIC_TempEx(uint16_t ic_temp)
{
	g_IC_TempEx = ic_temp;
}

uint16_t DEA_GetIC_TempEx(void)
{
	return g_IC_TempEx;
}

void DEA_SetIC_TempExUnfilt(uint16_t ic_temp_unfilt)
{
	g_IC_TempExUnfilt = ic_temp_unfilt;
}

uint16_t DEA_GetIC_TempExUnfilt(void)
{
	return g_IC_TempExUnfilt;
}

void DEA_SetBoardButtonStatus(BOARD_BUTTON_ENUM button)
{
	g_ButtonStat = button;
}

BOARD_BUTTON_ENUM DEA_GetBoardButtonStatus(void)
{
	return g_ButtonStat;
}

void DEA_SetDriveModeButton(uint8_t stat)
{
	g_DriveModeButtonStat = stat;
}

uint8_t DEA_GetDriveModeButton(void)
{
	return g_DriveModeButtonStat;
}

void DEA_SetPhoneButton(uint8_t stat)
{
	g_PhoneButton = stat;
}

uint8_t DEA_GetPhoneButton(void)
{
	return g_PhoneButton;
}

void DEA_SetFuelInjectionValid(uint16_t stat)	//valid signal
{
	g_FuelInjectionValid = stat;
}

uint8_t DEA_GetFuelInjectionValid(void)
{
	return g_FuelInjectionValid;
}

void DEA_SetRawFuelInjectionErrStat(uint8_t stat)	//591 project require: fuel injection stat
{
	g_FuelInjectionstat = stat;
}

uint8_t DEA_GetRawFuelInjectionErrStat(void)		//591 project require: fuel injection stat
{
	return g_FuelInjectionstat;
}


void DEA_SetFuelInjection(uint16_t fuel_injection)
{
	g_FuelInjection = fuel_injection;
}
/*
void DEA_SetBordComputerButton(uint8_t stat)
{
	g_BordComputerButtonStat = stat;
}

uint8_t DEA_GetBordComputerButton(void)
{
	return g_BordComputerButtonStat;
}
*/

uint16_t DEA_GetFuelInjection(void)
{
	return g_FuelInjection;
}

uint8_t DEA_GetTII_Valid(void)
{
	return g_DEA_TII_Valid;
}

void DEA_SetTII_Valid(uint8_t valid)
{
	g_DEA_TII_Valid = valid;
}

uint16_t DEA_GetTII_Value(void)
{
	return g_DEA_TII_Value;
}

void DEA_SetTII_Value(uint16_t value)
{
	g_DEA_TII_Value = value;
}

void DEA_SetDTE_Valid(uint8_t stat)
{
	g_DEA_DTE_Valid = stat;
}

uint8_t DEA_GetDTE_Valid(void)
{
	return g_DEA_DTE_Valid;
}

void DEA_SetDTE(uint16_t dte)
{
	g_DEA_DTE = dte;
}

uint16_t DEA_GetDTE(void)
{
	return g_DEA_DTE;
}

void DEA_SetLanguage(LANGUAGE_ENUM language)
{
	g_LanguageSet = language;
}

LANGUAGE_ENUM DEA_GetLanguage(void)
{
	return g_LanguageSet;
}

void DEA_SetMHULanguage(LANGUAGE_ENUM language)
{
	g_MHULanguageSet = language;
}

LANGUAGE_ENUM DEA_GetMHULanguage(void)
{
	return g_MHULanguageSet;
}


void DEA_SetUnitChangedFlagForCan(uint8_t stat)
{
	g_DEA_UnitChangedFlagForCan = stat;
}

uint8_t DEA_GetUnitChangedFlagForCan(void)
{
	return g_DEA_UnitChangedFlagForCan;
}

TIRE_STAT_STRU *DEA_GetTireStatPtr(void)
{
	return (&g_TireStat);
}

void DEA_SetTireFalut(uint8_t stat)
{
	g_TireFault = stat;
}

uint8_t DEA_GetTireFault(void)
{
	return g_TireFault;
}

void DEA_SetDISP_GR_GRB_Sig(uint8_t stat)
{
	g_DISP_GR_GRB_Sig = stat;
}

uint8_t DEA_GetDISP_GR_GRB_Sig(void)
{
	return g_DISP_GR_GRB_Sig;
}

void DEA_SetDISP_RQ_GR_GRB_Sig(uint8_t stat)
{
	g_DISP_RQ_GR_GRB_Sig = stat;
}

uint8_t DEA_GetDISP_RQ_GR_GRB_Sig(void)
{
	return g_DISP_RQ_GR_GRB_Sig;
}

void DEA_SetDISP_PO_GRB_Sig(uint8_t stat)
{
	g_DISP_PO_GRB_Sig = stat;
}

uint8_t DEA_GetDISP_PO_GRB_Sig(void)
{
	return g_DISP_PO_GRB_Sig;
}

void DEA_SetDISP_PRG_GRB_Sig(uint8_t stat)
{
	g_DISP_PRG_GRB_Sig = stat;
}

uint8_t DEA_GetDISP_PRG_GRB_Sig(void)
{
	return g_DISP_PRG_GRB_Sig;
}


void DEA_SetDISP_GR_PT_Sig(uint8_t stat)
{
	g_DISP_GR_PT_Sig = stat;
}

uint8_t DEA_GetDISP_GR_PT_Sig(void)
{
	return g_DISP_GR_PT_Sig;
}

void DEA_SetDISP_GSI_DRV_Sig(uint8_t stat)
{
	g_DISP_GSI_DRV_Sig = stat;
}

uint8_t DEA_GetDISP_GSI_DRV_Sig(void)
{
	return g_DISP_GSI_DRV_Sig;
}

void DEA_SetDISP_TARG_DRV_Sig(uint8_t stat)
{
	g_DISP_TARG_DRV_Sig = stat;
}

uint8_t DEA_GetDISP_TARG_DRV_Sig(void)
{
	return g_DISP_TARG_DRV_Sig;
}


void DEA_SetGear(GEAR_ENUM gear)
{
	g_Gear = gear;
}

GEAR_ENUM DEA_GetGear(void)
{
	return g_Gear;
}

void DEA_SetTargetGear(GEAR_ENUM target_gear)
{
	g_TargetGear = target_gear;
}

GEAR_ENUM DEA_GetTargetGear(void)
{
	return g_TargetGear;
}


void DEA_SetGearShiftStat(GEAR_SHIFT_ENUM gear_shift)
{
	g_GearShiftStat = gear_shift;
}

GEAR_SHIFT_ENUM DEA_GetGearShiftStat(void)
{
	return g_GearShiftStat;
}

void DEA_SetTransGearIO(uint8_t value)
{
	g_DEA_TransGearIO = value;
}


uint8_t DEA_GetTransGearIO(void)
{
	return g_DEA_TransGearIO;
}

void DEA_SetTransGearValid(uint8_t value)
{
	g_DEA_TransGearValid = value;
}


uint8_t DEA_GetTransGearValid(void)
{
	return g_DEA_TransGearValid;
}


void DEA_SetTurnIndicator(TURN_IND_STAT_ENUM turn_indicator_stat)
{
	g_TurnIndStat = turn_indicator_stat;
}

TURN_IND_STAT_ENUM DEA_GetTurnIndicator(void)
{
	return g_TurnIndStat;
}

void DEA_SetTransCntrlValid(uint8_t value)
{
	g_DEA_TransCntrlValid = value;
}


uint8_t DEA_GetTransCntrlValid(void)
{
	return g_DEA_TransCntrlValid;
}

void DEA_SetTransCntrl(uint8_t value)
{
	g_DEA_TransCntrlMd = value;
}


uint8_t DEA_GetTransCntrl(void)
{
	return g_DEA_TransCntrlMd;
}


void DEA_SetTransType(uint8_t value)
{
	g_DEA_TransCntlType = value;
}


uint8_t DEA_GetTransType(void)
{
	return g_DEA_TransCntlType;
}

void DEA_SetHighBeamStat(TELLTALE_ENUM stat)
{
	g_HighBeamStat = stat;
}

TELLTALE_ENUM DEA_GetHighBeamStat(void)
{
	return g_HighBeamStat;
}

void DEA_SetLowBeamStat(TELLTALE_ENUM stat)
{
	g_LowBeamStat = stat;
}

TELLTALE_ENUM DEA_GetLowBeamStat(void)
{
	return g_LowBeamStat;
}

void DEA_SetFogLampFront(TELLTALE_ENUM fog_lamp_fornt)
{
	g_FogLampFront = fog_lamp_fornt;
}

TELLTALE_ENUM DEA_GetFogLampFront(void)
{
	return g_FogLampFront;
}

void DEA_SetFogLampRear(TELLTALE_ENUM fog_lamp_rear)
{
	g_FogLampRear = fog_lamp_rear;
}

TELLTALE_ENUM DEA_GetFogLampRear(void)
{
	return g_FogLampRear;
}


void DEA_SetPositionLight(TELLTALE_ENUM position_light)
{
	g_PositionLight = position_light;
}

TELLTALE_ENUM DEA_GetPositionLight(void)
{
	return g_PositionLight;
}


void DEA_SetAutolight(ICON_STAT_ENUM icon_stat_rear)
{
	g_Autolight = icon_stat_rear;
}

ICON_STAT_ENUM DEA_GetAutolight(void)
{
	return g_Autolight;
}


void DEA_SetAutoWiping(ICON_STAT_ENUM stat)
{
	g_AutoWiping = stat;
}

ICON_STAT_ENUM DEA_GetAutoWiping(void)
{
	return g_AutoWiping;
}

void DEA_SetEPS_Stat(TELLTALE_ENUM stat)
{
	g_EPS_Stat = stat;
}

TELLTALE_ENUM DEA_GetEPS_Stat(void)
{
	return g_EPS_Stat;
}

void DEA_SetESC_Stat(TELLTALE_ENUM stat)
{
	g_ESC_Stat = stat;
}

TELLTALE_ENUM DEA_GetESC_Stat(void)
{
	return g_ESC_Stat;
}

void DEA_SetESC_OFF_Stat(TELLTALE_ENUM stat)
{
	g_ESC_OFF_Stat = stat;
}

TELLTALE_ENUM DEA_GetESC_OFF_Stat(void)
{
	return g_ESC_OFF_Stat;
}

void DEA_SetHDC_Stat(TELLTALE_ENUM stat)
{
	g_HDC_Stat = stat;
}

TELLTALE_ENUM DEA_GetHDC_Stat(void)
{
	return g_HDC_Stat;
}


void DEA_SetAirBag_Stat(TELLTALE_ENUM stat)
{
	g_AirBagStat = stat;
}

TELLTALE_ENUM DEA_GetAirBag_Stat(void)
{
	return g_AirBagStat;
}


void DEA_SetABS_Stat(TELLTALE_ENUM stat)
{
	g_ABSStat = stat;
}

TELLTALE_ENUM DEA_GetABS_Stat(void)
{
	return g_ABSStat;
}


void DEA_SetEBD_Stat(TELLTALE_ENUM stat)
{
	g_EBDStat = stat;
}

TELLTALE_ENUM DEA_GetEBD_Stat(void)
{
	return g_EBDStat;
}


void DEA_SetDriverSeatbelt_Stat(TELLTALE_ENUM stat)
{
	g_DSbeltStat = stat;
}

TELLTALE_ENUM DEA_GetDriverSeatbelt_Stat(void)
{
	return g_DSbeltStat;
}

void DEA_SetPassSeatbelt_Stat(TELLTALE_ENUM stat)
{
	g_PSbeltStat= stat;
}

TELLTALE_ENUM DEA_GetPassSeatbelt_Stat(void)
{
	return g_PSbeltStat;
}



void DEA_SetRadarStat(RADAR_STRU radar_value)
{
	portENTER_CRITICAL();
	g_RadarStat = radar_value;
	portEXIT_CRITICAL();
}

void DEA_GetRadarStat(RADAR_STRU *pRadar_value)
{
	portENTER_CRITICAL();
	*pRadar_value = g_RadarStat;
	portEXIT_CRITICAL();
}

void DEA_SetLDWS_Stat(LDWS_STRU ldws)
{
	portENTER_CRITICAL();
	g_LDWS_Stat = ldws;
	portEXIT_CRITICAL();
}

void DEA_GetLDWS_Stat(LDWS_STRU *pLdws)
{
	portENTER_CRITICAL();
	*pLdws = g_LDWS_Stat;
	portEXIT_CRITICAL();
}

void DEA_SetLDWSOnOffStat(uint8_t stat)
{
	g_DEA_LDWSOnOffStat = stat;
}

uint8_t DEA_GetLDWSOnOffStat(void)
{
	return g_DEA_LDWSOnOffStat;
}

void DEA_SetDoorStat(DOOR_STAT_STRU door_stat)
{
	portENTER_CRITICAL();
	g_DoorStat = door_stat;
	portEXIT_CRITICAL();
}

void DEA_GetDoorStat(DOOR_STAT_STRU *pDoorStat)
{
	portENTER_CRITICAL();
	*pDoorStat = g_DoorStat;
	portEXIT_CRITICAL();
}

void DEA_SetSeatMemoryStore(SEAT_MEMORY_ENUM seat)
{
	g_SeatMemory = seat;
}

SEAT_MEMORY_ENUM DEA_GetSeatMemoryStore(void)
{
	return g_SeatMemory;
}

void DEA_SetETG_Status(ETG_STATUS_STRU stat)
{
	portENTER_CRITICAL();
	g_ETG_Status = stat;
	portEXIT_CRITICAL();
}

void DEA_GetETG_Status(ETG_STATUS_STRU *pStat)
{
	portENTER_CRITICAL();
	*pStat = g_ETG_Status;
	portEXIT_CRITICAL();
}

void DEA_SetVehicleStopStat(TELLTALE_ENUM stat)
{
	g_VehicleStopStat = stat;
}

TELLTALE_ENUM DEA_GetVehicleStopStat(void)
{
	return g_VehicleStopStat;
}


void DEA_SetESC_ASR_Stat(TELLTALE_ENUM stat)
{
	g_ESC_ASR_Stat = stat;
}

TELLTALE_ENUM DEA_GetESC_ASR_Stat(void)
{
	return g_ESC_ASR_Stat;
}

void DEA_SetBCM_PDC_Stat(PDC_ENUM stat)
{
	g_BCM_PDC_Stat = stat;
}

PDC_ENUM DEA_GetBCM_PDC_Stat(void)
{
	return g_BCM_PDC_Stat;
}


void DEA_SetPDC_Stat(PDC_ENUM stat)
{
	g_PDC_Stat = stat;
}

PDC_ENUM DEA_GetPDC_Stat(void)
{
	return g_PDC_Stat;
}

void DEA_SetBSW_Stat(uint8_t stat)
{
	g_BSW_Stat = stat;
}

uint8_t DEA_GetBSW_Stat(void)
{
	return g_BSW_Stat;
}

void DEA_SetWLC_Stat(WLC_STRU stat)
{
	portENTER_CRITICAL();
	g_WLC_Stat = stat;
	portEXIT_CRITICAL();
}

void DEA_GetWLC_Stat(WLC_STRU *pStat)
{
	portENTER_CRITICAL();
	*pStat = g_WLC_Stat;
	portEXIT_CRITICAL();
}

void DEA_SetFuelLevelPercentValid(uint8_t value)
{
	g_DEA_FuelLevelPercentValid = value;
}

uint8_t DEA_GetFuelLevelPercentValid(void)
{
	return g_DEA_FuelLevelPercentValid;
}

void DEA_SetFuelLevelPercent(uint16_t value)
{
	g_DEA_FuelLevelPercent = value;
}

uint16_t DEA_GetFuelLevelPercent(void)
{
	return g_DEA_FuelLevelPercent;
}

void DEA_SetWarnMsg(WARN_MSG_STRU warn)
{
	portENTER_CRITICAL();
	g_WarnMsg = warn;
	portEXIT_CRITICAL();
}

void DEA_GetWarnMsg(WARN_MSG_STRU *pWarnMsg)
{
	portENTER_CRITICAL();
	*pWarnMsg = g_WarnMsg;
	portEXIT_CRITICAL();
}


void DEA_SetCompass(uint16_t value)
{
	g_compass = value;
}

uint16_t DEA_GetCompass(void)
{
	return g_compass;
}

void DEA_SetTravelTimeValue(uint32_t time)
{
	g_Display_Tt = time;
}

uint32_t DEA_GetTravelTimeValue(void)
{
	return g_Display_Tt;	
}


void DEA_SetAVSA(uint32_t avs)
{
	g_AVS_A = avs;
}

uint32_t DEA_GetAVSA(void)
{
	return g_AVS_A;	
}

void DEA_SetAVSA_Valid(uint8_t stat)
{
	g_AVS_A_Valid = stat;
}

uint8_t DEA_GetAVSA_valid(void)
{
	return g_AVS_A_Valid;	
}

void DEA_SetAVSB(uint32_t avs)
{
	g_AVS_B = avs;
}

uint32_t DEA_GetAVSB(void)
{
	return g_AVS_B;	
}


void DEA_SetAVSB_Valid(uint8_t stat)
{
	g_AVS_B_valid = stat;
}

uint8_t DEA_GetAVSB_valid(void)
{
	return g_AVS_B_valid;	
}

void DEA_SetBCM_VinValid(uint8_t stat)
{
	g_DEA_BCM_VINValid = stat;
}


uint8_t DEA_GetBCM_VinValid(void)
{
	return g_DEA_BCM_VINValid;
}

void DEA_SetBCM_Vin(uint8_t *pVin)
{
	portENTER_CRITICAL();
	memcpy(g_DEA_BCM_VIN, pVin, 7);
	portEXIT_CRITICAL();
}

void DEA_GetBCM_Vin(uint8_t *pVin)
{
	portENTER_CRITICAL();
	memcpy(pVin, g_DEA_BCM_VIN, 7);
	portEXIT_CRITICAL();
}

void DEA_SetIPC_Vin(uint8_t *pVin)
{
	portENTER_CRITICAL();
	memcpy(g_DEA_IPC_VIN, pVin, 7);
	portEXIT_CRITICAL();
}

void DEA_GetIPC_Vin(uint8_t *pVin)
{
	portENTER_CRITICAL();
	memcpy(pVin, g_DEA_IPC_VIN, 7);
	portEXIT_CRITICAL();
}

void DEA_PutICWarnMsg(WARN_MSG_STRU warnMsg)
{
	portENTER_CRITICAL();
	if(g_ICWarnMsgCnt < (sizeof(g_ICWarnMsgBuf)/sizeof(g_ICWarnMsgBuf[0]) - 1))
	{
		g_ICWarnMsgBuf[g_ICWarnMsgCnt++] = warnMsg;
	}
	portEXIT_CRITICAL();
}


WARN_MSG_STRU DEA_GetICWarnMsg(void)
{
	WARN_MSG_STRU retMsg = {0};
	portENTER_CRITICAL();
	if(g_ICWarnMsgCnt > 0)
	{
		retMsg = g_ICWarnMsgBuf[g_ICWarnMsgCnt-1];
		g_ICWarnMsgCnt--;
	}
	portEXIT_CRITICAL();
	return retMsg;
}

void DEA_SetSBRSoundRequest(uint8_t stat)
{
	g_DEA_SBRSoundRequestStat = stat;
}

uint8_t DEA_GetSBRSoundRequest(void)
{
	return g_DEA_SBRSoundRequestStat;
}


uint8_t DEA_GetICWarnMsgCnt(void)
{
	return g_ICWarnMsgCnt;
}


void DEA_SetCanTimeOutStat(PduIdType pduId, uint8_t stat)
{
	g_CanTimeoutBuf[pduId] = stat;
	IPC_SetCanMessageTimeoutStat(pduId, stat);
	if(stat == 0)
	{
		DEA_SetCanReceiveIndStat(pduId, 1);
	}
	if(stat)
	{
		TracePrintf(0,0, "Pdu Id %d Timeout!\r\n", pduId);
	}
}

uint8_t DEA_GetCanTimeOutStat(PduIdType pduId)
{
	return g_CanTimeoutBuf[pduId];
}

DEA_MHU_IPC_LASTCALLS_STRU *DEA_GetLastcallPtr(void)
{
	return &g_DEA_LastCall;
}

DEA_MHU_IPC_MEDIA_STRU *DEA_GetMediaPtr(void)
{
	return &g_DEA_Media;
}


void DEA_SetPhonePairStatus(DEA_MHU_IPC_PAIR_STATUS_ENUM pairStat)
{
	g_DEA_PhonePairStatus = pairStat;
}

DEA_MHU_IPC_PAIR_STATUS_ENUM DEA_GetPhonePairStatus(void)
{
	return g_DEA_PhonePairStatus;
}

void DEA_SetTELStatus(DEA_MHU_IPC_TEL_STRU telStat)
{
	g_DEA_TELStatus = telStat;
}

DEA_MHU_IPC_TEL_STRU DEA_GetTELStatus(void)
{
	return g_DEA_TELStatus;
}


void DEA_SetICTelStatusUpdatedFlag(uint8_t stat)
{
	g_IC_TelStatusUpdatedFlag = stat;
}

uint8_t DEA_GetICTelStatusUpdatedFlag(void)
{
	return g_IC_TelStatusUpdatedFlag;
}

void DEA_SetICTelStatus(DEA_IC_MHU_TEL_STRU stat)
{
	g_IC_TelStatus = stat;
}

DEA_IC_MHU_TEL_STRU DEA_GetICTelStatus(void)
{
	return g_IC_TelStatus;
}

void DEA_SetCallingTime(DEA_CALLING_TIME_STRU callingTime)
{
	g_DEA_CallingTime = callingTime;
}

DEA_CALLING_TIME_STRU DEA_GetCallingTime(void)
{
	return g_DEA_CallingTime;
}


void DEA_SetNaviChangedFlag(uint8_t stat)
{
	g_DEA_NaviChangedFlag = stat;
}
uint8_t  DEA_GetNaviChangedFlag(void)
{
	return g_DEA_NaviChangedFlag;
}

void DEA_SetNaviStatus(DEA_NAVI_STRU navi)
{
	g_DEA_NaviStatus = navi;
	
}

DEA_NAVI_STRU DEA_GetNaviStatus(void)
{
	return g_DEA_NaviStatus;
}


void DEA_SetNaviForIPCStatus(DEA_NAVI_FOR_IPC_STRU navi)
{
	g_DEA_NaviForIPCStatus = navi;
}

DEA_NAVI_FOR_IPC_STRU DEA_GetNaviForIPCStatus(void)
{
	return g_DEA_NaviForIPCStatus;
}

void DEA_SetCompassForIPC(DEA_COMPASS_FOR_IPC_STRU stat)
{
	g_DEA_Compass = stat;
}

DEA_COMPASS_FOR_IPC_STRU DEA_GetCompassForIPC(void)
{
	return g_DEA_Compass;
}

void DEA_SetDME_CrcMatch(uint8_t stat)
{
	g_DME_CrcMatch = stat;
}

uint8_t DEA_GetDME_CrcMatch(void)
{
	return g_DME_CrcMatch;
}

void DEA_SetEPBI_CrcMatch(uint8_t stat)
{
	g_EPBI_CrcMatch = stat;
}

uint8_t DEA_GetEPBI_CrcMatch(void)
{
	return g_EPBI_CrcMatch;
}

void  DEA_SetFuelSigStat(DEA_FUEL_SIG_ENUM stat)
{
	g_DEA_FuelSigStat = stat;
}

DEA_FUEL_SIG_ENUM DEA_GetFuelSigStat(void)
{
	return g_DEA_FuelSigStat;
}

void DEA_SetOATSigStat(DEA_OAT_SIG_ENUM stat)
{
	g_DEA_OatSigStat = stat;
}

DEA_OAT_SIG_ENUM DEA_GetOATSigStat(void)
{
	return g_DEA_OatSigStat;
}

void DEA_SetLVDSStat(DEA_LVDS_STAT_ENUM stat)
{
	g_DEA_FuelSigStat = stat;
}

DEA_LVDS_STAT_ENUM DEA_GetLVDSStat(void)
{
	return g_DEA_FuelSigStat;
}

void DEA_SetTTSecond(uint32_t tt)
{
	g_DEA_TTSecond = tt;
}

uint32_t DEA_GetTTSecond(void)
{
	return g_DEA_TTSecond;
}

void DEA_SetBmsRequiredChargingTime(uint16_t value)
{
	g_BmsRequiredChargingTime = value;
}

uint16_t DEA_GetBmsRequiredChargingTime(void)
{
	return g_BmsRequiredChargingTime;
}

void DEA_SetBmsRequiredChargingTimeValid(uint8_t value)
{
	g_BmsRequiredChargingTimeValid = value;
}

uint8_t DEA_GetBmsRequiredChargingTimeValid(void)
{
	return g_BmsRequiredChargingTimeValid;
}


uint8_t DEA_GetBmsChargingState(void)
{
	return g_BmsChargingState;
}

void DEA_SetBmsChargingState(uint8_t value)
{
	g_BmsChargingState = value;
}


void IPC_SetUpdatedStat(uint8_t id, uint8_t stat)
{
	if(stat)
	{
		s_CanUpdatedStat[id / 8] |= (0x01<<(id%8));
	}
	else
	{
		s_CanUpdatedStat[id / 8] &=~(0x01<<(id%8));
	}
}

uint8_t IPC_GetUpdatedStat(uint8_t id)
{
	if(s_CanUpdatedStat[id / 8] & (0x01<<(id%8)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DEA_SetCanReceiveIndStat(uint8_t id, uint8_t stat)
{
	if(stat)
	{
		s_CanReceivedIndStat[id / 8] |= (0x01<<(id%8));
	}
	else
	{
		s_CanReceivedIndStat[id / 8] &=~(0x01<<(id%8));
	}
}

uint8_t DEA_GetCanReceiveIndStat(uint8_t id)
{
	if(s_CanReceivedIndStat[id / 8] & (0x01<<(id%8)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/************Sig IDSTSTS*****************/

void DEA_SetIDSTSTS(uint8_t value)
{
    DeaIDSTSTS = value;
}

uint8_t DEA_GetIDSTSTS(void)
{
    return DeaIDSTSTS;
}

void DEA_SetHardwareStatus(uint32_t value)
{
    g_DeaHardwareStatus = value;
}

uint8_t DEA_GetHardwareStatus(uint32_t mask)
{
	if((g_DeaHardwareStatus>>mask)&0x01)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DEA_SetNightDimmerFlag(uint8_t value)
{
	g_DeaNightDimmerFlag = value;
}

uint8_t DEA_GetNightDimmerFlag(void)
{
	return g_DeaNightDimmerFlag;
}


void DEA_SetCLUSTER_ILL_REQ(uint8_t value)
{
	g_DeaCLUSTER_ILL_REQ = value;
}

uint8_t DEA_GetCLUSTER_ILL_REQ(void)
{
	return g_DeaCLUSTER_ILL_REQ;
}


void DEA_SetEngineStatusCopy(uint8_t value)
{
	g_DeaEngineStatusCopy = value;
}

uint8_t DEA_GetEngineStatusCopy(void)
{
	return g_DeaEngineStatusCopy;
}

void DEA_SetStaterControlStatus(uint8_t value)
{
	g_DeaStaterControlStatus = value;
}

uint8_t DEA_GetStaterControlStatus(void)
{
	return g_DeaStaterControlStatus;
}

/************Sig EPB_WORKMODE*****************/

void DEA_SetEPB_WORKMODE(uint8_t value)
{
    DeaEPB_WORKMODE = value;
}

uint8_t DEA_GetEPB_WORKMODE(void)
{

    return DeaEPB_WORKMODE;
}

/************Sig EPB_STAT*****************/

void DEA_SetEPB_STAT(uint8_t value)
{
    DeaEPB_STAT = value;

}

uint8_t DEA_GetEPB_STAT(void)
{
    return DeaEPB_STAT;
}

/************Sig DynamicBrakingStatus*****************/
void DEA_SetDynamicBrakingStatus(uint8_t value)
{
    DeaDynamicBrakingStatus = value;
}

uint8_t DEA_GetDynamicBrakingStatus(void)
{
    return DeaDynamicBrakingStatus;
}

/************Sig ISMSKSW*****************/

void DEA_SetISMSKSW(uint8_t value)
{
    DeaISMSKSW = value;
}

uint8_t DEA_GetISMSKSW(void)
{
    return DeaISMSKSW;
}



void DEA_SetTempOutputMode(uint8_t value)
{
    DeaTempOutputMode = value;
}

uint8_t DEA_GetTempOutputMode(void)
{
    return DeaTempOutputMode;
}

void DEA_SetFuelCapReset(uint8_t value)
{
    DeaFuelCapReset = value;
}

uint8_t DEA_GetFuelCapReset(void)
{
    return DeaFuelCapReset;
}

/************Sig VDC_LAMP*****************/

void DEA_SetVDC_LAMP(uint8_t value)
{
    DeaVDC_LAMP = value;
}

uint8_t DEA_GetVDC_LAMP(void)
{
    return DeaVDC_LAMP;
}



void DEA_SetCrankingStatus(uint8_t value)
{
	DeaCrankingStatus = value;
}

uint8_t DEA_GetCrankingStatus(void)
{
	return DeaCrankingStatus;
}

void DEA_SetTACHO(uint16_t value)
{
    DeaTACHO = value;
}

uint16_t DEA_GetTACHO(void)
{
    return DeaTACHO;
}


void DEA_SetRawTACHO(uint16_t value)
{
    DeaRawTACHO = value;
}

uint16_t DEA_GetRawTACHO(void)
{
    return DeaRawTACHO;
}

void DEA_SetCanMsgtoIpcInitFlag(uint8_t value)
{
    CanMsgtoIpcInitFlag = value;
}

uint8_t DEA_GetCanMsgtoIpcInitFlag(void)
{
    return CanMsgtoIpcInitFlag;
}

void DEA_SetFGAUGE(uint16_t value)
{
	DeaFGAUGE = value;
}

uint16_t DEA_GetFGAUGE(void)
{
	return DeaFGAUGE;
}

void DEA_SetFMAD(uint16_t value)
{
	DeaFMad = value;
}

uint16_t DEA_GetFMAD(void)
{
	return DeaFMad;
}


void DEA_SetVoltageState(uint8_t value)
{
	DeaVoltageState = value;
}

uint8_t DEA_GetVoltageState(void)
{
	return DeaVoltageState;
}

void DEA_SetSHIFT_IND(uint8_t value)
{
	DeaSHIFT_IND = value;
}

uint8_t DEA_GetSHIFT_IND(void)
{
	return DeaSHIFT_IND;
}


/************Sig TPD_FR*****************/

void DEA_SetTPD_FR(uint8_t value) /*yangxl */

{
    DeaTPD_FR = value;
}

uint8_t DEA_GetTPD_FR(void)
{
    return DeaTPD_FR;
}

/************Sig TPD_FL*****************/

void DEA_SetTPD_FL(uint8_t value) /*yangxl */
{
    DeaTPD_FL = value;
}

uint8_t DEA_GetTPD_FL(void)
{
    return DeaTPD_FL;
}

/************Sig TPD_RR*****************/
void DEA_SetTPD_RR(uint8_t value) /*yangxl */
{
    DeaTPD_RR = value;
}

uint8_t DEA_GetTPD_RR(void)
{
    return DeaTPD_RR;
}

/************Sig TPD_RL*****************/
void DEA_SetTPD_RL(uint8_t value) /*yangxl */
{
    DeaTPD_RL = value;
}

uint8_t DEA_GetTPD_RL(void)
{
    return DeaTPD_RL;
}

void DEA_SetACCSPDREQ(uint8_t value) /*yangxl */
{
    Dea_ACCSPDREQ = value;
}

uint8_t DEA_GetACCSPDREQ(void)
{
    return Dea_ACCSPDREQ;
}



/************Sig TS_FR*****************/
void DEA_SetTS_FR(uint8_t value) /*yangxl */
{
    DeaTS_FR = value;
}

uint8_t DEA_GetTS_FR(void)
{
    return DeaTS_FR;
}
/************Sig TS_FL*****************/

void DEA_SetTS_FL(uint8_t value) /*yangxl */
{
    DeaTS_FL = value;
}

uint8_t DEA_GetTS_FL(void)
{
    return DeaTS_FL;
}
/************Sig TS_RR*****************/
void DEA_SetTS_RR(uint8_t value) /*yangxl */
{
    DeaTS_RR = value;
}

uint8_t DEA_GetTS_RR(void)
{
    return DeaTS_RR;
}

/************Sig TS_RL*****************/
void DEA_SetTS_RL(uint8_t value) /*yangxl */
{
    DeaTS_RL = value;
}

uint8_t DEA_GetTS_RL(void)
{
    return DeaTS_RL;
}

/************Sig D_STYLE*****************/

void DEA_SetD_STYLE(uint8_t value) /*yangxl */
{
    DeaD_STYLE = value;
}
uint8_t DEA_GetD_STYLE(void)
{
    return DeaD_STYLE;
}

/************Sig REQ_DISP_MET*****************/
void DEA_SetREQ_DISP_MET(uint8_t value) /*yangxl */
{
    DeaREQ_DISP_MET = value;
}
uint8_t DEA_GetREQ_DISP_MET(void)
{
    return DeaREQ_DISP_MET;
}

/************Sig SONAR_S_REQ*****************/ 
void DEA_SetSONAR_S_REQ(uint8_t value) /*yangxl */
{
    DeaSONAR_S_REQ = value;
}
uint8_t DEA_GetSONAR_S_REQ(void)
{
    return DeaSONAR_S_REQ;
}

/************Sig RR_CEN*****************/
void DEA_SetRR_CEN(uint8_t value) /*yangxl */
{
    DeaRR_CEN = value;
}
uint8_t DEA_GetRR_CEN(void)
{
    return DeaRR_CEN;
}

/************Sig A_RR_CEN*****************/
void DEA_SetA_RR_CEN(uint8_t value)/*yangxl */
{
    DeaA_RR_CEN = value;
}
uint8_t DEA_GetA_RR_CEN(void)
{
    return DeaA_RR_CEN;
}
/************Sig RR_L*****************/
void DEA_SetRR_L(uint8_t value)/*yangxl */
{
    DeaRR_L = value;
}
uint8_t DEA_GetRR_L(void)
{
    return DeaRR_L;
}

/************Sig A_RR_L*****************/
void DEA_SetA_RR_L(uint8_t value)/*yangxl */
{
    DeaA_RR_L = value;
}
uint8_t DEA_GetA_RR_L(void)
{
    return DeaA_RR_L;
}

/************Sig RR_R*****************/
void DEA_SetRR_R(uint8_t value)/*yangxl */
{
    DeaRR_R = value;
}
uint8_t DEA_GetRR_R(void)
{
    return DeaRR_R;
}
/************Sig A_RR_R*****************/
void DEA_SetA_RR_R(uint8_t value)/*yangxl */
{
    DeaA_RR_R = value;
}
uint8_t DEA_GetA_RR_R(void)
{
    return DeaA_RR_R;
}
/************Sig STS_DISTANCE*****************/
void DEA_SetSTS_DISTANCE(uint8_t value)/*yangxl */
{
    DeaSTS_DISTANCE = value;
}
uint8_t DEA_GetSTS_DISTANCE(void)
{
    return DeaSTS_DISTANCE;
}

/************Sig rangeStatus_FR_Center*****************/
void DEA_SetrangeStatus_FR_Center(uint8_t value)/*yangxl */
{
    DearangeStatus_FR_Center = value;
}
uint8_t DEA_GetrangeStatus_FR_Center(void)
{
    return DearangeStatus_FR_Center;
}

/************Sig rangeStatus_FR_Left*****************/
void DEA_SetrangeStatus_FR_Left(uint8_t value)/*yangxl */
{
    DearangeStatus_FR_Left = value;
}
uint8_t DEA_GetrangeStatus_FR_Left(void)
{
    return DearangeStatus_FR_Left;
}

/************Sig rangeStatus_FR_Right*****************/
void DEA_SetrangeStatus_FR_Right(uint8_t value)/*yangxl */
{
    DearangeStatus_FR_Right = value;
}
uint8_t DEA_GetrangeStatus_FR_Right(void)
{
    return DearangeStatus_FR_Right;
}
/************Sig A_FR_Center*****************/
void DEA_SetA_FR_Center(uint8_t value)/*yangxl */
{
    DeaA_FR_Center = value;
}
uint8_t DEA_GetA_FR_Center(void)
{
    return DeaA_FR_Center;
}

/************Sig A_FR_CorLeft*****************/
void DEA_SetA_FR_CorLeft(uint8_t value)/*yangxl */
{
    DeaA_FR_CorLeft = value;
}
uint8_t DEA_GetA_FR_CorLeft(void)
{
    return DeaA_FR_CorLeft;
}
/************Sig A_FR_ConrRight*****************/
void DEA_SetA_FR_ConrRight(uint8_t value)/*yangxl */
{
    DeaA_FR_ConrRight = value;
}
uint8_t DEA_GetA_FR_ConrRight(void)
{
    return DeaA_FR_ConrRight;
}

/************Sig DetectDisplayRequest*****************/
void DEA_SetDetectDisplayRequest(uint8_t value) /*yangxl */
{
    DeaDetectDisplayRequest = value;
}
uint8_t DEA_GetDetectDisplayRequest(void)
{
    return DeaDetectDisplayRequest;
}

/************Sig ECO_DISP_RQ*****************/

void DEA_SetECO_DISP_RQ(uint8_t value) /*yangxl */
{
    DeaECO_DISP_RQ = value;
}

uint8_t DEA_GetECO_DISP_RQ(void)
{
    return DeaECO_DISP_RQ;
}

/************Sig GSINXTGP*****************/

void DEA_SetGSINXTGP(uint8_t value)
{
    DeaGSINXTGP = value;
}

uint8_t DEA_GetGSINXTGP(void)
{
    return DeaGSINXTGP;
}

/************Sig PEBS_INFO*****************/
void DEA_SetPEBS_INFO(uint8_t value) /*yangxl */
{
    DeaPEBS_INFO = value;
}
uint8_t DEA_GetPEBS_INFO(void)
{
    return DeaPEBS_INFO;
}
/************Sig WARN_VRU*****************/
void DEA_SetWARN_VRU(uint8_t value) /*yangxl */
{
    DeaWARN_VRU = value;
}
uint8_t DEA_GetWARN_VRU(void)
{
    return DeaWARN_VRU;
}

/************Sig MSG_VDC*****************/
void DEA_SetMSG_VDC(uint8_t value) /*yangxl */
{
    DeaMSG_VDC = value;
}

uint8_t DEA_GetMSG_VDC(void)
{
    return DeaMSG_VDC;
}

/************Sig PRE_WARN_FR*****************/
void DEA_SetPRE_WARN_FR(uint8_t value)  /*yangxl */
{
    DeaPRE_WARN_FR = value;
}
uint8_t DEA_GetPRE_WARN_FR(void)
{
    return DeaPRE_WARN_FR;
}
/************Sig AEB_ACTIVE*****************/
void DEA_SetAEB_ACTIVE(uint8_t value)  /*yangxl */
{
    DeaAEB_ACTIVE = value;
}
uint8_t DEA_GetAEB_ACTIVE(void)
{
    return DeaAEB_ACTIVE;
}

/************Sig WARN_LANE_L*****************/
void DEA_SetWARN_LANE_L(uint8_t value) /*yangxl */
{
    DeaWARN_LANE_L = value;
}
uint8_t DEA_GetWARN_LANE_L(void)
{
    return DeaWARN_LANE_L;
}

/************Sig WARN_LANE_R*****************/
void DEA_SetWARN_LANE_R(uint8_t value) /*yangxl */
{
    DeaWARN_LANE_R = value;
}
uint8_t DEA_GetWARN_LANE_R(void)
{
    return DeaWARN_LANE_R;
}

/************Sig LDW_INFO*****************/
void DEA_SetLDW_INFO(uint8_t value) /*yangxl */
{
    DeaLDW_INFO = value;
}
uint8_t DEA_GetLDW_INFO(void)
{
    return DeaLDW_INFO;
}

/************Sig LTP_FR*****************/

void DEA_SetLTP_FR(uint8_t value) /*yangxl */
{
    DeaLTP_FR = value;
}
uint8_t DEA_GetLTP_FR(void)
{
    return DeaLTP_FR;
}

/************Sig LTP_FL*****************/

void DEA_SetLTP_FL(uint8_t value) /*yangxl */
{
    DeaLTP_FL = value;
}
uint8_t DEA_GetLTP_FL(void)
{
    return DeaLTP_FL;
}

/************Sig LTP_RR*****************/
void DEA_SetLTP_RR(uint8_t value) /*yangxl */
{
    DeaLTP_RR = value;
}
uint8_t DEA_GetLTP_RR(void)
{
    return DeaLTP_RR;
}

/************Sig LTP_RL*****************/
void DEA_SetLTP_RL(uint8_t value) /*yangxl */
{
    DeaLTP_RL = value;
}
uint8_t DEA_GetLTP_RL(void)
{
    return DeaLTP_RL;
}


/************Sig SPDBLREQ*****************/
void DEA_SetSPDBLREQ(uint8_t value) /*yangxl */
{
    DeaSPDBLREQ = value;
}
uint8_t DEA_GetSPDBLREQ(void)
{
    return DeaSPDBLREQ;
}

/************Sig ASCDON_DISP*****************/
void DEA_SetASCDON_DISP(uint8_t value) /*yangxl */
{
    DeaASCDON_DISP = value;
}
uint8_t DEA_GetASCDON_DISP(void)
{
    return DeaASCDON_DISP;
}

/************Sig FT_FR*****************/
void DEA_SetFT_FR(uint8_t value) /*yangxl */
{
    DeaFT_FR = value;
}
uint8_t DEA_GetFT_FR(void)
{
    return DeaFT_FR;
}

/************Sig FT_FL*****************/
void DEA_SetFT_FL(uint8_t value) /*yangxl */
{
    DeaFT_FL = value;
}
uint8_t DEA_GetFT_FL(void)
{
    return DeaFT_FL;
}

/************Sig FT_RL*****************/
void DEA_SetFT_RL(uint8_t value) /*yangxl */
{
    DeaFT_RL = value;
}
uint8_t DEA_GetFT_RL(void)
{
    return DeaFT_RL;
}

/************Sig FT_RR*****************/

void DEA_SetFT_RR(uint8_t value) /*yangxl */
{
    DeaFT_RR = value;
}
uint8_t DEA_GetFT_RR(void)
{
    return DeaFT_RR;
}

/************Sig SRRL_SET*****************/
void DEA_SetSRRL_SET(uint8_t value)/*yangxl */
{
    DeaSRRL_SET = value;
}
uint8_t DEA_GetSRRL_SET(void)
{
    return DeaSRRL_SET;
}

/************Sig FAILD_DT*****************/
void DEA_SetFAILD_DT(uint8_t value)/*yangxl */
{
    DeaFAILD_DT = value;
}
uint8_t DEA_GetFAILD_DT(void)
{
    return DeaFAILD_DT;
}

/************Sig ALT_ST*****************/
void DEA_SetALT_ST(uint8_t value)/*yangxl */
{
    DeaALT_ST = value;
}
uint8_t DEA_GetALT_ST(void)
{
    return DeaALT_ST;
}

/************Sig BLOCK_DT*****************/
void DEA_SetBLOCK_DT(uint8_t value)/*yangxl */
{
    DeaBLOCK_DT = value;
}
uint8_t DEA_GetBLOCK_DT(void)
{
    return DeaBLOCK_DT;
}

/************Sig ALT_IND*****************/
void DEA_SetALT_IND(uint8_t value)/*yangxl */
{
    DeaALT_IND = value;
}
uint8_t DEA_GetALT_IND(void)
{
    return DeaALT_IND;
}


/************Sig SRRR_SET_R*****************/
void DEA_SetSRRR_SET_R(uint8_t value)/*yangxl */
{
    DeaSRRR_SET_R = value;
}
uint8_t DEA_GetSRRR_SET_R(void)
{
    return DeaSRRR_SET_R;
}

/************Sig FAILD_DT_R*****************/
void DEA_SetFAILD_DT_R(uint8_t value)/*yangxl */
{
    DeaFAILD_DT_R = value;
}
uint8_t DEA_GetFAILD_DT_R(void)
{
    return DeaFAILD_DT_R;
}

/************Sig ALT_ST_R*****************/
void DEA_SetALT_ST_R(uint8_t value)/*yangxl */
{
    DeaALT_ST_R = value;
}
uint8_t DEA_GetALT_ST_R(void)
{
    return DeaALT_ST_R;
}

/************Sig BLOCK_DT_R*****************/
void DEA_SetBLOCK_DT_R(uint8_t value)/*yangxl */
{
    DeaBLOCK_DT_R = value;
}
uint8_t DEA_GetBLOCK_DT_R(void)
{
    return DeaBLOCK_DT_R;
}


/************Sig ALT_IND_R*****************/
void DEA_SetALT_IND_R(uint8_t value)/*yangxl */
{
    DeaALT_IND_R = value;
}
uint8_t DEA_GetALT_IND_R(void)
{
    return DeaALT_IND_R;
}

/************Sig CAN_MASK*****************/
void DEA_SetCAN_MASK(uint8_t value)/*yangxl 2020-4-28 16:39:55 */
{
    DeaCAN_MASK = value;
}
uint8_t DEA_GetCAN_MASK(void)
{
    return DeaCAN_MASK;
}




#if 0
/************Sig TPMS_POSI_SYNC*****************/

void DEA_SetTPMS_POSI_SYNC(uint8_t value) /*yangxl */
{
    DeaTPMS_POSI_SYNC = value;
}

uint8_t DEA_GetTPMS_POSI_SYNC(void)
{
    return DeaTPMS_POSI_SYNC;
}
#endif

void DEA_SetRtcTime(ExternSTDType *pRtcTime)
{
	Dea_RtcTime = *pRtcTime;
}

ExternSTDType DEA_GetRtcTime(void)
{
	return Dea_RtcTime;
}

void DEA_SetIpcRecoverFlag(uint8_t value)
{
	DeaIpcRecoverFlag = value;
}

uint8_t DEA_GetIpcRecoverFlag(void)
{
	return DeaIpcRecoverFlag;
}




