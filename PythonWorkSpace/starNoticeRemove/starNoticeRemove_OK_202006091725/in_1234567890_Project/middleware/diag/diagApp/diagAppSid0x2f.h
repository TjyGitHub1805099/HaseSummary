/*************************************************************************************************************
* author: liqi@hangsheng.com.cn
* 
*************************************************************************************************************/



#ifndef _DIAG_APP_SID_0X2F_H
#define _DIAG_APP_SID_0X2F_H
#include"Std_Types.h"
#include"diagCommon.h"
#include"sid0x2f.h"
#include"diagCommon_cfg.h"

tUdsNRCType diagApp0x2fChkCtrlStateOrCtrlMask(const tIOCtrlInfo* ioctrlInfo, uint8*data, uint16 len);
boolean diagApp0x2fCheckPrecondition(const tIOCtrlInfo* ioctrlInfor);
uint8 diagApp0x2fGetIoCtrlCaseNum();
boolean diagApp0x2fStopTest(const tIOCtrlInfo* ioctrlInfo);
boolean diagApp0x2fReqHandle(uint8*data, uint16 len);
void diagApp0x2fIpcCallback(uint8 ch, uint8 *rx_data, uint32 len);


#endif
