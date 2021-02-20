#include "dea.h"
#include "eco.h"

#include "Std_Types.h"


CONDITION_MODE ECO_mode;

ECO_Infor ECO_data;

uint8_t condition_rank;	//send to HMI for ECO action display

uint16_t TimeSum_T3, TimeSum_T4;	//the condition time count
static uint8_t s_bat_on_flag = 0;
static uint8_t s_RawEcoMode = 0;

void EcoProgressClear(void)
{
	memset(&ECO_data,0,sizeof(ECO_data));
	condition_rank = 0;
	TimeSum_T3 = 0;
	TimeSum_T4 = 0;
}

EcoWireData(uint8_t value,uint8_t valid)
{
	DEA_SetFilterEcoRankValue(value);
	DEA_SetFilterEcoRankValid(valid);

}


void ECO_Init(void)
{

	if(DEA_GetSysRstType() == RST_IRRST)
	{	
		EcoProgressClear();
		s_bat_on_flag = 1;
		s_RawEcoMode = 0;
		EcoWireData(ECO_data.ECO_rank,ECO_data.ECOrankValid);

	}
}


CONDITION_MODE JudgeEcoMode(void)
{
	CONDITION_MODE MODE;
	if(DEA_GetFilterVehicleSpeedValid())	//signal valid
	{
		if((DEA_GetFilterVehicleSpeed() >= PARMETER_V_3)&&(DEA_GetFilterVehicleSpeed() <= PARMETER_V_5))
		{
			if((DEA_GetFilterAccelGuide_Y() * PARMETER_X1) < (30 - DEA_GetFilterAccelGuideValue()))	//<(30 - z)
			{
				MODE = Condition_one;
				condition_rank = 1;
			}
			else if((DEA_GetFilterAccelGuide_Y() <= (30 - DEA_GetFilterAccelGuideValue())) && ((30 - DEA_GetFilterAccelGuideValue()) <= DEA_GetFilterAccelGuide_Y() * PARMETER_X1))
			{
				MODE = Condition_two;
				condition_rank = 2;
			}
			else if((30 - DEA_GetFilterAccelGuideValue()) < DEA_GetFilterAccelGuide_Y())
			{
				MODE = Condition_three;
				condition_rank = 3;
			}
			else
			{
				MODE = Invalid_value;
				condition_rank = 0;
			}
		}
		else if(DEA_GetFilterVehicleSpeed() > PARMETER_V_5)		//the speed has expand 10times
		{
			if((THRESHOLD_B * PARMETER_X1) < (30 - DEA_GetFilterAccelGuideValue()))
			{
				MODE = Condition_one;
				condition_rank = 1;
			}
			else if((THRESHOLD_B <= (30 - DEA_GetFilterAccelGuideValue())) && ((30 - DEA_GetFilterAccelGuideValue()) <= THRESHOLD_B * PARMETER_X1))
			{
				MODE = Condition_two;
				condition_rank = 2;
			}
			else if((30 - DEA_GetFilterAccelGuideValue()) < THRESHOLD_B)
			{
				MODE = Condition_three;
				condition_rank = 3;
			}
			else
			{
				MODE = Invalid_value;
				condition_rank = 0;
			}
		}
		else if((DEA_GetFilterVehicleSpeed() < PARMETER_V_3) || (DEA_GetRawEcoShiftPosition() == 1) || (DEA_GetRawEcoShiftPosition() == 2) || (DEA_GetRawEcoShiftPosition() == 3))	//the speed has expand 10times
		{
			
			MODE = Condition_four;
			condition_rank = 4;
			
		}
		else
		{
			MODE = Invalid_value;
			condition_rank = 0;
		}
	}

	DEA_SetFilterEcoConddition(condition_rank);
	
	return MODE;
}



void ECO_VMAIN()
{
	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;
	static uint8_t s_RawEcoModeBack = 0;
	static uint16_t s_RawEcoModeCnt = 0;
	
	CONDITION_MODE	ECO_modeinfor;

	uint8_t eco_rate = 0;

	if(s_RawEcoMode != DEA_GetRawEcoMode())
	{
		if(s_RawEcoModeCnt < 10)
		{
			s_RawEcoModeCnt++;
		}
		else
		{
			s_RawEcoModeCnt = 0;
			s_RawEcoMode = DEA_GetRawEcoMode();
		}
	}
	else
	{
		s_RawEcoModeCnt = 0;
	}

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnBak == IGN_OFF)
		{
			s_IgnBak = IGN_ON;
			s_RawEcoModeCnt = 0;
			if(s_bat_on_flag == 1)
			{
				s_RawEcoMode = 0;
			}
			else
			{
				s_RawEcoMode = DEA_GetRawEcoMode();
			}
			EcoProgressClear();
			EcoWireData(ECO_data.ECO_rank,ECO_data.ECOrankValid);
		}

		ECO_modeinfor = JudgeEcoMode();	//get the ECO mode 
		
		switch(ECO_modeinfor)	//judge the Accguide  level
		{
			case Condition_one:
					if(s_RawEcoMode == 1)
					{
						if(TimeSum_T3 < Timecnt_Max)
							TimeSum_T3++;
					}
					
				break;
			case Condition_two:
					if(s_RawEcoMode == 1)
					{
						if(TimeSum_T3 < Timecnt_Max)
							TimeSum_T3++;
					}
				break;
			case Condition_three:
					if(s_RawEcoMode == 1)
					{
						if(TimeSum_T4 < Timecnt_Max)
							TimeSum_T4++;
					}
				break;
				
			default:
					
				break;
		}

		/*if((TimeSum_T3 >= Timecnt_Max) || (TimeSum_T4 >= Timecnt_Max))	//over flow 
		{
			TimeSum_T3 = TimeSum_T3/2;	//half processing
			TimeSum_T4 = TimeSum_T4/2;
		}	*/	
	}
	else
	{
		
		if(s_IgnBak == IGN_ON)
		{
			s_IgnBak = IGN_OFF;
			if((s_RawEcoMode == 1) && ((TimeSum_T3+TimeSum_T4) >= PARMETER_T1))
			{
				if((TimeSum_T3 > 0) || (TimeSum_T4 > 0))	//make sure the caculation is invalid
				{
					eco_rate = (TimeSum_T3 * 100)/(TimeSum_T3 + TimeSum_T4);	//ECO rate x%
				}
				else
				{
					eco_rate = 0;
				}

				if((eco_rate >= 0) && (eco_rate < 60))
				{
					ECO_data.ECO_rank = 1;
					ECO_data.ECOrankValid = 1;
				}
				else if((eco_rate >= 60) && (eco_rate < 75))
				{		
					ECO_data.ECO_rank = 2;
					ECO_data.ECOrankValid = 1;
				}
				else if((eco_rate >= 75) && (eco_rate < 85))
				{
					ECO_data.ECO_rank = 3;
					ECO_data.ECOrankValid = 1;
				}
				else if((eco_rate >= 85) && (eco_rate < 95))
				{
					ECO_data.ECO_rank = 4;
					ECO_data.ECOrankValid = 1;
				}
				else if((eco_rate >= 95) && (eco_rate <= 100))
				{
					ECO_data.ECO_rank = 5;
					ECO_data.ECOrankValid = 1;
				}
				else 
				{
					//do nothing
				}

				EcoWireData(ECO_data.ECO_rank, ECO_data.ECOrankValid);
				DEA_SetFilterEcoConddition(condition_rank);
			}
		}
	}

	
}










