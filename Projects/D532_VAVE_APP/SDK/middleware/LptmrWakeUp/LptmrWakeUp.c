
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :LptmrWakeUp.c
**          Author: huang
**
**          Date  : 2020-06-16
**          
**          
******************************************************************************/

#include "LptmrWakeUp.h"
#include "lptmr_driver.h"

static void Lptmr0WakeUpISR(void)
{
    /* Clear Compare Flag */
	LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);
}

void LptmrWakeUpInit(void)
{
	LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, FALSE);

	/* Install handler for the Timer overflow interrupt and enable it */
	INT_SYS_InstallHandler(LPTMR0_IRQn, &Lptmr0WakeUpISR, (isr_t*) 0);

	INT_SYS_SetPriority(LPTMR0_IRQn,4);
}

void WakeUpSourceDiasbleIRQ(void)
{
	//Disable the all wakeup sourece
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_ACC_ON_OFF,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTE,PINNUM_LVI_DET,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTA,PINNUM_IGN_DET,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTB,PINNUM_CAN_RX1,PORT_DMA_INT_DISABLED);
	PINS_DRV_SetPinIntSel(PORTC,PINNUM_CAN_RX2,PORT_DMA_INT_DISABLED);

	//Clear the all wakeup source interrupt flag
	PINS_DRV_ClearPortIntFlagCmd(PORTE);  //ACC and LVI HVI wakeup source
	PINS_DRV_ClearPortIntFlagCmd(PORTA);  //IGN
	PINS_DRV_ClearPortIntFlagCmd(PORTB);  //CAN RX
	PINS_DRV_ClearPortIntFlagCmd(PORTC);  //CAN RX

	INT_SYS_DisableIRQ(PORTE_IRQn);  //ACC and LVI HVI wakeup source
	INT_SYS_DisableIRQ(PORTA_IRQn);  //IGN wakeup source
	INT_SYS_DisableIRQ(PORTB_IRQn);  //disable CAN RX interrupt
	INT_SYS_DisableIRQ(PORTC_IRQn);

	LPTMR_DRV_StopCounter(INST_LPTMR1);
	INT_SYS_DisableIRQ(LPTMR0_IRQn);  //lptimer0 source
}

void LptmrStartCounter(void)
{
	LPTMR_DRV_StartCounter(INST_LPTMR1);
	INT_SYS_EnableIRQ(LPTMR0_IRQn);
}







