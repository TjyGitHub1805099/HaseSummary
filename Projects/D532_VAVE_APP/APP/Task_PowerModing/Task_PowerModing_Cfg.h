/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: Task_PowerModing_Cfg.h
Author: Stephen Du
Version: V1.0
Timestamp: 2018-09-06 16:36:07
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _TASK_POWERMODING_CFG_H_
#define _TASK_POWERMODING_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

#define TASK_POWERMODING_DEBUG_TRACE STD_ON
#define TASK_POWERMODING_STACK_MONITOR STD_OFF

#define DA_IPC_HW_CH 1
#define DA_IPC_NTFY_LEN 1

#define IC_IPC_HW_CH 0
#define IC_IPC_NTFY_LEN 1

#define USB_UPDATE_ENABLE_MFG_FLAG 0x165456EF


typedef enum
{
	CURENT_MODE = 0,
	ACC_STATUS,
	//WAKEUP_SOURCE,
	IGN_STATUS,
	TCU_ACC_STATUS,
	LCD_STATUS,
	AMP_STATUS,
	//REMOTE_START,
	BAT_STATUS,
	BOARD_TEMP,
	AMP_TEMP,
	REVERSE_STATUS,
}PM_IPC_TxFunctionIDType;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: Task_PowerModing_Cfg.h ===========================*/
