/**************************
description: public general process APIs for sid 0x28 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/04/27
**************************/

#include"sid0x28_if.h"

extern const tSid0x28CfgInfor gSid0x28CfgInfor;


tUdsNRCType sid0x28MainProcess(tUdsReqInfor *reqInfor)
{
	uint8 controlType = 0;
	uint8 communicationType = 0;
	uint32 ch = reqInfor->ch;
	const uint8* data = reqInfor->data;
	uint16 len = reqInfor->len;

	if(data == NULL || len!=3)
		return NRC_INCORRECT_MESSAGE_LENGTH;
	
	controlType = data[1]&0x7F;
	communicationType = data[2];
	switch(controlType)
	{
	case UDS_CNN_CTRL_TYPE_ENABLE_RX_AND_TX:
		if(gSid0x28CfgInfor.enableRxTxFlag == 0)
			return NRC_REQUEST_OUT_OF_RANGE;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			diagEnableComRx();
			diagEnableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			diagEnableNMTx();
			diagEnableNMRx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
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
		if(gSid0x28CfgInfor.enableRxDisableTxFlag == 0)
			return NRC_REQUEST_OUT_OF_RANGE;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			diagEnableComRx();
			diagDisableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			diagEnableNMRx();
			diagDisableNMTx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
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
		if(gSid0x28CfgInfor.disableRxEnableTxFlag == 0)
			return NRC_REQUEST_OUT_OF_RANGE;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			diagDisableComRx();
			diagEnableComTx();
			
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			diagDisableNMRx();
			diagEnableNMTx();
			
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
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
		if(gSid0x28CfgInfor.disableRxTxFlag == 0)
			return NRC_REQUEST_OUT_OF_RANGE;
		switch(communicationType)
		{
		case SID_0X28_CNN_TYPE_NORMAL_CNN_MSG:
			diagDisableComRx();
			diagDisableComTx();
			break;
		case SID_0X28_CNN_TYPE_NM_CNN_MSG:
			diagDisableNMRx();
			diagDisableNMTx();
			break;
		case SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG:
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


void sid0x28ResetStatus(uint32 ch)
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
