/********************************************************************
File name: System_Tick.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-04-05 11:59:28
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
#ifndef  _SYSTEM_TICK_H_
#define  _SYSTEM_TICK_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "Std_Types.h"

extern void vApplicationTickHook(void);
extern Std_ReturnType GetCounterValue(uint8 CounterID,uint32* Value);
extern Std_ReturnType GetElapsedCounterValue(uint8 CounterID,uint32* Value,uint32* ElapsedValue);
extern uint32 System_GetTickValue(void);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: System_Tick.h ===========================*/

