/********************************************************************
File name: CanApp_PduChanged.c
Author: Stephen Du
Version: V1.0
Timestamp: 2019-03-08 13:01:38
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
/*===============================================================================*/
/*=========================== START: HEAD FILE INCLUDE ==========================*/
/*===============================================================================*/
#include "Com_If.h"
#include "CircularQueue.h"
#include "SimFlag.h"
#include "Com.h"
#include"CanApp.h"
#include "SpiIpc.h"
#include "Task_IpcApp_Cfg.h"
#include "dea.h"
#include "CanKey.h"
#include "pmStateMachine_If.h"
#include "SecondPanel.h"
/*===============================================================================*/
/*============================ END: HEAD FILE INCLUDE ===========================*/
/*===============================================================================*/
extern uint8 Com_RxBufChange[];
extern volatile uint16 Com_IpduBufChangeQueueCh;
typedef void (*CanAppPduProcMatrixType)(void);
static uint8 is_da_pduid = 0;
/*===============================================================================*/
/*============================ START: USER EDIT AREA ============================*/
/*===============================================================================*/
void CanApp_PduProc_LWSOUT_M2(void) //Can ID 0x2
{
	is_da_pduid = 1;
}

void CanApp_PduProc_ACU_GeneralStatus_6(void)
{

}

void CanApp_PduProc_ShBW_GeneralStatus(void)
{

}

void CanApp_PduProc_SCU_GeneralStatus(void)
{

}
void CanApp_PduProc_ECM_TorqueControl_RN2(void)
{
	uint8_t l_u8Value = 0;
	
	Com_ReceiveSignal(SIG_HND_IDSTSTS, &l_u8Value);		//591 project Idle state
	DEA_SetRawRevolutionIdleStat(l_u8Value);
	DEA_SetRawIdleStopStatusValue(l_u8Value);

#if 0
	if(l_u8Value == 0b1)//yangxl 2019-6-5 14:08:54
	{
		if(!SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b0)
	{
		if(SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(0);
		}
	}
#endif
	//add by tjy @2019-7-31
	PM_CheckISS_ACTSignal();
	
	is_da_pduid = 1;
}
void CanApp_PduProc_ECM_TorqueControl_RN1(void)//Can ID 0x180
{
	uint16_t l_u16Value = 0;
	uint32_t temp = 0;
	uint8_t l_u8Value = 0;

	is_da_pduid = 1;
	
	//static uint16_t Rpm_last = 0;

	Com_ReceiveSignal(SIG_HND_TACHO, &l_u16Value);
	temp = (l_u16Value*125)/1000;	
	DEA_SetRawTACHO(l_u16Value);

	if(l_u16Value != 0xFFFF)
	{
		//if((temp - Rpm_last) >= 7)	//if the Rpm change is more than 7	591 project require
		{
			DEA_SetRawEngineRevolution((uint16_t)temp);
			DEA_SetRawEngineRevolutionErrorStat(0);
		}

		DEA_SetTACHO(temp);
	}
	else
	{
		DEA_SetRawEngineRevolution(0);
		DEA_SetRawEngineRevolutionErrorStat(1);
		DEA_SetTACHO(0);
	}
	//Rpm_last = temp;	//update the oldest revolution

	Com_ReceiveSignal(SIG_HND_RawPedal_APOFS, &l_u16Value); //591 project RawSensor
	temp = l_u16Value;

	DEA_SetGearRawSensorValue((uint8_t) temp);


    Com_ReceiveSignal(SIG_HND_SHIFT_IND, &l_u8Value);
	DEA_SetSHIFT_IND(l_u8Value);
}

void CanApp_PduProc_ECM_TorqueControl_N1(void)//Can ID 0x182
{
	is_da_pduid = 1;
}


void CanApp_PduProc_ECM_SystemControl_1f9(void)//Can ID 0x1f9
{
	//add by tjy @2019-7-31
	PM_CheckEngineStatusSignal();

	is_da_pduid = 1;
}

void CanApp_PduProc_USM_Fast_Frame(void) //Can ID 0x215
{
	is_da_pduid = 1;
}

void CanApp_PduProc_PEPS_Message_Sts(void) //Can ID 0x218
{

}
void CanApp_PduProc_Brake_StabilityControl_RN1(void) //Can ID 0x245
{
	is_da_pduid = 1;
}

void CanApp_PduProc_Front_Wheel_Speed_Frame(void) //Can ID 0x284
{
	uint16_t l_u16Value = 0;
	uint16_t temp = 0;

	is_da_pduid = 1;

	Com_ReceiveSignal(SIG_HND_VSO, &l_u16Value);	//591 project vehicle speed
	DEA_SetRawVehicleSpeedCanOutput(l_u16Value);
	temp = l_u16Value/10;	//expand 10times
#if 0
	if(temp <= 30)	//input value <= 3km/h ->value = 0
	{
		temp = 0;
	}
#endif

	if(l_u16Value !=0xFFFF)
	{
		DEA_SetRawVehicleSpeed(temp);
		DEA_SetRawVehicleSpeedErrorStat(0);
	}
	else
	{
		DEA_SetRawVehicleSpeed(0);
		DEA_SetRawVehicleSpeedErrorStat(1);
	}

}

void CanApp_PduProc_ITS_InfoStatus_N1(void) //Can ID 0x2B1
{
	uint8_t l_u8Value = 0;
	
	is_da_pduid = 1;

	Com_ReceiveSignal(SIG_HND_WARN_LANE_L , &l_u8Value);
	DEA_SetWARN_LANE_L(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_WARN_LANE_R , &l_u8Value);
	DEA_SetWARN_LANE_R(l_u8Value);
}

void CanApp_PduProc_ITS_InfoStatus_N2(void) //Can ID 0x2B7
{
	uint8_t l_u8Value = 0;
	
	is_da_pduid = 1;

	Com_ReceiveSignal(SIG_HND_AEB_ACTIVE , &l_u8Value);
	DEA_SetAEB_ACTIVE(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_PRE_WARN_FR , &l_u8Value);
	DEA_SetPRE_WARN_FR(l_u8Value);
}

#if 0
void CanApp_PduProc_ECM_to_ITM2(void) //Can ID 0x2C6
{
	is_da_pduid = 1;
}
#endif
void CanApp_PduProc_SRR_Left(void) //Can ID 0x2C8
{
	uint8_t l_u8Value = 0;
	Com_RxSig_GetSRRL_SET(&l_u8Value);
	if(l_u8Value == 0b10) //yangxl 2019-6-5 09:53:51
	{
		if(!SysSettingBSWSwitchGet())
		{
			SysSettingBSWSwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b01)
	{
		if(SysSettingBSWSwitchGet())
		{
			SysSettingBSWSwitchUpdate(0);
		}
	}
    is_da_pduid = 1;

	Com_ReceiveSignal(SIG_HND_SRRL_SET , &l_u8Value);
	DEA_SetSRRL_SET(l_u8Value);

	Com_ReceiveSignal(SIG_HND_FAILD_DT , &l_u8Value);
	DEA_SetFAILD_DT(l_u8Value);

	Com_ReceiveSignal(SIG_HND_ALT_ST , &l_u8Value);
	DEA_SetALT_ST(l_u8Value);

	Com_ReceiveSignal(SIG_HND_BLOCK_DT , &l_u8Value);
	DEA_SetBLOCK_DT(l_u8Value);

	Com_ReceiveSignal(SIG_HND_ALT_IND , &l_u8Value);
	DEA_SetALT_IND(l_u8Value);
}

void CanApp_PduProc_SRR_Right(void) //Can ID 0x2C9
{
	uint8_t l_u8Value = 0;
	
    is_da_pduid = 1;

	Com_ReceiveSignal(SIG_HND_SRRR_SET_R , &l_u8Value);
	DEA_SetSRRR_SET_R(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_FAILD_DT_R , &l_u8Value);
	DEA_SetFAILD_DT_R(l_u8Value);

	Com_ReceiveSignal(SIG_HND_ALT_ST_R , &l_u8Value);
	DEA_SetALT_ST_R(l_u8Value);

	Com_ReceiveSignal(SIG_HND_BLOCK_DT_R , &l_u8Value);
	DEA_SetBLOCK_DT_R(l_u8Value);

	Com_ReceiveSignal(SIG_HND_ALT_IND_R , &l_u8Value);
	DEA_SetALT_IND_R(l_u8Value);
}

void CanApp_PduProc_SMART_bcmMSG_PUSH(void) //Can ID 0x351
{
	uint8_t l_u8Value = 0;

	is_da_pduid = 1;

	Com_RxSig_GetSTARTER_CONT_STA(&l_u8Value);
	DEA_SetStaterControlStatus(l_u8Value);
	
	//add by tjy @2019-7-31
	PM_CheckRmtStartSignal();
}

void CanApp_PduProc_IKEY_352(void) //Can ID 0x352
{
	is_da_pduid = 1;
}

void CanApp_PduProc_Brake_GeneralData(void) //Can ID 0x354
{
	uint32 l_data = 0;
	uint8_t l_u8Value = 0;
	is_da_pduid = 1;
	Com_RxSig_GetVDC_LAMP(&l_u8Value);

	DEA_SetVDC_LAMP(l_u8Value);
	//Com_RxSig_GetMETER_TEST(&l_data);
	//DEA_SetHardwareStatus(l_data);
}

void CanApp_PduProc_BCMMSG1(void) //Can ID 0x358
{
	uint8_t l_u8value = 0;

	is_da_pduid = 1;
	
	Com_RxSig_GetCLUSTER_ILL_REQ(&l_u8value);
	DEA_SetCLUSTER_ILL_REQ(l_u8value);
}

void CanApp_PduProc_BCM_SystemControl_2(void) //Can ID 0x35A
{
	is_da_pduid = 1;
}

void CanApp_PduProc_BCM_SystemControl(void) //Can ID 0x35D
{
	is_da_pduid = 1;
	//PM_CheckRmtStartSignal();
}

void CanApp_PduProc_TPMS_GeneralStatus_BCM(void) //Can ID 0x385
{
	uint8_t l_u8value = 0u; 
	
	is_da_pduid = 1;
	Com_ReceiveSignal(SIG_HND_TPD_FR , &l_u8value);
	DEA_SetTPD_FR(l_u8value);
	Com_ReceiveSignal(SIG_HND_TPD_FL , &l_u8value);
	DEA_SetTPD_FL(l_u8value);
	Com_ReceiveSignal(SIG_HND_TPD_RR , &l_u8value);
	DEA_SetTPD_RR(l_u8value);
	Com_ReceiveSignal(SIG_HND_TPD_RL , &l_u8value);
	DEA_SetTPD_RL(l_u8value);
	
	Com_ReceiveSignal(SIG_HND_TS_FR , &l_u8value);
	DEA_SetTS_FR(l_u8value);
	Com_ReceiveSignal(SIG_HND_TS_FL , &l_u8value);
	DEA_SetTS_FL(l_u8value);
	Com_ReceiveSignal(SIG_HND_TS_RR , &l_u8value);
	DEA_SetTS_RR(l_u8value);
	Com_ReceiveSignal(SIG_HND_TS_RL , &l_u8value);
	DEA_SetTS_RL(l_u8value);
	Com_ReceiveSignal(SIG_HND_D_STYLE , &l_u8value);
	DEA_SetD_STYLE(l_u8value);

	Com_ReceiveSignal(SIG_HND_LTP_FL , &l_u8value);
	DEA_SetLTP_FL(l_u8value);
	Com_ReceiveSignal(SIG_HND_LTP_FR , &l_u8value);
	DEA_SetLTP_FR(l_u8value);
	Com_ReceiveSignal(SIG_HND_LTP_RL , &l_u8value);
	DEA_SetLTP_RL(l_u8value);
	Com_ReceiveSignal(SIG_HND_LTP_RR , &l_u8value);
	DEA_SetLTP_RR(l_u8value);

	Com_ReceiveSignal(SIG_HND_FT_FL , &l_u8value);
	DEA_SetFT_FL(l_u8value);
	Com_ReceiveSignal(SIG_HND_FT_FR , &l_u8value);
	DEA_SetFT_FR(l_u8value);
	Com_ReceiveSignal(SIG_HND_FT_RL , &l_u8value);
	DEA_SetFT_RL(l_u8value);
	Com_ReceiveSignal(SIG_HND_FT_RR , &l_u8value);
	DEA_SetFT_RR(l_u8value);
	
}

/*
void CanApp_PduProc_ATCVT_to_MMI_N1(void)
{
	uint16_t l_u16Value = 0;
	uint32_t temp = 0;

	Com_ReceiveSignal( SIG_HND_TACHO_D, &l_u16Value);
	temp = (l_u16Value*125)/1000;	

	if(l_u16Value != 0xFFFF)
	{
		DEA_SetRawEngineRevolution_3EC((uint16_t)temp);
		DEA_SetRawEngineRevolutionErrorStat_3EC(0);
	}
	else
	{
		DEA_SetRawEngineRevolution_3EC(0);
		DEA_SetRawEngineRevolutionErrorStat_3EC(1);

	}

}
*/
void CanApp_PduProc_ATCVT_to_MMI_RN1(void) //Can ID 0x421
{
	uint8_t l_u16Value = 0;
	uint8_t temp = 0;

	is_da_pduid = 1;

	Com_ReceiveSignal( SIG_HND_IND_RNG, &l_u16Value);	//591 project ID:421 shift position
	temp = l_u16Value;	

	DEA_SetRawEcoShiftPosition(temp);

	
	//Com_ReceiveSignal( SIG_HND_TM_TYPE, &l_u16Value);	//591 project ID:421 TM_Type
	//temp = l_u16Value;

	//DEA_SetRawGearATCheakValue(temp);
	
	#if 0
	Com_ReceiveSignal( SIG_HND_TOWMODEREQ, &l_u16Value);	//591 project ID:421 gear indication allow
	temp = l_u16Value;	

	DEA_SetRawGearIdicateAllow(temp);
	#endif
	

	Com_ReceiveSignal( SIG_HND_IND_RNG, &l_u16Value);	//591 project ID:421 gear value
	temp = l_u16Value;	

	DEA_SetRawGearValue(temp);

	
	Com_ReceiveSignal( SIG_HND_RGB_GP_REFUSE, &l_u16Value);	//591 project ID:421  Gear blink value
	temp = l_u16Value;	

	DEA_SetRawGearBlinkValue(temp);

}
#if 0
void CanApp_PduProc_AVM_Status(void) 
{
	//is_da_pduid = 1;
}
#endif
void CanApp_PduProc_ITS_InfoStatus_N3_7b(void)//Can ID 0x4CB
{
	uint8_t l_u8Value = 0;
	is_da_pduid = 1;
	Com_RxSig_GetSET_LDW_STA(&l_u8Value); //yangxl
	if(l_u8Value == 0b10)
	{
		if(!SysSettingLDWSwitchGet())
		{
			SysSettingLDWSwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b01)
	{
		if(SysSettingLDWSwitchGet())
		{
			SysSettingLDWSwitchUpdate(0);
		}
	}

	Com_RxSig_GetSET_LKA_STA(&l_u8Value); //yangxl
	if(l_u8Value == 0b10)
	{
		if(!SysSettingLKASwitchGet())
		{
			SysSettingLKASwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b01)
	{
		if(SysSettingLKASwitchGet())
		{
			SysSettingLKASwitchUpdate(0);
		}
	}

	Com_ReceiveSignal(SIG_HND_LDW_INFO , &l_u8Value);
	DEA_SetLDW_INFO(l_u8Value);
}

void CanApp_PduProc_ITS_InfoStatus_N2_1(void) //Can ID 0x4CC
{
	
	uint8_t l_u8Value = 0;
	is_da_pduid = 1;
	Com_RxSig_GetSET_PEBS_STA(&l_u8Value);
	if(l_u8Value == 0b10)
	{
		if(!SysSettingFEBSwitchGet())
		{
			SysSettingFEBSwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b01)
	{
		if(SysSettingFEBSwitchGet())
		{
			SysSettingFEBSwitchUpdate(0);
		}
	}
	
	Com_ReceiveSignal(SIG_HND_PEBS_INFO , &l_u8Value);
	DEA_SetPEBS_INFO(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_WARN_VRU , &l_u8Value);
	DEA_SetWARN_VRU(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_MSG_VDC , &l_u8Value);
	DEA_SetMSG_VDC(l_u8Value);

}

void CanApp_PduProc_RETREIVE_ASSIST_POWER_48V(void)
{

}

void CanApp_PduProc_CLIMATE_ControlData_F(void)
{
	uint16_t l_u16Value = 0;
	uint16_t temp = 0;

	Com_ReceiveSignal(SIG_HND_OutsideTemp_AD, &l_u16Value);	//591 project OAT
	DEA_SetOAT_CanRawValue(l_u16Value);
	temp = (l_u16Value*5);	//expand 10 times

	if(l_u16Value != 0xFF)
	{
		DEA_SetOAT_RawValue(temp);
		DEA_SetOAT_RawValid(1);
	}
	else
	{
		DEA_SetOAT_RawValue(0);
		DEA_SetOAT_RawValid(0);
	}

	
}

void CanApp_PduProc_ECM_GeneralStatus(void) //Can ID 0x551
{
	uint16_t l_u16Value = 0;
	uint16_t temp = 0;
	uint8_t l_u8value = 0;

	is_da_pduid = 1;
	
	Com_ReceiveSignal(SIG_HND_TWN, &l_u16Value);	//591 project Coolant tempreture
	temp = l_u16Value;

	if(l_u16Value !=0xFF)
	{
		DEA_SetCoolantTempRaw(temp);
		DEA_SetCoolantTempRawValid(1);
	}
	else
	{
		DEA_SetCoolantTempRaw(0);
		DEA_SetCoolantTempRawValid(0);
	}

	Com_ReceiveSignal(SIG_HND_TIM , &l_u8value);	//591 project fuel injection	

	DEA_SetFuelInjection(l_u8value);
	DEA_SetRawFuelInjectionErrStat(0);	//valid

	/*if(l_u16Value != 0xFFFF)
	{
		DEA_SetFuelInjection(temp);
		DEA_SetRawFuelInjectionErrStat(0);	//valid
	}
	else
	{
		DEA_SetFuelInjection(0);
		DEA_SetRawFuelInjectionErrStat(1);	//invalid
	}*/


	Com_ReceiveSignal(SIG_HND_ELAMP , &l_u16Value);	//591 project Eco Mode Lamp
	temp = l_u16Value;		

	DEA_SetRawEcoMode((uint8_t)temp);

	if(l_u16Value == 0b0)
	{
		if(SysSettingECOSwitchGet())
		{
			//SysSettingECOSwitchSet(0);
			SysSettingECOSwitchUpdate(0U);
		}
	}
	else if(l_u16Value == 0b01)
	{
		if(!SysSettingECOSwitchGet())
		{
			//SysSettingECOSwitchSet(1);
			SysSettingECOSwitchUpdate(1U);
		}
	}

	Com_RxSig_GetENG_STA(&l_u8value);
	DEA_SetEngineStatusCopy(l_u8value);

	Com_ReceiveSignal(SIG_HND_SPDREQ , &l_u8value);	//yangxl
	DEA_SetACCSPDREQ(l_u8value); //yangxl

	Com_ReceiveSignal(SIG_HND_SPDBLREQ , &l_u8value);	//yangxl
	DEA_SetSPDBLREQ(l_u8value);
	Com_ReceiveSignal(SIG_HND_ASCDON_DISP , &l_u8value);	//yangxl
	DEA_SetASCDON_DISP(l_u8value); //yangxl

	Com_ReceiveSignal(SIG_HND_CANMASK , &l_u8value);	//yangxl 2020-4-28 16:38:14
	DEA_SetCAN_MASK(l_u8value);

}

void CanApp_PduProc_DCM_GeneralStatus_5(void) //Can ID 0x563
{
	uint8 msg;

	is_da_pduid = 1;

	//Com_RxSig_GetE_CALL_display_request(&msg);
	Com_RxSig_GetDA_Muteout_Request(&msg);
	if(msg == 0x1){
		api_audio_enter_ecall();
	}else{
		api_audio_leave_ecall();
	}

	//add by tjy @2019-7-31
	PM_CheckDAwakeupRequestSignal();
	
	//add by tjy @2019-9-10
	PM_CheckAVM_Outdoor_Photo_RequestSignal();

	return;
}
#if 0
void CanApp_PduProc_DCM_GeneralStatus_5_1(void)
{

}
#endif
void CanApp_PduProc_PBD_Status(void) //Can ID 0x572
{
	is_da_pduid = 1;
}

void CanApp_PduProc_SONAR_GeneralStatus_8(void)  //Can ID 0x57A
{
	uint8_t l_u8Value = 0;
	Com_RxSig_GetSTS_SYS_MODE(&l_u8Value);
	if(l_u8Value == 0)
	{
		if(!SysSettingParkAssistantGet())
		{
			SysSettingParkAssistantUpdate(1);
		}
	}
	else if(l_u8Value == 0b010)
	{
		if(SysSettingParkAssistantGet())
		{
			SysSettingParkAssistantUpdate(0);
		}
	}

	Com_RxSig_GetSETTEING_EAPM(&l_u8Value);
	if(l_u8Value == 0b001)
	{
		if(SysSettingEAPMSwitchGet())
		{
			SysSettingEAPMSwitchUpdate(0);
		}
	}
	else if(l_u8Value == 0b010)
	{
		if(!SysSettingEAPMSwitchGet())
		{
			SysSettingEAPMSwitchUpdate(1);
		}
	}

	Com_RxSig_GetSTS_SYS_MODE(&l_u8Value);//yangxl 2019-11-19 10:24:54
	if((l_u8Value == 0b000)||(l_u8Value == 0b001)||(l_u8Value == 0b100)||(l_u8Value == 0b101))
	{
		if(SysSettingUPASwitchGet() == 0)
		{
			SysSettingUPASwitchUpdate(1);
		}
	}else if((l_u8Value == 0b010) || (l_u8Value == 0b011))
	{
		if(SysSettingUPASwitchGet() == 1)
		{
			SysSettingUPASwitchUpdate(0);
		}
	}
		
	Com_ReceiveSignal(SIG_HND_REQ_DISP_MET , &l_u8Value);	//yangxl
	DEA_SetREQ_DISP_MET(l_u8Value);

	Com_ReceiveSignal(SIG_HND_SONAR_S_REQ , &l_u8Value);	//yangxl
	DEA_SetSONAR_S_REQ(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_RR_CEN , &l_u8Value);	//yangxl
	DEA_SetRR_CEN(l_u8Value);

	Com_ReceiveSignal(SIG_HND_A_RR_CEN , &l_u8Value);	//yangxl
	DEA_SetA_RR_CEN(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_RR_L , &l_u8Value);	//yangxl
	DEA_SetRR_L(l_u8Value);

	Com_ReceiveSignal(SIG_HND_A_RR_L , &l_u8Value);	//yangxl
	DEA_SetA_RR_L(l_u8Value);

	Com_ReceiveSignal(SIG_HND_RR_R , &l_u8Value);	//yangxl
	DEA_SetRR_R(l_u8Value);

	Com_ReceiveSignal(SIG_HND_A_RR_R , &l_u8Value);	//yangxl
	DEA_SetA_RR_R(l_u8Value);

	Com_ReceiveSignal(SIG_HND_STS_DISTANCE , &l_u8Value);//yangxl
	DEA_SetSTS_DISTANCE(l_u8Value);

	
	Com_ReceiveSignal(SIG_HND_FR_CEN , &l_u8Value);//yangxl
	DEA_SetrangeStatus_FR_Center(l_u8Value);
	

	Com_ReceiveSignal(SIG_HND_FR_L , &l_u8Value);//yangxl
	DEA_SetrangeStatus_FR_Left(l_u8Value);

	Com_ReceiveSignal(SIG_HND_FR_R , &l_u8Value);//yangxl
	DEA_SetrangeStatus_FR_Right(l_u8Value);
	
	Com_ReceiveSignal(SIG_HND_A_FR_CEN , &l_u8Value);//yangxl
	DEA_SetA_FR_Center(l_u8Value);

	Com_ReceiveSignal(SIG_HND_A_FR_L , &l_u8Value);//yangxl
	DEA_SetA_FR_CorLeft(l_u8Value);

	Com_ReceiveSignal(SIG_HND_A_FR_R , &l_u8Value);//yangxl
	DEA_SetA_FR_ConrRight(l_u8Value);
	/*
	//Can_RxSig_GetDetectDisplayRequest(&l_u8Value);
	Com_ReceiveSignal(SIG_HND_DetectDisplayRequest , &l_u8Value);//yangxl
	DEA_SetDetectDisplayRequest(l_u8Value);
	*/


	
	is_da_pduid = 1;
}

void CanApp_PduProc_ECM_to_ITM(void)          //Can ID 0x580
{
	uint8_t l_u8Value = 0;
	
	is_da_pduid = 1;
	
	Com_ReceiveSignal(SIG_HND_GSINXTGP , &l_u8Value);//yangxl
	DEA_SetGSINXTGP(l_u8Value);
}

void CanApp_PduProc_EPB_SystemControl(void) //Can ID 0x58A
{
	uint8_t l_u8Value = 0;

	is_da_pduid = 1;

	Com_RxSig_GetEPB_WORKMODE(&l_u8Value);
	DEA_SetEPB_WORKMODE(l_u8Value);

	Com_RxSig_GetEPB_STAT(&l_u8Value);
	DEA_SetEPB_STAT(l_u8Value);
	
	Com_RxSig_GetDPB_STAT(&l_u8Value);
	DEA_SetDynamicBrakingStatus(l_u8Value);
}

void CanApp_PduProc_DVR_Request(void) //Can ID 0x5B1
{
	is_da_pduid = 1;
}

void CanApp_PduProc_AMP_5b9(void) //Can ID 0x5B9
{
	is_da_pduid = 1;
}

void CanApp_PduProc_AMP_Version_5c2(void) //Can ID 0x5C2
{
	is_da_pduid = 1;
}

void CanApp_PduProc_S_BOX_ControlData_SetPM(void) //Can ID 0x5C8
{
	uint16 sGetValue = 0;

	is_da_pduid = 1;
	getCanSwkeySigVal();

	Com_RxSig_GetINNER_PM25_VALUE(&sGetValue);
	GetSecondPanelPM25Value(SecondPanel_Inside, &sGetValue);
}

void CanApp_PduProc_S_BOX_ControlData_Seat(void) //Can ID 0x5C9
{
	is_da_pduid = 1;
}

void CanApp_PduProc_S_BOX_ControlData_enjoy(void) //Can ID 0x5CA
{
	is_da_pduid = 1;
}

void CanApp_PduProc_S_BOX_ControlData_AC(void) //Can ID 0x5CB
{
	is_da_pduid = 1;
}
#if 0
void CanApp_PduProc_SHVM_SeatVentSt(void)
{
	//is_da_pduid = 1;
}
#endif
void CanApp_PduProc_EPS_GeneralStatus(void) //Can ID 0x5E4
{
	is_da_pduid = 1;
}

void CanApp_PduProc_LCM_5E6(void) //Can ID 0x5E6
{
	is_da_pduid = 1;
}

void CanApp_PduProc_LCM_5E7(void) //Can ID 0x5E7
{
	is_da_pduid = 1;
}

void CanApp_PduProc_TPMS_GeneralStatus2(void)
{

}

void CanApp_PduProc_APA_HMI_system(void) //Can ID 0x5E9
{
	is_da_pduid = 1;

}
void CanApp_PduProc_DVR_STS(void) //Can ID 0x60A
{
	is_da_pduid = 1;
}

void CanApp_PduProc_BCM_GeneralStatus(void) //Can ID 0x60D
{
	is_da_pduid = 1;

	//add by tjy @2019-7-31
	PM_CheckDoorSwitchesStateSignal();
}

void CanApp_PduProc_USM_GeneralStatus(void) //Can ID 0x625
{
	is_da_pduid = 1;
}

void CanApp_PduProc_LowWarningSignal628(void)
{

}

void CanApp_PduProc_LOW_WNG_STATUS_629(void)
{

}

void CanApp_PduProc_BSW_GeneralStatus_3(void)
{
	#if 0
	uint8_t l_u8Value = 0;
	Com_RxSig_GetSET_W_BS(&l_u8Value);
	if(l_u8Value == 0b10) //yangxl 2019-6-5 09:53:51
	{
		if(!SysSettingBSWSwitchGet())
		{
			SysSettingBSWSwitchUpdate(1);
		}
	}
	else if(l_u8Value == 0b01)
	{
		if(SysSettingBSWSwitchGet())
		{
			SysSettingBSWSwitchUpdate(0);
		}
	}
	#endif
	is_da_pduid = 1;
}

void CanApp_PduProc_ECCSMSG8_OBD_IS(void)
{
	uint8_t	l_u16Value = 0;	
	uint8_t temp = 0;
#if 0	//yangxl 2019-7-3 09:49:29
	Com_ReceiveSignal(SIG_HND_IDSTSTS, &l_u16Value);		//591 project Idle state
	temp = l_u16Value;
	DEA_SetRawRevolutionIdleStat(temp);
#endif	

	//Com_ReceiveSignal(SIG_HND_ECO_DISP_RQ, &l_u16Value); //yangxl 2019-8-20 14:39:45
	//DEA_SetECO_DISP_RQ(l_u16Value);

	Com_ReceiveSignal(SIG_HND_ISINDMD, &l_u16Value);		//591 project Idle request
	temp = l_u16Value;
	DEA_SetRawRevolutionIdleRequest(temp);

	//Com_ReceiveSignal(SIG_HND_ECO_DISP_RQ , &l_u16Value);		//591 project AccelGuide x_InputData
	//temp = l_u16Value;
	//DEA_SetRawAccelGuide_X(temp);
#if 0	//yangxl 2019-7-3 09:49:29
	Com_ReceiveSignal(SIG_HND_IDSTSTS , &l_u16Value);		//591 project Idle Stop Status
	temp = l_u16Value;
	DEA_SetRawIdleStopStatusValue(temp);
	
	if(l_u16Value == 0b1)//yangxl 2019-6-5 14:08:54
	{
		if(!SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(1);
		}
	}
	else if(l_u16Value == 0b0)
	{
		if(SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(0);
		}
	}
#endif
	
	Com_ReceiveSignal(SIG_HND_ISMSKSW ,&temp);
	DEA_SetISMSKSW(temp);

	if(temp == 0b1)//yangxl 2019-10-10 09:50:56
	{
		if(!SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(1);
		}
	}
	else if(temp == 0b0)
	{
		if(SysSettingISSSwitchGet())
		{
			SysSettingISSSwitchUpdate(0);
		}
	}
}

/*===============================================================================*/
/*============================= END: USER EDIT AREA =============================*/
/*===============================================================================*/
#define CAN_RXPDU_TOTAL_NUM 53

const CanAppPduProcMatrixType CanApp_PduProc_Matrix[CAN_RXPDU_TOTAL_NUM] =
{
	CanApp_PduProc_LWSOUT_M2,
	CanApp_PduProc_ACU_GeneralStatus_6,
	CanApp_PduProc_ShBW_GeneralStatus,
	CanApp_PduProc_SCU_GeneralStatus,
	CanApp_PduProc_ECM_TorqueControl_RN2,
	CanApp_PduProc_ECM_TorqueControl_RN1,
	CanApp_PduProc_ECM_TorqueControl_N1,
	CanApp_PduProc_ECM_SystemControl_1f9,
	CanApp_PduProc_USM_Fast_Frame,
	CanApp_PduProc_PEPS_Message_Sts,
	CanApp_PduProc_Brake_StabilityControl_RN1,
	CanApp_PduProc_Front_Wheel_Speed_Frame,
	CanApp_PduProc_ITS_InfoStatus_N1,
	CanApp_PduProc_ITS_InfoStatus_N2,
	CanApp_PduProc_SRR_Left,
	CanApp_PduProc_SRR_Right,
	CanApp_PduProc_SMART_bcmMSG_PUSH,
	CanApp_PduProc_IKEY_352,
	CanApp_PduProc_Brake_GeneralData,
	CanApp_PduProc_BCMMSG1,
	CanApp_PduProc_BCM_SystemControl_2,
	CanApp_PduProc_BCM_SystemControl,
	CanApp_PduProc_TPMS_GeneralStatus_BCM,
	CanApp_PduProc_ATCVT_to_MMI_RN1,
	CanApp_PduProc_ITS_InfoStatus_N3_7b,
	CanApp_PduProc_ITS_InfoStatus_N2_1,
	CanApp_PduProc_RETREIVE_ASSIST_POWER_48V,
	CanApp_PduProc_CLIMATE_ControlData_F,
	CanApp_PduProc_ECM_GeneralStatus,
	CanApp_PduProc_DCM_GeneralStatus_5,
	CanApp_PduProc_PBD_Status,
	CanApp_PduProc_SONAR_GeneralStatus_8,
	CanApp_PduProc_ECM_to_ITM,
	CanApp_PduProc_EPB_SystemControl,
	CanApp_PduProc_DVR_Request,
	CanApp_PduProc_AMP_5b9,
	CanApp_PduProc_AMP_Version_5c2,
	CanApp_PduProc_S_BOX_ControlData_SetPM,
	CanApp_PduProc_S_BOX_ControlData_Seat,
	CanApp_PduProc_S_BOX_ControlData_enjoy,
	CanApp_PduProc_S_BOX_ControlData_AC,
	CanApp_PduProc_EPS_GeneralStatus,
	CanApp_PduProc_LCM_5E6,
	CanApp_PduProc_LCM_5E7,
	CanApp_PduProc_TPMS_GeneralStatus2,
	CanApp_PduProc_APA_HMI_system,
	CanApp_PduProc_DVR_STS,
	CanApp_PduProc_BCM_GeneralStatus,
	CanApp_PduProc_USM_GeneralStatus,
	CanApp_PduProc_LowWarningSignal628,
	CanApp_PduProc_LOW_WNG_STATUS_629,
	CanApp_PduProc_BSW_GeneralStatus_3,
	CanApp_PduProc_ECCSMSG8_OBD_IS,
};


#if 0
const uint8_t CanMsgSendToIPC[]= {

	1, //IPDU_HND_ACU_GeneralStatus_6 (0U)
	1, //IPDU_HND_ECM_TorqueControl_RN1 (1U)
	1, //IPDU_HND_USM_Fast_Frame (2U)
	1, //IPDU_HND_Brake_StabilityControl_RN1 (3U)
	0, //IPDU_HND_Front_Wheel_Speed_Frame (4U)
	1, //IPDU_HND_ITS_InfoStatus_N1 (5U)
	1, //IPDU_HND_ITS_InfoStatus_N2 (6U)
	1, //IPDU_HND_SMART_bcmMSG_PUSH (7U)
	1, //IPDU_HND_Brake_GeneralData (8U)
	1, //IPDU_HND_BCMMSG1 (9U)
	1, //IPDU_HND_BCM_SystemControl (10U)
	1, //IPDU_HND_TPMS_GeneralStatus_BCM (11U)
	0, //IPDU_HND_ATCVT_to_MMI_N1 (12U)
	1, //IPDU_HND_ATCVT_to_MMI_RN1 (13U)
	1, //IPDU_HND_ITS_InfoStatus_N3_7b (14U)
	1, //IPDU_HND_ITS_InfoStatus_N2_1 (15U)
	1, //IPDU_HND_CLIMATE_ControlData_F (16U)
	1, //IPDU_HND_ECM_GeneralStatus (17U)
	1, //IPDU_HND_SONAR_GeneralStatus_8 (18U)
	1, //IPDU_HND_ECM_to_ITM_8 (19U)
	1, //IPDU_HND_EPB_SystemControl (20U)
	1, //IPDU_HND_EPS_GeneralStatus (21U)
	1, //IPDU_HND_TPMS_GeneralStatus2 (22U)
	1, //IPDU_HND_BCM_GeneralStatus (23U)
	1, //IPDU_HND_USM_GeneralStatus (24U)
	1, //IPDU_HND_BSW_GeneralStatus_3 (25U)
	1, //IPDU_HND_ECCSMSG8_OBD_IS (26U)
	1, //IPDU_HND_CLUSTER_BasicInfo_1 (27U)
	1, //IPDU_HND_METMSG1 (28U)
	0, //IPDU_HND_CLUSTER_BasicInfo_2 (29U)
	0, //IPDU_HND_CLUSTER_GeneralInfomation_N2_4b (30U)
	0, //IPDU_HND_CLUSTER_BasicInfo_3 (31U)
	0, //IPDU_HND_CLUSTER_GeneralStatus (32U)
	0, //IPDU_HND_CLUSTER_WakeUpFrame (33U)
};


const uint8_t CanMsgLength[]= {

	6, //0X2A PDU:0
	8, //0X180 PDU:1
	6, //0X215 PDU:2
	8, //0X245 PDU:3
	8, //0X284 PDU:4
	8, //0X2B1 PDU:5
	8, //0X2B7 PDU:6
	8, //0X351 PDU:7
	8, //0X354 PDU:8
	8, //0X358 PDU:9
	8, //0X35D PDU:10
	8, //0X385 PDU:11
	2, //0X3EC PDU:12
	3, //0X421 PDU:13
	8, //0X4CB PDU:14
	8, //0X4CC PDU:15
	8, //0X54C PDU:16
	8, //0X551 PDU:17
	8, //0X57A PDU:18
	8, //0X580 PDU:19
	8, //0X58A PDU:20
	3, //0X5E4 PDU:21
	5, //0X5E8 PDU:22
	8, //0X60D PDU:23
	8, //0X625 PDU:24
	3, //0X665 PDU:25
	8, //0X6E2 PDU:26
	8, //0X280 PDU:27
	8, //0X2DE PDU:28
	8, //0X355 PDU:29
	8, //0X4F2 PDU:30
	8, //0X4F8 PDU:31
	8, //0X5C5 PDU:32
	1, //0X602 PDU:33
};

#else

/*
const uint8_t CanMsgSendToIPC[]= {

	1, //IPDU_HND_LWSOUT_M2 (0U)
	1, //IPDU_HND_ACU_GeneralStatus_6 (1U)
	1, //IPDU_HND_ShBW_GeneralStatus (2U)
	1, //IPDU_HND_SCU_GeneralStatus (3U)
	1, //IPDU_HND_ECM_TorqueControl_RN2 (4U)
	1, //IPDU_HND_ECM_TorqueControl_RN1 (5U)
	1, //IPDU_HND_ECM_TorqueControl_N1 (6U)
	1, //IPDU_HND_ECM_SystemControl_1f9 (7U)
	1, //IPDU_HND_USM_Fast_Frame (8U)
	1, //IPDU_HND_PEPS_Message_Sts (9U)
	1, //IPDU_HND_Brake_StabilityControl_RN1 (10U)
	1, //IPDU_HND_Front_Wheel_Speed_Frame (11U)
	1, //IPDU_HND_ITS_InfoStatus_N1 (12U)
	1, //IPDU_HND_ITS_InfoStatus_N2 (13U)
	1, //IPDU_HND_SRR_Left (14U)
	1, //IPDU_HND_SRR_Right (15U)
	1, //IPDU_HND_SMART_bcmMSG_PUSH (16U)
	1, //IPDU_HND_IKEY_352 (17U)
	1, //IPDU_HND_Brake_GeneralData (18U)
	1, //IPDU_HND_BCMMSG1 (19U)
	1, //IPDU_HND_BCM_SystemControl_2 (20U)
	1, //IPDU_HND_BCM_SystemControl (21U)
	1, //IPDU_HND_TPMS_GeneralStatus_BCM (22U)
	//1, //IPDU_HND_ATCVT_to_MMI_N1 (23U)
	1, //IPDU_HND_ATCVT_to_MMI_RN1 (24U)
	1, //IPDU_HND_ITS_InfoStatus_N3_7b (25U)
	1, //IPDU_HND_ITS_InfoStatus_N2_1 (26U)
	1, //IPDU_HND_RETREIVE_ASSIST_POWER_48V (27U)
	1, //IPDU_HND_CLIMATE_ControlData_F (28U)
	1, //IPDU_HND_ECM_GeneralStatus (29U)
	1, //IPDU_HND_DCM_GeneralStatus_5 (30U)
	1, //IPDU_HND_PBD_Status (31U)
	1, //IPDU_HND_SONAR_GeneralStatus_8 (32U)
	1, //IPDU_HND_ECM_to_ITM (33U)
	1, //IPDU_HND_EPB_SystemControl (34U)
	1, //IPDU_HND_DVR_Request (35U)
	1, //IPDU_HND_AMP_5b9 (36U)
	1, //IPDU_HND_AMP_Version_5c2 (37U)
	1, //IPDU_HND_S_BOX_ControlData_SetPM (38U)
	1, //IPDU_HND_S_BOX_ControlData_Seat (39U)
	1, //IPDU_HND_S_BOX_ControlData_enjoy (40U)
	1, //IPDU_HND_S_BOX_ControlData_AC (41U)
	1, //IPDU_HND_EPS_GeneralStatus (42U)
	1, //IPDU_HND_LCM_5E6 (43U)
	1, //IPDU_HND_LCM_5E7 (44U)
	1, //IPDU_HND_TPMS_GeneralStatus2 (45U)
	1, //IPDU_HND_APA_HMI_system (46U)
	1, //IPDU_HND_DVR_STS (47U)
	1, //IPDU_HND_BCM_GeneralStatus (48U)
	1, //IPDU_HND_USM_GeneralStatus (49U)
	1, //IPDU_HND_LowWarningSignal628 (50U)
	1, //IPDU_HND_LOW_WNG_STATUS_629 (51U)
	1, //IPDU_HND_BSW_GeneralStatus_3 (52U)
	1, //IPDU_HND_ECCSMSG8_OBD_IS (53U)
	0, //IPDU_HND_CLUSTER_BasicInfo_1 (54U)
	0, //IPDU_HND_METMSG1 (55U)
	0, //IPDU_HND_CLUSTER_BasicInfo_2 (56U)
	0, //IPDU_HND_DA_Information (57U)
	0, //IPDU_HND_CLUSTER_GeneralInfomation_N2_4b (58U)
	0, //IPDU_HND_CLUSTER_BasicInfo_3 (59U)
	0, //IPDU_HND_SystemTimeInfo (60U)
	0, //IPDU_HND_AMP_5A2 (61U)
	0, //IPDU_HND_FAP_OperationCommand (62U)
	0, //IPDU_HND_DA_ControlData_PM (63U)
	0, //IPDU_HND_DA_ControlData_seat (64U)
	0, //IPDU_HND_CLIMATE_ControlData (65U)
	0, //IPDU_HND_CLUSTER_GeneralStatus (66U)
	0, //IPDU_HND_PBD_LCM_IKEY_VDC (67U)
	0, //IPDU_HND_IKEY_Display_SwitchStatus (68U)
	0, //IPDU_HND_TCU_DA_information (69U)
	0, //IPDU_HND_CLUSTER_WakeUpFrame (70U)
	0, //IPDU_HND_DVROperationCommand (71U)
	0, //IPDU_HND_DA_WakeUpFrame (72U)
};

const uint8_t CanMsgLength[]= {

	8, //0X2 PDU:0
	8, //0X2A PDU:1
	8, //0X11A PDU:2
	8, //0X11B PDU:3
	8, //0X160 PDU:4
	8, //0X180 PDU:5
	8, //0X182 PDU:6
	8, //0X1F9 PDU:7
	8, //0X215 PDU:8
	8, //0X218 PDU:9
	8, //0X245 PDU:10
	8, //0X284 PDU:11
	8, //0X2B1 PDU:12
	8, //0X2B7 PDU:13
	8, //0X2C8 PDU:14
	8, //0X2C9 PDU:15
	8, //0X351 PDU:16
	8, //0X352 PDU:17
	8, //0X354 PDU:18
	8, //0X358 PDU:19
	8, //0X35A PDU:20
	8, //0X35D PDU:21
	8, //0X385 PDU:22
	8, //0X3EC PDU:23
	8, //0X421 PDU:24
	8, //0X4CB PDU:25
	8, //0X4CC PDU:26
	8, //0X502 PDU:27
	8, //0X54C PDU:28
	8, //0X551 PDU:29
	8, //0X563 PDU:30
	8, //0X572 PDU:31
	8, //0X57A PDU:32
	8, //0X580 PDU:33
	8, //0X58A PDU:34
	8, //0X5B1 PDU:35
	8, //0X5B9 PDU:36
	8, //0X5C2 PDU:37
	8, //0X5C8 PDU:38
	8, //0X5C9 PDU:39
	8, //0X5CA PDU:40
	8, //0X5CB PDU:41
	8, //0X5E4 PDU:42
	8, //0X5E6 PDU:43
	8, //0X5E7 PDU:44
	8, //0X5E8 PDU:45
	8, //0X5E9 PDU:46
	8, //0X60A PDU:47
	8, //0X60D PDU:48
	8, //0X625 PDU:49
	8, //0X628 PDU:50
	8, //0X629 PDU:51
	8, //0X665 PDU:52
	8, //0X6E2 PDU:53
	8, //0X280 PDU:54
	8, //0X2DE PDU:55
	8, //0X355 PDU:56
	8, //0X447 PDU:57
	8, //0X4F2 PDU:58
	8, //0X4F8 PDU:59
	8, //0X567 PDU:60
	8, //0X5A2 PDU:61
	8, //0X5A5 PDU:62
	8, //0X5A8 PDU:63
	8, //0X5B2 PDU:64
	8, //0X5B7 PDU:65
	8, //0X5C5 PDU:66
	4, //0X5E2 PDU:67
	8, //0X5E5 PDU:68
	8, //0X5FE PDU:69
	1, //0X602 PDU:70
	8, //0X60B PDU:71
	8, //0X635 PDU:72
};
*/

const uint8_t CanMsgSendToIPC[]= {

	1, //IPDU_HND_LWSOUT_M2 (0U)
	1, //IPDU_HND_ACU_GeneralStatus_6 (1U)
	1, //IPDU_HND_ShBW_GeneralStatus (2U)
	1, //IPDU_HND_SCU_GeneralStatus (3U)
	1, //IPDU_HND_ECM_TorqueControl_RN2 (4U)
	1, //IPDU_HND_ECM_TorqueControl_RN1 (5U)
	1, //IPDU_HND_ECM_TorqueControl_N1 (6U)
	1, //IPDU_HND_ECM_SystemControl_1f9 (7U)
	1, //IPDU_HND_USM_Fast_Frame (8U)
	1, //IPDU_HND_PEPS_Message_Sts (9U)
	1, //IPDU_HND_Brake_StabilityControl_RN1 (10U)
	1, //IPDU_HND_Front_Wheel_Speed_Frame (11U)
	1, //IPDU_HND_ITS_InfoStatus_N1 (12U)
	1, //IPDU_HND_ITS_InfoStatus_N2 (13U)
	1, //IPDU_HND_SRR_Left (14U)
	1, //IPDU_HND_SRR_Right (15U)
	1, //IPDU_HND_SMART_bcmMSG_PUSH (16U)
	1, //IPDU_HND_IKEY_352 (17U)
	1, //IPDU_HND_Brake_GeneralData (18U)
	1, //IPDU_HND_BCMMSG1 (19U)
	1, //IPDU_HND_BCM_SystemControl_2 (20U)
	1, //IPDU_HND_BCM_SystemControl (21U)
	1, //IPDU_HND_TPMS_GeneralStatus_BCM (22U)
	1, //IPDU_HND_ATCVT_to_MMI_RN1 (23U)
	1, //IPDU_HND_ITS_InfoStatus_N3_7b (24U)
	1, //IPDU_HND_ITS_InfoStatus_N2_1 (25U)
	1, //IPDU_HND_RETREIVE_ASSIST_POWER_48V (26U)
	1, //IPDU_HND_CLIMATE_ControlData_F (27U)
	1, //IPDU_HND_ECM_GeneralStatus (28U)
	1, //IPDU_HND_DCM_GeneralStatus_5 (29U)
	1, //IPDU_HND_PBD_Status (30U)
	1, //IPDU_HND_SONAR_GeneralStatus_8 (31U)
	1, //IPDU_HND_ECM_to_ITM (32U)
	1, //IPDU_HND_EPB_SystemControl (33U)
	1, //IPDU_HND_DVR_Request (34U)
	1, //IPDU_HND_AMP_5b9 (35U)
	1, //IPDU_HND_AMP_Version_5c2 (36U)
	1, //IPDU_HND_S_BOX_ControlData_SetPM (37U)
	1, //IPDU_HND_S_BOX_ControlData_Seat (38U)
	1, //IPDU_HND_S_BOX_ControlData_enjoy (39U)
	1, //IPDU_HND_S_BOX_ControlData_AC (40U)
	1, //IPDU_HND_EPS_GeneralStatus (41U)
	1, //IPDU_HND_LCM_5E6 (42U)
	1, //IPDU_HND_LCM_5E7 (43U)
	1, //IPDU_HND_TPMS_GeneralStatus2 (44U)
	1, //IPDU_HND_APA_HMI_system (45U)
	1, //IPDU_HND_DVR_STS (46U)
	1, //IPDU_HND_BCM_GeneralStatus (47U)
	1, //IPDU_HND_USM_GeneralStatus (48U)
	1, //IPDU_HND_LowWarningSignal628 (49U)
	1, //IPDU_HND_LOW_WNG_STATUS_629 (50U)
	1, //IPDU_HND_BSW_GeneralStatus_3 (51U)
	1, //IPDU_HND_ECCSMSG8_OBD_IS (52U)
	0, //IPDU_HND_CLUSTER_BasicInfo_1 (53U)
	0, //IPDU_HND_METMSG1 (54U)
	0, //IPDU_HND_CLUSTER_BasicInfo_2 (55U)
	0, //IPDU_HND_DA_Information (56U)
	0, //IPDU_HND_CLUSTER_BasicInfo_3 (57U)
	0, //IPDU_HND_SystemTimeInfo (58U)
	0, //IPDU_HND_AMP_5A2 (59U)
	0, //IPDU_HND_FAP_OperationCommand (60U)
	0, //IPDU_HND_DA_ControlData_PM (61U)
	0, //IPDU_HND_DA_ControlData_seat (62U)
	0, //IPDU_HND_CLIMATE_ControlData (63U)
	0, //IPDU_HND_CLUSTER_GeneralStatus (64U)
	0, //IPDU_HND_PBD_LCM_IKEY_VDC (65U)
	0, //IPDU_HND_IKEY_Display_SwitchStatus (66U)
	0, //IPDU_HND_TCU_DA_information (67U)
	0, //IPDU_HND_CLUSTER_WakeUpFrame (68U)
	0, //IPDU_HND_DVROperationCommand (69U)
	0, //IPDU_HND_DA_WakeUpFrame (70U)
};




const uint8_t CanMsgLength[]= {

	8, //0X2 PDU:0
	8, //0X2A PDU:1
	8, //0X11A PDU:2
	8, //0X11B PDU:3
	8, //0X160 PDU:4
	8, //0X180 PDU:5
	8, //0X182 PDU:6
	8, //0X1F9 PDU:7
	8, //0X215 PDU:8
	8, //0X218 PDU:9
	8, //0X245 PDU:10
	8, //0X284 PDU:11
	8, //0X2B1 PDU:12
	8, //0X2B7 PDU:13
	8, //0X2C8 PDU:14
	8, //0X2C9 PDU:15
	8, //0X351 PDU:16
	8, //0X352 PDU:17
	8, //0X354 PDU:18
	8, //0X358 PDU:19
	8, //0X35A PDU:20
	8, //0X35D PDU:21
	8, //0X385 PDU:22
	8, //0X421 PDU:23
	8, //0X4CB PDU:24
	8, //0X4CC PDU:25
	8, //0X502 PDU:26
	8, //0X54C PDU:27
	8, //0X551 PDU:28
	8, //0X563 PDU:29
	8, //0X572 PDU:30
	8, //0X57A PDU:31
	8, //0X580 PDU:32
	8, //0X58A PDU:33
	8, //0X5B1 PDU:34
	8, //0X5B9 PDU:35
	8, //0X5C2 PDU:36
	8, //0X5C8 PDU:37
	8, //0X5C9 PDU:38
	8, //0X5CA PDU:39
	8, //0X5CB PDU:40
	8, //0X5E4 PDU:41
	8, //0X5E6 PDU:42
	8, //0X5E7 PDU:43
	8, //0X5E8 PDU:44
	8, //0X5E9 PDU:45
	8, //0X60A PDU:46
	8, //0X60D PDU:47
	8, //0X625 PDU:48
	8, //0X628 PDU:49
	8, //0X629 PDU:50
	8, //0X665 PDU:51
	8, //0X6E2 PDU:52
	8, //0X280 PDU:53
	8, //0X2DE PDU:54
	8, //0X355 PDU:55
	8, //0X447 PDU:56
	8, //0X4F8 PDU:57
	8, //0X567 PDU:58
	8, //0X5A2 PDU:59
	8, //0X5A5 PDU:60
	8, //0X5A8 PDU:61
	8, //0X5B2 PDU:62
	8, //0X5B7 PDU:63
	8, //0X5C5 PDU:64
	4, //0X5E2 PDU:65
	8, //0X5E5 PDU:66
	8, //0X5FE PDU:67
	1, //0X602 PDU:68
	8, //0X60B PDU:69
	1, //0X635 PDU:70
};






#endif

extern tCom_SimRxFlag gCom_SimRxFlag[];
#define IPDU_TRANS_CNT_THRESHOLD (50)	//500ms
static uint8 s_IpduCnt[COM_RX_IPDU_MAX_NUM] = {0};
uint8 * const Com_IpduBufPtr[];
void CanApp_RxPduChanged(void)
{
    uint16 l_ipduId = 0XFFFFU;
	uint8 l_CanMsgBuf[SPI_MAX_FRAME_SIZE] = {FUN_ID_MULTI_CAN_MSG};
	uint16 l_CanMsgLen = 1;	//FUN_ID_MULTI_CAN_MSG hold 1 byte
	uint8 l_CanMsgCnt = 0;
	static uint8_t l_ReTCnt = 0;

	if(l_ReTCnt < 10)
	{
		l_ReTCnt++;
	}
	else
	{
		l_ReTCnt = 0;
		Can_MsgChangeProcess(IPDU_HND_ATCVT_to_MMI_RN1,FALSE);
		Can_MsgChangeProcess(IPDU_HND_LWSOUT_M2,FALSE);
    	Can_MsgChangeProcess(IPDU_HND_BCM_GeneralStatus,FALSE);
	}
    while (CQUEUE_ERR_PUSHPOP != CQueue_Pop(Com_IpduBufChangeQueueCh, &l_ipduId, 1, STD_OFF)) {
		#if 0
        SimFlag_SetFlagState(Com_RxBufChange, l_ipduId, FALSE);
		#else
		//Com_SetRxBufChange(l_ipduId,FALSE);
		gCom_SimRxFlag[l_ipduId].rxBufChange = FALSE;
		#endif
        if (CAN_RXPDU_TOTAL_NUM > l_ipduId) {
            if (CanApp_PduProc_Matrix[l_ipduId] != NULL) {
                CanApp_PduProc_Matrix[l_ipduId]();
                if (is_da_pduid) {
                	is_da_pduid = 0;
                	Can_MsgChangeProcess(l_ipduId,TRUE);
            	}
            }

			if(DEA_GetCanMsgtoIpcInitFlag() != 0)
			{
				if (CanMsgSendToIPC[l_ipduId]){
					s_IpduCnt[l_ipduId] = 0;
                	l_CanMsgBuf[l_CanMsgLen] = l_ipduId;
	                l_CanMsgLen += 1;
	                memcpy(l_CanMsgBuf+l_CanMsgLen, Com_IpduBufPtr[l_ipduId], CanMsgLength[l_ipduId]);
					l_CanMsgLen += CanMsgLength[l_ipduId];
					l_CanMsgCnt++;					
					if((l_CanMsgCnt >= 11) || (l_CanMsgLen > (SPI_MAX_DATA_SIZE-9)))	//(can message max len)+(id len)=8+1
					{
						PutMultCanMsgToIPC(l_CanMsgBuf, l_CanMsgLen);
						l_CanMsgLen = 1; //FUN_ID_MULTI_CAN_MSG hold 1 byte
						l_CanMsgCnt = 0;
					}

            	}
			}

			
			if (CanMsgSendToIPC[l_ipduId])
			{
				IPC_SetUpdatedStat(l_ipduId, 1);
			}
        }
    }

	if((DEA_GetCanMsgtoIpcInitFlag() == 0) && (IPC_GetSocReadyOkFlag()))
	{
		for(l_ipduId = 0; l_ipduId < CAN_RXPDU_TOTAL_NUM; l_ipduId++)
		{
			if ((CanMsgSendToIPC[l_ipduId]) && (IPC_GetUpdatedStat(l_ipduId))){
				s_IpduCnt[l_ipduId] = 0;
                l_CanMsgBuf[l_CanMsgLen] = l_ipduId;
                l_CanMsgLen += 1;
                memcpy(l_CanMsgBuf+l_CanMsgLen, Com_IpduBufPtr[l_ipduId], CanMsgLength[l_ipduId]);
				l_CanMsgLen += CanMsgLength[l_ipduId];
				l_CanMsgCnt++;
				if((l_CanMsgCnt >= 11) || (l_CanMsgLen > (SPI_MAX_DATA_SIZE-9))) //(can message max len)+(id len)=8+1
				{
					PutMultCanMsgToIPC(l_CanMsgBuf, l_CanMsgLen);
					l_CanMsgLen = 1; //FUN_ID_MULTI_CAN_MSG hold 1 byte
					l_CanMsgCnt = 0;
				}

            }
		}
		DEA_SetCanMsgtoIpcInitFlag(1);
	}
	
	if(IPC_GetSocReadyOkFlag())
	{
		for(l_ipduId = 0; l_ipduId < COM_RX_IPDU_MAX_NUM; l_ipduId++)
		{
			if ((s_IpduCnt[l_ipduId] >= IPDU_TRANS_CNT_THRESHOLD) && (IPC_GetUpdatedStat(l_ipduId))){
				s_IpduCnt[l_ipduId] = 0;
				if(l_CanMsgLen < (SPI_MAX_DATA_SIZE-1))
				{
					l_CanMsgBuf[l_CanMsgLen] = l_ipduId;
	                l_CanMsgLen += 1;
					if(l_CanMsgLen < (SPI_MAX_FRAME_SIZE-CanMsgLength[l_ipduId]))
					{
		                memcpy(l_CanMsgBuf+l_CanMsgLen, Com_IpduBufPtr[l_ipduId], CanMsgLength[l_ipduId]);
						l_CanMsgLen += CanMsgLength[l_ipduId];
						l_CanMsgCnt++;
						if((l_CanMsgCnt >= 11) || (l_CanMsgLen > (SPI_MAX_DATA_SIZE-9))) //(can message max len)+(id len)=8+1
						{
							PutMultCanMsgToIPC(l_CanMsgBuf, l_CanMsgLen);
							l_CanMsgLen = 1; //FUN_ID_MULTI_CAN_MSG hold 1 byte
							l_CanMsgCnt = 0;
						}
					}
				}
            }
		}

		for(l_ipduId = 0; l_ipduId < COM_RX_IPDU_MAX_NUM; l_ipduId++)
		{
			if(s_IpduCnt[l_ipduId] < IPDU_TRANS_CNT_THRESHOLD)
			{
				s_IpduCnt[l_ipduId]++;
			}
		}
	}
	
	if(l_CanMsgCnt > 0)
	{
		if(l_CanMsgCnt > 1)
		{
			PutMultCanMsgToIPC(l_CanMsgBuf, l_CanMsgLen);
		}
		else
		{
			PutMultCanMsgToIPC(l_CanMsgBuf+1, l_CanMsgLen-1);
		}
	}
}
/*============================= END OF FILE: CanApp_PduChanged.c ==========================*/
