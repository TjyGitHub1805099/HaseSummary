#if 0
#include "Platform_Types.h"
#include "SpeedOutput.h"



ftm_state_t flexTimer_pwm3_state;

void speedOutputPwmInit(void)
{

	FTM_DRV_Init(INST_FLEXTIMER_PWM3_CFG, &flexTimer_pwm3_cfg_InitConfig, &flexTimer_pwm3_state);
	FTM_DRV_InitPwm(INST_FLEXTIMER_PWM3_CFG, &flexTimer_pwm3_cfg_PwmConfig);
	
}

void speedPwmOutput(uint16 freq)
{
	uint16 PeriodTicks = 0;
	uint16 Ticksoffset = 0;
	if(freq < 2)
	{
		freq = 2;
	}
	else if(freq > 550)
	{
		freq = 550;
	}
	PeriodTicks = FTM_DRV_ConvertFreqToPeriodTicks(INST_FLEXTIMER_PWM3_CFG,freq);

	if((freq >= 150)&&(freq < 200))
	{
		Ticksoffset = 2;
	}
	else if((freq >= 200))
	{
		Ticksoffset = 1;
	}
	PeriodTicks = PeriodTicks + Ticksoffset;
	FTM_DRV_UpdatePwmPeriod(INST_FLEXTIMER_PWM3_CFG,1,PeriodTicks,true);

	FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM3_CFG,0,0,0x4000,0,true);
}

void noSpeedOutput(void)
{
	FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM3_CFG,0,0,0x0,0,true);
}

#endif


