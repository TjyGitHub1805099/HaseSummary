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
********************************************************************/
#ifdef COM_WIN32_DEBUG
#include <stdio.h>
#endif
#include <string.h>
#include "Com.h"
#ifndef COM_WIN32_DEBUG
#include "CanIf.h"
#include "System_Tick.h"
#endif
#include "CircularQueue.h"
#include "System_Portable.h"
#include "SimFlag.h"
#include "CircleList.h"
#include "trace.h"
#include "CanApp_Cbk.h"
#include "Com_Cbk.h"
#include "CanIf.h"
#include "Com_Cfg.h"
#include "main.h"

extern void Com_PduChangeCallback(PduIdType ipduId);
extern void Tp_SetTraceFlag(boolean flag);


#define COM_SHORT_PERIODS_TIMEOUT_VALUE 300	//ms
#define CAN_SIG_MAX_NUM_PER_MSG 64

/*****************************************************/
#ifdef COM_WIN32_DEBUG
extern int Sys_Tick;
uint32 System_GetTickValue(void) {
    return Sys_Tick;
}

uint8 CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr) {
    printf("Send Req:id-%d, Tick:%d\n", CanTxPduId, Sys_Tick);
    return 0;
}

/*****************************************************/
#endif

#define CAN_RX_CIRCLE_LIST_ITEM_LEN 14 // 2+4+8
#define CAN_RX_CIRCLEl_LIST_NUM	128
tCircleListItem gCanRxCircleListBuff[CAN_RX_CIRCLEl_LIST_NUM];
uint32 gLostCanMsgCounter = 0;

tCircleListInfor gCanRxCircleListInfor = {NULL,NULL,gCanRxCircleListBuff,CAN_RX_CIRCLE_LIST_ITEM_LEN};

volatile uint16 Com_IpduBufChangeQueueCh = CQUEUE_ERR_CHANNEL;



Com_ManagementType Com_Management = {COM_UNINIT};
uint16 Com_RxIpduQueueCh[COM_RX_IPDU_MAX_NUM];
uint16 Com_RxBufChangeQueue[COM_RX_IPDU_MAX_NUM];

#if(0 < COM_SIGNAL_MAX_NUM)
Com_SigRunType Com_SigRunInfo[COM_SIGNAL_MAX_NUM];
#endif

#if 0
#if(0 < COM_IPDU_MAX_NUM)
SimFlag_BufDef(Com_IpduStarted, COM_IPDU_MAX_NUM);
#define Com_GetStartFlag(ComIpduId) SimFlag_GetFlagState(Com_IpduStarted,ComIpduId)
#define Com_SetStartFlag(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduStarted,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_FirstDM, COM_IPDU_MAX_NUM);
#define Com_GetFirstDM(ComIpduId) SimFlag_GetFlagState(Com_FirstDM,ComIpduId)
#define Com_SetFirstDM(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_FirstDM,ComIpduId, TrueOrFalse)
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
uint16 Com_RxTempQueueCh = CQUEUE_ERR_CHANNEL;
Com_RxIpduRunType Com_RxIpduRunInfo[COM_RX_IPDU_MAX_NUM];

SimFlag_BufDef(Com_RxIpduDM, COM_RX_IPDU_MAX_NUM);
#define Com_GetRxDM(ComIpduId) SimFlag_GetFlagState(Com_RxIpduDM,ComIpduId)
#define Com_SetRxDM(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_RxIpduDM,ComIpduId, TrueOrFalse)

SimFlag_BufDef(Com_RxIpduDMChecked, COM_RX_IPDU_MAX_NUM);
#define Com_GetRxDMChecked(ComIpduId) SimFlag_GetFlagState(Com_RxIpduDMChecked,ComIpduId)
#define Com_SetRxDMChecked(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_RxIpduDMChecked,ComIpduId, TrueOrFalse)

volatile SimFlag_BufDef(Com_RxBufChange, COM_RX_IPDU_MAX_NUM);
#define Com_GetRxBufChange(ComIpduId) SimFlag_GetFlagState(Com_RxBufChange,ComIpduId)
#define Com_SetRxBufChange(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_RxBufChange,ComIpduId, TrueOrFalse)
#endif

#if(0 < COM_TX_IPDU_MAX_NUM)
volatile Com_TxIpduRunType Com_TxIpduRunInfo[COM_TX_IPDU_MAX_NUM];
uint16 Com_TxTrigQueueCh = CQUEUE_ERR_CHANNEL;
volatile uint16 Com_TxTrigQueue[COM_TX_IPDU_MAX_NUM];

SimFlag_BufDef(Com_IpduTxDMRun, COM_TX_IPDU_MAX_NUM);
#define Com_GetTxDMRun(ComIpduId) SimFlag_GetFlagState(Com_IpduTxDMRun,ComIpduId)
#define Com_SetTxDMRun(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_IpduTxDMRun,ComIpduId, TrueOrFalse)

volatile SimFlag_BufDef(Com_TxCyclicEvent, COM_TX_IPDU_MAX_NUM);
#define Com_GetTxCycleEvent(ComIpduId) SimFlag_GetFlagState(Com_TxCyclicEvent,ComIpduId)
#define Com_SetTxCycleEvent(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_TxCyclicEvent,ComIpduId, TrueOrFalse)

volatile SimFlag_BufDef(Com_TxTrigEvent, COM_TX_IPDU_MAX_NUM);
#define Com_GetTxTrigEvent(ComIpduId) SimFlag_GetFlagState(Com_TxTrigEvent,ComIpduId)
#define Com_SetTxTrigEvent(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_TxTrigEvent,ComIpduId, TrueOrFalse)

volatile SimFlag_BufDef(Com_TxOffsetTrig, COM_TX_IPDU_MAX_NUM);
#define Com_GetTxOffsetTrig(ComIpduId) SimFlag_GetFlagState(Com_TxOffsetTrig,ComIpduId)
#define Com_SetTxOffsetTrig(ComIpduId,TrueOrFalse) SimFlag_SetFlagState(Com_TxOffsetTrig,ComIpduId, TrueOrFalse)
#endif
#else
#if(0 < COM_IPDU_MAX_NUM)
tCom_SimGlobalFlag gCom_SimGlobalFlag[COM_IPDU_MAX_NUM];

#define Com_GetStartFlag(ComIpduId) (gCom_SimGlobalFlag[ComIpduId].startFlag)
#define Com_SetStartFlag(ComIpduId,TrueOrFalse) (gCom_SimGlobalFlag[ComIpduId].startFlag = TrueOrFalse)

#define Com_GetFirstDM(ComIpduId) (gCom_SimGlobalFlag[ComIpduId].firstDM)
#define Com_SetFirstDM(ComIpduId,TrueOrFalse) (gCom_SimGlobalFlag[ComIpduId].firstDM = TrueOrFalse)
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
boolean Com_RxShortPeriodsTimeoutFlag[COM_RX_IPDU_MAX_NUM] = {0};
uint16 Com_RxTempQueueCh = CQUEUE_ERR_CHANNEL;
Com_RxIpduRunType Com_RxIpduRunInfo[COM_RX_IPDU_MAX_NUM];
tCom_SimRxFlag gCom_SimRxFlag[COM_RX_IPDU_MAX_NUM];

#define Com_GetRxDM(ComIpduId) (gCom_SimRxFlag[ComIpduId].rxDM)
#define Com_SetRxDM(ComIpduId,TrueOrFalse) (gCom_SimRxFlag[ComIpduId].rxDM = TrueOrFalse)

#define Com_GetRxDMChecked(ComIpduId) (gCom_SimRxFlag[ComIpduId].rxDMChecked)
#define Com_SetRxDMChecked(ComIpduId,TrueOrFalse) (gCom_SimRxFlag[ComIpduId].rxDMChecked = TrueOrFalse)

#define Com_GetRxBufChange(ComIpduId) (gCom_SimRxFlag[ComIpduId].rxBufChange)
#define Com_SetRxBufChange(ComIpduId,TrueOrFalse) (gCom_SimRxFlag[ComIpduId].rxBufChange = TrueOrFalse)
#endif

#if(0 < COM_TX_IPDU_MAX_NUM)
volatile Com_TxIpduRunType Com_TxIpduRunInfo[COM_TX_IPDU_MAX_NUM];
uint16 Com_TxTrigQueueCh = CQUEUE_ERR_CHANNEL;
volatile uint16 Com_TxTrigQueue[COM_TX_IPDU_MAX_NUM];
tCom_SimTxFlag gCom_SimTxFlag[COM_TX_IPDU_MAX_NUM];

#define Com_GetTxDMRun(ComIpduId) (gCom_SimTxFlag[ComIpduId].txDMRun)
#define Com_SetTxDMRun(ComIpduId,TrueOrFalse) (gCom_SimTxFlag[ComIpduId].txDMRun = TrueOrFalse)

#define Com_GetTxCycleEvent(ComIpduId) (gCom_SimTxFlag[ComIpduId].txCycleEvent)
#define Com_SetTxCycleEvent(ComIpduId,TrueOrFalse) (gCom_SimTxFlag[ComIpduId].txCycleEvent = TrueOrFalse)

#define Com_GetTxTrigEvent(ComIpduId) (gCom_SimTxFlag[ComIpduId].txTrigEvent)
#define Com_SetTxTrigEvent(ComIpduId,TrueOrFalse) (gCom_SimTxFlag[ComIpduId].txTrigEvent = TrueOrFalse)

#define Com_GetTxOffsetTrig(ComIpduId) (gCom_SimTxFlag[ComIpduId].txOffsetTrig)
#define Com_SetTxOffsetTrig(ComIpduId,TrueOrFalse) (gCom_SimTxFlag[ComIpduId].txOffsetTrig = TrueOrFalse)
#endif

#endif

static uint32 gComTxSigChgFlag[COM_TX_IPDU_MAX_NUM][2];

const static uint8 ByteHighBitsMask[8] = {
    1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u
};

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

static boolean Com_Inner_OpaqueTypeSignal(uint16 SignalId, uint8 GetOrSet, uint8 *SrcBuffer, uint8 *Value)
{
    uint8 startByte = 0u, stopByte = 0u;
    uint8 loop = 0u;
    boolean returnValue = E_OK;

    if (32 >= Com_SigConfInfo[SignalId].Length) {
        returnValue = E_NOT_OK;
    } else {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        stopByte = startByte + Com_SigConfInfo[SignalId].TypeUint8NBytes - 1;

        for (loop = startByte; loop <= stopByte; loop++) {
            if (COM_GET_SIG_VALUE == GetOrSet) {
                *((uint8 *)Value + loop) = *(SrcBuffer + loop);
            } else {
                *(SrcBuffer + loop) = *((uint8 *)Value + loop);
            }
        }
    }
    return (returnValue);
}


static boolean Com_Inner_GetIntelTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{   //Little endian
    uint8 startByte = 0u, startByteBit = 0u;
    uint8 stopByte = 0u, stopByteBit = 0u;
    uint8 firstByteActiveBit = 0u;
    uint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByte = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) >> 3;
    stopByteBit = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) & 7;
    if (startByte == stopByte) {
        getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[stopByteBit]) >> (startByteBit);
    } else {
        firstByteActiveBit = 8 - startByteBit;
        for (loop = startByte; loop <= stopByte; loop++) {
            if (loop == startByte) {
                getValue = *(SrcBuffer + loop) >> (startByteBit);
            } else if (loop == stopByte) {
                getValue +=  (*(SrcBuffer + loop) & ByteHighBitsMask[stopByteBit]) << (((stopByte - startByte - 1) << 3) + firstByteActiveBit);
            } else {
                getValue += *(SrcBuffer + loop) << (((loop - startByte - 1) << 3) + firstByteActiveBit);
            }
        }
    }
    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType)) {
        if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1))) {
            getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
        }
    }
    *((uint32 *)GetValue) = (uint32)getValue;
    return (E_OK);
}

static boolean Com_Inner_SetIntelTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{
    uint8 startByte = 0u, startByteBit = 0u;
    uint8 stopByte = 0u, stopByteBit = 0u;
    uint8 firstByteActiveBit = 0u;
    uint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByte = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) >> 3;
    stopByteBit = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) & 7;
    if (startByte == stopByte) {
        *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[stopByteBit - startByteBit] << startByteBit));
        *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << startByteBit);
    } else {
        firstByteActiveBit = 8 - startByteBit;
        for (loop = startByte; loop <= stopByte; loop++) {
            if (loop == startByte) {
                *(SrcBuffer + loop) &= (uint8)((1 << startByteBit) - 1);
                *(SrcBuffer + loop) |= (uint8)((newValue & ((1 << (firstByteActiveBit)) - 1)) << startByteBit);
                newValue = newValue >> firstByteActiveBit;
            } else if (loop == stopByte) {
                *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[stopByteBit]));
                *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[stopByteBit]);
            } else {
                *(SrcBuffer + loop) = (uint8)(newValue & 255);
                newValue = newValue >> 8;
            }
        }
    }
    return (E_OK);
}

#ifndef COM_GENCODE_FROM_XML
static boolean Com_Inner_GetMotorTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{
    uint8 startByte = 0u, startByteBit = 0u;
    uint8 stopByte = 0u, stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByteBit = (startByteBit + 9 - (Com_SigConfInfo[SignalId].Length & 7)) & 7;
    stopByte = startByte + ((Com_SigConfInfo[SignalId].Length + 6 - startByteBit) >> 3);

    if (startByte == stopByte) {
        getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[startByteBit]) >> stopByteBit;
    } else {
        lastByteActiveBit = 8 - stopByteBit;
        for (loop = stopByte; loop >= startByte; loop--) {
            if (loop == stopByte) {
                getValue = *(SrcBuffer + loop) >> stopByteBit;
            } else if (loop == startByte) {
                getValue += (*(SrcBuffer + loop) & ByteHighBitsMask[startByteBit]) << (((stopByte - startByte - 1) << 3) + lastByteActiveBit);
            } else {
                getValue += *(SrcBuffer + loop) << (((stopByte - loop - 1) << 3) + lastByteActiveBit);
            }
        }
    }

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType)) {
        if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1))) {
            getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
        }
    }
    *((uint32 *)GetValue) = (uint32)getValue;
    return (E_OK);
}

static boolean Com_Inner_SetMotorTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{
    uint8 startByte = 0u, startByteBit = 0u;
    uint8 stopByte = 0u, stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByteBit = (startByteBit + 9 - (Com_SigConfInfo[SignalId].Length & 7)) & 7;
    stopByte = startByte + ((Com_SigConfInfo[SignalId].Length + 6 - startByteBit) >> 3);

    if (startByte == stopByte) {
        *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[startByteBit - stopByteBit] << stopByteBit));
        *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << stopByteBit);
    } else {
        lastByteActiveBit = 8 - stopByteBit;
        for (loop = stopByte; loop >= startByte; loop--) {
            if (loop == stopByte) {
                *(SrcBuffer + loop) &= (uint8)((1 << stopByteBit) - 1);
                *(SrcBuffer + loop) |= (uint8)((newValue & ((1 << lastByteActiveBit) - 1)) << stopByteBit);
                newValue = newValue >> lastByteActiveBit;
            } else if (loop == startByte) {
                *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[startByteBit]));
                *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[startByteBit]);
            } else {
                *(SrcBuffer + loop) = (uint8)(newValue & 255);
                newValue = newValue >> 8;
            }
        }
    }
    return (E_OK);
}
#else
static boolean Com_Inner_GetMotorTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{   //Big endian
    uint8 startByte = 0u，startByteBit = 0u;
    uint8 stopByte = 0u，stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    } else {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
        stopByte = startByte - ((Com_SigConfInfo[SignalId].Length + startByteBit - 1) >> 3);
        stopByteBit = (startByteBit + Com_SigConfInfo[SignalId].Length - 1) & 7;

        if (startByte == stopByte) {
            getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[stopByteBit]) >> startByteBit;
        } else {
            lastByteActiveBit = 8 - startByteBit;
            for (loop = startByte ; loop >= stopByte; loop--) {
                if (loop == startByte) {
                    getValue = *(SrcBuffer + loop) >> startByteBit;
                } else if (loop == stopByte) {
                    getValue += (*(SrcBuffer + loop) & ByteHighBitsMask[stopByteBit]) << (((startByte - loop - 1) << 3) + lastByteActiveBit);
                } else {
                    getValue += *(SrcBuffer + loop) << (((startByte - loop - 1) << 3) + lastByteActiveBit);
                }
            }
        }
        if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
                || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
                || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType)) {
            if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1))) {
                getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
            }
        }
        *((uint32 *)GetValue) = (uint32)getValue;
    }
    return (E_OK);
}

static boolean Com_Inner_SetMotorTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{   //Big endian
    uint8 startByte = 0u，startByteBit = 0u;
    uint8 stopByte = 0u，stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)) {
        return (E_NOT_OK);
    } else {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
        stopByte = startByte - ((Com_SigConfInfo[SignalId].Length + startByteBit - 1) >> 3);
        stopByteBit = (startByteBit + Com_SigConfInfo[SignalId].Length - 1) & 7;

        if (startByte == stopByte) {
            *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[stopByteBit - startByteBit] << startByteBit));
            *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << startByteBit);
        } else {
            lastByteActiveBit = 8 - startByteBit;
            for (loop = startByte ; loop >= stopByte; loop--) {
                if (loop == startByte) {
                    *(SrcBuffer + loop) &= (uint8)((1 << startByteBit) - 1);
                    *(SrcBuffer + loop) |= ((newValue & ByteHighBitsMask[lastByteActiveBit - 1]) << startByteBit);
                    newValue = newValue >> lastByteActiveBit;
                } else if (loop == stopByte) {
                    *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[stopByteBit]));
                    *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[stopByteBit]);
                } else {
                    *(SrcBuffer + loop) = (uint8)(newValue & 255);
                    newValue = newValue >> 8;
                }
            }
        }
    }
    return (E_OK);
}
#endif

static boolean Com_SigFilter_Type1(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_ALWAYS
    SignalId = SignalId;
    OldValue = OldValue;
    NewValue = NewValue;
    return (TRUE);
}

static boolean Com_SigFilter_Type2(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEVER
    SignalId = SignalId;
    OldValue = OldValue;
    NewValue = NewValue;
    return (FALSE);
}

static boolean Com_SigFilter_Type3(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_EQUALS_X
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.FilterX)
            == (sint32)(NewValue & Com_SigConfInfo[SignalId].Filter.Mask)) {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type4(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_DIFFERS_X
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.FilterX)
            != (sint32)(NewValue & Com_SigConfInfo[SignalId].Filter.Mask)) {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type5(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_DIFFERS_MASKED_OLD
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.Mask & OldValue)
            != (NewValue & Com_SigConfInfo[SignalId].Filter.Mask)) {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type6(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_WITHIN   not suilt for boolean type
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType)) {
        if ((Com_SigConfInfo[SignalId].Filter.Min < (sint32)NewValue)
                || ((sint32)NewValue < Com_SigConfInfo[SignalId].Filter.Max)) {
            l_ReturnValue = TRUE;
        }
    } else if (COM_SIG_BOOLEAN != Com_SigConfInfo[SignalId].SignalType) {
        if ((Com_SigConfInfo[SignalId].Filter.Min < NewValue)
                || (NewValue < Com_SigConfInfo[SignalId].Filter.Max)) {
            l_ReturnValue = TRUE;
        }
    } else {
        //do nothing,to match MISRA C rules
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type7(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_OUTSIDE   not suilt for boolean type
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType)) {
        if (((sint32)NewValue < Com_SigConfInfo[SignalId].Filter.Min)
                || (Com_SigConfInfo[SignalId].Filter.Max < (sint32)NewValue)) {
            l_ReturnValue = TRUE;
        }
    } else if (COM_SIG_BOOLEAN != Com_SigConfInfo[SignalId].SignalType) {
        if ((NewValue < Com_SigConfInfo[SignalId].Filter.Min)
                || (Com_SigConfInfo[SignalId].Filter.Max < NewValue)) {
            l_ReturnValue = TRUE;
        }
    } else {
        //do nothing,to match MISRA C rules
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type8(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_ONE_EVERY_N
    boolean l_ReturnValue = FALSE;

    Com_SigRunInfo[SignalId].Filter.Occurence++;
    if (Com_SigRunInfo[SignalId].Filter.Occurence == Com_SigConfInfo[SignalId].Filter.Offset) {
        l_ReturnValue = TRUE;
    }
    if (Com_SigRunInfo[SignalId].Filter.Occurence == Com_SigConfInfo[SignalId].Filter.Period) {
        Com_SigRunInfo[SignalId].Filter.Occurence = 0;
    }
    return (l_ReturnValue);
}

static boolean Com_SigFilter_Type9(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_DIFFER_VALUE
    boolean l_ReturnValue = FALSE;
	uint32 deltaValue = 0;
	
	deltaValue = (OldValue>NewValue) ? OldValue-NewValue : NewValue-OldValue;
    if ((deltaValue & Com_SigConfInfo[SignalId].Filter.Mask) >= (Com_SigConfInfo[SignalId].Filter.FilterX)) {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}


static boolean (*Com_GetSigProcess[])(uint16 SignalId, uint8* SrcBuffer, uint32 *GetValue) =
{
    Com_Inner_GetMotorTypeSignal, Com_Inner_GetIntelTypeSignal
};

static boolean (*Com_SetSigProcess[])(uint16 SignalId, uint8* SrcBuffer, uint32 SetValue) =
{
    Com_Inner_SetMotorTypeSignal, Com_Inner_SetIntelTypeSignal
};

static boolean (*Com_SigFilter[])(uint16 SignalId, uint32 OldValue, uint32 NewValue) =
{
    Com_SigFilter_Type1,//COM_F_ALWAYS,
    Com_SigFilter_Type2,//COM_F_NEVER
    Com_SigFilter_Type3,//COM_F_MASKED_NEW_EQUALS_X
    Com_SigFilter_Type4,//COM_F_MASKED_NEW_DIFFERS_X
    Com_SigFilter_Type5,//COM_F_MASKED_NEW_DIFFERS_MASKED_OLD
    Com_SigFilter_Type6,//COM_F_NEW_IS_WITHIN
    Com_SigFilter_Type7,//COM_F_NEW_IS_OUTSIDE
    Com_SigFilter_Type8, //COM_F_ONE_EVERY_N
    Com_SigFilter_Type9, //COM_F_DIFFER_VALUE
};

static void Com_Inner_RxTimeoutAction(PduIdType pduId, uint32 time)
{
    uint16 l_RxSigLoop = 0u;
    uint8 l_tempClearBuf[8] = {COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING,
                               COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING, COM_NOTUSED_BUF_PADDING
                              };

    for (l_RxSigLoop = Com_IpduId2SigId[pduId][0]; l_RxSigLoop <= Com_IpduId2SigId[pduId][1]; l_RxSigLoop++) {
        if ((32 < Com_SigConfInfo[l_RxSigLoop].Length)
                || (COM_SIG_UINT8_N == Com_SigConfInfo[l_RxSigLoop].SignalType)
                || (COM_SIG_OPAQUE == Com_SigConfInfo[l_RxSigLoop].EndianType)) {
            if (COM_RXTIMEOUTACTION_NONE != Com_SigConfInfo[l_RxSigLoop].RxTimeoutAction) {
                Com_Inner_OpaqueTypeSignal(l_RxSigLoop, COM_SET_SIG_VALUE, Com_IpduBufPtr[pduId], l_tempClearBuf);
                Com_RxIpdu_TickBuf[pduId] = time;
            }
        } else {
            if (COM_RXTIMEOUTACTION_REPLACE == Com_SigConfInfo[l_RxSigLoop].RxTimeoutAction) {
                (*Com_SetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[pduId], Com_SigConfInfo[l_RxSigLoop].InitValue);
                Com_RxIpdu_TickBuf[pduId] = time;
            } else if (COM_RXTIMEOUTACTION_SUBSTITUTE == Com_SigConfInfo[l_RxSigLoop].RxTimeoutAction) {
                (*Com_SetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[pduId], Com_SigConfInfo[l_RxSigLoop].SubstitutionValue);
                Com_RxIpdu_TickBuf[pduId] = time;
            }
        }
    }
}
static boolean checkIpduChange(uint8 *msgDataPtr,PduIdType RxPduId)
{
	uint8 i = 0,l_validDataLen = 8;
	boolean l_ret_IpduMsgChange = FALSE;
	if((NULL != msgDataPtr) && (RxPduId < COM_RX_IPDU_MAX_NUM))
	{
		switch(RxPduId)
			{
				/*************************************************************************
				   this code to filter out checksum and counter signal from following msgs.
				   requested by LiZhenjun,added by zhaozhuang @2019-12-10
				*************************************************************************/
				
				//DLC:5 checksum and conter at byte 4
				case IPDU_HND_LWSOUT_M2:
					l_validDataLen = 4;
					break;
				
				//following msgs DLC:8 checksum and conter at byte 6 and 7
				case IPDU_HND_Front_Wheel_Speed_Frame:
				case IPDU_HND_ITS_InfoStatus_N1:
				case IPDU_HND_ITS_InfoStatus_N2:
				case IPDU_HND_SRR_Left:
				case IPDU_HND_SRR_Right:
				case IPDU_HND_ITS_InfoStatus_N3_7b:
				case IPDU_HND_ITS_InfoStatus_N2_1:
					l_validDataLen = 6;
					break;
		
				default:
					l_validDataLen = 8;
					break;
		
			}
			/*checking is changed or not*/
			for(i=0;i<l_validDataLen;i++)
			{
				if(msgDataPtr[i] != Com_IpduBufPtr[RxPduId][i])
				{
					l_ret_IpduMsgChange = TRUE;
					break;
				}
			}

	}
	return l_ret_IpduMsgChange;
}
static void Com_Inner_RxProcess(void)
{
    uint8 l_msgDataPtr[64];
    boolean l_FilterResult = FALSE, l_bufChange = FALSE,l_AlgorithmNeed = FALSE;
    PduIdType l_RxPduId = 0xFFFFu;
    uint32 l_oldValue = 0u, l_newValue = 0u;
    uint32 l_sysTick = 0u;
    uint32 l_pduLen = 0u;
	uint16 l_RxSigLoop = 0u;
	uint16 l_queueIndex = CQUEUE_ERR_PUSHPOP;
	uint8 rxTmpBuff[CAN_RX_CIRCLE_LIST_ITEM_LEN] = {0};
	uint8 i = 0;
	uint8 l_circleListPopCount = 0;

	while(isCircleListEmpty(&gCanRxCircleListInfor) == FALSE)
	{
		//add by tjy,if pop count lagger than CAN_RX_CIRCLEl_LIST_NUM ,return
		if(++l_circleListPopCount > CAN_RX_CIRCLEl_LIST_NUM )
			return;
		
		if(circleListPop(&gCanRxCircleListInfor,rxTmpBuff,CAN_RX_CIRCLE_LIST_ITEM_LEN) == FALSE)
			return;
		
		l_sysTick = (uint32)(rxTmpBuff[2]<<24|rxTmpBuff[3]<<16|rxTmpBuff[4]<<8|rxTmpBuff[5]);
		memcpy(l_msgDataPtr, &rxTmpBuff[6], 8);
		l_RxPduId = (uint16)(rxTmpBuff[0]<<8 | rxTmpBuff[1]);
        if ((COM_TX_RX_MODE_DIRECT != Com_RxIpduConfInfo[l_RxPduId].RxMode)
                && (0 != Com_RxIpduConfInfo[l_RxPduId].Timeout)) {
            if (STD_ON == Com_GetRxDM(l_RxPduId)) {
				if(l_sysTick >0){
	                if ((0 == Com_RxIpduConfInfo[l_RxPduId].FirstTimeout) ) {
						
						//if ((l_sysTick - Com_RxIpduRunInfo[l_RxPduId].Timeout) > l_shortPeriodTimeoutValue) {
						if ((l_sysTick - Com_RxIpduRunInfo[l_RxPduId].Timeout) >= COM_SHORT_PERIODS_TIMEOUT_VALUE) {
							//if(Com_RxShortPeriodsTimeoutFlag[l_RxPduId] == FALSE){
								//Com_RxShortPeriodsTimeoutFlag[l_RxPduId] = TRUE;
								if (NULL != Com_IpduErrorCbk[l_RxPduId]) {
									(*Com_IpduErrorCbk[l_RxPduId])(COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS);
								}
							//}
						}

			    		if ((l_sysTick - Com_RxIpduRunInfo[l_RxPduId].Timeout) >= Com_RxIpduConfInfo[l_RxPduId].Timeout) {
							Com_Inner_RxTimeoutAction(l_RxPduId, l_sysTick);
							if (NULL != Com_IpduErrorCbk[l_RxPduId]) {
								(*Com_IpduErrorCbk[l_RxPduId])(COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS);
							}
			#ifdef COM_WIN32_DEBUG
							printf("RxTimeout:%d %d\n", l_RxPduId, l_sysTick);
			#endif
						}
	                } else if(STD_ON == Com_GetFirstDM(l_RxPduId)){
	                    Com_SetFirstDM(l_RxPduId, STD_OFF);
	                    if ((l_sysTick - Com_RxIpduRunInfo[l_RxPduId].Timeout) >= Com_RxIpduConfInfo[l_RxPduId].FirstTimeout) {
	                        Com_Inner_RxTimeoutAction(l_RxPduId, l_sysTick);
	                        if (NULL != Com_IpduErrorCbk[l_RxPduId]) {
	                            (*Com_IpduErrorCbk[l_RxPduId])(COM_TXRX_CBK_IND_NONE);
	                        }
#ifdef COM_WIN32_DEBUG
	                        printf("RxFirstTimeout:%d %d\n", l_RxPduId, l_sysTick);
#endif
	                    }
	                }
				}
	            Com_RxIpduRunInfo[l_RxPduId].Timeout = l_sysTick;
	            Com_SetRxDMChecked(l_RxPduId, STD_ON);
        	}
			else {
				Com_RxIpduRunInfo[l_RxPduId].Timeout = l_sysTick;
        	}
        }

//sync N591 logic ,add by tjy @2019-11-20
#if 1
 		//1st:find if not changed       
		l_bufChange = FALSE;
		l_bufChange = checkIpduChange(l_msgDataPtr,l_RxPduId);		
		if( TRUE == l_bufChange )
		{
			l_bufChange = FALSE;
			#if 0//add by tjy @2019-1-8
			l_AlgorithmNeed = FALSE;
			
			//2nd:find if need l_Algorith
			for (l_RxSigLoop = Com_IpduId2SigId[l_RxPduId][0]; l_RxSigLoop <= Com_IpduId2SigId[l_RxPduId][1]; l_RxSigLoop++)
			{
				if( COM_F_MASKED_NEW_DIFFERS_MASKED_OLD != Com_SigConfInfo[l_RxSigLoop].Filter.Algorithm )//add by tjy ,if Algorithm no affect
				{
					l_AlgorithmNeed = TRUE;
					break;
				}
			}
			#else
			l_AlgorithmNeed = TRUE;//avoid some not defined single changed in IT-MASTER ,IOC send to SOC.
			#endif
			if(TRUE == l_AlgorithmNeed)
			{
				for (l_RxSigLoop = Com_IpduId2SigId[l_RxPduId][0]; l_RxSigLoop <= Com_IpduId2SigId[l_RxPduId][1]; l_RxSigLoop++) {
					if ((32 < Com_SigConfInfo[l_RxSigLoop].Length)
							|| (COM_SIG_UINT8_N == Com_SigConfInfo[l_RxSigLoop].SignalType)
							|| (COM_SIG_OPAQUE == Com_SigConfInfo[l_RxSigLoop].EndianType)) {
						Com_Inner_OpaqueTypeSignal(l_RxSigLoop, COM_SET_SIG_VALUE, Com_IpduBufPtr[l_RxPduId], l_msgDataPtr);
						Com_RxIpdu_TickBuf[l_RxPduId] = l_sysTick;
						l_bufChange = TRUE;
					} else {
#if (STD_ON == USE_RX_SIG_FILTER)
						(*Com_GetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[l_RxPduId], &l_oldValue);
						(*Com_GetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, l_msgDataPtr, &l_newValue);
						l_FilterResult = (*Com_SigFilter[Com_SigConfInfo[l_RxSigLoop].Filter.Algorithm])(l_RxSigLoop, l_oldValue, l_newValue);
						if (TRUE == l_FilterResult)
#endif
						{
							(*Com_SetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[l_RxPduId], l_newValue);
							Com_RxIpdu_TickBuf[l_RxPduId] = l_sysTick;
							l_bufChange = TRUE;
						}
					}
				}
			}
			else
			{
				l_bufChange = TRUE;
				Com_RxIpdu_TickBuf[l_RxPduId] = l_sysTick;
				for(i=0;i<8;i++)
				{
					Com_IpduBufPtr[l_RxPduId][i] = l_msgDataPtr[i];
				}
			}
		}
#else
		l_bufChange = FALSE;
        for (l_RxSigLoop = Com_IpduId2SigId[l_RxPduId][0]; l_RxSigLoop <= Com_IpduId2SigId[l_RxPduId][1]; l_RxSigLoop++) {
            if ((32 < Com_SigConfInfo[l_RxSigLoop].Length)
                    || (COM_SIG_UINT8_N == Com_SigConfInfo[l_RxSigLoop].SignalType)
                    || (COM_SIG_OPAQUE == Com_SigConfInfo[l_RxSigLoop].EndianType)) {
                Com_Inner_OpaqueTypeSignal(l_RxSigLoop, COM_SET_SIG_VALUE, Com_IpduBufPtr[l_RxPduId], l_msgDataPtr);
                Com_RxIpdu_TickBuf[l_RxPduId] = l_sysTick;
                l_bufChange = TRUE;
            } else {
#if (STD_ON == USE_RX_SIG_FILTER)
                (*Com_GetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[l_RxPduId], &l_oldValue);
                (*Com_GetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, l_msgDataPtr, &l_newValue);
                l_FilterResult = (*Com_SigFilter[Com_SigConfInfo[l_RxSigLoop].Filter.Algorithm])(l_RxSigLoop, l_oldValue, l_newValue);
                if (TRUE == l_FilterResult)
#endif
                {
                    (*Com_SetSigProcess[Com_SigConfInfo[l_RxSigLoop].EndianType])(l_RxSigLoop, Com_IpduBufPtr[l_RxPduId], l_newValue);
                    Com_RxIpdu_TickBuf[l_RxPduId] = l_sysTick;
                    l_bufChange = TRUE;
                }
            }
        }
#endif

        if (TRUE == l_bufChange) {
			Com_PudChangeCallback(l_RxPduId);
            if (FALSE == Com_GetRxBufChange(l_RxPduId)) {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                CQueue_Push(Com_IpduBufChangeQueueCh, &l_RxPduId, 1, STD_OFF);
#else
                CQueue_Push(Com_IpduBufChangeQueueCh, &l_RxPduId, 1);
#endif
                Com_SetRxBufChange(l_RxPduId, TRUE);
            }
            if (0 != Com_RxIpduConfInfo[l_RxPduId].QueueDepth) {
                l_pduLen = Com_RxIpduConfInfo[l_RxPduId].PduLength;
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                l_queueIndex = CQueue_Push(Com_RxIpduQueueCh[l_RxPduId], &l_sysTick, 1, STD_OFF);
#else
                l_queueIndex = CQueue_Push(Com_RxIpduQueueCh[l_RxPduId], &l_sysTick, 1);
#endif
                if ((Com_RxIpduConfInfo[l_RxPduId].QueueDepth > l_queueIndex) && (NULL != Com_RxIpduQueuePtr[l_RxPduId])) {
                    memcpy(Com_RxIpduQueuePtr[l_RxPduId] + l_queueIndex * l_pduLen, l_msgDataPtr, l_pduLen);
                }
            }
        }
        if (NULL != Com_IpduInformCbk[l_RxPduId]) {
            (*Com_IpduInformCbk[l_RxPduId])(COM_TXRX_CBK_IND_NONE);
        }
	}
}

static void Com_Inner_RxTimeoutProcess(void)
{
    uint16 l_rxIpduLoop = 0u;
    uint32 l_sysTick = 0u;
	//uint16 l_shortPeriodTimeoutValue = 0;

    for (l_rxIpduLoop = 0; l_rxIpduLoop < Com_RxIpduMaxNum; l_rxIpduLoop++) {
        if ((FALSE == Com_GetStartFlag(l_rxIpduLoop)) || (STD_OFF == Com_GetRxDM(l_rxIpduLoop))) {
            continue;
        }
        if ((COM_TX_RX_MODE_DIRECT != Com_RxIpduConfInfo[l_rxIpduLoop].RxMode)
                && (0 != Com_RxIpduConfInfo[l_rxIpduLoop].Timeout)) {
            if (STD_OFF == Com_GetRxDMChecked(l_rxIpduLoop)) {
                l_sysTick = System_GetTickValue();
				if(l_sysTick >0){
	                if ((0 == Com_RxIpduConfInfo[l_rxIpduLoop].FirstTimeout) ) {
						
						if ((l_sysTick - Com_RxIpduRunInfo[l_rxIpduLoop].Timeout) >= COM_SHORT_PERIODS_TIMEOUT_VALUE) {
	                        if (NULL != Com_IpduErrorCbk[l_rxIpduLoop]) {
	                            (*Com_IpduErrorCbk[l_rxIpduLoop])(COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS);
	                        }
	                    }

						if ((l_sysTick - Com_RxIpduRunInfo[l_rxIpduLoop].Timeout) >= Com_RxIpduConfInfo[l_rxIpduLoop].Timeout) {
	                        Com_RxIpduRunInfo[l_rxIpduLoop].Timeout = l_sysTick;
	                        Com_Inner_RxTimeoutAction(l_rxIpduLoop, l_sysTick);
	                        if (NULL != Com_IpduErrorCbk[l_rxIpduLoop]) {
	                            (*Com_IpduErrorCbk[l_rxIpduLoop])(COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS);
	                        }
	                    }
	                } 
	                else if(STD_ON == Com_GetFirstDM(l_rxIpduLoop)){
						if ((l_sysTick - Com_RxIpduRunInfo[l_rxIpduLoop].Timeout) >= Com_RxIpduConfInfo[l_rxIpduLoop].FirstTimeout) {
							Com_RxIpduRunInfo[l_rxIpduLoop].Timeout = l_sysTick;
							Com_SetFirstDM(l_rxIpduLoop, STD_OFF);
							Com_Inner_RxTimeoutAction(l_rxIpduLoop, l_sysTick);
							if (NULL != Com_IpduErrorCbk[l_rxIpduLoop]) {
								(*Com_IpduErrorCbk[l_rxIpduLoop])(COM_TXRX_CBK_IND_NONE);
							}
						}
						
	                }
				}
            }
            Com_SetRxDMChecked(l_rxIpduLoop, STD_OFF);
        }
    }
}


static void Com_Inner_TxTrigProcess(void)
{
    uint16 l_txPduId;
    PduInfoType l_TxIpduInfo = {0u, 0u};
    uint32 l_sysTick = 0;
    uint16 l_TrigLen = 0u, l_TrigPduId = Com_TxIpduMaxNum;
	boolean txTriggerTye = CanAppTxDefaultValue;
	uint16 sigStartIndex = 0;
	uint16 sigEndIndex = 0;
	uint16 sigOffSet = 0;
	uint8 sigSumNum = 0;

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
    if (E_OK == CQueue_GetLen(Com_TxTrigQueueCh, &l_TrigLen, STD_OFF))
#else
    if (E_OK == CQueue_GetLen(Com_TxTrigQueueCh, &l_TrigLen))
#endif
    {
        while ( 0 < l_TrigLen)
        {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
            if ( CQUEUE_ERR_PUSHPOP != CQueue_Pop(Com_TxTrigQueueCh, &l_TrigPduId, 1, STD_OFF))
#else
            if ( CQUEUE_ERR_PUSHPOP != CQueue_Pop(Com_TxTrigQueueCh, &l_TrigPduId, 1))
#endif
            {
                l_txPduId = l_TrigPduId - Com_TxIpduStartIndex;
                if ((l_txPduId >= Com_TxIpduMaxNum) || (FALSE == Com_GetStartFlag(l_TrigPduId))) {
                    continue;
                }
                l_sysTick = System_GetTickValue();
                if (Com_TxIpduRunInfo[l_txPduId].PreSendTime > l_sysTick) {
                    //system error, consider re-transmit immediatly
                    Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_MAINFUNCTIONTX, COM_E_TIME);
                } else {
                	
                    if (Com_TxIpduRunInfo[l_txPduId].RepeatCounter == Com_TxIpduConfInfo[l_txPduId].NumberOfRepetitions) {
                        if ((l_sysTick - Com_TxIpduRunInfo[l_txPduId].PreSendTime) >= Com_TxIpduConfInfo[l_txPduId].MinimumDelayTime) {

							TracePrintf(COM_TRACE,TRACE_INFO,"%d,trigger canId:0x%x\r\n",Com_TxIpduRunInfo[l_txPduId].RepeatCounter,l_txPduId);
                            l_TxIpduInfo.SduDataPtr = (uint8 *)Com_IpduBufPtr[l_TrigPduId];
                            l_TxIpduInfo.SduLength = Com_TxIpduConfInfo[l_txPduId].PduLength;
                            if(CanIf_Transmit(Com_TxIpduConfInfo[l_txPduId].CanIfPduId, &l_TxIpduInfo) == E_OK)
                            {
                            	Com_TxIpduRunInfo[l_txPduId].PreSendTime = l_sysTick;
								if (0 < Com_TxIpduRunInfo[l_txPduId].RepeatCounter) {
                                	Com_TxIpduRunInfo[l_txPduId].RepeatCounter--;
                            	}
							}
							
                            if (0 == Com_TxIpduRunInfo[l_txPduId].RepeatCounter) {
                                Com_SetTxTrigEvent(l_txPduId, STD_OFF);

						#if 1
								sigStartIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][0];
								sigEndIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][1];
								sigSumNum = sigEndIndex-sigStartIndex+1;
								if(sigSumNum<=CAN_SIG_MAX_NUM_PER_MSG)
								{
									for(sigOffSet = 0;sigOffSet<sigSumNum;sigOffSet++)
									{
										if(gComTxSigChgFlag[l_txPduId][sigOffSet/32] & (1<<(sigOffSet%32)))
										{
											if (CanApp_Tx_Matrix[sigStartIndex+sigOffSet-COM_TX_SIGNAL_START_INDEX] != NULL) {
												CanApp_Tx_Matrix[sigStartIndex+sigOffSet-COM_TX_SIGNAL_START_INDEX](0, &txTriggerTye);
											}
										}
										
									}
								}
								gComTxSigChgFlag[l_txPduId][0] = 0;
								gComTxSigChgFlag[l_txPduId][1] = 0;
						#endif	


                            } else {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                                CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1, STD_OFF);
#else
                                CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1);
#endif
                            }
                            if ((0 != Com_TxIpduConfInfo[l_txPduId].Timeout) && (STD_OFF == Com_GetTxDMRun(l_txPduId))) {
                                Com_SetTxDMRun(l_txPduId, STD_ON);
                                if ((0 == Com_TxIpduConfInfo[l_txPduId].FirstTimeout)) {
									Com_TxIpduRunInfo[l_txPduId].Timeout = l_sysTick + Com_TxIpduConfInfo[l_txPduId].Timeout;
                                    
                                } else if(STD_ON == Com_GetFirstDM(l_TrigPduId)){
                                    Com_SetFirstDM(l_TrigPduId, STD_OFF);
                                    Com_TxIpduRunInfo[l_txPduId].Timeout = l_sysTick + Com_TxIpduConfInfo[l_txPduId].FirstTimeout;
                                }
                            }
                        } else {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                            CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1, STD_OFF);
#else
                            CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1);
#endif
                        }
                    } else {
                        if ((l_sysTick - Com_TxIpduRunInfo[l_txPduId].PreSendTime) >= Com_TxIpduConfInfo[l_txPduId].RepetitionPeriod) {

							TracePrintf(COM_TRACE,TRACE_INFO,"%d,trigger canId:0x%x\r\n",Com_TxIpduRunInfo[l_txPduId].RepeatCounter,l_txPduId);
                            l_TxIpduInfo.SduDataPtr = (uint8 *)Com_IpduBufPtr[l_TrigPduId];
                            l_TxIpduInfo.SduLength = Com_TxIpduConfInfo[l_txPduId].PduLength;
                            if(CanIf_Transmit(Com_TxIpduConfInfo[l_txPduId].CanIfPduId, &l_TxIpduInfo) == E_OK)
                            {
                            	Com_TxIpduRunInfo[l_txPduId].PreSendTime = l_sysTick;
                            	if (0 < Com_TxIpduRunInfo[l_txPduId].RepeatCounter) {
                                	Com_TxIpduRunInfo[l_txPduId].RepeatCounter--;
                            	}
                            }
							
                            if (0 == Com_TxIpduRunInfo[l_txPduId].RepeatCounter) {
                                Com_SetTxTrigEvent(l_txPduId, STD_OFF);
#if 1
								sigStartIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][0];
								sigEndIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][1];
								sigSumNum = sigEndIndex-sigStartIndex+1;
								if(sigSumNum<=CAN_SIG_MAX_NUM_PER_MSG)
								{
									for(sigOffSet = 0;sigOffSet<sigSumNum;sigOffSet++)
									{
										if(gComTxSigChgFlag[l_txPduId][sigOffSet/32] & (1<<(sigOffSet%32)))
										{
											if (CanApp_Tx_Matrix[sigStartIndex+sigOffSet-COM_TX_SIGNAL_START_INDEX] != NULL) {
												CanApp_Tx_Matrix[sigStartIndex+sigOffSet-COM_TX_SIGNAL_START_INDEX](0, &txTriggerTye);
											}
										}
										
									}
								}
								gComTxSigChgFlag[l_txPduId][0] = 0;
								gComTxSigChgFlag[l_txPduId][1] = 0;
	#endif							
								
                            } else {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                                CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1, STD_OFF);
#else
                                CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1);
#endif
                            }
                            
                            if ((STD_OFF == Com_GetTxDMRun(l_txPduId)) && (0 != Com_TxIpduConfInfo[l_txPduId].Timeout)) {
                                Com_SetTxDMRun(l_txPduId, STD_ON);
                                Com_TxIpduRunInfo[l_txPduId].Timeout = l_sysTick + Com_TxIpduConfInfo[l_txPduId].Timeout;
                            }
                        } else {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                            CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1, STD_OFF);
#else
                            CQueue_Push(Com_TxTrigQueueCh, &l_TrigPduId, 1);
#endif
                        }
                    }
                }
            }
            l_TrigLen--;
        }
    }
}

static void Com_Inner_TxCycleAndTimeoutProcess(void)
{
    uint16 l_txPduId;
    PduInfoType l_TxIpduInfo = {0u, 0u};
    uint32 l_sysTick = 0;
	uint16 l_diff_time = 0 ;

    for (l_txPduId = 0; l_txPduId < Com_TxIpduMaxNum; l_txPduId++) {
        if (COM_PERIODIC_ENABLE == (COM_PERIODIC_ENABLE & Com_GetStatus())) {
            if ((FALSE == Com_GetStartFlag(l_txPduId + Com_TxIpduStartIndex))
                    || (COM_TX_RX_MODE_DIRECT == Com_TxIpduConfInfo[l_txPduId].TxMode)) {
                continue;
            }
            l_sysTick = System_GetTickValue();
            if ((l_sysTick + COM_CONFIGURATION_TIMEBASE - 1) >= Com_TxIpduConfInfo[l_txPduId].FirstOffset) {
                if (l_sysTick < Com_TxIpduConfInfo[l_txPduId].FirstOffset) {
                    if (((Com_TxIpduConfInfo[l_txPduId].FirstOffset - l_sysTick) << 1) <= COM_CONFIGURATION_TIMEBASE) {
                        Com_SetTxCycleEvent(l_txPduId, STD_ON);
                        Com_SetTxOffsetTrig(l_txPduId, STD_ON);
                    }
                } else {
                    if (STD_OFF == Com_GetTxOffsetTrig(l_txPduId)) {
                        Com_SetTxCycleEvent(l_txPduId, STD_ON);
                        Com_SetTxOffsetTrig(l_txPduId, STD_ON);
                    }
                    if (0 != Com_TxIpduConfInfo[l_txPduId].Period) {
						
						//l_diff_time
						
						//calculate cycle diff time.add by tjy,@2019-8-6
						if( Com_TxIpduConfInfo[l_txPduId].Period <= 20 )
							l_diff_time = 2 ;//<=20ms
						else
						{
							l_diff_time = Com_TxIpduConfInfo[l_txPduId].Period /10 ;//>20ms
							if( l_diff_time >= 5)
								l_diff_time = 5;
						}
						
						if((l_sysTick -Com_TxIpduConfInfo[l_txPduId].FirstOffset -Com_TxIpduRunInfo[l_txPduId].PreSendTime) >= (Com_TxIpduConfInfo[l_txPduId].Period-l_diff_time))
							Com_SetTxCycleEvent(l_txPduId, STD_ON);
                    }
                }
            }
            if ((STD_ON == Com_GetTxCycleEvent(l_txPduId)) && (STD_OFF == Com_GetTxTrigEvent(l_txPduId))) {
                l_TxIpduInfo.SduDataPtr = (uint8 *)Com_IpduBufPtr[l_txPduId + Com_TxIpduStartIndex];
                l_TxIpduInfo.SduLength = Com_TxIpduConfInfo[l_txPduId].PduLength;

				CanIf_Transmit(Com_TxIpduConfInfo[l_txPduId].CanIfPduId, &l_TxIpduInfo);
                if ((STD_OFF == Com_GetTxDMRun(l_txPduId)) && (0 != Com_TxIpduConfInfo[l_txPduId].Timeout)) {
                    Com_SetTxDMRun(l_txPduId, STD_ON);
                    if ((STD_ON == Com_GetFirstDM(l_txPduId + Com_TxIpduStartIndex)) && (0 != Com_TxIpduConfInfo[l_txPduId].FirstTimeout)) {
                        Com_SetFirstDM(l_txPduId + Com_TxIpduStartIndex, STD_OFF);
                        Com_TxIpduRunInfo[l_txPduId].Timeout = l_sysTick + Com_TxIpduConfInfo[l_txPduId].FirstTimeout;
                    } else {
                        Com_TxIpduRunInfo[l_txPduId].Timeout = l_sysTick + Com_TxIpduConfInfo[l_txPduId].Timeout;
                    }
                }
            }
        }
        //Tx deadline monitor
        if (0 != Com_TxIpduConfInfo[l_txPduId].Timeout) {
            if ((STD_ON == Com_GetTxDMRun(l_txPduId)) && (l_sysTick >= Com_TxIpduRunInfo[l_txPduId].Timeout)) {
                Com_SetTxDMRun(l_txPduId, STD_OFF);
                if (NULL != Com_IpduErrorCbk[l_txPduId + Com_TxIpduStartIndex]) {
                    (*Com_IpduErrorCbk[l_txPduId + Com_TxIpduStartIndex])(COM_TXRX_CBK_IND_NONE);
                }
            }
        }
    }
}

uint8 Com_GetIpduDataLen(PduIdType pduId)
{
    uint8 l_pduLen = 0XFF;

    if (pduId < Com_RxIpduMaxNum) {
        l_pduLen = Com_RxIpduConfInfo[pduId].PduLength;
    } else if (pduId < Com_IpduMaxNum) {
        l_pduLen = Com_TxIpduConfInfo[pduId].PduLength;
    }
    return l_pduLen;
}

uint8 Com_CopyIpduRawData(PduIdType pduId, uint8 *bufPtr, uint32* rcvedTime)
{
    uint8 l_pduLen = 0u;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((NULL == bufPtr) || (pduId >= Com_IpduMaxNum)) {
        return l_pduLen;
    }
#endif
    if ((COM_RX_IPDU_START_INDEX <= pduId) && (pduId < Com_RxIpduMaxNum)) {
        l_pduLen = Com_RxIpduConfInfo[pduId].PduLength;
    } else if ((Com_TxIpduStartIndex <= pduId) && (pduId < Com_TxIpduMaxNum)) {
        l_pduLen = Com_TxIpduConfInfo[pduId - Com_TxIpduStartIndex].PduLength;
    } else {}
    memcpy(bufPtr, Com_IpduBufPtr[pduId], l_pduLen);
    if (NULL != rcvedTime) {
        *rcvedTime = Com_RxIpdu_TickBuf[pduId];
    }

    return l_pduLen;
}

uint8 Com_CopyRxQueuedData(PduIdType pduId, uint8 *bufPtr, uint32* rcvedTime)
{
    uint8 l_pduLen = 0u;
    PduIdType l_RxPduId = pduId;
    uint16 l_queueIndex = 0u;
    uint32 l_rcvedTime = 0u;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((NULL == bufPtr) || (l_RxPduId >= Com_RxIpduMaxNum)) {
        return l_pduLen;
    }
#endif
    if (0 != Com_RxIpduConfInfo[l_RxPduId].QueueDepth) {
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
        l_queueIndex = CQueue_Pop(Com_RxIpduQueueCh[l_RxPduId], &l_rcvedTime, 1, STD_OFF);
#else
        l_queueIndex = CQueue_Pop(Com_RxIpduQueueCh[l_RxPduId], &l_rcvedTime, 1);
#endif
        if ((Com_RxIpduConfInfo[l_RxPduId].QueueDepth > l_queueIndex) && (NULL != Com_RxIpduQueuePtr[l_RxPduId])) {
            l_pduLen = Com_RxIpduConfInfo[l_RxPduId].PduLength;
            memcpy(bufPtr, Com_RxIpduQueuePtr[l_RxPduId] + l_queueIndex * l_pduLen, l_pduLen);
        }
        if (NULL != rcvedTime) {
#if (STD_ON == COM_RXPDU_RECEIVE_TICK_ENABLE)
            *rcvedTime = l_rcvedTime;
#else
            *rcvedTime = 0;
#endif
        }
    }
    return l_pduLen;
}

void Com_Init(void)
{
    uint16 l_rxPduId = 0XFFFF;
    uint16 l_SigId = 0XFFFF;
	uint16 i = 0;

    if (0 != (Com_Management.ComStatus & COM_INIT)) {
        return;
    }
	

	if(circleListInit(&gCanRxCircleListInfor, CAN_RX_CIRCLE_LIST_ITEM_LEN, CAN_RX_CIRCLEl_LIST_NUM)==FALSE)
		return;
	for(i=0;i<COM_IPDU_MAX_NUM;i++)
	{
		gCom_SimGlobalFlag[i].firstDM = STD_ON;
		gCom_SimGlobalFlag[i].startFlag = STD_ON;
	}

	for(i=0;i<COM_RX_IPDU_MAX_NUM;i++)
	{
		gCom_SimRxFlag[i].rxDM = STD_ON;
		gCom_SimRxFlag[i].rxDMChecked = STD_OFF;
		gCom_SimRxFlag[i].rxBufChange = STD_OFF;
	}

	for(i=0;i<COM_TX_IPDU_MAX_NUM;i++)
	{
		gCom_SimTxFlag[i].txDMRun = STD_OFF;
		gCom_SimTxFlag[i].txCycleEvent = STD_OFF;
		gCom_SimTxFlag[i].txTrigEvent = STD_OFF;
		gCom_SimTxFlag[i].txOffsetTrig = STD_OFF;
	}

	
#if(0 < COM_SIGNAL_MAX_NUM)
    for (l_SigId = 0; l_SigId < COM_SIGNAL_MAX_NUM; l_SigId++) {
        Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
    }
#endif
    Com_IpduBufChangeQueueCh = CQueue_Init(CQUEUE_BUFTYPE_UINT16, (void*)Com_RxBufChangeQueue, Com_RxIpduMaxNum);
    //Com_RxTempQueueCh = CQueue_Init(CQUEUE_BUFTYPE_UINT16, Com_RxTempQueueMgr, Com_RxTempQueueSize);
    Com_TxTrigQueueCh = CQueue_Init(CQUEUE_BUFTYPE_UINT16, (void*)Com_TxTrigQueue, Com_TxIpduMaxNum);
    if ((CQUEUE_ERR_CHANNEL != Com_IpduBufChangeQueueCh)
		/*&&(CQUEUE_ERR_CHANNEL != Com_RxTempQueueCh)*/
		&& (CQUEUE_ERR_CHANNEL != Com_TxTrigQueueCh)) {
        memset((uint8*)&Com_RxIpduQueueCh[0], 0xFF, Com_RxIpduMaxNum << 1);
        for (l_rxPduId = 0; l_rxPduId < Com_RxIpduMaxNum; l_rxPduId++) {
            if (0 != Com_RxIpduConfInfo[l_rxPduId].QueueDepth) {
                Com_RxIpduQueueCh[l_rxPduId] = CQueue_Init(CQUEUE_BUFTYPE_UINT32, (void *)Com_RxIpduQueueMgrPtr[l_rxPduId], Com_RxIpduConfInfo[l_rxPduId].QueueDepth);
                if (CQUEUE_ERR_CHANNEL == Com_RxIpduQueueCh[l_rxPduId]) {
                    return;
                }
            }
        }
    }
    Com_Management.ComStatus = (COM_INIT | COM_RX_ENABLE | COM_TX_ENABLE | COM_PERIODIC_ENABLE);
}

void Com_DeInit(void)
{
    Com_Management.ComStatus = COM_UNINIT;
}

void Com_TxPeriodicStart(void)
{
    uint16 l_pduId = 0XFFFF;
    uint16 l_txPduId = 0XFFFF;
    uint16 l_SigId = 0XFFFF;

    for (l_txPduId = 0; l_txPduId < Com_TxIpduMaxNum; l_txPduId++) {
        if ((COM_TX_RX_MODE_PERIODIC == Com_TxIpduConfInfo[l_txPduId].TxMode)
                || (COM_TX_RX_MODE_MIXED == Com_TxIpduConfInfo[l_txPduId].TxMode)) {
            Com_SetTxCycleEvent(l_txPduId, STD_OFF);
            Com_SetStartFlag(l_txPduId, STD_OFF);
#if(0 < COM_SIGNAL_MAX_NUM)
            l_pduId = l_txPduId + Com_TxIpduStartIndex;
            for (l_SigId = Com_IpduId2SigId[l_pduId][0]; l_SigId <= Com_IpduId2SigId[l_pduId][1]; l_SigId++) {
                Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
            }
#endif
        }
    }
    if (COM_INIT == (COM_INIT & Com_GetStatus())) {
        Com_Management.ComStatus |= COM_PERIODIC_ENABLE;
    }
}

void Com_TxPeriodicStop(void)
{
    Com_Management.ComStatus &= (~COM_PERIODIC_ENABLE);
}

#if (0 < COM_IPDUGROUP_MAX_NUM)
void Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId)
{
    uint8 l_IpduIdLoop = 1U;
    uint16 l_pduId = 0XFFFF;
    uint16 l_txPduId = 0XFFFF;
    uint16 l_SigId = 0XFFFF;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == COM_IPDUGROUP_MAX_NUM) || (COM_IPDUGROUP_MAX_NUM <= IpduGroupId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTART, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif
#if(0 < COM_IPDU_MAX_NUM)
    for (l_IpduIdLoop = 1; l_IpduIdLoop < * (Com_IpduGroupPtr[IpduGroupId]); l_IpduIdLoop++) {
        l_pduId = *(Com_IpduGroupPtr[IpduGroupId] + l_IpduIdLoop);
        if (FALSE == Com_GetStartFlag(l_pduId)) {
            Com_SetStartFlag(l_pduId, TRUE);
#if(0 < COM_TX_IPDU_MAX_NUM)
            if (l_pduId >= Com_RxIpduMaxNum) {
                l_txPduId = l_pduId - Com_TxIpduStartIndex;
                Com_TxIpduRunInfo[l_txPduId].RepeatCounter =  0;
                Com_SetTxTrigEvent(l_txPduId, STD_OFF);
                Com_SetTxCycleEvent(l_txPduId, STD_OFF);
                // Com_SetFirstDM(l_pduId, STD_ON);
            }
#endif
#if(0 < COM_SIGNAL_MAX_NUM)
            for (l_SigId = Com_IpduId2SigId[l_pduId][0]; l_SigId <= Com_IpduId2SigId[l_pduId][1]; l_SigId++) {
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
    uint16 l_pduId = 0XFFFF;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == COM_IPDUGROUP_MAX_NUM) || (COM_IPDUGROUP_MAX_NUM <= IpduGroupId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_IPDUGROUPSTOP, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif
#if(0 < COM_IPDU_MAX_NUM)
    for (l_IpduIdLoop = 1; l_IpduIdLoop < * (Com_IpduGroupPtr[IpduGroupId]); l_IpduIdLoop++) {
        l_pduId = *(Com_IpduGroupPtr[IpduGroupId] + l_IpduIdLoop);
        Com_SetStartFlag(l_pduId, FALSE);
    }
#endif
}
#endif

void Com_DisableReceptionDM(PduIdType ComIpduId)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == Com_RxIpduMaxNum) || (Com_RxIpduMaxNum <= ComIpduId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
    Com_SetRxDM(ComIpduId, STD_OFF);
#endif
}

void Com_EnableReceptionDM(PduIdType ComIpduId)
{
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == Com_RxIpduMaxNum) || (Com_RxIpduMaxNum <= ComIpduId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif

#if(0 < COM_RX_IPDU_MAX_NUM)
    if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[ComIpduId].RxMode)) {
        Com_SetRxDM(ComIpduId, STD_ON);
    } else {
        Com_SetRxDM(ComIpduId, STD_OFF);
    }
#endif
}

#if (0 < COM_IPDUGROUP_MAX_NUM)
void Com_DisableReceptionDMByGroup(Com_IpduGroupIdType IpduGroupId)
{
    uint8 l_RxIpduId = 0;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == COM_RX_IPDUGROUP_MAX_NUM) || (COM_RX_IPDUGROUP_MAX_NUM <= IpduGroupId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif
#if(0 < COM_RX_IPDU_MAX_NUM)
    for (l_RxIpduId = 1; l_RxIpduId < * (Com_IpduGroupPtr[IpduGroupId]); l_RxIpduId++)
    {
        Com_SetRxDM(l_RxIpduId, STD_OFF);
    }
#endif
}

void Com_EnableReceptionDMByGroup(Com_IpduGroupIdType IpduGroupId)
{
    uint8 l_RxIpduId = 1u;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == COM_RX_IPDUGROUP_MAX_NUM) || (COM_RX_IPDUGROUP_MAX_NUM <= IpduGroupId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_DISABLERECEPTIONDM, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif
#if(0 < COM_RX_IPDU_MAX_NUM)
    for (l_RxIpduId = 1; l_RxIpduId < * (Com_IpduGroupPtr[IpduGroupId]); l_RxIpduId++) {
        if (COM_TX_RX_MODE_DIRECT != (Com_RxIpduConfInfo[l_RxIpduId].RxMode)) {
            Com_SetRxDM(l_RxIpduId, STD_ON);
        } else {
            Com_SetRxDM(l_RxIpduId, STD_OFF);
        }
    }
#endif
}
#endif

void Com_TxStart(void)
{
    uint16 l_txPduId = 0XFFFF;
    uint16 l_pduId = 0XFFFF;
    uint16 l_SigId = 0XFFFF;

    for (l_txPduId = 0; l_txPduId < Com_TxIpduMaxNum; l_txPduId++) {
        Com_TxIpduRunInfo[l_txPduId].RepeatCounter =  0;
        Com_SetTxTrigEvent(l_txPduId, STD_OFF);
        Com_SetTxCycleEvent(l_txPduId, STD_OFF);
        // Com_SetStartFlag(l_txPduId, STD_ON);
        // Com_SetFirstDM(l_pduId, STD_ON);
#if(0 < COM_SIGNAL_MAX_NUM)
        l_pduId = l_txPduId + Com_TxIpduStartIndex;
        for (l_SigId = Com_IpduId2SigId[l_pduId][0]; l_SigId <= Com_IpduId2SigId[l_pduId][1]; l_SigId++) {
            Com_SigRunInfo[l_SigId].Filter.Occurence = 0;
        }
#endif
    }
    Com_Management.ComStatus |= COM_TX_ENABLE;
}

void Com_TxStop(void)
{
	
	Com_Management.ComStatus &= (~COM_TX_ENABLE);

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
    CQueue_Clear(Com_TxTrigQueueCh, STD_OFF);
#else
    CQueue_Clear(Com_TxTrigQueueCh);
#endif
	memset(gComTxSigChgFlag,0,sizeof(gComTxSigChgFlag));
}

void Com_RxStop()
{
	uint16 i = 0;
	
	for(i=0;i<COM_RX_IPDU_MAX_NUM;i++)
	{
		//Com_RxShortPeriodTimeoutFlag[i] = FALSE;
		//Com_RxLongPeriodTimeoutFlag[i] = FALSE;
		Com_SetRxDM(i,STD_OFF);
		Com_SetRxDMChecked(i,STD_OFF);
		Com_SetRxBufChange(i,STD_OFF);
	}


	if(COM_RX_ENABLE & Com_Management.ComStatus )
	{
		Com_Management.ComStatus &= ~COM_RX_ENABLE;
	}
}

void Com_RxStart()
{
	uint16 i = 0;

	circleListClear(&gCanRxCircleListInfor);
	for(i=0;i<COM_RX_IPDU_MAX_NUM;i++)
	{
		//Com_RxShortPeriodTimeoutFlag[i] = FALSE;
		//Com_RxLongPeriodTimeoutFlag[i] = FALSE;
		Com_SetRxDM(i,STD_ON);
		Com_SetRxDMChecked(i,STD_OFF);
		Com_SetRxBufChange(i,STD_OFF);

	}
	
	Com_Management.ComStatus |= COM_RX_ENABLE;
}



uint8 Com_SendSignal(Com_SignalIdType SignalId, void *SignalDataPtr)
{
    boolean l_returnValue = E_OK;
    boolean l_filterPass = TRUE;
    uint32 l_oldValue = 0U, l_newValue = 0U;
    uint16 l_pduId = Com_SigId2IpduId[SignalId];
    uint16 l_txPduId = 0U;
	uint16 sigStartIndex = 0;
	uint16 sigEndIndex = 0;
	uint16 counter = 0;
	uint16 index = 0;
	boolean matchFlag = FALSE;
	
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (COM_TX_ENABLE != (COM_TX_ENABLE & Com_GetStatus()))
            || (COM_TX_SIGNAL_START_INDEX > SignalId) || (COM_SIGNAL_MAX_NUM <= SignalId)
            || (NULL_PTR == SignalDataPtr)
            || (FALSE == Com_GetStartFlag(l_pduId))) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
        return (E_NOT_OK);
    }
#endif
#if(0 < COM_TX_SIGNAL_MAX_NUM)
    switch (Com_SigConfInfo[SignalId].SignalType) {
    case COM_SIG_BOOLEAN:
    case COM_SIG_UINT8:
    case COM_SIG_SINT8:
        l_newValue = *(uint8 *)SignalDataPtr;
        break;
    case COM_SIG_SINT16:
    case COM_SIG_UINT16:
        l_newValue = *(uint16 *)SignalDataPtr;
        break;
    case COM_SIG_SINT32:
    case COM_SIG_UINT32:
        l_newValue = *(uint32 *)SignalDataPtr;
        break;
    default:
        break;
    }
    if ((32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_OPAQUE == Com_SigConfInfo[SignalId].EndianType)) {
        // EnterCriticalCode();// change to mutex lock
        Com_Inner_OpaqueTypeSignal(SignalId, COM_SET_SIG_VALUE, Com_IpduBufPtr[l_pduId], (uint8*)SignalDataPtr);
        // ExitCriticalCode();
    } else {
        // EnterCriticalCode();
#if (STD_ON == USE_TX_SIG_FILTER)
        (*Com_GetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, Com_IpduBufPtr[l_pduId], &l_oldValue);
        l_filterPass = (*Com_SigFilter[Com_SigConfInfo[SignalId].Filter.Algorithm])(SignalId, l_oldValue, l_newValue);
#endif
        (*Com_SetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, Com_IpduBufPtr[l_pduId], l_newValue);
        // ExitCriticalCode();
    }
#if(0 < COM_TX_IPDU_MAX_NUM)
    if (Com_SigId2IpduId[SignalId] >= Com_TxIpduStartIndex) {
        l_txPduId = Com_SigId2IpduId[SignalId] - Com_TxIpduStartIndex;
        if ((Com_TxIpduConfInfo[l_txPduId].TxMode != COM_TX_RX_MODE_PERIODIC)
                /*&& (COM_SIG_TX_TRIGGERED == Com_SigConfInfo[SignalId].TransferProperty)*/
                && (TRUE == l_filterPass)) {
			sigStartIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][0];
			sigEndIndex = Com_IpduId2SigId[l_txPduId+Com_TxIpduStartIndex][1];
			for(index = sigStartIndex;index<=sigEndIndex;index++)
			{
				if(index==SignalId)
				{
					matchFlag = TRUE;
					break;
				}
				counter++;
			}
			
			if(matchFlag == TRUE)
			{
				gComTxSigChgFlag[l_txPduId][counter/32] |= 1<<(counter%32);
			}
			
			if (STD_OFF == Com_GetTxTrigEvent(l_txPduId)) {
                Com_SetTxTrigEvent(l_txPduId, STD_ON);
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
                CQueue_Push(Com_TxTrigQueueCh, &l_pduId, 1, STD_OFF);
#else
                CQueue_Push(Com_TxTrigQueueCh, &l_pduId, 1);
#endif
            }
            Com_TxIpduRunInfo[l_txPduId].RepeatCounter = Com_TxIpduConfInfo[l_txPduId].NumberOfRepetitions;
        }
    }
#endif
    l_returnValue = E_OK;
#else
    l_returnValue = E_NOT_OK;
#endif
    return (l_returnValue);
}

uint8 Com_SetSignal(Com_SignalIdType SignalId, void *SignalDataPtr)
{
	uint8 l_ret = 0;
	uint16 l_pduId = Com_SigId2IpduId[SignalId];
	uint32 l_newValue = 0U;

#if 0
#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (COM_TX_ENABLE != (COM_TX_ENABLE & Com_GetStatus()))
            || (COM_TX_SIGNAL_START_INDEX > SignalId) || (COM_SIGNAL_MAX_NUM <= SignalId)
            || (NULL_PTR == SignalDataPtr)
            || (FALSE == Com_GetStartFlag(l_pduId))) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
        return (E_NOT_OK);
    }
#endif
#else
if ((COM_SIGNAL_MAX_NUM <= SignalId) || (NULL_PTR == SignalDataPtr)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_SENDSIGNAL, COM_SERVICE_NOT_AVAILABLE);
		l_ret = E_NOT_OK;
        return (l_ret);
    }

#endif

#if(0 < COM_TX_SIGNAL_MAX_NUM)
    switch (Com_SigConfInfo[SignalId].SignalType) {
    case COM_SIG_BOOLEAN:
    case COM_SIG_UINT8:
    case COM_SIG_SINT8:
        l_newValue = *(uint8 *)SignalDataPtr;
        break;
    case COM_SIG_SINT16:
    case COM_SIG_UINT16:
        l_newValue = *(uint16 *)SignalDataPtr;
        break;
    case COM_SIG_SINT32:
    case COM_SIG_UINT32:
        l_newValue = *(uint32 *)SignalDataPtr;
        break;
    default:
        break;
    }
    if ((32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_OPAQUE == Com_SigConfInfo[SignalId].EndianType)) {
        // EnterCriticalCode();// change to mutex lock
        Com_Inner_OpaqueTypeSignal(SignalId, COM_SET_SIG_VALUE, Com_IpduBufPtr[l_pduId], (uint8*)SignalDataPtr);
        // ExitCriticalCode();
    } else {
	(*Com_SetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, Com_IpduBufPtr[l_pduId], l_newValue);
    }
#endif
    return (l_ret);
}

uint8 Com_ReceiveSignal(Com_SignalIdType SignalId, void *SignalDataPtr)
{
    boolean l_returnValue = E_OK;
    PduIdType l_pduId = Com_SigId2IpduId[SignalId];
    uint32 l_signalValue = 0xFFFF;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus()))
            || (0U == COM_SIGNAL_MAX_NUM) || (COM_SIGNAL_MAX_NUM <= SignalId)
            || (NULL_PTR == SignalDataPtr)
            || (FALSE == Com_GetStartFlag(l_pduId))) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RECEIVESIGNAL, COM_SERVICE_NOT_AVAILABLE);
        return (E_NOT_OK);
    }
#endif
#if(0 < COM_SIGNAL_MAX_NUM)
    if ((32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_OPAQUE == Com_SigConfInfo[SignalId].EndianType)) {
        EnterCriticalCode();
        Com_Inner_OpaqueTypeSignal(SignalId, COM_GET_SIG_VALUE, Com_IpduBufPtr[l_pduId], (uint8*)SignalDataPtr);
        ExitCriticalCode();
    } else {
        EnterCriticalCode();
        (*Com_GetSigProcess[Com_SigConfInfo[SignalId].EndianType])(SignalId, Com_IpduBufPtr[l_pduId], &l_signalValue);
        ExitCriticalCode();
        switch (Com_SigConfInfo[SignalId].SignalType)
        {
        case COM_SIG_BOOLEAN:
        case COM_SIG_UINT8:
        case COM_SIG_SINT8:
            *(uint8*)SignalDataPtr = (uint8)l_signalValue;
            break;
        case COM_SIG_SINT16:
        case COM_SIG_UINT16:
            *(uint16*)SignalDataPtr = (uint16)l_signalValue;
            break;
        case COM_SIG_SINT32:
        case COM_SIG_UINT32:
            *(uint32*)SignalDataPtr = (uint32)l_signalValue;
            break;
        default:
            break;
        }
    }
    l_returnValue = E_OK;

#else
    l_returnValue = E_NOT_OK;
#endif
    return (l_returnValue);
}

uint8 (*Com_Sig_Value[])(Com_SignalIdType SignalId, void *SignalDataPtr) =
{
    Com_ReceiveSignal, Com_SendSignal
};

void Com_RxIndication(PduIdType ComRxPduId, const PduInfoType * PduInfoPtr)
{
	uint8 tmpBuff[CAN_RX_CIRCLE_LIST_ITEM_LEN] = {0};
	uint32 curTick = 0;
	
//#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus()))
            || (0U == Com_RxIpduMaxNum) || (Com_RxIpduMaxNum <= ComRxPduId)
            || (NULL_PTR == PduInfoPtr)
            || (8 < Com_RxIpduConfInfo[ComRxPduId].PduLength)
            || (FALSE == Com_GetStartFlag(ComRxPduId))) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
//#endif



	if (NULL != PduInfoPtr->SduDataPtr && Com_RxIpduConfInfo[ComRxPduId].PduLength <= PduInfoPtr->SduLength) {
    #if(0 < COM_RX_IPDU_MAX_NUM)
			Com_RxShortPeriodsTimeoutFlag[ComRxPduId] = FALSE;
		
		#if 0
			#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
			l_queueIndex = CQueue_Push(Com_RxTempQueueCh, &ComRxPduId, 1, STD_ON);
			#else
			l_queueIndex = CQueue_Push(Com_RxTempQueueCh, &ComRxPduId, 1);
			#endif
			if (Com_RxTempQueueSize > l_queueIndex) {
	        	memcpy((uint8*)&Com_RxTempQueue[l_queueIndex][0], PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
		    	Com_RxTemp_TickBuf[l_queueIndex] = System_GetTickValue();
		    }
		#else
			tmpBuff[0] = (uint8)((ComRxPduId&0xFF00)>>8);
			tmpBuff[1] = (uint8)((ComRxPduId&0xFF));
			curTick = System_GetTickValue();
			tmpBuff[2] = (uint8)((curTick&0xFF000000)>>24);
			tmpBuff[3] = (uint8)((curTick&0xFF0000)>>16);
			tmpBuff[4] = (uint8)((curTick&0xFF00)>>8);
			tmpBuff[5] = (uint8)((curTick&0xFF));

			memcpy(&tmpBuff[6],PduInfoPtr->SduDataPtr,8);
			if(circleListInsert(&gCanRxCircleListInfor,tmpBuff,CAN_RX_CIRCLE_LIST_ITEM_LEN) == FALSE)
				gLostCanMsgCounter++;
		#endif
	#endif
	} else {
	    Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_RXINDICATION, COM_SERVICE_NOT_AVAILABLE);
	}

}

void Com_TxConfirmation(PduIdType ipduId)
{
    PduIdType l_pduId = ipduId;

#if (STD_ON == COM_DEV_ERROR_DETECT)
    if ((COM_INIT != (COM_INIT & Com_GetStatus()))
            || (0U == Com_TxIpduMaxNum) || (Com_TxIpduMaxNum <= l_pduId)) {
        Det_ReportError(COM_MODULE_ID, TRACE_ERRO, COMSERVICEID_TXCONFIRMATION, COM_SERVICE_NOT_AVAILABLE);
        return;
    }
#endif

	Com_TxIpduRunInfo[l_pduId].PreSendTime = System_GetTickValue();
	Com_SetTxCycleEvent(l_pduId, STD_OFF);

    Com_SetTxDMRun(l_pduId, STD_OFF);
    if (NULL != Com_IpduInformCbk[l_pduId + Com_TxIpduStartIndex]) {
        (*Com_IpduInformCbk[l_pduId + Com_TxIpduStartIndex])(COM_TXRX_CBK_IND_NONE);
#ifdef COM_WIN32_DEBUG
        printf("TxConfirm:%d\n", l_pduId);
#endif
    }
}

void Com_MainFunctionRx(void)
{
	static boolean firstBootFlag = TRUE;

	if(firstBootFlag == TRUE)
	{
		firstBootFlag = FALSE;
		gLostCanMsgCounter = 0;
	}
    if (COM_RX_ENABLE != (COM_RX_ENABLE & Com_GetStatus())) {
        return;
    }
#if(0 < COM_RX_IPDU_MAX_NUM)
    Com_Inner_RxProcess();
    Com_Inner_RxTimeoutProcess();
#endif
}

void Com_MainFunctionTx(void)
{
    static uint8 l_count = 0u;

    if (l_count < 2) {
        l_count++;
    } else if (l_count >= 2) {
        if (COM_TX_ENABLE != (COM_TX_ENABLE & Com_GetStatus())) {
            return;
        }
#if(0 < COM_TX_IPDU_MAX_NUM)
        Com_Inner_TxTrigProcess();
        Com_Inner_TxCycleAndTimeoutProcess();
#endif
    }
}

/*============================== END OF FILE: Com.c =============================*/
