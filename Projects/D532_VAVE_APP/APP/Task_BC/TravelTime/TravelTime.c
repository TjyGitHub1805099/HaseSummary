#include "dea.h"
#include "TravelTime.h"
#include "string.h"
#include "Std_Types.h"



uint32_t TravelTimeOldValue = 0;

/**travel tinme information**/
TRAVEL_TIMEInfor	TravelTime;




/**reset the time data**/
void ResetTime(void)
{
	DEA_SetFilterTravelTime_H(0);
	DEA_SetFilterTravelTime_M(0);
	DEA_SetFilterTravelTime_S(0);
	
	DEA_SetFilterTravelTimeValid(0);
}

void Write_Time(uint8_t hour,uint8_t minume,uint8_t second,uint8_t valid)
{
	DEA_SetFilterTravelTime_H(hour);
	DEA_SetFilterTravelTime_M(minume);
	DEA_SetFilterTravelTime_S(second);
	
	DEA_SetFilterTravelTimeValid(valid);
}

void TravelTime_Init(void)
{

	if(DEA_GetSysRstType() == RST_IRRST)
	{	
		memset(&TravelTime,0,sizeof(TravelTime));
		Write_Time(TravelTime.Time_h,TravelTime.Time_m,TravelTime.Time_s,TravelTime.TimeValid);

	}
}



void TravelTime_VMAIN(void)	//1S cycle
{
	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;

	uint16_t temp = 0;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnBak == IGN_OFF)
		{
			s_IgnBak = IGN_ON;

			TravelTime.Timecnt = TravelTimeOldValue;	//get the last time
		}
		
		TravelTime.Timecnt++;

		if(TravelTime.Timecnt < 3600)	//less than 1 hour
		{
			if(TravelTime.Timecnt >= 60)	//over 1min
			{
				TravelTime.Time_m = TravelTime.Timecnt/60; //get the minume
				TravelTime.Time_s = TravelTime.Timecnt%60; //get the second
			}
			else
			{
				TravelTime.Time_m = 0;
				TravelTime.Time_s = TravelTime.Timecnt;
			}

			TravelTime.TimeValid = 1;
			TravelTime.Time_h = 0;
		}
		else
		{
			if(TravelTime.Timecnt < TIMECNT_MAX)	//over 1 hour & less than 100 hour
			{
				TravelTime.Time_h = TravelTime.Timecnt/3600; //get the hour
				temp = TravelTime.Timecnt%3600; 
				TravelTime.Time_m = temp/60;	//get the minume

				TravelTime.TimeValid = 1;
				TravelTime.Time_s = 0;
			}
			else
			{
				TravelTime.Time_s = 0;
				TravelTime.Time_h = 0;
				TravelTime.Time_m = 0;

				TravelTime.Timecnt = 0;
				TravelTime.TimeValid = 1;
			}
		}

		Write_Time(TravelTime.Time_h,TravelTime.Time_m,TravelTime.Time_s,TravelTime.TimeValid);
		
	}
	else
	{
		if(s_IgnBak == IGN_ON)
		{
			s_IgnBak = IGN_OFF;

			TravelTimeOldValue = TravelTime.Timecnt;	//save the travel time sum
		}

		memset(&TravelTime,0,sizeof(TravelTime));
		Write_Time(TravelTime.Time_h,TravelTime.Time_m,TravelTime.Time_s,TravelTime.TimeValid);
	}
}


