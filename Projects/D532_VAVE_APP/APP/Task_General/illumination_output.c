#if 1
#include "Platform_Types.h"
#include "WaterTempWarn.h"
//#include "flexTimer_pwm7_cfg.h"
#include "illumination_output.h"


//ftm_state_t flexTimer_pmw7_state;

void illumination_output_init(void)
{
#if 0
	uint16 PeriodTicks = 0;
	FTM_DRV_Init(INST_FLEXTIMER_PWM7_CFG, &flexTimer_pwm7_cfg_InitConfig, &flexTimer_pmw7_state);
	FTM_DRV_InitPwm(INST_FLEXTIMER_PWM7_CFG, &flexTimer_pwm7_cfg_PwmConfig);
	//PeriodTicks = FTM_DRV_ConvertFreqToPeriodTicks(INST_FLEXTIMER_PWM7_CFG,400);
	//PeriodTicks = PeriodTicks+PERIODTICKS_OFFSET;
	//FTM_DRV_UpdatePwmPeriod(INST_FLEXTIMER_PWM7_CFG, 1, 200, true);

	FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM7_CFG, 0, FTM_PWM_UPDATE_IN_DUTY_CYCLE, 0, 0, true);
#endif
}

void illumination_output(int duty)
{
	//FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM7_CFG, 0, 0, (duty*0x8000)/1000, 0, true);
}

#endif
