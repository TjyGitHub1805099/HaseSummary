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
#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_
#include "can2_cfg.h"
#include <null.h>
#include "ComStack_Types.h"

#define CAN_CHN_CFG_2					2

#define canFifoFilterFormat_2			FLEXCAN_RX_FIFO_ID_FORMAT_A


#define CanInitCfg(Instance)			can##Instance##_InitConfig
#define CanState(Instance)				can##Instance##_cfg_State
#define CanFifoFilterFormat(Instance)	canFifoFilterFormat_##Instance
#define CanPnConfig(Instance)			can##Instance##_PNConfig
#define CanIdFilterTable(Instance)		canFilterIdTable_##Instance
#define CanIdFilter(Instance)			canIdFilter_##Instance

#define CanIdFilterMask(Instance)	canIdFilteMask_##Instance
#define CanNumOfFilter(Instance)	canNumOfFilter_##Instance
#define CanNumOfRxPdu(Instance)		canNumOfRxPdu_##Instance
#define CanNumOfTxPdu(Instance)		canNumOfTxPdu_##Instance
#define CanNumOfMask(Instance)		canNumOfMask_##Instance
#define CanRxPduInf(Instance)		can##Instance##_RxPduInf



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
	uint8_t instance;
	uint32_t canId;
	CanIf_userType usrType;
	flexcan_msgbuff_id_type_t canIdType;
	uint8_t cbkId;

} CanIf_txPduCfgType;

typedef struct
{
	CanIf_userType usrType;
	uint32_t id;
	uint16_t cbkId;

} CanIf_patchTdType;

#ifdef CAN_CHN_CFG_0
#define CanIdFilterCfg(CAN_CHN_CFG_0)		CanIdFilter(CAN_CHN_CFG_0)
#define CanIdFilterTableCfg(CAN_CHN_CFG_0)	CanIdFilterTable(CAN_CHN_CFG_0)
#define CanIdFilterMaskCfg(CAN_CHN_CFG_0)	CanIdFilterMask(CAN_CHN_CFG_0)
#define CanNumOfFilterCfg(CAN_CHN_CFG_0)	CanNumOfFilter(CAN_CHN_CFG_0)
#define CanNumOfRxPduCfg(CAN_CHN_CFG_0)		CanNumOfRxPdu(CAN_CHN_CFG_0)
#define CanNumOfTxPduCfg(CAN_CHN_CFG_0)		CanNumOfTxPdu(CAN_CHN_CFG_0)
#define CanNumOfMaskCfg(CAN_CHN_CFG_0)		CanNumOfMask(CAN_CHN_CFG_0)
#define CanRxPduInfCfg(CAN_CHN_CFG_0)		CanRxPduInf(CAN_CHN_CFG_0)

extern const flexcan_id_table_t CanIdFilterCfg(CAN_CHN_CFG_0);
extern const uint32_t CanIdFilterTableCfg(CAN_CHN_CFG_0)[];
extern const uint32_t CanIdFilterMaskCfg(CAN_CHN_CFG_0)[];
extern const uint32_t CanNumOfFilterCfg(CAN_CHN_CFG_0);
extern const uint32_t CanNumOfRxPduCfg(CAN_CHN_CFG_0);
extern const uint32_t CanNumOfTxPduCfg(CAN_CHN_CFG_0);
extern const uint32_t CanNumOfMaskCfg(CAN_CHN_CFG_0);
extern const CanIf_patchTdType  CanRxPduInfCfg(CAN_CHN_CFG_0)[]
#endif

#ifdef CAN_CHN_CFG_1
#define CanIdFilterCfg(CAN_CHN_CFG_1)	CanIdFilter(CAN_CHN_CFG_1)
#define CanIdFilterTableCfg(CAN_CHN_CFG_1)	CanIdFilterTable(CAN_CHN_CFG_1)
#define CanIdFilterMaskCfg(CAN_CHN_CFG_1)		CanIdFilterMask(CAN_CHN_CFG_1)
#define CanNumOfFilterCfg(CAN_CHN_CFG_1)		CanNumOfFilter(CAN_CHN_CFG_1)
#define CanNumOfRxPduCfg(CAN_CHN_CFG_1)		CanNumOfRxPdu(CAN_CHN_CFG_1)
#define CanNumOfTxPduCfg(CAN_CHN_CFG_1)		CanNumOfTxPdu(CAN_CHN_CFG_1)
#define CanNumOfMaskCfg(CAN_CHN_CFG_1)		CanNumOfMask(CAN_CHN_CFG_1)
#define CanRxPduInfCfg(CAN_CHN_CFG_1)		CanRxPduInf(CAN_CHN_CFG_1)

extern const flexcan_id_table_t CanIdFilterCfg(CAN_CHN_CFG_1);
extern const uint32_t CanIdFilterTableCfg(CAN_CHN_CFG_1)[];
extern const uint32_t CanIdFilterMaskCfg(CAN_CHN_CFG_1)[];
extern const uint32_t CanNumOfFilterCfg(CAN_CHN_CFG_1);
extern const uint32_t CanNumOfRxPduCfg(CAN_CHN_CFG_1);
extern const uint32_t CanNumOfTxPduCfg(CAN_CHN_CFG_1);
extern const uint32_t CanNumOfMaskCfg(CAN_CHN_CFG_1);
extern const CanIf_patchTdType  CanRxPduInfCfg(CAN_CHN_CFG_1)[]
#endif

#ifdef CAN_CHN_CFG_2
#define CanIdFilterCfg(CAN_CHN_CFG_2)	CanIdFilter(CAN_CHN_CFG_2)
#define CanIdFilterTableCfg(CAN_CHN_CFG_2)	CanIdFilterTable(CAN_CHN_CFG_2)
#define CanIdFilterMaskCfg(CAN_CHN_CFG_2)		CanIdFilterMask(CAN_CHN_CFG_2)
#define CanNumOfFilterCfg(CAN_CHN_CFG_2)		CanNumOfFilter(CAN_CHN_CFG_2)
#define CanNumOfRxPduCfg(CAN_CHN_CFG_2)		CanNumOfRxPdu(CAN_CHN_CFG_2)
#define CanNumOfTxPduCfg(CAN_CHN_CFG_2)		CanNumOfTxPdu(CAN_CHN_CFG_2)
#define CanNumOfMaskCfg(CAN_CHN_CFG_2)		CanNumOfMask(CAN_CHN_CFG_2)
#define CanRxPduInfCfg(CAN_CHN_CFG_2)		CanRxPduInf(CAN_CHN_CFG_2)

extern const flexcan_id_table_t CanIdFilterCfg(CAN_CHN_CFG_2);
extern const uint32_t CanIdFilterTableCfg(CAN_CHN_CFG_2)[];
extern const uint32_t CanIdFilterMaskCfg(CAN_CHN_CFG_2)[];
extern const uint32_t CanNumOfFilterCfg(CAN_CHN_CFG_2);
extern const uint32_t CanNumOfRxPduCfg(CAN_CHN_CFG_2);
extern const uint32_t CanNumOfTxPduCfg(CAN_CHN_CFG_2);
extern const uint32_t CanNumOfMaskCfg(CAN_CHN_CFG_2);
extern const CanIf_patchTdType  CanRxPduInfCfg(CAN_CHN_CFG_2)[];
#endif


extern const CanIf_txPduCfgType  CanIf_txPduCfg[];
extern const void (*CanIfUserTxConfirmations[])(PduIdType TxPduId);
extern const void (*CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
extern const void (*CanIfBusOffNotify)(uint8_t instance);
#endif
