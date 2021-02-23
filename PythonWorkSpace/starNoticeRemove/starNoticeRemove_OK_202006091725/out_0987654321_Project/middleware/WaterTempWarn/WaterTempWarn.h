#ifndef   WATER_TEMP_WARN_H
#define   WATER_TEMP_WARN_H


#define WARN_A_DUTY  0x2000
#define WARN_B_DUTY  0x4000
#define WARN_C_DUTY  0x6000

#define FREQ 4
#define PERIODTICKS_OFFSET  560

typedef enum
{
	NORMAL = 0,
	WARNING_A,
	WARNING_B,
	WARNING_C,
}warningType;


extern void waterTempWarnOutput(warningType warnVal);
extern void waterTempPwmInit(void);

#endif