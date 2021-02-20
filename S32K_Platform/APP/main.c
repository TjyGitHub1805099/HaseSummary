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
#include "adc0_pal_cfg.h"
#include "hwCrc_cfg.h"
#include "dmaController_cfg.h"
#include "eim_cfg.h"
#include "erm_cfg.h"
#include "can2_cfg.h"
#include "FreeRTOS.h"
#include "lpi2c0_cfg.h"
#include "pwrManager_cfg.h"
#include "rtcTimer_cfg.h"
#include "flexTimer_pwm3_cfg.h"
#include "flexTimer_pwm5_cfg.h"
#include "wdog_pal_cfg.h"
#include "uart0_pal_cfg.h"
#include "uart1_pal_cfg.h"
#include "uart2_pal_cfg.h"
#include "spi2_pal.h"
#include "tp_if.h"
#include "dtcCore_if.h"

  volatile int exit_code = 0;
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "interrupt_manager.h"
#include "clock_manager.h"
#include "timers.h"
#include "BoardDefines.h"
#include "trace.h"
#include "CanIf.h"
#include "Port_if.h"
#include "uart_pal.h"
#include "KeyBackLight.h"
#include "LcdBackLight.h"
#include "TempAmp.h"
#include "TempBoard.h"
#include "adc_wrapper_if.h"

extern void Reset_Handler(void);
const volatile uint32_t APP __attribute__ ((section(".cfmConfigure"))) = (uint32_t)Reset_Handler;
const volatile uint32_t RCHW __attribute__ ((section(".cfmConfigure"))) = 0x015A015A;
const volatile uint32_t DELAY __attribute__ ((section(".cfmConfigure"))) = 5000000;
//const volatile uint32_t APPLOC __attribute__ ((section(".cfmConfigure"))) = (uint32_t)&APPKEY;


extern void Rtos_Init(void);
#define PEX_RTOS_INIT Rtos_Init

extern void Rtos_Start(void);
#define PEX_RTOS_START Rtos_Start

void GpioSetup(void)
{
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    /* Configure ports */
    PINS_DRV_ClearPins(PORT_IOC_BOOT_MODE, (1 << PORT_PIN_IOC_BOOT_MODE));
    PINS_DRV_SetPins(PORT_UDBATT_AD_EN, (1 << PORT_PIN_UDBATT_AD_EN));
    PINS_DRV_SetPins(PORT_PMIC2_FEED_5V0_ON, (1 << PORT_PIN_PMIC2_FEED_5V0_ON));
    PINS_DRV_SetPins(PORT_PMIC_PWRON, (1 << PORT_PIN_PMIC_PWRON));
    PINS_DRV_SetPins(PORT_UG5V_ON, (1 << PORT_PIN_UG5V_ON));
    PINS_DRV_SetPins(PORT_UG3V3_ON, (1 << PORT_PIN_UG3V3_ON));
    PINS_DRV_SetPins(PORT_UG1V8_ON, (1 << PORT_PIN_UG1V8_ON));
    PINS_DRV_SetPins(PORT_UG1V2_ON, (1 << PORT_PIN_UG1V2_ON));
    PINS_DRV_SetPins(PORT_LCD_PWR_ON, (1 << PORT_PIN_LCD_PWR_ON));
    PINS_DRV_SetPins(PORT_BL_PWR_ON, (1 << PORT_PIN_BL_PWR_ON));
    PINS_DRV_SetPins(PORT_BL_EN, (1 << PORT_PIN_BL_EN));
    PINS_DRV_SetPins(PORT_CHARGE_5V_ON, (1 << PORT_PIN_CHARGE_5V_ON));
    PINS_DRV_SetPins(PORT_VCC12VOUT_EN, (1 << PORT_PIN_VCC12VOUT_EN));
    PINS_DRV_SetPins(PORT_PWR_BT_WLAN_ON, (1 << PORT_PIN_PWR_BT_WLAN_ON));

    PINS_DRV_SetPins(PORT_CAN_STB, (1 << PORT_PIN_CAN_STB));
    PINS_DRV_SetPins(PORT_CAN_WAKE, (1 << PORT_PIN_CAN_WAKE));
    PINS_DRV_SetPins(PORT_CAN_EN, (1 << PORT_PIN_CAN_EN));

	PINS_DRV_SetPins(PORT_CHARGE1_CTL1, (1 << PORT_PIN_CHARGE1_CTL1));
    PINS_DRV_SetPins(PORT_CHARGE1_CTL2, (1 << PORT_PIN_CHARGE1_CTL2));
    PINS_DRV_SetPins(PORT_CHARGE1_CTL3, (1 << PORT_PIN_CHARGE1_CTL3));
	PINS_DRV_SetPins(PORT_CHARGE1_EN, (1 << PORT_PIN_CHARGE1_EN));

    PINS_DRV_SetPins(PORT_4G_RST_3V3, (1 << PORT_PIN_4G_RST_3V3));
    PINS_DRV_SetPins(PORT_4G_PWRKEY_3V3, (1 << PORT_PIN_4G_PWRKEY_3V3));
    PINS_DRV_SetPins(PORT_4G_DTR_3V3, (1 << PORT_PIN_4G_DTR_3V3));
    PINS_DRV_SetPins(PORT_4G_RI_3V3, (1 << PORT_PIN_4G_RI_3V3));
    PINS_DRV_SetPins(PORT_AMP_STB, (1 << PORT_PIN_AMP_STB));
    PINS_DRV_SetPins(PORT_IOC_PMIC_WDI, (1 << PORT_PIN_IOC_PMIC_WDI));
    PINS_DRV_SetPins(PORT_MCU_SLEEP_STATUS, (1 << PORT_PIN_MCU_SLEEP_STATUS));
    PINS_DRV_SetPins(PORT_MCU_WAKEUP_4G, (1 << PORT_PIN_MCU_WAKEUP_4G));

    PINS_DRV_SetPins(PORT_SPI_IPC_ACK, (1 << PORT_PIN_SPI_IPC_ACK));
    PINS_DRV_SetPins(PORT_SPI_IPC_REQ, (1 << PORT_PIN_SPI_IPC_REQ));
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

    GpioSetup();
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
    ftm_state_t flexTimer_pwm3_state, flexTimer_pwm5_state;
	lpi2c_master_state_t i2c_master_state;

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/

    /* Write your code here */
    /* For example: for(;;) { } */
	
    prvSetupHardware();
    /*Periphery Initialize*/

	//if(POWER_SYS_GetResetSrcStatusCmd(RCM, RCM_WATCH_DOG) != true)
	{
	
	}
	
    /* Initialize FTM PWM channel */
    FTM_DRV_Init(INST_FLEXTIMER_PWM3_CFG, &flexTimer_pwm3_cfg_InitConfig, &flexTimer_pwm3_state);
    FTM_DRV_Init(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_InitConfig, &flexTimer_pwm5_state);
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM3_CFG, &flexTimer_pwm3_cfg_PwmConfig);
    FTM_DRV_InitPwm(INST_FLEXTIMER_PWM5_CFG, &flexTimer_pwm5_cfg_PwmConfig);

	EDMA_DRV_Init(&dmaController_State, &dmaController_InitConfig,
                       edmaChnStateArray,
                       edmaChnConfigArray,
                       6);
	TraceInit();
	CanIf_Init_2();
	
	AdcWrapper_Init(ADC_PAL_INSTANCE_0,&adc0_pal_InitConfig);
	LPI2C_DRV_MasterInit(0,&lpi2c0_MasterConfig,&i2c_master_state);
	KeyBL_Init();
	LcdBL_Init();
	TempAmp_Init();
	TempBoard_Init();
	POWER_SYS_Init((power_manager_user_config_t * (*)[])powerConfigsArr,1,(power_manager_callback_user_config_t * (*)[])powerStaticCallbacksConfigsArr,1);
	BattVolt_Init();

	Tp_Init();
	dtcCoreInit();
	
	WDG_Init(INST_WDOG_PAL_CFG,&wdog_pal_config);
	
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
