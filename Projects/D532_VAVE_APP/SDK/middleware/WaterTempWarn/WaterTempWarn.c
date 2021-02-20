

#include "Platform_Types.h"
#include "WaterTempWarn.h"
#include "flexTimer_pwm1_cfg.h"



ftm_state_t flexTimer_pwm1_state;

void waterTempPwmInit(void)
{
	uint16 PeriodTicks = 0;
	FTM_DRV_Init(INST_FLEXTIMER_PWM1_CFG, &flexTimer_pwm1_cfg_InitConfig, &flexTimer_pwm1_state);
	FTM_DRV_InitPwm(INST_FLEXTIMER_PWM1_CFG, &flexTimer_pwm1_cfg_PwmConfig);
	PeriodTicks = FTM_DRV_ConvertFreqToPeriodTicks(INST_FLEXTIMER_PWM1_CFG,FREQ);
	PeriodTicks = PeriodTicks+PERIODTICKS_OFFSET;
	FTM_DRV_UpdatePwmPeriod(INST_FLEXTIMER_PWM1_CFG,1,PeriodTicks,true);	
}

void waterTempWarnOutput(warningType warnVal)
{
	switch(warnVal)
	{
	case NORMAL:
		//FTM_DRV_DeinitPwm(INST_FLEXTIMER_PWM1_CFG);
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1_CFG,0,0,0x0,0,true);
		break;
	case WARNING_A:
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1_CFG,0,0,WARN_A_DUTY,0,true);
		break;
	case WARNING_B:
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1_CFG,0,0,WARN_B_DUTY,0,true);
		break;
	case WARNING_C:
		FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM1_CFG,0,0,WARN_C_DUTY,0,true);
		break;
	default:
		break;
	}
}


