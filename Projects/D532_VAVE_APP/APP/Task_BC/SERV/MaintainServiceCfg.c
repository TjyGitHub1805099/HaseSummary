#include "MaintainServiceCfg.h"
#include "dea.h"
#include "Ea.h"
#include "bc_common.h"
#include "NvM.h"
#include "MemMap.h"
#include "NvM_if.h"
#include "MaintainService.h"

//#pragma segment BACK_RAM_START
NVM_SEC_VAR_UNSPECIFIED SERV_CODING_STRU g_SERV_Coding = {0};
//#pragma segment BACK_RAM_END


uint8_t InitServCfg(void)
{	
	if(DEA_GetSysRstType() == RST_IRRST)
	{	
		if(g_SERV_Coding.u8ValidFlag != MAGIC_BYTE)
		{
			g_SERV_Coding.u8ValidFlag = MAGIC_BYTE;
			g_SERV_Coding.AlarmFlag = SERV_ENABLE;
			g_SERV_Coding.u16ServValue = SERV_DEFAULT_INTERVAL;
			g_SERV_Coding.u32OdoValueLastReset = DEA_GetOdo();
			NvM_WriteBlock(SERV_BACK_RAM_INDEX, (uint8_t *)&g_SERV_Coding);
		}
	}
	return 0;
}

SERV_RESET_RET_ENUM ServReset(uint16_t ServiceValue)
{
	g_SERV_Coding.u8ValidFlag = MAGIC_BYTE;
	g_SERV_Coding.u16ServValue = ServiceValue;
	g_SERV_Coding.u32OdoValueLastReset = DEA_GetOdo();
	if(NvM_WriteBlock(SERV_BACK_RAM_INDEX, (uint8_t *)&g_SERV_Coding) != E_OK)
	{
		return SERV_RESET_NVM_ERROR;// 0
	}
	else
	{
		SERV_SetAccomplishFlag(1);
		return SERV_RESET_OK;// 1
	}
}


SERV_RESET_RET_ENUM ServResetExt(void)
{
	g_SERV_Coding.u8ValidFlag = MAGIC_BYTE;
	g_SERV_Coding.u32OdoValueLastReset = DEA_GetOdo();
	if(NvM_WriteBlock(SERV_BACK_RAM_INDEX, (uint8_t *)&g_SERV_Coding) != E_OK)
	{
		return SERV_RESET_NVM_ERROR;
	}
	else
	{
		return SERV_RESET_OK;
	}
}

uint8_t SERV_GetAlarmFlag(void)
{
	return g_SERV_Coding.AlarmFlag;
}


void ServAlarmFunctionFlagSet(uint8_t stat)
{
	g_SERV_Coding.u8ValidFlag = MAGIC_BYTE;
	g_SERV_Coding.AlarmFlag = stat;
	NvM_WriteBlock(SERV_BACK_RAM_INDEX, (uint8_t *)&g_SERV_Coding);
}


