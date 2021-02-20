#include "Eeprom.h"
#include "Swdl.h"
#include "Std_Types.h"

#ifdef PRJ_APPLICATION
SWDL_START_SEC_VAR_FAST_POWER_ON_NOINIT_UNSPECIFIED
uint8 AppUpdateReqFlag[2] = {0x5a,0x80};
SWDL_STOP_SEC_VAR_FAST_POWER_ON_NOINIT_UNSPECIFIED
#endif

Std_ReturnType Swdl_SetUpdateFlag(uint8 type)
{
	*(uint8*)SWDL_FLAG_START_ADDR = 0xa5;
	*(uint8*)(SWDL_FLAG_START_ADDR+1) = type | SWDL_TARGET_SOC | SWDL_TARGET_IOC;
	return (E_OK);
}

Std_ReturnType Swdl_ClearUpdateFlag(void)
{
	*(uint8*)SWDL_FLAG_START_ADDR = 0;
	*(uint8*)(SWDL_FLAG_START_ADDR+1) = 0;
	return (E_OK);
}

Std_ReturnType Swdl_GetUpdateFlag(uint8 *pFlag)
{
	if (0xa5 == *(uint8*)SWDL_FLAG_START_ADDR)
	{
		*pFlag = *(uint8*)(SWDL_FLAG_START_ADDR+1);
	}
	else
	{
		*pFlag = 0;
	}
	return (E_OK);
}

Std_ReturnType Swdl_IsAppValid(void)
{
	Std_ReturnType l_retrun = E_OK;
	uint8* l_appValidEndAddr = 0;
	uint8 *lp_appValidStartAddr = (uint8*)(SWDL_APP_VALID_FLAG_START_ADDR+SWDL_APP_VALID_FLAG_SIZE);
	uint8 l_loop = 0;

	l_appValidEndAddr = (uint8*) (* (uint32*)SWDL_APP_VALID_FLAG_START_ADDR);
	if((SWDL_APP_START_ADDR > * (uint32*)SWDL_APP_VALID_FLAG_START_ADDR)||(* (uint32*)SWDL_APP_VALID_FLAG_START_ADDR > SWDL_CHIP_MAX_ADDR))
	{
		l_retrun = E_NOT_OK;
	}
	else
	{
		for (l_loop = 0; l_loop < 8; l_loop++)
		{
			if (*(lp_appValidStartAddr + l_loop) == *(l_appValidEndAddr + l_loop))
			{
				switch (l_loop)
				{
				case 0:
					if (*(lp_appValidStartAddr + l_loop) != 0x5a)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 1:
					if (*(lp_appValidStartAddr + l_loop) != 0xa1)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 2:
					if (*(lp_appValidStartAddr + l_loop) != 0xca)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 3:
					if (*(lp_appValidStartAddr + l_loop) != 0xa5)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 4:
					if (*(lp_appValidStartAddr + l_loop) != 0xca)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 5:
					if (*(lp_appValidStartAddr + l_loop) != 0xe7)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 6:
					if (*(lp_appValidStartAddr + l_loop) != 0x6f)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				case 7:
					if (*(lp_appValidStartAddr + l_loop) != 0x2e)
					{
						l_retrun = E_NOT_OK;
					}
					break;
				default:
					l_retrun = E_NOT_OK;
					break;
				}
				if (E_NOT_OK == l_retrun)
				{
					break;
				}
			}
			else
			{
				l_retrun = E_NOT_OK;
				break;
			}
		}
	}
	return (l_retrun);
}


