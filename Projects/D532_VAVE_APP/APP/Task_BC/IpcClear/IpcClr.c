
#include "IpcClr.h"
#include "odo.h"
#include "dea.h"


/**定义时间计时结束值**/
uint32_t Ipc_time = 0;

/**定义时间计时初始值**/
uint32_t Ipc_time_last = 0;

/**定义熄火到达2h标志位**/
uint8_t  Ipc_timeout = 0;

/**定义一个虚拟标志位**/
uint8_t  First_mode = 0;


/**该函数暂时模拟从RTC时间模块读取时间，暂时定义返回值单位为S**/
uint32_t  RTC_Time()
{	
	static uint32_t Real_time = 0;
	return Real_time;
}


void Reset_CurrentTripData()
{
	if(Ipc_timeout == 1)
	{
		Ipc_timeout = 0;

		ODO_ResetTripB(); /**本次里程清零**/

		AVS_ResetTripBAVS(); /**本次里程平均车速,本次里程行驶时间清零**/

		AFC_vResetB(); /**本次里程平均油耗清零**/
		
	}
	
}

/*******/

/**熄火超过两小时，清除与本次里程相关的数据，如:本次里程平均车速，本次平均油耗，本次里程，本次行驶时间**/
void CurrentTrip_MAIN()
{
	static IGN_STAT_ENUM IgnStatBak = IGN_OFF;
	uint32_t Ipc_time_err = 0; /**定义熄火时间**/

	if(DEA_GetIgnStat() == IGN_OFF) //IGN_ON->IGN_OFF
	{
		if(IgnStatBak == IGN_ON) // IGN_ON before
		{
			IgnStatBak = IGN_OFF;
			
			Ipc_time_last = RTC_Time(); 

			Ipc_timeout = 0;
			
		}
		
	}
	else //IGN_OFF->IGN_ON
	{
		if(IgnStatBak == IGN_OFF) // IGN_OFF before
		{
			IgnStatBak = IGN_ON;

			Ipc_time = RTC_Time(); 
			
			Ipc_time_err = Ipc_time - Ipc_time_last;
			
			if(Ipc_time_err < 7200) /**熄火时间还未达到2h**/
			{
				Ipc_timeout = 0;
			}
			else  /**熄火达到2h**/          		
			{
				Ipc_timeout = 1;  /**置位超时标志位**/
			}

			Reset_CurrentTripData();
		}
	}
	
}





