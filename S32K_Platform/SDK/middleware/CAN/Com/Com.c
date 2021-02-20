/********************************************************************
File name: Com.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-10 09:54:28
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
notes:
extends by chen zedong
1. adding setting default value for message
2. supports for messageCycleTimeFast & delayTime to configuration for messgae
3. supports for mixing mode & direct mode processing,
4. some time parameter first time is set to 0
5. many of code of the tx_task & tx-event has been re-designed by time parameter

********************************************************************/
#include "Com.h"
#include "CanIf.h"
#include "interrupt_manager.h"
#include "System_Tick.h"
#include "CircularQueue.h"
#include "System_Portable.h"
#include "SimFlag.h"
#include "FlagGroup.h"

extern const PduIdType ComSigId2IpduId[];
extern const Com_SignalIdType ComIpduId2SigId[][2];
extern uint8 *ComIpduBufPtr[];
extern const uint8 *ComIpduBufPtr_Default[];
extern boolean (*Com_GetSigProcess[])(Com_SignalIdType SignalId, uint8* SrcBuffer, uint32 *GetValue);
extern boolean (*Com_SetSigProcess[])(Com_SignalIdType SignalId, uint8* SrcBuffer, uint32 SetValue);
extern boolean (*Com_SigFilter[])(Com_SignalIdType SignalId, uint32 OldValue, uint32 NewValue);

Com_ManagementType Com_Management={COM_UNINIT};

#if(0 < COM_IPDUGROUP_MAX_NUM)
extern PduIdType *ComIpduGroupPtr[];
#endif

#if(0 < COM_SIGNAL_MAX_NUM)
extern const Com_SigConfType Com_SigConfInfo[COM_SIGNAL_MAX_NUM];
Com_SigRunType Com_SigRunInfo[COM_SIGNAL_MAX_NUM];
#endif

#if(0 < COM_IPDU_MAX_NUM)
extern const Type1FuncPtrType ComIpduLongTimeoutCbk[];

SimFlag_BufDef(Com_IpduStartFlag,COM_IPDU_MAX_NUM);
#define Com_GetStartFlag(ComIpduId) SimFlag_GetFlagState(Com_IpduStartFlag,ComIpduId)
#define Com_SetStartFlag(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduStartFlag,ComIpduId, TrueOrFalse)
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
extern const Com_RxIpduConfType Com_RxIpduConfInfo[COM_RX_IPDU_MAX_NUM];
extern const Type1FuncPtrType ComRxIpduIndicateCbk[];
extern const Type1FuncPtrType ComRxIpduShortTimeoutCbk[];

Com_RxIpduRunType Com_RxIpduRunInfo[COM_RX_IPDU_MAX_NUM];
static uint8 Com_RxChangedPduBuf[COM_RX_IPDU_MAX_NUM];
static uint8 RxBuffer[COM_RX_TEMP_BUFFER_SIZE][10];

static Com_RxIpduBufferQueueType Com_RxIpduTempQueue =
{
	0, 0, 0, RxBuffer
};

SimFlag_BufDef(Com_IpduRxDMFlag,COM_RX_IPDU_MAX_NUM);
#define Com_GetRxDM(ComIpduId) SimFlag_GetFlagState(Com_IpduRxDMFlag,ComIpduId)
#define Com_SetRxDM(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduRxDMFlag,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduRxBufChangeFlag,COM_RX_IPDU_MAX_NUM);
#define Com_GetBufChangedFlag(ComIpduId) SimFlag_GetFlagState(Com_IpduRxBufChangeFlag,ComIpduId)
#define Com_SetBufChangedFlag(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduRxBufChangeFlag,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduRxInstantDMFlag,COM_RX_IPDU_MAX_NUM);
#define Com_GetRxInstantDM(ComIpduId) SimFlag_GetFlagState(Com_IpduRxInstantDMFlag,ComIpduId)
#define Com_SetRxInstantDM(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduRxInstantDMFlag,ComIpduId, TrueOrFalse)

#endif

#if(0 < COM_TX_IPDU_MAX_NUM)
extern const Com_TxIpduConfType Com_TxIpduConfInfo[COM_TX_IPDU_MAX_NUM];
extern const Type1FuncPtrType ComTxIpduNotifyCbk[];
extern const Type1FuncPtrType ComTxIpduErrorCbk[];

Com_TxIpduRunType Com_TxIpduRunInfo[COM_TX_IPDU_MAX_NUM];

static uint32* Com_IpduTriggerHandle=NULL_PTR;

FlagGroup_BufDef(Com_IpduTriggerFlag,COM_TX_IPDU_MAX_NUM);

SimFlag_BufDef(Com_IpduTxDMFlag,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxDM(ComIpduId) SimFlag_GetFlagState(Com_IpduTxDMFlag,ComIpduId)
#define Com_SetTxDM(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxDMFlag,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxCyclicFlag,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxCyclic(ComIpduId) SimFlag_GetFlagState(Com_IpduTxCyclicFlag,ComIpduId)
#define Com_SetTxCyclic(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxCyclicFlag,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxLastInFast,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxLastInFast(ComIpduId) SimFlag_GetFlagState(Com_IpduTxLastInFast,ComIpduId)
#define Com_SetTxLastInFast(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxLastInFast,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxOffsetCycle,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxOffsetCycle(ComIpduId) SimFlag_GetFlagState(Com_IpduTxOffsetCycle,ComIpduId)
#define Com_SetTxOffsetCycle(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxOffsetCycle,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxStartEv,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxStartEv(ComIpduId) SimFlag_GetFlagState(Com_IpduTxStartEv,ComIpduId)
#define Com_SetTxStartEv(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxStartEv,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxStartEvTrig,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxStartEvTrig(ComIpduId) SimFlag_GetFlagState(Com_IpduTxStartEvTrig,ComIpduId)
#define Com_SetTxStartEvTrig(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxStartEvTrig,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxStartEvClr,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxStartEvClr(ComIpduId) SimFlag_GetFlagState(Com_IpduTxStartEvClr,ComIpduId)
#define Com_SetTxStartEvClr(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxStartEvClr,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxStartEvRep,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxStartEvRep(ComIpduId) SimFlag_GetFlagState(Com_IpduTxStartEvRep,ComIpduId)
#define Com_SetTxStartEvRep(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxStartEvRep,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_IpduTxRepeatFlag,COM_TX_IPDU_MAX_NUM);
#define Com_GetTxRepeatFlag(ComIpduId) SimFlag_GetFlagState(Com_IpduTxRepeatFlag,ComIpduId)
#define Com_SetTxRepeatFlag(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxRepeatFlag,ComIpduId, TrueOrFalse)

#endif

#ifdef USE_MACRO_FUNC
#define Com_GetStatus() Com_Management.ComStatus
#define Com_GetConfigurationId()((uint32)COM_CONFIGURATION_ID)
#else
Com_StatusType Com_GetStatus(void)
{
	return (Com_Management.ComStatus);
}

uint32 Com_GetConfigurationId(void)
{
	return ((uint32)COM_CONFIGURATION_ID);
}
#endif

void Com_DisableReceptionDM(PduIdType ComIpduId)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_RX_IPDU_MAX_NUM) || (COM_RX_IPDU_MAX_NUM <= ComIpduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
	if ((COM_TX_RX_MODE_PERIODIC == (Com_RxIpduConfInfo[ComIpduId].RxMode))
	        || (COM_TX_RX_MODE_MIXED == (Com_RxIpduConfInfo[ComIpduId].RxMode)))
	{
		Com_SetRxDM(ComIpduId, STD_OFF);
		Com_SetRxInstantDM(ComIpduId, STD_OFF);
	}
#endif
}

void Com_EnableReceptionDM(PduIdType ComIpduId)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_RX_IPDU_MAX_NUM) || (COM_RX_IPDU_MAX_NUM <= ComIpduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
	if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[ComIpduId].RxMode))
	{
		Com_SetRxDM(ComIpduId, STD_ON);
		Com_RxIpduRunInfo[ComIpduId].LTimeoutCounter = Com_RxIpduConfInfo[ComIpduId].LTimeout;
		Com_SetRxInstantDM(ComIpduId, STD_ON);
		Com_RxIpduRunInfo[ComIpduId].STimeoutCounter = Com_RxIpduConfInfo[ComIpduId].STimeout;
	}
	else
	{
		Com_SetRxDM(ComIpduId, STD_OFF);
		Com_SetRxInstantDM(ComIpduId, STD_OFF);
	}
#endif
}

#if (0 < COM_IPDUGROUP_MAX_NUM)
void Com_DisableReceptionDMByGroup(Com_IpduGroupIdType IpduGroupId)
{
	uint8 l_RxIpduId = 0;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;
#endif

#if (STD_ON == COM_DEV_ERROR_DETECT)
	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_RX_IPDUGROUP_MAX_NUM) || (COM_RX_IPDUGROUP_MAX_NUM <= IpduGroupId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif
#if(0 < COM_RX_IPDU_MAX_NUM)
	for (l_RxIpduId = 1; l_RxIpduId < * (ComIpduGroupPtr[IpduGroupId]); l_RxIpduId++)
	{
		Com_SetRxDM(l_RxIpduId, STD_OFF);
		Com_RxIpduRunInfo[l_RxIpduId].LTimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].LTimeout;
		Com_SetRxInstantDM(l_RxIpduId, STD_OFF);
		Com_RxIpduRunInfo[l_RxIpduId].STimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].STimeout;
	}
#endif
}

void Com_EnableReceptionDMByGroup(Com_IpduGroupIdType IpduGroupId)
{
	uint8 l_RxIpduId = 1u;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_RX_IPDUGROUP_MAX_NUM) || (COM_RX_IPDUGROUP_MAX_NUM <= IpduGroupId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif
#if(0 < COM_RX_IPDU_MAX_NUM)
	for (l_RxIpduId = 1; l_RxIpduId < * (ComIpduGroupPtr[IpduGroupId]); l_RxIpduId++)
	{
		if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[l_RxIpduId].RxMode))
		{
			Com_SetRxDM(l_RxIpduId, STD_ON);
			Com_RxIpduRunInfo[l_RxIpduId].LTimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].LTimeout;
			Com_SetRxInstantDM(l_RxIpduId, STD_ON);
			Com_RxIpduRunInfo[l_RxIpduId].STimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].STimeout;
		}
		else
		{
			Com_SetRxDM(l_RxIpduId, STD_OFF);
			Com_SetRxInstantDM(l_RxIpduId, STD_OFF);
		}
	}
#endif
}
#endif

static boolean Com_GetHighPriTrigIpdu(PduIdType *ComTxPduId)
{
#if(0 < COM_TX_IPDU_MAX_NUM)
	return(FlagGroup_GetSettedFlag(Com_IpduTriggerHandle,FLAGGROUP_MODE_LOWEST,ComTxPduId));
#else
	return(E_NOT_OK);
#endif
}

static void Com_SetIpduTrigger(PduIdType ComTxPduId, boolean ActiveOrIdle)
{
#if(0 < COM_TX_IPDU_MAX_NUM)
	FlagGroup_SetFlagState(Com_IpduTriggerHandle,ComTxPduId,ActiveOrIdle);
#endif
}

static boolean Com_OpaqueTypeSignal(uint16 SignalId, uint8 GetOrSet, uint8 *SrcBuffer, void *Value)
{
    uint8 startByte = 0u;
    uint8 stopByte = 0u;
    uint8 loop = 0u;
    boolean returnValue = E_OK;

    if (32 >= Com_SigConfInfo[SignalId].Length)
    {
        returnValue = E_NOT_OK;
    }
    else
    {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        stopByte = startByte + Com_SigConfInfo[SignalId].TypeUint8NBytes - 1;

        for (loop = startByte; loop <= stopByte; loop++)
        {
            if (COM_GET_SIG_VALUE == GetOrSet)
            {
                *((uint8 *)Value + loop) = *(SrcBuffer + loop);
            }
            else
            {
                *(SrcBuffer + loop) = *((uint8 *)Value + loop);
            }
        }
    }
    return (returnValue);
}

void Com_Init(const void *config)
{
	uint8 l_IpduId = 0XFF;
	uint16 l_SigId = 0XFFFF;
	uint8 l_PduBufIndex = 0;

	if(0 != (Com_Management.ComStatus & COM_INIT))
	{
		return;
	}
	#if(0 < COM_TX_IPDU_MAX_NUM)
	Com_IpduTriggerHandle = FlagGroup_Init(Com_IpduTriggerFlag,COM_TX_IPDU_MAX_NUM);
	#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
	for (l_IpduId = 0; l_IpduId < COM_RX_IPDU_MAX_NUM; l_IpduId++)
	{
		for (l_PduBufIndex = 0; l_PduBufIndex < Com_RxIpduConfInfo[l_IpduId].PduLength; l_PduBufIndex++)
		{
			ComIpduBufPtr[l_IpduId][l_PduBufIndex] = ComIpduBufPtr_Default[l_IpduId][l_PduBufIndex];

		}
		Com_SetStartFlag(l_IpduId, STD_OFF);
	}
	CQueue_Init(CQUEUE_CH_CANPDUCHANGE, Com_RxChangedPduBuf, COM_RX_IPDU_MAX_NUM);
#endif

#if(0 < COM_TX_IPDU_MAX_NUM)
	for (l_IpduId = 0; l_IpduId < COM_TX_IPDU_MAX_NUM; l_IpduId++)
	{

		for (l_PduBufIndex = 0; l_PduBufIndex < Com_TxIpduConfInfo[l_IpduId].PduLength; l_PduBufIndex++)
		{
			ComIpduBufPtr[l_IpduId + COM_TX_IPDU_START_INDEX][l_PduBufIndex] = ComIpduBufPtr_Default[l_IpduId + COM_TX_IPDU_START_INDEX][l_PduBufIndex];

		}
		Com_SetStartFlag(l_IpduId + COM_TX_IPDU_START_INDEX, STD_OFF);
	}
#endif

#if(0 < COM_SIGNAL_MAX_NUM)
	for (l_SigId = 0; l_SigId < COM_SIGNAL_MAX_NUM; l_SigId++)
	{
		//if(COM_F_ONE_EVERY_N == Com_SigConfInfo[l_SigId].Filter.Algorithm)
		//{
		Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
		//}
	}
#endif
	Com_Management.ComStatus |= (COM_INIT | COM_RX_ENABLE | COM_TX_ENABLE);
}

void Com_DeInit(void)
{
	Com_Management.ComStatus = COM_UNINIT;
}

#if (0 < COM_IPDUGROUP_MAX_NUM)
void Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId)
{
	uint8 l_IpduIdLoop = 1U;
	uint16 l_IpduId = 0XFFFF;
	uint16 l_TxIpduId = 0XFFFF;
	uint16 l_SigId = 0XFFFF;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTART, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_IPDUGROUP_MAX_NUM) || (COM_IPDUGROUP_MAX_NUM <= IpduGroupId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTART, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif

#if(0 < COM_IPDU_MAX_NUM)
	for (l_IpduIdLoop = 1; l_IpduIdLoop < * (ComIpduGroupPtr[IpduGroupId]); l_IpduIdLoop++)
	{
		l_IpduId = *(ComIpduGroupPtr[IpduGroupId] + l_IpduIdLoop);
		if (FALSE == Com_GetStartFlag(l_IpduId))
		{
			Com_SetStartFlag(l_IpduId, TRUE);
#if(0 < COM_RX_IPDU_MAX_NUM)
			if (l_IpduId < COM_RX_IPDU_MAX_NUM)
			{				
				Com_SetStartFlag(l_IpduId, STD_ON);
				Com_SetBufChangedFlag(l_IpduId, FALSE);
				
				if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[l_IpduId].RxMode))
				{
					Com_SetRxDM(l_IpduId, STD_ON);
					Com_RxIpduRunInfo[l_IpduId].LTimeoutCounter = Com_RxIpduConfInfo[l_IpduId].LTimeout;
					Com_SetRxInstantDM(l_IpduId, STD_ON);
					Com_RxIpduRunInfo[l_IpduId].STimeoutCounter = Com_RxIpduConfInfo[l_IpduId].STimeout;
				}
				else
				{
					Com_SetRxDM(l_IpduId, STD_OFF);
					Com_SetRxInstantDM(l_IpduId, STD_OFF);
				}	
			}
#endif
#if(0 < COM_TX_IPDU_MAX_NUM)
			if (l_IpduId >= COM_RX_IPDU_MAX_NUM)
			{
				l_TxIpduId = l_IpduId - COM_TX_IPDU_START_INDEX;
				Com_SetIpduTrigger(l_TxIpduId, STD_IDLE);
				Com_SetTxLastInFast(l_TxIpduId, STD_OFF);
				Com_SetTxOffsetCycle(l_TxIpduId, STD_OFF);
				Com_SetTxStartEv(l_TxIpduId, STD_OFF);
				Com_SetTxStartEvClr(l_TxIpduId, STD_OFF);
				Com_SetTxStartEvRep(l_TxIpduId, STD_OFF);
				Com_SetTxStartEvTrig(l_TxIpduId, STD_OFF);
				Com_SetTxDM(l_TxIpduId, STD_OFF);
				Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
				//Com_SetStartFlag(l_TxIpduId, TRUE);
				Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter = 0;
				Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter =  0;
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = 0;
				Com_TxIpduRunInfo[l_TxIpduId].DelayTime = 0;

				if (((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_PERIODIC)
				        || ((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_MIXED))
				{
					Com_SetTxCyclic(l_TxIpduId, STD_ON);
				}
				Com_SetStartFlag(l_TxIpduId + COM_TX_IPDU_START_INDEX, STD_ON);
			}
#endif
#if(0 < COM_SIGNAL_MAX_NUM)
			for (l_SigId = ComIpduId2SigId[l_IpduId][0]; l_SigId <= ComIpduId2SigId[l_IpduId][1]; l_SigId++)
			{
				Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
			}
#endif
		}
	}
#endif
}

void Com_IpduGroupStop(Com_IpduGroupIdType IpduGroupId)
{
	uint8 l_IpduIdLoop = 1U;
	uint16 l_IpduId = 0XFFFF;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTOP, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_IPDUGROUP_MAX_NUM) || (COM_IPDUGROUP_MAX_NUM <= IpduGroupId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTOP, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif
#if(0 < COM_IPDU_MAX_NUM)
	for (l_IpduIdLoop = 1; l_IpduIdLoop < * (ComIpduGroupPtr[IpduGroupId]); l_IpduIdLoop++)
	{
		l_IpduId = *(ComIpduGroupPtr[IpduGroupId] + l_IpduIdLoop);

		Com_SetStartFlag(l_IpduId, FALSE);

#if(0 < COM_TX_IPDU_MAX_NUM)
		if ((COM_TX_IPDU_START_INDEX <= l_IpduId) && (l_IpduId < COM_IPDU_MAX_NUM))
		{
			Com_SetIpduTrigger(l_IpduId - COM_TX_IPDU_START_INDEX, STD_IDLE);
		}
#endif
	}
#endif
}
#endif

void Com_PeriodicStart(void)
{
#if(0 < COM_TX_IPDU_MAX_NUM)
	uint16 l_TxIpduId = 0;

	Com_Management.ComStatus |= COM_PERIODIC_ENABLE;
	for (l_TxIpduId = 0; l_TxIpduId < COM_TX_IPDU_MAX_NUM; l_TxIpduId++)
	{
		if ((0 != (COM_TX_RX_MODE_PERIODIC & Com_TxIpduConfInfo[l_TxIpduId].TxMode))
		        || (0 != (COM_TX_RX_MODE_MIXED & Com_TxIpduConfInfo[l_TxIpduId].TxMode)))
		{
			Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter = 0;
		}
	}
#endif
}

void Com_PeriodicStop(void)
{
	Com_Management.ComStatus &= (~COM_PERIODIC_ENABLE);
}

void Com_TxStart(void)
{
#if(0 < COM_TX_IPDU_MAX_NUM)
	uint16 l_TxIpduId = 0XFFFF;
	uint16 l_SigId = 0XFFFF;

	for (l_TxIpduId = 0; l_TxIpduId < COM_TX_IPDU_MAX_NUM; l_TxIpduId++)
	{
		Com_SetIpduTrigger(l_TxIpduId, STD_IDLE);
		Com_SetTxLastInFast(l_TxIpduId, STD_OFF);
		Com_SetTxOffsetCycle(l_TxIpduId, STD_OFF);
		Com_SetTxStartEv(l_TxIpduId, STD_OFF);
		Com_SetTxStartEvClr(l_TxIpduId, STD_OFF);
		Com_SetTxStartEvRep(l_TxIpduId, STD_OFF);
		Com_SetTxStartEvTrig(l_TxIpduId, STD_OFF);
		Com_SetTxDM(l_TxIpduId, STD_OFF);
		Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
		//Com_SetStartFlag(l_TxIpduId, TRUE);
		Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter = 0;
		Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter =  0;
		Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = 0;
		Com_TxIpduRunInfo[l_TxIpduId].DelayTime = 0;

		if (((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_PERIODIC)
		        || ((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_MIXED))
		{
			Com_SetTxCyclic(l_TxIpduId, STD_ON);
		}

		Com_SetStartFlag(l_TxIpduId + COM_TX_IPDU_START_INDEX, STD_ON);

	}
#if(0 < COM_SIGNAL_MAX_NUM)
	for (l_SigId = 0; l_SigId < COM_TX_SIGNAL_MAX_NUM; l_SigId++)
	{
		Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
	}
#endif
	Com_Management.ComStatus |= COM_TX_ENABLE;
#endif
}

void Com_TxStop(void)
{
	Com_Management.ComStatus &= (~COM_TX_ENABLE);
}

void Com_RxStart(void)
{
	uint16 l_RxIpduId = 0XFFFF;
	uint16 l_SigId = 0XFFFF;

#if(0 < COM_RX_IPDU_MAX_NUM)
	for (l_RxIpduId = 0; l_RxIpduId < COM_RX_IPDU_MAX_NUM; l_RxIpduId++)
	{
		Com_SetStartFlag(l_RxIpduId, STD_ON);
		Com_SetBufChangedFlag(l_RxIpduId, FALSE);			
		if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[l_RxIpduId].RxMode))
		{
			Com_SetRxDM(l_RxIpduId, STD_ON);			
			Com_RxIpduRunInfo[l_RxIpduId].LTimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].LTimeout;
			Com_SetRxInstantDM(l_RxIpduId, STD_ON);
			Com_RxIpduRunInfo[l_RxIpduId].STimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].STimeout;
			Com_RxIpduRunInfo[l_RxIpduId].STimeoutCounter = Com_RxIpduConfInfo[l_RxIpduId].STimeout;
		}
		else
		{
			Com_SetRxDM(l_RxIpduId, STD_OFF);
			Com_SetRxInstantDM(l_RxIpduId, STD_OFF);
		}	
	}
#if(0 < COM_SIGNAL_MAX_NUM)
	for (l_SigId = 0; l_SigId < COM_RX_SIGNAL_MAX_NUM; l_SigId++)
	{
		Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
	}
#endif
	Com_Management.ComStatus |= COM_RX_ENABLE;
#endif
}

void Com_RxStop(void)
{
	Com_Management.ComStatus &= (~COM_RX_ENABLE);
}

void Com_StartTxEvent(Com_SignalIdType SignalId);
static void Com_StartTxEventPdu(PduIdType l_TxIpduId);

void Com_StopTxEventWithRepetion(Com_SignalIdType SignalId)
{
#if(0 < COM_TX_IPDU_MAX_NUM)
	PduIdType l_TxIpduId = ComSigId2IpduId[SignalId] - COM_TX_IPDU_START_INDEX;
	Com_SetTxStartEvRep(l_TxIpduId, STD_ON);
	Com_SetTxStartEvClr(l_TxIpduId, STD_ON);
	Com_StartTxEventPdu(l_TxIpduId);
	Com_SetTxCyclic(l_TxIpduId, STD_OFF);
#endif
}

void Com_StopTxEvent(Com_SignalIdType SignalId)
{
#if(0 < COM_TX_IPDU_MAX_NUM)

	PduIdType l_TxIpduId = ComSigId2IpduId[SignalId] - COM_TX_IPDU_START_INDEX;
	Com_SetTxStartEvClr(l_TxIpduId, STD_ON);

#endif
}

static void Com_StartTxEventPdu(PduIdType l_TxIpduId)
{
	uint32 diffTime = 0;
	boolean trig = FALSE;

	if (Com_GetTxRepeatFlag(l_TxIpduId) > 0)
	{
		Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter =  Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions + 1;
		if (Com_GetTxStartEvRep(l_TxIpduId) > 0)
		{
			Com_SetTxStartEv(l_TxIpduId, STD_ACTIVE);
			return;
		}
		Com_SetTxStartEvRep(l_TxIpduId, STD_OFF);
		Com_SetTxStartEvClr(l_TxIpduId, STD_OFF);

		return;
	}
	EnterCriticalCode();
	if (Com_TxIpduRunInfo[l_TxIpduId].DelayTime == 0)
	{
		trig = TRUE;
	}
	if ((0 != Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions))
	{
		Com_SetTxRepeatFlag(l_TxIpduId, STD_ON);
	}
	ExitCriticalCode();
	if ((COM_TX_RX_MODE_DIRECT == Com_TxIpduConfInfo[l_TxIpduId].TxMode) ||
	        (COM_TX_RX_MODE_MIXED == Com_TxIpduConfInfo[l_TxIpduId].TxMode) ||
	        (Com_GetTxStartEvRep(l_TxIpduId) > 0))
	{
		if ((0 != Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions))
		{
			Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter =  Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions + 1;
		}
		if (Com_GetTxOffsetCycle(l_TxIpduId) > 0)
		{
			if (Com_TxIpduConfInfo[l_TxIpduId].TxMode == COM_TX_RX_MODE_MIXED)
			{
				Com_SetTxCyclic(l_TxIpduId, STD_ON);
			}
			if (Com_TxIpduRunInfo[l_TxIpduId].CycleOffset >= Com_TxIpduConfInfo[l_TxIpduId].DelayTime)
			{
				Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
				if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
				{		
					if (STD_OFF == Com_GetTxDM(l_TxIpduId))
					{
						Com_SetTxDM(l_TxIpduId, STD_ON);
					}
				}
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;

				Com_SetTxStartEvTrig(l_TxIpduId, STD_ON);

				if (Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter > 1)
				{
					if (Com_TxIpduConfInfo[l_TxIpduId].DelayTime >= Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast)
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast;
					}
				}
			}
			else
			{
				Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime - Com_TxIpduRunInfo[l_TxIpduId].CycleOffset;
				Com_SetTxStartEv(l_TxIpduId, STD_ACTIVE);
			}
			Com_SetTxOffsetCycle(l_TxIpduId, STD_OFF);
			return;
		}
		if (Com_GetTxCyclic(l_TxIpduId) == 0)
		{
			if ((Com_TxIpduRunInfo[l_TxIpduId].DelayTime == 0) || (trig == TRUE))
			{
				Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
				if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
				{
					if (STD_OFF == Com_GetTxDM(l_TxIpduId))
					{
						Com_SetTxDM(l_TxIpduId, STD_ON);
					}
				}
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;
				Com_SetTxStartEvTrig(l_TxIpduId, STD_ON);

				if (Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter > 1)
				{
					if (Com_TxIpduConfInfo[l_TxIpduId].DelayTime >= Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast)
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast;
					}
				}
			}
			else
			{
				Com_SetTxStartEv(l_TxIpduId, STD_ACTIVE);
			}
		}
		else
		{
			if (Com_TxIpduConfInfo[l_TxIpduId].TimePeriod >= Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter)
			{
				diffTime = Com_TxIpduConfInfo[l_TxIpduId].TimePeriod - Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter;

				if (diffTime >= Com_TxIpduConfInfo[l_TxIpduId].DelayTime)
				{
					diffTime = 0;
				}
			}
			if ((0 != Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions))
			{
				if (diffTime == 0)
				{
					if (Com_TxIpduConfInfo[l_TxIpduId].DelayTime >= Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast)
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast;
					}
					Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
					Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;
					if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
					{	
						if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
						{	
							if (STD_OFF == Com_GetTxDM(l_TxIpduId))
							{
								Com_SetTxDM(l_TxIpduId, STD_ON);
							}
						}
					}
					if ((Com_GetTxStartEvRep(l_TxIpduId) > 0) && (Com_GetTxStartEvClr(l_TxIpduId) > 0))
					{
						Com_SetTxStartEvTrig(l_TxIpduId, STD_ON);
					}
				}
				else
				{
					Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime - diffTime;
					if ((Com_GetTxStartEvRep(l_TxIpduId) > 0) && (Com_GetTxStartEvClr(l_TxIpduId) > 0))
					{
						Com_SetTxStartEv(l_TxIpduId, STD_ACTIVE);
					}
				}
			}
		}
	}
}

void Com_StartTxEvent(Com_SignalIdType SignalId)
{
#if(0 < COM_TX_IPDU_MAX_NUM)

	PduIdType l_TxIpduId = ComSigId2IpduId[SignalId] - COM_TX_IPDU_START_INDEX;
	if (Com_GetTxRepeatFlag(l_TxIpduId) == 0)
	{
		Com_TxIpduRunInfo[l_TxIpduId].DelayTime = 0;
	}
	if (((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_PERIODIC)
	        || ((Com_TxIpduConfInfo[l_TxIpduId].TxMode) == COM_TX_RX_MODE_MIXED))
	{
		Com_SetTxCyclic(l_TxIpduId, STD_ON);
	}
	Com_StartTxEventPdu(l_TxIpduId);
#endif
}

uint8 Com_SendSignal(Com_SignalIdType SignalId, void *SignalDataPtr)
{
	boolean ReturnValue = E_OK;
	boolean l_SendEvent = TRUE;
	uint32 l_OldValue = 0;
	uint32 l_NewValue = 0;
	PduIdType IpduId = ComSigId2IpduId[SignalId];

#if (STD_ON == COM_DEV_ERROR_DETECT)
	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	if (COM_TX_ENABLE != (COM_TX_ENABLE & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	if ((COM_TX_SIGNAL_START_INDEX > SignalId) || (COM_SIGNAL_MAX_NUM <= SignalId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_E_ID);
		ReturnValue = E_NOT_OK;
	}
	if (NULL_PTR == SignalDataPtr)
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	if (E_NOT_OK == ReturnValue)
	{
		return (ReturnValue);
	}
#endif
#if(0 < COM_TX_SIGNAL_MAX_NUM)
	if (FALSE == Com_GetStartFlag(ComSigId2IpduId[SignalId]))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	else
	{
		switch (Com_SigConfInfo[SignalId].SignalType)
		{
		case COM_SIG_BOOLEAN:
		case COM_SIG_UINT8:
		case COM_SIG_SINT8:
			l_NewValue = *(uint8 *)SignalDataPtr;
			break;
		case COM_SIG_SINT16:
		case COM_SIG_UINT16:
			l_NewValue = *(uint16 *)SignalDataPtr;
			break;
		case COM_SIG_SINT32:
		case COM_SIG_UINT32:
			l_NewValue = *(uint32 *)SignalDataPtr;
			break;
		default:
			break;
		}
		if ((32 < Com_SigConfInfo[SignalId].Length)
		        || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)
		        || (COM_SIG_OPAQUE == Com_SigConfInfo[SignalId].EndianType))
		{
			EnterCriticalCode();
			Com_OpaqueTypeSignal(SignalId, COM_SET_SIG_VALUE, ComIpduBufPtr[IpduId], (uint8*)SignalDataPtr);
			ExitCriticalCode();
		}
		else
		{
			EnterCriticalCode();
#if (STD_ON == USE_TX_SIG_FILTER)
			(*Com_GetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, ComIpduBufPtr[IpduId], &l_OldValue);
			l_SendEvent = (*Com_SigFilter[Com_SigConfInfo[SignalId].Filter.Algorithm])(SignalId, l_OldValue, l_NewValue);
#endif
			if (TRUE == l_SendEvent)
			{
				(*Com_SetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, ComIpduBufPtr[IpduId], l_NewValue);
			}
			ExitCriticalCode();
		}
		if ((COM_SIG_TX_TRIGGERED == Com_SigConfInfo[SignalId].TransferProperty) && (TRUE == l_SendEvent))
		{
			Com_StartTxEvent(SignalId);
		}
		ReturnValue = E_OK;
	}
#else
	ReturnValue = E_NOT_OK;
#endif
	return (ReturnValue);
}

//this function need suit for both rx and tx signal:ref internal communication
uint8 Com_ReceiveSignal(Com_SignalIdType SignalId, void *SignalDataPtr)
{
	boolean ReturnValue = E_OK;
	PduIdType IpduId = ComSigId2IpduId[SignalId];
	uint32 SignalValue = 0xFFFF;

#if (STD_ON == COM_DEV_ERROR_DETECT)
	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	if (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
//#ifdef COM_INTERNAL_COMMUNICATON
	if ((0U == COM_SIGNAL_MAX_NUM) || (COM_SIGNAL_MAX_NUM <= SignalId))
//#else
//   if ((0U == COM_RX_SIGNAL_MAX_NUM) || (COM_RX_SIGNAL_MAX_NUM <= SignalId))
//#endif
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_E_ID);
		ReturnValue = E_NOT_OK;
	}
	if (NULL_PTR == SignalDataPtr)
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	if (E_NOT_OK == ReturnValue)
	{
		return (E_NOT_OK);
	}
#endif
#if(0 < COM_RX_SIGNAL_MAX_NUM)
	if (FALSE == Com_GetStartFlag(ComSigId2IpduId[SignalId]))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_SERVICE_NOT_AVAILABLE);
		ReturnValue = E_NOT_OK;
	}
	else
	{
		if ((32 < Com_SigConfInfo[SignalId].Length)
		        || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)
		        || (COM_SIG_OPAQUE == Com_SigConfInfo[SignalId].EndianType))
		{
			EnterCriticalCode();
			Com_OpaqueTypeSignal(SignalId, COM_GET_SIG_VALUE, ComIpduBufPtr[IpduId], (uint8*)SignalDataPtr);
			ExitCriticalCode();
		}
		else
		{
			EnterCriticalCode();
			(*Com_GetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, ComIpduBufPtr[IpduId], &SignalValue);
			ExitCriticalCode();
			switch (Com_SigConfInfo[SignalId].SignalType)
			{
			case COM_SIG_BOOLEAN:
			case COM_SIG_UINT8:
			case COM_SIG_SINT8:
				*(uint8*)SignalDataPtr = (uint8)SignalValue;
				break;
			case COM_SIG_SINT16:
			case COM_SIG_UINT16:
				*(uint16*)SignalDataPtr = (uint16)SignalValue;
				break;
			case COM_SIG_SINT32:
			case COM_SIG_UINT32:
				*(uint32*)SignalDataPtr = (uint32)SignalValue;
				break;
			default:
				break;
			}
		}
		ReturnValue = E_OK;
	}
#else
	ReturnValue = E_NOT_OK;
#endif
	return (ReturnValue);
}

uint8 (*Com_Sig_Value[])(Com_SignalIdType SignalId, void *SignalDataPtr) =
{
	Com_ReceiveSignal, Com_SendSignal
};

void Com_RxIndication(PduIdType ComRxPduId, const PduInfoType *PduInfoPtr)
{	
	uint16 l_loop  = 0u;
	
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 l_ReturnValue = E_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
		l_ReturnValue = E_NOT_OK;
	}
	if (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
		l_ReturnValue = E_NOT_OK;
	}
	if ((0U == COM_RX_IPDU_MAX_NUM) || (COM_RX_IPDU_MAX_NUM <= ComRxPduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_E_ID);
		l_ReturnValue = E_NOT_OK;
	}
	if (NULL_PTR == PduInfoPtr)
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
		l_ReturnValue = E_NOT_OK;
	}
	if (E_NOT_OK == l_ReturnValue)
	{
		return;
	}
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)

	if (FALSE == Com_GetStartFlag(ComRxPduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
	}
	else
	{
	
		if (COM_RX_TEMP_BUFFER_SIZE > Com_RxIpduTempQueue.Count)
		{
			*(uint16 *)*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Tail) = ComRxPduId;
			for (l_loop = 0; l_loop < Com_RxIpduConfInfo[ComRxPduId].PduLength; l_loop++)
			{
				*(*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Tail) + 2 + l_loop) = *(PduInfoPtr->SduDataPtr + l_loop);
			}
			EnterCriticalCode();
			Com_RxIpduTempQueue.Count++;
			Com_RxIpduTempQueue.Tail++;
			Com_RxIpduTempQueue.Tail = Com_RxIpduTempQueue.Tail % COM_RX_TEMP_BUFFER_SIZE;
			ExitCriticalCode();
		}
		else
		{
			Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_E_LIMIT);
		}
		
		if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[ComRxPduId].RxMode))
		{
			Com_SetRxDM(ComRxPduId, STD_ON);
			Com_RxIpduRunInfo[ComRxPduId].LTimeoutCounter = Com_RxIpduConfInfo[ComRxPduId].LTimeout;
			Com_SetRxInstantDM(ComRxPduId, STD_ON);
			Com_RxIpduRunInfo[ComRxPduId].STimeoutCounter = Com_RxIpduConfInfo[ComRxPduId].STimeout;
		}
		else
		{
			Com_SetRxDM(ComRxPduId, STD_OFF);
			Com_SetRxInstantDM(ComRxPduId, STD_OFF);
		}

	}
#endif
}

void Com_TxConfirmation(PduIdType ComTxPduId)
{
	uint16 l_TxIpduId = 0XFFFF;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_TXCONFIRMATION, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_TX_IPDU_MAX_NUM) || (COM_IPDU_MAX_NUM <= ComTxPduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_TXCONFIRMATION, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_OK != returnValue)
	{
		return;
	}
#endif
#if(0 < COM_TX_IPDU_MAX_NUM)
	EnterCriticalCode();
	l_TxIpduId = ComTxPduId - COM_TX_IPDU_START_INDEX;
	Com_SetTxDM(l_TxIpduId, STD_OFF);
	ExitCriticalCode();

	if (NULL != ComTxIpduNotifyCbk[l_TxIpduId])
	{
		(*ComTxIpduNotifyCbk[l_TxIpduId])();
	}

#endif
}


void Com_TxErrorCallBack(PduIdType ComTxPduId)
{
	uint16 l_TxIpduId = 0XFFFF;
#if (STD_ON == COM_DEV_ERROR_DETECT)
	uint8 returnValue = E_NOT_OK;

	if (COM_INIT != (COM_INIT & Com_GetStatus()))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_TXCONFIRMATION, COM_SERVICE_NOT_AVAILABLE);
		returnValue = E_NOT_OK;
	}
	if ((0U == COM_TX_IPDU_MAX_NUM) || (COM_IPDU_MAX_NUM <= ComTxPduId))
	{
		Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_TXCONFIRMATION, COM_E_ID);
		returnValue = E_NOT_OK;
	}
	if (E_NOT_OK == returnValue)
	{
		return;
	}
#endif
#if(0 < COM_TX_IPDU_MAX_NUM)
	l_TxIpduId = ComTxPduId - COM_TX_IPDU_START_INDEX;
	if ((STD_ON == Com_GetTxRepeatFlag(l_TxIpduId)))
	{
		if (0 != Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter)
		{
			Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter--;
			Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
		}
		else
		{
			Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
			if (NULL != ComTxIpduErrorCbk[l_TxIpduId])
			{
				(*ComTxIpduErrorCbk[l_TxIpduId])();
			}
		}
	}
#endif
}


uint8 Com_GetIpduData(PduIdType pduId, uint8 *destBuf)
{
	uint8 l_dataLen = 0xFFu;
	uint8 l_copyCount = 0u;

	if (NULL != destBuf)
	{
		if (pduId < COM_RX_IPDU_MAX_NUM)
		{
			l_dataLen = Com_RxIpduConfInfo[pduId].PduLength;
		}
		else if (pduId < COM_IPDU_MAX_NUM)
		{
			l_dataLen = Com_TxIpduConfInfo[pduId].PduLength;
		}
		for (l_copyCount = 0; l_copyCount < l_dataLen; l_copyCount++)
		{
			*(destBuf + l_copyCount) = *(ComIpduBufPtr[pduId] + l_copyCount);
		}
	}
	return (l_dataLen);
}

uint8 Com_GetIpduDataLen(PduIdType pduId)
{
	uint8 returnValue = 0XFF;

	if (pduId < COM_RX_IPDU_MAX_NUM)
	{
		returnValue = Com_RxIpduConfInfo[pduId].PduLength;
	}
	else if (pduId < COM_IPDU_MAX_NUM)
	{
		returnValue = Com_TxIpduConfInfo[pduId].PduLength;
	}
	return (returnValue);
}

boolean Com_IsRxBufChanged(PduIdType pduId)
{
	boolean returnValue = STD_IDLE;

	if (pduId < COM_RX_IPDU_MAX_NUM)
	{
		returnValue = Com_GetBufChangedFlag(pduId);
	}
	return (returnValue);
}

uint8 Com_ClearRxBufChangedFlag(PduIdType pduId)
{
	uint8 returnValue = E_NOT_OK;

	if (pduId < COM_RX_IPDU_MAX_NUM)
	{
		Com_SetBufChangedFlag(pduId, STD_OFF);
		returnValue = E_OK;
	}
	return (returnValue);
}

void Com_MainFunctionRx(void)
{	
	uint8 l_loop = 0;
	boolean l_FilterResult = FALSE;
	PduIdType l_ComRxPduId = 0xFFFFu;
	uint32 l_OldValue = 0u;
	uint32 l_NewValue = 0u;
	uint8 l_bufChangeFlag = FALSE;
	uint16 tempCount = 0xFFFF;
	uint8 lastPduId[COM_RX_IPDU_MAX_NUM] = {0};
	uint8 l_RxChangedPduId = 0xFFu;

	if (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus()))
	{
		return;
	}

#if(0 < COM_RX_IPDU_MAX_NUM)
	//here consider clear rxchanged cqueue
	//EnterCriticalCode();
	tempCount = Com_RxIpduTempQueue.Count;
	while ((tempCount > 0)) 
	{
		l_bufChangeFlag = FALSE;
		l_ComRxPduId = *(uint16 *)(*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Head));
		if (lastPduId[l_ComRxPduId] != 0)
		{
			EnterCriticalCode();
			*(uint16 *)*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Tail) = l_ComRxPduId;
			for (l_loop = 0; l_loop < Com_RxIpduConfInfo[l_ComRxPduId].PduLength; l_loop++)
			{
				*(*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Tail) + 2 + l_loop) = *(*(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Head) + 2 + l_loop);
			}
			Com_RxIpduTempQueue.Tail++;
			tempCount--;
			Com_RxIpduTempQueue.Tail = Com_RxIpduTempQueue.Tail % COM_RX_TEMP_BUFFER_SIZE;
			Com_RxIpduTempQueue.Head++;
			Com_RxIpduTempQueue.Head = Com_RxIpduTempQueue.Head % COM_RX_TEMP_BUFFER_SIZE;
			ExitCriticalCode();
			continue;
		}
		ComRxIpduIndicateCbk[l_ComRxPduId]();
		EnterCriticalCode();
		for (l_loop = ComIpduId2SigId[l_ComRxPduId][0]; l_loop <= ComIpduId2SigId[l_ComRxPduId][1]; l_loop++)
		{
			if ((32 < Com_SigConfInfo[l_loop].Length)
			        || (COM_SIG_UINT8_N == Com_SigConfInfo[l_loop].SignalType)
			        || (COM_SIG_OPAQUE == Com_SigConfInfo[l_loop].EndianType))
			{
				Com_OpaqueTypeSignal(l_loop, COM_SET_SIG_VALUE, ComIpduBufPtr[l_ComRxPduId], *(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Head) + 2);
			}
			else
			{
#if (STD_ON == USE_RX_SIG_FILTER)
				(*Com_GetSigProcess[Com_SigConfInfo[l_loop].EndianType])(l_loop, ComIpduBufPtr[l_ComRxPduId], &l_OldValue);
				(*Com_GetSigProcess[Com_SigConfInfo[l_loop].EndianType])(l_loop, *(Com_RxIpduTempQueue.Buffer + Com_RxIpduTempQueue.Head) + 2, &l_NewValue);
				l_FilterResult = (*Com_SigFilter[Com_SigConfInfo[l_loop].Filter.Algorithm])(l_loop, l_OldValue, l_NewValue);
				if (TRUE == l_FilterResult)
#endif
				{
					(*Com_SetSigProcess[Com_SigConfInfo[l_loop].EndianType])(l_loop, ComIpduBufPtr[l_ComRxPduId], l_NewValue);
					l_bufChangeFlag = TRUE;
				}
			}
		}
		lastPduId[l_ComRxPduId] = 1;
		tempCount--;
		Com_RxIpduTempQueue.Head++;
		Com_RxIpduTempQueue.Head = Com_RxIpduTempQueue.Head % COM_RX_TEMP_BUFFER_SIZE;
		Com_RxIpduTempQueue.Count--;
		if (TRUE == l_bufChangeFlag)
		{
			Com_SetBufChangedFlag(l_ComRxPduId, STD_ON);
			l_RxChangedPduId = (uint8)(l_ComRxPduId);
			CQueue_Push(CQUEUE_CH_CANPDUCHANGE, &l_RxChangedPduId, 1,STD_OFF);
		}
		ExitCriticalCode();
	}
	//ExitCriticalCode();

	for (l_loop = 0; l_loop < COM_RX_IPDU_MAX_NUM; l_loop++)
	{
		if (FALSE == Com_GetStartFlag(l_loop))
		{
			continue;
		}
				
		//if rx deadline monitoring disabled, no callout call
		if ((NULL != ComIpduLongTimeoutCbk[l_loop]) && (STD_ON == Com_GetRxDM(l_loop)))
		{
			if (Com_RxIpduRunInfo[l_loop].LTimeoutCounter == 0)
			{
				Com_SetRxDM(l_loop,STD_OFF);
				(*ComIpduLongTimeoutCbk[l_loop])();	
			}
			if (Com_RxIpduRunInfo[l_loop].LTimeoutCounter > 0)
			{
				Com_RxIpduRunInfo[l_loop].LTimeoutCounter--;
			}
		}
		if ((NULL != ComRxIpduShortTimeoutCbk[l_loop])&& (STD_ON == Com_GetRxInstantDM(l_loop)))
		{
			if (Com_RxIpduRunInfo[l_loop].STimeoutCounter == 0)
			{
				Com_SetRxInstantDM(l_loop,STD_OFF);
				(*ComRxIpduShortTimeoutCbk[l_loop])();	
			}
			if (Com_RxIpduRunInfo[l_loop].STimeoutCounter > 0)
			{
				Com_RxIpduRunInfo[l_loop].STimeoutCounter--;
			}
		}	
	}
#endif
}

void Com_MainFunctionTx(void)
{
	uint8 l_TxIpduId;
	PduIdType l_TransmitHnd = 0XFFFF;
	PduInfoType l_TxIpduInfo = {0u, 0u};

	if (COM_TX_ENABLE != (COM_TX_ENABLE & Com_GetStatus()))
	{
		return;
	}

#if(0 < COM_TX_IPDU_MAX_NUM)

	for (l_TxIpduId = 0; l_TxIpduId < COM_TX_IPDU_MAX_NUM; l_TxIpduId++)
	{
		//l_IpduId = COM_TX_IPDU_START_INDEX+l_TxIpduId;
		//before period timer to avoid this two condition happened at same time
		if (FALSE == Com_GetStartFlag(l_TxIpduId))
		{
			continue;
		}
		if (STD_ON == Com_GetTxDM(l_TxIpduId))
		{
			if (0 == Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter)
			{
				EnterCriticalCode();
				Com_SetTxDM(l_TxIpduId, STD_OFF);
				//Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;
				//if((STD_ON == Com_GetTxRepeatFlag(l_TxIpduId))
				// &&(Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter!=Com_TxSigConfInfo[l_TxIpduId].ComNumberOfRepetitions))
				//{

				Com_SetIpduTrigger(l_TxIpduId, STD_IDLE);
				Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
				ExitCriticalCode();
				//}
				if (NULL != ComIpduLongTimeoutCbk[l_TxIpduId + COM_TX_IPDU_START_INDEX])
				{
					(*ComIpduLongTimeoutCbk[l_TxIpduId + COM_TX_IPDU_START_INDEX])();
				}
			}
			if (0 < Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter)
			{
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter--;
			}
		}
		if (Com_GetTxOffsetCycle(l_TxIpduId) > 0)
		{
			Com_TxIpduRunInfo[l_TxIpduId].CycleOffset++;
		}
		if (Com_GetTxCyclic(l_TxIpduId) > 0)
		{
			if (0 == Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter)
			{
				Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter = Com_TxIpduConfInfo[l_TxIpduId].TimePeriod;
			}
			if (0 < Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter)
			{
				Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter--;
			}
		}
		if (Com_TxIpduRunInfo[l_TxIpduId].DelayTime == 0)
		{
			Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;

			if ((Com_GetTxStartEv(l_TxIpduId) > 0) || (Com_GetTxStartEvTrig(l_TxIpduId) > 0))
			{
				Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
				if (Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter == 1)
				{
					Com_SetTxStartEv(l_TxIpduId, STD_OFF);
					Com_SetTxStartEvTrig(l_TxIpduId, STD_OFF);
					Com_SetTxStartEvRep(l_TxIpduId, STD_OFF);
					if (COM_TX_RX_MODE_DIRECT == Com_TxIpduConfInfo[l_TxIpduId].TxMode)
					{
						Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
					}
					Com_SetTxOffsetCycle(l_TxIpduId, STD_ON);
					Com_TxIpduRunInfo[l_TxIpduId].CycleOffset = 0;
				}
				if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
				{
					if (STD_OFF == Com_GetTxDM(l_TxIpduId))
					{
						Com_SetTxDM(l_TxIpduId, STD_ON);
					}
				}
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;
			}
			if (Com_GetTxCyclic(l_TxIpduId) > 0)
			{
				Com_SetIpduTrigger(l_TxIpduId, STD_ACTIVE);
				Com_TxIpduRunInfo[l_TxIpduId].TimeoutCounter = Com_TxIpduConfInfo[l_TxIpduId].Timeout;
				
				if (Com_TxIpduConfInfo[l_TxIpduId].Timeout > 0)
				{
					if (STD_OFF == Com_GetTxDM(l_TxIpduId))
					{
						Com_SetTxDM(l_TxIpduId, STD_ON);
					}
				}
				if (Com_GetTxRepeatFlag(l_TxIpduId) == 0)
				{
					if (Com_TxIpduConfInfo[l_TxIpduId].TimePeriod >= Com_TxIpduConfInfo[l_TxIpduId].DelayTime )
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].TimePeriod;
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
					}
				}
				if (Com_GetTxLastInFast(l_TxIpduId) > 0)
				{
					if (Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter >= Com_TxIpduConfInfo[l_TxIpduId].DelayTime)
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter;
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
					}
				}
			}
			if (Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter > 1)
			{
				if (Com_TxIpduConfInfo[l_TxIpduId].DelayTime >= Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast)
				{
					Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
				}
				else
				{
					Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].CycleTimeFast;
				}
			}
			if ((COM_TX_RX_MODE_MIXED == Com_TxIpduConfInfo[l_TxIpduId].TxMode) && (Com_GetTxRepeatFlag(l_TxIpduId) > 0))
			{
				if ((Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter == 1))
				{
					if (Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter >= Com_TxIpduConfInfo[l_TxIpduId].DelayTime)
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduRunInfo[l_TxIpduId].PeriodCounter;
						Com_SetTxOffsetCycle(l_TxIpduId, STD_ON);
						Com_TxIpduRunInfo[l_TxIpduId].CycleOffset = 0;
						Com_SetTxLastInFast(l_TxIpduId, STD_OFF);
						Com_SetTxRepeatFlag(l_TxIpduId, STD_OFF);
					}
					else
					{
						Com_TxIpduRunInfo[l_TxIpduId].DelayTime = Com_TxIpduConfInfo[l_TxIpduId].DelayTime;
						Com_SetTxLastInFast(l_TxIpduId, STD_ON);
					}
				}
			}
		}
		if (Com_TxIpduRunInfo[l_TxIpduId].DelayTime > 0)
		{
			Com_TxIpduRunInfo[l_TxIpduId].DelayTime--;
		}
		if (Com_GetTxStartEvClr(l_TxIpduId) > 0)
		{
			if (Com_GetTxStartEvRep(l_TxIpduId) == 0)
			{
				Com_SetTxCyclic(l_TxIpduId, STD_OFF);
				Com_SetTxStartEv(l_TxIpduId, STD_OFF);
				Com_SetTxStartEvTrig(l_TxIpduId, STD_OFF);
			}
			else
			{
				if ((0 != Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions))
				{
					Com_TxIpduRunInfo[l_TxIpduId].RepeatCounter =  Com_TxIpduConfInfo[l_TxIpduId].NumberOfRepetitions;
				}
			}
			Com_SetTxStartEvClr(l_TxIpduId, STD_OFF);
		}
	}

	while (E_OK == Com_GetHighPriTrigIpdu(&l_TransmitHnd))//add wait timer
	{
		l_TxIpduInfo.SduDataPtr = (uint8 *)ComIpduBufPtr[l_TransmitHnd + COM_TX_IPDU_START_INDEX];
		l_TxIpduInfo.SduLength = Com_TxIpduConfInfo[l_TransmitHnd].PduLength;
		if (E_OK == CanIf_Transmit(Com_TxIpduConfInfo[l_TransmitHnd].CanIfPduId, &l_TxIpduInfo))
		{
			Com_SetIpduTrigger(l_TransmitHnd, FALSE);
			if (Com_TxIpduRunInfo[l_TransmitHnd].RepeatCounter > 0)
			{
				--Com_TxIpduRunInfo[l_TransmitHnd].RepeatCounter;
#if 0 //requirements for test to stop cycle transmit in mixed mode
				if (Com_TxIpduRunInfo[l_TransmitHnd].RepeatCounter == 0)
				{
					//Com_StopTxEvent(SIG_HND_GPSTimeMonth);
				}
#endif
			}
			if (Com_TxIpduConfInfo[l_TransmitHnd].NumberOfRepetitions == 0)
			{

				Com_SetTxOffsetCycle(l_TransmitHnd, STD_ON);
				Com_TxIpduRunInfo[l_TransmitHnd].CycleOffset = 0;
				Com_SetTxStartEv(l_TransmitHnd, STD_OFF);
				Com_SetTxStartEvTrig(l_TransmitHnd, STD_OFF);

			}

		}
		else
		{
			Com_TxIpduRunInfo[l_TransmitHnd].DelayTime = 0;
			break;
		}
	}
#endif
}
/*============================== END OF FILE: Com.c =============================*/


