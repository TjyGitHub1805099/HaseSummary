#include "PowerSupply.h"
#include "Delay.h"
#include "pin_wrapper_if.h"
#include "trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "NvM_Cbk.h"
#include "Ex_Wdg.h"
#include "Standby.h"
#include "NvM_if.h"
#include "main.h"




void Soc_Power_Onoff(boolean onoff)
{
	if(onoff == STD_ON)
	{
		Port_SetOutLevel(PMIC_EN,PORT_HIGH);
		Port_SetOutLevel(PMIC_PWR_ON,PORT_HIGH);
		DelayMs(9);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		Port_SetOutLevel(UGPWR_ON,PORT_HIGH);
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_HIGH);
	//	Port_SetOutLevel(TFT0_EN,PORT_HIGH);
	//	Port_SetOutLevel(TFT1_EN,PORT_HIGH);
		Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
	//	Port_SetOutLevel(CAMMERA1_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA2_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA3_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA4_ON,PORT_LOW);//open camera
	}
	else
	{
		//======================================SOC : RESET=======================================
		//when power off soc , must first set reset then power off
		Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
		Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);
		Port_SetOutLevel(UGPWR_ON,PORT_LOW);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);
	//	Port_SetOutLevel(TFT0_EN,PORT_LOW);
	//	Port_SetOutLevel(TFT1_EN,PORT_LOW);
	//	Port_SetOutLevel(CAMMERA1_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA2_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA3_ON,PORT_LOW);//open camera
	//	Port_SetOutLevel(CAMMERA4_ON,PORT_LOW);//open camera

	}	
}




void PowerOff(void)
{
#if 1
  //======================================SOC : BOOT========================================
  //info soc in normal mode
  Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);

  //======================================SOC : POWER OFF start=============================

  //======================================SOC : OTHER=======================================
  Port_SetOutLevel(WDI_EN,PORT_HIGH); //disable WatchDog

  //======================================SOC : GPS=========================================
  Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_LOW);
  Port_SetOutLevel(GPS_ON,PORT_LOW);
  //Port_SetOutLevel(GPS_RST_Q,PORT_LOW);//not deinit it cause GPS_REST_Q request by se peng cheng from email @2019-10-24
  //======================================SOC : TUNER=======================================
  Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_LOW);

  //======================================SOC : AMP=========================================
  //internal AMP
  Port_SetOutLevel(EX_AMP_EN,PORT_LOW);
  Port_SetOutLevel(AMP_D_MUTEN,PORT_LOW);
  DelayMs(20);
  Port_SetOutLevel(AMP_D_EN,PORT_LOW);
  DelayMs(150);

  Port_SetOutLevel(DSP_RST_N,PORT_LOW);//dsp reset disable
  DelayMs(10);
  Port_SetOutLevel(UG3V3_ON,PORT_LOW);

  Port_SetOutLevel(CHARGE_EN,PORT_LOW);
  Port_SetOutLevel(CHARGE_CTL3,PORT_LOW);

  //======================================SOC : CAMMERA=====================================
  Port_SetOutLevel(CAMMERA1_ON,PORT_LOW);//close camera1
  Port_SetOutLevel(CAMMERA2_ON,PORT_LOW);//close camera2
  Port_SetOutLevel(CAMMERA3_ON,PORT_LOW);//close camera3
  Port_SetOutLevel(CAMMERA4_ON,PORT_LOW);//close camera4

  //======================================SOC : BT==========================================
  Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);

  //when power off soc , must first set reset then power off
  Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);

  Port_SetOutLevel(UGPWR_ON,PORT_LOW);
  //======================================SOC : PIMC========================================
  Port_SetOutLevel(PMIC_EN,PORT_LOW);
  Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);

  Port_SetOutLevel(FAN_EN,PORT_LOW);//disenable fan control
  Port_SetOutLevel(UDBATT_AD_EN,PORT_LOW);//batery voltage discontact to s32k adc pin

  //======================================PCI : CAN1 CAN2 ======================================
  //remove here to solve can interface ack from sleep less than 45ms
  Port_SetOutLevel(CAN_STB1,PORT_LOW);
  Port_SetOutLevel(CAN_INH1,PORT_LOW);
  Port_SetOutLevel(CAN_EN1,PORT_LOW);
  Port_SetOutLevel(CAN_STB2,PORT_LOW);
  Port_SetOutLevel(CAN_INH2,PORT_LOW);
  Port_SetOutLevel(CAN_EN2,PORT_LOW);
  Port_SetOutLevel(CAN_WAKE1,PORT_HIGH);
  Port_SetOutLevel(CAN_WAKE2,PORT_HIGH);

  Port_SetOutLevel(IOC_FUEL_EN, PORT_LOW);//disenable fuel
  Port_SetOutLevel(IOC_AP_ON_OFF,PORT_LOW);

  //======================================SOC : TFT=========================================
//  Port_SetOutLevel(TFT0_EN,PORT_LOW);
//  Port_SetOutLevel(TFT1_EN,PORT_LOW);

  Port_SetOutLevel(UG5V_ON,PORT_LOW);

  DelayMs(100);//used for PMIC release power,request by ZHOUGUANGYONG @2020-1-16,
#endif

}


#if ( STD_ON == UG8V_ERR_CHECK_EN )

typedef enum
{
	UG8V_ERR_SCAN = 0,
	UG8V_ERR_DEAL_DISABLE_UG8V,	
	UG8V_ERR_DEAL_ENABLE_UG8V, 
	UG8V_ERR_WAIT,
} UG8V_DEAL_STATUS_Type;
	
UG8V_DEAL_STATUS_Type l_ug8v_check_status = UG8V_ERR_SCAN;
uint32 l_ug8v_err_tick = 0;

void UG8V_Err_Check_Main()
{
	switch(l_ug8v_check_status)
	{
		case UG8V_ERR_SCAN://err check		
			if(0 == GetPinValue_UG8V_ERR())//err occur
			{
				l_ug8v_err_tick = System_GetTickValue();
				l_ug8v_check_status = UG8V_ERR_DEAL_DISABLE_UG8V;
				TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ERR occur!!!!\n\r");
			}
		break;		
			
		case UG8V_ERR_DEAL_DISABLE_UG8V://err deal:disable UG8V
			Port_SetOutLevel(UG8V_ON,PORT_LOW);//disable UG8V
			//
			l_ug8v_err_tick = System_GetTickValue();
			l_ug8v_check_status = UG8V_ERR_DEAL_ENABLE_UG8V;
			
			TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ON disable!!!!\n\r");
		break;

		case UG8V_ERR_DEAL_ENABLE_UG8V://err deal:enable UG8V
			if(( System_GetTickValue() - l_ug8v_err_tick ) > 10 )//delay 10ms : 0->1
			{
				Port_SetOutLevel(UG8V_ON,PORT_HIGH);//enable UG8V	
				//
				l_ug8v_err_tick = System_GetTickValue();
				l_ug8v_check_status = UG8V_ERR_WAIT;
				TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ON enable!!!!\n\r");
			}
		break;
			
		case UG8V_ERR_WAIT://deal wait
			if(( System_GetTickValue() - l_ug8v_err_tick ) > 30 )//delay 30ms : cause UG8V_ON enable UG8V_ERR will general a pulse . 
			{
				l_ug8v_check_status = UG8V_ERR_SCAN;
				TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:ug8v goto scan!!!!\n\r");
			}
		break;
			
		default:
			l_ug8v_check_status = UG8V_ERR_SCAN ;
		break;
	}
}
#endif


//CAUTION:this API can't be used in Interrupt Context
/*
Para:
moduleID real value is defined by moduleIdType in system_S32K148.h 
typedef enum
{	IGN_WAKEUP = 0,    //0
	ACC_WAKEUP,        //1
	TCUACC_WAKEUP,     //2
	CAN_WAKEUP,        //3
	LVI_WAKEUP,        //4
	IPC_UPDATER,       //5
	PM_RESET,          //6
	PM_UPDATE,         //7
	SYS_TIMEOVER,      //8
	NTF_UPDATE,        //9
	GENERAL_TEST,      //10
	MAX_ID
}moduleIdType;

*/
void SysResetWrapper(uint32 moduleID)
{
	INT_SYS_DisableIRQGlobal();
	vTaskSuspendAll();
	PowerOff();
	DelayMs(1000);
	SystemSoftwareReset(moduleID,FALSE);
}

//when sys entry ALL_OFF mode ,and wake up single ocure ,exeute this logic
void SysResetWrapper_AndStore(uint32 moduleID)
{
	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:if reset ,start store!!\n\r");

	//wait Nvm_MainFunction() in task_general.c to finish.
	//avoid re-enter issue
	Nvm_LockMainMutex();
	
	DisableIrq();
	vTaskSuspendAll();
	PowerOff();
	
	gSystemInitDoneFlag = FALSE;
	Ex_Wdg_Refresh();//add by tjy,@2019-12-12
			
	NvM_ResetForWriteAll();
	NvM_WriteAll();
	NvM_MainFunction();
	
	Ex_Wdg_Refresh();//add by tjy,@2019-12-12
	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:if reset ,end store!!\n\r");

	//when PowerOff() must delay 1000ms to ensure SOC power off
	DelayMs(800);
	
	INT_SYS_DisableIRQGlobal();
	SystemSoftwareReset(moduleID,FALSE);		
}


