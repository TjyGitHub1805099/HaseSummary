/********************************************************************
File name: System_Signal.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-18 09:54:28
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#include "System_Signal.h"

typedef struct
{
	void (*Sys_Sig_Init)(uint16 offset);
	boolean (*Sys_Sig_Subscribe)(uint16 signalId, uint16 taskId);
	boolean (*Sys_Sig_SubSpecValue)(uint16 signalId, uint16 taskId, uint32 specValue);
	boolean (*Sys_Sig_GetStatus)(uint16 signalId, uint32* status);
	boolean (*Sys_Sig_SetStatus)(uint16 signalId, uint32 status, boolean fromISR);
} SystemSignalConfigType;

SystemSignalConfigType SystemSignalConfig[SYSTEM_SIG_MODULEID_MAX] =
{
	/*===========================================================================*/
	/*===================== START: USER SIGNAL REGISTER =========================*/
	/*===========================================================================*/
	{Alarm_Init, Alarm_Subscribe, NULL, Alarm_GetStatus, NULL},
	{VirtualSig_Init, VirtualSig_Subscribe, VirtualSig_SubSpecValue, VirtualSig_GetStatus, VirtualSig_SetStatus},

	/*===========================================================================*/
	/*====================== END: USER SIGNAL REGISTER ==========================*/
	/*===========================================================================*/
};

void System_Signal_Init(void)
{
	uint16 l_signalId = 0;

	for (l_signalId = 0; l_signalId < SYSTEM_SIG_MODULEID_MAX; l_signalId++)
	{
		if (NULL != SystemSignalConfig[l_signalId].Sys_Sig_Init)
		{
			(*SystemSignalConfig[l_signalId].Sys_Sig_Init)(l_signalId << SYSTEM_SIG_ID_OFFSET_BIT);
		}
	}
}

boolean System_Signal_Subscribe(uint16 signalId, uint16 taskId)
{
	boolean l_returnValue = E_NOT_OK;

	if ((signalId >> SYSTEM_SIG_ID_OFFSET_BIT) < SYSTEM_SIG_MODULEID_MAX)
	{
		if (NULL != SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_Subscribe)
		{
			l_returnValue = (*SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_Subscribe)(signalId & ((1 << SYSTEM_SIG_ID_OFFSET_BIT) - 1), taskId);
		}
	}
	return (l_returnValue);
}

boolean System_Signal_SubSpecValue(uint16 signalId, uint16 taskId, uint32 specValue)
{
	boolean l_returnValue = E_NOT_OK;

	if ((signalId >> SYSTEM_SIG_ID_OFFSET_BIT) < SYSTEM_SIG_MODULEID_MAX)
	{
		if (NULL != SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_SubSpecValue)
		{
			l_returnValue = (*SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_SubSpecValue)(signalId & ((1 << SYSTEM_SIG_ID_OFFSET_BIT) - 1), taskId, specValue);
		}
	}
	return (l_returnValue);
}

boolean System_Signal_Receive(uint16 signalId, uint32* value)
{
	boolean l_returnValue = E_NOT_OK;

	if ((signalId >> SYSTEM_SIG_ID_OFFSET_BIT) < SYSTEM_SIG_MODULEID_MAX)
	{
		if (NULL != SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_GetStatus)
		{
			l_returnValue = (*SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_GetStatus)(signalId & ((1 << SYSTEM_SIG_ID_OFFSET_BIT) - 1),value);
		}
	}
	return (l_returnValue);
}

boolean System_Signal_Send(uint16 signalId, uint32 value , boolean fromISR)
{
	boolean l_returnValue = E_NOT_OK;

	if ((signalId >> SYSTEM_SIG_ID_OFFSET_BIT) < SYSTEM_SIG_MODULEID_MAX)
	{
		if (NULL != SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_SetStatus)
		{
			l_returnValue = (*SystemSignalConfig[signalId >> SYSTEM_SIG_ID_OFFSET_BIT].Sys_Sig_SetStatus)(signalId & ((1 << SYSTEM_SIG_ID_OFFSET_BIT) - 1), value, fromISR);
		}
	}
	return (l_returnValue);
}
/*=========================== END OF FILE: System_Signal.c ===========================*/

