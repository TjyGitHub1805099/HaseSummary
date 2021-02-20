/**
*@file  PriQueue.h
*@brief Describe here.
*@date    2018-03-29 15:01:21
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-29  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _PRIQUEUE_H_
#define _PRIQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "PriQueue_Cfg.h"
#include "FlagGroup.h"
/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/

/*===============================================================================*/
/*============================ START: GLOBAL DEFINE =============================*/
/*===============================================================================*/

/************************** START: MACRO DEFINE **************************/
#define PRIQUEUE_ERROR 0XFFFFU

#define PriQueue_BufDef(name,size) PriQueueHeadType PriQueueHead_##name;\
    PriQueueNodeType PriQueueNode_##name[size];\
    FlagGroup_BufDef(PriQueueFlag_##name,size)

#define PriQueue_GetFlagBufName(name) PriQueueFlag_##name
#define PriQueue_GetHeadBufName(name) PriQueueHead_##name
#define PriQueue_GetNodeBufName(name) PriQueueNode_##name
/*************************** END: MACRO DEFINE ***************************/

/********************** START: INTERNAL TYPE DEFINE **********************/
typedef struct _PriQueueManageType
{
    boolean init;
    uint32* flagHandlePtr;
} PriQueueManageType;
/*********************** END: INTERNAL TYPE DEFINE ***********************/

/********************** START: EXPORT TYPE DEFINE **********************/
typedef struct _PriQueueNodeType
{
    uint16 priority;
    uint16 memIndex;
    uint32 value;
} PriQueueNodeType;

typedef struct _PriQueueHeadType
{
    uint16 count;
    uint16 magicNum;//lowest bit indicate valid
    uint16 crc;
} PriQueueHeadType;

typedef struct _PriQueueCfgType
{
    const uint16 depth;
    uint32* flagBufPtr;
    PriQueueHeadType* headPtr;
    PriQueueNodeType* nodePtr;
} PriQueueCfgType;

typedef enum _PriQueueModeEType
{
    PRIQUEUE_MODE_NEWEST,
    PRIQUEUE_MODE_OLDEST,
    PRIQUEUE_MODE_VALUEEQU,
    PRIQUEUE_MODE_VALUEMAX,
    PRIQUEUE_MODE_VALUEMIN,
    PRIQUEUE_MODE_ALL,
} PriQueueModeEType;
/*********************** END: EXPORT TYPE DEFINE ***********************/
/*===============================================================================*/
/*============================= END: GLOBAL DEFINE ==============================*/
/*===============================================================================*/

/*========================== START: FUNCTION PROTOTYPE ==========================*/
/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[inout] None
*@param[out] None
*@retval E_NOT_OK
*@retval E_OK
*
*@brief You must call this func first before you use other func in this module.
*/
extern boolean PriQueue_Init(uint8 channel);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[inout] None
*@param[out] None
*@retval E_NOT_OK
*@retval E_OK
*
*@brief Clean all the queued information.
*/
extern boolean PriQueue_Clean(uint8 channel);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] autoCover: TRUE/FALSE
*If autoCover equal TRUE and the queue is full,it will remove the lowest and oldest data and add the new info to queue
*@param[in] priority
*@param[in] value
*@param[inout] None
*@param[out] None
*@retval E_NOT_OK
*@retval E_OK
*
*@brief Insert user info to queue by pririty.
*/
extern uint16 PriQueue_Insert(uint8 channel, boolean autoCover, uint16 priority, uint32 value);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] mode
*PRIQUEUE_MODE_NEWEST: Return the newest item index.
*PRIQUEUE_MODE_OLDEST: Return the oldest item index.
*PRIQUEUE_MODE_VALUEEQU: Return both the priority and value are equal item index.
*PRIQUEUE_MODE_VALUEMAX: Return the specified priority and biggest value item index.
*PRIQUEUE_MODE_VALUEMIN: Return the specified priority and least value item index.
*PRIQUEUE_MODE_ALL: Return all items index(in range).
*The return value should split into two parts.
*
*@param[in] priority
*@param[in] value
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR: Error happend.
*@retval Index Num not equal PRIQUEUE_ERROR.
*
*@brief If the return value not equal PRIQUEUE_ERROR and the mode equal PRIQUEUE_MODE_ALL,the reutrn value should split two parts by user,
*The high uint16 means the end index, the low uint16 means the start index.
*/
extern uint32 PriQueue_SearchByPriority(uint8 channel, PriQueueModeEType mode, uint16 priority, uint32 value);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] startIndex: 0~0xfffe
*Input 0 in default.
*If you want to find more item and to imporve effiency, you can input the last returned value for startIndex.
*@param[in] value
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR: Error happend.
*@retval Index Num not equal PRIQUEUE_ERROR.
*
*@brief None
*/
extern uint16 PriQueue_SearchByValue(uint8 channel, uint16 startIndex, uint32 value);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] startIndex
*@param[in] endIndex
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR: Error happend.
*@retval Index Num not equal PRIQUEUE_ERROR.
*
*@brief If the return value not equal PRIQUEUE_ERROR and the mode equal PRIQUEUE_MODE_ALL,
*the reutrn value should split two parts by user,The high uint16 means the end index, the low uint16 means the start index.
*/
extern uint32 PriQueue_RemoveByIndex(uint8 channel, uint16 startIndex, uint16 endIndex);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] mode
*PRIQUEUE_MODE_NEWEST: Remove the newest item.
*PRIQUEUE_MODE_OLDEST: Remove the oldest item.
*PRIQUEUE_MODE_VALUEEQU: Remove both the priority and value are equal item.
*PRIQUEUE_MODE_VALUEMAX: Remove the specified priority and biggest value item.
*PRIQUEUE_MODE_VALUEMIN: Remove the specified priority and least value item.
*PRIQUEUE_MODE_ALL: Remove all items which equal the specified priority.
*
*@param[in] priority
*@param[in] value
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR: Error happend.
*@retval Index Num not equal PRIQUEUE_ERROR.
*
*@brief If the return value not equal PRIQUEUE_ERROR and the mode equal PRIQUEUE_MODE_ALL,
*the reutrn value should split into two parts by user,The high uint16 means the end index, the low uint16 means the start index.
*/
extern uint32 PriQueue_RemoveByPriority(uint8 channel, PriQueueModeEType mode, uint16 priority, uint32 value);

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PRIQUEUE_CH_MAX
*@param[in] startIndex: 0~0xfffe
*Input 0 in default.
*If you want to find more item and to imporve effiency, you can input the last returned value for startIndex.
*@param[in] value
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR: Error happend.
*@retval Index Num not equal PRIQUEUE_ERROR.
*
*@brief None
*/
extern uint16 PriQueue_RemoveByValue(uint8 channel, uint16 startIndex, uint32 value);

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel
*@param[in] index: 0~0xfffe
*@param[inout] None
*@param[out] None
*@retval PRIQUEUE_ERROR/0~PriQueue_MaxChannel
*
*@brief None
*/
extern uint16 PriQueue_GetPhyMemIndexByIndex(uint8 channel, uint16 index);
/*=========================== END: FUNCTION PROTOTYPE ===========================*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_PRIQUEUE_H_
/*=========================== END OF FILE: PriQueue.h ===========================*/
