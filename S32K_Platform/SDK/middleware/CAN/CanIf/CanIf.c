/**************************
author: chen zedong@hangsheng.com.cn
version: v1.0.0
notes:
	1.this is a interface layer to CAN application according current architect
	2.multiple channals app supported may be aware of configuration,especially the number of MB, rx fifo mask
	individual mask should be checked by datasheet
	3.to run correctly can moudule,flexcan module gernerated code was fixed by manually.
	4.dma transfering in fifo is not surrported , adding related code in FLEXCAN_CompleteRxMessageFifoData if necessary.
**************************/

#include "CanIf.h"
#include "S32K146.h"
#include "flexcan_driver.h"
#include "flexcan_hw_access.h"
void CanIf_Indication (uint8 instance, flexcan_event_type_t eventType,
                       flexcan_state_t *flexcanState);

typedef struct
{
    CanIf_ControllerModeType ControllerMode;
    CanIf_PduModeType PduMode;

} CanIfControllerType;

static CanIfControllerType CanIfControllers[CAN_INSTANCE_COUNT];
static uint16 CanTxStartIndex[CAN_INSTANCE_COUNT];
static uint16 CanNrOfTxPdu[CAN_INSTANCE_COUNT];
static uint16 CanNrOfRxPdu[CAN_INSTANCE_COUNT];
static flexcan_msgbuff_t canRecvBuffFifo, canRecvBuff[32];
static CanIf_patchTdType  *CanRxPduChn[CAN_INSTANCE_COUNT];

extern CAN_Type * const g_flexcanBase[];
#define RxFifoFilterElementNum(x) (((x) + 1U) * 8U)
#define CanIf_Init(Instance)						 \
	{\
	flexcan_data_info_t rx_info;\
	uint32 i = 0,nrOfaVaiMbMask = 0,nrOfSetMask = 0;\
	uint32 numOfFilters = 0, numOfMb = 0, mbInx = 0;	\
	FLEXCAN_DRV_Init(Instance, &CanState(Instance), &CanInitCfg(Instance));\
	if (CanInitCfg(Instance).is_rx_fifo_needed)\
	{\
		FLEXCAN_DRV_ConfigRxFifo(Instance, CanFifoFilterFormat(Instance), &CanIdFilter(Instance));\
		FLEXCAN_DRV_RxFifo(Instance, &canRecvBuffFifo);\
		nrOfaVaiMbMask = 8 + CanInitCfg(Instance).num_id_filters * 2;\
		nrOfSetMask = (CanNumOfMask(Instance) > nrOfaVaiMbMask)? CanNumOfMask(Instance) - nrOfaVaiMbMask : 0;\
		numOfFilters = RxFifoFilterElementNum(CanInitCfg(Instance).num_id_filters);\
		numOfMb = (CanNumOfFilter(Instance) > numOfFilters) ? CanNumOfFilter(Instance) - numOfFilters : 0;\
		mbInx = CanInitCfg(Instance).num_id_filters * 2 + 8;\
	}\
	else\
	{\
		numOfMb = CanNumOfFilter(Instance);\
		nrOfSetMask = CanNumOfMask(Instance);\
	}\
	if (numOfMb > 0)\
	{\
		rx_info.msg_id_type = CanIdFilter(Instance).isExtendedFrame? FLEXCAN_MSG_ID_EXT : FLEXCAN_MSG_ID_STD;\
		rx_info.fd_enable = CanInitCfg(Instance).fd_enable;\
	}\
	FLEXCAN_DRV_InstallEventCallback(Instance, CanIf_Indication, 0);\
	for (i = 0; i < nrOfSetMask; i++)\
	{\
		FLEXCAN_DRV_SetRxIndividualMask(Instance, CanFifoFilterFormat(Instance), i, CanIdFilterMask(Instance)[i]);\
	}\
	for (i = 0; i < numOfMb; i++)\
	{\
		FLEXCAN_DRV_ConfigRxMb(Instance, mbInx + i, &rx_info, CanIdFilter(Instance).idFilter[numOfFilters + i]);\
		FLEXCAN_DRV_Receive(Instance, mbInx + i, &canRecvBuff[i]);\
	}\
	CanNrOfTxPdu[Instance] = CanNumOfTxPdu(Instance);\
	CanNrOfRxPdu[Instance] = CanNumOfRxPdu(Instance);\
	CanTxStartIndex[Instance]  = mbInx + numOfMb;\
	CanIfControllers[Instance].PduMode = CANIF_ONLINE;\
	CanRxPduChn[Instance] = CanRxPduInf(Instance);\
	}

#ifdef CAN_CHN_CFG_0
void CanIf_Init_0()
{
    CanIf_Init(0);
#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(0, true, &CanPnConfig(0));
#endif
};
#endif

#ifdef CAN_CHN_CFG_1
void CanIf_Init_1()
{
    CanIf_Init(1);
#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(1, true, &CanPnConfig(1));
#endif
};
#endif
#ifdef CAN_CHN_CFG_2
void CanIf_Init_2()
{
    CanIf_Init(2);
#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(2, true, &CanPnConfig(2));
#endif
};
#endif

uint8 CanIf_SetControllerMode(uint8 Instance, CanIf_ControllerModeType ControllerMode)
{
    CAN_Type *base = NULL;
    uint8 i = 0;

    base = g_flexcanBase[Instance];
    switch (ControllerMode)
    {
    case CANIF_CS_STARTED:

        FLEXCAN_Enable(base);
        FLEXCAN_SetErrIntCmd(base, CAN_CTRL1_BOFFMSK_MASK, true);
        break;

    case CANIF_CS_SLEEP:
        for ( i = 0; i < CanNrOfTxPdu[Instance]; i++)
        {
            FLEXCAN_DRV_AbortTransfer(Instance, i + CanTxStartIndex[Instance]);

        }
        FLEXCAN_Disable(base);
        break;

    default:
        break;
    }
    return 0;
}

uint8  CanIf_SetPduMode(uint8 Instance, CanIf_PduModeType PduModeRequest)
{
    switch (PduModeRequest)
    {
    case CANIF_OFFLINE:
    case CANIF_TX_OFFLINE:

        CanIfControllers[Instance].PduMode = PduModeRequest;
        break;
    case CANIF_ONLINE:

        CanIfControllers[Instance].PduMode = CANIF_ONLINE;
        break;
    case CANIF_TX_OFFLINE_ACTIVE:

        CanIfControllers[Instance].PduMode = CANIF_TX_OFFLINE_ACTIVE;
        break;
    default:

        break;
    }
    return 0;
}

uint8 CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr)
{
    uint8 inst = CanIf_txPduCfg[CanTxPduId].instance;

    if ((CanIfControllers[inst].PduMode == CANIF_OFFLINE) ||
            (CanIfControllers[inst].PduMode == CANIF_TX_OFFLINE))
    {
        return 1;
    }
    uint32 id = CanIf_txPduCfg[CanTxPduId].canId;
    uint8 i = 0;
    uint8 mbId = CanTxPduId + CanTxStartIndex[inst];
    flexcan_data_info_t txInf = {0};
    txInf.data_length = PduInfoPtr->SduLength;
    txInf.msg_id_type = CanIf_txPduCfg[CanTxPduId].canIdType;
    status_t sta;

    for (i = 0; i < CanNrOfTxPdu[inst]; i++)
    {
        sta = FLEXCAN_DRV_GetTransferStatus(inst, mbId);
        if (sta == STATUS_SUCCESS)
        {
            break;
        }
        mbId++;
    }
    if (i >= CanNrOfTxPdu[inst])
    {
        return 1;
    }
    FLEXCAN_DRV_ConfigTxMb(inst, mbId, &txInf, id);
    FLEXCAN_DRV_Send(inst, mbId, &txInf, id, PduInfoPtr->SduDataPtr);
    return 0;
}

void CanIf_Indication (uint8 instance, flexcan_event_type_t eventType, flexcan_state_t *flexcanState)
{
    uint8 dlc;
    uint16 pduId;
    uint32 id;
    uint32 max = CanNrOfRxPdu[instance] - 1;
    uint32 min = 0, mid;
    uint8 isMatch = 0;
    uint8 *data;
    uint32 mbId = *(uint32 *)(flexcanState->callbackParam);
    PduInfoType pduInfo;
    uint8 mdType;

    if ((CanIfControllers[instance].PduMode == CANIF_OFFLINE) ||
            (CanIfControllers[instance].PduMode == CANIF_TX_OFFLINE_ACTIVE))
    {
        return;
    }
    switch (eventType)
    {
    case FLEXCAN_EVENT_RXFIFO_COMPLETE:
        mbId = 0;
        FLEXCAN_DRV_RxFifo(instance, &canRecvBuffFifo);
    case FLEXCAN_EVENT_RX_COMPLETE:
        id = flexcanState->mbs[mbId].mb_message->msgId;
        dlc = flexcanState->mbs[mbId].mb_message->dataLen;
        data = flexcanState->mbs[mbId].mb_message->data;
        if (eventType == FLEXCAN_EVENT_RX_COMPLETE)
        {
            FLEXCAN_DRV_Receive(instance,  mbId, &canRecvBuff[mbId]);
        }
        while (min <= max)
        {
            mid = (min + max) >> 1;

            if (CanRxPduChn[instance][mid].id > id)
            {
                max = mid - 1;
            }
            else if (CanRxPduChn[instance][mid].id == id)
            {
                isMatch = 1;
                pduId = CanRxPduChn[instance][mid].cbkId;
                mdType = CanRxPduChn[instance][mid].usrType;
                break;
            }
            else
            {
                min = mid + 1;
            }
        }
        if (isMatch)
        {
            pduInfo.SduLength = dlc;
            pduInfo.SduDataPtr = data;
            if (CanIfUserRxIndications[mdType] != NULL)
            {
                CanIfUserRxIndications[mdType](pduId, &pduInfo);
            }
        }
        break;

    case FLEXCAN_EVENT_TX_COMPLETE:
        pduId = CanIf_txPduCfg[mbId - CanTxStartIndex[instance]].cbkId;
        mdType =  CanIf_txPduCfg[mbId - CanTxStartIndex[instance]].usrType;
        if (CanIfUserTxConfirmations[mdType] != NULL)
        {
            CanIfUserTxConfirmations[mdType](pduId);
        }
        break;

#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    case FLEXCAN_EVENT_WAKEUP_TIMEOUT: break;
    case FLEXCAN_EVENT_WAKEUP_MATCH:
        pduId = 2;
        break;
#endif
    }
}

uint8 CanIf_SetTransceiverMode(uint8 Transceiver, CanIf_TransceiverModeType TransceiverMode)
{
#if 0
    if (Transceiver == 0)
    {
        switch (TransceiverMode)
        {

        case CANIF_TRCV_MODE_NORMAL:
            Port_Can_Stby_Pin(PORT_SET_VALUE, PORT_HIGH);
            Port_Can_En_Pin(PORT_SET_VALUE, PORT_HIGH);
            break;
        case CANIF_TRCV_MODE_STANDBY:
            Port_Can_Stby_Pin(PORT_SET_VALUE, PORT_LOW);
            Port_Can_En_Pin(PORT_SET_VALUE, PORT_LOW);
            break;

        case CANIF_TRCV_MODE_SLEEP:
            break;
        default: break;
        }
    }
#endif
    return 0;
}
