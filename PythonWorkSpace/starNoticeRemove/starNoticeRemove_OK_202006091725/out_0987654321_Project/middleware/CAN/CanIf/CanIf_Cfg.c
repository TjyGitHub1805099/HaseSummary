

#include "CanIf_Cfg.h"
#include "tp_if.h"
#include "Com_Cbk.h"
#include "Com_Cfg.h"

#include "CanIf.h"
#include "S32K148.h"
#include "Com.h"
#include "can0_cfg.h"
#include "can1_cfg.h"
#include "CanMixProc_if.h"


extern void CanNm_BusOffCbk( uint8_t canInstance );

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



const CanIf_TxPduCfgType  CanIf_TxPduCfg[CANIF_TX_PDU_NUM] = 
{	
	{
		
		.instance = CAN_CHN_CFG_0, 
        .canId = 0x280,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 0,
	},
	{
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x2de,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 1,
	},
	{
		
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x355,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 2,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x447,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 3,
	},
	{
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x4f8,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 4,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x567,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 5,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5a8,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 6,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5b7,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 7,
	},
	{
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x5c5,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 8,
	},	
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5e5,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 9,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x5fe,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 10,
	},
	{
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x602,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 11,
	},	
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x635,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_COM,
        .cbkId = 12,
	},
	{
	    
		.instance = CAN_CHN_CFG_0,
		.canId = 0x71D,
		.canIdType =  FLEXCAN_MSG_ID_STD,
		.usrType = CANIF_USER_TYPE_CAN_PDUR,
		.cbkId = CANIF_TX_PDU_CFG_CBK_ID_0,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x734,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_4,
	},
	{
		
		.instance = CAN_CHN_CFG_0,
        .canId = 0x763,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_1,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x765,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_5,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x767,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_2,
	},
	{
		
		.instance = CAN_CHN_CFG_1,
        .canId = 0x7ba,
        .canIdType =  FLEXCAN_MSG_ID_STD,
        .usrType = CANIF_USER_TYPE_CAN_TP,
        .cbkId = CANIF_TX_PDU_CFG_CBK_ID_3,
	},
	
};




#ifdef CAN_CHN_CFG_0



CanIf_RxIdFilterType CanIf_RxIdFilter_0[CANIF_RX_FILTER_NUM_0] =
{
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2a,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x11a,
        .idMask = 0xfffffffe,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x160,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x180,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x218,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x245,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x284,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2b1,
        .idMask = 0xfffffff9,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2c8,
        .idMask = 0xfffffffe,
    },	
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x351,
        .idMask = 0xfffffff2,
    },	
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x385,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x421,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x4cb,
        .idMask = 0xfffffff8,
    }, 
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x502,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x54c,
        .idMask = 0xffffffe2,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x57a,
        .idMask = 0xffffff05,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5e4,
        .idMask = 0xfffffff2,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x60d,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x625,
        .idMask = 0xfffffff2,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x6e2,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x72d,
        .idMask = 0xffffff91,
	},
	{	
		.isRemoteFrame = STD_OFF,
		.isExtendedFrame = STD_OFF,
		.idFilter = 0x7DF,
		.idMask = 0xffffffff,
	},
};






const CanIf_RxPduCfgType CanIf_RxPduCfg_0[CANIF_RX_PDU_NUM_0] =
{
	{
		
		.id = 0x2a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ACU_GeneralStatus_6
	},
	{
		
		.id = 0x11a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ShBW_GeneralStatus
	},
	{
		
		.id = 0x11b,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SCU_GeneralStatus
	},
	{
		
		.id = 0x160,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_RN2
	},	
	{
		
		.id = 0x180,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_RN1
	},
	{
		
		.id = 0x218,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_PEPS_Message_Sts
	},
	{
		
		.id = 0x245,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Brake_StabilityControl_RN1
	},
	{
		
		.id = 0x284,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Front_Wheel_Speed_Frame
	},
	{
		
		.id = 0x2b1,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N1
	},
	{
		
		.id = 0x2b7,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N2
	},
	{
		
		.id = 0x2c8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SRR_Left
	},
	{
		
		.id = 0x2c9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SRR_Right
	},	
	{
		
		.id = 0x351,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SMART_bcmMSG_PUSH
	},
	{
		
		.id = 0x354,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_Brake_GeneralData
	},
	{
		
		.id = 0x358,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCMMSG1
	},
	{
		
		.id = 0x35d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl
	},
	{
		
		.id = 0x385,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_TPMS_GeneralStatus_BCM
	},
	{
		
		.id = 0x421,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ATCVT_to_MMI_RN1
	},
	{
		
		.id = 0x4cb,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N3_7b
	},
	{
		
		.id = 0x4cc,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ITS_InfoStatus_N2_1
	},
	{
		
		.id = 0x502,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_RETREIVE_ASSIST_POWER_48V
	},
	{
		
		.id = 0x54c,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_CLIMATE_ControlData_F
	},
	{
		
		.id = 0x551,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_GeneralStatus
	},
	{
		
		.id = 0x57a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_SONAR_GeneralStatus_8
	},
	{
		
		.id = 0x580,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_to_ITM
	},
	{
		
		.id = 0x58a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_EPB_SystemControl
	},
	{
		
		.id = 0x5e4,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_EPS_GeneralStatus
	},
	{
		
		.id = 0x5e8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_TPMS_GeneralStatus2
	},
	{
		
		.id = 0x5e9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_APA_HMI_system
	},
	{
		
		.id = 0x60d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_GeneralStatus
	},
	{
		
		.id = 0x625,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_USM_GeneralStatus
	},
	{
		
		.id = 0x628,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LowWarningSignal628
	},
	{
		
		.id = 0x629,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LOW_WNG_STATUS_629
	},
	{
		
		.id = 0x6e2,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECCSMSG8_OBD_IS
	},
	{
		
		.id = 0x72D,
		.usrType = CANIF_USER_TYPE_CAN_PDUR,
		.cbkId = 0
	},
	{
		
		.id = 0x743,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_1
	},
	{
		
		.id = 0x7DF,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_0
	},

};




#endif

#ifdef CAN_CHN_CFG_1
CanIf_RxIdFilterType CanIf_RxIdFilter_1[CANIF_RX_FILTER_NUM_1] = {
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x2,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x182,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x1f9,
        .idMask = 0xffffffff,
    },		
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x352,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x35a,
        .idMask = 0xfffffff8,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x563,
        .idMask = 0xffffffff,
    },	
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5c8,
        .idMask = 0xffffffff,
    },
	{	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x5ca,
        .idMask = 0xfffffffe,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x714,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x747,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x7b7,
        .idMask = 0xffffffff,
    },
    {	
		.isRemoteFrame = STD_OFF,
        .isExtendedFrame = STD_OFF,
        .idFilter = 0x7df,
        .idMask = 0xffffffff,
    },
};

const CanIf_RxPduCfgType CanIf_RxPduCfg_1[CANIF_RX_PDU_NUM_1] = {
	{
		
		.id = 0x2,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_LWSOUT_M2
	},
	{
		
		.id = 0x182,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_TorqueControl_N1
	},
	{
		
		.id = 0x1f9,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_ECM_SystemControl_1f9
	},
	{
		
		.id = 0x352,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_IKEY_352
	},
	{
		
		.id = 0x35a,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl_2
	},
	{
		
		.id = 0x35d,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_BCM_SystemControl
	},
	{
		
		.id = 0x563,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_DCM_GeneralStatus_5
	},
	{
		
		.id = 0x5c8,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_SetPM
	},
	{
		
		.id = 0x5ca,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_enjoy
	},
	{
		
		.id = 0x5cb,
		.usrType = CANIF_USER_TYPE_CAN_COM,
		.cbkId = IPDU_HND_S_BOX_ControlData_AC
	},
	{
		
		.id = 0x714,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_4
	},
	{
		
		.id = 0x745,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_5
	},
	{
		
		.id = 0x747,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_2
	},
	{
		
		.id = 0x7B7,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_3
	},
	{
		
		.id = 0x7DF,
		.usrType = CANIF_USER_TYPE_CAN_TP,
		.cbkId = CANIF_RX_PDU_CFG_CBK_ID_0
	},
	
};

#endif
#ifdef CAN_CHN_CFG_2
CanIf_RxIdFilterType CanIf_RxIdFilter_2[CANIF_RX_FILTER_NUM_2] = {
	
};

const CanIf_RxPduCfgType CanIf_RxPduCfg_2[CANIF_RX_PDU_NUM_2] = {
	
};

#endif

flexcan_msgbuff_t *CanIf_RecvMsgBuff[] = {
	can0RecvBuff,
	can1RecvBuff,
	NULL,
};

const flexcan_rx_fifo_id_element_format_t CanIf_RxFifoIdFormat[] = {
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	
	FLEXCAN_RX_FIFO_ID_FORMAT_A,	
};

#if 1	
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
	&can1_InitConfig_manual,	
	NULL,	

};

#else
const flexcan_user_config_t *Can_InitConfig[] = {
	&can0_InitConfig,
	&can1_InitConfig,	
	NULL,	

};
#endif


volatile flexcan_state_t *Can_cfg_State[] = {
	&can0_cfg_State,
	&can1_cfg_State,	
	NULL,	

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
	CanIf_RxIdFilter_1,	
	NULL,	
};

const uint16 CanTxStartIndex[CAN_INSTANCE_COUNT] = {CANIF_RX_FILTER_NUM_0,CANIF_RX_FILTER_NUM_1,0};
const uint16 CanIfNumOfTxPdu[CAN_INSTANCE_COUNT] = {CANIF_TX_PDU_NUM_0,CANIF_TX_PDU_NUM_1,0};
const uint16 CanIfNumOfRxPdu[CAN_INSTANCE_COUNT] = {CANIF_RX_PDU_NUM_0,CANIF_RX_PDU_NUM_1,0};



void (*const CanIfUserTxConfirmations[])(PduIdType TxPduId) =
{
    NULL,
    NULL,
    Com_TxConfirmation,
    Tp_TxConfirmation,
    NULL,

};


void (*const CanIfUserRxIndications[])(PduIdType RxPduId, const PduInfoType* PduInfoPtr) =
{
    NULL,
    NULL,
    Com_RxIndication,
	Tp_RxIndication,
    CanMixProc_RxIndication,
};

