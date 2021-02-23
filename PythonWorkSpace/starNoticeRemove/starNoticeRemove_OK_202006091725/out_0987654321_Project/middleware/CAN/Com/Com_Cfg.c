




#include "Com_Cfg.h"
#include "Com.h"
#include "Com_Cbk.h"




const uint16 Com_RxTempQueueSize = COM_RX_TEMP_BUFFER_SIZE;
const uint16 Com_IpduMaxNum = COM_IPDU_MAX_NUM;
const uint16 Com_RxIpduMaxNum = COM_RX_IPDU_MAX_NUM;
const uint16 Com_TxIpduMaxNum = COM_TX_IPDU_MAX_NUM;
const uint16 Com_TxIpduStartIndex = COM_TX_IPDU_START_INDEX;





volatile uint8 Com_RxTempQueue[COM_RX_TEMP_BUFFER_SIZE][8];
PduIdType Com_RxTempQueueMgr[COM_RX_TEMP_BUFFER_SIZE];


volatile uint32 Com_RxTemp_TickBuf[COM_RX_TEMP_BUFFER_SIZE];
volatile uint32 Com_RxIpdu_TickBuf[COM_RX_IPDU_MAX_NUM];


uint8 Com_IpduBuf_0X2[8] =   {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2A[8] =  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X11A[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X11B[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X160[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X180[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X182[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X1F9[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X218[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X245[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X284[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2B1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2B7[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2C8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2C9[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X351[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X352[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X354[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X358[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X35A[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X35D[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X385[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X421[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X4CB[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X4CC[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X502[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X54C[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X551[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X563[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X57A[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X580[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X58A[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5C8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5CA[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5CB[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5E4[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5E8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5E9[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X60D[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X625[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X628[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X629[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X6E2[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

uint8 Com_IpduBuf_0X280[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X2DE[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X355[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X447[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X4F8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X567[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5A8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5B7[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5C5[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5E5[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X5FE[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8 Com_IpduBuf_0X602[1] = {0x00};
uint8 Com_IpduBuf_0X635[1] = {0x00};

uint8 * const Com_IpduBufPtr[] =
{
	&Com_IpduBuf_0X2[0],
	&Com_IpduBuf_0X2A[0],
	&Com_IpduBuf_0X11A[0],
	&Com_IpduBuf_0X11B[0],
	&Com_IpduBuf_0X160[0],
	&Com_IpduBuf_0X180[0],
	&Com_IpduBuf_0X182[0],
	&Com_IpduBuf_0X1F9[0],
	&Com_IpduBuf_0X218[0],
	&Com_IpduBuf_0X245[0],
	&Com_IpduBuf_0X284[0],
	&Com_IpduBuf_0X2B1[0],
	&Com_IpduBuf_0X2B7[0],
	&Com_IpduBuf_0X2C8[0],
	&Com_IpduBuf_0X2C9[0],
	&Com_IpduBuf_0X351[0],
	&Com_IpduBuf_0X352[0],
	&Com_IpduBuf_0X354[0],
	&Com_IpduBuf_0X358[0],
	&Com_IpduBuf_0X35A[0],
	&Com_IpduBuf_0X35D[0],
	&Com_IpduBuf_0X385[0],
	&Com_IpduBuf_0X421[0],
	&Com_IpduBuf_0X4CB[0],
	&Com_IpduBuf_0X4CC[0],
	&Com_IpduBuf_0X502[0],
	&Com_IpduBuf_0X54C[0],
	&Com_IpduBuf_0X551[0],
	&Com_IpduBuf_0X563[0],
	&Com_IpduBuf_0X57A[0],
	&Com_IpduBuf_0X580[0],
	&Com_IpduBuf_0X58A[0],
	&Com_IpduBuf_0X5C8[0],
	&Com_IpduBuf_0X5CA[0],
	&Com_IpduBuf_0X5CB[0],
	&Com_IpduBuf_0X5E4[0],
	&Com_IpduBuf_0X5E8[0],
	&Com_IpduBuf_0X5E9[0],
	&Com_IpduBuf_0X60D[0],
	&Com_IpduBuf_0X625[0],
	&Com_IpduBuf_0X628[0],
	&Com_IpduBuf_0X629[0],
	&Com_IpduBuf_0X6E2[0],
	
	&Com_IpduBuf_0X280[0],
	&Com_IpduBuf_0X2DE[0],
	&Com_IpduBuf_0X355[0],
	&Com_IpduBuf_0X447[0],
	&Com_IpduBuf_0X4F8[0],
	&Com_IpduBuf_0X567[0],
	&Com_IpduBuf_0X5A8[0],
	&Com_IpduBuf_0X5B7[0],
	&Com_IpduBuf_0X5C5[0],
	&Com_IpduBuf_0X5E5[0],
	&Com_IpduBuf_0X5FE[0],
	&Com_IpduBuf_0X602[0],
	&Com_IpduBuf_0X635[0],
};

uint32 * const Com_RxIpduQueueMgrPtr[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

uint8 * const Com_RxIpduQueuePtr[] =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};







Type1FuncPtrType const Com_IpduInformCbk[] =
{
	Com_RxIndicate_0X2_LWSOUT_M2,
	Com_RxIndicate_0X2A_ACU_GeneralStatus_6,
	Com_RxIndicate_0X11A_ShBW_GeneralStatus,
	Com_RxIndicate_0X11B_SCU_GeneralStatus,
	Com_RxIndicate_0X160_ECM_TorqueControl_RN2,
	Com_RxIndicate_0X180_ECM_TorqueControl_RN1,
	Com_RxIndicate_0X182_ECM_TorqueControl_N1,
	Com_RxIndicate_0X1F9_ECM_SystemControl_1f9,
	Com_RxIndicate_0X218_PEPS_Message_Sts,
	Com_RxIndicate_0X245_Brake_StabilityControl_RN1,
	Com_RxIndicate_0X284_Front_Wheel_Speed_Frame,
	Com_RxIndicate_0X2B1_ITS_InfoStatus_N1,
	Com_RxIndicate_0X2B7_ITS_InfoStatus_N2,
	Com_RxIndicate_0X2C8_SRR_Left,
	Com_RxIndicate_0X2C9_SRR_Right,
	Com_RxIndicate_0X351_SMART_bcmMSG_PUSH,
	Com_RxIndicate_0X352_IKEY_352,
	Com_RxIndicate_0X354_Brake_GeneralData,
	Com_RxIndicate_0X358_BCMMSG1,
	Com_RxIndicate_0X35A_BCM_SystemControl_2,
	Com_RxIndicate_0X35D_BCM_SystemControl,
	Com_RxIndicate_0X385_TPMS_GeneralStatus_BCM,
	Com_RxIndicate_0X421_ATCVT_to_MMI_RN1,
	Com_RxIndicate_0X4CB_ITS_InfoStatus_N3_7b,
	Com_RxIndicate_0X4CC_ITS_InfoStatus_N2_1,
	Com_RxIndicate_0X502_RETREIVE_ASSIST_POWER_48V,
	Com_RxIndicate_0X54C_CLIMATE_ControlData_F,
	Com_RxIndicate_0X551_ECM_GeneralStatus,
	Com_RxIndicate_0X563_DCM_GeneralStatus_5,
	Com_RxIndicate_0X57A_SONAR_GeneralStatus_8,
	Com_RxIndicate_0X580_ECM_to_ITM,
	Com_RxIndicate_0X58A_EPB_SystemControl,
	Com_RxIndicate_0X5C8_S_BOX_ControlData_SetPM,
	Com_RxIndicate_0X5CA_S_BOX_ControlData_enjoy,
	Com_RxIndicate_0X5CB_S_BOX_ControlData_AC,
	Com_RxIndicate_0X5E4_EPS_GeneralStatus,
	Com_RxIndicate_0X5E8_TPMS_GeneralStatus2,
	Com_RxIndicate_0X5E9_APA_HMI_system,
	Com_RxIndicate_0X60D_BCM_GeneralStatus,
	Com_RxIndicate_0X625_USM_GeneralStatus,
	Com_RxIndicate_0X628_LowWarningSignal628,
	Com_RxIndicate_0X629_LOW_WNG_STATUS_629,
	Com_RxIndicate_0X6E2_ECCSMSG8_OBD_IS,
	
	Com_TxNotify_0X280_CLUSTER_BasicInfo_1,
	Com_TxNotify_0X2DE_METMSG1,
	Com_TxNotify_0X355_CLUSTER_BasicInfo_2,
	Com_TxNotify_0X447_DA_Information,
	Com_TxNotify_0X4F8_CLUSTER_BasicInfo_3,
	Com_TxNotify_0X567_SystemTimeInfo,
	Com_TxNotify_0X5A8_DA_ControlData,
	Com_TxNotify_0X5B7_CLIMATE_ControlData,
	Com_TxNotify_0X5C5_CLUSTER_GeneralStatus,
	Com_TxNotify_0X5E5_IKEY_Display_SwitchStatus,
	Com_TxNotify_0X5FE_TCU_DA_information,
	Com_TxNotify_0X602_CLUSTER_WakeUpFrame,
	Com_TxNotify_0X635_DA_WakeUpFrame,
};

Type1FuncPtrType const Com_IpduErrorCbk[] =
{
	Com_RxTimeout_0X2_LWSOUT_M2,
	Com_RxTimeout_0X2A_ACU_GeneralStatus_6,
	Com_RxTimeout_0X11A_ShBW_GeneralStatus,
	Com_RxTimeout_0X11B_SCU_GeneralStatus,
	Com_RxTimeout_0X160_ECM_TorqueControl_RN2,
	Com_RxTimeout_0X180_ECM_TorqueControl_RN1,
	Com_RxTimeout_0X182_ECM_TorqueControl_N1,
	Com_RxTimeout_0X1F9_ECM_SystemControl_1f9,
	Com_RxTimeout_0X218_PEPS_Message_Sts,
	Com_RxTimeout_0X245_Brake_StabilityControl_RN1,
	Com_RxTimeout_0X284_Front_Wheel_Speed_Frame,
	Com_RxTimeout_0X2B1_ITS_InfoStatus_N1,
	Com_RxTimeout_0X2B7_ITS_InfoStatus_N2,
	Com_RxTimeout_0X2C8_SRR_Left,
	Com_RxTimeout_0X2C9_SRR_Right,
	Com_RxTimeout_0X351_SMART_bcmMSG_PUSH,
	Com_RxTimeout_0X352_IKEY_352,
	Com_RxTimeout_0X354_Brake_GeneralData,
	Com_RxTimeout_0X358_BCMMSG1,
	Com_RxTimeout_0X35A_BCM_SystemControl_2,
	Com_RxTimeout_0X35D_BCM_SystemControl,
	Com_RxTimeout_0X385_TPMS_GeneralStatus_BCM,
	Com_RxTimeout_0X421_ATCVT_to_MMI_RN1,
	Com_RxTimeout_0X4CB_ITS_InfoStatus_N3_7b,
	Com_RxTimeout_0X4CC_ITS_InfoStatus_N2_1,
	Com_RxTimeout_0X502_RETREIVE_ASSIST_POWER_48V,
	Com_RxTimeout_0X54C_CLIMATE_ControlData_F,
	Com_RxTimeout_0X551_ECM_GeneralStatus,
	Com_RxTimeout_0X563_DCM_GeneralStatus_5,
	Com_RxTimeout_0X57A_SONAR_GeneralStatus_8,
	Com_RxTimeout_0X580_ECM_to_ITM,
	Com_RxTimeout_0X58A_EPB_SystemControl,
	Com_RxTimeout_0X5C8_S_BOX_ControlData_SetPM,
	Com_RxTimeout_0X5CA_S_BOX_ControlData_enjoy,
	Com_RxTimeout_0X5CB_S_BOX_ControlData_AC,
	Com_RxTimeout_0X5E4_EPS_GeneralStatus,
	Com_RxTimeout_0X5E8_TPMS_GeneralStatus2,
	Com_RxTimeout_0X5E9_APA_HMI_system,
	Com_RxTimeout_0X60D_BCM_GeneralStatus,
	Com_RxTimeout_0X625_USM_GeneralStatus,
	Com_RxTimeout_0X628_LowWarningSignal628,
	Com_RxTimeout_0X629_LOW_WNG_STATUS_629,
	Com_RxTimeout_0X6E2_ECCSMSG8_OBD_IS,
	
	Com_TxError_0X280_CLUSTER_BasicInfo_1,
	Com_TxError_0X2DE_METMSG1,
	Com_TxError_0X355_CLUSTER_BasicInfo_2,
	Com_TxError_0X447_DA_Information,
	Com_TxError_0X4F8_CLUSTER_BasicInfo_3,
	Com_TxError_0X567_SystemTimeInfo,
	Com_TxError_0X5A8_DA_ControlData,
	Com_TxError_0X5B7_CLIMATE_ControlData,
	Com_TxError_0X5C5_CLUSTER_GeneralStatus,
	Com_TxError_0X5E5_IKEY_Display_SwitchStatus,
	Com_TxError_0X5FE_TCU_DA_information,
	Com_TxError_0X602_CLUSTER_WakeUpFrame,
	Com_TxError_0X635_DA_WakeUpFrame,
};








const PduIdType Com_SigId2IpduId[] =
{
	
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 
	IPDU_HND_LWSOUT_M2, 

	
	IPDU_HND_ACU_GeneralStatus_6, 
	IPDU_HND_ACU_GeneralStatus_6, 
	IPDU_HND_ACU_GeneralStatus_6, 
	IPDU_HND_ACU_GeneralStatus_6, 

	
	IPDU_HND_ShBW_GeneralStatus, 
	IPDU_HND_ShBW_GeneralStatus, 
	IPDU_HND_ShBW_GeneralStatus, 

	
	IPDU_HND_SCU_GeneralStatus, 
	IPDU_HND_SCU_GeneralStatus, 

	
	IPDU_HND_ECM_TorqueControl_RN2, 
	IPDU_HND_ECM_TorqueControl_RN2, 

	
	IPDU_HND_ECM_TorqueControl_RN1, 
	IPDU_HND_ECM_TorqueControl_RN1, 
	IPDU_HND_ECM_TorqueControl_RN1, 

	
	IPDU_HND_ECM_TorqueControl_N1, 

	
	IPDU_HND_ECM_SystemControl_1f9, 
	IPDU_HND_ECM_SystemControl_1f9, 

	
	IPDU_HND_PEPS_Message_Sts, 
	IPDU_HND_PEPS_Message_Sts, 

	
	IPDU_HND_Brake_StabilityControl_RN1, 
	IPDU_HND_Brake_StabilityControl_RN1, 

	
	IPDU_HND_Front_Wheel_Speed_Frame, 
	IPDU_HND_Front_Wheel_Speed_Frame, 
	IPDU_HND_Front_Wheel_Speed_Frame, 
	IPDU_HND_Front_Wheel_Speed_Frame, 
	IPDU_HND_Front_Wheel_Speed_Frame, 

	
	IPDU_HND_ITS_InfoStatus_N1, 
	IPDU_HND_ITS_InfoStatus_N1, 
	IPDU_HND_ITS_InfoStatus_N1, 
	IPDU_HND_ITS_InfoStatus_N1, 
	IPDU_HND_ITS_InfoStatus_N1, 
	IPDU_HND_ITS_InfoStatus_N1, 

	
	IPDU_HND_ITS_InfoStatus_N2, 
	IPDU_HND_ITS_InfoStatus_N2, 
	IPDU_HND_ITS_InfoStatus_N2, 
	IPDU_HND_ITS_InfoStatus_N2, 

	
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 
	IPDU_HND_SRR_Left, 

	
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 
	IPDU_HND_SRR_Right, 

	
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 
	IPDU_HND_SMART_bcmMSG_PUSH, 

	
	IPDU_HND_IKEY_352, 
	IPDU_HND_IKEY_352, 

	
	IPDU_HND_Brake_GeneralData, 
	IPDU_HND_Brake_GeneralData, 
	IPDU_HND_Brake_GeneralData, 
	IPDU_HND_Brake_GeneralData, 
	IPDU_HND_Brake_GeneralData, 

	
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 
	IPDU_HND_BCMMSG1, 

	
	IPDU_HND_BCM_SystemControl_2, 
	IPDU_HND_BCM_SystemControl_2, 
	IPDU_HND_BCM_SystemControl_2, 
	IPDU_HND_BCM_SystemControl_2, 
	IPDU_HND_BCM_SystemControl_2, 

	
	IPDU_HND_BCM_SystemControl, 
	IPDU_HND_BCM_SystemControl, 
	IPDU_HND_BCM_SystemControl, 
	IPDU_HND_BCM_SystemControl, 
	IPDU_HND_BCM_SystemControl, 

	
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 
	IPDU_HND_TPMS_GeneralStatus_BCM, 

	
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 
	IPDU_HND_ATCVT_to_MMI_RN1, 

	
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 
	IPDU_HND_ITS_InfoStatus_N3_7b, 

	
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 
	IPDU_HND_ITS_InfoStatus_N2_1, 

	
	IPDU_HND_RETREIVE_ASSIST_POWER_48V, 
	IPDU_HND_RETREIVE_ASSIST_POWER_48V, 
	IPDU_HND_RETREIVE_ASSIST_POWER_48V, 

	
	IPDU_HND_CLIMATE_ControlData_F, 

	
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 
	IPDU_HND_ECM_GeneralStatus, 

	
	IPDU_HND_DCM_GeneralStatus_5, 
	IPDU_HND_DCM_GeneralStatus_5, 
	IPDU_HND_DCM_GeneralStatus_5, 
	IPDU_HND_DCM_GeneralStatus_5, 
	IPDU_HND_DCM_GeneralStatus_5, 
	IPDU_HND_DCM_GeneralStatus_5, 

	
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 
	IPDU_HND_SONAR_GeneralStatus_8, 

	
	IPDU_HND_ECM_to_ITM, 
	IPDU_HND_ECM_to_ITM, 
	IPDU_HND_ECM_to_ITM, 
	IPDU_HND_ECM_to_ITM, 

	
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 
	IPDU_HND_EPB_SystemControl, 

	
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 
	IPDU_HND_S_BOX_ControlData_SetPM, 

	
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 
	IPDU_HND_S_BOX_ControlData_enjoy, 

	
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 
	IPDU_HND_S_BOX_ControlData_AC, 

	
	IPDU_HND_EPS_GeneralStatus, 

	
	IPDU_HND_TPMS_GeneralStatus2, 
	IPDU_HND_TPMS_GeneralStatus2, 
	IPDU_HND_TPMS_GeneralStatus2, 
	IPDU_HND_TPMS_GeneralStatus2, 
	IPDU_HND_TPMS_GeneralStatus2, 

	
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 
	IPDU_HND_APA_HMI_system, 

	
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 
	IPDU_HND_BCM_GeneralStatus, 

	
	IPDU_HND_USM_GeneralStatus, 
	IPDU_HND_USM_GeneralStatus, 
	IPDU_HND_USM_GeneralStatus, 
	IPDU_HND_USM_GeneralStatus, 
	IPDU_HND_USM_GeneralStatus, 

	
	IPDU_HND_LowWarningSignal628, 

	
	IPDU_HND_LOW_WNG_STATUS_629, 

	
	IPDU_HND_ECCSMSG8_OBD_IS, 
	IPDU_HND_ECCSMSG8_OBD_IS, 
	IPDU_HND_ECCSMSG8_OBD_IS, 
	IPDU_HND_ECCSMSG8_OBD_IS, 

	
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 
	IPDU_HND_CLUSTER_BasicInfo_1, 

	
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 
	IPDU_HND_METMSG1, 

	
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 
	IPDU_HND_CLUSTER_BasicInfo_2, 

	
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 
	IPDU_HND_DA_Information, 

	
	IPDU_HND_CLUSTER_BasicInfo_3, 
	IPDU_HND_CLUSTER_BasicInfo_3, 
	IPDU_HND_CLUSTER_BasicInfo_3, 
	IPDU_HND_CLUSTER_BasicInfo_3, 

	
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 
	IPDU_HND_SystemTimeInfo, 

	
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 
	IPDU_HND_DA_ControlData, 

	
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 
	IPDU_HND_CLIMATE_ControlData, 

	
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 
	IPDU_HND_CLUSTER_GeneralStatus, 

	
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 
	IPDU_HND_IKEY_Display_SwitchStatus, 

	
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 
	IPDU_HND_TCU_DA_information, 

	
	IPDU_HND_CLUSTER_WakeUpFrame, 

	
	IPDU_HND_DA_WakeUpFrame, 
};


const Com_SignalIdType Com_IpduId2SigId[][2] =
{
	{0, 6},
	{7, 10},
	{11, 13},
	{14, 15},
	{16, 17},
	{18, 20},
	{21, 21},
	{22, 23},
	{24, 25},
	{26, 27},
	{28, 32},
	{33, 38},
	{39, 42},
	{43, 52},
	{53, 62},
	{63, 75},
	{76, 77},
	{78, 82},
	{83, 91},
	{92, 96},
	{97, 101},
	{102, 122},
	{123, 130},
	{131, 147},
	{148, 167},
	{168, 170},
	{171, 171},
	{172, 182},
	{183, 188},
	{189, 214},
	{215, 218},
	{219, 230},
	{231, 245},
	{246, 257},
	{258, 271},
	{272, 272},
	{273, 277},
	{278, 287},
	{288, 298},
	{299, 303},
	{304, 304},
	{305, 305},
	{306, 309},
	{310, 318},
	{319, 338},
	{339, 348},
	{349, 355},
	{356, 359},
	{360, 366},
	{367, 381},
	{382, 413},
	{414, 421},
	{422, 428},
	{429, 435},
	{436, 436},
	{437, 437},
};








const PduIdType Com_IpduGroup_Rx[] = 
{
   
	44,
	IPDU_HND_LWSOUT_M2,
	IPDU_HND_ACU_GeneralStatus_6,
	IPDU_HND_ShBW_GeneralStatus,
	IPDU_HND_SCU_GeneralStatus,
	IPDU_HND_ECM_TorqueControl_RN2,
	IPDU_HND_ECM_TorqueControl_RN1,
	IPDU_HND_ECM_TorqueControl_N1,
	IPDU_HND_ECM_SystemControl_1f9,
	IPDU_HND_PEPS_Message_Sts,
	IPDU_HND_Brake_StabilityControl_RN1,
	IPDU_HND_Front_Wheel_Speed_Frame,
	IPDU_HND_ITS_InfoStatus_N1,
	IPDU_HND_ITS_InfoStatus_N2,
	IPDU_HND_SRR_Left,
	IPDU_HND_SRR_Right,
	IPDU_HND_SMART_bcmMSG_PUSH,
	IPDU_HND_IKEY_352,
	IPDU_HND_Brake_GeneralData,
	IPDU_HND_BCMMSG1,
	IPDU_HND_BCM_SystemControl_2,
	IPDU_HND_BCM_SystemControl,
	IPDU_HND_TPMS_GeneralStatus_BCM,
	IPDU_HND_ATCVT_to_MMI_RN1,
	IPDU_HND_ITS_InfoStatus_N3_7b,
	IPDU_HND_ITS_InfoStatus_N2_1,
	IPDU_HND_RETREIVE_ASSIST_POWER_48V,
	IPDU_HND_CLIMATE_ControlData_F,
	IPDU_HND_ECM_GeneralStatus,
	IPDU_HND_DCM_GeneralStatus_5,
	IPDU_HND_SONAR_GeneralStatus_8,
	IPDU_HND_ECM_to_ITM,
	IPDU_HND_EPB_SystemControl,
	IPDU_HND_S_BOX_ControlData_SetPM,
	IPDU_HND_S_BOX_ControlData_enjoy,
	IPDU_HND_S_BOX_ControlData_AC,
	IPDU_HND_EPS_GeneralStatus,
	IPDU_HND_TPMS_GeneralStatus2,
	IPDU_HND_APA_HMI_system,
	IPDU_HND_BCM_GeneralStatus,
	IPDU_HND_USM_GeneralStatus,
	IPDU_HND_LowWarningSignal628,
	IPDU_HND_LOW_WNG_STATUS_629,
	IPDU_HND_ECCSMSG8_OBD_IS,
};

const PduIdType Com_IpduGroup_Tx[] = 
{
   
	14,
	IPDU_HND_CLUSTER_BasicInfo_1,
	IPDU_HND_METMSG1,
	IPDU_HND_CLUSTER_BasicInfo_2,
	IPDU_HND_DA_Information,
	IPDU_HND_CLUSTER_BasicInfo_3,
	IPDU_HND_SystemTimeInfo,
	IPDU_HND_DA_ControlData,
	IPDU_HND_CLIMATE_ControlData,
	IPDU_HND_CLUSTER_GeneralStatus,
	IPDU_HND_IKEY_Display_SwitchStatus,
	IPDU_HND_TCU_DA_information,
	IPDU_HND_CLUSTER_WakeUpFrame,
	IPDU_HND_DA_WakeUpFrame,
};

const PduIdType *Com_IpduGroupPtr[] =
{
    Com_IpduGroup_Rx,
    Com_IpduGroup_Tx,
};








#if(0 < COM_RX_IPDU_MAX_NUM)
const Com_RxIpduConfType Com_RxIpduConfInfo[COM_RX_IPDU_MAX_NUM] =
{
	
	{
		5, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		5, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0 
	},
	
	{
		3, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		50, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		50, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		40, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		3, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		60, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		2, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		50, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		4, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0 
	},
	
	{
		6, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		3, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		5, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		500, 
		2000, 
		0, 
		0 
	},
	
	{
		5, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		6, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		4, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		4, 
		0, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0 
	},
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0 
	},
};
#endif







#if(0 < COM_TX_IPDU_MAX_NUM)
const Com_TxIpduConfType Com_TxIpduConfInfo[COM_TX_IPDU_MAX_NUM] =
{
	
	{
		8, 
		0, 
		COM_TX_RX_MODE_PERIODIC, 
		20, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		1, 
		COM_TX_RX_MODE_PERIODIC, 
		10, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		2, 
		COM_TX_RX_MODE_PERIODIC, 
		40, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		3, 
		COM_TX_RX_MODE_DIRECT, 
		0, 
		0, 
		0, 
		0, 
		0, 
		50, 
		3 
	},
	
	{
		8, 
		4, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		5, 
		COM_TX_RX_MODE_PERIODIC, 
		500, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		6, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		1 
	},
	
	{
		8, 
		7, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		20, 
		3 
	},
	
	{
		8, 
		8, 
		COM_TX_RX_MODE_PERIODIC, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
	
	{
		8, 
		9, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		100, 
		3 
	},
	
	{
		8, 
		10, 
		COM_TX_RX_MODE_MIXED, 
		100, 
		2000, 
		0, 
		0, 
		0, 
		20, 
		3 
	},
	
	{
		1, 
		11, 
		COM_TX_RX_MODE_DIRECT, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		1 
	},
	
	{
		1, 
		12, 
		COM_TX_RX_MODE_DIRECT, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		0 
	},
};
#endif








#if(0 < COM_SIGNAL_MAX_NUM)
const Com_SigConfType Com_SigConfInfo[COM_SIGNAL_MAX_NUM] =
{
    
    
	
	{
		7, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_SINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		25, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		24, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		26, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		3, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		3, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		3, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		21, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		21, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		57, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		10, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		34, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		25, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		4, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		4, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		32, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		44, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		45, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		52, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		60, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		48, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		41, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		34, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		62, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		12, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		12, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		14, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		54, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		52, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		53, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		49, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		22, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		2, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		22, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		27, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		2, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		14, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		38, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		34, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		33, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		45, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		80, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		255, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		254, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		46, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		41, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		54, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		53, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		32, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		62, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		20, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		52, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		42, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		33, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		27, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		24, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		62, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		50, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		24, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		42, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		2, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		7, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		1, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		22, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		17, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		27, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		26, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		25, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		24, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		38, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		42, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		46, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		45, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		42, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		60, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		58, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		22, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		21, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		42, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		38, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		34, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		20, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		18, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		54, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		49, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		41, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		48, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_MASKED_NEW_DIFFERS_MASKED_OLD, 65535, 0, 0, 0, 0, 0}
	},
	
    
    #if(0 < COM_TX_SIGNAL_MAX_NUM)
	
	{
		39, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		10, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		10, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		61, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		60, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		57, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		12, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		4, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		14, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		18, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		17, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		16, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		36, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		62, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		61, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		30, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		28, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		10, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		10, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT16, 
		2, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		22, 
		5, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		17, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		27, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		35, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		4, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		12, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		20, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		38, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		46, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		45, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		44, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		27, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		6, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		4, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		1, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		0, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		10, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		9, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		8, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		20, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		19, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		18, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		17, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		45, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		41, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		53, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		51, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		38, 
		7, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		62, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		61, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		16, 
		3, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		15, 
		24, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT32, 
		4, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		32, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		43, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		55, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		2, 
		1, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_BOOLEAN, 
		1, 
		COM_SIG_TX_PENDING, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		5, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		13, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		63, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		59, 
		4, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		3, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		11, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		23, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		21, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		31, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		29, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		39, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		37, 
		6, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		47, 
		2, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	
	{
		7, 
		8, 
		COM_SIG_MOTOR_BIG_ENDIAN, 
		COM_SIG_UINT8, 
		1, 
		COM_SIG_TX_TRIGGERED, 
		COM_RXTIMEOUTACTION_NONE, 
		0, 
		0, 
		
		{COM_F_ALWAYS, 65535, 0, 0, 0, 0, 0}
	},
	#endif
    
};
#endif





