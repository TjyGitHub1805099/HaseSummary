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
#include "dmaController_cfg.h"
#include "flash_cfg.h"
#include "lpi2c0_cfg.h"
#include "lpi2c1_cfg.h"
#include "flexTimer_pwm3_cfg.h"
#include "flexTimer_pwm4_cfg.h"
#include "flexTimer_pwm5_cfg.h"
#include "hwCrc_cfg.h"
#include "uart_debug_pal.h"
#include "spi_ipc_pal_cfg.h"
#include "pwrManager_cfg.h"
#include "spi_M_ipc_pal_cfg.h"
#include "FreeRTOS.h"
#include "adc0_pal_cfg.h"
#include "adc1_pal_cfg.h"
#include "can0_cfg.h"
#include "wdog_pal_cfg.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "BoardDefines.h"
#include "Std_Types.h"
#include "iic_if.h"
#include "eeprom.h"
#include "trace.h"
#include "pin_mux.h"
#include "Spi_Ipc_gw_api.h"
#include "Delay.h"
#include "download.h"
#include "Flash_if.h"
#include "SPI_CircularQueue.h"

#include "adc_wrapper_if.h"
#include "HardwareVersion.h"


//#define USB_UPDATE_ENABLE_MFG_FLAG 0x165456EF
#define BACKRAM_LANGUAGE 	*((uint8_t*)(0x2001E7FB))	//0x2001E800-5	value: 0-english not 0:chinese


__attribute__ ((section(".backRamStart"))) uint32 g_BootLoaderJumpCmd;

typedef void(*JumpToPtr)(void);

//#define DEBUG_USBSWDL

extern void Rtos_Init(void);
#define PEX_RTOS_INIT Rtos_Init

extern void Rtos_Start(void);
#define PEX_RTOS_START Rtos_Start

extern uint8 gIocSWVersion[];

uint32 gClearUpdataFlag1 = 0;

boolean gSystemInitDoneFlag = FALSE;

uint32 l_UpdateFlag = 0;
uint32 l_CombinedKeyFlag=0;

JumpToPtr	pJumpTo;
const uint32 *pAppEntry =0x00032004;

uint8 gCircuitVersion = 0xFF;

void JumpToApp()
{
	INT_SYS_DisableIRQ(LPUART0_RxTx_IRQn);
	pJumpTo = *pAppEntry;
	pJumpTo();
}

//#define GetPinValue_STREE_A()                             Port_GetPinLevel(ACC_ON_OFF)
//#define GetPinValue_STREE_B()                             Port_GetPinLevel(ACC_ON_OFF)

/**********************************
*
*	CAUTION:
*	in bootloader,if enable CAN clock,
*	when CAN speed is too high,
*	it will be failed to write flash.
*	so, it's better to disable CAN module,make sure disable CAN MODULE CLOCK
*	And,also set lower interrupt priority for uart/spi of IPC.
**********************************/

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

void GpioSetup(void)
{
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS,g_pin_mux_InitConfigArr);
	Port_SetOutLevel(WDI_EN,PORT_HIGH);	//disable WatchDog
	//AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);
}


static void prvSetupHardware( void )
{
  if( JUMP_TO_APP_AT_ONCE == g_BootLoaderJumpCmd )
  {
	g_BootLoaderJumpCmd = 0 ;
	JumpToApp();
  }
  /* Initialize and configure clocks
   *  - Setup system clocks, dividers
   *  - see clock manager component for more details
   */
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
				 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

  GpioSetup();
}

void UsbBootGpioInit(void)
{

	//PINS_DRV_Init(8, g_pin_mux_InitConfigArr1);
#if    1
		//Port_SetOutLevel(CAN_STB1,PORT_HIGH);
		//Port_SetOutLevel(CAN_STB2,PORT_HIGH);
		//Port_SetOutLevel(CAN_EN1,PORT_HIGH);
		//Port_SetOutLevel(CAN_EN2,PORT_HIGH);
		//Port_SetOutLevel(CAN_WAKE1,PORT_LOW);
		//Port_SetOutLevel(CAN_WAKE2,PORT_LOW);

		//CanIf_Init_0();
		//CanIf_Init_1();

		//Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
		Port_SetOutLevel(UDBATT_AD_EN,PORT_HIGH);
		Port_SetOutLevel(PWR_SYNC,PORT_LOW);
		DelayMs(10);

		Port_SetOutLevel(UGPWR_ON,PORT_HIGH);
		Port_SetOutLevel(UG3V3_ON,PORT_HIGH);
		Port_SetOutLevel(PMIC_PWR_ON,PORT_HIGH);
		DelayMs(10);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		DelayMs(10);
		Port_SetOutLevel(UG6V5_ON,PORT_HIGH);
		Port_SetOutLevel(UG8V_ON,PORT_HIGH);
		Port_SetOutLevel(UG5V_ON,PORT_HIGH);
		Port_SetOutLevel(GPS_ON,PORT_HIGH);
		Port_SetOutLevel(GPS_ANT_PWR_ON,PORT_HIGH);
		Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_HIGH);


		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_HIGH);
		Port_SetOutLevel(TFT0_EN,PORT_HIGH);// lcd default closed
		Port_SetOutLevel(TFT1_EN,PORT_HIGH);// lcd default closed
		Port_SetOutLevel(TFT3_EN,PORT_HIGH);
		Port_SetOutLevel(UG_CAMMERA_ON,PORT_LOW);
		Port_SetOutLevel(FAN_EN,PORT_HIGH);
		Port_SetOutLevel(IOC_AP_ON_OFF,PORT_HIGH);
		Port_SetOutLevel(DSP_RST_N,PORT_HIGH);
		DelayMs(5);
		Port_SetOutLevel(CHARGE_EN,PORT_HIGH);
		Port_SetOutLevel(CHARGE_CTL3,PORT_HIGH);

		Port_SetOutLevel(AMP_D_EN,PORT_HIGH);
		Port_SetOutLevel(AMP_D_MUTEN,PORT_HIGH);
		Port_SetOutLevel(TUNER_ANT_PWR_ON,PORT_HIGH);
		
		gCircuitVersion = getCircuitVerion();
		INT_SYS_DisableIRQ(ADC0_IRQn);
		INT_SYS_SetPriority(ADC0_IRQn,10);

		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "CircuitVersion:%d \n\r",gCircuitVersion);
		if( gCircuitVersion >= 0 )
		{
			Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
			DelayMs(1);
			Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
		}
		else
		{
			Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
			DelayMs(1);
			Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
		}

		Port_SetOutLevel(SPI_M_IPC_REQ , PORT_HIGH);
		Port_SetOutLevel(SPI_M_IPC_ACK , PORT_HIGH);
#endif
}

void BootModuleInit(void)
{
	AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);
	ipc_gw_init_M();
	SPI_RxTxQueue_Init();
	spiSemCreate();
	Flash_If_Init();
	downloadInit();
	UsbBootGpioInit();
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS_SPI, g_pin_mux_InitConfigArr_Spi);  //OverWrite Set SPI PINS
}

boolean checkCombinedKeyStatus(void)
{
	uint16 counter = 0;
	uint16 i = 0;
	boolean ret = FALSE;
	
	for(i=0;i<200;i++)
	{
		DelayUs(100);
		if((Port_GetPinLevel(STREEA_DET) == STD_OFF)&&(Port_GetPinLevel(STREEB_DET) == STD_OFF))
		{
			counter++;
		}
	}

	if(counter >= 160)
		ret = TRUE;

	return ret;
}

void Soc_Power_Onoff(boolean onoff)
{
	if(onoff == STD_ON)
	{
		Port_SetOutLevel(PMIC_PWR_ON,PORT_HIGH);
		Port_SetOutLevel(UGPWR_ON,PORT_HIGH);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_HIGH);
		Port_SetOutLevel(TFT0_EN,PORT_HIGH);
		Port_SetOutLevel(TFT1_EN,PORT_HIGH);
		Port_SetOutLevel(TFT3_EN,PORT_HIGH);
		//Port_SetOutLevel(UG_CAMMERA_ON,PORT_HIGH);
		DelayMs(20);
		if( gCircuitVersion >= 0 )
		{
			Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
			DelayMs(1);
			Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
		}
		else
		{
			Port_SetOutLevel(IMX_TRACE_RST, PORT_LOW);
			DelayMs(1);
			Port_SetOutLevel(IMX_TRACE_RST, PORT_HIGH);
		}
	}
	else
	{
		Port_SetOutLevel(PMIC_PWR_ON,PORT_LOW);
		Port_SetOutLevel(UGPWR_ON,PORT_LOW);
		Port_SetOutLevel(PMIC_EN,PORT_LOW);
		Port_SetOutLevel(PWR_BT_WLAN_ON,PORT_LOW);
		Port_SetOutLevel(TFT0_EN,PORT_LOW);
		Port_SetOutLevel(TFT1_EN,PORT_LOW);
		Port_SetOutLevel(TFT3_EN,PORT_LOW);
		Port_SetOutLevel(UG_CAMMERA_ON,PORT_LOW);
	}
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
	//int i;
	Std_ReturnType ret = E_NOT_OK;
	
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internalp initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

    prvSetupHardware();
	EDMA_DRV_Init(&dmaController_State, &dmaController_cfg_InitConfig0,
                       edmaChnStateArray,
                       edmaChnConfigArray,
                       sizeof(edmaChnStateArray)/sizeof(edma_chn_state_t*));
	TraceInit();

	TracePrintf(TRACE_TRACE, TRACE_INFO, "N532-VAVE-BOOTLOADER: POWER UP(V%s)\r\n",gIocSWVersion);
	//TracePrintf(INPUT_TRACE, TRACE_NONMASK, "CircuitVersion:%d \n\r",gCircuitVersion);

	INT_SYS_SetPriority(DMA0_IRQn,8);
	INT_SYS_SetPriority(DMA1_IRQn,8);
	INT_SYS_SetPriority(DMA2_IRQn,8);
	INT_SYS_SetPriority(DMA3_IRQn,8);

	INT_SYS_SetPriority(LPSPI0_IRQn,8);
	INT_SYS_SetPriority(LPSPI1_IRQn,8);
	
	INT_SYS_SetPriority(LPUART1_RxTx_IRQn,9);
	INT_SYS_SetPriority(LPUART2_RxTx_IRQn,9);
	I2C_Init(INST_LPI2C1_CFG);
	#ifdef    DEBUG_USBSWDL
	BootModuleInit();
	Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
	TracePrintf(TRACE_TRACE, TRACE_INFO, "USB BOOTLOADER: INIT DONE\r\n");
	#else
	//static uint32 gClearUpdataFlag1 = 0;
	//Eep_Write(0,(uint8 *)(&gClearUpdataFlag1),sizeof(gClearUpdataFlag1));
	ret = Eep_Read(4,(uint8 *)&l_CombinedKeyFlag,4);
	if(ret != E_OK)
		TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER:A IIC ERR!\r\n");
	else
		TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER:A IIC OK\r\n");
	if(DISABLE_COMBINED_KEY_FLAG != l_CombinedKeyFlag)
	{
		Port_SetOutLevel(UG3V3_ON, PORT_HIGH);
		DelayMs(30);
		if(checkCombinedKeyStatus()==TRUE)
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: MFG INIT START\r\n");
			Port_SetOutLevel(IOC_BOOT_MODE,PORT_HIGH);
			BootModuleInit();
			TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: MFG INIT END\r\n");

			DelayMs(6000);
			if(checkCombinedKeyStatus()==TRUE)
			{
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT START\r\n");
				Soc_Power_Onoff(STD_OFF);
				Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
				DelayMs(1000);
				Soc_Power_Onoff(STD_ON);
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT END\r\n");
			}
			
		}
		else
		{
			ret = Eep_Read(0,(uint8 *)&l_UpdateFlag,4);
			if(ret != E_OK)
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER:B IIC ERR!\r\n");
			else
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER:B IIC OK\r\n");
			if(USB_UPDATE_ENABLE_FLAG == l_UpdateFlag)
			{
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT START\r\n");
				Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
				if( 0 == BACKRAM_LANGUAGE)
				{
					Port_SetOutLevel(SPI_IPC_REQ,PORT_LOW);
				}
				else
				{
					Port_SetOutLevel(SPI_IPC_REQ,PORT_HIGH);
				}
				BootModuleInit();
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT END\r\n");
			}
			else
			{
				TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: JUMP APP\r\n");
				JumpToApp();
			}
		}
	}
	else
	{
		ret = Eep_Read(0,(uint8 *)&l_UpdateFlag,4);
		if(USB_UPDATE_ENABLE_FLAG == l_UpdateFlag)
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT START\r\n");
			Port_SetOutLevel(IOC_BOOT_MODE,PORT_LOW);
			BootModuleInit();
			TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: USB INIT END\r\n");
		}
		else
		{
			TracePrintf(TRACE_TRACE, TRACE_INFO, "BOOTLOADER: JUMP APP\r\n");
			JumpToApp();
		}
	}

	
	#endif
	
	INT_SYS_SetPriority(LPI2C1_Master_IRQn,9);
   	gSystemInitDoneFlag = TRUE;

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


void lpi2c0_cfg_MasterCallbackAMP(uint32_t instance,lpi2c_master_event_t masterEvent,void *userData){}
void lpi2c0_cfg_MasterCallbackRTC(uint32_t instance,lpi2c_master_event_t masterEvent,void *userData){}
void lpi2c0_cfg_MasterCallbackGYRO(uint32_t instance,lpi2c_master_event_t masterEvent,void *userData){}
void lpi2c0_cfg_MasterCallbackACCE(uint32_t instance,lpi2c_master_event_t masterEvent,void *userData){}


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
