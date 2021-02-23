/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :SysWakeUp.c
**          Author: tjy
**
**          Date  : 2019-5-5
**          
**          
******************************************************************************/
#include "pin_mux.h"
#include "SysWakeUp.h"
#include "System_Signal.h"
#include "BoardDefines.h"
#include "interrupt_manager.h"
#include "system_S32K148.h"

#include "Standby.h"
#include "main.h"



//used to when sleep trigered : LVI and 7ms matched or normal 2s over
//sys entry sleep sequence,if wake up event apear,set gInterruptAwake
//when end of sleep seqence ,if gInterruptAwake = TRUE ,reset sys
volatile boolean gSystemsleeping = FALSE;
volatile boolean gInterruptAwake = FALSE;

//=====================================wake up event deal=====================================
#if 0
/* Interrupt service routine for tcu_acc */
static void SysTcu_Acc2WakeUpISR(void)
{
    /* Clear button IRQ flag */
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
/* Interrupt service routine for acc and lvi_det1 */
static void SysAccAndLviDetWakeUpISR(void)
{
	/* Clear button IRQ flag */
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

	#if(STD_ON == SYS_SLEEP_IN_MAIN)//don't move ,disable interrupt avoid in normal use ACC on
		/* Disable ACC and LVI HVI port IRQ */
		INT_SYS_DisableIRQ(PORTE_IRQn);
	#endif
}

/* Interrupt service routine for CAN1 RX */
static void SysCan1WakeUpISR(void)
{
    /* Clear button CAN1 flag */
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
	
	#if(STD_ON == SYS_SLEEP_IN_MAIN)//don't move ,disable interrupt avoid in normal use ACC on
		/* Disable CAN1 port IRQ */
		//INT_SYS_DisableIRQ(PORTB_IRQn);//move to main.c avoid when CAN1 arrive between SysCanWakeUpEnable() and SLEEP ,changed by tjy @2019-12-18
	#endif
}

/* Interrupt service routine for CAN2 RX */
static void SysCan2WakeUpISR(void)
{
	//uint32 isfr = 0 ;
	//isfr = PINS_DRV_GetPortIntFlag(PORTC);
    /* Clear button CAN2 flag */
	PINS_DRV_ClearPortIntFlagCmd(PORTC);
	//PINS_DRV_ClearPinIntFlagCmd(PORTC,PINNUM_CAN_RX2);
	//isfr = PINS_DRV_GetPortIntFlag(PORTC);

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
	
	#if(STD_ON == SYS_SLEEP_IN_MAIN)//don't move ,disable interrupt avoid in normal use ACC on
		/* Disable CAN2 port IRQ */
		//INT_SYS_DisableIRQ(PORTC_IRQn);//move to main.c avoid when CAN2 arrive between SysCanWakeUpEnable() and SLEEP ,changed by tjy @2019-12-18
	#endif
}

//=====================================wake up enable=====================================
//used to when lvi detected ,delay 7ms,if lvi hold,set lvi wake up enable
void SysLviWakeUpEnable(void)
{
	/* Install hander for LVI_DET ISR */
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_INT_FALLING_EDGE);//lvi_det ( > 7.8v )

	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET1,PORT_INT_RISING_EDGE);//lvi_det1 ( > 7.8v  or < 16.8v)

	if(TRUE == gSystemPowerUp)
	{
		INT_SYS_SetPriority(PORTE_IRQn,8);//in RTOS
	}
	else
	{
		INT_SYS_SetPriority(PORTE_IRQn,4);
	}
	
	INT_SYS_InstallHandler(PORTE_IRQn, SysAccAndLviDetWakeUpISR,NULL);
	/* Enable LVI port IRQ */
	INT_SYS_EnableIRQ(PORTE_IRQn);
}

void SysLocationWakeUpEnable(void)
{
	/* Install hander for ACC ISR */
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_ACC_ON_OFF,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_ACC_ON_OFF,PORT_INT_FALLING_EDGE);//acc

	/* Install hander for TCU_ACC ISR */
	//PINS_DRV_SetPinIntSel(PORTD,PINNUM_TCU_ACC_DET,PORT_DMA_INT_DISABLED);
	//PINS_DRV_SetPinIntSel(PORTD,PINNUM_TCU_ACC_DET,PORT_INT_FALLING_EDGE);//tcu_acc

	/* Install hander for IGN ISR */
	//PINS_DRV_SetPinIntSel(PORTA,PINNUM_IGN_DET,PORT_DMA_INT_DISABLED);
	//PINS_DRV_SetPinIntSel(PORTA,PINNUM_IGN_DET,PORT_INT_FALLING_EDGE);//ign


	if(TRUE == gSystemPowerUp)//in RTOS
	{
		INT_SYS_SetPriority(PORTE_IRQn,8);//acc and lvi_det		
		//INT_SYS_SetPriority(PORTD_IRQn,8);//tcu_acc
		//INT_SYS_SetPriority(PORTA_IRQn,8);//ign
	}
	else//in main
	{
		INT_SYS_SetPriority(PORTE_IRQn,4);//acc and lvi_det
		//INT_SYS_SetPriority(PORTD_IRQn,4);//tcu_acc
		//INT_SYS_SetPriority(PORTA_IRQn,4);//ign
	}

	INT_SYS_InstallHandler(PORTE_IRQn, SysAccAndLviDetWakeUpISR,NULL);
	//INT_SYS_InstallHandler(PORTD_IRQn, SysTcu_Acc2WakeUpISR,NULL);

	//INT_SYS_InstallHandler(PORTA_IRQn, SysIgn2WakeUpISR,NULL);
	
	/* Enable button port IRQ */
	INT_SYS_EnableIRQ(PORTE_IRQn);
	//INT_SYS_EnableIRQ(PORTD_IRQn);
	//INT_SYS_EnableIRQ(PORTA_IRQn);
}

void SysCanWakeUpEnable(void)
{
	/* Install hander for CAN1 RX ISR */
	PINS_DRV_SetPinIntSel(PORTB,PINNUM_CAN_RX1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTB,PINNUM_CAN_RX1,PORT_INT_FALLING_EDGE);//can1 rx
	
	/* Install hander for CAN2 RX ISR */
	PINS_DRV_SetPinIntSel(PORTC,PINNUM_CAN_RX2,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTC,PINNUM_CAN_RX2,PORT_INT_FALLING_EDGE);//can2 rx

	if(TRUE == gSystemPowerUp)//in RTOS
	{
		INT_SYS_SetPriority(PORTB_IRQn,8);//can1 rx
		INT_SYS_SetPriority(PORTC_IRQn,8);//can2 rx
	}
	else//in main
	{
		INT_SYS_SetPriority(PORTB_IRQn,4);//can1 rx
		INT_SYS_SetPriority(PORTC_IRQn,4);//can2 rx
	}

	INT_SYS_InstallHandler(PORTB_IRQn, SysCan1WakeUpISR,NULL);
	INT_SYS_InstallHandler(PORTC_IRQn, SysCan2WakeUpISR,NULL);
	
	/* Enable button port IRQ */
	INT_SYS_EnableIRQ(PORTB_IRQn);
	INT_SYS_EnableIRQ(PORTC_IRQn);
}


