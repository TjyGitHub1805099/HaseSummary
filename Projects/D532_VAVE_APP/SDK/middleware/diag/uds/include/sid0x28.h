/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _SID_0X28_H
#define _SID_0X28_H

#include"diagCommon_if.h"

typedef enum{
	UDS_CNN_CTRL_TYPE_ENABLE_RX_AND_TX = 0,
	UDS_CNN_CTRL_TYPE_ENABLE_RX_AND_DISABLE_TX = 01,
	UDS_CNN_CTRL_TYPE_DISABLE_RX_AND_ENABLE_TX = 02,
	UDS_CNN_CTRL_TYPE_DISABLE_RX_AND_TX = 03,
}tUdsCnnCtrlType;

typedef enum{
	SID_0X28_CNN_TYPE_NORMAL_CNN_MSG = 01,
	SID_0X28_CNN_TYPE_NM_CNN_MSG = 02,
	SID_0X28_CNN_TYPE_NORMAL_AND_NM_CNN_MSG = 03
}tUdsCnnType;


typedef struct{
	uint8 enableRxTxFlag:1;
	uint8 enableRxDisableTxFlag:1;
	uint8 disableRxEnableTxFlag:1;
	uint8 disableRxTxFlag:1;
}tSid0x28CfgInfor;

#endif
