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
	
	
		Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
	
	
	
	
	}
	else
	{
		
		
		Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
		Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);
		Port_SetOutLevel(UGPWR_ON,PORT_LOW);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);
	
	
	
	
	
	

	}	
}




void PowerOff(void)
{
#if 1
  
  
  Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);

  

  
  Port_SetOutLevel(WDI_EN,PORT_HIGH); 

  
  Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_LOW);
  Port_SetOutLevel(GPS_ON,PORT_LOW);
  
  
  Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_LOW);

  
  
  Port_SetOutLevel(EX_AMP_EN,PORT_LOW);
  Port_SetOutLevel(AMP_D_MUTEN,PORT_LOW);
  DelayMs(20);
  Port_SetOutLevel(AMP_D_EN,PORT_LOW);
  DelayMs(150);

  Port_SetOutLevel(DSP_RST_N,PORT_LOW);
  DelayMs(10);
  Port_SetOutLevel(UG3V3_ON,PORT_LOW);

  Port_SetOutLevel(CHARGE_EN,PORT_LOW);
  Port_SetOutLevel(CHARGE_CTL3,PORT_LOW);

  
  Port_SetOutLevel(CAMMERA1_ON,PORT_LOW);
  Port_SetOutLevel(CAMMERA2_ON,PORT_LOW);
  Port_SetOutLevel(CAMMERA3_ON,PORT_LOW);
  Port_SetOutLevel(CAMMERA4_ON,PORT_LOW);

  
  Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);

  
  Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);

  Port_SetOutLevel(UGPWR_ON,PORT_LOW);
  
  Port_SetOutLevel(PMIC_EN,PORT_LOW);
  Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);

  Port_SetOutLevel(FAN_EN,PORT_LOW);
  Port_SetOutLevel(UDBATT_AD_EN,PORT_LOW);

  
  
  Port_SetOutLevel(CAN_STB1,PORT_LOW);
  Port_SetOutLevel(CAN_INH1,PORT_LOW);
  Port_SetOutLevel(CAN_EN1,PORT_LOW);
  Port_SetOutLevel(CAN_STB2,PORT_LOW);
  Port_SetOutLevel(CAN_INH2,PORT_LOW);
  Port_SetOutLevel(CAN_EN2,PORT_LOW);
  Port_SetOutLevel(CAN_WAKE1,PORT_HIGH);
  Port_SetOutLevel(CAN_WAKE2,PORT_HIGH);

  Port_SetOutLevel(IOC_FUEL_EN, PORT_LOW);
  Port_SetOutLevel(IOC_AP_ON_OFF,PORT_LOW);

  



  Port_SetOutLevel(UG5V_ON,PORT_LOW);

  DelayMs(100);
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
		case UG8V_ERR_SCAN:
			if(0 == GetPinValue_UG8V_ERR())
			{
				l_ug8v_err_tick = System_GetTickValue();
				l_ug8v_check_status = UG8V_ERR_DEAL_DISABLE_UG8V;
				TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ERR occur!!!!\n\r");
			}
		break;		
			
		case UG8V_ERR_DEAL_DISABLE_UG8V:
			Port_SetOutLevel(UG8V_ON,PORT_LOW);
			
			l_ug8v_err_tick = System_GetTickValue();
			l_ug8v_check_status = UG8V_ERR_DEAL_ENABLE_UG8V;
			
			TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ON disable!!!!\n\r");
		break;

		case UG8V_ERR_DEAL_ENABLE_UG8V:
			if(( System_GetTickValue() - l_ug8v_err_tick ) > 10 )
			{
				Port_SetOutLevel(UG8V_ON,PORT_HIGH);
				
				l_ug8v_err_tick = System_GetTickValue();
				l_ug8v_check_status = UG8V_ERR_WAIT;
				TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:UG8V_ON enable!!!!\n\r");
			}
		break;
			
		case UG8V_ERR_WAIT:
			if(( System_GetTickValue() - l_ug8v_err_tick ) > 30 )
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




void SysResetWrapper(uint32 moduleID)
{
	INT_SYS_DisableIRQGlobal();
	vTaskSuspendAll();
	PowerOff();
	DelayMs(1000);
	SystemSoftwareReset(moduleID,FALSE);
}


void SysResetWrapper_AndStore(uint32 moduleID)
{
	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:if reset ,start store!!\n\r");

	
	
	Nvm_LockMainMutex();
	
	DisableIrq();
	vTaskSuspendAll();
	PowerOff();
	
	gSystemInitDoneFlag = FALSE;
	Ex_Wdg_Refresh();
			
	NvM_ResetForWriteAll();
	NvM_WriteAll();
	NvM_MainFunction();
	
	Ex_Wdg_Refresh();
	TracePrintf(PM_TRACE, TRACE_ERRO, "[PM-I]:if reset ,end store!!\n\r");

	
	DelayMs(800);
	
	INT_SYS_DisableIRQGlobal();
	SystemSoftwareReset(moduleID,FALSE);		
}


