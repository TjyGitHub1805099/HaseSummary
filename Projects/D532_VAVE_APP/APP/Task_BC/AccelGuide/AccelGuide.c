#include "dea.h"
#include "AccelGuide.h"

#include "Std_Types.h"


/**data struct**/
AccelGuide_Infor Acceldata;


void AccelProgressClear(void)
{
	memset(&Acceldata,0,sizeof(Acceldata));
	
}

void AccelWireData(uint8_t temp,uint8_t temp1,uint8_t valid)
{
	DEA_SetFilterAccelGuideValue(temp);
	DEA_SetFilterAccelGuide_Y(temp1);
	DEA_SetFilterAccelGuideValValid(valid);
}

void AccelGuide_Init(void)
{

	if(DEA_GetSysRstType() == RST_IRRST)
	{	
		AccelProgressClear();
		
		AccelWireData(Acceldata.AccelValue,Acceldata.Parameter_Y,Acceldata.AccelValueValid);

	}
}



void AccelGuide_VMAIN()
{
	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;

	uint16_t speed = 0;
	uint8_t temp1 = 0;
	uint16_t temp2 = 0;


	uint8_t Parameter_X = 0;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnBak == IGN_OFF)
		{
			s_IgnBak = IGN_ON;
		}

		speed = DEA_GetFilterVehicleSpeed()/10;	//smaller 10times to return from display speed to true speed value
		temp1 = THRESHOLD_A - THRESHOLD_B;


		if(speed == 0)
		{	
			Acceldata.Parameter_Y = THRESHOLD_A;
		}
		else if(speed < PARMETER_V5)
		{	
			temp2 = (uint16_t)(THRESHOLD_A*N - (speed*N)/40)/30;	//Threshold value
			Acceldata.Parameter_Y = (uint8_t)temp2;
		}
		else
		{
			Acceldata.Parameter_Y = THRESHOLD_B;
		}

		if(DEA_GetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS) == 0)	//signal not timeout
		{
			Parameter_X = DEA_GetRawAccelGuide_X();

			if(Parameter_X == 30)
			{
				Acceldata.AccelValue = 0;
			}
			else if(Parameter_X < 30)
			{
				if(speed == 0)
				{	
					temp2 = (uint16_t)((30 - Parameter_X)*N)/30;
					Acceldata.AccelValue = (uint8_t)temp2;
				}
				else if(speed < PARMETER_V5)
				{	
					temp2 = (uint16_t)((30 - Parameter_X)*N + (temp1*speed*N)/PARMETER_V5)/30;
					Acceldata.AccelValue = (uint8_t)temp2;
					if(Acceldata.AccelValue > N)
					{
						Acceldata.AccelValue = N;	//set the limit
					}
				}
				else
				{
					temp2 = (uint16_t)((30 + temp1 - Parameter_X)*N)/30;
					Acceldata.AccelValue = (uint8_t)temp2;
					if(Acceldata.AccelValue > N)
					{
						Acceldata.AccelValue = N;	//set the limit
					}
					
				}
			}
			else if(Parameter_X == 31)
			{
				Acceldata.AccelValue = 30;
			}
			
			Acceldata.AccelValueValid = 1;

		}
		else
		{
			Parameter_X = 0xFF;	//invalid value
			Acceldata.AccelValueValid = 0;
		}

		AccelWireData(Acceldata.AccelValue,Acceldata.Parameter_Y,Acceldata.AccelValueValid);
	}
	else
	{
		if(s_IgnBak == IGN_ON)
		{
			s_IgnBak = IGN_OFF;
			AccelProgressClear();
			AccelWireData(Acceldata.AccelValue,Acceldata.Parameter_Y,Acceldata.AccelValueValid);
		}
	}
}








