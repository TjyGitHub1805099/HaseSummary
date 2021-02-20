#ifndef   FANCTRL_H
#define   FANCTRL_H

#define FAN_PWM_CHANNEL		6

extern void SetFanSpeed(uint8 PwmDuty);
extern void FanPwmInit(void);
extern uint16 GetFanFreq(void);
#endif