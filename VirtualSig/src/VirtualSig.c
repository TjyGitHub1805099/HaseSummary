/********************************************************************
File name: VirtualSig.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-01-22 18:20:35
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
#include "VirtualSig.h"
#include "System_Task.h"
#include "System_Portable.h"
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
#include "trace.h"
#endif

extern const uint16 VirtualSig_MaxId;
extern VirtualSig_CfgInfoType VirtualSig_CfgInfo[];
extern VirtualSig_RuntimeInfoType VirtualSig_RuntimeInfo[];

#define VIRTUALSIG_MODULE_UNINIT (0U)
#define VIRTUALSIG_MODULE_INIT (1U)

#define VIRTUALSIG_REGTYPE_CHANGED (0U)
#define VIRTUALSIG_REGTYPE_SPECIFY (1U)

#define VIRTUALSIG_INITIALIZE 0XFFFFFFFF

typedef struct
{
	uint8 Init;
	uint16 Offset;
} VirtualSig_ModuleType;

VirtualSig_ModuleType VirtualSig_Module = {VIRTUALSIG_MODULE_UNINIT,0};

/*
*@par Service ID[hex]
*0X0
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] moduleOffset
*Module offset value signed by System Signal module.
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Call this function to initialize the module.
*You do not need to call this function after you registered to system signal module.
*/
void VirtualSig_Init(uint16 moduleOffset)
{
    uint16 l_regCount = 0;

    if (VIRTUALSIG_MODULE_UNINIT == VirtualSig_Module.Init)
    {
        VirtualSig_Module.Offset =  moduleOffset;
        for (l_regCount = 0; l_regCount < VirtualSig_MaxId; l_regCount++)
        {
			EnterCriticalCode();
			VirtualSig_RuntimeInfo[l_regCount].Status = VIRTUALSIG_INITIALIZE;
            VirtualSig_RuntimeInfo[l_regCount].RegCount = 0;
            VirtualSig_RuntimeInfo[l_regCount].RegTaskInfo = VirtualSig_CfgInfo[l_regCount].RegTaskInfoPtr;
			ExitCriticalCode();
		}
        VirtualSig_Module.Init = VIRTUALSIG_MODULE_INIT;
    }	
}

/*
*@par Service ID[hex]
*0X1
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] taskId
*Task identifier which want to receive this signal.
*
*@param[inout] None
*
*@param[out] None
*
*@retval E_OK: Subscribe successed.
*@retval E_NOT_OK: Subscribe failed due to max number limited or module not initialize.
*
*Call this function to subscribe if you want to receive this signal event in every change.
*/
boolean VirtualSig_Subscribe(uint16 signalId, uint16 taskId)
{
    boolean l_returnValue = E_NOT_OK;
    uint16 l_signalId = signalId;
    uint16 l_taskId = taskId;
    uint16 l_regCount = 0U;

    if ((VIRTUALSIG_MODULE_INIT == VirtualSig_Module.Init) && (l_signalId < VirtualSig_MaxId))
    {
        if (VirtualSig_RuntimeInfo[l_signalId].RegCount < VirtualSig_CfgInfo[l_signalId].RegMaxNum)
        {
            for (l_regCount = 0; l_regCount < VirtualSig_RuntimeInfo[l_signalId].RegCount; l_regCount++)
            {
                if ((VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->TaskId == l_taskId)
                {
                    break;
                }
            }
            if (l_regCount >= VirtualSig_RuntimeInfo[l_signalId].RegCount)
            {
            	EnterCriticalCode();
                (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + VirtualSig_RuntimeInfo[l_signalId].RegCount)->Type = VIRTUALSIG_REGTYPE_CHANGED;
                (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + VirtualSig_RuntimeInfo[l_signalId].RegCount)->TaskId = l_taskId;
                VirtualSig_RuntimeInfo[l_signalId].RegCount++;
				ExitCriticalCode();
                l_returnValue = E_OK;
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
				TracePrintf(VIRTUAL_TRACE,TRACE_INFO,"[ViSig-I]>RegOK. SigId:%d,TskId:%d\r\n",l_signalId,l_taskId);
#endif
            }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
			else
			{
				TracePrintf(VIRTUAL_TRACE,TRACE_WARNING,"[ViSig-W]>ReReg. SigId:%d\r\n",l_signalId);
			}
#endif			
        }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
        else
        {
			TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>RegLimit. SigId:%d\r\n",l_signalId);
        }
#endif
    }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
    else
    {
		TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>Para Err. SigId:%d,InitState:%d\r\n",l_signalId,VirtualSig_Module.Init);
    }
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0X2
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] taskId
*Task identifier which want to receive this signal.
*
*@param[in] specValue
*Specific value which want to receive.
*
*@param[inout] None
*
*@param[out] None
*
*@retval E_OK: Subscribe successed.
*@retval E_NOT_OK: Subscribe failed due to max number limited or module not initialize.
*
*Call this function to subscribe if you want to receive this signal event when it equal specified.
*/
boolean VirtualSig_SubSpecValue(uint16 signalId, uint16 taskId, uint32 specValue)
{
    boolean l_returnValue = E_NOT_OK;
    uint16 l_signalId = signalId;
    uint16 l_taskId = taskId;
    uint16 l_regCount = 0U;

    if ((VIRTUALSIG_MODULE_INIT == VirtualSig_Module.Init) && (l_signalId < VirtualSig_MaxId))
    {
        if (VirtualSig_RuntimeInfo[l_signalId].RegCount < VirtualSig_CfgInfo[l_signalId].RegMaxNum)
        {
            for (l_regCount = 0; l_regCount < VirtualSig_RuntimeInfo[l_signalId].RegCount; l_regCount++)
            {
                if ((VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->TaskId == l_taskId)
                {
                    break;
                }
            }
            if (l_regCount >= VirtualSig_RuntimeInfo[l_signalId].RegCount)
            {
				EnterCriticalCode();
				(VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + VirtualSig_RuntimeInfo[l_signalId].RegCount)->Type = VIRTUALSIG_REGTYPE_SPECIFY;
                (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + VirtualSig_RuntimeInfo[l_signalId].RegCount)->TaskId = l_taskId;
                (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + VirtualSig_RuntimeInfo[l_signalId].RegCount)->SpecValue = specValue;
                VirtualSig_RuntimeInfo[l_signalId].RegCount++;
				ExitCriticalCode();
                l_returnValue = E_OK;
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
				TracePrintf(VIRTUAL_TRACE,TRACE_INFO,"[ViSig-I]>RegOK. SigId:%d,TskId:%d\r\n",l_signalId,l_taskId);
#endif
            }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
			else
			{
				TracePrintf(VIRTUAL_TRACE,TRACE_WARNING,"[ViSig-W]>ReReg. SigId:%d\r\n",l_signalId);
			}
#endif		
        }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
        else
        {
			TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>RegLimit. SigId:%d\r\n",l_signalId);
        }
#endif
    }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
    else
    {
		TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>Para Err. SigId:%d,InitState:%d\r\n",l_signalId,VirtualSig_Module.Init);
    }
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0X3
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[inout] None
*
*@param[out] status
*Only the retval equal E_OK,then this status value is right.
*
*@retval E_OK: Get status successed.
*@retval E_NOT_OK: Get status failed due to max number limited or module not initialize.
*
*Call this function to get signal status.
*/
boolean VirtualSig_GetStatus(uint16 signalId, uint32* status)
{
	boolean l_returnValue = E_NOT_OK;
    uint16 l_signalId = signalId;

    if ((VIRTUALSIG_MODULE_INIT == VirtualSig_Module.Init)&&(l_signalId < VirtualSig_MaxId))
    {
        *status = VirtualSig_RuntimeInfo[l_signalId].Status;
        l_returnValue = E_OK;
    }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
	else
	{
		TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>Para Err. SigId:%d,InitState:%d\r\n",l_signalId,VirtualSig_Module.Init);
	}
#endif

    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0X4
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] status
*Signal status value.
*
*@param[inout] None
*
*@param[out] status
*
*@retval E_OK: Set status success.
*@retval E_NOT_OK: Set status failed due to not initialize or status not changed.

*@retval None
*
*Call this function to set signal status.
*/
boolean VirtualSig_SetStatus(uint16 signalId, uint32 status, boolean fromISR)
{
    boolean l_returnValue = E_NOT_OK;
    boolean l_sendMsgFlag = FALSE;
    uint16 l_signalId = signalId;
    uint16 l_regCount = 0U;
    SystemTaskMsgInfoType l_message;

    if ((VIRTUALSIG_MODULE_INIT == VirtualSig_Module.Init) && (l_signalId < VirtualSig_MaxId))
    {
        if (VirtualSig_RuntimeInfo[l_signalId].Status != status)
        {
            if(VIRTUALSIG_F_DIFFER == VirtualSig_CfgInfo[l_signalId].Filter)
            {
                VirtualSig_RuntimeInfo[l_signalId].Status = status;
            }
            if (0 != VirtualSig_RuntimeInfo[l_signalId].RegCount)
            {
                l_message.msgId = VirtualSig_Module.Offset | l_signalId;
                l_message.size = sizeof(status);
                l_message.parBufPtr = (uint8*)&status;
                for (l_regCount = 0; l_regCount < VirtualSig_RuntimeInfo[l_signalId].RegCount; l_regCount++)
                {
                    if ((VIRTUALSIG_REGTYPE_SPECIFY == (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->Type)
                            && (status == (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->SpecValue))
                    {
                        l_sendMsgFlag = TRUE;
                    }
                    else if (VIRTUALSIG_REGTYPE_CHANGED == (VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->Type)
                    {
                        l_sendMsgFlag = TRUE;
                    }
                    if (TRUE == l_sendMsgFlag)
                    {
                        l_returnValue = System_Task_SendMessage((VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->TaskId, &l_message, fromISR);
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
                        TracePrintf(VIRTUAL_TRACE,TRACE_INFO,"[ViSig-I]>SigId:%d send2 TskId:%d,Result:%d\r\n",l_signalId,(VirtualSig_RuntimeInfo[l_signalId].RegTaskInfo + l_regCount)->TaskId,l_returnValue);
#endif
                    }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
					else
					{
                        TracePrintf(VIRTUAL_TRACE,TRACE_INFO,"[ViSig-I]>Ignor(CondNotMatch). SigId:%d\r\n",l_signalId);
					}
#endif
                }
            }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
			else
			{
				TracePrintf(VIRTUAL_TRACE,TRACE_WARNING,"[ViSig-W]>Ignor(ZeroReg). SigId:%d,OldValue:%d,NewValue:%d\r\n",l_signalId,VirtualSig_RuntimeInfo[l_signalId].Status,status);
			}
#endif
        }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
		else
		{
			TracePrintf(VIRTUAL_TRACE,TRACE_WARNING,"[ViSig-W]>Ignor(ValueEqual). SigId:%d,OldValue:%d,NewValue:%d\r\n",l_signalId,VirtualSig_RuntimeInfo[l_signalId].Status,status);	
		}
#endif
    }
#if (STD_ON == VIRTUALSIG_DEBUG_TRACE)
	else
	{
		TracePrintf(VIRTUAL_TRACE,TRACE_ERRO,"[ViSig-E]>Para Err. SigId:%d,InitState:%d\r\n",signalId,VirtualSig_Module.Init);
	}
#endif
    return (l_returnValue);
}
/*=========================== END OF FILE: VirtualSig.c ===========================*/

