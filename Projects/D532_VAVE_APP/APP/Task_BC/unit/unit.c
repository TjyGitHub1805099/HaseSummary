#include "unit.h"
#include "dea.h"
#include "NvM.h"
#include "bc_common.h"
#include "MemMap.h"
#include "NvM_if.h"



//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED UNIT_STRU g_Unit_Coding = {0};
//#pragma segment BACK_RAM_END

void UNIT_Init(void)
{
	if(g_Unit_Coding.MagicByte != MAGIC_BYTE)
	{
		g_Unit_Coding.MagicByte = MAGIC_BYTE;
		g_Unit_Coding.LanguageFormat = 0;
		g_Unit_Coding.FuelConsumptionUnit = 0;
		g_Unit_Coding.TimeFormat = 0;
		g_Unit_Coding.DriveTimeFlag = 0;
		g_Unit_Coding.DriveTimeThreshold = 30;	//30 minutes
		NvM_WriteBlock(UNIT_NVRAM_INDEX, (uint8_t*)&g_Unit_Coding);
	}

	if(g_Unit_Coding.LanguageFormat == 0)
	{
		DEA_SetLanguage(LANGUAGE_CHINESE);
	}
	else
	{
		DEA_SetLanguage(LANGUAGE_ENGLISH);
	}

	if(g_Unit_Coding.TimeFormat == 0) //设置一天几小时制
	{	
		DEA_SetTimeFormat(TIME_24H);
	}
	else
	{
		DEA_SetTimeFormat(TIME_12H);
	}
}

uint8_t UNIT_GetDriveTimeFlag(void)
{
	return g_Unit_Coding.DriveTimeFlag;
}

uint8_t UNIT_GetDriveTimeThreshold(void)
{
	return g_Unit_Coding.DriveTimeThreshold;
}


void UNIT_vMain(void)
{
	uint8_t l_writeFlag = 0;
	if(DEA_GetMHULanguage() != DEA_GetLanguage())
	{
		DEA_SetLanguage(DEA_GetMHULanguage());
		l_writeFlag = 1;
	}

	if(DEA_GetMHUTimeFormat() != DEA_GetTimeFormat())
	{
		DEA_SetTimeFormat(DEA_GetMHUTimeFormat());
		l_writeFlag = 1;
	}

	if(l_writeFlag)
	{
		DEA_SetUnitChangedFlagForCan(1);
		NvM_WriteBlock(UNIT_NVRAM_INDEX, (uint8_t*)&g_Unit_Coding);
	}
}
