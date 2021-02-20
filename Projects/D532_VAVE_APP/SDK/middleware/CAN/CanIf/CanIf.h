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
#ifndef CANIF_H_
#define CANIF_H_

#include "flexcan_driver.h"
#include "flexcan_hw_access.h"
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
	
typedef struct
{
    CanIf_ControllerModeType ControllerMode;
    CanIf_PduModeType PduMode;

} CanIfControllerType;

typedef struct {
    boolean isRemoteFrame;      /*!< Remote frame*/
    boolean isExtendedFrame;    /*!< Extended frame*/
    uint32 idFilter;
	uint32 idMask;
} CanIf_RxIdFilterType;

typedef enum
{
	CANIF_USER_TYPE_CAN_OSEK_NM,
	CANIF_USER_TYPE_CAN_NM,
	CANIF_USER_TYPE_CAN_COM,
	CANIF_USER_TYPE_CAN_TP,
	CANIF_USER_TYPE_CAN_PDUR,
	CANIF_USER_TYPE_J1939TP
} CanIf_userType;

typedef enum
{
	/* Transceiver mode NORMAL */
	CANIF_TRCV_MODE_NORMAL = 0,
	/* Transceiver mode STANDBY */
	CANIF_TRCV_MODE_STANDBY,
	/* Transceiver mode SLEEP */
	CANIF_TRCV_MODE_SLEEP
} CanIf_TransceiverModeType;

typedef struct
{
	uint8 instance;
	uint32 canId;
	flexcan_msgbuff_id_type_t canIdType;
	CanIf_userType usrType;
	uint8 cbkId;
} CanIf_TxPduCfgType;

typedef struct
{
	uint32 id;
	CanIf_userType usrType;
	uint16 cbkId;
} CanIf_RxPduCfgType;

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

uint8 CanIf_SetControllerMode(uint8 Instance, CanIf_ControllerModeType ControllerMode);
uint8  CanIf_SetPduMode(uint8 Instance,CanIf_PduModeType PduModeRequest);
uint8 CanIf_Transmit(PduIdType CanTxPduId, const PduInfoType *PduInfoPtr);
uint8 CanIf_SetTransceiverMode(uint8 Transceiver,CanIf_TransceiverModeType TransceiverMode);
boolean CanIf_GetBusOffState(uint8 instance);
void CanIfBusOffNotify(uint8 instance);
boolean CanIf_GetDriverInitResult(uint8 instance);


#endif

