
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "StackMacros.h"
#include "Cpu_UsageI.h"

#include "System_Signal.h"
#include "System_Task.h"


#if (usercfgCPU_USAGE_CALCULATE==1)
static uint32 max_idle_cnt=3641;
static uint32 idle_cntl=0;
static uint32 idle_cnth=0;
static uint32 tick_cnt=0;
uint32 OSCPUusage=0;
boolean StartFlag =0;

void CpuUserCount(void)
{
	if(StartFlag == 0)
	{
		tick_cnt++;
		if(tick_cnt>=usercfgCPU_USAGE_CALC_PERIOD)
		{
		    
		    OSCPUusage=(idle_cnth*100)/max_idle_cnt;
		    idle_cntl=0;
		    idle_cnth=0;
		    tick_cnt=0;
		}
	}
}

void CpuUserTask_Idle(void)
{
	#if    1
	idle_cntl++;
	if(idle_cntl>=100)
	{
		idle_cntl=0;
		idle_cnth++;
	}
	#else
	idle_cnth++;
	#endif
}

void uTaskCPUUsageInit(void)
{
#if   0
    vTaskDelay(2);
    taskENTER_CRITICAL();
    max_idle_cnt=0;
    idle_cntl=0;
    idle_cnth=0;
    tick_cnt=0;
    taskEXIT_CRITICAL();

    
    vTaskDelay(usercfgCPU_USAGE_CALC_PERIOD/portTICK_PERIOD_MS);
    
    taskENTER_CRITICAL();
    max_idle_cnt=idle_cnth;
	StartFlag = 0;
    taskEXIT_CRITICAL();
#endif
}


#endif







