/*************************************************************************************************************
* author: liqi@hangsheng.com.cn
* 
*************************************************************************************************************/

#ifndef _DIAG_APP_SID_0X22_H
#define _DIAG_APP_SID_0X22_H


#include"Std_Types.h"
#include"diagCommon_cfg.h"
#include "Task_M_IpcApp_Cfg.h" 

boolean diagApp0x22ReqHandle(const uint8 *data,uint16 len);
void diagApp0x22IpcCallback(uint8 ch,uint8 *rx_data, uint32 len);

#endif
