#include "VEL_LT.h"
#include "VEL_LT_cfg.h"
#include "MemMap.h"
#include "NvM_if.h"

//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED VEL_LT_CFG_STRU g_VEL_LT_Config = {0};
//#pragma segment BACK_RAM_END



uint8_t VEL_LT_GetOnOff(void)
{
	return g_VEL_LT_Config.VEL_LT_OnOff;
}

uint16_t VEL_LT_GetConfigValue(void)
{
	return g_VEL_LT_Config.VEL_LT_ConfigValue;
}


void VEL_LT_CFG_vInit(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		if(g_VEL_LT_Config.MagicByte != MAGIC_BYTE)
		{
			g_VEL_LT_Config.MagicByte = MAGIC_BYTE;
			g_VEL_LT_Config.VEL_LT_OnOff = VEL_LT_OFF;
			g_VEL_LT_Config.VEL_LT_ConfigValue = VEL_LT_VALUE_DEFAULT;
			NvM_WriteBlock(VEL_LT_NVRAM_INDEX, (uint8_t *)&g_VEL_LT_Config);
		}
	}
}

uint8_t VEL_LT_SetValue(uint16_t vel_config)
{
	Std_ReturnType res;

	if((vel_config) >= (VEL_LT_VALUE_MIN))
	{
		g_VEL_LT_Config.VEL_LT_OnOff = VEL_LT_ON;
		g_VEL_LT_Config.VEL_LT_ConfigValue = (vel_config);
	}
	else
	{
		g_VEL_LT_Config.VEL_LT_OnOff = VEL_LT_OFF;
		g_VEL_LT_Config.VEL_LT_ConfigValue = VEL_LT_VALUE_DEFAULT;	
	}
	
	g_VEL_LT_Config.MagicByte = MAGIC_BYTE;
	res = NvM_WriteBlock(VEL_LT_NVRAM_INDEX, (uint8_t *)&g_VEL_LT_Config);
	return res;
}

void VEL_LT_Write(uint8_t mode)
{
	DEA_SetSpeedLimitAlarm(mode);
}


