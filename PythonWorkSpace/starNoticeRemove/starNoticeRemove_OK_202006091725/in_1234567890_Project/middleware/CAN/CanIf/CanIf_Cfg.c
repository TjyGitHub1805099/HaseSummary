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

#include "CanIf_Cfg.h"
#include "tp_if.h"
#include "Com_Cbk.h"
#include "Com_Cfg.h"
//#include "CanNm_Cbk.h"
#include "CanIf.h"
#include "S32K148.h"
#include "Com.h"
#include "can0_cfg.h"
#include "can1_cfg.h"
#include "CanMixProc_if.h"


extern void CanNm_BusOffCbk( uint8_t canInstance );
//const void (*CanIf_BusOffCbk)(uint8_t instance) = CanNm_BusOffCbk;
void (* const CanIf_BusOffCbk)(uint8_t instance) = CanNm_BusOffCbk;

#ifdef CAN_CHN_CFG_0
flexcan_msgbuff_t can0RecvBuffFifo __attribute__((section(".canSectionData")));
flexcan_msgbuff_t can0RecvBuff[32] __attribute__((section(".canSectionData")));
#endif
#ifdef CAN_CHN_CFG_1
flexcan_msgbuff_t can1RecvBuffFifo __attribute__((section(".canSectionData")));
flexcan_msgbuff_t can1RecvBuff[32] __attribute__((section(".canSectionData")));
#endif
#ifdef CAN_CHN_CFG_2
flexcan_msgbuff_t can2RecvBuffFifo __attribute__((section(".canSectionData")));
flexcan_msgbuff_t can2RecvBuff[32] __attribute__((section(".canSectionData")));
#endif

#if (CANIF_USE_RXFIFO == STD_ON)
const flexcan_msgbuff_t *CanIf_RecvMsgBuffFifo[] = {
#ifdef CAN_CHN_CFG_0
	&can0RecvBuffFifo,
#else
	NULL,
#endif
	NULL,
	NULL,
};
#endif

/********************************************************************
MODULE: All Tx Id
Author: Yi
Version: V1.0
Timestamp: 2018-11-13 09:46:59
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

const CanIf_TxPduCfgType  CanIf_TxPduCfg[CANIF_TX_PDU_NUM] = //Include all physic channel pdu
{	
	{
		// 1
		.instance = CAN_CHN_CFG_0, 
        .canId = 0x280,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 0,
	},
	{
		// 2
		.instance = CAN_CHN_CFG_0,
        .canId = 0x2de,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 1,
	},
	{
		// 3
		//.instance = CAN_CHN_CFG_1,
		.instance = CAN_CHN_CFG_0,
        .canId = 0x355,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 2,
	},
	{
		// 4
		.instance = CAN_CHN_CFG_1,
        .canId = 0x447,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 3,
	},
	{
		// 5
		.instance = CAN_CHN_CFG_0,
        .canId = 0x4f8,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 4,
	},
	{
		// 6
		.instance = CAN_CHN_CFG_1,
        .canId = 0x567,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 5,
	},
	{
		// 7
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5a8,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 6,
	},
	{
		// 8
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5b7,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 7,
	},
	{
		// 9
		.instance = CAN_CHN_CFG_0,
        .canId = 0x5c5,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 8,
	},	
	{
		// 10
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5e5,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 9,
	},
	{
		// 11
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5fe,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 10,
	},
	{
		// 12
		.instance = CAN_CHN_CFG_0,
        .canId = 0x602,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 11,
	},	
	{
		// 13
		.instance = CAN_CHN_CFG_1,
        .canId = 0x635,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 12,
	},
	{
	    // 14
		.instance = CAN_CHN_CFG_0,
		.canId = 0x71D,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.usrType = CANIF_USER_TYPE_CAN_PDUR,
		.cbkId = CANIF_TX_PDU_CFG_CBK_ID_0,
	},
	{
		// 15
		.instance = CAN_CHN_CFG_1,
        .canId = 0x734,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_4,
	},
	{
		// 16
		.instance = CAN_CHN_CFG_0,
        .canId = 0x763,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_1,
	},
	{
		// 17
		.instance = CAN_CHN_CFG_1,
        .canId = 0x765,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_5,
	},
	{
		// 18
		.instance = CAN_CHN_CFG_1,
        .canId = 0x767,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_2,
	},
	{
		// 19
		.instance = CAN_CHN_CFG_1,
        .canId = 0x7ba,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_3,
	},
	
};
/*=========================== END OF MODULE: All Tx Id =====================*/



#ifdef CAN_CHN_CFG_0
/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
MODULE: Rx MASK
Author: Yi
Version: V1.0
Timestamp: 2018-11-13 09:46:59
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

CanIf_RxIdFilterType CanIf_RxIdFilter_0[CANIF_RX_FILTER_NUM_0] =
{
	{	//1 0x2a
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2a,
        .idMask = 0xffffffff,
    },
	{	//2 0x11a 0x11b
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x11a,
        .idMask = 0xfffffffe,
    },
	{	//3 0x160
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x160,
        .idMask = 0xffffffff,
    },
    {	//4 0x180
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x180,
        .idMask = 0xffffffff,
    },
	{	//5 0x218
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x218,
        .idMask = 0xffffffff,
    },
    {	//6 0x245
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x245,
        .idMask = 0xffffffff,
    },
	{	//7 0x284
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x284,
        .idMask = 0xffffffff,
    },
	{	//8 0x2b1 0x2b7
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2b1,
        .idMask = 0xfffffff9,
    },
    {	//9 0x2c8 0x2c9
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2c8,
        .idMask = 0xfffffffe,
    },	
	{	//10 0x351 0x354 0x358 0x35d
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x351,
        .idMask = 0xfffffff2,
    },	
	{	//11 0x385
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x385,
        .idMask = 0xffffffff,
    },
	{	//12 0x421
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x421,
        .idMask = 0xffffffff,
    },
	{	//13 0x4cb 0x4cc
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x4cb,
        .idMask = 0xfffffff8,
    }, 
    {	//14 0x502
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x502,
        .idMask = 0xffffffff,
    },
	{	//15 0x54c 0x551
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x54c,
        .idMask = 0xffffffe2,
    },
	{	//16 0x57a 0x580 0x58a
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x57a,
        .idMask = 0xffffff05,
    },
	{	//17 0x5e4 0x5e8 0x5e9
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5e4,
        .idMask = 0xfffffff2,
    },
	{	//18 0x60d
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x60d,
        .idMask = 0xffffffff,
    },
    {	//19 0x625 0x628 0x629
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x625,
        .idMask = 0xfffffff2,
    },
	{	//20 0x6e2
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x6e2,
        .idMask = 0xffffffff,
    },
    {	//21 0x72D,0x743
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x72d,
        .idMask = 0xffffff91,
	},
	{	//22 0x7DF
		.isRemoteFrame = STD_OFF,
		.isExtendedFrame = STD_OFF,
		.idFilter = 0x7DF,
		.idMask = 0xffffffff,
	},
};
/*=========================== END OF MODULE: Rx MASK ===========================*/



/********************************************************************
MODULE: All Rx Id
Author: Yi
Version: V1.0
Timestamp: 2018-11-13 09:46:59
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

const CanIf_RxPduCfgType CanIf_RxPduCfg_0[CANIF_RX_PDU_NUM_0] =
{
	{
		//1
		.id = 0x2a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ACU_GeneralStatus_6
	},
	{
		//2
		.id = 0x11a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ShBW_GeneralStatus
	},
	{
		//3
		.id = 0x11b,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SCU_GeneralStatus
	},
	{
		//4
		.id = 0x160,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_RN2
	},	
	{
		//5
		.id = 0x180,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_RN1
	},
	{
		//6
		.id = 0x218,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_PEPS_Message_Sts
	},
	{
		//7
		.id = 0x245,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Brake_StabilityControl_RN1
	},
	{
		//8
		.id = 0x284,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Front_Wheel_Speed_Frame
	},
	{
		//9
		.id = 0x2b1,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N1
	},
	{
		//10
		.id = 0x2b7,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N2
	},
	{
		//11
		.id = 0x2c8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SRR_Left
	},
	{
		//12
		.id = 0x2c9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SRR_Right
	},	
	{
		//13
		.id = 0x351,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SMART_bcmMSG_PUSH
	},
	{
		//14
		.id = 0x354,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Brake_GeneralData
	},
	{
		//15
		.id = 0x358,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCMMSG1
	},
	{
		//16
		.id = 0x35d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl
	},
	{
		//17
		.id = 0x385,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_TPMS_GeneralStatus_BCM
	},
	{
		//18
		.id = 0x421,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ATCVT_to_MMI_RN1
	},
	{
		//19
		.id = 0x4cb,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N3_7b
	},
	{
		//20
		.id = 0x4cc,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N2_1
	},
	{
		//21
		.id = 0x502,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_RETREIVE_ASSIST_POWER_48V
	},
	{
		//22
		.id = 0x54c,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_CLIMATE_ControlData_F
	},
	{
		//23
		.id = 0x551,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_GeneralStatus
	},
	{
		//24
		.id = 0x57a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SONAR_GeneralStatus_8
	},
	{
		//25
		.id = 0x580,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_to_ITM
	},
	{
		//26
		.id = 0x58a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_EPB_SystemControl
	},
	{
		//27
		.id = 0x5e4,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_EPS_GeneralStatus
	},
	{
		//28
		.id = 0x5e8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_TPMS_GeneralStatus2
	},
	{
		//29
		.id = 0x5e9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_APA_HMI_system
	},
	{
		//30
		.id = 0x60d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_GeneralStatus
	},
	{
		//31
		.id = 0x625,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_USM_GeneralStatus
	},
	{
		//32
		.id = 0x628,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LowWarningSignal628
	},
	{
		//33
		.id = 0x629,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LOW_WNG_STATUS_629
	},
	{
		//34
		.id = 0x6e2,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECCSMSG8_OBD_IS
	},
	{
		// 35
		.id = 0x72D,
		.usrType = CANIF_USER_TYPE_CAN_PDUR,
		.cbkId = 0
	},
	{
		//36
		.id = 0x743,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_1
	},
	{
		//37
		.id = 0x7DF,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_0
	},

};
/*=========================== END OF MODULE: All Rx Id =====================*/



#endif

#ifdef CAN_CHN_CFG_1
CanIf_RxIdFilterType CanIf_RxIdFilter_1[CANIF_RX_FILTER_NUM_1] = {
	{	//1 0x2
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2,
        .idMask = 0xffffffff,
    },
    {	//2 0x182
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x182,
        .idMask = 0xffffffff,
    },
	{	//3 0x1f9
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x1f9,
        .idMask = 0xffffffff,
    },		
	{	//4 0x352
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x352,
        .idMask = 0xffffffff,
    },
	{	//5 0x35a 0x35d
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x35a,
        .idMask = 0xfffffff8,
    },
	{	//6 0x563 
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x563,
        .idMask = 0xffffffff,
    },	
    {	//7 0x5c8 
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5c8,
        .idMask = 0xffffffff,
    },
	{	//8 0x5ca 0x5cb
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5ca,
        .idMask = 0xfffffffe,
    },
    {	//9 0x714
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x714,
        .idMask = 0xffffffff,
    },
    {	//10 0x747
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x747,
        .idMask = 0xffffffff,
    },
    {	//11 0x7b7 
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x7b7,
        .idMask = 0xffffffff,
    },
    {	//12 0x7DF
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x7df,
        .idMask = 0xffffffff,
    },
};

const CanIf_RxPduCfgType CanIf_RxPduCfg_1[CANIF_RX_PDU_NUM_1] = {
	{
		// 1
		.id = 0x2,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LWSOUT_M2
	},
	{
		// 2
		.id = 0x182,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_N1
	},
	{
		// 3
		.id = 0x1f9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_SystemControl_1f9
	},
	{
		// 4
		.id = 0x352,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_IKEY_352
	},
	{
		// 5
		.id = 0x35a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl_2
	},
	{
		// 6
		.id = 0x35d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl
	},
	{
		// 7
		.id = 0x563,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_DCM_GeneralStatus_5
	},
	{
		// 8
		.id = 0x5c8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_SetPM
	},
	{
		// 9
		.id = 0x5ca,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_enjoy
	},
	{
		// 10
		.id = 0x5cb,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_AC
	},
	{
		// 11
		.id = 0x714,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_4
	},
	{
		// 12
		.id = 0x745,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_5
	},
	{
		// 13
		.id = 0x747,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_2
	},
	{
		// 14
		.id = 0x7B7,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_3
	},
	{
		// 15
		.id = 0x7DF,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_0
	},
	
};

#endif
#ifdef CAN_CHN_CFG_2
CanIf_RxIdFilterType CanIf_RxIdFilter_2[CANIF_RX_FILTER_NUM_2] = {
	//NULL
};

const CanIf_RxPduCfgType CanIf_RxPduCfg_2[CANIF_RX_PDU_NUM_2] = {
	//NULL
};

#endif

flexcan_msgbuff_t *CanIf_RecvMsgBuff[] = {
	can0RecvBuff,//can0
	can1RecvBuff,//can1
	NULL,//can2
};

const flexcan_rx_fifo_id_element_format_t CanIf_RxFifoIdFormat[] = {
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	// can 0
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	// can 1
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	// can 2
};

#if 1	// if cpu is in HSRUN(112MHZ) mode, CAN bitrate can only be changed by manually
const flexcan_user_config_t can0_InitConfig_manual = {
    .fd_enable = false,
    .pe_clock = FLEXCAN_CLK_SOURCE_SYS,
    .max_num_mb = 32,
    .num_id_filters = FLEXCAN_RX_FIFO_ID_FILTERS_32,
    .is_rx_fifo_needed = false,
    .flexcanMode = FLEXCAN_NORMAL_MODE,
    .payload = FLEXCAN_PAYLOAD_SIZE_8,
    .bitrate = {
        .propSeg = 7,
        .phaseSeg1 = 2,
        .phaseSeg2 = 1,
        .preDivider = 15,
        .rJumpwidth = 1
    },
    .bitrate_cbt = {
        .propSeg = 7,
        .phaseSeg1 = 2,
        .phaseSeg2 = 1,
        .preDivider = 15,
        .rJumpwidth = 1
    },
    .transfer_type = FLEXCAN_RXFIFO_USING_INTERRUPTS,
    .rxFifoDMAChannel = 0U
};

const flexcan_user_config_t can1_InitConfig_manual = {
    .fd_enable = false,
    .pe_clock = FLEXCAN_CLK_SOURCE_SYS,
    .max_num_mb = 32,
    .num_id_filters = FLEXCAN_RX_FIFO_ID_FILTERS_32,
    .is_rx_fifo_needed = false,
    .flexcanMode = FLEXCAN_NORMAL_MODE,
    .payload = FLEXCAN_PAYLOAD_SIZE_8,
    .bitrate = {
        .propSeg = 7,
        .phaseSeg1 = 2,
        .phaseSeg2 = 1,
        .preDivider = 15,
        .rJumpwidth = 1
    },
    .bitrate_cbt = {
        .propSeg = 7,
        .phaseSeg1 = 2,
        .phaseSeg2 = 1,
        .preDivider = 15,
        .rJumpwidth = 1
    },
    .transfer_type = FLEXCAN_RXFIFO_USING_INTERRUPTS,
    .rxFifoDMAChannel = 0U
};

const flexcan_user_config_t *Can_InitConfig[] = {
	&can0_InitConfig_manual,
	&can1_InitConfig_manual,	// for CAN 1
	NULL,	// for CAN 2

};

#else
const flexcan_user_config_t *Can_InitConfig[] = {
	&can0_InitConfig,
	&can1_InitConfig,	// for CAN 1
	NULL,	// for CAN 2

};
#endif


volatile flexcan_state_t *Can_cfg_State[] = {
	&can0_cfg_State,
	&can1_cfg_State,	// for CAN 1
	NULL,	// for CAN 2

};

const CanIf_RxPduCfgType * const CanIf_RxPduCfg[CAN_INSTANCE_COUNT] =
{
 	&CanIf_RxPduCfg_0[0],
 	&CanIf_RxPduCfg_1[0],
 	NULL
};

const uint8 CanIf_RxFilterNum[] = {
	CANIF_RX_FILTER_NUM_0,
	CANIF_RX_FILTER_NUM_1,
	0,
};

const CanIf_RxIdFilterType *CanIf_RxIdFilter[] = {
	CanIf_RxIdFilter_0,
	CanIf_RxIdFilter_1,	// for CAN 1
	NULL,	// for CAN 2
};

const uint16 CanTxStartIndex[CAN_INSTANCE_COUNT] = {CANIF_RX_FILTER_NUM_0,CANIF_RX_FILTER_NUM_1,0};
const uint16 CanIfNumOfTxPdu[CAN_INSTANCE_COUNT] = {CANIF_TX_PDU_NUM_0,CANIF_TX_PDU_NUM_1,0};
const uint16 CanIfNumOfRxPdu[CAN_INSTANCE_COUNT] = {CANIF_RX_PDU_NUM_0,CANIF_RX_PDU_NUM_1,0};


//const void (*CanIfUserTxConfirmations[])(PduIdType TxPduId) =
void (*const CanIfUserTxConfirmations[])(PduIdType TxPduId) =
{
    NULL,
    NULL,// CanNm_TxConfirmation,
    Com_TxConfirmation,
    Tp_TxConfirmation,
    NULL,

};

//const void (*CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr) =
void (*const CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr) =
{
    NULL,
    NULL,//  CanNm_RxIndication,
    Com_RxIndication,
	Tp_RxIndication,
    CanMixProc_RxIndication,
};

