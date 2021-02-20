/**************************
description: public general process APIs for sid 0x2f of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"sid0x2f_cfg.h"
#include"sid0x27_if.h"

const tSid0x2fCfgInfor gSid0x2fCfgInfor = {
	
};


const tIOCtrlInfo gIOControlInfo[] = {

	{IOCTRL_ID_DUMMY_TEST,TRUE,UDS_INNER_SECURITY_LEVEL_1 | UDS_INNER_SECURITY_LEVEL_2,sid0x2fDid0X0101DummyVerifyPara,sid0x2fDid0X0101DummyCheckExtraDataOrState,sid0x2fDid0X0101DummyStartTest,sid0x2fDid0X0101DummyStopTest},
};


tIOCtrlStatus gIOCtrlStatus[] = {
	{IOCTRL_ID_DUMMY_TEST,IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU},
};


uint8 gIOCtrlDidNum = 0;

void sid0x2fInit()
{
	gIOCtrlDidNum = sizeof(gIOControlInfo)/sizeof(tIOCtrlInfo);
}

