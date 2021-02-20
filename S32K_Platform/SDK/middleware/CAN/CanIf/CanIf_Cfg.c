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

#include "CanIf_Cfg.h"
#include"tp_if.h"

const void (*CanIfBusOffNotify)(uint8_t instance) = NULL;

const uint32_t CanIdFilterTableCfg(CAN_CHN_CFG_2)[] =
{
		0X70,
		0X300,
		0X320,
		0X326,
		0X328,
		0X330,
		0X340,
		//0X350,
		//0X378,
		//0X391,
		0X700,
		0X7FF,
};

const flexcan_id_table_t CanIdFilterCfg(CAN_CHN_CFG_2) =
{
	.isExtendedFrame = 0,
	.isRemoteFrame = 0,
	.idFilter = CanIdFilterTableCfg(CAN_CHN_CFG_2)
};

const uint32_t CanIdFilterMaskCfg(CAN_CHN_CFG_2)[] =
{
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	//0xFFFFFFFF,
	//0xFFFFFFFF,
	//0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
};

const uint32_t CanNumOfTxPduCfg(CAN_CHN_CFG_2) = 6;
const CanIf_txPduCfgType  CanIf_txPduCfg[] =
{
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X60,
		.cbkId = 0,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X85,
		.cbkId = 1,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X393,
		.cbkId = 2,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X394,
		.cbkId = 3,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X395,
		.cbkId = 4,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X397,
		.cbkId = 5,
		.usrType = CANIF_USER_TYPE_CAN_COM,
	},
	{
		.instance = 2,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.canId = 0X701,
		.cbkId = 6,
		.usrType = CANIF_USER_TYPE_CAN_TP,
	},
};
const CanIf_patchTdType CanRxPduInfCfg(CAN_CHN_CFG_2)[] =
{
	{
		.id = 0X70,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 0,
	},
	{
		.id = 0X300,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 1,
	},
	{
		.id = 0X320,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 2,
	},
	{
		.id = 0X326,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 3,
	},
	{
		.id = 0X328,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 4,
	},
	{
		.id = 0X330,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 5,
	},
	{
		.id = 0X340,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 6,
	},
	/*
	{
		.id = 0X350,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 7,
	},
	{
		.id = 0X378,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 8,
	},
	{
		.id = 0X391,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = 9,
	},
	*/
	{
		.id = 0X700,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = 10,
	},
	{
		.id = 0X7ff,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = 11,
	},
};


const uint32_t CanNumOfFilterCfg(CAN_CHN_CFG_2) = sizeof(CanIdFilterTableCfg(CAN_CHN_CFG_2)) / sizeof(uint32_t);
const uint32_t CanNumOfRxPduCfg(CAN_CHN_CFG_2) = sizeof(CanRxPduInfCfg(CAN_CHN_CFG_2)) / sizeof(CanIf_patchTdType);
const uint32_t CanNumOfMaskCfg(CAN_CHN_CFG_2) = sizeof(CanIdFilterMaskCfg(CAN_CHN_CFG_2)) / sizeof(uint32_t);

const void (*CanIfUserTxConfirmations[])(PduIdType TxPduId) =
{
		NULL,
		NULL,
		NULL,
		Tp_TxConfirmation,
		NULL,

};

const void (*CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr) =
{
		NULL,
		NULL,
		NULL,
		Tp_RxIndication,
		NULL,
};






