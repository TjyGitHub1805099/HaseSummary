/*============================[R E V I S I O N   H I S T O R Y]=================================*/
/************************************************************************************************/
/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * The integer value 1 is being added or subtracted from a pointer.
 */
 
/* MISRA-C:2004 Rule 19.1,
 * #include statements in a file should only be preceded by other preprocessor directives or comments.
 */
  
/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

#define PM_C_AR_MAJOR_VERSION   5U
#define PM_C_AR_MINOR_VERSION   2U
#define PM_C_AR_PATCH_VERSION   0U

#define PM_C_SW_MAJOR_VERSION   2U
#define PM_C_SW_MINOR_VERSION   5U
#define PM_C_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "pmStateMachine.h"
#include "pmStateMachine_If.h"

#include "trace.h"
#include "Delay.h"

#include "SysWakeUp.h"
#include "PowerSupply.h"
#include "pm_Cfg.h"
#include "BattVoltage.h"
#include "TempBoard.h"
#include "TempAmp.h"
#include "Alarm.h"

#include "Task_PowerMode_If.h"
#include "BoardDefines.h"
#include "System_Signal.h"
#include "PowerSupply.h"
#include "Standby.h"


#include "Task_M_IpcApp_Cfg.h" 

#include "Spi_Ipc_gw_api.h"
#include "Task_PowerModing_Cfg.h"
#include "Eeprom.h"
#include "SleepM.h"
#include "NvM_if.h"
#include "Com_If.h"
#include "task.h"
#include "tdf853x.h"
#include "HardwareVersion.h"

#include "amp_control.h"

#include "pin_wrapper_if.h"

/*============================[V E R S I O N  C H E C K]========================================*/
/************************************************************************************************/

#if (PM_C_AR_MAJOR_VERSION != PM_H_AR_MAJOR_VERSION)
#error "pmStateMachine.c:Mismatch in Specification Major Version"
#endif

#if (PM_C_AR_MINOR_VERSION != PM_H_AR_MINOR_VERSION)
#error "pmStateMachine.c:Mismatch in Specification Minor Version"
#endif

#if (PM_C_AR_PATCH_VERSION != PM_H_AR_PATCH_VERSION)
#error "pmStateMachine.c:Mismatch in Specification Patch Version"
#endif

#if (PM_C_SW_MAJOR_VERSION != PM_H_SW_MAJOR_VERSION)
#error "pmStateMachine.c:Mismatch in Specification Major Version"
#endif

#if (PM_C_SW_MINOR_VERSION != PM_H_SW_MINOR_VERSION)
#error "pmStateMachine.c:Mismatch in Specification Minor Version"
#endif

/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/


/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
uint8 SysErrorCount;
static uint8 FactoryDefaultFlag = FALSE;

boolean socOnFlag = FALSE;
static boolean PM_ignoreAllEvent = FALSE;

boolean gIgn_Off_UpdateFlag = FALSE;
boolean gIgn_Off_UpdateInProgressFlag = FALSE;

PMManagerType PMManager =
{
	PM_MODE_ALL_OFF,//preMode
	PM_MODE_ALL_OFF,//curMode
	PM_MODE_ALL_OFF,//nextMode
	PM_ANIMATE_INIT,//animateState
	STD_ON,//daLcdUsrCtr
	STD_ON,//icLcdUsrCtr
	STD_ON,//ampMute
	STD_OFF,//diagActive
	{PM_VOLT_STATE_NORMAL, PM_MBTEMP_STATE_NORMAL, PM_AMPTEMP_STATE_NORMAL},
	//0xFF,
	0xFF	
};
	
PMSignalStatusType PMSignalStatus =
{
	.Acc_status=STD_OFF,
	.Tcu_Acc_status=STD_OFF,
	.Ign_status=STD_OFF,
	.RmtStart_Flag=STD_OFF,
	.RmtStart_TimeOut=STD_OFF,
	.Reverse_status=STD_OFF,
	.EngineStatus=PM_EngineStatus_stopped,
	.DoorSwitchesState=PM_DoorSwitchesState_none,
	.CanSleepStatus=STD_OFF,
	.TouchScreenStatus=STD_OFF,
	.DAwakeupRequest=STD_OFF,
	.EngOff2TempOff_OnceFlag = FALSE,
	.ISS_ACT=STD_OFF,
	.AVM_Outdoor_Photo_Request=STD_OFF,
	.AVM_Outdoor_Photo_Result=0,//0: idle 1:success 2:fail
	.AVM_Outdoor_Photo_Request_StartTimer=0,
};

/*============================[E X T E R N A L  V A R I A B L E]======================================*/
/************************************************************************************************/
extern boolean gSystemInitDoneFlag;
extern boolean gSystemPowerUp;
extern boolean RVM_CammeraOnOffFlag;


/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/

/*============================================[A M P]===========================================*/
void Port_AMP_MUTE(boolean Flag)
{
	if(Flag == 0)
	{
		Port_SetOutLevel(AMP_D_MUTEN, PORT_LOW);
	}
	else
	{
		Port_SetOutLevel(AMP_D_MUTEN, PORT_HIGH);
	}
}

void Port_AMP_STB(boolean Flag)
{
	if(Flag == 0)
	{
		Port_SetOutLevel(AMP_D_EN, PORT_LOW);
	}
	else
	{
		Port_SetOutLevel(AMP_D_EN, PORT_HIGH);
	}
}

void PM_EX_AMP_OnOff(boolean OnOff)
{
	static uint8 l_AmpState = 0xff;
	
	if (OnOff != l_AmpState)
	{
		l_AmpState = OnOff;
		if(STD_OFF == OnOff)
		{
			if(N1210A == PMManager.variantCtrl)
			{
				Port_SetOutLevel(EX_AMP_EN,PORT_LOW);
				PMManager.ampMute = STD_OFF;
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:extern AMP off\r\n");
			}
		}
		else
		{
			if(N1210A == PMManager.variantCtrl)
			{
				Port_SetOutLevel(EX_AMP_EN,PORT_HIGH);
				PMManager.ampMute = STD_ON;
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:extern AMP on\r\n");
			}
		}
	}
}


void PM_AMP_OnOff(boolean OnOff)
{
	static uint8 l_AmpState = 0xff;

	return;
	
	if (OnOff != l_AmpState)
	{
		l_AmpState = OnOff;
		if(STD_OFF == OnOff)
		{
			if(N1210A == PMManager.variantCtrl)
			{
				Port_SetOutLevel(EX_AMP_EN,PORT_LOW);
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:extern AMP off\r\n");
			}
			else 
			{
				Port_AMP_MUTE(PM_AMP_MUTE);
				Port_AMP_STB(PM_AMP_STANDBY);
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:inner AMP off\r\n");
			}
			PMManager.ampMute = STD_OFF;
		}
		else
		{
			if(N1210A == PMManager.variantCtrl)
			{
				Port_SetOutLevel(EX_AMP_EN,PORT_HIGH);
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:extern AMP on\r\n");
			}
			else
			{
				Port_AMP_MUTE(PM_AMP_UNMUTE);
				Port_AMP_STB(PM_AMP_WORKING);
				DelayMs(5);
				//api_amp_on();
				TracePrintf(PM_TRACE,TRACE_WARNING, "[PM-I]:inner AMP on\r\n");
			}
			PMManager.ampMute = STD_ON;
		}
	}

}

/*============================================[T F T]===========================================*/
void DA_TftBackLightOnOff(boolean OnOff)
{
	static uint8 l_DalcdState = 0xff;
	
	if (OnOff != l_DalcdState)
	{
		l_DalcdState = OnOff;
		if(STD_ON == OnOff)
		{
			Port_SetOutLevel(TFT1_EN, PORT_HIGH);
			PMManager.daLcdUsrCtr = STD_ON;
			TracePrintf(PM_TRACE,TRACE_WARNING, "DA_TftBackLightOn\r\n");
		}
		else
		{
			Port_SetOutLevel(TFT1_EN, PORT_LOW);
			PMManager.daLcdUsrCtr = STD_OFF;
			TracePrintf(PM_TRACE,TRACE_WARNING, "DA_TftBackLightOff\r\n");
		}
	}
}

void IC_TftBackLightOnOff(boolean OnOff)
{
	static uint8 l_IclcdState = 0xff;
	
	if (OnOff != l_IclcdState)
	{
		l_IclcdState = OnOff;
		if(STD_ON == OnOff)
		{
			Port_SetOutLevel(TFT0_EN, PORT_HIGH);
			PMManager.icLcdUsrCtr = STD_ON;
			TracePrintf(PM_TRACE,TRACE_WARNING, "IC_TftBackLightOn\r\n");
		}
		else
		{
			Port_SetOutLevel(TFT0_EN, PORT_LOW);
			PMManager.icLcdUsrCtr = STD_OFF;
			TracePrintf(PM_TRACE,TRACE_WARNING, "IC_TftBackLightOff\r\n");
		}
	}
}

/*============================================[N O T - U S E D]===========================================*/
void setUsbPower(uint8 state)
{
	if (STD_ON == state)	
	{		
		Port_SetOutLevel(CHARGE_EN,PORT_HIGH);		
		Port_SetOutLevel(CHARGE_CTL3,PORT_HIGH);			
		//Port_SetOutLevel(USB_CHARGER_EN_1,PORT_HIGH);		
		//Port_SetOutLevel(USB_CHARGER_EN_2,PORT_HIGH);	
	}	
	else
	{		
		Port_SetOutLevel(CHARGE_EN,PORT_LOW);		
		Port_SetOutLevel(CHARGE_CTL3,PORT_LOW);			
		//Port_SetOutLevel(USB_CHARGER_EN_1,PORT_LOW);		
		//Port_SetOutLevel(USB_CHARGER_EN_2,PORT_LOW);	
	}
}
void setFactoryDefault(uint8 mode)
{
	FactoryDefaultFlag = mode;
}

#if 1
void SysTimeOverRestart(void)
{
	DelayMs(100);
	SysErrorCount++;

	SysResetWrapper(SYS_TIMEOVER);
}
#endif
void PM_Mode_SocOverTimeRestart(void)
{
	if(SysErrorCount < 3)
	{
#ifdef POWERMODING_DEBUG
		TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-E]>SocPwrOverTime RestartSys.\n\r");
#endif
		SysTimeOverRestart();
	}
}

boolean PM_Internal_IsEnvOk(void)
{
	boolean l_returnValue = E_NOT_OK;

	if ((PM_VOLT_STATE_NORMAL == PMManager.safeState.volt)
	        && (PM_MBTEMP_STATE_NORMAL == PMManager.safeState.mbTemp))
	{
		l_returnValue = E_OK;
	}
	return (l_returnValue);
}

/*============================================[D I A G N O S E   U S E D]===========================================*/
void PM_Mode_Restart(void)
{
#ifdef POWERMODING_DEBUG
	TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]>Close AMP & LCD\n\r");
#endif

	SysResetWrapper(PM_RESET);
}
void PM_Mode_Update(void)
{
	static uint32 BOOT_FLAG = USB_UPDATE_ENABLE_FLAG;
#ifdef POWERMODING_DEBUG
	TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]>Close AMP& LCD\n\r");
#endif

	Eep_Write(0,(uint8*)&BOOT_FLAG,sizeof(BOOT_FLAG));
	DelayMs(10);
	
	SysResetWrapper(PM_UPDATE);
}

/*============================================[S O C   U S E D]===========================================*/
void PM_SOC_ModeChange(uint8 mode)
{
    uint8 IC_ret = IPC_GW_ERR_OK;
	uint8 DA_ret = IPC_GW_ERR_OK;
	uint8 retry = 5;
    uint8 CurMode = mode;
	switch( CurMode )
	{
		case PM_MODE_ALL_OFF:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:ALL_OFF(%d)\n\r",CurMode);break;
		case PM_MODE_TEMPO_OFF:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:TEMPO_OFF(%d)\n\r",CurMode);break;
		case PM_MODE_ENG_OFF:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:ENG_OFF(%d)\n\r",CurMode);break;
		case PM_MODE_ENG_ON:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:ENG_ON(%d)\n\r",CurMode);break;
		case PM_MODE_CRANKING:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:CRANKING(%d)\n\r",CurMode);break;
		case PM_MODE_RMT_CHK:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:RMT_CHK(%d)\n\r",CurMode);break;
		case PM_MODE_RMT_START:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:Send mode to soc:RMT_START(%d)\n\r",CurMode);break;
		default:TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]>Send mode to soc: err mode(%d)\n\r",CurMode);
	}
	//TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]>Send mode to soc:%d\n\r",CurMode);
   // if (isIPCLinkUp())
	//{
		do 
		{
			//ipc_gw_send_request_no_reply(IPC_CHANNEL_POWERMODING, IPC_TX_SYS_GETMODE,&CurMode,1);
			
			//IC_ret = Ic_Ipc_send_notification(IC_IPC_HW_CH,IPC_CH_DA_PWR,CURENT_MODE,IPC_FEATURE_IOC_NTF,&CurMode,IC_IPC_NTFY_LEN);
			//DA_ret = ipc_send_notification(DA_IPC_HW_CH,IPC_CH_DA_PWR,CURENT_MODE,&CurMode,DA_IPC_NTFY_LEN);
			if(socOnFlag == TRUE)
			{
				DA_ret = ipc_send_notification(DA_IPC_HW_CH,IPC_CH_DA_PWR,CURENT_MODE,&CurMode,DA_IPC_NTFY_LEN);
			}	
			if((IPC_GW_ERR_OK != IC_ret)||(IPC_GW_ERR_OK != DA_ret))
			{
				if(IPC_GW_ERR_OK != IC_ret)
		        	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-E]>Send to IC ipc err\n\r"); 
				if(IPC_GW_ERR_OK != DA_ret)
		        	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-E]>Send to DA ipc err\n\r"); 
			}
			else
			{
				break;
			}
		} while(((IPC_GW_ERR_OK != IC_ret)||(IPC_GW_ERR_OK != DA_ret)) && (retry-- > 0)) ;
	//}
}

void PM_ModeChangeCallback(uint8 oldMode, uint8 newMode)
{
	//uint8 l_oldMode = oldMode;
	PM_SOC_ModeChange(newMode);
}

/*====================================[S E T  G L O B A L  V A R I A B L E]====================================*/
//get engine status in can frame(0x1f9)
void PM_CheckEngineStatusSignal(void)
{
	Com_RxSig_GetEngineStatus_1f9(&PMSignalStatus.EngineStatus);
}
//get ISS_ACT status in can frame(0x160)
void PM_CheckISS_ACTSignal(void)
{
	Com_RxSig_GetISACT(&PMSignalStatus.ISS_ACT);
}

//get remote start status in can frame(0x351)
void PM_CheckRmtStartSignal(void)
{
	uint8 CurRmtStartFlag = 0xFF;
	Com_RxSig_Getremote_ENG_ST_STATE(&CurRmtStartFlag);
	if(1 == CurRmtStartFlag)
		PMSignalStatus.RmtStart_Flag = STD_ON;
	else
		PMSignalStatus.RmtStart_Flag = STD_OFF;	
}

//get door status in can frame(0x60d)
void PM_CheckDoorSwitchesStateSignal(void)
{
	Com_RxSig_GetDOOR_SW(&PMSignalStatus.DoorSwitchesState);
}

//set touch screen even status
void PM_SetTouchScreenStateSignal(void)
{
	if(PM_MODE_TEMPO_OFF == PMManager.curMode)
		PMSignalStatus.TouchScreenStatus = STD_ON;
}

//get TCU  DA_wakeup_Request ()status in can frame(0x563)
void PM_CheckDAwakeupRequestSignal(void)
{
	uint8 DAwakeupRequest = 0xFF;
	Com_RxSig_GetDA_wakeup_Request(&DAwakeupRequest);
	if(1 == DAwakeupRequest)
		PMSignalStatus.DAwakeupRequest = STD_ON;
	else
		PMSignalStatus.DAwakeupRequest = STD_OFF;	
}

//get TCU  AVM_Outdoor_Photo_Request ()status in can frame(0x563)
void PM_CheckAVM_Outdoor_Photo_RequestSignal(void)
{
	uint8 AVM_Outdoor_Photo_Request = 0xFF;
	Com_RxSig_GetAVM_OutdoorPhoto_Request(&AVM_Outdoor_Photo_Request);
	if(1 == AVM_Outdoor_Photo_Request)//TCU only send 3 frame at sys wake up
		PMSignalStatus.AVM_Outdoor_Photo_Request = STD_ON;
	//else
	//	PMSignalStatus.AVM_Outdoor_Photo_Request = STD_OFF;	
}
//get TCU  AVM_Outdoor_Photo_Request ()status in can frame(0x563)
uint8 PM_GetAVM_Outdoor_Photo_RequestSignal(void)
{
	return PMSignalStatus.AVM_Outdoor_Photo_Request;	
}

//get AVM photo result in devipc
void PM_CheckAVMPhotoResult(uint8 result)
{
	PMSignalStatus.AVM_Outdoor_Photo_Result = result;
}

/*================================[R E M O T E  C H E C K  F U N C T I O N]================================*/
void APP_RemoteCheckDealFuntion(void)
{
	static uint8 avm_photo_state = 0 ;
	uint8 l_value = 0 ;
	
	if(PM_MODE_RMT_CHK == PMManager.curMode)//sys in remote check mode ,trigger by can msg
	{
		if(STD_ON == PMSignalStatus.AVM_Outdoor_Photo_Request)//sys enable AVM take photo
		{
			switch(avm_photo_state)
			{
				case 0://answer TCU and open Rearview Mirror
					PMSignalStatus.AVM_Outdoor_Photo_Request_StartTimer = System_GetTickValue();//recode start timer
				
					//answer TCU ,DA have receive AVM photo request(set to 0X5FE)
					l_value = 3;//reference at <532_DA_CAN_MSG_DESIGN_V1.4.xlsx>
					Com_TxSig_TCU_PicturePushResult(1, &l_value);

					//open Rearview Mirror ,Nofold
					l_value = 1;//reference at <532_DA_CAN_MSG_DESIGN_V1.4.xlsx>
					Com_TxSig_DA_RearviewMirrorOnOff(1, &l_value);

					avm_photo_state = 1;
					
					TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:avm_photo_state = %d\n\r",avm_photo_state);
				break;
				case 1://result check
					//max wait 90s or wait result from AVM in devipc
					if((System_GetTickValue() - PMSignalStatus.AVM_Outdoor_Photo_Request_StartTimer) > 90000)//time out
					{
						//result failed
						PMSignalStatus.AVM_Outdoor_Photo_Result = 2;//reference at <532_DA_CAN_MSG_DESIGN_V1.4.xlsx>
					}

					//AMV result vaild:faild(value = 2) or timeout(value = 2) or success(value = 1) 
					if(0 != PMSignalStatus.AVM_Outdoor_Photo_Result)
					{
						avm_photo_state = 2;
						
						TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:avm_photo_state = %d\n\r",avm_photo_state);
					}
				break;
				case 2:
					//answer the AVM photo result
					l_value = PMSignalStatus.AVM_Outdoor_Photo_Result;
					Com_TxSig_TCU_PicturePushResult(1, &l_value);

					//close Rearview Mirror ,flod
					l_value = 2;//reference at <532_DA_CAN_MSG_DESIGN_V1.4.xlsx>
					Com_TxSig_DA_RearviewMirrorOnOff(1, &l_value);

					avm_photo_state = 3;
					
					TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:avm_photo_state = %d\n\r",avm_photo_state);

					PMSignalStatus.AVM_Outdoor_Photo_Request_StartTimer = System_GetTickValue();//recode start timer
				break;
				default:
					if((System_GetTickValue() - PMSignalStatus.AVM_Outdoor_Photo_Request_StartTimer) > 2000)//wait answer can send out
					{
						//clear this flag,will enable PM_Mode_Cycle to notify SM go to sleep and disable CAN TX
						PMSignalStatus.AVM_Outdoor_Photo_Request = STD_OFF;
						PMSignalStatus.AVM_Outdoor_Photo_Result = STD_OFF;
						
						avm_photo_state = 0 ;
						
						TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:clear AVM_Outdoor_Photo_Request\n\r");
					}
				break;
			}
		}
	}
}

/*================================[R E T U R N  G L O B A L  V A R I A B L E]================================*/
uint8 PM_GetCurrentMode(void)
{
	return PMManager.curMode;
}

uint8 PM_GetAccStatus(void)
{
	return PMSignalStatus.Acc_status;
}

uint8 PM_GetIgnStatus(void)
{
	return PMSignalStatus.Ign_status;
}

uint8 PM_GetTcuAccStatus(void)
{
	return PMSignalStatus.Tcu_Acc_status;
}

uint8 PM_GetReverseStatus(void)
{
	return PMSignalStatus.Reverse_status;
}

PMVoltStateType PM_GetVoltStatus(void)
{
	return PMManager.safeState.volt;
}

PMMbTempStateType PM_GetMbTempStatus(void)
{
	return PMManager.safeState.mbTemp;
}

PMAmpTempStateType PM_GetAmpTempStatus(void)
{
	return PMManager.safeState.ampTemp;
}

uint8 DA_PM_GetLcdStatus(void)
{
	return PMManager.daLcdUsrCtr;
}

uint8 IC_PM_GetLcdStatus(void)
{
	return PMManager.icLcdUsrCtr;
}

uint8 PM_GetAmpStatus(void)
{
	return PMManager.ampMute;
}

void PM_setSocStatus(void)
{
	socOnFlag = TRUE;
}

void RVM_CammeraOnOff(uint8 OnOff)
 {
	 if(STD_ON == OnOff)
	 {
		 Port_SetOutLevel(UG_CAMMERA_ON,PORT_HIGH);
		 RVM_CammeraOnOffFlag = STD_ON;
	 }
	 else
	 {
		 Port_SetOutLevel(UG_CAMMERA_ON,PORT_LOW);
		 RVM_CammeraOnOffFlag = STD_OFF;
	 }
 }

void PM_RxTimeoutClear_DAwakeupRequest(void)
{
	PMSignalStatus.DAwakeupRequest = STD_OFF;
}

void PM_RxTimeoutClear_AVM_Outdoor_Photo_Request(void)
{
	PMSignalStatus.AVM_Outdoor_Photo_Request = STD_OFF;
}

/*======================================[M O D E   I N   A L L]======================================*/
//used to notifaction NM local wake or allow sleep
void PM_Mode_Cycle(void)
{
	static uint8 Acc_status  = 0xFF;
	static uint8 Ign_status = 0xFF;
	static uint8 DAwakeupRequest = 0xFF;
	static uint8 AVM_Outdoor_Photo_Request = 0xFF;
	
	if((Acc_status != PMSignalStatus.Acc_status)||(Ign_status != PMSignalStatus.Ign_status)
		||(DAwakeupRequest != PMSignalStatus.DAwakeupRequest) || (AVM_Outdoor_Photo_Request != PMSignalStatus.AVM_Outdoor_Photo_Request) )
	{
		if((STD_OFF == PMSignalStatus.Acc_status)
			&&(STD_OFF == PMSignalStatus.Ign_status)
			&&(STD_OFF == PMSignalStatus.DAwakeupRequest)
			&&(STD_OFF == PMSignalStatus.AVM_Outdoor_Photo_Request))
		{
			 AppSoftWare_SleepConditonSet( 0 , ACCEPT_SLEEP);//notification SM goto APPLICATION and send readyToSlepp in 0x5c5/0x5fe
		}
		else
		{
			AppSoftWare_SleepConditonSet(0, KEEP_WAKEUP);//notification SM goto APPLICATION and send refuseToSlepp in 0x5c5/0x5fe
		}
		
		Acc_status	= PMSignalStatus.Acc_status;
		Ign_status = PMSignalStatus.Ign_status;
		DAwakeupRequest = PMSignalStatus.DAwakeupRequest;
		AVM_Outdoor_Photo_Request = PMSignalStatus.AVM_Outdoor_Photo_Request;
	}
	   
}
//used to batery voltage cycle deal
void PM_Mode_Volt_Nml(void)
{
	if(PM_VOLT_STATE_NORMAL != PMManager.safeState.volt)
	{
		PMManager.safeState.volt = PM_VOLT_STATE_NORMAL;
		if(TRUE == socOnFlag)
		{
			ipc_send_notification(DA_IPC_HW_CH,IPC_CH_DA_PWR,BAT_STATUS,&(PMManager.safeState.volt),DA_IPC_NTFY_LEN);
		}	
	}
}
void PM_Mode_Volt_Wrn(void)
{
	PMManager.safeState.volt = PM_VOLT_STATE_WARNING;
	if(TRUE == socOnFlag)
	{
		ipc_send_notification(DA_IPC_HW_CH,IPC_CH_DA_PWR,BAT_STATUS,&(PMManager.safeState.volt),DA_IPC_NTFY_LEN);
	}
}
void PM_Mode_Volt_Err(void)
{
	PMManager.safeState.volt = PM_VOLT_STATE_ERROR;
	if(TRUE == socOnFlag)
	{
		ipc_send_notification(DA_IPC_HW_CH,IPC_CH_DA_PWR,BAT_STATUS,&(PMManager.safeState.volt),DA_IPC_NTFY_LEN);
	}
}

/*======================================[M O D E   I N   A L L  O F F]======================================*/
void PM_All_Off_Entry(void)
{
#ifdef POWERMODING_DEBUG
	TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_entry,start 5s\n\r");
#endif

	Alarm_Start(ALARM_ID_5S);
}

void PM_All_Off_Main(void) 
{
	static uint8 powerOnStart500ms = FALSE;
	uint32	status = 0;
	//if from other 
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if(ALARM_STATE_RUNNING == status)//if sys change from wake to sleep ,5s must finished ,used for soc shut down.
		return;
	
	//this condition used to solve ,when ACC & IGN all off ,after 500ms ECU entry sleep.
	if(( FALSE == powerOnStart500ms ) && (STD_OFF == PMSignalStatus.CanSleepStatus) )
	{
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_main,start 500ms\n\r");
		#endif
		
		powerOnStart500ms = TRUE;
		Alarm_Start(ALARM_ID_500MS);
		return;
	}
}

void PM_All_Off_Exit(void)
{
	uint32	status = 0;
	
	#ifdef POWERMODING_DEBUG
		TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_exit,stop 5s & 500ms\n\r");
	#endif	
	
	//close 500ms
	Alarm_GetStatus(SYSTEM_SIG_ID_500MS, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_500MS);

	//close 5s
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_5S);
}

#if ( STD_ON == PM_U1000_SOLUTION )
//this function cycle excute 
void PM_ALL_OFF_WakeUpComeCheck()
{
	static uint8 tft1_en = FALSE;
	static uint32 AllOff_Entry_CurTick = 0 ;

	//1.sys power up lager than 1000ms
	//2.entry ALL_OFF mode again , recode sys tick
	//3.if ACC or IGN or CAN come ,delay 500 ms reset sys  
	if( (PM_MODE_ALL_OFF == PM_GetCurrentMode()) && (System_GetTickValue() > 1000) )
	{
		if(AllOff_Entry_CurTick == 0)
			AllOff_Entry_CurTick = System_GetTickValue();
	
		if(( System_GetTickValue() - AllOff_Entry_CurTick ) > 500 )
		{
			if( STD_ON == PMSignalStatus.Acc_status)
			{
				SysResetWrapper_AndStore(ACC_WAKEUP);
			}
			else if( STD_ON == PMSignalStatus.Ign_status)
			{
				SysResetWrapper_AndStore(IGN_WAKEUP);
			}
			else if( STD_OFF == PMSignalStatus.CanSleepStatus)
			{
				SysResetWrapper_AndStore(CAN_WAKEUP);
			}
		}
	}
	else
		AllOff_Entry_CurTick = 0;
}

#endif
//at ALL OFF status,event ACC 0->1 arrived
void PM_All_Off_AccOn(void)
{
	uint32	status = 0;
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if(ALARM_STATE_RUNNING == status)//if sys change from wake to sleep ,5s must finished ,used for soc shut down.
	{
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:ignore acc on ,between 5s\n\r");
		#endif	
		return;
	}

	if(STD_OFF == PMSignalStatus.Ign_status)
	{
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
	}
	else
	{
		if(PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)//IGN = ON and Cranking
			PMManager.nextMode = PM_MODE_CRANKING;
		else
			PMManager.nextMode = PM_MODE_ENG_ON;
	}
}

//at ALL OFF status,event can awake arrived
void PM_All_Off_CanAwake(void)
{
	uint32  status = 0;
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if(ALARM_STATE_RUNNING == status)//if sys change from wake to sleep ,5s must finished ,used for soc shut down.
	{
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:ignore can awake ,between 5s\n\r");
		#endif
		return;
	}
	
	//close 5s
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_5S);

	//close 500ms
	Alarm_GetStatus(SYSTEM_SIG_ID_500MS, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_500MS);

	PMManager.nextMode = PM_MODE_TEMPO_OFF;
}
//at ALL OFF status,event can sleep and 2 seconds timer arrived
void PM_All_Off_5sOver(void)
{
	uint32	status = 0;
	
	#ifdef POWERMODING_DEBUG
		TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_5s over,system sleep\n\r");
	#endif
	
	//close 5s
	Alarm_GetStatus(SYSTEM_SIG_ID_5S, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_5S);

#if 1
	//wait Nvm_MainFunction() in task_general.c to finish.
	//avoid re-enter issue
	Nvm_LockMainMutex();

	gSystemInitDoneFlag = FALSE;
	Ex_Wdg_Refresh();//add by tjy,@2019-12-12
	
	//vTaskEndScheduler();
	DisableIrq();
	vTaskSuspendAll();

	NvM_ResetForWriteAll();
	NvM_WriteAll();
	NvM_MainFunction();
	
	Ex_Wdg_Refresh();//add by tjy,@2019-12-12
	
#endif
	SysEntryStandby();
}
//at ALL OFF status,event 1 seconds arrived
void PM_All_Off_1sOver(void)
{
	uint32	status = 0;

	#ifdef POWERMODING_DEBUG
		TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_1s over,system sleep\n\r");
	#endif

	//close 1s
	Alarm_GetStatus(SYSTEM_SIG_ID_1S, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_1S);

	SysEntryStandby();
}
//at ALL OFF status,event 500ms timer arrived,used to wake up by can wake ,but not receive again
void PM_All_Off_500MsOver(void)
{
	uint32	status = 0;

	#ifdef POWERMODING_DEBUG
		TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:all_off_500ms over,system sleep\n\r");
	#endif

	//close 500ms
	Alarm_GetStatus(SYSTEM_SIG_ID_500MS, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_500MS);


	SysEntryStandby();
}
void PM_Mode_7MsOver(void)
{
	uint16 l_sysVol = 0,l_needSleep = FALSE;
	Alarm_Stop(ALARM_ID_7MS);

	if(SYS_LVI_APPEAR == GetPinValue_LVI_DET())//lvi check ,if vol < 7.8v ,need sleep
	{
		l_needSleep = TRUE;
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:stop 7ms ,LVI system entry standby(normal)\n\r");
		#endif
	}
	else if(SYS_HVI_APPEAR == GetPinValue_LVI_DET1())//lvi check ,if vol < 7.8v or vol > 16.8,need sleep
	{
		l_needSleep = TRUE;
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:stop 7ms ,HVI system entry standby(normal)\n\r");
		#endif
	}
	else 
	{
		l_needSleep = FALSE;
		#ifdef POWERMODING_DEBUG
			TracePrintf(PM_TRACE, TRACE_NONMASK, "[PM-I]:stop 7ms ,sys recover\n\r");
		#endif
	}

	if(TRUE == l_needSleep)//if need sleep ,entry sleep sequnce
	{
		//don't move ,must ahead SysLviWakeUpEnable ,used to after soc power off done ,set wake up enable ,but not reset 
		gSystemsleeping = TRUE;
		SysLviWakeUpEnable();
		SysEntryStandby();
	}
}

/*======================================[M O D E   I N   T E M P  O F F]======================================*/
void PM_Tempo_Off_Entry(void)
{
}

void PM_Tempo_Off_Main(void) 
{
	uint8 l_value = 0;
	
	if((STD_ON == PMSignalStatus.RmtStart_Flag)
		&&(STD_ON == PMSignalStatus.Acc_status)
		&&(STD_ON == PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_RMT_START;
		return;
	}

	if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT)
		&&(STD_ON == PMSignalStatus.Acc_status)
		&&(STD_ON == PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	}

	if(STD_ON == PMSignalStatus.TouchScreenStatus)
	{
		//must check ACC on ,if leave from TempOff to TempOn(EngOff) triggered by TouchScreen
		if(STD_ON == PMSignalStatus.Acc_status)
		{
			PMSignalStatus.TouchScreenStatus = STD_OFF;//set only in temp_off mode
			PMManager.nextMode = PM_MODE_ENG_OFF;
			#ifdef POWERMODING_DEBUG
				TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:receive DA touch screen event\n\r");
			#endif
			return;
		}
	}

	//this condition use to deal :trigger by global variable DAwakeupRequest
	if((STD_ON == PMSignalStatus.DAwakeupRequest)
		&&(STD_OFF == PMSignalStatus.Acc_status))
	{
		PMManager.nextMode = PM_MODE_RMT_CHK;
		return;
	}

	//this condition use to deal :trigger by global variable AVM_Outdoor_Photo_Request
	if((STD_ON == PMSignalStatus.AVM_Outdoor_Photo_Request)
		&&(STD_OFF == PMSignalStatus.Acc_status))
	{
		PMManager.nextMode = PM_MODE_RMT_CHK;
		PMSignalStatus.AVM_Outdoor_Photo_Request_StartTimer = System_GetTickValue();//recode start timer
		return;
	}

	//add by tjy,@2019-12-13 request by liuxin
	if((STD_ON == PMSignalStatus.Acc_status)
		&&(STD_ON == PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_ENG_ON;
		return;
	}
}

void PM_Tempo_Off_Exit(void)
{
}
//at TEMP OFF status,event ACC 0->1
void PM_Tempo_Off_AccOn(void)
{
	uint32 status =0;
	//stop the OTA update count down.
	Alarm_GetStatus(SYSTEM_SIG_ID_60S, &status);
	if(ALARM_STATE_RUNNING == status)
		Alarm_Stop(ALARM_ID_60S);
	if(STD_ON == PMSignalStatus.Ign_status)
	{

		if(STD_ON == PMSignalStatus.RmtStart_Flag)
		{
			PMManager.nextMode = PM_MODE_RMT_START;
			return;
		}

		if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
		{
			PMManager.nextMode = PM_MODE_CRANKING;
			return;
		}
		else
		{
			PMManager.nextMode = PM_MODE_ENG_ON;
			return;
		}

	}
}

//at TEMP OFF status,event ACC 1->0
void PM_Tempo_Off_AccOff(void)
{
	//this single from Sleep Management ,if can bus no msg after 2s
	if((STD_OFF ==PMSignalStatus.Ign_status) 
		&& (STD_ON == PMSignalStatus.CanSleepStatus))
		PMManager.nextMode = PM_MODE_ALL_OFF;
}

//at TEMP OFF status,event IGN 1->0
void PM_Tempo_Off_IgnOff(void)
{
	//this single from Sleep Management ,if can bus no msg after 2s
	if((STD_OFF == PMSignalStatus.Acc_status) 
		&& (STD_ON == PMSignalStatus.CanSleepStatus))
		PMManager.nextMode = PM_MODE_ALL_OFF;
}

//at TEMP OFF status,event IGN 0->1
void PM_Tempo_Off_IgnOn(void)
{
	uint32 status =0;

	if(STD_ON == PMSignalStatus.Acc_status)
	{
		Alarm_GetStatus(SYSTEM_SIG_ID_60S, &status);
		if(ALARM_STATE_RUNNING == status)
			Alarm_Stop(ALARM_ID_60S);

		if(STD_ON == PMSignalStatus.RmtStart_Flag)
		{
			PMManager.nextMode = PM_MODE_RMT_START;
			return;
		}

		if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
		{
			PMManager.nextMode = PM_MODE_CRANKING;
			return;
		}
		else
		{
			PMManager.nextMode = PM_MODE_ENG_ON;
			return;
		}
	}
}

//at TEMP OFF status,event can sleep arrived
void PM_Tempo_Off_CanSleep(void)
{
	if(gIgn_Off_UpdateFlag)
	{
		TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:SOC OTA Update, NO Entry PM_MODE_ALL_OFF\n\r");
		Alarm_Start(ALARM_ID_60S);
	}
	else
	{
		if((STD_OFF == PMSignalStatus.Acc_status)
			&&(STD_OFF == PMSignalStatus.Ign_status))
			PMManager.nextMode = PM_MODE_ALL_OFF;
	}
}

void PM_Mode_Update_start(void)
{

	uint8 uReqData[2];
	uint32 status =0;

	uReqData[0] = 0;
	uReqData[1] = 0;
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_SWDL, 9, uReqData, 2);

	//PMManager.nextMode = PM_MODE_OTA_UPDATE;
	PMManager.nextMode = PM_MODE_OTA_UPDATE;

	gIgn_Off_UpdateFlag = FALSE;

	gIgn_Off_UpdateInProgressFlag = TRUE;
	Com_RxStop();
	Com_TxStop();

	ipc_gw_unregister(IPC_GW_CHANNEL_CANAPP);

	Alarm_GetStatus(SYSTEM_SIG_ID_60S, &status);
	if(ALARM_STATE_RUNNING == status)
		Alarm_Stop(ALARM_ID_60S);

}




/*======================================[M O D E   I N   E N G  O F F]======================================*/
void PM_Eng_Off_Entry(void)
{
}
void PM_Eng_Off_Main(void) 
{	
	//in temp_on(eng_off) mode,when left door opened ,jump tp temp_off mode ,once
	if((FALSE == PMSignalStatus.EngOff2TempOff_OnceFlag) && ((PMSignalStatus.DoorSwitchesState & PM_DoorSwitchesState_left_open) != 0 ))
	{
		PMSignalStatus.EngOff2TempOff_OnceFlag = TRUE;
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
	
	if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT)
		&&(STD_ON == PMSignalStatus.Acc_status)
		&&(STD_ON ==PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	}

	if(STD_OFF == PMSignalStatus.Acc_status)
	{
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
}

void PM_Eng_Off_Exit(void)
{
}
//at ENG OFF(TEMP ON) status,event ACC 0->1
void PM_Eng_Off_AccOn(void)
{
	if(STD_ON == PMSignalStatus.Ign_status)
	{
		if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
			PMManager.nextMode = PM_MODE_CRANKING;
		else
			PMManager.nextMode = PM_MODE_ENG_ON;
	}
}

//at ENG OFF(TEMP ON) status,event ACC 1->0
void PM_Eng_Off_AccOff(void)
{
	if(STD_OFF == PMSignalStatus.Ign_status)
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
}
//at ENG OFF(TEMP ON) status,event IGN 0->1
void PM_Eng_Off_IgnOn(void)
{
	if(STD_ON == PMSignalStatus.Acc_status)
	{		
		if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
			PMManager.nextMode = PM_MODE_CRANKING;
		else
			PMManager.nextMode = PM_MODE_ENG_ON;
	}

}

/*======================================[M O D E   I N   E N G  O N]======================================*/
void PM_Eng_On_Entry(void)
{
	//should clear EngOff2TempOff_OnceFlag,if leave from EngOff to EngOn or Cranking triggered by IGN 0->1
	//used to if back to EngOff and left door open
	PMSignalStatus.EngOff2TempOff_OnceFlag = FALSE;

}
void PM_Eng_On_Main(void) 
{
	//this condition not use single,so should every cycle to check this condition.
	if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT)
		&&(STD_ON == PMSignalStatus.Acc_status )
		&&(STD_ON == PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	}
	if((STD_OFF == PMSignalStatus.Acc_status)
		&&(STD_OFF == PMSignalStatus.Ign_status))
	{
		if( STD_ON == PMSignalStatus.CanSleepStatus )
			PMManager.nextMode = PM_MODE_ALL_OFF;
		else
			PMManager.nextMode = PM_MODE_TEMPO_OFF;		
		return;
	}
}
void PM_Eng_On_Exit(void)
{
}

//at ENG ON status,event ACC 1->0
void PM_Eng_On_AccOff(void)
{
	if(STD_OFF == PMSignalStatus.Ign_status)
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
}

//at ENG ON status,event IGN 1->0
void PM_Eng_On_IgnOff(void)
{
	if(STD_ON == PMSignalStatus.Acc_status)
		PMManager.nextMode = PM_MODE_ENG_OFF;
	else
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
}

/*======================================[M O D E   I N   C R A N K I N G]======================================*/
void PM_Cranking_Entry(void)
{
	//should clear EngOff2TempOff_OnceFlag,if leave from EngOff to EngOn or Cranking triggered by IGN 0->1
	//used to if back to EngOff and left door open
	PMSignalStatus.EngOff2TempOff_OnceFlag = FALSE;
}

void PM_Cranking_Main(void)
{
	//this condition use to exit cranking mode
	if(PM_EngineStatus_cranking != PMSignalStatus.EngineStatus)
	{
		if((STD_ON == PMSignalStatus.Acc_status)
			&&(STD_ON == PMSignalStatus.Ign_status))
		{
			PMManager.nextMode = PM_MODE_ENG_ON;
			return;
		}	
	}
}

void PM_Cranking_Exit(void)
{
	//if leave Cranking Mode froced to clear EngineStatus ,avoid if 0x1f9 not changed EngineStatus
	//but ACC and IGN all off ,sys need sleep
	PMSignalStatus.EngineStatus = PM_EngineStatus_stopped;
}

//avoid if 0x1f9 not changed PMSignalStatus.EngineStatus
//but ACC IGN is all off,sys need sleep
void PM_Cranking_AccOff(void)
{
	if(STD_OFF == PMSignalStatus.Ign_status)
	{		
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
}
//avoid if 0x1f9 not changed PMSignalStatus.EngineStatus
//but ACC IGN is all off,sys need sleep
void PM_Cranking_IgnOff(void)
{
	if(STD_OFF == PMSignalStatus.Acc_status)
	{		
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
}

/*======================================[M O D E   I N   R M T  C H E C K]======================================*/
void PM_Rmt_Chk_Entry(void)
{
}
void PM_Rmt_Chk_Main(void)
{
	uint8 l_value = 0;
	static uint8 state = 0 ;

	if((PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT)
		&&(STD_ON == PMSignalStatus.Acc_status)
		&&(STD_ON == PMSignalStatus.Ign_status))
	{		
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	}

	if(STD_OFF == PMSignalStatus.DAwakeupRequest)
	{
		if(STD_ON == PMSignalStatus.Acc_status)
		{
			PMManager.nextMode = PM_MODE_TEMPO_OFF;
			return;
		}
	}

	if((STD_ON == PMSignalStatus.CanSleepStatus)
		&&(STD_OFF == PMSignalStatus.Acc_status)
		&&(STD_OFF == PMSignalStatus.Ign_status))
	{
		PMManager.nextMode = PM_MODE_ALL_OFF;
		return;
	}
	
	if(STD_ON == PMSignalStatus.AVM_Outdoor_Photo_Request)//sys enable AVM take photo
		APP_RemoteCheckDealFuntion();

}
void PM_Rmt_Chk_Exit(void)
{	
	//if leave Remote Check Mode forced cleat DAwakeupRequest and AVM_Outdoor_Photo_Request
	PMSignalStatus.DAwakeupRequest =STD_OFF;
	PMSignalStatus.AVM_Outdoor_Photo_Request =STD_OFF;
}

//at REMOTE CHECK status,event ACC 0->1
void PM_Remote_Check_AccOn(void)
{
	if(STD_OFF == PMSignalStatus.Ign_status)
	{		
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
	else if((STD_ON == PMSignalStatus.Ign_status)&&(PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
	{
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	}
}

//at REMOTE CHECK status,event IGN 0->1
void PM_Remote_Check_IgnOn(void)
{
	 if((STD_ON == PMSignalStatus.Acc_status)&&(PM_EngineStatus_cranking == PMSignalStatus.EngineStatus)&&(STD_OFF == PMSignalStatus.ISS_ACT))
	 { 
		PMManager.nextMode = PM_MODE_CRANKING;
		return;
	 } 
}

//at REMOTE CHECK status,event SM can sleep
void PM_Remote_Check_CanSleep(void)
{	
	PMManager.nextMode = PM_MODE_ALL_OFF;
}

/*======================================[M O D E   I N   R M T  S T A R T]======================================*/
void PM_Rmt_Start_Entry(void)
{
	uint32	status = 0;
#ifdef POWERMODING_DEBUG
	TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]:do'stop Alarm:1s\n\r");
#endif	
	//close 1s
	Alarm_GetStatus(SYSTEM_SIG_ID_1S, &status);
	if( ALARM_STATE_RUNNING == status )
		Alarm_Stop(ALARM_ID_1S);

}
void PM_Rmt_Start_Main(void) 
{
	//add to avoid unknow triger ,sys jump problem
	if(STD_OFF == PMSignalStatus.RmtStart_Flag)
	{
		if((STD_ON == PMSignalStatus.Acc_status)&&(STD_ON == PMSignalStatus.Ign_status))
		{
			PMManager.nextMode = PM_MODE_ENG_ON;
			return;
		}else if((STD_OFF == PMSignalStatus.Acc_status)&&(STD_OFF == PMSignalStatus.Ign_status))
		{
			PMManager.nextMode = PM_MODE_TEMPO_OFF;
			return;
		}
	}

	if(STD_OFF == PMSignalStatus.Ign_status)
	{
		//if leave Remote Start Mode forced cleat RmtStart_Flag
		PMSignalStatus.RmtStart_Flag = STD_OFF;
		
		PMManager.nextMode = PM_MODE_TEMPO_OFF;//add by tjy @2020-01-04 request by DNTC from email
		return;
	}

	
}
void PM_Rmt_Start_Exit(void)
{
	//if leave Remote Start Mode forced cleat RmtStart_Flag
	PMSignalStatus.RmtStart_Flag = STD_OFF;
}

//avoid if 0x351 not changed PMSignalStatus.RmtStart_Flag
//but ACC IGN is all off,sys need sleep
void PM_Rmt_Start_AccOff(void)
{	
	if(STD_OFF == PMSignalStatus.Ign_status)
	{
		//if leave Remote Start Mode forced cleat RmtStart_Flag
		PMSignalStatus.RmtStart_Flag = STD_OFF;
		
		PMManager.nextMode = PM_MODE_TEMPO_OFF;
		return;
	}
}
//avoid if 0x351 not changed PMSignalStatus.RmtStart_Flag
//but ACC IGN is all off,sys need sleep
void PM_Rmt_Start_IgnOff(void)
{	
	//if leave Remote Start Mode forced cleat RmtStart_Flag
	PMSignalStatus.RmtStart_Flag = STD_OFF;

	PMManager.nextMode = PM_MODE_TEMPO_OFF;//add by tjy @2020-01-04 request by DNTC from email
	return;	
}



void PM_OTA_Update_Main(void)
{
#ifdef POWERMODING_DEBUG
	//TracePrintf(PM_TRACE, TRACE_WARNING, "[PM-I]>enter Rmt_Start Main func\n\r");
#endif
}


uint8 PM_Get_Current_Mode(void)
{

	return PMManager.curMode;

}



/*=================================================[P R I N T F]=================================================*/
//power management:status change printf
uint8 string_strcat(char *des,char *src,uint16 des_max_len)
{
	uint8 ret = E_NOT_OK;
	if( des_max_len > ( strlen(des)+strlen(src) ) )
	{
		strcat(des,src);
		ret = E_OK;
	}
	return ret;
}
void PM_StateChangePrintf(uint32 event,PMManagerType *pPMManager)
{
	static PMModeType curModeBuf = PM_MODE_ALL_OFF;
	char stringP[0x50]="[PM-I]:";
	if(( curModeBuf == pPMManager->curMode) && ( PM_SIG_CYCLE == event ))
		return;

	//event in
	string_strcat(stringP,"rcv sngl < ",sizeof(stringP));
	switch(event)
	{
		case	PM_SIG_ACC_ON:string_strcat(stringP,"acc_on",sizeof(stringP));break;
		case	PM_SIG_ACC_OFF:string_strcat(stringP,"acc_off",sizeof(stringP));break;
		case	PM_SIG_IGN_ON:string_strcat(stringP,"ign_on",sizeof(stringP));break;
		case	PM_SIG_IGN_OFF:string_strcat(stringP,"ign_off",sizeof(stringP));break;
		case	PM_SIG_TCU_ACC_ON:string_strcat(stringP,"tcu_acc_on",sizeof(stringP));break;
		case	PM_SIG_TCU_ACC_OFF:string_strcat(stringP,"tcu_acc_off",sizeof(stringP));break;
		case	PM_SIG_REMOTE_START:string_strcat(stringP,"remote_start",sizeof(stringP));break;
		case	PM_SIG_REMOTE_STOP:string_strcat(stringP,"remote_stop",sizeof(stringP));break;
		case	PM_SIG_CAN_AWAKE:string_strcat(stringP,"can_awake",sizeof(stringP));break;
		case	PM_SIG_CAN_SLEEP:string_strcat(stringP,"can_sleep",sizeof(stringP));break;
		case	PM_SIG_CUSTOMER_HANDOVER:string_strcat(stringP,"customer_handover",sizeof(stringP));break;
		case	PM_SIG_FIVE_SECONDS:string_strcat(stringP,"five_senconds",sizeof(stringP));break;
		case	PM_SIG_VOLT_NORMAL:string_strcat(stringP,"volt_normal",sizeof(stringP));break;
		case	PM_SIG_VOLT_WARNING:string_strcat(stringP,"volt_warning",sizeof(stringP));break;
		default:string_strcat(stringP,"cycle",sizeof(stringP));break;
	}
	string_strcat(stringP," > , ",sizeof(stringP));

	//mode change
	if( curModeBuf != pPMManager->curMode)
	{
		switch(curModeBuf)
		{
			case	PM_MODE_ALL_OFF:string_strcat(stringP,"AllOff",sizeof(stringP));break;
			case	PM_MODE_TEMPO_OFF:string_strcat(stringP,"TempoOff",sizeof(stringP));break;
			case	PM_MODE_ENG_OFF:string_strcat(stringP,"EngOff",sizeof(stringP));break;
			case	PM_MODE_ENG_ON:string_strcat(stringP,"EngOn",sizeof(stringP));break;
			case	PM_MODE_CRANKING:string_strcat(stringP,"Cranking",sizeof(stringP));break;
			case	PM_MODE_RMT_CHK:string_strcat(stringP,"RemoteCheck",sizeof(stringP));break;
			case	PM_MODE_RMT_START:string_strcat(stringP,"RemoteStart",sizeof(stringP));break;
			default:break;
		}
		string_strcat(stringP,"2",sizeof(stringP));
		switch(pPMManager->curMode)
		{
			case	PM_MODE_ALL_OFF:string_strcat(stringP,"AllOff",sizeof(stringP));break;
			case	PM_MODE_TEMPO_OFF:string_strcat(stringP,"TempoOff",sizeof(stringP));break;
			case	PM_MODE_ENG_OFF:string_strcat(stringP,"EngOff",sizeof(stringP));break;
			case	PM_MODE_ENG_ON:string_strcat(stringP,"EngOn",sizeof(stringP));break;
			case	PM_MODE_CRANKING:string_strcat(stringP,"Cranking",sizeof(stringP));break;
			case	PM_MODE_RMT_CHK:string_strcat(stringP,"RemoteCheck",sizeof(stringP));break;
			case	PM_MODE_RMT_START:string_strcat(stringP,"RemoteStart",sizeof(stringP));break;
			default:break;
		}
		curModeBuf = pPMManager->curMode;
	}
	
	string_strcat(stringP,"\r\n",sizeof(stringP));
	TracePrintf(PM_TRACE, TRACE_NONMASK, stringP);
}

/*======================================[M O D E   M A I N  E N T R Y  E X I T]======================================*/
//status change:deal funtion(entry main and exit )
void (*PM_ModeFuncList[][PM_MODE_FUNC_MAX])(void) =
{
	//mode_main			mode_entry			mode_exit
	{PM_All_Off_Main,   PM_All_Off_Entry,   PM_All_Off_Exit},
	{PM_Tempo_Off_Main, PM_Tempo_Off_Entry, PM_Tempo_Off_Exit},
	{PM_Eng_Off_Main,   PM_Eng_Off_Entry,   PM_Eng_Off_Exit},
	{PM_Eng_On_Main,    PM_Eng_On_Entry,    PM_Eng_On_Exit},
	{PM_Cranking_Main,  PM_Cranking_Entry,  PM_Cranking_Exit},
	{PM_Rmt_Chk_Main,   PM_Rmt_Chk_Entry,   PM_Rmt_Chk_Exit},
	{PM_Rmt_Start_Main, PM_Rmt_Start_Entry, PM_Rmt_Start_Exit},
	{PM_OTA_Update_Main, NULL,				NULL},
};

/*======================================[E V E N T  D E A L  F U N C T I O N]======================================*/
//event occur:deal function
void (*PM_ModeEventFuncList[PM_SIG_MAX][PM_MODE_MAX])(void) =
{
//    					   PM_MODE_ALL_OFF         	PM_MODE_TEMPO_OFF      	PM_MODE_ENG_OFF		PM_MODE_ENG_ON   	PM_MODE_CRANKING    PM_MODE_REMOTE_CHECK      	PM_MODE_REMOTE_START		PM_MODE_OTA_UPDATE
/*0:PM_SIG_ACC_ON*/       {PM_All_Off_AccOn,		PM_Tempo_Off_AccOn,		PM_Eng_Off_AccOn,	NULL,				NULL,				PM_Remote_Check_AccOn,    	NULL,						NULL},
/*1:PM_SIG_ACC_OFF*/      {NULL,					PM_Tempo_Off_AccOff,	PM_Eng_Off_AccOff,	PM_Eng_On_AccOff,	PM_Cranking_AccOff,	NULL,           			PM_Rmt_Start_AccOff,		NULL},
/*2:PM_SIG_IGN_ON*/       {NULL,					PM_Tempo_Off_IgnOn,		PM_Eng_Off_IgnOn,  	NULL,  				NULL,				PM_Remote_Check_IgnOn,    	NULL,						NULL},
/*3:PM_SIG_IGN_OFF*/      {NULL,					PM_Tempo_Off_IgnOff,	NULL,				PM_Eng_On_IgnOff,	PM_Cranking_IgnOff,	NULL,           			PM_Rmt_Start_IgnOff, 		NULL},
/*4:PM_SIG_TCU_ACC_ON*/   {NULL,					NULL,					NULL,				NULL,				NULL,				NULL,           			NULL,						NULL},
/*5:PM_SIG_TCU_ACC_OFF*/  {NULL,					NULL,					NULL,				NULL,				NULL,				NULL,     					NULL,						NULL},
/*6:PM_SIG_REMOTE_START*/ {NULL,					NULL,					NULL,     			NULL,   			NULL,      			NULL,            			NULL,						NULL},
/*7:PM_SIG_REMOTE_STOP*/  {NULL,					NULL,         			NULL,     			NULL,   		 	NULL,      		 	NULL,            		   	NULL,						NULL},
/*8:PM_SIG_CAN_AWAKE*/    {PM_All_Off_CanAwake,		NULL,         			NULL,     			NULL,   			NULL,      			NULL,            			NULL,						NULL},
/*9:PM_SIG_CAN_SLEEP*/    {NULL,					PM_Tempo_Off_CanSleep, 	NULL,     			NULL,   			NULL,      			PM_Remote_Check_CanSleep,	NULL,						NULL},
/*10:PM_SIG_HANDOVER*/    {NULL,					NULL,					NULL,     			NULL,   			NULL,      			NULL,            			NULL,						NULL},
/*11:PM_SIG_CYCLE*/       {PM_Mode_Cycle,		  	PM_Mode_Cycle,		 	PM_Mode_Cycle,		PM_Mode_Cycle,	 	PM_Mode_Cycle,		PM_Mode_Cycle,				PM_Mode_Cycle,				NULL},
/*12:PM_SIG_FIVE_SECONDS*/ {PM_All_Off_5sOver,	  	NULL,		 			NULL,	    		NULL,	 			NULL,      			NULL,            			NULL,						NULL},
/*13:PM_SIG_VOLT_NORMAL*/ {PM_Mode_Volt_Nml,	  	PM_Mode_Volt_Nml,      	PM_Mode_Volt_Nml,  	PM_Mode_Volt_Nml,	PM_Mode_Volt_Nml,   PM_Mode_Volt_Nml,         	PM_Mode_Volt_Nml,			NULL},
/*14:PM_SIG_VOLT_WARNING*/{PM_Mode_Volt_Wrn,	  	PM_Mode_Volt_Wrn,      	PM_Mode_Volt_Wrn,  	PM_Mode_Volt_Wrn,	PM_Mode_Volt_Wrn,   PM_Mode_Volt_Wrn,         	PM_Mode_Volt_Wrn,			NULL},
/*15:PM_SIG_VOLT_ERROR*/  {PM_Mode_Volt_Err,	  	PM_Mode_Volt_Err, 	 	PM_Mode_Volt_Err,	PM_Mode_Volt_Err,	PM_Mode_Volt_Err,	PM_Mode_Volt_Err,		   	PM_Mode_Volt_Err,			NULL},
/*16:PM_SIG_SYS_UPDATE*/  {NULL,         			NULL,         			NULL,     			PM_Mode_Update,  	NULL,      			NULL,            			NULL,						NULL},
/*17:PM_SIG_ONE_SECONDS*/ {PM_All_Off_1sOver,	  	NULL,		 			NULL, 				NULL,	 			NULL, 	 			NULL, 		   				NULL,						NULL},
/*18:PM_SIG_500MS*/       {PM_All_Off_500MsOver,  	NULL,		 			NULL, 				NULL,	 			NULL, 	 			NULL, 	       				NULL,						NULL},
/*19:PM_SIG_7MS*/ 	  	  {PM_Mode_7MsOver,			PM_Mode_7MsOver,		PM_Mode_7MsOver,	PM_Mode_7MsOver,	PM_Mode_7MsOver,	PM_Mode_7MsOver,			PM_Mode_7MsOver,			NULL},
/*20:PM_SIG_60S*/		  {NULL,           			PM_Mode_Update_start,	NULL, 				NULL,   			NULL, 	 			NULL, 	       				NULL,						NULL},
};

/*=================================================[P-M  M A I N]=================================================*/
//power management:main funtion
void PM_Main(uint32 event)
{
	if (FALSE == PM_ignoreAllEvent)
	{	
		//receive event deal
		if (NULL != PM_ModeEventFuncList[event][PMManager.curMode])
		{
			(*PM_ModeEventFuncList[event][PMManager.curMode])();
		}
			
		//state change:execute exit and entry
		if (PMManager.curMode != PMManager.nextMode)
		{
			PM_ModeChangeCallback(PMManager.curMode, PMManager.nextMode);
			if (NULL != PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_EXIT])
			{
				(*PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_EXIT])();
			}
			PMManager.preMode = PMManager.curMode;
			PMManager.curMode = PMManager.nextMode;
			if (NULL != PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_ENTRY])
			{
				(*PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_ENTRY])();
			}
		}

		//main function
		if (NULL !=PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_MAIN])
		{
			(*PM_ModeFuncList[PMManager.curMode][PM_MODE_FUNC_MAIN])();
		}

		//printf info
		PM_StateChangePrintf(event,&PMManager);
	}
}

