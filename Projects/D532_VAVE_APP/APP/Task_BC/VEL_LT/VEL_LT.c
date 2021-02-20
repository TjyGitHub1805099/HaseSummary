#include "VEL_LT.h"
#include "VEL_LT_cfg.h"

uint8_t VEL_LT_Mode = 0;


void VEL_LT_vInit(void)
{
	VEL_LT_CFG_vInit();
	DEA_SetSpeedLimitValue(VEL_LT_GetConfigValue());
}

void VEL_LT_vMain(void)
{
	static uint8_t s_cnt = 0;
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(VEL_LT_GetOnOff() == VEL_LT_ON)
		{
			if(DEA_GetFilterVehicleSpeed() >= VEL_LT_GetConfigValue())
			{
				if(VEL_LT_Mode == 0)
				{
					if(s_cnt < 10)
					{
						s_cnt++;
					}
					else
					{
						VEL_LT_Mode = 1;
					}
				}
				else
				{
					s_cnt = 0;
				}
			}
			else if(DEA_GetFilterVehicleSpeed() < (VEL_LT_GetConfigValue()-VEL_LT_HYSTERESIS))
			{
				if(VEL_LT_Mode == 1)
				{
					if(s_cnt < 10)
					{
						s_cnt++;
					}
					else
					{
						VEL_LT_Mode = 0;
					}
				}
				else
				{
					s_cnt = 0;
				}
			}
		}
		else
		{
			VEL_LT_Mode = 0;
		}
	}
	else
	{
		VEL_LT_Mode = 0;
	}

	VEL_LT_Write(VEL_LT_Mode);
}
