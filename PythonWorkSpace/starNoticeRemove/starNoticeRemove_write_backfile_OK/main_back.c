






#include "Cpu.h"
#include "pin_mux.h"
#include "clockManager_cfg.h"
#include "FreeRTOS.h"



#include "flexTimer_pwm5_cfg.h"
#include "uart_debug_pal.h"
#include "uart_acpanel_cfg.h"
#include "dmaController_cfg.h"
#include "flash_cfg.h"
#include "can0_cfg.h"
#include "lpi2c0_cfg.h"
#include "lpi2c1_cfg.h"
#include "wdog_pal_cfg.h"
#include "flexTimer_pwm1_cfg.h"
#include "flexTimer_pwm3_cfg.h"
#include "flexTimer_pwm4_cfg.h"



#include "BoardDefines.h"
#include "adc0_pal_cfg.h"
#include "pwrManager_cfg.h"
#include "uart0_pal_cfg.h"
#include "spi_saf775d.h"
#include "spi_M_ipc_pal_cfg.h"
#include "flexTimer_oc1.h"
#include "uart_gps_cfg.h"
#include "FanBack_Pwm2_Cap.h"
#include "uart_acpanel_cfg.h"
#include "can1_cfg.h"
#include "rtcTimer_cfg.h"
#include "hwCrc_cfg.h"
#include "spi_ipc_pal_cfg.h"
#include "adc1_pal_cfg.h"
#include "Ex_Wdg.h"
#include "Can_Nm.h"
#include "CanIf.h"
#include "Flash_if.h"

  volatile int exit_code = 0;


#include "trace.h"
#include "CanIf.h"
#include "tp_if.h"
#include "Com_If.h"
#include "S32K148.h"

#include "dtcCore_if.h"
#include "Standby.h"
#include "iic_if_cfg.h"
#include "iic_if.h"
#include "ExternRTC.h"
#include "adc_wrapper_if.h"
#include "spiSem.h"
#include "Eeprom.h"
#include "Memif.h"
#include "Ea_if.h"
#include "NvM_if.h"
#include "SysWakeUp.h"
#include "Key.h"
#include "SPI_CircularQueue.h"
#include "Spi_Ipc_gw_api.h"
#include "SleepM.h"

#include "BattVoltage.h"
#include "WaterTempWarn.h"
#include "Delay.h"

#include "VelSpdPulseOut.h"
#include "FanCtrl.h"
#include "dtcApp_if.h"
#include "HardwareVersion.h"

#include "amp_i2c.h"
#include "SWVersion.h"

#include "pmStateMachine.h"

#include "pin_wrapper_if.h"

#include "BinSig_Acc_Cfg.h"
#include "Standby.h"
#include"update_sys.h"
#include "pmStateMachine.h"



extern void Rtos_Init(void);
#define PEX_RTOS_INIT Rtos_Init

extern void Rtos_Start(void);
#define PEX_RTOS_START Rtos_Start

extern void Reset_Handler(void);
extern bool RCM_GetSrcStatusCmd(const RCM_Type * const baseAddr,const rcm_source_names_t srcName);

extern const uint16 EaMarkerAddStart; 
extern const uint16 EA_VERSION;
extern const uint8 gIocSWSubmitterName[];

#define BOOTLOADER_SW_VERSION_ROM_ADDR ((uint8 *)0x410)
#define MarkerAddLen 8




ftm_state_t flexTimer_pwm5_state;

volatile boolean gSystemInitDoneFlag = FALSE;
boolean gSoftWareResetFlag = FALSE;
boolean gPowerOnResetFlag = FALSE;
boolean gNewEepromFlag = FALSE;
volatile boolean gRunInMainFlag = TRUE;

volatile uint8 gCircuitVersion = 0xFF;
volatile uint8 gVariantCtrlVersion = 0xFF;
uint8 gITMasterCtrlVersion = 0;

uint8 gBootloaderSWVersionHex[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] = {0};

volatile uint8 gCurrentDuty = 0;
uint8 gPreDuty = 0;



volatile boolean gSystemPowerUp = FALSE;
volatile boolean RVM_CammeraOnOffFlag = STD_OFF;


#define WAKEUP_PIN_NUMBER 6

pin_settings_config_t Wakeup_pin_mux_InitConfigArr[WAKEUP_PIN_NUMBER] =
{
	
    {
        .base          = PORTE,
        .pinPortIdx    = 11u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTE,
        .direction     = GPIO_INPUT_DIRECTION,
    },
	
	{
        .base          = PORTE,
        .pinPortIdx    = 10u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTE,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
    {
        .base          = PORTE,
        .pinPortIdx    = 13u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTE,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
	{
		.base		   = PORTB,
		.pinPortIdx    = 0u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTB,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
  	
	{
		.base		   = PORTC,
		.pinPortIdx    = 6u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTC,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
    
    {
        .base          = PORTD,
        .pinPortIdx    = 11u,
        .pullConfig    = PORT_INTERNAL_PULL_UP_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_OUTPUT_DIRECTION,
        .initValue     = 1u,
    },
};


#define NUM_OF_CONFIGURED_PINS_SPI 2

pin_settings_config_t g_pin_mux_InitConfigArr_Spi[NUM_OF_CONFIGURED_PINS_SPI] =
{
	{
		.base		   = PORTA,
		.pinPortIdx    = 18u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_ALT4,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = NULL,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 30u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_ALT4,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = NULL,
	}
};

uint8 getBootloaderSWVersion(uint8* versionBuffer,uint8 bufLen)
{
	uint8 i = 0;
	
	if(versionBuffer!=NULL && bufLen>=IOC_SOFTWARE_VERSION_ROM_HEX_LEN)
	{
		for(i=0;i<IOC_SOFTWARE_VERSION_ROM_HEX_LEN;i++)
		{
			versionBuffer[i] = gBootloaderSWVersionHex[i];
		}

		return IOC_SOFTWARE_VERSION_ROM_HEX_LEN;
	}

	return 0;
}
void readBootloaderSWVersion()
{
	uint8 i = 0;
	uint8 *bootSWVersionRomAddr = BOOTLOADER_SW_VERSION_ROM_ADDR;

	
	for(i=0;i<IOC_SOFTWARE_VERSION_ROM_HEX_LEN;i++)
	{
		gBootloaderSWVersionHex[i] = *bootSWVersionRomAddr;
		bootSWVersionRomAddr++;
	}
}

void checkEepromVersion(void)
{
	uint8  l_Buffer[MarkerAddLen]={0u};
	uint16 l_Version = 0;
	Eep_Read(EaMarkerAddStart,l_Buffer,MarkerAddLen);

	l_Version = (uint16)((l_Buffer[2]<<8)|(l_Buffer[3]));
	if(EA_VERSION != l_Version)
	{
		
		gNewEepromFlag = TRUE;
	}
	else
	{
		gNewEepromFlag = FALSE;
		
	}

}

void GpioSetup(void)
{
	uint8 sysNeedGoToSleep = FALSE;

	
#if(STD_ON == SYS_SLEEP_IN_MAIN)
	if(JUMP_TO_APP_AT_ONCE == g_SysSleepInMainCmd)
	{	
		g_SysSleepInMainCmd = 0 ;

		
		PINS_DRV_Init(WAKEUP_PIN_NUMBER, Wakeup_pin_mux_InitConfigArr);
		Ex_Wdg_Disable();
		
		
		if(ACC_OFF == BINSIG_ACC_GET_PORT_VALUE)
			sysNeedGoToSleep = TRUE;
		else if( (SYS_LVI_APPEAR == GetPinValue_LVI_DET()) || (SYS_HVI_APPEAR == GetPinValue_LVI_DET1()) )
			sysNeedGoToSleep = TRUE;

		if(TRUE == sysNeedGoToSleep)
		{
			
			reInitGpioPin();

			
			SysLviWakeUpEnable();
			SysLocationWakeUpEnable();
			SysCanWakeUpEnable();

			
			if(POWER_SYS_GetCurrentMode() != POWER_MANAGER_RUN)
				POWER_SYS_SetMode(2, POWER_MANAGER_POLICY_AGREEMENT);

			
			CLOCK_SYS_UpdateConfiguration(1U, CLOCK_MANAGER_POLICY_AGREEMENT);
			
			if(powerConfigsArr[1]->sleepOnExitValue)
				powerConfigsArr[1]->sleepOnExitValue = FALSE;
			POWER_SYS_SetMode(1, POWER_MANAGER_POLICY_AGREEMENT);
			

			
			INT_SYS_DisableIRQ(PORTB_IRQn);
			INT_SYS_DisableIRQ(PORTC_IRQn);
			
			
			CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
			
			
			POWER_SYS_Init((power_manager_user_config_t * (*)[])powerConfigsArr,POWER_MANAGER_CONFIG_CNT,(power_manager_callback_user_config_t * (*)[])powerStaticCallbacksConfigsArr,1);
			
			if(POWER_SYS_GetCurrentMode() != POWER_MANAGER_RUN)
				POWER_SYS_SetMode(2, POWER_MANAGER_POLICY_AGREEMENT);
			POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);
		
			
			
			
		}
	}
#endif

	
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
	Ex_Wdg_Disable();

	
	gSoftWareResetFlag = RCM_GetSrcStatusCmd(RCM,RCM_SOFTWARE);
	gPowerOnResetFlag = RCM_GetSrcStatusCmd(RCM,RCM_POWER_ON);

	
	if( ( SYS_LVI_APPEAR != GetPinValue_LVI_DET() ) 
		&& ( SYS_HVI_APPEAR != GetPinValue_LVI_DET1() ) )
	{
		
		SysLvi2DetEnable();
		
		
		
		
		Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
		
		
		Port_SetOutLevel(UG5V_ON,PORT_HIGH);
  		
		DelayMs(10);
		Port_SetOutLevel(UG5V_ON,PORT_LOW);
		DelayMs(10);
		Port_SetOutLevel(UG5V_ON,PORT_HIGH);
		
		
		Port_SetOutLevel(CAN_STB1,PORT_HIGH);
		Port_SetOutLevel(CAN_INH1,PORT_HIGH);
		Port_SetOutLevel(CAN_EN1,PORT_HIGH);
		Port_SetOutLevel(CAN_STB2,PORT_HIGH);
		Port_SetOutLevel(CAN_INH2,PORT_HIGH);
		Port_SetOutLevel(CAN_EN2,PORT_HIGH);
		Port_SetOutLevel(CAN_WAKE1,PORT_LOW);
		Port_SetOutLevel(CAN_WAKE2,PORT_LOW);

		
		CanNm_Init();
		CanIf_Init_0();
		CanIf_Init_1();

		
  		Port_SetOutLevel(PMIC_PWR_ON,PORT_HIGH);
  		
  		DelayMs(6);
  		Port_SetOutLevel(PMIC_EN,PORT_LOW);
  		
  		DelayMs(10);
  		Port_SetOutLevel(UG3V3_ON,PORT_HIGH);
		
		
  		
  		SleepM_WakeUpAtPowerOn(0);

		
		
		AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);
		
		Port_SetOutLevel(UDBATT_AD_EN,PORT_HIGH);

  		
  		Port_SetOutLevel(UGPWR_ON,PORT_HIGH);
  		Port_SetOutLevel(IOC_AP_ON_OFF,PORT_HIGH);
  		
  		Port_SetOutLevel(FACEID_ON,PORT_HIGH);
		Port_SetOutLevel(IOC_FUEL_EN, PORT_HIGH);
		DelayMs(12);
		Port_SetOutLevel(DSP_RST_N,PORT_HIGH);
		
  		
  		
  		Port_SetOutLevel(CHARGE_EN,PORT_HIGH);
		Port_SetOutLevel(CHARGE_CTL3,PORT_HIGH);

  		
  		Port_SetOutLevel(SPI_M_IPC_REQ , PORT_HIGH);
  		Port_SetOutLevel(SPI_M_IPC_ACK , PORT_HIGH);

  		
  		
  		Port_SetOutLevel(AMP_D_EN,PORT_HIGH);
  		Port_SetOutLevel(AMP_D_MUTEN,PORT_HIGH);
  		
  		Port_SetOutLevel(EX_AMP_EN,PORT_HIGH);

  		
  		Port_SetOutLevel(GPS_ON,PORT_HIGH);
  		Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_HIGH);
  		Port_SetOutLevel(GPS_RST_Q,PORT_HIGH);

  		
  		Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_HIGH);

		
  		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_HIGH);

		
		Port_SetOutLevel(FAN_EN,PORT_HIGH);
		Port_SetOutLevel(WDI_EN,PORT_HIGH); 
		DelayMs(20);

		
  		
  		gCircuitVersion = getCircuitVerion();
		
		Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);

		
  		
  		
  		
  		
		
		Port_SetOutLevel(CAMMERA1_ON,PORT_HIGH);
		Port_SetOutLevel(CAMMERA2_ON,PORT_HIGH);
		Port_SetOutLevel(CAMMERA3_ON,PORT_HIGH);
		Port_SetOutLevel(CAMMERA4_ON,PORT_HIGH);

  		PINS_DRV_Init(NUM_OF_CONFIGURED_PINS_SPI, g_pin_mux_InitConfigArr_Spi);  
  		
  		
  		
  		gSystemPowerUp = TRUE;
	}
	else
	{
		SysLviWakeUpEnable();
		
		TraceInit();
		if(SYS_LVI_APPEAR == GetPinValue_LVI_DET())
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "[Main]:LVI system entry standby\n\r");
		}
		else if(SYS_HVI_APPEAR == GetPinValue_LVI_DET1())
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "[Main]:HVI system entry standby\n\r");
		}
		
		SysEntryStandby();
	}
	
}

static void prvSetupHardware( void )
{

    
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    POWER_SYS_Init((power_manager_user_config_t * (*)[])powerConfigsArr,POWER_MANAGER_CONFIG_CNT,(power_manager_callback_user_config_t * (*)[])powerStaticCallbacksConfigsArr,1);
    POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);
    
    GpioSetup();
}

void SYSPriorityManagement(void)
{
	uint16 i = 0 ;
	
	for( i = 0 ; i <= FTM7_Ovf_Reload_IRQn ; i++ )
	{
		INT_SYS_SetPriority(i,13);
	}

	INT_SYS_SetPriority(LPSPI0_IRQn,8);
	INT_SYS_SetPriority(LPSPI1_IRQn,8);

	INT_SYS_SetPriority(DMA0_IRQn,8);
	INT_SYS_SetPriority(DMA1_IRQn,8);
	INT_SYS_SetPriority(DMA2_IRQn,8);
	INT_SYS_SetPriority(DMA3_IRQn,8);
	
	INT_SYS_SetPriority(CAN0_ORed_0_15_MB_IRQn,9);
	INT_SYS_SetPriority(CAN0_ORed_16_31_MB_IRQn,9);

	
	INT_SYS_SetPriority(CAN1_ORed_0_15_MB_IRQn,9);
	INT_SYS_SetPriority(CAN1_ORed_16_31_MB_IRQn,9);


	INT_SYS_SetPriority(LPI2C0_Master_IRQn,9);	
	INT_SYS_SetPriority(LPI2C1_Master_IRQn,9);	


	INT_SYS_SetPriority(LPUART2_RxTx_IRQn,9);

    INT_SYS_SetPriority(ADC0_IRQn,10);
	INT_SYS_SetPriority(ADC1_IRQn,10);

	INT_SYS_SetPriority(LPUART1_RxTx_IRQn,11);

	INT_SYS_SetPriority(FTM2_Ch0_Ch1_IRQn,6);

}



int main(void)
{
  
  
  
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   
  #endif
  
  
  
    prvSetupHardware();

    INT_SYS_SetPriority(ADC0_IRQn,10);
    
    FTM_DRV_Init(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_InitConfig, &flexTimer_pwm5_state);
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_PwmConfig);

	readBootloaderSWVersion();
	
    
    FanPwmInit();
	
	

	
	

	
	
	

	EDMA_DRV_Init(&dmaController_State, &dmaController_cfg_InitConfig0,
                       edmaChnStateArray,
                       edmaChnConfigArray,
                       sizeof(edmaChnStateArray)/sizeof(edma_chn_state_t*));


	TraceInit();
	TracePrintf(TRACE_TRACE, TRACE_INFO, "N534-SYSTEM POWER UP(V%s %s)\r\n",gIocSWVersion,gIocSWSubmitterName);

	Com_Init();

	KeyInit();
	SetFanSpeed(gCurrentDuty);

	I2C_Init(INST_LPI2C0_CFG);
	I2C_Init(INST_LPI2C1_CFG);

	amp_i2c_init();

#if 1
	checkEepromVersion();
	Ea_Init();
	NvM_Init();
	Nvm_MainMutexInit();
	NvM_ReadAll();
	NvM_MainFunction();

	if(TRUE == gNewEepromFlag)
	{
		NvM_WriteAll();
		NvM_MainFunction();
		
		NvM_Init();
		NvM_ReadAll();
		NvM_MainFunction();
	}
#endif

	ExtrRtc_Init();

	
	AdcWrapper_Init(ADC_PAL_INSTANCE_1,&adc1_pal_InitConfig);

   	Tp_Init();
	dtcCoreInit();
	dtcAppInit();
#if  0
   	LcdBL_Init();
	
	TempBoard_Init();
	POWER_SYS_Init(powerConfigsArr,POWER_MANAGER_CONFIG_CNT,powerStaticCallbacksConfigsArr,1);
	BattVolt_Init();


	WDG_Init(INST_WDOG_PAL_CFG,&wdog_pal_config);
#else
	
	BattVolt_Init();
	ipc_gw_init_M();
	
#endif
	Task_IpcAppInit();
	SPI_RxTxQueue_Init();
	spiSemCreate(0);
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "CircuitVerAdVal:%d \n\r",gCircuitVersion);
	gVariantCtrlVersion = getVariantCtrlVerion();
	gITMasterCtrlVersion = getITMasterCtrlVerion();
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "ITMasterCtrlVersion:%d \n\r",gITMasterCtrlVersion);

#if UPDATE_BOOTLOADER_FEATURE
  	Flash_If_Init();  
#endif

  
  SYSPriorityManagement();		
  gSystemInitDoneFlag = TRUE;	
  SysLvi2DetEnable();			
  
  Ex_Wdg_Refresh();				

  
  
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  
  #endif
  
  
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  
} 




