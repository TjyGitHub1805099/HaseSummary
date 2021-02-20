/**
*@file  PriQueue.c
*@brief Describe here.
*@date    2018-03-29 15:10:14
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
#include "PriQueue.h"
#include "FlagGroup.h"
#include "Crc.h"
#include "System_Portable.h"
/*============================= START: USER INCLUDED ============================*/
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
#include <stdio.h>
#endif
/*============================== END: USER INCLUDE ==============================*/

/*========================== START: EXTERNAL REFERENCE ==========================*/
extern const uint8 PriQueue_MaxChannel;
extern PriQueueManageType PriQueueManage[];
extern PriQueueCfgType PriQueueCfg[];
/*=========================== END: EXTERNAL REFERENCE ===========================*/

/*============================ START: GLOBAL DEFINE =============================*/

/*============================= END: GLOBAL DEFINE ==============================*/

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
*@param[inout] None
*@param[out] None
*@retval E_NOT_OK
*@retval E_OK
*
*@brief You must call this func first before you use other func in this module.
*/
boolean PriQueue_Init(uint8 channel)
{
    boolean l_returnValue = E_NOT_OK;
    uint16 l_curIndex = 0U, l_queueIndex = 0U;
    uint32 l_crcLen = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_Init,Channel:%d\n", channel);
#endif

    if ((PriQueue_MaxChannel > channel)
            && (FALSE == PriQueueManage[channel].init)
            && (NULL_PTR != PriQueueCfg[channel].nodePtr)
            && (NULL_PTR != PriQueueCfg[channel].headPtr))
    {
        PriQueueManage[channel].flagHandlePtr = FlagGroup_Init(PriQueueCfg[channel].flagBufPtr, PriQueueCfg[channel].depth);
        FlagGroup_ClearAll(PriQueueManage[channel].flagHandlePtr);
        l_crcLen = (uint32)(&PriQueueCfg[channel].headPtr->crc) - (uint32)(PriQueueCfg[channel].headPtr);
        if (PriQueueCfg[channel].headPtr->crc == Crc_CalculateCRC32((uint8*)(PriQueueCfg[channel].headPtr), l_crcLen, CRC_STEP_SINGLE, 0))
        {
            while (l_curIndex < PriQueueCfg[channel].headPtr->count)
            {
                l_queueIndex = (PriQueueCfg[channel].nodePtr + l_curIndex)->memIndex;
                l_curIndex++;
                FlagGroup_SetFlagState(PriQueueManage[channel].flagHandlePtr, l_queueIndex, TRUE);
            }
        }
        else
        {
            PriQueueCfg[channel].headPtr->count = 0U;
        }
        PriQueueManage[channel].init = TRUE;
        l_returnValue = E_OK;
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_Init return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
*@param[inout] None
*@param[out] None
*@retval E_NOT_OK
*@retval E_OK
*
*@brief Clean all the queued information.
*/
boolean PriQueue_Clean(uint8 channel)
{
    boolean l_returnValue = E_NOT_OK;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_Clean Channel:%d\n", channel);
#endif

    if ((PriQueue_MaxChannel > channel) && (TRUE == PriQueueManage[channel].init))
    {
        PriQueueCfg[channel].headPtr->count = 0U;
        FlagGroup_ClearAll(PriQueueManage[channel].flagHandlePtr);
        l_returnValue = E_OK;
    }

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_Clean return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint16 PriQueue_Insert(uint8 channel, boolean autoCover, uint16 priority, uint32 value)
{
    uint16 l_returnValue = PRIQUEUE_ERROR;
    uint16 l_curIndex = 0u, l_memIndex = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_Insert Channel:%d autoCover:%d priority:%d value:%d\n", channel, autoCover, priority, value);
#endif
    if ((PriQueue_MaxChannel > channel) && (TRUE == PriQueueManage[channel].init))
    {
        if (PriQueueCfg[channel].headPtr->count < PriQueueCfg[channel].depth)
        {
            for (l_curIndex = 0; l_curIndex < PriQueueCfg[channel].headPtr->count; l_curIndex++)
            {
                if ((PriQueueCfg[channel].nodePtr + l_curIndex)->priority <= priority)
                {
                    break;
                }
            }
            l_returnValue = l_curIndex;
            EnterCriticalCode();
            for (l_curIndex = PriQueueCfg[channel].headPtr->count; l_curIndex > l_returnValue; l_curIndex--)
            {
                (PriQueueCfg[channel].nodePtr + l_curIndex)->memIndex = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->memIndex;
                (PriQueueCfg[channel].nodePtr + l_curIndex)->priority = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->priority;
                (PriQueueCfg[channel].nodePtr + l_curIndex)->value = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->value;
            }
            FlagGroup_GetClearedFlag(PriQueueManage[channel].flagHandlePtr, FLAGGROUP_MODE_LOWEST, &l_memIndex);
            (PriQueueCfg[channel].nodePtr + l_returnValue)->memIndex = l_memIndex;
            (PriQueueCfg[channel].nodePtr + l_returnValue)->priority = priority;
            (PriQueueCfg[channel].nodePtr + l_returnValue)->value = value;
            PriQueueCfg[channel].headPtr->count++;
            FlagGroup_SetFlagState(PriQueueManage[channel].flagHandlePtr, l_memIndex, TRUE);
            ExitCriticalCode();
        }
        else
        {
            if (TRUE == autoCover)
            {
                for (l_curIndex = 0; l_curIndex < PriQueueCfg[channel].depth; l_curIndex++)
                {
                    if ((PriQueueCfg[channel].nodePtr + l_curIndex)->priority <= priority)
                    {
                        break;
                    }
                }
                l_returnValue = l_curIndex;
                EnterCriticalCode();
                l_memIndex = (PriQueueCfg[channel].nodePtr + PriQueueCfg[channel].depth - 1)->memIndex;
                for (l_curIndex = PriQueueCfg[channel].depth - 1; l_curIndex > l_returnValue; l_curIndex--)
                {
                    (PriQueueCfg[channel].nodePtr + l_curIndex)->memIndex = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->memIndex;
                    (PriQueueCfg[channel].nodePtr + l_curIndex)->priority = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->priority;
                    (PriQueueCfg[channel].nodePtr + l_curIndex)->value = (PriQueueCfg[channel].nodePtr + l_curIndex - 1)->value;
                }
                (PriQueueCfg[channel].nodePtr + l_returnValue)->memIndex = l_memIndex;
                (PriQueueCfg[channel].nodePtr + l_returnValue)->priority = priority;
                (PriQueueCfg[channel].nodePtr + l_returnValue)->value = value;
                ExitCriticalCode();
            }
        }
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_Insert return %d\n", l_returnValue);
#endif
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("count: %d\n", PriQueueCfg[channel].headPtr->count);
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint32 PriQueue_SearchByPriority(uint8 channel, PriQueueModeEType mode, uint16 priority, uint32 value)
{
    uint32 l_returnValue = PRIQUEUE_ERROR;
    uint16 l_curIndex = 0u;
    uint8 l_matchFlag = FALSE;
    uint32 l_maxValue = 0u;
    uint32 l_minValue = 0u;
    uint32 l_maxValueIndex = 0u;
    uint32 l_minValueIndex = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_SearchByPriority Channel:%d mode:%d priority:%d\n", channel, mode, priority);
#endif

    if ((PriQueue_MaxChannel > channel) && (TRUE == PriQueueManage[channel].init))
    {
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
        printf("count: %d\n", PriQueueCfg[channel].headPtr->count);
#endif
        for (l_curIndex = 0; l_curIndex < PriQueueCfg[channel].headPtr->count; l_curIndex++)
        {
            if (((PriQueueCfg[channel].nodePtr + l_curIndex)->priority == priority) && (FALSE == l_matchFlag))
            {
                l_returnValue = l_curIndex;
                l_maxValue = (PriQueueCfg[channel].nodePtr + l_curIndex)->value;
                l_minValue = l_maxValue;
                l_maxValueIndex = l_curIndex;
                l_minValueIndex = l_curIndex;
                if (PRIQUEUE_MODE_NEWEST == mode)
                {
                    break;
                }
                else
                {
                    l_matchFlag = TRUE;
                    if ((PRIQUEUE_MODE_VALUEEQU == mode) && ((PriQueueCfg[channel].nodePtr + l_curIndex)->value == value))
                    {
                        break;
                    }
                }
            }
            else if ((TRUE == l_matchFlag) && ((PriQueueCfg[channel].nodePtr + l_curIndex)->priority != priority))
            {
                break;
            }
            else if ((TRUE == l_matchFlag) && ((PriQueueCfg[channel].nodePtr + l_curIndex)->priority == priority))
            {
                l_maxValue = (l_maxValue >= value) ? l_maxValue : value;
                l_minValue = (l_minValue <= value) ? l_minValue : value;
                l_maxValueIndex = (l_maxValue >= value) ? l_maxValueIndex : l_curIndex;
                l_minValueIndex = (l_minValue <= value) ? l_minValueIndex : l_curIndex;
            }
            else {}
        }
        if (TRUE == l_matchFlag)
        {
            if (PRIQUEUE_MODE_OLDEST == mode)
            {
                l_returnValue = l_curIndex - 1;
            }
            else if (PRIQUEUE_MODE_ALL == mode)
            {
                l_returnValue |= ((l_curIndex - 1) << 16);
            }
            else if (PRIQUEUE_MODE_VALUEMAX == mode)
            {
                l_returnValue = l_maxValueIndex;
            }
            else if (PRIQUEUE_MODE_VALUEMIN == mode)
            {
                l_returnValue = l_minValueIndex;
            }
            else {}
        }
    }
    // if (PRIQUEUE_MODE_ALL != mode)
    // {
    //     l_returnValue = (PriQueueCfg[channel].nodePtr + l_returnValue)->memIndex;
    // }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_SearchByPriority return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint16 PriQueue_SearchByValue(uint8 channel, uint16 startIndex, uint32 value)
{
    uint16 l_returnValue = PRIQUEUE_ERROR;
    uint16 l_curIndex = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_SearchByValue Channel:%d startIndex:%d value:%d\n", channel, startIndex, value);
#endif

    if ((PriQueue_MaxChannel > channel) && (TRUE == PriQueueManage[channel].init))
    {
        for (l_curIndex = startIndex; l_curIndex < PriQueueCfg[channel].headPtr->count; l_curIndex++)
        {
            if ((PriQueueCfg[channel].nodePtr + l_curIndex)->value == value)
            {
                l_returnValue = l_curIndex;
                break;
            }
        }
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_SearchByValue return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint32 PriQueue_RemoveByIndex(uint8 channel, uint16 startIndex, uint16 endIndex)
{
    uint32 l_returnValue = PRIQUEUE_ERROR;
    uint16 l_curIndex = 0u;
    uint16 l_endIndex = 0u;
    uint16 l_removeLen = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_RemoveByIndex Channel:%d startIndex:%d endIndex:%d\n", channel, startIndex, endIndex);
#endif
    if ((PriQueue_MaxChannel > channel)
            && (TRUE == PriQueueManage[channel].init)
            && (startIndex < PriQueueCfg[channel].depth)
            && (startIndex <= endIndex))
    {
        if (startIndex < PriQueueCfg[channel].headPtr->count)
        {
            l_curIndex = startIndex;
            l_endIndex = (endIndex < PriQueueCfg[channel].depth) ? endIndex : (PriQueueCfg[channel].depth - 1);
            for (; l_curIndex <= l_endIndex; l_curIndex++)
            {
                FlagGroup_SetFlagState(PriQueueManage[channel].flagHandlePtr, (PriQueueCfg[channel].nodePtr + l_curIndex)->memIndex, FALSE);
            }
            l_curIndex = l_endIndex + 1;
            l_removeLen = l_endIndex + 1 - startIndex;
            for (; l_curIndex < PriQueueCfg[channel].headPtr->count; l_curIndex++)
            {
                EnterCriticalCode();
                (PriQueueCfg[channel].nodePtr + l_curIndex - l_removeLen)->memIndex = (PriQueueCfg[channel].nodePtr + l_curIndex)->memIndex;
                (PriQueueCfg[channel].nodePtr + l_curIndex - l_removeLen)->priority = (PriQueueCfg[channel].nodePtr + l_curIndex)->priority;
                (PriQueueCfg[channel].nodePtr + l_curIndex - l_removeLen)->value = (PriQueueCfg[channel].nodePtr + l_curIndex)->value;
                ExitCriticalCode();
            }
            PriQueueCfg[channel].headPtr->count -= l_removeLen;
        }
        l_returnValue = startIndex | (l_endIndex << 16);
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_RemoveByIndex return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint32 PriQueue_RemoveByPriority(uint8 channel, PriQueueModeEType mode, uint16 priority, uint32 value)
{
    uint32 l_returnValue = PRIQUEUE_ERROR;
    uint32 l_curIndex = PRIQUEUE_ERROR;
    uint16 l_startIndex = 0u, l_endIndex = 0u;

#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_RemoveByPriority Channel:%d mode:%d priority:%d\n", channel, mode, priority);
#endif
    l_curIndex = PriQueue_SearchByPriority(channel, mode, priority, value);
    if (PRIQUEUE_ERROR != l_curIndex)
    {
        l_startIndex = (uint16)(l_curIndex & 0xFFFF);
        if (PRIQUEUE_MODE_ALL == mode)
        {
            l_endIndex = (uint16)(l_curIndex >> 16) & 0xFFFF;
        }
        else
        {
            l_endIndex = l_startIndex;
        }
        l_returnValue = PriQueue_RemoveByIndex(channel, l_startIndex, l_endIndex);
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_RemoveByPriority return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

/*
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] channel: 0~PriQueue_MaxChannel
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
uint16 PriQueue_RemoveByValue(uint8 channel, uint16 startIndex, uint32 value)
{
    uint16 l_returnValue = PRIQUEUE_ERROR;
    uint32 l_temp = 0u;
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("Call PriQueue_RemoveByValue Channel:%d startIndex:%d value:%d\n", channel, startIndex, value);
#endif
    l_returnValue = PriQueue_SearchByValue(channel, startIndex, value);
    if (PRIQUEUE_ERROR != l_returnValue)
    {
        l_temp = PriQueue_RemoveByIndex(channel, l_returnValue, l_returnValue);
        l_returnValue = (uint16)(l_temp & 0xFFFFu);
    }
#if (STD_ON==PRIQUEUE_DEBUG_TRACE)
    printf("PriQueue_RemoveByValue return %d\n", l_returnValue);
#endif
    return (l_returnValue);
}

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
uint16 PriQueue_GetPhyMemIndexByIndex(uint8 channel, uint16 index)
{
    uint16 l_returnValue = PRIQUEUE_ERROR;
    if ((PriQueue_MaxChannel > channel) && (TRUE == PriQueueManage[channel].init) && (index < PriQueueCfg[channel].headPtr->count))
    {
        l_returnValue = (PriQueueCfg[channel].nodePtr + index)->memIndex;
    }
    return (l_returnValue);
}
/*=========================== END OF FILE: PriQueue.c ===========================*/
