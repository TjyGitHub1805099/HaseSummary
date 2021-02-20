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
#ifndef CANIF_H_
#define CANIF_H_
#include "S32K146.h"
#include "flexcan_driver.h"
#include "flexcan_hw_access.h"
#include "can2_cfg.h"
#include "CanIf_Cfg.h"
typedef enum
{
    CANIF_CS_UNINIT = 0,
    CANIF_CS_STOPPED,
    CANIF_CS_STARTED,
    CANIF_CS_SLEEP
} CanIf_ControllerModeType;

typedef enum
{
	/* = 0 Transmit and receive path of the
	corresponding channel are disabled 
	=> no communication mode
	*/
	CANIF_OFFLINE,
	
	/* 
	Transmit path of the corresponding
	channel is disabled. The receive path
	is enabled.
	*/	
	CANIF_TX_OFFLINE,
	
	/* 
	Transmit path of the corresponding
	channel is in offline active mode (see
	SWS_CANIF_00072). The receive
	path is disabled.
	*/	
	CANIF_TX_OFFLINE_ACTIVE,
	
	/* 
	CanIfTxOfflineActiveSupport = TRUE.
	CANIF_ONLINE Transmit and receive path of the
	corresponding channel are enabled
	=> full operation mode
	*/
	CANIF_ONLINE

} CanIf_PduModeType;


//
#ifdef CAN_CHN_CFG_0
extern void CanIf_Init_0();
#endif
#ifdef CAN_CHN_CFG_1
extern void CanIf_Init_1();
#endif
#ifdef CAN_CHN_CFG_2
extern void CanIf_Init_2();
#endif

uint8_t CanIf_SetControllerMode(uint8_t Instance, CanIf_ControllerModeType ControllerMode);
uint8_t  CanIf_SetPduMode(uint8_t Instance,CanIf_PduModeType PduModeRequest);
uint8_t CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr);
uint8_t CanIf_SetTransceiverMode(uint8_t Transceiver,CanIf_TransceiverModeType TransceiverMode);

#endif
