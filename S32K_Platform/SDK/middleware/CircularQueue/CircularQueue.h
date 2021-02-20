/**
*@file  CircularQueue.h
*@brief Containing the entire or parts of Circular Queue code.
*@date    2017-10-26 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-10-26  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CIRCULAR_QUEUE_H_
#define _CIRCULAR_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "CircularQueue_Cfg.h"

extern uint8 CQueue_Init(uint8 channel, uint8 *buffer, uint16 size);
extern uint8 CQueue_Push(uint8 channel, uint8 *buffer, uint16 len, uint8 fromISR);
extern uint16 CQueue_Pop(uint8 channel, uint8 *buffer, uint16 len, uint8 fromISR);
extern uint8 CQueue_Clear(uint8 channel, uint8 fromISR);

extern uint8 CQueue_GetLen(uint8 channel, uint16 *len, uint8 fromISR);
extern uint8 CQueue_IsEmpty(uint8 channel, uint8 *isEmpty, uint8 fromISR);
extern uint8 CQueue_IsFull(uint8 channel, uint8 *isFull, uint8 fromISR);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CIRCULAR_QUEUE_H_
/*=========================== END OF FILE: CircularQueue.h ===========================*/
