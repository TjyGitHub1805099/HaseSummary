/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : VelSpdPulseOut.c
**          Author: yangxl
**
**          Date  : 2018-12-26 13:35:10
**          
**          
******************************************************************************/

/******************************************************************************
**	  INCLUDES
******************************************************************************/
#include "VelSpdPulseOut.h"
#include "S32K148.h"
#include "system_S32K148.h"
#include "BoardDefines.h"
#include "Cpu.h"
#include "pin_mux.h"
#include "clockManager_cfg.h"




extern uint16_t GetVehicleSpeed(void); /* units 0.1KM/H  */

static VelSpdOut_Pulse s_SpdOutPulse = {0u};
ftm_state_t flexTimer_oc1_state;
static uint8_t SPEED_OUT_PIN = 0;


/**************************************************************************** 
Function	: VehicleSpeedChangeFreq        
Input		: units 0.1KM/H          
Output		: units 0.1HZ       
Return		: 
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void VehicleSpeedChangeFreq(uint16_t speed)
{
	uint32_t s_VehicleSpeed = 0ul;
	
	s_VehicleSpeed = speed;
	
	if((s_VehicleSpeed >= SPEED_MIN_OUT)&&(s_VehicleSpeed <= SPEED_MAX_OUT))
	{
		s_VehicleSpeed = s_VehicleSpeed*PPK_VALUE;     
		s_SpdOutPulse.freq = (uint16_t)(s_VehicleSpeed/3600ul);
		
	}else
	{
		s_SpdOutPulse.freq = 0ul;
	}
}

/**************************************************************************** 
Function	: GetVehicleSpeedFreq        
Input		:          
Output		:         
Return		: units 0.1HZ 
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint16_t GetVehicleSpeedFreq(void)
{
	return s_SpdOutPulse.freq;
}

#if 1

/**************************************************************************** 
Function	: VehicleSpeedTimerInit        
Input		:        
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void VehicleSpeedTimerInit(void)
{
	/* Initialize Flex Timer instance as simple timer */
	FTM_DRV_Init(SPEED_TIMER_INST, &flexTimer_oc1_InitConfig, &flexTimer_oc1_state);
	/* Install handler for the Timer interrupt and enable it */
	INT_SYS_InstallHandler(SPEED_TIMER_INT, &SpdInterrupt, (isr_t*) 0);
	INT_SYS_EnableIRQ(SPEED_TIMER_INT);
	FTM_DRV_EnableInterrupts(SPEED_TIMER_INST, FTM_CHANNEL0_INT_ENABLE);
}

/**************************************************************************** 
Function	: VehicleSpeedTimerStart        
Input		:         
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void VehicleSpeedTimerStart(void)
{
	FTM_DRV_InitOutputCompare(SPEED_TIMER_INST, &flexTimer_oc1_OutputCompareConfig);
}


/**************************************************************************** 
Function	: VehicleSpeedTimerStop        
Input		:         
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void VehicleSpeedTimerStop(void)
{
	FTM_DRV_DeinitOutputCompare(SPEED_TIMER_INST, &flexTimer_oc1_OutputCompareConfig);
	SPEED_OUT_PIN = 0;
	PINS_DRV_WritePin(PTB_BASE, 8, SPEED_OUT_PIN);
}

/**************************************************************************** 
Function	: VehicleSpeedOut        
Input		: Freq units 0.1HZ         
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
uint16_t VehicleSpeedOut(uint16_t Freq)
{
	uint16_t s_tintc = 0ul;
	uint16_t s_Freq = 0ul;
	uint32_t s_period = 0ul;

	s_Freq = Freq;
#if 0
	if((s_SpdOutPulse.timflg == 1u)&&(s_Freq != 0u))
	{
		//s_period = TIM_BUS_FREQ*10/(s_Freq*2);  /* units 1us */
		s_period = TIM_BUS_FREQ*5u/s_Freq;  /* units 1us */
		s_SpdOutPulse.timcnt = (uint16_t)(s_period/TIM_INTER_MAX_PERIOD);
		s_SpdOutPulse.timTc = (uint16_t)(s_period%TIM_INTER_MAX_PERIOD);
		s_SpdOutPulse.timflg = 0u;
		
	}
	else
	{
		;
	}

#endif

	if(s_SpdOutPulse.timcnt>0u)
	{
		s_SpdOutPulse.timcnt--;
		s_tintc = TIM_INTER_MAX_PERIOD;
	}
	else
	{
		if(s_SpdOutPulse.timTc == 0u)
		{
			if(s_Freq != 0u)
			{
				SPEED_OUT_PIN = !(SPEED_OUT_PIN);
				//s_tintc = 50;
				#if 1
				s_period = TIM_BUS_FREQ*5u/s_Freq;  /* units 1us */
				s_SpdOutPulse.timcnt = (uint16_t)(s_period/TIM_INTER_MAX_PERIOD);
				s_SpdOutPulse.timTc = (uint16_t)(s_period%TIM_INTER_MAX_PERIOD);
				s_SpdOutPulse.timflg = 0u;
				if(s_SpdOutPulse.timcnt > 0u)
				{
					s_tintc = TIM_INTER_MAX_PERIOD;
					s_SpdOutPulse.timcnt--;
				}
				else
				{
					s_tintc = s_SpdOutPulse.timTc;
					s_SpdOutPulse.timTc = 0u;
					s_SpdOutPulse.timcnt = 0u;
				}
				#endif
			}
			else
			{
				SPEED_OUT_PIN = 0u;
				s_tintc = TIM_INTER_MAX_PERIOD;
			}
			s_SpdOutPulse.timflg = 1u;
			
		}
		else
		{
			s_tintc = s_SpdOutPulse.timTc;
			s_SpdOutPulse.timTc = 0u;
			s_SpdOutPulse.timcnt = 0u;
		}
	}
	PINS_DRV_WritePin(PTB_BASE, 8, SPEED_OUT_PIN);
	return s_tintc;
}


/**************************************************************************** 
Function	: SpdInterrupt        
Input		:         
Output		:         
Return		:  
Author	    : yangxl
Description :        
Date  		: 
*****************************************************************************/
void SpdInterrupt (void)
{
	const uint32_t  maxCounterValue = 0xFFFF;
	uint16_t next_cnt;
	uint32_t counterValue = FTM_DRV_CounterRead(SPEED_TIMER_INST);
	//Port_SetOutLevel(IOC_SPEED_OUTPUT,(count_cnt%2));
	//PINS_DRV_WritePin(PTB_BASE, 8, (count_cnt%2));
	
	next_cnt = VehicleSpeedOut(GetVehicleSpeedFreq());
	if (next_cnt > (maxCounterValue - counterValue))
    {
        next_cnt = (uint16_t)(next_cnt - (maxCounterValue - counterValue));
    }
    else
    {
        next_cnt = (uint16_t)(counterValue + next_cnt);
    }
	FTM_DRV_UpdateOutputCompareChannel(SPEED_TIMER_INST, 0, next_cnt, FTM_ABSOLUTE_VALUE, false);
	FTM_DRV_ClearStatusFlags(SPEED_TIMER_INST, FTM_CHANNEL0_FLAG);
}

#endif

