




#include "Com_Cfg.h"
#include "Com.h"
#include "Com_Cbk.h"
#include "dea.h"
#include "dtcApp_cfg.h"
#include "dtcApp_if.h"
#include "CanApp.h"
#include "CanKey.h"
#include "System_Tick.h"
#include "Crc.h"
#include "pmStateMachine_If.h"
#include "Can_Nm.h"
#include "CanKey.h"


boolean gCanMsgRxSTRGTimeoutFlag = TRUE;

static void Set_RxIndicate(uint8 * buf, uint8 index)
{

	if((*buf & (1 << index)) == (1 << index))
	{
		*buf &= ~(1 << index);
		g_CanMsgRxTimeoutFlag = TRUE;
	}
	
}

static void Set_RxTimeout(uint8 * buf, uint8 index)
{
	if((*buf & (1 << index)) == 0)
	{
		*buf |= 1 << index;
		g_CanMsgRxTimeoutFlag = TRUE;
	}		
}

void Com_PudChangeCallback(PduIdType ipduId)
{
	IPC_SetUpdatedStat(ipduId, 1);
}





void Com_RxIndicate_0X2_LWSOUT_M2(uint32 data)
{
	gCanMsgRxSTRGTimeoutFlag = FALSE;
	DEA_SetCanTimeOutStat(IPDU_HND_LWSOUT_M2, 0U);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_STRG,IVI_RX_CAN_MSG_MASK_STRG_0X2,TRUE);
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_STRG,AVM_RX_CAN_MSG_MASK_STRG_0X2,TRUE);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_LWSOUT_M2>>3], IPDU_HND_LWSOUT_M2 & 0x7);
}
void Com_RxTimeout_0X2_LWSOUT_M2(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		gCanMsgRxSTRGTimeoutFlag = TRUE;
	    DEA_SetCanTimeOutStat(IPDU_HND_LWSOUT_M2, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_LWSOUT_M2>>3], IPDU_HND_LWSOUT_M2 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_STRG,IVI_RX_CAN_MSG_MASK_STRG_0X2,TRUE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_STRG,AVM_RX_CAN_MSG_MASK_STRG_0X2,TRUE);
	}
}

void Com_RxIndicate_0X2A_ACU_GeneralStatus_6(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ACU_GeneralStatus_6, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ACU_GeneralStatus_6>>3], IPDU_HND_ACU_GeneralStatus_6 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ACU,IC_RX_CAN_MSG_MASK_ACU_0X02A,FALSE);
}
void Com_RxTimeout_0X2A_ACU_GeneralStatus_6(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ACU_GeneralStatus_6, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ACU_GeneralStatus_6>>3], IPDU_HND_ACU_GeneralStatus_6 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ACU,IC_RX_CAN_MSG_MASK_ACU_0X02A,FALSE);
}

void Com_RxIndicate_0X11A_ShBW_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ShBW_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ShBW_GeneralStatus>>3], IPDU_HND_ShBW_GeneralStatus & 0x7);

}
void Com_RxTimeout_0X11A_ShBW_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ShBW_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ShBW_GeneralStatus>>3], IPDU_HND_ShBW_GeneralStatus & 0x7);
	}	
}

void Com_RxIndicate_0X11B_SCU_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_SCU_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_SCU_GeneralStatus>>3], IPDU_HND_SCU_GeneralStatus & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_SCU,IC_RX_CAN_MSG_MASK_SCU_0X11B,TRUE);
}
void Com_RxTimeout_0X11B_SCU_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_SCU_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_SCU_GeneralStatus>>3], IPDU_HND_SCU_GeneralStatus & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_SCU,IC_RX_CAN_MSG_MASK_SCU_0X11B,TRUE);
	}
}

void Com_RxIndicate_0X160_ECM_TorqueControl_RN2(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_RN2>>3], IPDU_HND_ECM_TorqueControl_RN2 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X160,TRUE);
}
void Com_RxTimeout_0X160_ECM_TorqueControl_RN2(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_RN2>>3], IPDU_HND_ECM_TorqueControl_RN2 & 0x7);
	}	

	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X160,TRUE);
	}
}

void Com_RxIndicate_0X180_ECM_TorqueControl_RN1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_RN1>>3], IPDU_HND_ECM_TorqueControl_RN1 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X180,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X180,FALSE);
}
void Com_RxTimeout_0X180_ECM_TorqueControl_RN1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_RN1>>3], IPDU_HND_ECM_TorqueControl_RN1 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X180,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X180,FALSE);
	}
}

void Com_RxIndicate_0X182_ECM_TorqueControl_N1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_N1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_N1>>3], IPDU_HND_ECM_TorqueControl_N1 & 0x7);
	
}
void Com_RxTimeout_0X182_ECM_TorqueControl_N1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_N1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_TorqueControl_N1>>3], IPDU_HND_ECM_TorqueControl_N1 & 0x7);
	}
		
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		
	}
}

void Com_RxIndicate_0X1F9_ECM_SystemControl_1f9(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_SystemControl_1f9, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_SystemControl_1f9>>3], IPDU_HND_ECM_SystemControl_1f9 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X1F9,FALSE);
}
void Com_RxTimeout_0X1F9_ECM_SystemControl_1f9(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_SystemControl_1f9, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_SystemControl_1f9>>3], IPDU_HND_ECM_SystemControl_1f9 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X1F9,FALSE);
	}
	
}

void Com_RxIndicate_0X218_PEPS_Message_Sts(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_PEPS_Message_Sts, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_PEPS_Message_Sts>>3], IPDU_HND_PEPS_Message_Sts & 0x7);
}
void Com_RxTimeout_0X218_PEPS_Message_Sts(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_PEPS_Message_Sts, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_PEPS_Message_Sts>>3], IPDU_HND_PEPS_Message_Sts & 0x7);
	}		
}

void Com_RxIndicate_0X245_Brake_StabilityControl_RN1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_Brake_StabilityControl_RN1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_Brake_StabilityControl_RN1>>3], IPDU_HND_Brake_StabilityControl_RN1 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_VDC_0X245,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_VDC_0X245,TRUE);
	
}
void Com_RxTimeout_0X245_Brake_StabilityControl_RN1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_Brake_StabilityControl_RN1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_Brake_StabilityControl_RN1>>3], IPDU_HND_Brake_StabilityControl_RN1 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_VDC_0X245,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_VDC_0X245,TRUE);
		
	}
}

void Com_RxIndicate_0X284_Front_Wheel_Speed_Frame(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_Front_Wheel_Speed_Frame>>3], IPDU_HND_Front_Wheel_Speed_Frame & 0x7);
	dtcApp_SetVehicleSpeedTimeoutFlag(FALSE);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_ABS_0X284,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_ABS_0X284,FALSE);
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,AVM_RX_CAN_MSG_MASK_ABS_0X284, FALSE);
}
void Com_RxTimeout_0X284_Front_Wheel_Speed_Frame(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_Front_Wheel_Speed_Frame>>3], IPDU_HND_Front_Wheel_Speed_Frame & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		dtcApp_SetVehicleSpeedTimeoutFlag(TRUE);
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_ABS_0X284,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_ABS_0X284,FALSE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,AVM_RX_CAN_MSG_MASK_ABS_0X284, FALSE);
	}
}

void Com_RxIndicate_0X2B1_ITS_InfoStatus_N1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N1>>3], IPDU_HND_ITS_InfoStatus_N1 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X2B1,TRUE);
}
void Com_RxTimeout_0X2B1_ITS_InfoStatus_N1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N1>>3], IPDU_HND_ITS_InfoStatus_N1 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X2B1,TRUE);
}

void Com_RxIndicate_0X2B7_ITS_InfoStatus_N2(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N2>>3], IPDU_HND_ITS_InfoStatus_N2 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_RADAR,IC_RX_CAN_MSG_MASK_RADAR_0X2B7,TRUE);
}
void Com_RxTimeout_0X2B7_ITS_InfoStatus_N2(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N2>>3], IPDU_HND_ITS_InfoStatus_N2 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_RADAR,IC_RX_CAN_MSG_MASK_RADAR_0X2B7,TRUE);
}

void Com_RxIndicate_0X2C8_SRR_Left(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_SRR_Left, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_SRR_Left>>3], IPDU_HND_SRR_Left & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_BSW,IC_RX_CAN_MSG_MASK_BSW_0X2C8,TRUE);
}
void Com_RxTimeout_0X2C8_SRR_Left(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_SRR_Left, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_SRR_Left>>3], IPDU_HND_SRR_Left & 0x7);
	}	

	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_BSW,IC_RX_CAN_MSG_MASK_BSW_0X2C8,TRUE);
	}
}

void Com_RxIndicate_0X2C9_SRR_Right(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_SRR_Right, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_SRR_Right>>3], IPDU_HND_SRR_Right & 0x7);
}
void Com_RxTimeout_0X2C9_SRR_Right(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_SRR_Right, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_SRR_Right>>3], IPDU_HND_SRR_Right & 0x7);
	}
}

void Com_RxIndicate_0X351_SMART_bcmMSG_PUSH(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_SMART_bcmMSG_PUSH, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_SMART_bcmMSG_PUSH>>3], IPDU_HND_SMART_bcmMSG_PUSH & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_IKEY,IC_RX_CAN_MSG_MASK_IKEY_0X351,TRUE);

	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_IKEY,IVI_RX_CAN_MSG_MASK_IKEY_0X351,TRUE);
}
void Com_RxTimeout_0X351_SMART_bcmMSG_PUSH(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_SMART_bcmMSG_PUSH, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_SMART_bcmMSG_PUSH>>3], IPDU_HND_SMART_bcmMSG_PUSH & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_IKEY,IC_RX_CAN_MSG_MASK_IKEY_0X351,TRUE);
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_IKEY,IVI_RX_CAN_MSG_MASK_IKEY_0X351,TRUE);
	}
}

void Com_RxIndicate_0X352_IKEY_352(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_IKEY_352, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_IKEY_352>>3], IPDU_HND_IKEY_352 & 0x7);
}
void Com_RxTimeout_0X352_IKEY_352(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_IKEY_352, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_IKEY_352>>3], IPDU_HND_IKEY_352 & 0x7);
	}	
}

void Com_RxIndicate_0X354_Brake_GeneralData(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_Brake_GeneralData, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_Brake_GeneralData>>3], IPDU_HND_Brake_GeneralData & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_ABS_0X354,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_ABS_0X354,FALSE);
}
void Com_RxTimeout_0X354_Brake_GeneralData(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_Brake_GeneralData, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_Brake_GeneralData>>3], IPDU_HND_Brake_GeneralData & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IVI_RX_CAN_MSG_MASK_ABS_0X354,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC,IC_RX_CAN_MSG_MASK_ABS_0X354,FALSE);
		
	}
}

void Com_RxIndicate_0X358_BCMMSG1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_BCMMSG1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_BCMMSG1>>3], IPDU_HND_BCMMSG1 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X358,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X358,FALSE);
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_BCM,AVM_RX_CAN_MSG_MASK_BCM_0X358, FALSE);

}
void Com_RxTimeout_0X358_BCMMSG1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_BCMMSG1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_BCMMSG1>>3], IPDU_HND_BCMMSG1 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X358,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X358,FALSE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_BCM,AVM_RX_CAN_MSG_MASK_BCM_0X358,FALSE);
	}
}
void Com_RxIndicate_0X35A_BCM_SystemControl_2(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_BCM_SystemControl_2, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_SystemControl_2>>3], IPDU_HND_BCM_SystemControl_2 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X35A,FALSE);
}
void Com_RxTimeout_0X35A_BCM_SystemControl_2(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_BCM_SystemControl_2, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_SystemControl_2>>3], IPDU_HND_BCM_SystemControl_2 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X35A,TRUE);
	}
}

void Com_RxIndicate_0X35D_BCM_SystemControl(uint32 data)
{
	
	uint8 msaterWakeUpCmd = 0 ;
	if( E_OK == Com_RxSig_GetBCM_WAKE_SLEEP_COM(&msaterWakeUpCmd) )
	{
		CanNm_RxIndication(0,msaterWakeUpCmd);
	}
	

	DEA_SetCanTimeOutStat(IPDU_HND_BCM_SystemControl, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_SystemControl>>3], IPDU_HND_BCM_SystemControl & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X35D,FALSE);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X35D,FALSE);
}
void Com_RxTimeout_0X35D_BCM_SystemControl(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_BCM_SystemControl, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_SystemControl>>3], IPDU_HND_BCM_SystemControl & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X35D,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X35D,FALSE);
	}
}

void Com_RxIndicate_0X385_TPMS_GeneralStatus_BCM(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_TPMS_GeneralStatus_BCM, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_TPMS_GeneralStatus_BCM>>3], IPDU_HND_TPMS_GeneralStatus_BCM & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_TPMS,IVI_RX_CAN_MSG_MASK_TPMS_0X385,TRUE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_TPMS,IC_RX_CAN_MSG_MASK_TPMS_0X385,TRUE);
}
void Com_RxTimeout_0X385_TPMS_GeneralStatus_BCM(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_TPMS_GeneralStatus_BCM, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_TPMS_GeneralStatus_BCM>>3], IPDU_HND_TPMS_GeneralStatus_BCM & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_TPMS,IVI_RX_CAN_MSG_MASK_TPMS_0X385,TRUE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_TPMS,IC_RX_CAN_MSG_MASK_TPMS_0X385,TRUE);
	}
}

void Com_RxIndicate_0X421_ATCVT_to_MMI_RN1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ATCVT_to_MMI_RN1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ATCVT_to_MMI_RN1>>3], IPDU_HND_ATCVT_to_MMI_RN1 & 0x7);
	DEA_SetGearDataRecive(1);	
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_CVT,IVI_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_CVT,IC_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_CVT,AVM_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
}
void Com_RxTimeout_0X421_ATCVT_to_MMI_RN1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ATCVT_to_MMI_RN1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ATCVT_to_MMI_RN1>>3], IPDU_HND_ATCVT_to_MMI_RN1 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_CVT,IVI_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_CVT,IC_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_CVT,AVM_RX_CAN_MSG_MASK_CVT_0X421,TRUE);
	}
}

void Com_RxIndicate_0X4CB_ITS_InfoStatus_N3_7b(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N3_7b, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N3_7b>>3], IPDU_HND_ITS_InfoStatus_N3_7b & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X4CB,TRUE);
}
void Com_RxTimeout_0X4CB_ITS_InfoStatus_N3_7b(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N3_7b, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N3_7b>>3], IPDU_HND_ITS_InfoStatus_N3_7b & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X4CB,TRUE);
}

void Com_RxIndicate_0X4CC_ITS_InfoStatus_N2_1(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2_1, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N2_1>>3], IPDU_HND_ITS_InfoStatus_N2_1 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_RADAR,IC_RX_CAN_MSG_MASK_RADAR_0X4CC,TRUE);
}
void Com_RxTimeout_0X4CC_ITS_InfoStatus_N2_1(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ITS_InfoStatus_N2_1, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ITS_InfoStatus_N2_1>>3], IPDU_HND_ITS_InfoStatus_N2_1 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_RADAR,IC_RX_CAN_MSG_MASK_RADAR_0X4CC,TRUE);
}
void Com_RxIndicate_0X502_RETREIVE_ASSIST_POWER_48V(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_RETREIVE_ASSIST_POWER_48V, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_RETREIVE_ASSIST_POWER_48V>>3], IPDU_HND_RETREIVE_ASSIST_POWER_48V & 0x7);
}
void Com_RxTimeout_0X502_RETREIVE_ASSIST_POWER_48V(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_RETREIVE_ASSIST_POWER_48V, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_RETREIVE_ASSIST_POWER_48V>>3], IPDU_HND_RETREIVE_ASSIST_POWER_48V & 0x7);
	}	
}
void Com_RxIndicate_0X54C_CLIMATE_ControlData_F(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_CLIMATE_ControlData_F, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_CLIMATE_ControlData_F>>3], IPDU_HND_CLIMATE_ControlData_F & 0x7);
	
}
void Com_RxTimeout_0X54C_CLIMATE_ControlData_F(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_CLIMATE_ControlData_F, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_CLIMATE_ControlData_F>>3], IPDU_HND_CLIMATE_ControlData_F & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		
	}
}

void Com_RxIndicate_0X551_ECM_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_GeneralStatus>>3], IPDU_HND_ECM_GeneralStatus & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X551,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X551,FALSE);	
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_HCM,AVM_RX_CAN_MSG_MASK_HCM_0X551, FALSE);	
}
void Com_RxTimeout_0X551_ECM_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_GeneralStatus>>3], IPDU_HND_ECM_GeneralStatus & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_ECM,IVI_RX_CAN_MSG_MASK_ECM_0X551,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X551,FALSE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_HCM,AVM_RX_CAN_MSG_MASK_HCM_0X551, FALSE);
	}

}

void Com_RxIndicate_0X563_DCM_GeneralStatus_5(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_DCM_GeneralStatus_5, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_DCM_GeneralStatus_5>>3], IPDU_HND_DCM_GeneralStatus_5 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_TCU,IVI_RX_CAN_MSG_MASK_TCU_0X563,FALSE);
}

extern void PM_RxTimeoutClear_DAwakeupRequest(void);
extern void PM_RxTimeoutClear_AVM_Outdoor_Photo_Request(void);

void Com_RxTimeout_0X563_DCM_GeneralStatus_5(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_DCM_GeneralStatus_5, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_DCM_GeneralStatus_5>>3], IPDU_HND_DCM_GeneralStatus_5 & 0x7);

		
		PM_RxTimeoutClear_DAwakeupRequest();
		PM_RxTimeoutClear_AVM_Outdoor_Photo_Request();
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_TCU,IVI_RX_CAN_MSG_MASK_TCU_0X563,FALSE);

}

void Com_RxIndicate_0X57A_SONAR_GeneralStatus_8(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_SONAR_GeneralStatus_8, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_SONAR_GeneralStatus_8>>3], IPDU_HND_SONAR_GeneralStatus_8 & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_UPA,IVI_RX_CAN_MSG_MASK_UPA_0X57A,TRUE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_BSW,IC_RX_CAN_MSG_MASK_BSW_0X57A,TRUE);	
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_BSW,AVM_RX_CAN_MSG_MASK_BSW_0X57A,TRUE);
}
void Com_RxTimeout_0X57A_SONAR_GeneralStatus_8(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_SONAR_GeneralStatus_8, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_SONAR_GeneralStatus_8>>3], IPDU_HND_SONAR_GeneralStatus_8 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_UPA,IVI_RX_CAN_MSG_MASK_UPA_0X57A,TRUE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_BSW,IC_RX_CAN_MSG_MASK_BSW_0X57A,TRUE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_BSW,AVM_RX_CAN_MSG_MASK_BSW_0X57A,TRUE);
	}
}
void Com_RxIndicate_0X580_ECM_to_ITM(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECM_to_ITM, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_to_ITM>>3], IPDU_HND_ECM_to_ITM & 0x7);
	
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X580,TRUE);
}
void Com_RxTimeout_0X580_ECM_to_ITM(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECM_to_ITM, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECM_to_ITM>>3], IPDU_HND_ECM_to_ITM & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X580,TRUE);

	}
}

void Com_RxIndicate_0X58A_EPB_SystemControl(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_EPB_SystemControl, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_EPB_SystemControl>>3], IPDU_HND_EPB_SystemControl & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_VDC,IC_RX_CAN_MSG_MASK_VDC_0X58A,TRUE);
}
void Com_RxTimeout_0X58A_EPB_SystemControl(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_EPB_SystemControl, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_EPB_SystemControl>>3], IPDU_HND_EPB_SystemControl & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_VDC,IC_RX_CAN_MSG_MASK_VDC_0X58A,TRUE);
}

void Com_RxIndicate_0X5C8_S_BOX_ControlData_SetPM(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_SetPM, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_SetPM>>3], IPDU_HND_S_BOX_ControlData_SetPM & 0x7);
}
void Com_RxTimeout_0X5C8_S_BOX_ControlData_SetPM(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_SetPM, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_SetPM>>3], IPDU_HND_S_BOX_ControlData_SetPM & 0x7);
		gCanSwKeyVal.keyval = 0;
	}
}

void Com_RxIndicate_0X5CA_S_BOX_ControlData_enjoy(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_enjoy, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_enjoy>>3], IPDU_HND_S_BOX_ControlData_enjoy & 0x7);
}
void Com_RxTimeout_0X5CA_S_BOX_ControlData_enjoy(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_enjoy, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_enjoy>>3], IPDU_HND_S_BOX_ControlData_enjoy & 0x7);
	}
}

void Com_RxIndicate_0X5CB_S_BOX_ControlData_AC(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_AC, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_AC>>3], IPDU_HND_S_BOX_ControlData_AC & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_HVAC,IVI_RX_CAN_MSG_MASK_HVAC_0X5CB,FALSE);
	
}
void Com_RxTimeout_0X5CB_S_BOX_ControlData_AC(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_S_BOX_ControlData_AC, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_S_BOX_ControlData_AC>>3], IPDU_HND_S_BOX_ControlData_AC & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_HVAC,IVI_RX_CAN_MSG_MASK_HVAC_0X5CB,FALSE);
	}
	
}

void Com_RxIndicate_0X5E4_EPS_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_EPS_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_EPS_GeneralStatus>>3], IPDU_HND_EPS_GeneralStatus & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_EPS,IVI_RX_CAN_MSG_MASK_EPS_0X5E4,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_EPS,IC_RX_CAN_MSG_MASK_EPS_0X5E4,FALSE);
}
void Com_RxTimeout_0X5E4_EPS_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_EPS_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_EPS_GeneralStatus>>3], IPDU_HND_EPS_GeneralStatus & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_EPS,IVI_RX_CAN_MSG_MASK_EPS_0X5E4,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_EPS,IC_RX_CAN_MSG_MASK_EPS_0X5E4,FALSE);
	}
}

void Com_RxIndicate_0X5E8_TPMS_GeneralStatus2(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_TPMS_GeneralStatus2, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_TPMS_GeneralStatus2>>3], IPDU_HND_TPMS_GeneralStatus2 & 0x7);
	
}
void Com_RxTimeout_0X5E8_TPMS_GeneralStatus2(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_TPMS_GeneralStatus2, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_TPMS_GeneralStatus2>>3], IPDU_HND_TPMS_GeneralStatus2 & 0x7);
	}	
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		
	}
}

void Com_RxIndicate_0X5E9_APA_HMI_system(uint32 data)
{

}
void Com_RxTimeout_0X5E9_APA_HMI_system(uint32 data)
{

}

void Com_RxIndicate_0X60D_BCM_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_BCM_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_GeneralStatus>>3], IPDU_HND_BCM_GeneralStatus & 0x7);
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
	AVM_dtcAppReportCanCommuFault(FALSE,AVM_FAULT_INDEX_RX_CAN_MSG_BCM,AVM_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
}
void Com_RxTimeout_0X60D_BCM_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_BCM_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_BCM_GeneralStatus>>3], IPDU_HND_BCM_GeneralStatus & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_BCM,IVI_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_BCM,IC_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
		AVM_dtcAppReportCanCommuFault(TRUE,AVM_FAULT_INDEX_RX_CAN_MSG_BCM,AVM_RX_CAN_MSG_MASK_BCM_0X60D,FALSE);
	}
}

void Com_RxIndicate_0X625_USM_GeneralStatus(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_USM_GeneralStatus, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_USM_GeneralStatus>>3], IPDU_HND_USM_GeneralStatus & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_IPDM,IC_RX_CAN_MSG_MASK_IPDM_0X625,FALSE);

	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_RX_CAN_MSG_USM,IVI_RX_CAN_MSG_MASK_USM_0X625,FALSE);
}
void Com_RxTimeout_0X625_USM_GeneralStatus(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_USM_GeneralStatus, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_USM_GeneralStatus>>3], IPDU_HND_USM_GeneralStatus & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_IPDM,IC_RX_CAN_MSG_MASK_IPDM_0X625,FALSE);
		IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_RX_CAN_MSG_USM,IVI_RX_CAN_MSG_MASK_USM_0X625,FALSE);
	}
}

void Com_RxIndicate_0X628_LowWarningSignal628(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_LowWarningSignal628, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_LowWarningSignal628>>3], IPDU_HND_LowWarningSignal628 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_LCML,IC_RX_CAN_MSG_MASK_LCML_0X628,TRUE);
}
void Com_RxTimeout_0X628_LowWarningSignal628(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_LowWarningSignal628, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_LowWarningSignal628>>3], IPDU_HND_LowWarningSignal628 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_LCML,IC_RX_CAN_MSG_MASK_LCML_0X628,TRUE);
	}
}

void Com_RxIndicate_0X629_LOW_WNG_STATUS_629(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_LOW_WNG_STATUS_629, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_LOW_WNG_STATUS_629>>3], IPDU_HND_LOW_WNG_STATUS_629 & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_LCMR,IC_RX_CAN_MSG_MASK_LCMR_0X629,TRUE);
}
void Com_RxTimeout_0X629_LOW_WNG_STATUS_629(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_LOW_WNG_STATUS_629, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_LOW_WNG_STATUS_629>>3], IPDU_HND_LOW_WNG_STATUS_629 & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_LCMR,IC_RX_CAN_MSG_MASK_LCMR_0X629,TRUE);
	}
}

void Com_RxIndicate_0X6E2_ECCSMSG8_OBD_IS(uint32 data)
{
	DEA_SetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS, 0U);
	Set_RxIndicate(&g_CanAppPduTimeOutFlag[IPDU_HND_ECCSMSG8_OBD_IS>>3], IPDU_HND_ECCSMSG8_OBD_IS & 0x7);
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X6E2,FALSE);
}
void Com_RxTimeout_0X6E2_ECCSMSG8_OBD_IS(uint32 data)
{
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS)
	{
		DEA_SetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS, 1U);
		Set_RxTimeout(&g_CanAppPduTimeOutFlag[IPDU_HND_ECCSMSG8_OBD_IS>>3], IPDU_HND_ECCSMSG8_OBD_IS & 0x7);
	}
	if(data == COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS)
	{
		IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_RX_CAN_MSG_ECM,IC_RX_CAN_MSG_MASK_ECM_0X6E2,FALSE);
	}
}


uint8 CanApp_Tx_Count[COM_TX_IPDU_MAX_NUM];




void Com_TxNotify_0X280_CLUSTER_BasicInfo_1(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLUSTER_BasicInfo_1 - COM_TX_IPDU_START_INDEX]++;
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_TX_MUTE,IC_CAN_TX_MUTE,FALSE);
}
void Com_TxError_0X280_CLUSTER_BasicInfo_1(uint32 data)
{
	IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_TX_MUTE,IC_CAN_TX_MUTE,FALSE);
}

void Com_TxNotify_0X2DE_METMSG1(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_METMSG1 - COM_TX_IPDU_START_INDEX]++;
	IC_dtcAppReportCanCommuFault(FALSE,IC_FAULT_INDEX_TX_MUTE,IC_CAN_TX_MUTE,FALSE);
}
void Com_TxError_0X2DE_METMSG1(uint32 data)
{
	IC_dtcAppReportCanCommuFault(TRUE,IC_FAULT_INDEX_TX_MUTE,IC_CAN_TX_MUTE,FALSE);
}

void Com_TxNotify_0X355_CLUSTER_BasicInfo_2(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLUSTER_BasicInfo_2 - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X355_CLUSTER_BasicInfo_2(uint32 data)
{

}

void Com_TxNotify_0X447_DA_Information(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_DA_Information - COM_TX_IPDU_START_INDEX]++;
	
	
}
void Com_TxError_0X447_DA_Information(uint32 data)
{

}
void Com_TxNotify_0X4F8_CLUSTER_BasicInfo_3(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLUSTER_BasicInfo_3 - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X4F8_CLUSTER_BasicInfo_3(uint32 data)
{

}

void Com_TxNotify_0X567_SystemTimeInfo(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_SystemTimeInfo - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X567_SystemTimeInfo(uint32 data)
{

}

void Com_TxNotify_0X5A8_DA_ControlData(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_DA_ControlData - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X5A8_DA_ControlData(uint32 data)
{

}

void Com_TxNotify_0X5B7_CLIMATE_ControlData(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLIMATE_ControlData - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X5B7_CLIMATE_ControlData(uint32 data)
{

}

void Com_TxNotify_0X5C5_CLUSTER_GeneralStatus(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLUSTER_GeneralStatus - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X5C5_CLUSTER_GeneralStatus(uint32 data)
{

}

void Com_TxNotify_0X5E5_IKEY_Display_SwitchStatus(uint32 data)
{
	static uint8 s_value = 0;
	CanApp_Tx_Count[IPDU_HND_IKEY_Display_SwitchStatus - COM_TX_IPDU_START_INDEX]++;
	if(s_value < 15)
	{
		s_value++;
	}
	else
	{
		s_value = 0;

	}
	Com_SetSignal(SIG_HND_Message_counter, &s_value);
	CanApp_setMsg_0x5e5_CheckSum(SIG_HND_Message_counter);
}
void Com_TxError_0X5E5_IKEY_Display_SwitchStatus(uint32 data)
{

}

void Com_TxNotify_0X5FE_TCU_DA_information(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_TCU_DA_information - COM_TX_IPDU_START_INDEX]++;
	IVI_dtcAppReportCanCommuFault(FALSE,IVI_FAULT_INDEX_TX_MUTE,IVI_CAN_TX_MUTE,FALSE);
}
void Com_TxError_0X5FE_TCU_DA_information(uint32 data)
{
	IVI_dtcAppReportCanCommuFault(TRUE,IVI_FAULT_INDEX_TX_MUTE,IVI_CAN_TX_MUTE,FALSE);
}

void Com_TxNotify_0X602_CLUSTER_WakeUpFrame(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_CLUSTER_WakeUpFrame - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X602_CLUSTER_WakeUpFrame(uint32 data)
{

}

void Com_TxNotify_0X635_DA_WakeUpFrame(uint32 data)
{
	CanApp_Tx_Count[IPDU_HND_DA_WakeUpFrame - COM_TX_IPDU_START_INDEX]++;
}
void Com_TxError_0X635_DA_WakeUpFrame(uint32 data)
{

}


