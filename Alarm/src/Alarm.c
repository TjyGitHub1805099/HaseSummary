/********************************************************************
File name: Alarm.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-15 09:54:28
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
#include "Alarm.h"
#include "System_Task.h"
#include "System_Portable.h"
#if (STD_ON == ALARM_DEBUG_TRACE)
#include "trace.h"
#endif

extern Alarm_RuntimeInfoType Alarm_RuntimeInfo[];
extern Alarm_ConfigInfoType Alarm_ConfigInfo[];
extern uint16* Alarm_RegTaskList[];
extern const uint16 Alarm_MaxId;

#define ALARM_MODULE_UNINIT (0U)
#define ALARM_MODULE_INIT (1U)

typedef struct
{
	boolean init;
} Alarm_ModuleManageType;

Alarm_ModuleManageType Alarm_ModuleManage = {ALARM_MODULE_UNINIT};

void Alarm_Init(uint16 moduleOffset)
{
	uint16 l_alarmCounter = 0;

	moduleOffset = moduleOffset;
	if (ALARM_MODULE_UNINIT == Alarm_ModuleManage.init)
	{
		for (; l_alarmCounter < Alarm_MaxId; l_alarmCounter++)
		{
			if (STD_ON == Alarm_ConfigInfo[l_alarmCounter].start)
			{
				Alarm_RuntimeInfo[l_alarmCounter].state = ALARM_STATE_RUNNING;
			}
			else
			{
				Alarm_RuntimeInfo[l_alarmCounter].state = ALARM_STATE_STOPPED;
			}
			Alarm_RuntimeInfo[l_alarmCounter].counter = Alarm_ConfigInfo[l_alarmCounter].time;
			Alarm_RuntimeInfo[l_alarmCounter].regCount = 0;
		}
		Alarm_ModuleManage.init = ALARM_MODULE_INIT;
	}
}

void Alarm_Main(uint32 count)
{
	uint8 l_alarmCounter = 0;
	uint8 l_regCounter = 0;
	SystemTaskMsgInfoType l_message;

	if(ALARM_MODULE_INIT == Alarm_ModuleManage.init)
	{
		for (; l_alarmCounter < Alarm_MaxId; l_alarmCounter++)
		{
			if ((0 != Alarm_RuntimeInfo[l_alarmCounter].regCount) && (ALARM_STATE_RUNNING == Alarm_RuntimeInfo[l_alarmCounter].state))
			{
				if (count <= Alarm_RuntimeInfo[l_alarmCounter].counter)
				{
					Alarm_RuntimeInfo[l_alarmCounter].counter -= count;
				}
				else
				{
					Alarm_RuntimeInfo[l_alarmCounter].counter = 0;
				}
				
				if (0 == Alarm_RuntimeInfo[l_alarmCounter].counter)
				{
					Alarm_RuntimeInfo[l_alarmCounter].counter = Alarm_ConfigInfo[l_alarmCounter].time;
					if (0 != Alarm_RuntimeInfo[l_alarmCounter].regCount)
					{
						l_message.msgId = l_alarmCounter;
						l_message.size = 0;
						for (l_regCounter = 0; l_regCounter < Alarm_RuntimeInfo[l_alarmCounter].regCount; l_regCounter++)
						{        
							System_Task_SendMessage(Alarm_RegTaskList[l_alarmCounter][l_regCounter], &l_message, FALSE);
						}
					}
					if (ALARM_TYPE_SINGLE == Alarm_ConfigInfo[l_alarmCounter].type)
					{
						Alarm_RuntimeInfo[l_alarmCounter].state = ALARM_STATE_OVERFLOW;
					}
				}
			}
		}
	}
}

boolean Alarm_Start(uint16 alarmId)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;

	if ((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
		Alarm_RuntimeInfo[l_alarmId].state = ALARM_STATE_RUNNING;
		l_returnValue = E_OK;
#if (STD_ON == ALARM_DEBUG_TRACE)
		TracePrintf(ALARM_TRACE,TRACE_INFO,"[Alarm-I]>StartOK. AlarmId:%d\r\n",l_alarmId);
#endif
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif

	return (l_returnValue);
}

boolean Alarm_Stop(uint16 alarmId)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;

	if ((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
		EnterCriticalCode();
		Alarm_RuntimeInfo[l_alarmId].counter = Alarm_ConfigInfo[l_alarmId].time;
		Alarm_RuntimeInfo[l_alarmId].state = ALARM_STATE_STOPPED;
		ExitCriticalCode();
		l_returnValue = E_OK;
#if (STD_ON == ALARM_DEBUG_TRACE)
		TracePrintf(ALARM_TRACE,TRACE_INFO,"[Alarm-I]>StopOK. AlarmId:%d\r\n",l_alarmId);
#endif
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif
	return (l_returnValue);
}

boolean Alarm_Pause(uint16 alarmId)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;

	if ((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
		Alarm_RuntimeInfo[l_alarmId].state = ALARM_STATE_PAUSE;
		l_returnValue = E_OK;
#if (STD_ON == ALARM_DEBUG_TRACE)
		TracePrintf(ALARM_TRACE,TRACE_INFO,"[Alarm-I]>PauseOK. AlarmId:%d\r\n",l_alarmId);
#endif
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif
	return (l_returnValue);
}

boolean Alarm_Resume(uint16 alarmId)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;

	if ((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
		Alarm_RuntimeInfo[l_alarmId].state = ALARM_STATE_RUNNING;
		l_returnValue = E_OK;
#if (STD_ON == ALARM_DEBUG_TRACE)
		TracePrintf(ALARM_TRACE,TRACE_INFO,"[Alarm-I]>ResumeOK. AlarmId:%d\r\n",l_alarmId);
#endif
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif
	return (l_returnValue);
}

boolean Alarm_Subscribe(uint16 alarmId, uint16 taskId)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;
	uint16 l_taskId = taskId;

	if ((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
        if(Alarm_RuntimeInfo[l_alarmId].regCount < Alarm_ConfigInfo[l_alarmId].regMaxNum)
        {
			EnterCriticalCode();
			Alarm_RegTaskList[l_alarmId][Alarm_RuntimeInfo[l_alarmId].regCount] = l_taskId;
    		Alarm_RuntimeInfo[l_alarmId].regCount++;
			ExitCriticalCode();
    		l_returnValue = E_OK;
#if (STD_ON == ALARM_DEBUG_TRACE)
			TracePrintf(ALARM_TRACE,TRACE_INFO,"[Alarm-I]>RegOK. AlarmId:%d,TskId:%d\r\n",l_alarmId,l_taskId);
#endif
		}
#if (STD_ON == ALARM_DEBUG_TRACE)
		else
		{
			TracePrintf(ALARM_TRACE,TRACE_WARNING,"[Alarm-W]>ReReg. AlarmId:%d\r\n",l_alarmId);
		}
#endif
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif
	return (l_returnValue);
}

boolean Alarm_GetStatus(uint16 alarmId, uint32* status)
{
	boolean l_returnValue = E_NOT_OK;
	uint16 l_alarmId = alarmId;

	if((ALARM_MODULE_INIT == Alarm_ModuleManage.init)&&(l_alarmId < Alarm_MaxId))
	{
   	 	*status = Alarm_RuntimeInfo[l_alarmId].state;
		l_returnValue = E_OK;
	}
#if (STD_ON == ALARM_DEBUG_TRACE)
	else
	{
		TracePrintf(ALARM_TRACE,TRACE_ERRO,"[Alarm-E]>Para Err. AlarmId:%d,InitState:%d\r\n",l_alarmId,Alarm_ModuleManage.init);
	}
#endif
	return(l_returnValue);
}
/*=========================== END OF FILE: Alarm.c ===========================*/

