/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : VelSpdPulseOut.h
**          Author: yangxl
**
**          Date  : 2018-12-26 13:36:39
**          
**          
******************************************************************************/
#ifndef _VELSPDPULSEOUT_H_
#define _VELSPDPULSEOUT_H_

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include"Std_Types.h"
#include "flexTimer_oc1.h"

/******************************************************************************
**    MACROS
******************************************************************************/
//#define SPEED_OUT_PIN						/* GPIO_PIN */

#define SPEED_MAX_OUT 			(3500U)   	/* units 0.1km/h */
#define SPEED_MIN_OUT 			(20U)		/* units 0.1km/h */

#define TIM_BUS_FREQ 			(3500000UL)	/* units HZ */
#define TIM_INTER_MAX_PERIOD 	(TIM_BUS_FREQ/100UL)/* units 10MS */

#define PPK_VALUE 				(637UL*8UL)

#define SPEED_TIMER_INST		INST_FLEXTIMER_OC1
#define SPEED_TIMER(num)		(FTM##num##_Ch0_Ch1_IRQn)
#if SPEED_TIMER_INST == 1U
	#define SPEED_TIMER_INT			SPEED_TIMER(1)
#elif SPEED_TIMER_INST == 2U
	#define SPEED_TIMER_INT			SPEED_TIMER(2)
#elif SPEED_TIMER_INST == 3U
	#define SPEED_TIMER_INT			SPEED_TIMER(3)
#elif SPEED_TIMER_INST == 4U
	#define SPEED_TIMER_INT			SPEED_TIMER(4)
#elif SPEED_TIMER_INST == 5U
	#define SPEED_TIMER_INT			SPEED_TIMER(5)
#elif SPEED_TIMER_INST == 6U
	#define SPEED_TIMER_INT			SPEED_TIMER(6)
#elif SPEED_TIMER_INST == 7U
	#define SPEED_TIMER_INT			SPEED_TIMER(7)
#elif SPEED_TIMER_INST == 8U
	#define SPEED_TIMER_INT			SPEED_TIMER(8)
#else
	#error "SPEED_TIMER_INST not ok!"
#endif


/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/
typedef struct
{
	uint8_t timflg;
	uint16_t timcnt;
	uint16_t timTc;
	uint16_t freq;
} VelSpdOut_Pulse;

extern void VehicleSpeedTimerInit(void);
extern void VehicleSpeedTimerStart(void);
extern void VehicleSpeedTimerStop(void);
extern void VehicleSpeedChangeFreq(uint16_t speed);
extern uint16_t GetVehicleSpeedFreq(void);
extern void SpdInterrupt (void);


#endif


