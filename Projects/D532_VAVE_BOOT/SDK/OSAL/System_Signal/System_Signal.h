/********************************************************************
File name: System_Signal.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-18 09:54:28
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
#ifndef _SYSTEM_SIGNAL_H_
#define _SYSTEM_SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "System_Signal_Cfg.h"

extern void System_Signal_Init(void);
extern boolean System_Signal_Subscribe(uint16 signalId, uint16 taskId);
extern boolean System_Signal_SubSpecValue(uint16 signalId, uint16 taskId, uint32 specValue);
extern boolean System_Signal_Receive(uint16 signalId, uint32* value);
extern boolean System_Signal_Send(uint16 signalId, uint32 value, boolean fromISR);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: System_Signal.h ===========================*/
