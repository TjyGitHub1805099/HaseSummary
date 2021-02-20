/**************************
description: public general process APIs for sid 0x28 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/04/27
**************************/

#include"sid0x28_if.h"

extern const tSid0x28CtrlTypeCfgInfor gSid0x28CtrlTypeCfgInfor;
extern const tSid0x28CnnTypeCfgInfor gSid0x28CnnTypeCfgInfor;


tUdsNRCType sid0x28ParseReq(const uint8* data, uint16 len)
{
	uint8 controlType = 0;
	uint8 communicationType = 0;

	if(data == NULL || len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	controlType = data[1]&0x7F;
	communicationType = data[2];
	switch(controlType)
	{
	case UDS_CNN_CTRL_TYPE_ENABLE_RX_AND_TX:
		if(gSid0x28CtrlTypeCfgInfor.enableRxTxFlag == 0)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableComRx();
			diagEnableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.nmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableNMTx();
			diagEnableNMRx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalAndNmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableComRx();
			diagEnableComTx();
			diagEnableNMTx();
			diagEnableNMRx();
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
		break;
	case UDS_CNN_CTRL_TYPE_ENABLE_RX_AND_DISABLE_TX:
		if(gSid0x28CtrlTypeCfgInfor.enableRxDisableTxFlag == 0)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableComRx();
			diagDisableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.nmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableNMRx();
			diagDisableNMTx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalAndNmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagEnableComRx();
			diagDisableComTx();
			diagEnableNMRx();
			diagDisableNMTx();
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
		break;
	case UDS_CNN_CTRL_TYPE_DISABLE_RX_AND_ENABLE_TX:
		if(gSid0x28CtrlTypeCfgInfor.disableRxEnableTxFlag == 0)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableComRx();
			diagEnableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.nmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableNMRx();
			diagEnableNMTx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalAndNmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableComRx();
			diagEnableComTx();
			diagDisableNMRx();
			diagEnableNMTx();
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
		break;
	case UDS_CNN_CTRL_TYPE_DISABLE_RX_AND_TX:
		if(gSid0x28CtrlTypeCfgInfor.disableRxTxFlag == 0)
			return NRC_SUB_FUNCTION_NOT_SUPPORTED;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableComRx();
			diagDisableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.nmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableNMRx();
			diagDisableNMTx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
			if(gSid0x28CnnTypeCfgInfor.normalAndNmCnnMsg == 0)
				return NRC_REQUEST_OUT_OF_RANGE;
			diagDisableComRx();
			diagDisableComTx();
			diagDisableNMRx();
			diagDisableNMTx();
			break;
		default:
			return NRC_REQUEST_OUT_OF_RANGE;
		}
		break;
	default:
		return NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return NRC_NONE;
}


void sid0x28ResetStatus()
{
	sid0x28ComCtrlEnableAll();
}

void sid0x28ComCtrlEnableAll()
{
	diagEnableComRx();
	diagEnableComTx();
	diagEnableNMTx();
	diagEnableNMRx();
}
