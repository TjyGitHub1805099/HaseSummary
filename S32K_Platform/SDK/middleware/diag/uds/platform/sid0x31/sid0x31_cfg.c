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

const tRoutineCtlInfor gRoutineCtlInfors[] = {
		{ROUTINE_CTL_DUMMY_REQ,	UDS_EXTENDED_DIAG_SESSION,UDS_INNER_SECURITY_LEVEL_0, TRUE,sid0x31Did0x0101DummyVerifyData,sid0x31Did0x0101DummyCheckPrecondition,sid0x31Did0x0101DummyStartTest,sid0x31Did0x0101DummyStopTest,sid0x31Did0x0101DummyReqResult},
		//1TODO:add other cfg here
};

tRoutineCtrlStatusArray gRoutinueCtrlStatus[] = {
	{ROUTINE_CTL_DUMMY_REQ,ROUTINE_CTRL_STATUS_STOP},
		//1TODO:add other status for new did
};

uint8 gRoutineCtrlTotalNum = 0;

void sid0x31Init()
{
	gRoutineCtrlTotalNum = sizeof(gRoutineCtlInfors)/sizeof(tRoutineCtlInfor);
}