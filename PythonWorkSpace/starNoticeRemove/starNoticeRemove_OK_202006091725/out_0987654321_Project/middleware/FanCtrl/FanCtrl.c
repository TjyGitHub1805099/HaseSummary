

#include "Platform_Types.h"
#include "FanCtrl.h"
#include "flexTimer_pwm4_cfg.h"
#include "trace.h"
#include "flexTimer_oc1.h"
#include "ftm_common.h"
#include "FanBack_Pwm2_Cap.h"

ftm_state_t flexTimer_pwm2_state;
ftm_state_t flexTimer_pwm4_state;
uint32  FanCapfrequency;
void FanPwmInit(void)
{
	
	FTM_DRV_Init(INST_FANBACK_PWM2_CAP, &FanBack_Pwm2_Cap_InitConfig, &flexTimer_pwm2_state);
	FTM_DRV_InitInputCapture(INST_FANBACK_PWM2_CAP, &FanBack_Pwm2_Cap_InputCaptureConfig);

	FanCapfrequency = FTM_DRV_GetFrequency(INST_FANBACK_PWM2_CAP);

	FTM_DRV_Init(INST_FLEXTIMER_PWM4_CFG, &flexTimer_pwm4_cfg_InitConfig, &flexTimer_pwm4_state);
	FTM_DRV_InitPwm(INST_FLEXTIMER_PWM4_CFG, &flexTimer_pwm4_cfg_PwmConfig);

}

void SetFanSpeed(uint8 PwmDuty)
{
	uint32 dutyVal = 0;
	
	
	
	
	
    if(PwmDuty > 100)
	{
		PwmDuty = 100;
	}
	dutyVal = (100 - PwmDuty)*0x8000/100;
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[FanCtrl-N]>dutyVal:0x%x \n\r",dutyVal);
	
	FTM_DRV_UpdatePwmChannel(INST_FLEXTIMER_PWM4_CFG,0,0,dutyVal,0,true);	
}

uint16 GetFanFreq(void)
{
	 uint16  inputCaptureMeas = 0;

	 inputCaptureMeas = FTM_DRV_GetInputCaptureMeasurement(INST_FANBACK_PWM2_CAP, FAN_PWM_CHANNEL);
	 
	 if(0 != inputCaptureMeas)
	 {
		  
          inputCaptureMeas = FanCapfrequency / (inputCaptureMeas);
	 }
	 return (inputCaptureMeas);
}



