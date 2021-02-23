
#include "pin_mux.h"
#include "SysWakeUp.h"
#include "System_Signal.h"
#include "BoardDefines.h"
#include "interrupt_manager.h"
#include "system_S32K148.h"

#include "Standby.h"
#include "main.h"






volatile boolean gSystemsleeping = FALSE;
volatile boolean gInterruptAwake = FALSE;


#if 0

static void SysTcu_Acc2WakeUpISR(void)
{
    
	PINS_DRV_ClearPortIntFlagCmd(PORTD);
	
	if(TRUE == gSystemsleeping)
	{
		gInterruptAwake = TRUE;
	}
	else
	{
    	SystemSoftwareReset(TCUACC_WAKEUP,TRUE);
	}
}
#endif

static void SysAccAndLviDetWakeUpISR(void)
{
	
	PINS_DRV_ClearPortIntFlagCmd(PORTE);
	
	if(TRUE == gSystemsleeping)
	{
		gInterruptAwake = TRUE;
	}
	else
	{	
		#if(STD_ON != SYS_SLEEP_IN_MAIN)
			SystemSoftwareReset(ACC_WAKEUP,TRUE);
		#endif
	}

	#if(STD_ON == SYS_SLEEP_IN_MAIN)
		
		INT_SYS_DisableIRQ(PORTE_IRQn);
	#endif
}


static void SysCan1WakeUpISR(void)
{
    
    PINS_DRV_ClearPortIntFlagCmd(PORTB);

	if(TRUE == gSystemsleeping)
	{
		gInterruptAwake = TRUE;
	}
	else
	{
		#if(STD_ON != SYS_SLEEP_IN_MAIN)
    		SystemSoftwareReset(CAN_WAKEUP,TRUE);
		#endif
	}
	
	#if(STD_ON == SYS_SLEEP_IN_MAIN)
		
		
	#endif
}


static void SysCan2WakeUpISR(void)
{
	
	
    
	PINS_DRV_ClearPortIntFlagCmd(PORTC);
	
	

	if(TRUE == gSystemsleeping)
	{
		gInterruptAwake = TRUE;
	}
	else
	{
		#if(STD_ON != SYS_SLEEP_IN_MAIN)
    		SystemSoftwareReset(CAN_WAKEUP,TRUE);
		#endif
	}
	
	#if(STD_ON == SYS_SLEEP_IN_MAIN)
		
		
	#endif
}



void SysLviWakeUpEnable(void)
{
	
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_INT_FALLING_EDGE);

	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_INT_RISING_EDGE);

	if(TRUE == gSystemPowerUp)
	{
		INT_SYS_SetPriority(PORTE_IRQn,8);
	}
	else
	{
		INT_SYS_SetPriority(PORTE_IRQn,4);
	}
	
	INT_SYS_InstallHandler(PORTE_IRQn, SysAccAndLviDetWakeUpISR,NULL);
	
	INT_SYS_EnableIRQ(PORTE_IRQn);
}

void SysLocationWakeUpEnable(void)
{
	
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_ACC_ON_OFF,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_ACC_ON_OFF,PORT_INT_FALLING_EDGE);

	
	
	

	
	
	


	if(TRUE == gSystemPowerUp)
	{
		INT_SYS_SetPriority(PORTE_IRQn,8);
		
		
	}
	else
	{
		INT_SYS_SetPriority(PORTE_IRQn,4);
		
		
	}

	INT_SYS_InstallHandler(PORTE_IRQn, SysAccAndLviDetWakeUpISR,NULL);
	

	
	
	
	INT_SYS_EnableIRQ(PORTE_IRQn);
	
	
}

void SysCanWakeUpEnable(void)
{
	
	PINS_DRV_SetPinIntSel(PORTB,PINNUM_CAN_RX1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTB,PINNUM_CAN_RX1,PORT_INT_FALLING_EDGE);
	
	
	PINS_DRV_SetPinIntSel(PORTC,PINNUM_CAN_RX2,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTC,PINNUM_CAN_RX2,PORT_INT_FALLING_EDGE);

	if(TRUE == gSystemPowerUp)
	{
		INT_SYS_SetPriority(PORTB_IRQn,8);
		INT_SYS_SetPriority(PORTC_IRQn,8);
	}
	else
	{
		INT_SYS_SetPriority(PORTB_IRQn,4);
		INT_SYS_SetPriority(PORTC_IRQn,4);
	}

	INT_SYS_InstallHandler(PORTB_IRQn, SysCan1WakeUpISR,NULL);
	INT_SYS_InstallHandler(PORTC_IRQn, SysCan2WakeUpISR,NULL);
	
	
	INT_SYS_EnableIRQ(PORTB_IRQn);
	INT_SYS_EnableIRQ(PORTC_IRQn);
}


