#include "dea.h"
#include "IdleStopTime.h"
#include "MemMap.h"

//#define IDLE_STOP_STATE_MACHINE

BACKRAM_SEC_VAR_UNSPECIFIED uint32_t CumulativeValue_cnt;
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t FuelCumulativeValue_cnt;
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t FuelCumulativeValue;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t Cumulative_h;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t Cumulative_m;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t Cumulative_s;


#ifdef IDLE_STOP_STATE_MACHINE
IDLETIME_STATE IDLETIME_state = OFF;
IDLETIME_STATE IDLETIME_state_last = OFF;
#endif

uint32_t TripValue_cnt = 0;
uint32_t FuelTripValue_cnt = 0;

uint32_t FuelLtripValue = 0;
uint8_t Ltrip_h = 0;
uint8_t Ltrip_m = 0;
uint8_t Ltrip_s = 0;

uint8_t IDLE_FUEL_CumulativeFlag = 0;
uint8_t IDLE_FUEL_TripFlag = 0;

#define IDLE_FUEL_CumulativeOnOffFlagGet() (SysSettingAutoStopSumGet())
#define IDLE_FUEL_TripOnOffFlagGet() (SysSettingAutoStopSingleGet())

uint16_t IDLE_FUEL_GetEngineSpeed(void)
{
	return DEA_GetRawTACHO();
}

void IDLE_FUEL_CumulativeReset(void)
{
	IDLE_FUEL_CumulativeFlag = 1;
}

void IDLE_FUEL_TripReset(void)
{
	IDLE_FUEL_TripFlag = 1;
}

uint8_t IDLE_FUEL_CumulativeFlagGetAndReset(void)
{
	if(IDLE_FUEL_CumulativeFlag)
	{
		IDLE_FUEL_CumulativeFlag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IDLE_FUEL_TripFlagGetAndReset(void)
{
	if(IDLE_FUEL_TripFlag)
	{
		IDLE_FUEL_TripFlag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}


void IDLE_FUEL_Init(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{	
		FuelCumulativeValue = 0;

	    Cumulative_h = 0;
		Cumulative_m = 0;
		Cumulative_s = 0;

		CumulativeValue_cnt = 0;
		FuelCumulativeValue_cnt = 0;

		FuelCumulativeValue = 0;
		
		Cumulative_h = 0;
		Cumulative_m = 0;
		Cumulative_s = 0;
	}
}

void IDLE_FUEL_CumulativeWrite(uint32_t FuelCumulativeValue, uint8_t Cumulative_h, uint8_t Cumulative_m, uint8_t Cumulative_s)
{
	DEA_SetFilterfuelCumulativeValue(FuelCumulativeValue);
	DEA_SetFilterCumulativeValue_H(Cumulative_h);
	DEA_SetFilterCumulativeValue_M(Cumulative_m);
	DEA_SetFilterCumulativeValue_S(Cumulative_s);
}

void IDLE_FUEL_TripWrite(uint32_t FuelLtripValue, uint8_t Ltrip_h, uint8_t Ltrip_m, uint8_t Ltrip_s)
{
	DEA_SetFilterfuelLtripValue(FuelLtripValue);		
	DEA_SetFilterlTripValue_H(Ltrip_h);
	DEA_SetFilterlTripValue_M(Ltrip_m);
	DEA_SetFilterlTripValue_S(Ltrip_s);
}

void IDLE_FUEL_CumulativeClear(void)
{
	CumulativeValue_cnt = 0;	//count clear
	FuelCumulativeValue_cnt = 0;
	Cumulative_h = 0;
	Cumulative_m = 0;
	Cumulative_s = 0;
	FuelCumulativeValue = 0;
}


void IDLE_FUEL_TripClear(void)
{
	TripValue_cnt = 0;
	FuelTripValue_cnt= 0;
	Ltrip_h = 0;
	Ltrip_m = 0;
	Ltrip_s = 0;
	FuelLtripValue = 0;
}

void IDLE_FUEL_vMAIN()
{
	static IGN_STAT_ENUM IGN_state = IGN_OFF;
	static uint16_t cycle_cntidle = 0, cycle_cntidleSum = 0;
	static uint8_t s_idle_fuel_cumulative_reset_flag = 0, s_idle_fuel_trip_reset_flag = 0;
	uint8_t idlestatus = 0;

	if((semiActiveIdleStopFunctionGet()) 
		&& (IDLE_FUEL_TripOnOffFlagGet() || IDLE_FUEL_CumulativeOnOffFlagGet()))
	{
		if((DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0) 
			&& (DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2) == 0))
		{
			if(IDLE_FUEL_GetEngineSpeed() == 0)
			{
				idlestatus = DEA_GetRawIdleStopStatusValue();
			}
			else
			{
				idlestatus = 0;
			}
		}
		else
		{
			idlestatus = 0;
		}
		
		if(DEA_GetIgnStat() == IGN_ON)
		{
			if(IGN_state != IGN_ON)
			{
				IGN_state = IGN_ON;
				IDLE_FUEL_CumulativeWrite(FuelCumulativeValue, Cumulative_h, Cumulative_m, Cumulative_s);

				cycle_cntidle = 0;
				IDLE_FUEL_TripClear();
				IDLE_FUEL_TripWrite(FuelLtripValue, Ltrip_h, Ltrip_m, Ltrip_s);
			}

			if(IDLE_FUEL_CumulativeFlagGetAndReset())
			{
				s_idle_fuel_cumulative_reset_flag = 1;
				cycle_cntidleSum = 0;
				IDLE_FUEL_CumulativeClear();
				IDLE_FUEL_CumulativeWrite(FuelCumulativeValue, Cumulative_h, Cumulative_m, Cumulative_s);
			}

			if(IDLE_FUEL_TripFlagGetAndReset())
			{	
				s_idle_fuel_trip_reset_flag = 1;
				cycle_cntidle = 0;
				IDLE_FUEL_TripClear();
				IDLE_FUEL_TripWrite(FuelLtripValue, Ltrip_h, Ltrip_m, Ltrip_s);
			}
		#if 0
			if(s_idle_fuel_cumulative_reset_flag == 1)
			{
				if(DEA_GetSW_OKBottonValue() != 2)	//ok button not long press
				{
					s_idle_fuel_cumulative_reset_flag = 0;
				}
			}

			if(s_idle_fuel_trip_reset_flag == 1)
			{
				if(DEA_GetSW_OKBottonValue() != 2) //ok button not long press
				{
					s_idle_fuel_trip_reset_flag = 0;
				}
			}
		#else
			if(s_idle_fuel_cumulative_reset_flag == 1) //yangxl
			{
				if(DEA_GetKeyStrgOkValue() != 2)	//ok button not long press
				{
					s_idle_fuel_cumulative_reset_flag = 0;
				}
			}

			if(s_idle_fuel_trip_reset_flag == 1)
			{
				if(DEA_GetKeyStrgOkValue() != 2) //ok button not long press
				{
					s_idle_fuel_trip_reset_flag = 0;
				}
			}
		#endif
			if(idlestatus)
			{
				if((s_idle_fuel_cumulative_reset_flag == 0) && IDLE_FUEL_CumulativeOnOffFlagGet())
				{		
					FuelCumulativeValue_cnt++;
					CumulativeValue_cnt++;	//count action 100ms cycle	
					cycle_cntidleSum++;
				}
				if((s_idle_fuel_trip_reset_flag == 0) && IDLE_FUEL_TripOnOffFlagGet())	
				{
					FuelTripValue_cnt++;
					TripValue_cnt++;
					cycle_cntidle++;
				}
			}


			if((IDLE_FUEL_CumulativeOnOffFlagGet()) && (cycle_cntidleSum >= 10))	//cycle 1s
			{
				cycle_cntidleSum = 0;
				
				//CumulativeValue
				if(CumulativeValue_cnt < 600)	//< 1min
				{
					Cumulative_s = CumulativeValue_cnt/10;
					Cumulative_m = 0;
					Cumulative_h = 0;
				}
				else if(CumulativeValue_cnt < 36000)	//< 1hour
				{
					Cumulative_m = CumulativeValue_cnt/600;
					Cumulative_s = (CumulativeValue_cnt%600)/10;
					Cumulative_h = 0;
				}
				else	//>= 1hour
				{
					Cumulative_h = CumulativeValue_cnt/36000;
					Cumulative_s = ((CumulativeValue_cnt%36000)%600)/10;
					Cumulative_m = (CumulativeValue_cnt%36000)/600;
				}

				if(Cumulative_h >= 100)
				{
					CumulativeValue_cnt =0;
					Cumulative_h = 0;
					Cumulative_m = 0;
					Cumulative_s = 0;
				}

				//fuelCumulativeValue
				FuelCumulativeValue = (FuelCumulativeValue_cnt * PARAMETER_ISFUEL)/1000;	//change FuelCumulativeValue_cnt to 1s
				
				if(FuelCumulativeValue >= IDLEFUEL_MAX)
				{
					FuelCumulativeValue = IDLEFUEL_MAX;
				}

				IDLE_FUEL_CumulativeWrite(FuelCumulativeValue, Cumulative_h, Cumulative_m, Cumulative_s);
			}

			if((IDLE_FUEL_TripOnOffFlagGet()) && (cycle_cntidle >= 10))	//cycle 1s
			{
				cycle_cntidle = 0;
				//LtripValue
				if(TripValue_cnt < 600)	//< 1min
				{
					Ltrip_s = TripValue_cnt/10;
					Ltrip_m = 0;
					Ltrip_h = 0;
				}
				else if(TripValue_cnt < 36000)	//< 1hour
				{
					Ltrip_m = TripValue_cnt/600;
					Ltrip_s = (TripValue_cnt%600)/10;
					Ltrip_h = 0;
				}
				else	//>= 1hour
				{
					Ltrip_h = TripValue_cnt/36000;
					Ltrip_s = ((TripValue_cnt%36000)%600)/10;
					Ltrip_m = (TripValue_cnt%36000)/600;
				}

				//limit value handle
				if(Ltrip_h >= 100)
				{
					TripValue_cnt =0;
					Ltrip_h = 0;
					Ltrip_m = 0;
					Ltrip_s = 0;
				}

				
				//fuelLtripValue
				FuelLtripValue = (FuelTripValue_cnt * PARAMETER_ISFUEL)/1000;	//change FuelTripValue_cnt to 1s

				if(FuelLtripValue >= IDLEFUEL_MAX)
				{
					FuelLtripValue = IDLEFUEL_MAX;
				}

				IDLE_FUEL_TripWrite(FuelLtripValue, Ltrip_h, Ltrip_m, Ltrip_s);
			}
		}
		else
		{
			if(IGN_state != IGN_OFF)
			{
				IGN_state = IGN_OFF;
				cycle_cntidle = 0;
			}
		}
	}
}

#if 0 //IDLE_STOP_STATE_MACHINE
	switch(IDLETIME_state)
	{
		case OFF:
				
				if(IDLETIME_state_last == COUNT_OFF)
				{
					TripValue_cnt = 0;
					FuelTripValue_cnt = 0;
				}


				
				if(IGN_state == IGN_ON)
				{
					IDLETIME_state = COUNT_OFF;
				}

				IDLETIME_state_last = OFF;
			
			break;
			
		case COUNT_ON:

				CumulativeValue_cnt++;	//count action
				TripValue_cnt++;

				FuelCumulativeValue_cnt++;
				FuelTripValue_cnt++;
				
				if(IGN_state == IGN_OFF)
				{
					IDLETIME_state = OFF;
				}
				else if(/**1 == DEA_GetRestConfirm()**/0)	//the rest button press confirm
				{	
					IDLETIME_state = COUNTVALUE_RESET;
				}
				else if(0 == idlestatus)
				{	
					IDLETIME_state = COUNT_OFF;
				}

				IDLETIME_state_last = COUNT_ON;
			break;
			
		case COUNT_OFF:

				
				if(IDLETIME_state_last == OFF)
				{
					TripValue_cnt = 0;
					FuelTripValue_cnt = 0;
				}


				
				if(IGN_state == IGN_OFF)
				{
					IDLETIME_state = OFF;
				}
				else if(/**1 == DEA_GetRestConfirm()**/0)	//the rest button press confirm
				{	
					IDLETIME_state = COUNTVALUE_RESET;
				}
				else if(1 == idlestatus)
				{	
					IDLETIME_state = COUNT_ON;
				}

				IDLETIME_state_last = COUNT_OFF;
			break;
			
		case COUNTVALUE_RESET:

				CumulativeValue_cnt = 0;	//count clear
				TripValue_cnt = 0;

				FuelCumulativeValue_cnt = 0;
				FuelTripValue_cnt= 0;
				
				if(IGN_state == IGN_OFF)
				{
					IDLETIME_state = OFF;
				}
				else if((1 == idlestatus) && (/**0 == DEA_GetRestConfirm()**/0))
				{	
					IDLETIME_state = COUNT_ON;
				}
				else if((0 == idlestatus) && (/**1 == DEA_GetRestConfirm()**/0))
				{	
					IDLETIME_state = COUNT_OFF;
				}

				IDLETIME_state_last = COUNTVALUE_RESET;
			break;

	}
#endif









