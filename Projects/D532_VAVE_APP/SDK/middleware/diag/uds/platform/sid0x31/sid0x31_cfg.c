/**************************
description: public general process APIs for sid 0x31 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x31_cfg.h"
#include"sid0x27.h"

const tSid0x31CfgInfor gSid0x31CfgInfor = {
	
};

const tRoutineCtlInfor gRoutineCtlInfors_ivi[] = {
		{ROUTINE_CTL_DUMMY_REQ,	UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,sid0x31Did0x0101DummyVerifyData,sid0x31Did0x0101DummyCheckPrecondition,sid0x31Did0x0101DummyStartTest,sid0x31Did0x0101DummyStopTest,sid0x31Did0x0101DummyReqResult},
		//1TODO:add other cfg here
};


tRoutineCtrlStatusInfor gRoutinueCtrlStatus_ivi[] = {
	{ROUTINE_CTL_DUMMY_REQ,ROUTINE_CTRL_STATUS_STOP},
};

const tRoutineCtlInfor gRoutineCtlInfors_ic[] = {
		{ROUTINE_CTL_DUMMY_REQ,	UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,sid0x31Did0x0101DummyVerifyData,sid0x31Did0x0101DummyCheckPrecondition,sid0x31Did0x0101DummyStartTest,sid0x31Did0x0101DummyStopTest,sid0x31Did0x0101DummyReqResult},
		//1TODO:add other cfg here
};

tRoutineCtrlStatusInfor gRoutinueCtrlStatus_ic[] = {
	{ROUTINE_CTL_DUMMY_REQ,ROUTINE_CTRL_STATUS_STOP},
};


const tRoutineCtlInfor gRoutineCtlInfors_avm[] = {
		{ROUTINE_AVM_FAC_CALIBRATION_REQ,	UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,sid0x31Did0x010506AVMVerifyData,NULL,sid0x31Did0x0105StartAVMCalibrationFAC,NULL,sid0x31AVMCalibrationReqResult},
		{ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ,	UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,sid0x31Did0x010506AVMVerifyData,NULL,sid0x31Did0x0106StartAVMCalibrationSALE,NULL,sid0x31AVMCalibrationReqResult},
		{ROUTINE_AVM_SIMU_CALIBRATION_REQ, UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31Did0x01FEStartAVMCalibrationSIMU,NULL,sid0x31AVMCalibrationReqResultSIMU},
		{ROUTINE_AVM_SIMU_SWITCH_FRONT_REQ, UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31Did0x01FEStartAVMCalibrationSIMU,NULL,NULL},
		{ROUTINE_AVM_SIMU_SWITCH_REAR_REQ, UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31Did0x01FEStartAVMCalibrationSIMU,NULL,NULL},
		{ROUTINE_AVM_SIMU_SWITCH_LEFT_REQ, UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31Did0x01FEStartAVMCalibrationSIMU,NULL,NULL},
		{ROUTINE_AVM_SIMU_SWITCH_RIGHT_REQ, UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31Did0x01FEStartAVMCalibrationSIMU,NULL,NULL},

		//1TODO:add other cfg here
};

tRoutineCtrlStatusInfor gRoutinueCtrlStatus_avm[] = {
	{ROUTINE_AVM_FAC_CALIBRATION_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_SIMU_CALIBRATION_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_SIMU_SWITCH_FRONT_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_SIMU_SWITCH_REAR_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_SIMU_SWITCH_LEFT_REQ,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_AVM_SIMU_SWITCH_RIGHT_REQ,ROUTINE_CTRL_STATUS_STOP},
	
};

const tRoutineCtlInfor gRoutineCtlInfors_hud[] = {
	{ROUTINE_HUD_RESET_USER_1,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_RESET_USER_2,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_RESET_USER_3,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_RESET_USER_4,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_RESET_USER_5,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_RESET_USER_DEFAULT,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudResetStart,NULL,sid0x31HudResetConfirm},
	{ROUTINE_HUD_CALIBRATION_DISPLAY,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, FALSE,NULL,NULL,sid0x31HudCalibrationDisplayStart,sid0x31HudCalibrationDisplayStop,sid0x31HudCalibrationDisplayStatus},
	{ROUTINE_HUD_SELF_CHECK_DTC,UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,NULL,NULL,sid0x31HudSelfCheckDTC,NULL,NULL},	
};

tRoutineCtrlStatusInfor gRoutinueCtrlStatus_hud[] = {
    {ROUTINE_HUD_RESET_USER_1,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_RESET_USER_2,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_RESET_USER_3,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_RESET_USER_4,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_RESET_USER_5,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_RESET_USER_DEFAULT,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_CALIBRATION_DISPLAY,ROUTINE_CTRL_STATUS_STOP},
	{ROUTINE_HUD_SELF_CHECK_DTC,ROUTINE_CTRL_STATUS_STOP},
};


const tRoutineCtlInfor *gRoutineCtlInfors[UDS_CH_MAX_NUM] = {
	gRoutineCtlInfors_ivi,
	gRoutineCtlInfors_ic,
	gRoutineCtlInfors_avm,
	gRoutineCtlInfors_hud,
};


tRoutineCtrlStatusInfor *gRoutinueCtrlStatusArray[UDS_CH_MAX_NUM] = {
	gRoutinueCtrlStatus_ivi,
	gRoutinueCtrlStatus_ic,
	gRoutinueCtrlStatus_avm,
	gRoutinueCtrlStatus_hud,
};

uint8 gRoutineCtrlTotalNum[UDS_CH_MAX_NUM];

void sid0x31Init()
{
#if 0
	uint8 ch = 0;
	for(ch=0;ch<UDS_CH_MAX_NUM;ch++)
		gRoutineCtrlTotalNum[ch] = sizeof(gRoutineCtlInfors[ch])/sizeof(tRoutineCtlInfor);
#else
	gRoutineCtrlTotalNum[0] = sizeof(gRoutineCtlInfors_ivi)/sizeof(tRoutineCtlInfor);
	gRoutineCtrlTotalNum[1] = sizeof(gRoutineCtlInfors_ic)/sizeof(tRoutineCtlInfor);
	gRoutineCtrlTotalNum[2] = sizeof(gRoutineCtlInfors_avm)/sizeof(tRoutineCtlInfor);
	gRoutineCtrlTotalNum[3] = sizeof(gRoutineCtlInfors_hud)/sizeof(tRoutineCtlInfor);
#endif
}
