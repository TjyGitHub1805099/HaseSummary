




#include "Com_If.h"





extern uint8 (*Com_Sig_Value[])(Com_SignalIdType SignalId, void* SignalDataPtr);

uint8 Com_RxSig_GetSTRANGLE(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	sint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STRANGLE, &l_value);
	l_phyValue = 0.1 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTRSPEED(uint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	uint16 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STRSPEED, &l_value);
	l_phyValue = 4 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetLWSCAL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LWSCAL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLWSOK(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LWSOK, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHCKSUM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHCKSUM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSGCOUNT(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSGCOUNT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLWSTRIM(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LWSTRIM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAIRBAG_FAIL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AIRBAG_FAIL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSecond_ROW_L_BELT_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Second_ROW_L_BELT_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSecond_ROW_C_BELT_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Second_ROW_C_BELT_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSecond_ROW_R_BELT_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Second_ROW_R_BELT_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSBW_FALSTAT(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SBW_FALSTAT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetREFUSE_BUZZER(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_REFUSE_BUZZER, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSBW_WARNING_MSG(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SBW_WARNING_MSG, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSCU_WARNING_MSG_11B(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SCU_WARNING_MSG_11B, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALARM_BUZZER(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALARM_BUZZER, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetIDSTSTS(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_IDSTSTS, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetISACT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ISACT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTACHO(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TACHO, &l_value);
	l_phyValue = 0.125 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetSHIFT_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SHIFT_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRawPedal_APOFS(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RawPedal_APOFS, &l_value);
	l_phyValue = 0.125 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetVirtualAcceleratorAngle_ForTM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VirtualAcceleratorAngle_ForTM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEngineStatus_1f9(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EngineStatus_1f9, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetECM_48V_Status_Display(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ECM_48V_Status_Display, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPEPS_REMOTE_STS(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PEPS_REMOTE_STS, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRemote_disable_start(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Remote_disable_start, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetVDCFAIL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VDCFAIL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSGCNTRR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSGCNTRR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetVSO(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VSO, &l_value);
	l_phyValue = 0.01 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetWRFR(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WRFR, &l_value);
	l_phyValue = 0.0416 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetWRFL(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WRFL, &l_value);
	l_phyValue = 0.0416 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetMessageCounterFront(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MessageCounterFront, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHKSUMFR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHKSUMFR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARN_LANE_L(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARN_LANE_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARN_LANE_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARN_LANE_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALIVE_CNT_N1(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALIVE_CNT_N1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHECKSUM_N1(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHECKSUM_N1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHandOffStrgWhlDetnSta(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HandOffStrgWhlDetnSta, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLKADrvrTkovReq(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LKADrvrTkovReq, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPRE_WARN_FR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PRE_WARN_FR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAEB_ACTIVE(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AEB_ACTIVE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALIVE_CNT_N2(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALIVE_CNT_N2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHECKSUM_N2(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHECKSUM_N2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSFS(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SFS, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetUI(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_UI, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFAILD_DT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FAILD_DT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetBLOCK_DT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_BLOCK_DT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALT_ST(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALT_ST, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALT_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALT_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRIGHT_ID_FLG(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RIGHT_ID_FLG, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_CNT_SRR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_CNT_SRR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHKSUM_SRR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHKSUM_SRR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSRRL_SET(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SRRL_SET, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSFS_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SFS_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetUI_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_UI_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFAILD_DT_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FAILD_DT_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetBLOCK_DT_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_BLOCK_DT_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALT_ST_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALT_ST_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALT_IND_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALT_IND_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRIGHT_ID_FLG_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RIGHT_ID_FLG_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_CNT_SRR_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_CNT_SRR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHKSUM_SRR_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHKSUM_SRR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSRRR_SET_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SRRR_SET_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getremote_ENG_ST_STATE(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_remote_ENG_ST_STATE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARNING_REQ_PIC(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARNING_REQ_PIC, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRING_ILLUMI_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RING_ILLUMI_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARNING_REQ_BUZZER1(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARNING_REQ_BUZZER1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARNING_REQ_BUZZER2(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARNING_REQ_BUZZER2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARNING_REQ_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARNING_REQ_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetKEY_WARNING_BUZZER(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_KEY_WARNING_BUZZER, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAHL_IND_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AHL_IND_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetNo_INFOR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_No_INFOR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTARTER_CONT_STA(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STARTER_CONT_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetIGN1_FAILURE_WARNING(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_IGN1_FAILURE_WARNING, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetESCL_FAILURE_WARNING(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ESCL_FAILURE_WARNING, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSSB_FAILURE_WARNING(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SSB_FAILURE_WARNING, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPEPS_APUfbResult352(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PEPS_APUfbResult352, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPEPS_WALfbResult352(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PEPS_WALfbResult352, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetVDCStateForDAMenu(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VDCStateForDAMenu, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetVDC_LAMP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VDC_LAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetABS_LAMP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ABS_LAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEBD_LAMP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EBD_LAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHDC_LAMP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HDC_LAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAuto_Light_SW_Status(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Auto_Light_SW_Status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetOIL_P_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_OIL_P_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetKEY_NO(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_KEY_NO, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCLUSTER_ILL_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CLUSTER_ILL_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetH_POSITION(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_H_POSITION, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPMS_RFWL_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPMS_RFWL_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTRUNK_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TRUNK_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetGLASS_HATCH(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_GLASS_HATCH, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPMS_W_L_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPMS_W_L_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetBCM_Area2(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_BCM_Area2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDR_Window_status(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DR_Window_status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAS_Window_status(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AS_Window_status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRL_Window_status(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RL_Window_status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRR_Window_status(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RR_Window_status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetBCM_WAKE_SLEEP_COM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_BCM_WAKE_SLEEP_COM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCABRESW(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CABRESW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRLS_FW_value(uint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	uint16 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RLS_FW_value, &l_value);
	l_phyValue = 6 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetRLS_Amb_value(uint32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	uint32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RLS_Amb_value, &l_value);
	l_phyValue = 20 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetSe_IN_Re(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Se_IN_Re, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetD_STYLE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_D_STYLE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSYSTEM_FAIL_TPMS(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SYSTEM_FAIL_TPMS, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFT_FR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FT_FR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFT_FL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FT_FL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFT_RL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FT_RL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFT_RR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FT_RR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLTP_FL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LTP_FL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLTP_FR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LTP_FR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLTP_RL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LTP_RL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLTP_RR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LTP_RR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPD_FL(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPD_FL, &l_value);
	l_phyValue = 0.25 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPD_FR(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPD_FR, &l_value);
	l_phyValue = 0.25 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPD_RL(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPD_RL, &l_value);
	l_phyValue = 0.25 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPD_RR(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPD_RR, &l_value);
	l_phyValue = 0.25 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetTS_FL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TS_FL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTS_FR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TS_FR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTS_RL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TS_RL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTS_RR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TS_RR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTPMS_POSI_SYNC(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TPMS_POSI_SYNC, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPRESS_FRONT(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PRESS_FRONT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPRESS_REAR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PRESS_REAR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetIND_RNG(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_IND_RNG, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetATLAMP_INDSSIG_CVTLAMPSIG(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ATLAMP_INDSSIG_CVTLAMPSIG, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRGB_GP_REFUSE(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RGB_GP_REFUSE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTrFaultContBuz(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TrFaultContBuz, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTrFaultBuz(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TrFaultBuz, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTrFaultWdRemind(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TrFaultWdRemind, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRGB_PBP_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RGB_PBP_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTrFaultWdRemind2(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TrFaultWdRemind2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSET_LDW_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SET_LDW_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLDW_INFO(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LDW_INFO, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_LDW_TMP_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_LDW_TMP_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_LDW_PER_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_LDW_PER_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_CAM_BLK(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_CAM_BLK, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALIVE_CNT_N3_7b(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALIVE_CNT_N3_7b, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHECKSUM_N3_7b(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHECKSUM_N3_7b, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSET_LKA_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SET_LKA_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLKA_INFO(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LKA_INFO, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_HIGH_TEMP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_HIGH_TEMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_LKA_TMP_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_LKA_TMP_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_LKA_PER_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_LKA_PER_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_HANDSOFF_STDBY(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_HANDSOFF_STDBY, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTA_CHG_ALERT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STA_CHG_ALERT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_SW1(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_SW1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_SW2(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_SW2, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_SW3(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_SW3, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSET_PEBS_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SET_PEBS_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPEBS_INFO(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PEBS_INFO, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWARN_VRU(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WARN_VRU, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_TMP_ERR_N2_1(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_TMP_ERR_N2_1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_PER_ERR_N2_1(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_PER_ERR_N2_1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_BLKT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_BLKT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_VRU(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_VRU, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMSG_VDC(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MSG_VDC, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetALIVE_CNT_N2_1(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ALIVE_CNT_N2_1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHECKSUM_N2_1(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHECKSUM_N2_1, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_MSG_TMP_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_MSG_TMP_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_MSG_PER_ERR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_MSG_PER_ERR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_HEADW_SET(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_HEADW_SET, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_DrSeSp(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_DrSeSp, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_DisToTgt(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_DisToTgt, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_TgtDet(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_TgtDet, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_TextMessage(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_TextMessage, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_GoNoti(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_GoNoti, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_TOREQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_TOREQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_INFO(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_INFO, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRETREIVE_48V(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RETREIVE_48V, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetASSIST_48V(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ASSIST_48V, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRETREIVE_ASSIST_POWER_PERCENTAGE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RETREIVE_ASSIST_POWER_PERCENTAGE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetOutsideTemp_AD(float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	float32 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_OutsideTemp_AD, &l_value);
	l_phyValue = -40 + 0.5 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetTWN(sint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	sint16 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TWN, &l_value);
	l_phyValue = -40 + l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetCANMASK(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CANMASK, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLED(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LED, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTIM(uint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	uint16 l_phyValue = 0;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TIM, &l_value);
	l_phyValue = 80 * l_value;
	*value = l_phyValue;
	return (ReturnValue);
}


uint8 Com_RxSig_GetWTENP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WTENP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSPDREQ(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SPDREQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSPDBLREQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SPDBLREQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetASCDON_DISP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ASCDON_DISP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetENG_STA(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ENG_STA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetELAMP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ELAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetS48V_Waring_IND(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_S48V_Waring_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDA_wakeup_Request(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DA_wakeup_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDVR_CrashVideo_Request(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DVR_CrashVideo_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDVR_Photo_Request(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DVR_Photo_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetE_CALL_display_request(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_E_CALL_display_request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDA_Muteout_Request(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DA_Muteout_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAVM_OutdoorPhoto_Request(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVM_OutdoorPhoto_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSoundObstacleLevel_Request(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SoundObstacleLevel_Request, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetREQ_DISP_MET(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_REQ_DISP_MET, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTS_SETTING(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STS_SETTING, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRR_CEN(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RR_CEN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_RR_CEN(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_RR_CEN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRR_L(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RR_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_RR_L(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_RR_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRR_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_RR_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_RR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTATUS_Dynamic_EAPM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STATUS_Dynamic_EAPM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSETTEING_EAPM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SETTEING_EAPM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTATUS_Warning_EAPM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STATUS_Warning_EAPM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTS_AUTO_DISP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STS_AUTO_DISP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTS_SYS_MODE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STS_SYS_MODE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSOUND_EAPM(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SOUND_EAPM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetREQ_DISP_CEN(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_REQ_DISP_CEN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSONAR_S_REQ(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SONAR_S_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_CEN(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_CEN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_FR_CEN(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_FR_CEN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_L(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_FR_L(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_FR_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_R(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetA_FR_R(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_A_FR_R, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSTS_DISTANCE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_STS_DISTANCE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSOUND_ZONE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SOUND_ZONE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSOUND_CONT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SOUND_CONT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetENG_OIL_WARNING_LAMP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ENG_OIL_WARNING_LAMP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCAP_IND_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CAP_IND_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetGPF_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_GPF_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetGSINXTGP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_GSINXTGP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_CONG_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_CONG_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_WORKMODE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_WORKMODE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_STAT(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_STAT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_FAIL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_FAIL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_WARNMSG_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_WARNMSG_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_HAZARD_IND(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_HAZARD_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPB_BUZZER_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPB_BUZZER_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHKSUM(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHKSUM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAVH_ACT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVH_ACT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAVH_OFFSW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVH_OFFSW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDPB_STAT(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DPB_STAT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAVH_FAIL(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVH_FAIL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_LAMP_LEVEL(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_LAMP_LEVEL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_UP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_UP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_DOWN(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_DOWN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_LEFT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_LEFT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_RIGHT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_RIGHT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_OK(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_OK, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_DISP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_DISP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetL_SW_TEL_ON(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_L_SW_TEL_ON, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPESPONSE_ERRO_LEFT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PESPONSE_ERRO_LEFT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetR_SW_DVR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_R_SW_DVR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetR_SW_LKA(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_R_SW_LKA, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRESPONSE_ERRO_RIGHT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RESPONSE_ERRO_RIGHT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPinao_FR_DEFOGSW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Pinao_FR_DEFOGSW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPinao_RR_DEFOGSW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Pinao_RR_DEFOGSW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetOUTSIDEMIRROR_MODE_F(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_OUTSIDEMIRROR_MODE_F, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWPC_Status(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WPC_Status, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWPC_Error_Feedback(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WPC_Error_Feedback, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetWPC_Percentage(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_WPC_Percentage, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_LGHT_OnOff(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_LGHT_OnOff, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_LGHT_LinkedSwitch(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_LGHT_LinkedSwitch, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_LGHT_ThemeColor(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_LGHT_ThemeColor, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_LGHT_Color(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_LGHT_Color, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_LGHT_MusicRhythmSignal(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_LGHT_MusicRhythmSignal, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_COLOR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_COLOR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_MUSIC_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_MUSIC_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_TEM_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_TEM_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAMB_SPEEDING_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AMB_SPEEDING_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_compressor(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_compressor, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_fan_speed_eedback(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_fan_speed_eedback, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_mode(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_mode, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_temp_d(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_temp_d, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_auto(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_auto, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_onoff(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_onoff, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_dual(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_dual, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_temp_p(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_temp_p, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_recirc(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_recirc, value);
	return (ReturnValue);
}


uint8 Com_RxSig_Getac_rr_defog(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ac_rr_defog, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTarget_Temp_E(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_Target_Temp_E, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetMAX_AC(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_MAX_AC, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAUTO_MODE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AUTO_MODE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDEFROST_F(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DEFROST_F, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetEPSFAIL(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EPSFAIL, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetSELECT_PRESS_SUPPORT(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_SELECT_PRESS_SUPPORT, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRESET_TPMS(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RESET_TPMS, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetPSI_MES_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_PSI_MES_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetKPA_MES_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_KPA_MES_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetKGF_MES_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_KGF_MES_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_B_PR(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_B_PR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_R_INT_IN(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_R_INT_IN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_QU_INDICATION(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_QU_INDICATION, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_PA_STATE(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_PA_STATE, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_ST(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_ST, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_IN(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_IN, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_CPC(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_CPC, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_S_SI(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_S_SI, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_D_RQ(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_D_RQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetAPA_PR_BAR(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_APA_PR_BAR, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDOOR_SW(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DOOR_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTAIL_L_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TAIL_L_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHL_LOW_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HL_LOW_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetTURN_IND(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_TURN_IND, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHL_HIGH_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HL_HIGH_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_FOG_L_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_FOG_L_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetRR_FOG_L(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_RR_FOG_L, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetBUZZ_REQ(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_BUZZ_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetDTRL_REQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DTRL_REQ, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetIGN_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_IGN_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetACC_SW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ACC_SW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetOIL_P_SW_USM(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_OIL_P_SW_USM, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_WIP(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_WIP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHL_LOW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HL_LOW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetHL_HIGH(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_HL_HIGH, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetFR_FOG(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FR_FOG, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLOW_WNG_628(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LOW_WNG_628, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetLOW_WNG_629(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_LOW_WNG_629, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetISMSKSW(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ISMSKSW, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetISINDMD(uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ISINDMD, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetISSAP(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_ISSAP, value);
	return (ReturnValue);
}


uint8 Com_RxSig_GetCHRGLREQ(boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_CHRGLREQ, value);
	return (ReturnValue);
}


uint8 Com_TxSig_VSP20_MET(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.01);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_VSP20_MET, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VSP20_MET, &l_value);
		l_phyValue = 0.01 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_BAC_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_BAC_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DC_EXISTANCE(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DC_EXISTANCE, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FGAUGE(boolean getOrSet, uint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FGAUGE, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FMad(boolean getOrSet, uint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FMad, value);
	return (ReturnValue);
}


uint8 Com_TxSig_ACFRDEFSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_ACFRDEFSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_PBW_WARNING(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_PBW_WARNING, value);
	return (ReturnValue);
}


uint8 Com_TxSig_CAP_IND_RST_STA(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_CAP_IND_RST_STA, value);
	return (ReturnValue);
}


uint8 Com_TxSig_BAC_SW_AS(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_BAC_SW_AS, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AVE_ECO(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.1);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_AVE_ECO, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVE_ECO, &l_value);
		l_phyValue = 0.1 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_DTE(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.1);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_DTE, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_DTE, &l_value);
		l_phyValue = 0.1 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_RENSWN(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RENSWN, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RENSWP(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RENSWP, value);
	return (ReturnValue);
}


uint8 Com_TxSig_STRDWNSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_STRDWNSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_STRUPSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_STRUPSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DOWNSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DOWNSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_UPSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_UPSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_NOTMRNG(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_NOTMRNG, value);
	return (ReturnValue);
}


uint8 Com_TxSig_MRNG(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_MRNG, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TOWMODE(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TOWMODE, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RNGSW1(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RNGSW1, value);
	return (ReturnValue);
}


uint8 Com_TxSig_ODOFFSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_ODOFFSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TPMS_RST(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TPMS_RST, value);
	return (ReturnValue);
}


uint8 Com_TxSig_POWSW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_POWSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_SportMode_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_SportMode_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_REQ_TRAILER_CHK(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_REQ_TRAILER_CHK, value);
	return (ReturnValue);
}


uint8 Com_TxSig_B_Circuit_ST(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_B_Circuit_ST, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RENSWR(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RENSWR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_ECO_MODE_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_ECO_MODE_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_VSP40_MET(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.01);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_VSP40_MET, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VSP40_MET, &l_value);
		l_phyValue = 0.01 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_VS_DISP(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.01);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_VS_DISP, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_VS_DISP, &l_value);
		l_phyValue = 0.01 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_ICCExistance(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_ICCExistance, value);
	return (ReturnValue);
}


uint8 Com_TxSig_EX_TEMP(boolean getOrSet, sint16 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint8 l_value = 0;
	sint16 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint8)((l_phyValue+40));
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_EX_TEMP, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_EX_TEMP, &l_value);
		l_phyValue = -40 + l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_MARKET(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_MARKET, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TSOC_MET(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TSOC_MET, value);
	return (ReturnValue);
}


uint8 Com_TxSig_VSP_REV_BUZZER(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_VSP_REV_BUZZER, value);
	return (ReturnValue);
}


uint8 Com_TxSig_STAT_CHG_REQ(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_STAT_CHG_REQ, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Display_Unit_355(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Display_Unit_355, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Oil_change_counter_reset(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Oil_change_counter_reset, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Sunroof_SW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Sunroof_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FL_WindowSW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FL_WindowSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FR_WindowSW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FR_WindowSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RL_WindowSW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RL_WindowSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RR_WindowSW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RR_WindowSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DA_RearviewMirrorOnOff(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DA_RearviewMirrorOnOff, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DA_RoomLampOnOff(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DA_RoomLampOnOff, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FUEL_Remain(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.1);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_FUEL_Remain, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_FUEL_Remain, &l_value);
		l_phyValue = 0.1 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_FUEL_FirstW_L(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FUEL_FirstW_L, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FUEL_SecondW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FUEL_SecondW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AVE_Fuel_Single(boolean getOrSet, float32 *value)
{
	uint8 ReturnValue = E_NOT_OK;
	uint16 l_value = 0;
	float32 l_phyValue = 0;

	if(COM_SET_SIG_VALUE == getOrSet)
	{
		l_phyValue = *value;
		l_value = (uint16)((l_phyValue)/0.1);
		ReturnValue = (*Com_Sig_Value[COM_SET_SIG_VALUE])(SIG_HND_AVE_Fuel_Single, &l_value);	
	}
	else
	{
		ReturnValue = (*Com_Sig_Value[COM_GET_SIG_VALUE])(SIG_HND_AVE_Fuel_Single, &l_value);
		l_phyValue = 0.1 * l_value;
		*value = l_phyValue;
	}
	return (ReturnValue);
}


uint8 Com_TxSig_YEAR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_YEAR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_MONTH(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_MONTH, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DAY(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DAY, value);
	return (ReturnValue);
}


uint8 Com_TxSig_HOUR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_HOUR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_MINUTE(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_MINUTE, value);
	return (ReturnValue);
}


uint8 Com_TxSig_SECOND(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_SECOND, value);
	return (ReturnValue);
}


uint8 Com_TxSig_GPS_Signal(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_GPS_Signal, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Lamp_headlampAngle(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Lamp_headlampAngle, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Lamp_atmosphere_OnOff(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Lamp_atmosphere_OnOff, value);
	return (ReturnValue);
}


uint8 Com_TxSig_RhythmSignal(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_RhythmSignal, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Lamp_atmosphereColor(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Lamp_atmosphereColor, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Wireless_charge_Switch(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Wireless_charge_Switch, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DA_Source_Signal_5A8(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DA_Source_Signal_5A8, value);
	return (ReturnValue);
}


uint8 Com_TxSig_PANEL_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_PANEL_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Color_Adjustment_DA(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Color_Adjustment_DA, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Music_Rhythm_Switch_DA(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Music_Rhythm_Switch_DA, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Tem_Switch_DA(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Tem_Switch_DA, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Speeding_Switch_DA(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Speeding_Switch_DA, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Speeding_info(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Speeding_info, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Birthday(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Birthday, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Amb_Holiday(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Amb_Holiday, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Rearview_mirror_adjustment(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Rearview_mirror_adjustment, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_TempUp_D(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_TempUp_D, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_TempDn_D(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_TempDn_D, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Mode(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Mode, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Defronst(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Defronst, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_AirMAX_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_AirMAX_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Recirc_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Recirc_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_A_C(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_A_C, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_SYNC(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_SYNC, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Fan_speed_setting(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Fan_speed_setting, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Reserve(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Reserve, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_AUTO(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_AUTO, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_BlowerPlus(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_BlowerPlus, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_BlowerMinus(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_BlowerMinus, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_AirVentDirectSetting(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_AirVentDirectSetting, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_TempUp_P(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_TempUp_P, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_TempDn_P(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_TempDn_P, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Redefronst(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Redefronst, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_ONOFF(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_ONOFF, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Target_Temp_TS_D(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Target_Temp_TS_D, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_DeforstVR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_DeforstVR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Recirc_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Recirc_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_AC_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_AC_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_SYNC_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_SYNC_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Redeforst_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Redeforst_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_ONOFF_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_ONOFF_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_AirMAX_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_AirMAX_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Reserve3(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Reserve3, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AC_Target_Temp_TS_P(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AC_Target_Temp_TS_P, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TempUp_E(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TempUp_E, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TempDn_E(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TempDn_E, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Target_E_Temp_TS_VR(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Target_E_Temp_TS_VR, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Auto_condition_mode_gear(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Auto_condition_mode_gear, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DIS_TOTAL(boolean getOrSet, uint32 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DIS_TOTAL, value);
	return (ReturnValue);
}


uint8 Com_TxSig_CLUSTER_REFUSE(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_CLUSTER_REFUSE, value);
	return (ReturnValue);
}


uint8 Com_TxSig_FUEL_LOW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_FUEL_LOW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_BRAKE_SYSTEM(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_BRAKE_SYSTEM, value);
	return (ReturnValue);
}


uint8 Com_TxSig_MTSRC_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_MTSRC_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_CLUSTER_WAKE_SLEEP_REQ(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_CLUSTER_WAKE_SLEEP_REQ, value);
	return (ReturnValue);
}


uint8 Com_TxSig_SNOW_SW(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_SNOW_SW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_HandBreakSwitch(boolean getOrSet, boolean *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_HandBreakSwitch, value);
	return (ReturnValue);
}


uint8 Com_TxSig_APUcfg(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_APUcfg, value);
	return (ReturnValue);
}


uint8 Com_TxSig_WALcfg(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_WALcfg, value);
	return (ReturnValue);
}


uint8 Com_TxSig_VDC_ONOFF(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_VDC_ONOFF, value);
	return (ReturnValue);
}


uint8 Com_TxSig_check_sum(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_check_sum, value);
	return (ReturnValue);
}


uint8 Com_TxSig_Message_counter(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_Message_counter, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DA_RefusetoSleep(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DA_RefusetoSleep, value);
	return (ReturnValue);
}


uint8 Com_TxSig_AutoHoldSW(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_AutoHoldSW, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_SpeakerReady(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_SpeakerReady, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_SpeakerOff(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_SpeakerOff, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_VedioPushResult(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_VedioPushResult, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_VedioPushFail(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_VedioPushFail, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_PicturePushResult(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_PicturePushResult, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_PictureFail(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_PictureFail, value);
	return (ReturnValue);
}


uint8 Com_TxSig_TCU_AutoOnOff(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_TCU_AutoOnOff, value);
	return (ReturnValue);
}


uint8 Com_TxSig_CLUSTER_WAKEUP_SIGNAL(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_CLUSTER_WAKEUP_SIGNAL, value);
	return (ReturnValue);
}


uint8 Com_TxSig_DA_WakeUp_Signal(boolean getOrSet, uint8 *value)
{
	uint8 ReturnValue = E_NOT_OK;

	ReturnValue = (*Com_Sig_Value[getOrSet])(SIG_HND_DA_WakeUp_Signal, value);
	return (ReturnValue);
}


