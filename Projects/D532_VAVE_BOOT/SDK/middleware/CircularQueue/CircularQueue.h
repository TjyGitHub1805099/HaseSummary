/**
* @file:    CircularQueue.h
* @brief:   Add your description here for this file.
* @author:  Stephen Du
* @date:    2018-06-20 20:31:04
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-06-20 20:31:04  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CIRCULARQUEUE_H_
#define _CIRCULARQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "CircularQueue_Cfg.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "System_Portable.h"
/*------------------------------ END: USER INCLUDE ------------------------------*/

typedef enum
{
    CQUEUE_BUFTYPE_SINT8,
    CQUEUE_BUFTYPE_SINT16,
    CQUEUE_BUFTYPE_SINT32,
    CQUEUE_BUFTYPE_UINT8,
    CQUEUE_BUFTYPE_UINT16,
    CQUEUE_BUFTYPE_UINT32,
} CQueue_BufType;

#define CQUEUE_CH_AUTO 0XFFFFU
#define CQUEUE_POP_ALL 0

#define CQUEUE_ERR_CHANNEL 0XFFFFU
#define CQUEUE_ERR_PUSHPOP 0XFFFFU

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:CQUEUE_CH_AUTO: Auto initial a avaliable channel
*					0~CQUEUE_CHANNEL_MAX:Initial user specified fixed channel
*@param[in] bufType:CQUEUE_BUFTYPE_SINT8/CQUEUE_BUFTYPE_SINT16/
*					CQUEUE_BUFTYPE_SINT32/CQUEUE_BUFTYPE_UINT8/
*     				CQUEUE_BUFTYPE_UINT16/CQUEUE_BUFTYPE_UINT32
*@param[in] size:1~65536
*@param[inout] None
*@param[out] None
*@retval CQUEUE_ERR_CHANNEL: No avaliable channel
*        0~CQUEUE_CHANNEL_MAX: The channel you get, this num will be used in others API in this module
*
*@brief Service for resetting all internal variables.
*/
extern uint16 CQueue_Init(uint16 channel, CQueue_BufType bufType, void *buffer, uint16 size);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[inout] None
*@param[out] None
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*@brief Service for resetting all internal variables.
*/
extern uint8 CQueue_Uninit(uint16 channel);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] buffer:user avaiable data buffer
*@param[in] len:1~65536
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval CQUEUE_ERR_PUSHPOP: Request failed.
*@retval Index: Return tail index.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint16 CQueue_Push(uint16 channel, void *buffer, uint16 len, uint8 fromISR);
#else
extern uint16 CQueue_Push(uint16 channel, void *buffer, uint16 len);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] buffer:user avaiable data buffer
*@param[in] len:1~65536
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval CQUEUE_ERR_PUSHPOP: Request failed.
*@retval Index: Return head index.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint16 CQueue_Push2Head(uint16 channel, void *buffer, uint16 len, uint8 fromISR);
#else
extern uint16 CQueue_Push2Head(uint16 channel, void *buffer, uint16 len);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] buffer:user avaiable data buffer
*@param[in] len:CQUEUE_POP_ALL：It will pop all data
*			1~65536: This len should less than real len in queue, otherwise it will return 0.
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval CQUEUE_ERR_PUSHPOP: Request failed.
*@retval Index/len: if len!=0,return start index; if len==0,return len.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint16 CQueue_Pop(uint16 channel, void *buffer, uint16 len, uint8 fromISR);
#else
extern uint16 CQueue_Pop(uint16 channel, void *buffer, uint16 len);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint8 CQueue_Clear(uint16 channel, uint8 fromISR);
#else
extern uint8 CQueue_Clear(uint16 channel);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint8 CQueue_GetLen(uint16 channel, uint16 *len, uint8 fromISR);
#else
extern uint8 CQueue_GetLen(uint16 channel, uint16 *len);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint8 CQueue_IsEmpty(uint16 channel, uint8 *isEmpty, uint8 fromISR);
#else
extern uint8 CQueue_IsEmpty(uint16 channel, uint8 *isEmpty);
#endif

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel:0~CQUEUE_CHANNEL_MAX
*@param[in] fromISR:If the environment has OS and the context can't be getted,
*					you need set this para according to the caller
*@param[inout] None
*@param[out] None
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*@brief Service for resetting all internal variables.
*/
#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
extern uint8 CQueue_IsFull(uint16 channel, uint8 *isFull, uint8 fromISR);
#else
extern uint8 CQueue_IsFull(uint16 channel, uint8 *isFull);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CIRCULARQUEUE_H_
/*============================= END OF FILE: CircularQueue.h ============================*/
