/**************************
author: Yi
version: v2.0.0
notes:
	1.this is a interface layer to CAN application according current architect
	2.multiple channals app supported may be aware of configuration,especially the number of MB, rx fifo mask
	individual mask should be checked by datasheet
	3.to run correctly can moudule,flexcan module gernerated code was fixed by manually.
	4.dma transfering in fifo is not surrported , adding related code in FLEXCAN_CompleteRxMessageFifoData if necessary.
**************************/
#include "can0_cfg.h"
#include "S32K148.h"
#include "CanIf.h"
#include "flexcan_driver.h"
#include "flexcan_hw_access.h"
#include "Can_Nm.h"
#include "can1_cfg.h"
#include "dtcApp_if.h"
#include <string.h>

extern void (*const CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
extern void (*const CanIfUserTxConfirmations[])(PduIdType TxPduId);
extern void (* const CanIf_BusOffCbk)(uint8_t instance);

static boolean gCanDriverInitOkFlag[CAN_INSTANCE_COUNT] = {FALSE,FALSE,FALSE};

static boolean Can_BusOff[] = {
	FALSE,
	FALSE,
	FALSE
}; 

static CanIfControllerType CanIfControllers[CAN_INSTANCE_COUNT];

#define CAN_IF_MSG_MAIL_NUM 32
static uint8 gCanMsgMailPduIdPair[CAN_INSTANCE_COUNT][CAN_IF_MSG_MAIL_NUM];


void CanIf_Indication (uint8 instance, flexcan_event_type_t eventType,flexcan_state_t *flexcanState);

boolean CanIf_GetDriverInitResult(uint8 instance)
{
	if(instance <CAN_INSTANCE_COUNT)
		return gCanDriverInitOkFlag[instance];
	
	return FALSE;
}

void CanIf_Init(uint8 Instance)
{
	uint8 numRxMb = 0, i;
	flexcan_data_info_t rx_info;

	if(Instance >= CAN_CH_MAX_NUM || Can_InitConfig[Instance] == NULL || NULL == Can_cfg_State[Instance])
		return;
	memset(gCanMsgMailPduIdPair[Instance],0xFF,CAN_IF_MSG_MAIL_NUM);
	
	if(FLEXCAN_DRV_Init(Instance, (flexcan_state_t*)Can_cfg_State[Instance], Can_InitConfig[Instance]) != STATUS_SUCCESS)
	{
		gCanDriverInitOkFlag[Instance] = FALSE;
		return;
	}
	else
		gCanDriverInitOkFlag[Instance] = TRUE;
	
	#if (STD_ON == CANIF_USE_RXFIFO)
	//FIFO USED FOR DMA
	if (Can_InitConfig[Instance]->is_rx_fifo_needed){
			FLEXCAN_DRV_ConfigRxFifo(Instance, CanIf_RxFifoIdFormat[Instance], CanIf_RxFifoIdCfg[Instance]);
			FLEXCAN_DRV_RxFifo(Instance, CanIf_RecvMsgBuffFifo[Instance]);
			nrOfaVaiMbMask = 8 + Can_InitConfig[Instance]->num_id_filters * 2;
			nrOfSetMask = (CANIF_RX_FILTER_NUM_0 > nrOfaVaiMbMask)? CANIF_RX_FILTER_NUM_0 - nrOfaVaiMbMask : 0;
			numOfFilters = RxFifoFilterElementNum(Can_InitConfig[Instance]->num_id_filters);
			numOfMb = (CANIF_RX_FILTER_NUM_0 > numOfFilters) ? CANIF_RX_FILTER_NUM_0 - numOfFilters : 0;
			mbInx = Can_InitConfig[Instance]->num_id_filters * 2 + 8;
		}
	#else
	FLEXCAN_DRV_SetRxMaskType(Instance, FLEXCAN_RX_MASK_INDIVIDUAL);
	numRxMb = CanIf_RxFilterNum[Instance];

	FLEXCAN_DRV_InstallEventCallback(Instance, CanIf_Indication, 0);

	for (i = 0; i < numRxMb; i++){
		FLEXCAN_DRV_SetRxIndividualMask(Instance, CanIf_RxIdFilter[Instance][i].isExtendedFrame, i, CanIf_RxIdFilter[Instance][i].idMask);
	}
	
	CanIfControllers[Instance].PduMode = CANIF_ONLINE;
	for (i = 0; i < numRxMb; i++){
		rx_info.msg_id_type = CanIf_RxIdFilter[Instance][i].isExtendedFrame? FLEXCAN_MSG_ID_EXT : FLEXCAN_MSG_ID_STD;
		rx_info.fd_enable = Can_InitConfig[Instance]->fd_enable;
		FLEXCAN_DRV_ConfigRxMb(Instance, i, &rx_info, CanIf_RxIdFilter[Instance][i].idFilter);
		FLEXCAN_DRV_Receive(Instance, i, &CanIf_RecvMsgBuff[Instance][i]);
	}

	//CanIfControllers[Instance].PduMode = CANIF_ONLINE;
	#endif
	
}



#ifdef CAN_CHN_CFG_0
void CanIf_Init_0()
{
    CanIf_Init(0);
	#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(0, true, &can0_PNConfig);
	#endif
};
#endif



#ifdef CAN_CHN_CFG_1
void CanIf_Init_1()
{
    CanIf_Init(1);
	#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(0, true, &can0_PNConfig);
	#endif
};
#endif


#ifdef CAN_CHN_CFG_2
void CanIf_Init_2()
{
    CanIf_Init(2);
	#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    //FLEXCAN_DRV_ConfigPN(0, true, &can0_PNConfig);
	#endif
};
#endif


uint8 CanIf_SetControllerMode(uint8 Instance, CanIf_ControllerModeType ControllerMode)
{
	uint8 i = 0;
	
    CAN_Type *base = NULL;
    CAN_Type * const g_flexcanBase2[] = CAN_BASE_PTRS;
    base = g_flexcanBase2[Instance];
    
    switch (ControllerMode){
    case CANIF_CS_STARTED:

        FLEXCAN_SetErrIntCmd(base, CAN_CTRL1_BOFFMSK_MASK, TRUE);
        FLEXCAN_SetErrIntCmd(base, CAN_CTRL1_BOFFREC_MASK, FALSE);
    	FLEXCAN_SetOperationMode(base, FLEXCAN_NORMAL_MODE);
    	FLEXCAN_Enable(base);
    	Can_BusOff[Instance] = FALSE;
        break;

    case CANIF_CS_SLEEP://did not test
        for ( i = CanTxStartIndex[Instance]; i < 32; i++){
            FLEXCAN_DRV_AbortTransfer(Instance, i);
        }
        FLEXCAN_Disable(base);
        break;

    default:
        break;
    }
    return 0;
}


//this function did not test
uint8  CanIf_SetPduMode(uint8 Instance, CanIf_PduModeType PduModeRequest)
{
    switch (PduModeRequest){
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
    uint8 inst = 0;
    uint32 id = 0;
	uint8 mbId = 0;
	uint8 ret = E_NOT_OK;
	boolean gotMaxId = FALSE;
	uint16 maxPduId = 0;
	uint16 index = 0;
	
    flexcan_data_info_t txInf = {0};
    status_t sta = STATUS_ERROR;
	
	if(PduInfoPtr == NULL){
		return E_NOT_OK;
	}

	inst = CanIf_TxPduCfg[CanTxPduId].instance;
	id = CanIf_TxPduCfg[CanTxPduId].canId;
	mbId = CanTxStartIndex[inst];

    txInf.data_length = PduInfoPtr->SduLength;
    txInf.msg_id_type = CanIf_TxPduCfg[CanTxPduId].canIdType;

    if ((CanIfControllers[inst].PduMode == CANIF_OFFLINE)
		||(CanIfControllers[inst].PduMode == CANIF_TX_OFFLINE)){
        return E_NOT_OK;
    }

    for (; mbId < 32; mbId++)
	{
        sta = FLEXCAN_DRV_GetTransferStatus(inst, mbId);
        if (sta == STATUS_SUCCESS || (gCanMsgMailPduIdPair[inst][mbId] == CanTxPduId))
        {
        	if(sta != STATUS_SUCCESS)
				FLEXCAN_DRV_AbortTransfer(inst,mbId);
    		FLEXCAN_DRV_ConfigTxMb(inst, mbId, &txInf, id);
			//#################CAUTION###################
			Can_cfg_State[inst]->mbs[mbId].mb_message = &CanIf_RecvMsgBuff[inst][mbId];
			Can_cfg_State[inst]->mbs[mbId].mb_message->msgId = id;
			//################END#######################
			ret = E_OK;
			gCanMsgMailPduIdPair[inst][mbId] = CanTxPduId;
    		FLEXCAN_DRV_Send(inst, mbId, &txInf, id, PduInfoPtr->SduDataPtr);
        	break;
        }
    }

	
	if(ret != E_OK)//all msgMailBuffers are busy
	{
		mbId = CanTxStartIndex[inst];
		maxPduId = gCanMsgMailPduIdPair[inst][mbId];
		mbId++;
		for(;mbId<32;mbId++)
		{
			//find the max pduId, its priority is lowest
			if(gCanMsgMailPduIdPair[inst][mbId]!= 0xFF)
			{
				if(maxPduId<=gCanMsgMailPduIdPair[inst][mbId])
				{
					maxPduId = gCanMsgMailPduIdPair[inst][mbId];
					index = mbId;
					gotMaxId = TRUE;
				}
			}
		}

		if(gotMaxId)
		{
			mbId = index;
			FLEXCAN_DRV_AbortTransfer(inst,mbId);
			FLEXCAN_DRV_ConfigTxMb(inst, mbId, &txInf, id);
			//#################CAUTION###################
			Can_cfg_State[inst]->mbs[mbId].mb_message = &CanIf_RecvMsgBuff[inst][mbId];
			Can_cfg_State[inst]->mbs[mbId].mb_message->msgId = id;
			//################END#######################
			ret = E_OK;
			gCanMsgMailPduIdPair[inst][mbId] = CanTxPduId;
			FLEXCAN_DRV_Send(inst, mbId, &txInf, id, PduInfoPtr->SduDataPtr);
		}
	}
	
    return ret;
}

void CanIf_Indication (uint8 instance, flexcan_event_type_t eventType, flexcan_state_t *flexcanState)
{
    uint8 dlc;
    uint16 pduId;
    uint32 id;
    sint8 max = CanIfNumOfRxPdu[instance] - 1;
    sint8 min = 0, mid;
    uint8 isMatch = 0;
    uint8 *data;
    uint32 mbId = *(uint32 *)(flexcanState->callbackParam);
    PduInfoType pduInfo;
    uint8 mdType;
	uint8 i = 0;
	
    if ((CanIfControllers[instance].PduMode == CANIF_OFFLINE)
		||(CanIfControllers[instance].PduMode == CANIF_TX_OFFLINE_ACTIVE)) {
		if(eventType == FLEXCAN_EVENT_RX_COMPLETE)
		{
			flexcan_msgbuff_t rcvMsgBufTemp;
			FLEXCAN_DRV_Receive(instance,  mbId, &rcvMsgBufTemp);
		}
        return;
    }

	gDiagRandomNumber++;
	dtcAppReportBusOffState(FALSE);
    switch (eventType) {
    case FLEXCAN_EVENT_RXFIFO_COMPLETE:
        mbId = 0;
		#if (STD_ON == CANIF_USE_RXFIFO)
        FLEXCAN_DRV_RxFifo(instance, CanIf_RecvMsgBuffFifo[instance]);
		#endif

    case FLEXCAN_EVENT_RX_COMPLETE:
        id = flexcanState->mbs[mbId].mb_message->msgId;
        dlc = flexcanState->mbs[mbId].mb_message->dataLen;
        data = flexcanState->mbs[mbId].mb_message->data;
        if (eventType == FLEXCAN_EVENT_RX_COMPLETE){
            FLEXCAN_DRV_Receive(instance,  mbId, &CanIf_RecvMsgBuff[instance][mbId]);
        }
		if(NULL != CanIf_RxPduCfg[instance]){
			while (min <= max){
	            mid = (min + max) >> 1;

	            if (CanIf_RxPduCfg[instance][mid].id > id){
	                max = mid - 1;
	            } else if (CanIf_RxPduCfg[instance][mid].id == id) {
	                isMatch = 1;
	                pduId = CanIf_RxPduCfg[instance][mid].cbkId;
	                mdType = CanIf_RxPduCfg[instance][mid].usrType;
	                break;
	            } else {
	                min = mid + 1;
	            }
	        }
		}
        if (isMatch) {
            pduInfo.SduLength = dlc;
            pduInfo.SduDataPtr = data;
            if (CanIfUserRxIndications[mdType] != NULL) 
			{
                CanIfUserRxIndications[mdType](pduId, &pduInfo);
				
				//used in can nm:start
				CanNm_EcuRxIndication(0);
				if((0X35D == id)&&(0X03 == (pduInfo.SduDataPtr[1]&0X03)))
					gSleepM_WakeUp_Flag = TRUE ;
				//used in can nm:end

				//used for yang xiaolang MSG = 602,add by tjy @2019-10-28
				if( TRUE != gSleepM_WakeUp_Flag2 )
				{
					if(0X60D == id)
					{
						if(	(0X00 != (pduInfo.SduDataPtr[0]&0XFC)) ||
							(0X00 != (pduInfo.SduDataPtr[1]&0X09)) ||
							((0X00 != (pduInfo.SduDataPtr[1]&0XE0)) && (0X80 > (pduInfo.SduDataPtr[1]&0XE0))) ||
							(0X00 != (pduInfo.SduDataPtr[2]&0X04)) || 
							(0X00 != (pduInfo.SduDataPtr[3]&0X3E)))
						{
							gSleepM_WakeUp_Flag2 = TRUE;
						}
					}

					if(0X351 == id)
					{
						if((0X00 != (pduInfo.SduDataPtr[0]&0X01)) || (0X00 != (pduInfo.SduDataPtr[5]&0X0F)) || (0X00 != (pduInfo.SduDataPtr[6]&0X80)))
						{
							gSleepM_WakeUp_Flag2 = TRUE ;
						}
					}
				}
            }
        }
        break;

    case FLEXCAN_EVENT_TX_COMPLETE:
		#if 0
        pduId = CanIf_TxPduCfg[mbId - CanTxStartIndex[instance]].cbkId;
        mdType =  CanIf_TxPduCfg[mbId - CanTxStartIndex[instance]].usrType;
        if (CanIfUserTxConfirmations[mdType] != NULL)
        {
            CanIfUserTxConfirmations[mdType](pduId);
        }

		#else
		id = flexcanState->mbs[mbId].mb_message->msgId;
		for(i=0;i < CANIF_TX_PDU_NUM;i++){
			if(CanIf_TxPduCfg[i].canId == id){
				pduId = CanIf_TxPduCfg[i].cbkId;
				mdType = CanIf_TxPduCfg[i].usrType;
				if (CanIfUserTxConfirmations[mdType] != NULL){
					CanIfUserTxConfirmations[mdType](pduId);
				}
				break;
			}
		}
		#endif
        break;

#if FEATURE_CAN_HAS_PRETENDED_NETWORKING
    case FLEXCAN_EVENT_WAKEUP_TIMEOUT: 
		break;
    case FLEXCAN_EVENT_WAKEUP_MATCH:
        pduId = 2;
        break;
	case FLEXCAN_EVENT_SELF_WAKEUP: 
		break; 
#endif
    }
}

void CanIfBusOffNotify(uint8 instance)
{
	gDiagRandomNumber = 0;
	if (CanIf_BusOffCbk != NULL) {
		CanIf_BusOffCbk(instance);
	}
	if (Can_BusOff[instance] == FALSE) {
		Can_BusOff[instance] = TRUE;
	}
}

boolean CanIf_GetBusOffState(uint8 instance)
{
	return Can_BusOff[instance];
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

