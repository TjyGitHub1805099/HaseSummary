/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _UDS_CFG_H
#define _UDS_CFG_H
#include"diagCommon_cfg.h"
#include"uds.h"

#define UDS_PHY_BUFF_LEN (512)
#define UDS_FUNC_BUFF_LEN 8
#define UDS_BIN_FILE_MAX_LEN 262144 //256K


#define UDS_TIMER_VALUE_P2_SERVER_MAX  50 //ms
#define UDS_TIMER_VALUE_P2_SERVER_MAX_ENHANCED 5000 //ms
#define UDS_TIMER_VALUE_S3_SERVER 5000 //ms

#define UDS_RESNED_COUNTER 2


extern uint16 gRespDataLen[UDS_CH_MAX_NUM];   //define in uds_cfg.c line 27
extern uint8 *gUDSBufferPtr[UDS_CH_MAX_NUM];  //define in uds_cfg.c line 15
extern const tUdsCfgInfor gUdsCfgInfor;       //define in uds_cfg.c line 35
extern boolean gEnableInnerSidFlag;           //define in uds_cfg.c line 33
#endif
