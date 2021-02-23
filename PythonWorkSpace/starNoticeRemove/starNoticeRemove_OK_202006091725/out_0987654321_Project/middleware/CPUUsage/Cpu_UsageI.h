


#define usercfgCPU_USAGE_CALCULATE    0

#if (usercfgCPU_USAGE_CALCULATE==1)
#define usercfgCPU_USAGE_CALC_PERIOD 	1000//ms


extern void CpuUserCount(void);
extern void CpuUserTask_Idle(void);
extern void uTaskCPUUsageInit(void);

#endif



