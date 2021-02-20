/**
* @file:    CircularQueue_Cfg.c
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
#include "Std_Types.h"
#include "CircularQueue_Cfg.h"

/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "System_Portable.h"
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#include "System_Mutex.h"
#endif
/*------------------------------ END: USER INCLUDE ------------------------------*/
void CQueue_EnterCriticalCode(void)
{
    EnterCriticalCode();
}

void CQueue_ExitCriticalCode(void)
{
    ExitCriticalCode();
}

#if(SYS_OS_NOOS != SYS_OS_TYPE)
static SystemMutexType CQueue_Mutex[CQUEUE_CHANNEL_MAX];

uint8 CQueue_Mutex_Create(uint8 channel)
{
    uint8 l_returnValue = E_NOT_OK;

    if (channel < CQUEUE_CHANNEL_MAX) {
        l_returnValue = System_Mutex_Create(&CQueue_Mutex[channel]);
    }

    return l_returnValue;
}

uint8 CQueue_Mutex_Delete(uint8 channel)
{
    uint8 l_returnValue = E_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != CQueue_Mutex[channel])) {
        System_Mutex_Delete(CQueue_Mutex[channel]);
    }

    return l_returnValue;
}

#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
uint8 CQueue_Mutex_Lock(uint8 channel, uint8 fromISR)
#else
uint8 CQueue_Mutex_Lock(uint8 channel)
#endif
{
    uint8 l_returnValue = E_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != CQueue_Mutex[channel])) {
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        System_Mutex_Lock(CQueue_Mutex[channel], fromISR);
#else
        System_Mutex_Lock(CQueue_Mutex[channel]);
#endif
    }

    return l_returnValue;
}

#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
uint8 CQueue_Mutex_Unlock(uint8 channel, uint8 fromISR)
#else
uint8 CQueue_Mutex_Unlock(uint8 channel)
#endif

{
    uint8 l_returnValue = E_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != CQueue_Mutex[channel])) {
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        System_Mutex_Unlock(CQueue_Mutex[channel], fromISR);
#else
        System_Mutex_Unlock(CQueue_Mutex[channel]);
#endif
    }

    return l_returnValue;
}
#endif //if(SYS_OS_NOOS != SYS_OS_TYPE)
/*============================= END OF FILE: CircularQueue_Cfg.c ============================*/
