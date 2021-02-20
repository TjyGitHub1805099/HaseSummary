/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Standby.c
**          Author: tjy
**
**          Date  : 2019-5-5
**          
**          
******************************************************************************/
#include "pin_mux.h"
#include "power_manager.h"
#include "clockManager_cfg.h"
#include "System_Signal.h"

#include "Standby.h"
#include "PowerSupply.h"
#include "SysWakeUp.h"
#include "pin_wrapper_if.h"
#include "pmStateMachine.h"
#include "MemMap.h"

#define TEXT_START_ADDR 0x00032420	//Reset_Handler:0x00032420
#define TEXT_END_ADDR   0x00094176	//from app.map .text.__destroy_global_chain


#define  NUM_OF_PERIPHERAL_STANDBY   	13U
#define CLOCK_MANAGER_CALLBACK_CNT 	0U

#define PIN_INIT  0

#if (0 == PIN_INIT )
//#define NUM_OF_CONFIG_PINS (156-8)
//#define NUM_OF_CONFIG_PINS (156-9)//PTA16 , GPS_RST_Q ,don't deinit.request by se peng cheng from email @2019-10-24
//#define NUM_OF_CONFIG_PINS (156-10)//PTA12 , IMX_TRACE_RST ,don't deinit.request by se peng cheng from email @2019-12-9
//#define NUM_OF_CONFIG_PINS (156-11)//PTD11 , WDG_EN ,don't deinit.request by zhou guang yong from external watch dog @2019-12-12
#define NUM_OF_CONFIG_PINS (156-10)//PTA12 , IMX_TRACE_RST ,deinit as input PULL UP enable.request by se peng cheng solve dark current @2020-1-15

#else
#define NUM_OF_CONFIG_PINS (156)
#endif

extern uint32 g_BootLoaderJumpCmd;
extern boolean gSystemInitDoneFlag;
extern boolean gSystemPowerUp;

extern uint32 g_defaultIsrRegisterRecord[CRASH_CALL_BACK_DEEP];
extern uint32 g_defaultIsrFlag;

NVM_SEC_VAR_UNSPECIFIED	uint32 gCrashRegisterRecord[CRASH_CALL_BACK_DEEP] = {0};


#if 1
//used to end of stand by initial GPIO pin as input mode to adapt dack current less than 1mA
const pin_settings_config_t Standby_pin_mux_InitConfigArr[NUM_OF_CONFIG_PINS] =
{
    {
        .base          = PORTA,
        .pinPortIdx    = 18u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },
	{
        .base          = PORTA,
        .pinPortIdx    = 19u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
        .base          = PORTA,
        .pinPortIdx    = 20u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTE,
		.pinPortIdx    = 16u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
	{
		.base		   = PORTE,
		.pinPortIdx    = 15u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 21u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
	{
		.base		   = PORTD,
		.pinPortIdx    = 1u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
	{
		.base		   = PORTD,
		.pinPortIdx    = 0u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 22u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

#if PIN_INIT
    {
		.base		   = PORTE,
		.pinPortIdx    = 11u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTE,
		.pinPortIdx    = 10u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 13u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 23u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 5u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 4u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 24u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
	{
		.base		   = PORTA,
		.pinPortIdx    = 25u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTB,
		.pinPortIdx    = 7u,
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
		.base		   = PORTB,
		.pinPortIdx    = 6u,
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
		.base		   = PORTA,
		.pinPortIdx    = 26u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTE,
		.pinPortIdx    = 14u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTE,
		.pinPortIdx    = 3u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTA,
		.pinPortIdx    = 27u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
	    .base		   = PORTE,
		.pinPortIdx    = 12u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTA,
		.pinPortIdx    = 28u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 17u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
    {
        .base          = PORTA,
        .pinPortIdx    = 29u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },
	{
		.base		   = PORTA,
		.pinPortIdx    = 30u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTD,
		.pinPortIdx    = 16u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 15u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 9u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 31u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	{
		.base		   = PORTD,
		.pinPortIdx    = 14u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 13u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTB,
		.pinPortIdx    = 18u,
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
        .base          = PORTB,
        .pinPortIdx    = 19u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTB,
		.pinPortIdx    = 20u,
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
        .base          = PORTB,
        .pinPortIdx    = 21u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },


	{
		.base		   = PORTE,
		.pinPortIdx    = 8u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTB,
		.pinPortIdx    = 5u,
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
		.base		   = PORTB,
		.pinPortIdx    = 4u,
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
		.pinPortIdx    = 3u,
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
		.base		   = PORTC,
		.pinPortIdx    = 2u,
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
		.base		   = PORTD,
		.pinPortIdx    = 7u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 6u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 5u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTD,
        .pinPortIdx    = 12u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
#if PIN_INIT
	{
		.base		   = PORTD,
		.pinPortIdx    = 11u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTD,
		.pinPortIdx    = 10u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTC,
        .pinPortIdx    = 1u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 0u,
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
        .pinPortIdx    = 9u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTD,
        .pinPortIdx    = 8u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 17u,
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
		.base		   = PORTC,
		.pinPortIdx    = 16u,
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
        .base          = PORTB,
        .pinPortIdx    = 22u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 15u,
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
		.base		   = PORTB,
		.pinPortIdx    = 23u,
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
        .base          = PORTB,
        .pinPortIdx    = 24u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTC,
        .pinPortIdx    = 14u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTB,
		.pinPortIdx    = 25u,
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
		.base		   = PORTB,
		.pinPortIdx    = 26u,
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
		.base		   = PORTB,
		.pinPortIdx    = 3u,
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
		.base		   = PORTB,
		.pinPortIdx    = 27u,
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
		.base		   = PORTB,
		.pinPortIdx    = 28u,
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
		.base		   = PORTB,
		.pinPortIdx    = 2u,
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
		.base		   = PORTB,
		.pinPortIdx    = 29u,
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
        .base          = PORTB,
        .pinPortIdx    = 30u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
	{
		.base		   = PORTC,
		.pinPortIdx    = 13u,
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
        .base          = PORTB,
        .pinPortIdx    = 31u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
    {
        .base          = PORTC,
        .pinPortIdx    = 18u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 12u,
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
		.base		   = PORTC,
		.pinPortIdx    = 19u,
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
		.base		   = PORTC,
		.pinPortIdx    = 20u,
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
        .base          = PORTC,
        .pinPortIdx    = 21u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 22u,
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
		.base		   = PORTC,
		.pinPortIdx    = 23u,
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
		.base		   = PORTC,
		.pinPortIdx    = 24u,
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
		.base		   = PORTC,
		.pinPortIdx    = 25u,
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
		.base		   = PORTC,
		.pinPortIdx    = 11u,
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
        .base          = PORTC,
        .pinPortIdx    = 26u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTC,
		.pinPortIdx    = 10u,
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
		.base		   = PORTC,
		.pinPortIdx    = 27u,
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
		.base		   = PORTB,
		.pinPortIdx    = 1u,
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
	
#if PIN_INIT
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
#endif

	{
		.base		   = PORTC,
		.pinPortIdx    = 28u,
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
        .base          = PORTC,
        .pinPortIdx    = 9u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTC,
        .pinPortIdx    = 8u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTC,
        .pinPortIdx    = 29u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    
	{
		.base		   = PORTA,
		.pinPortIdx    = 7u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTC,
		.pinPortIdx    = 30u,
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
        .base          = PORTA,
        .pinPortIdx    = 6u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTC,
        .pinPortIdx    = 31u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTC,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTE,
        .pinPortIdx    = 7u,
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
        .base          = PORTD,
        .pinPortIdx    = 18u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTD,
        .pinPortIdx    = 19u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTA,
        .pinPortIdx    = 17u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTB,
        .pinPortIdx    = 17u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTD,
        .pinPortIdx    = 20u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTB,
		.pinPortIdx    = 16u,
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
		.base		   = PORTB,
		.pinPortIdx    = 15u,
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
		.base		   = PORTB,
		.pinPortIdx    = 14u,
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
		.base		   = PORTD,
		.pinPortIdx    = 21u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTB,
        .pinPortIdx    = 13u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
	{
		.base		   = PORTB,
		.pinPortIdx    = 12u,
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
		.base		   = PORTD,
		.pinPortIdx    = 22u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

#if PIN_INIT
	{
		.base		   = PORTD,
		.pinPortIdx    = 4u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

    {
        .base          = PORTD,
        .pinPortIdx    = 3u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
	{
		.base		   = PORTD,
		.pinPortIdx    = 2u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTD,
        .pinPortIdx    = 23u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTA,
		.pinPortIdx    = 3u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 2u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTD,
        .pinPortIdx    = 24u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTB,
        .pinPortIdx    = 11u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTB,
        .pinPortIdx    = 10u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTB,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTD,
        .pinPortIdx    = 25u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTB,
		.pinPortIdx    = 9u,
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
        .base          = PORTD,
        .pinPortIdx    = 26u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTD,
		.pinPortIdx    = 27u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTB,
		.pinPortIdx    = 8u,
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
		.base		   = PORTD,
		.pinPortIdx    = 28u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

    {
        .base          = PORTA,
        .pinPortIdx    = 1u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTD,
        .pinPortIdx    = 29u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTD,
        .direction     = GPIO_INPUT_DIRECTION,
    },
    {
        .base          = PORTA,
        .pinPortIdx    = 0u,
        .pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
        .passiveFilter = false,
        .driveSelect   = PORT_LOW_DRIVE_STRENGTH,
        .mux           = PORT_MUX_AS_GPIO,
        .pinLock       = false,
        .intConfig     = PORT_DMA_INT_DISABLED,
        .clearIntFlag  = false,
        .gpioBase      = PTA,
        .direction     = GPIO_INPUT_DIRECTION,
    },

	{
		.base		   = PORTD,
		.pinPortIdx    = 30u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTD,
		.pinPortIdx    = 31u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTD,
		.direction	   = GPIO_INPUT_DIRECTION,
	},


	{
		.base		   = PORTC,
		.pinPortIdx    = 7u,
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

#if PIN_INIT
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
#endif

    {
        .base          = PORTE,
        .pinPortIdx    = 17u,
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
    
#if PIN_INIT
	{
		.base		   = PORTA,
		.pinPortIdx    = 16u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTE,
		.pinPortIdx    = 18u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
			
	{
		.base		   = PORTA,
		.pinPortIdx    = 15u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 6u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 2u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 19u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},	
	{
		.base		   = PORTE,
		.pinPortIdx    = 20u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 14u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 21u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 22u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 13u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 23u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 24u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 25u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
#if 1//PIN_INIT
	{
		.base		   = PORTA,
		.pinPortIdx    = 12u,
		.pullConfig    = PORT_INTERNAL_PULL_UP_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTA,
		.pinPortIdx    = 11u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
#if PIN_INIT
	{
		.base		   = PORTA,
		.pinPortIdx    = 10u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTE,
		.pinPortIdx    = 1u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 0u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTE,
		.pinPortIdx    = 26u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
	
#if PIN_INIT
	{
		.base		   = PORTC,
		.pinPortIdx    = 5u,
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
#endif

#if PIN_INIT
	{
		.base		   = PORTC,
		.pinPortIdx    = 4u,
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
#endif

	{
		.base		   = PORTA,
		.pinPortIdx    = 5u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

#if PIN_INIT
	{
		.base		   = PORTA,
		.pinPortIdx    = 4u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},
#endif

	{
		.base		   = PORTE,
		.pinPortIdx    = 27u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTE,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 9u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

	{
		.base		   = PORTA,
		.pinPortIdx    = 8u,
		.pullConfig    = PORT_INTERNAL_PULL_NOT_ENABLED,
		.passiveFilter = false,
		.driveSelect   = PORT_LOW_DRIVE_STRENGTH,
		.mux		   = PORT_MUX_AS_GPIO,
		.pinLock	   = false,
		.intConfig	   = PORT_DMA_INT_DISABLED,
		.clearIntFlag  = false,
		.gpioBase	   = PTA,
		.direction	   = GPIO_INPUT_DIRECTION,
	},

};

#endif


void CloseAllInt(void)
{
	uint32_t  ulNewBASEPRI=1;
	__asm volatile
	(
		"	msr primask, %0	" :: "r" ( ulNewBASEPRI )
	);
}

void reInitGpioPin(void)
{
	PINS_DRV_Init(NUM_OF_CONFIG_PINS, Standby_pin_mux_InitConfigArr);
}

//sys entry stanby sequence
void SysEntryStandby(void)
{
	Ex_Wdg_Refresh();//add by tjy,@2019-12-12

	//Stop task Scheduler
	if(TRUE == gSystemInitDoneFlag)
	{
		DisableIrq();
		vTaskSuspendAll();
	}

	//set backram flag ,used for bootloader jump to app as soon as possible when wake up
	g_BootLoaderJumpCmd = JUMP_TO_APP_AT_ONCE;

	//disable PCI interrupt
	INT_SYS_DisableIRQ(CAN0_ORed_0_15_MB_IRQn);
	INT_SYS_DisableIRQ(CAN0_ORed_16_31_MB_IRQn);
	INT_SYS_DisableIRQ(CAN1_ORed_0_15_MB_IRQn);
	INT_SYS_DisableIRQ(CAN1_ORed_16_31_MB_IRQn);
	
	INT_SYS_DisableIRQ(ADC0_IRQn);
	INT_SYS_DisableIRQ(ADC1_IRQn);
	
	INT_SYS_DisableIRQ(DMA0_IRQn);
	INT_SYS_DisableIRQ(DMA1_IRQn);
	INT_SYS_DisableIRQ(DMA2_IRQn);
	INT_SYS_DisableIRQ(DMA3_IRQn);

	INT_SYS_DisableIRQ(LPI2C0_Master_IRQn);
	INT_SYS_DisableIRQ(LPI2C1_Master_IRQn);
	INT_SYS_DisableIRQ(LPUART2_RxTx_IRQn);
	INT_SYS_DisableIRQ(LPUART1_RxTx_IRQn);
	INT_SYS_DisableIRQ(FTM2_Ch0_Ch1_IRQn);

	INT_SYS_DisableIRQ(LPSPI0_IRQn);
	INT_SYS_DisableIRQ(LPSPI1_IRQn);

	//power off SOC
	if( TRUE == gSystemPowerUp)
	{
		Ex_Wdg_Refresh();//add by tjy,@2019-12-12
		PowerOff();
	}
	//release dump energy
	//DelayMs(1000);
	//used solve dack current less than 1mA
	PINS_DRV_Init(NUM_OF_CONFIG_PINS, Standby_pin_mux_InitConfigArr);

	//Ex_Wdg_Disable();//move to SystemSoftwareReset(), add by tjy,@2019-12-12 ,must be disable wdg!!!!!!
	
	//don't move ,must ahead judge of gInterruptAwake ,clear it
	gSystemsleeping = FALSE;
	if(TRUE == gInterruptAwake)
	{
		gInterruptAwake = FALSE;
		DelayMs(1000);//used to ensure PMIC power off thorough,when wake up event apeared and need wake up system.
		SystemSoftwareReset(PM_RESET,FALSE);
	}

#if(STD_ON == SYS_SLEEP_IN_MAIN)//if need reset and sleep in main
	g_SysSleepInMainCmd = JUMP_TO_APP_AT_ONCE;
	SystemSoftwareReset(PM_RESET,FALSE);
#endif

	
	//set wake up event
	//SysWakeUpEnable();
	SysLviWakeUpEnable();
	SysLocationWakeUpEnable();
	SysCanWakeUpEnable();

	//if VLPR mode only can be switched from RUN mode
	if(POWER_SYS_GetCurrentMode() != POWER_MANAGER_RUN)
		POWER_SYS_SetMode(2, POWER_MANAGER_POLICY_AGREEMENT);//active RUN mode
	
	CLOCK_SYS_UpdateConfiguration(1U, CLOCK_MANAGER_POLICY_AGREEMENT);
	POWER_SYS_SetMode(1, POWER_MANAGER_POLICY_AGREEMENT);
}



//=====================================standby enable=====================================
/* Interrupt service routine for LVI and HVI*/
static void SysLviDetSleepISR(void)
{
    /* Clear button IRQ flag */
    PINS_DRV_ClearPortIntFlagCmd(PORTE);

	if(TRUE == gSystemInitDoneFlag)
	{
		//SysLviSleep = TRUE;
		System_Signal_Send(SYSTEM_SIG_ID_LVI_SLEEP,0, STD_ON);
	}
	else
	{
		Soc_Power_Onoff(STD_OFF);
		SystemSoftwareReset(LVI_SLEEP,TRUE);
	}
}

/* LVI sleep interrupt enable*/
void SysLvi2DetEnable(void)
{
	/* Install hander for LVI ISR */
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_INT_RISING_EDGE);//0->1
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_INT_FALLING_EDGE);//1->0

	if( TRUE == gSystemInitDoneFlag )
		INT_SYS_SetPriority(PORTE_IRQn,8);//in RTOS
	else
		INT_SYS_SetPriority(PORTE_IRQn,4);
	
	INT_SYS_InstallHandler(PORTE_IRQn, SysLviDetSleepISR,NULL);
	/* Enable IGN port IRQ */
	INT_SYS_EnableIRQ(PORTE_IRQn);
}

//if sys need to go to sleep , should disable all interruput except NVM such as i2c 
void DisableIrq(void)
{
    uint8 i = 0;
    for(i=0;i<=FTM7_Ovf_Reload_IRQn;i++)
	{
	  if(( i != LPI2C0_Master_IRQn ) && ( i != LPI2C1_Master_IRQn))
	  {
		  INT_SYS_DisableIRQ(i);
	  }	
	}
}

void fault_backtrace(cpu_addr_t lr, fault_exc_frame_t *frame)
{
    
#if 0
	fault_info_t info;
    fault_gather_information(lr, frame, &info);

    fault_dump_information(&info);

    fault_dump_task(&info);

    fault_dump_cpu_frame(&frame->cpu_frame);

#if defined (TOS_CFG_CPU_ARM_FPU_EN) && (TOS_CFG_CPU_ARM_FPU_EN == 1U)
    if (info.is_ext_stk_frm) {
        fault_dump_fpu_frame(&frame->fpu_frame);
    }
#endif

    fault_call_stack_backtrace(&info, K_FAULT_CALL_STACK_BACKTRACE_DEPTH);

    cpu_fault_diagnosis();

    fault_spin();
#endif
	//fault_dump_cpu_frame(&frame->cpu_frame);

	uint8 i = 0;
	uint8 j = 0;
	uint8 counter = 0;
	fault_cpu_frame_t *cpu_frame = &frame->cpu_frame;
	uint32 *dataPtr = (uint32*)cpu_frame;
	
	for(i=0;i<8;i++)
	{
		dataPtr = (uint32*)cpu_frame;
		for(j=0;j<8;j++)
		{
			if(dataPtr != NULL)
			{
				//Reset_Handler:0x00032420
				if((*dataPtr) >= TEXT_START_ADDR && (*dataPtr)<TEXT_END_ADDR)
				{
					if(counter<CRASH_CALL_BACK_DEEP)
					{
						g_defaultIsrRegisterRecord[counter++] = *dataPtr;
					}
				}
				dataPtr += 1;
			}
			else
				break;
		}
		cpu_frame+= 1;
	}
}

#if 0//remove by tjy at 2019-10-22
void DefaultISR(void)
{
	PowerOff();
	DEA_SetResetMagicWord(0);

	g_defaultIsrFlag = CRASH_HISTORY_FLAG;
	memset(g_defaultIsrRegisterRecord,0,sizeof(g_defaultIsrRegisterRecord));
	__asm volatile (
		"MOV	 r0, lr\n\t"
		"TST	 lr, #0x04\n\t"
		"ITE	 EQ\n\t"
		"MRSEQ	 r1, MSP\n\t"
		"MRSNE	 r1, PSP\n\t"
		"BL 	 fault_backtrace\n\t"
	);
	g_BootLoaderJumpCmd = JUMP_TO_APP_AT_ONCE;

	
	SystemSoftwareReset(DEFAULT_ISR,TRUE);
}
#endif
