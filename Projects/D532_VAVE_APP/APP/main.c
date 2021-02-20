/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K14x
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "pin_mux.h"
#include "clockManager_cfg.h"
#include "FreeRTOS.h"

//#include "flexTimer_pwm3_cfg.h"
//#include "flexTimer_pwm4_cfg.h"
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
//#include "flexTimer_pwm1_cfg.h"
//#include "flexTimer_pwm3_cfg.h"
//#include "flexTimer_pwm4_cfg.h"
#include "BoardDefines.h"
#include "adc0_pal_cfg.h"
#include "pwrManager_cfg.h"
#include "uart0_pal_cfg.h"
#include "spi_saf775d.h"
#include "spi_M_ipc_pal_cfg.h"
#include "flexTimer_oc1.h"
#include "uart_gps_cfg.h"
#include "FanBack_Pwm2_Cap.h"
#include "flexTimer_mc1.h"
#include "lpTmr1.h"
#include "uart_acpanel_cfg.h"
#include "can1_cfg.h"
#include "rtcTimer_cfg.h"
#include "hwCrc_cfg.h"
#include "spi_ipc_pal_cfg.h"
#include "adc1_pal_cfg.h"
#include "Ex_Wdg.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */

#include "trace.h"
#include "CanIf.h"
#include "tp_if.h"
#include "Com_If.h"
#include "S32K148.h"
//#include "dtccore_if.h"
#include "dtcCore_if.h"

#include "iic_if_cfg.h"
#include "iic_if.h"

#include "adc_wrapper_if.h"

#include "Eeprom.h"
#include "Memif.h"
#include "Ea_if.h" 
#include "NvM_if.h"

#include "Key.h"
#include "SPI_CircularQueue.h"
#include "Spi_Ipc_gw_api.h"


#include "BattVoltage.h"
#include "WaterTempWarn.h"
#include "Delay.h"
//#include "SpeedOutput.h"
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
#include "update_sys.h"
#include "SleepM.h"
#include "LptmrWakeUp.h"

extern void Rtos_Init(void);
#define PEX_RTOS_INIT Rtos_Init

extern void Rtos_Start(void);
#define PEX_RTOS_START Rtos_Start

extern void Reset_Handler(void);

extern const uint16 EaMarkerAddStart; 
extern const uint16 EA_VERSION;
extern const uint8 gIocSWVersion[];
extern const uint8 gIocSWSubmitterName[];

#define BOOTLOADER_SW_VERSION_ROM_ADDR 0x410
#define MarkerAddLen 8


//ftm_state_t flexTimer_pwm3_state;
//ftm_state_t flexTimer_pwm4_state;
ftm_state_t flexTimer_pwm5_state;

boolean gSystemInitDoneFlag = FALSE;
boolean gSoftWareResetFlag = FALSE;
boolean gPowerOnResetFlag = FALSE;
boolean gNewEepromFlag = FALSE;
boolean gRunInMainFlag = TRUE;

uint8 gCircuitVersion = 0xFF;
uint8 gVariantCtrlVersion = 0xFF;
uint8 gITMasterCtrlVersion = 0;

uint8 gBootloaderSWVersionHex[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] = {0};

uint8 gCurrentDuty = 0;
uint8 gPreDuty = 0;

//used to when in main ,soc not power on 
//if sleep event triger ,in standby sequence should not power off soc
boolean gSystemPowerUp = FALSE;
boolean RVM_CammeraOnOffFlag = STD_OFF;

//only configer re sleep used PINS
#define WAKEUP_PIN_NUMBER 6//ACC, LVI HVI, CAN1_RX CAN2_RX, EX_WDG_EN

pin_settings_config_t Wakeup_pin_mux_InitConfigArr[WAKEUP_PIN_NUMBER] =
{
	//ACC
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
	//HVI
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
    //LVI
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
    //CAN1_RX
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
  	//CAN2_RX
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
    //EX_WDG_EN
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

//OverWrite Set SPI PINS
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

	//cpy from ROM
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
		//return NEWEEPROM;
		gNewEepromFlag = TRUE;
	}
	else
	{
		gNewEepromFlag = FALSE;
		//return NOCHANGE;
	}

}

#if (STD_ON == MAIN_SM_STATE_USE_TIMER)
uint16 g_nmTimerOffset = 0 ;
void ftmTimerISR(void)
{
	INT_SYS_DisableIRQ(FTM0_Ovf_Reload_IRQn);
#if 0
    /* Static variable for storing the timer overflow count */
    static uint8_t s_overflowCount = 0U;
    static uint8 a = 0 ;
	if(0==a)
	{
		Port_SetOutLevel(EE_WP, PORT_HIGH);
		a=1;
	}
	else
	{
		Port_SetOutLevel(EE_WP, PORT_LOW);
		a=0;
	}
#endif
    g_nmTimerOffset += 10;//10ms

	//remove here to solve can interface ack from sleep less than 45ms , s1 < 200ms -C
	SleepM_WakeUpAtPowerOn(0);

    /* Clear FTM Timer Overflow flag */
    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1, (uint32_t)FTM_TIME_OVER_FLOW_FLAG);
    INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);
}
#endif

//
static void PM_SleepManagement(void)
{
	uint8 sysNeedGoToSleep = FALSE;

	//if sleep in main is enable
#if(STD_ON == SYS_SLEEP_IN_MAIN)
	if(JUMP_TO_APP_AT_ONCE == g_SysSleepInMainCmd)//must sleep cause trigered by APP
	{	
		g_SysSleepInMainCmd = 0 ;

		//initial WAKEUP pin
		PINS_DRV_Init(WAKEUP_PIN_NUMBER, Wakeup_pin_mux_InitConfigArr);
		Ex_Wdg_Disable();//add by tjy,@2019-12-12 ,must be disable wdg!!!!!!

		//ahead here , avoid ACC / LVI / HVI single arrived here ,add by tjy @2020-12-29
		LptmrWakeUpInit();//add by tjy,at 2020-12-29,copy from QQROS_C26 Init lptimer wakeup

		//set wake up event
		SysLviWakeUpEnable();
		SysLocationWakeUpEnable();
		SysCanWakeUpEnable();

		//the follwing condition sys need sleep
		if(ACC_OFF == BINSIG_ACC_GET_PORT_VALUE)//1:acc off sys need sleep
		{
			sysNeedGoToSleep = TRUE;
		}
		else if( (SYS_LVI_APPEAR == GetPinValue_LVI_DET()) || (SYS_HVI_APPEAR == GetPinValue_LVI_DET1()) )//2:acc on but LVI or HVI appear need sleep
		{
			sysNeedGoToSleep = TRUE;
		}
		//
		if(TRUE == sysNeedGoToSleep)
		{
			//========================================sleep configer:start==========================================
			//because VLPR mode only can be switched from RUN mode
			if(POWER_SYS_GetCurrentMode() != POWER_MANAGER_RUN)//if current mode isn't RUN mode
				POWER_SYS_SetMode(2, POWER_MANAGER_POLICY_AGREEMENT);//active RUN mode

			//if sleep need re configuration lock management,close not used clock
			CLOCK_SYS_UpdateConfiguration(1U, CLOCK_MANAGER_POLICY_AGREEMENT);
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!don't move !very importance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if(powerConfigsArr[1]->sleepOnExitValue)//avoid sleepOnExitValue not false ,set it false
				powerConfigsArr[1]->sleepOnExitValue = FALSE;
			POWER_SYS_SetMode(1, POWER_MANAGER_POLICY_AGREEMENT);//active VLPR mode
			//========================================sleep configer:end============================================	

			//========================================from sleep wake up:start======================================
			//sys clk configuration to 112MHZ,and open used clock
			CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
			
			//sys power managed to HSRUN
			POWER_SYS_Init(powerConfigsArr,POWER_MANAGER_CONFIG_CNT,powerStaticCallbacksConfigsArr,1);
			//because HSRUN mode only can be switched from RUN mode
			if(POWER_SYS_GetCurrentMode() != POWER_MANAGER_RUN)
				POWER_SYS_SetMode(2, POWER_MANAGER_POLICY_AGREEMENT);//active RUN mode
			POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);//active HSRUN mode
		
			//========================================from sleep wake up:end=======================================	
		}
		else
		{
			DelayMs(1000);//add by tjy,delay 1s to insure SOC Complete discharge
		}

		//==========================from sleep wake up first colse CAN1/CAN2/ACC/HVI/LVI interrupt=================
		WakeUpSourceDiasbleIRQ();  //add by tjy,at 2020-12-28,After wakeup disable wakeup source
	}
#endif

}
void GpioSetup(void)
{
	ftm_state_t ftmStateStruct;

	//initial all pin ,because execute reInitGpioPin upper
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
	Ex_Wdg_Disable();//add by tjy,@2019-12-12 ,must be disable wdg!!!!!!

	//ahead here ,read sys reset type
	gSoftWareResetFlag = RCM_GetSrcStatusCmd(RCM,RCM_SOFTWARE);
	gPowerOnResetFlag = RCM_GetSrcStatusCmd(RCM,RCM_POWER_ON);

	//if baterry in nomal
	if( ( SYS_LVI_APPEAR != GetPinValue_LVI_DET() ) //7.8V < Vbat
		&& ( SYS_HVI_APPEAR != GetPinValue_LVI_DET1() ) )// 7.8V < Vbat < 16.8v
	{
		//enable lvi sleep interrupt,if LVI appear in main before rtos ,reset soon
		SysLvi2DetEnable();
		
		//set used pin
		//======================================SOC : BOOT========================================
		//info soc in normal mode
		Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
		
		//======================================SOC : POWER ON start==============================
		Port_SetOutLevel(PWR_SYNC,PORT_LOW);
		Port_SetOutLevel(UG3V3_ON,PORT_HIGH);

		//======================================PCI : CAN1 CAN2 ======================================
		//remove here to solve can interface ack from sleep less than 45ms , s1 < 200ms -A
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
		
		//remove here to solve can interface ack from sleep less than 45ms , s1 < 200ms -B
		Port_SetOutLevel(UG5V_ON,PORT_HIGH);

		//======================================SOC : PIMC========================================
		Port_SetOutLevel(PMIC_PWR_ON,PORT_HIGH);
		//hardware require : must lager than 6ms between PMIC_PWR_ON and PMIC_EN, cause at 120MHZ clk DelayMs(10)=7.32ms
		DelayMs(10);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);

		//this logic move to 10 ms timer:add by tjy at 2020-06-05
		#if (STD_ON != MAIN_SM_STATE_USE_TIMER)
			//remove here to solve can interface ack from sleep less than 45ms , s1 < 200ms -C
			SleepM_WakeUpAtPowerOn(0);
		#else
			/* Initialize Flex Timer instance as simple timer */
			FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig, &ftmStateStruct);

			/* Install handler for the Timer overflow interrupt and enable it */
			INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn, &ftmTimerISR, (isr_t*) 0);
			INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);

			/* Setup the counter to trigger an interrupt every 10 ms */
			FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig);

			#if 0
				//don't move ,set can err and busoff interruput larger than 5 (not execute in main),add by tjy @2019-8-29
				INT_SYS_SetPriority(CAN0_ORed_IRQn, 8);
				INT_SYS_SetPriority(CAN0_Error_IRQn, 8);
			#endif
			/* Start the counter */
			FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);			
		#endif

		//remove here to solve cause after ADC initial ,must lager than 6ms,adc sample data valid
		AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);

		//hardware require : must lager than 6.6ms between PMIC_EN and UGPWR_ON
		Port_SetOutLevel(UGPWR_ON,PORT_HIGH);
		//hardware require : must lager than 9ms after UGPWR_ON set
		DelayMs(12);
		Port_SetOutLevel(UG6V5_ON,PORT_HIGH);
		Port_SetOutLevel(UG8V_ON,PORT_HIGH);
		
		//======================================SOC : FUCTION=====================================
		//======================================SOC : CAMMERA=====================================
		Port_SetOutLevel(UG_CAMMERA_ON,PORT_LOW);//close camera
		//Port_SetOutLevel(UG_CAMMERA_ON,PORT_HIGH); //测试反馈更改为高
		Port_SetOutLevel(IOC_AP_ON_OFF,PORT_HIGH);
		Port_SetOutLevel(DSP_RST_N,PORT_HIGH);//dsp reset disable
		Port_SetOutLevel(CHARGE_EN,PORT_HIGH);
		Port_SetOutLevel(CHARGE_CTL3,PORT_HIGH);
		
		//======================================SOC : SPI=========================================
		Port_SetOutLevel(SPI_M_IPC_REQ , PORT_HIGH);//spi ipc ,ioc request pin
		Port_SetOutLevel(SPI_M_IPC_ACK , PORT_HIGH);//spi ipc ,ioc ack pin
		
		//======================================SOC : AMP=========================================
		//internal AMP
		Port_SetOutLevel(AMP_D_EN,PORT_HIGH);
		Port_SetOutLevel(AMP_D_MUTEN,PORT_HIGH);
		//external AMP
		Port_SetOutLevel(EX_AMP_EN,PORT_HIGH);
	
		//======================================SOC : GPS=========================================
		Port_SetOutLevel(GPS_ON,PORT_HIGH);
		Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_HIGH);
		Port_SetOutLevel(GPS_RST_Q,PORT_HIGH);
		
		//======================================SOC : TUNER=======================================
		Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_HIGH);
		
		//======================================SOC : TFT=========================================
		Port_SetOutLevel(TFT0_EN,PORT_HIGH);
		Port_SetOutLevel(TFT1_EN,PORT_HIGH);
		Port_SetOutLevel(TFT3_EN,PORT_HIGH);
	
		//======================================SOC : BT==========================================
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_HIGH);
	
		//======================================SOC : OTHER=======================================
		Port_SetOutLevel(UDBATT_AD_EN,PORT_HIGH);//batery voltage contact to s32k adc pin
		Port_SetOutLevel(IOC_FUEL_EN, PORT_HIGH);//enable fuel
		Port_SetOutLevel(FAN_EN,PORT_HIGH);//enable fan control 
		Port_SetOutLevel(WDI_EN,PORT_HIGH); //disable WatchDog
	
		//======================================SOC : RESET end====================================
		//hardware version get
		gCircuitVersion = getCircuitVerion();//must lager than 6ms between adc initial and sample ,onterwise adc sample data invalid
		//soc reset
		Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
		DelayMs(1);
		Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);

		PINS_DRV_Init(NUM_OF_CONFIGURED_PINS_SPI, g_pin_mux_InitConfigArr_Spi);  //OverWrite Set SPI PINS

		//SOC power on ,if sleep should power off it
		gSystemPowerUp = TRUE;
	}
	else//if power up but LVI and HVI not match ,entry standby used to 50ms micro interrupt
	{
		SysLviWakeUpEnable();//enable LVI wake up interrupt,if matched reset soon
		
		TraceInit();
		if(SYS_LVI_APPEAR == GetPinValue_LVI_DET())//Vbat < 7.8V
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "[Main]:LVI system entry standby\n\r");
		}
		else if(SYS_HVI_APPEAR == GetPinValue_LVI_DET1())//Vbat < 7.8V or //Vbat > 16.8V
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "[Main]:HVI system entry standby\n\r");
		}
		
		SysEntryStandby();
	}
	
}

static void prvSetupHardware( void )
{

    /* Initialize and configure clocks
     *  - Setup system clocks, dividers
     *  - see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    POWER_SYS_Init(powerConfigsArr,POWER_MANAGER_CONFIG_CNT,powerStaticCallbacksConfigsArr,1);

    PM_SleepManagement();//add by tjy ,at 2020-12-29

    POWER_SYS_SetMode(0, POWER_MANAGER_POLICY_AGREEMENT);//active HSRUN mode

    GpioSetup();
}

void SYSPriorityManagement(void)
{
	uint16 i = 0 ;
	//Priority : must lager than 5
	for( i = 0 ; i <= FTM7_Ovf_Reload_IRQn ; i++ )
	{
		INT_SYS_SetPriority(i,13);
	}

	INT_SYS_SetPriority(LPSPI0_IRQn,8);//IPC
	INT_SYS_SetPriority(LPSPI1_IRQn,8);//IPC

	INT_SYS_SetPriority(DMA0_IRQn,8);//IPC
	INT_SYS_SetPriority(DMA1_IRQn,8);//IPC
	INT_SYS_SetPriority(DMA2_IRQn,8);//IPC
	INT_SYS_SetPriority(DMA3_IRQn,8);//IPC
	
	INT_SYS_SetPriority(CAN0_ORed_0_15_MB_IRQn,9);
	INT_SYS_SetPriority(CAN0_ORed_16_31_MB_IRQn,9);

	
	INT_SYS_SetPriority(CAN1_ORed_0_15_MB_IRQn,9);
	INT_SYS_SetPriority(CAN1_ORed_16_31_MB_IRQn,9);


	INT_SYS_SetPriority(LPI2C0_Master_IRQn,9);	//AMP GYRO ACCE RTC
	INT_SYS_SetPriority(LPI2C1_Master_IRQn,9);	//EEPROM


	INT_SYS_SetPriority(LPUART2_RxTx_IRQn,9);//AC panel

    INT_SYS_SetPriority(ADC0_IRQn,10);
	INT_SYS_SetPriority(ADC1_IRQn,10);

	INT_SYS_SetPriority(LPUART1_RxTx_IRQn,11);//pritf

	INT_SYS_SetPriority(FTM2_Ch0_Ch1_IRQn,6);

}


/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */
  //int i = 0;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                   ***/
  /* Write your code here */
  /* For example: for(;;) { } */
    prvSetupHardware();

    INT_SYS_SetPriority(ADC0_IRQn,10);
    /* Initialize FTM PWM channel */
    FTM_DRV_Init(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_InitConfig, &flexTimer_pwm5_state);
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_PwmConfig);

	readBootloaderSWVersion();
	
    // fan
    FanPwmInit();
	//FTM_DRV_Init(INST_FLEXTIMER_PWM4_CFG, &flexTimer_pwm4_cfg_InitConfig, &flexTimer_pwm4_state);
	//FTM_DRV_InitPwm(INST_FLEXTIMER_PWM4_CFG, &flexTimer_pwm4_cfg_PwmConfig);

	//speedOutput
	VehicleSpeedTimerInit();

	//waterTemp
	waterTempPwmInit();
	
	// <--2k
	EDMA_DRV_Init(&dmaController_State, &dmaController_cfg_InitConfig0,
                       edmaChnStateArray,
                       edmaChnConfigArray,
                       sizeof(edmaChnStateArray)/sizeof(edma_chn_state_t*));

//gps:
	//GpsUartInit();

//trace:Power On
	TraceInit();
	TracePrintf(TRACE_TRACE, TRACE_INFO, "N532-VAVE-SYSTEM POWER UP(V%s %s)\r\n",gIocSWVersion,gIocSWSubmitterName);

//COM
	Com_Init();
//KEY
	KeyInit();
	SetFanSpeed(gCurrentDuty);

//I2C
	I2C_Init(INST_LPI2C0_CFG);
	I2C_Init(INST_LPI2C1_CFG);

	//Eep_Init();

	/* audio & amp */
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
	
	//extern uint8 TestID;
//	TracePrintf(TRACE_TRACE, TRACE_INFO, "TestID: %d\n\r",TestID);
#endif

	ExtrRtc_Init();

	//AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);
	AdcWrapper_Init(ADC_PAL_INSTANCE_1,&adc1_pal_InitConfig);

   	Tp_Init();
	dtcCoreInit();
	dtcAppInit();
#if  0
   	LcdBL_Init();
	//TempAmp_Init();
	TempBoard_Init();
	POWER_SYS_Init(powerConfigsArr,POWER_MANAGER_CONFIG_CNT,powerStaticCallbacksConfigsArr,1);
	BattVolt_Init();


	WDG_Init(INST_WDOG_PAL_CFG,&wdog_pal_config);
#else
	//POWER_SYS_Init(powerConfigsArr,POWER_MANAGER_CONFIG_CNT,powerStaticCallbacksConfigsArr,1);
	BattVolt_Init();
	ipc_gw_init_M();
	//SysLvi2DetEnable();
#endif
	Task_IpcAppInit();
	SPI_RxTxQueue_Init();
	spiSemCreate();
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "CircuitVerAdVal:%d \n\r",gCircuitVersion);
	gVariantCtrlVersion = getVariantCtrlVerion();
	gITMasterCtrlVersion = getITMasterCtrlVerion();
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "ITMasterCtrlVersion:%d \n\r",gITMasterCtrlVersion);

//	SecondPanelInit();  //remove this features
//	AudioRadioListInit();

#if UPDATE_BOOTLOADER_FEATURE
  	Flash_If_Init();  //Init flash
#endif

#if (STD_ON == MAIN_SM_STATE_USE_TIMER)
	//used for fix some bug :add at 2020-6-5
	//when NM MSG wake up IT-MASTER,some SM time(S1) not match
	INT_SYS_DisableIRQ(FTM0_Ovf_Reload_IRQn);
	FTM_DRV_CounterStop(INST_FLEXTIMER_MC1);
	TracePrintf(TRACE_TRACE, TRACE_INFO, "APP INIT DONE , run time = %dms\r\n",g_nmTimerOffset);
#endif

  /***Don't add some code at the end of these Logic ***/
  SYSPriorityManagement();		//1.when entry RTOS reset used Priority lager than 5
  gSystemInitDoneFlag = TRUE;	//2.when entry FREE RTOS set gSystemInitDoneFlag = TRUE
  SysLvi2DetEnable();			//3.when entry RTOS reset LVI Priority lager than 5
  Ex_Wdg_Enable();				//4.enable external watch dog,add by tjy,@2019-12-12
  Ex_Wdg_Refresh();				//when entry RTOS refresh external watch dog

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
