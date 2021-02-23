
#include "CircularQueue.h"




void CQueue_EnterCriticalCode(void);
void CQueue_ExitCriticalCode(void);
#if(SYS_OS_NOOS != SYS_OS_TYPE)
uint8 CQueue_Mutex_Create(uint8 channel);
uint8 CQueue_Mutex_Delete(uint8 channel);
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
uint8 CQueue_Mutex_Lock(uint8 channel, uint8 fromISR);
uint8 CQueue_Mutex_Unlock(uint8 channel, uint8 fromISR);
#else
uint8 CQueue_Mutex_Lock(uint8 channel);
uint8 CQueue_Mutex_Unlock(uint8 channel);
#endif
#endif

#define CQUEUE_STATE_INIT (uint8)(0X01)
#define CQUEUE_STATE_LOCK (uint8)(0X02)
#define CQUEUE_STATE_UNINIT (uint8)(0XFE)
#define CQUEUE_STATE_UNLOCK (uint8)(0XFD)

typedef struct {
    uint8 state;
    uint16 head;
    uint16 tail;
    uint16 count;
} CQueue_ManagerType;

typedef struct {
    CQueue_BufType bufType;
    uint16 size;
    void *bufPtr;
} CQueue_InitCfgType;

CQueue_ManagerType CQueue_Manager[CQUEUE_CHANNEL_MAX];
CQueue_InitCfgType CQueue_InitCfg[CQUEUE_CHANNEL_MAX];

static uint16 CQueue_UsedChannel = 0u;

uint16 CQueue_Init(CQueue_BufType bufType, void *buffer, uint16 size)
{
    uint16 l_return = CQUEUE_ERR_CHANNEL;

    if ((NULL != buffer) && (0 != size) && (size < 0XFFFF)) {
        if ((CQueue_UsedChannel < CQUEUE_CHANNEL_MAX)
                && (0 == (CQueue_Manager[CQueue_UsedChannel].state & CQUEUE_STATE_INIT))) {
            CQueue_EnterCriticalCode();
            CQueue_InitCfg[CQueue_UsedChannel].bufType = bufType;
            CQueue_InitCfg[CQueue_UsedChannel].size = size;
            CQueue_InitCfg[CQueue_UsedChannel].bufPtr = buffer;
            CQueue_Manager[CQueue_UsedChannel].head = 0;
            CQueue_Manager[CQueue_UsedChannel].tail = 0;
            CQueue_Manager[CQueue_UsedChannel].count = 0;
#if(SYS_OS_NOOS != SYS_OS_TYPE)
            if (E_OK == CQueue_Mutex_Create(CQueue_UsedChannel)) {
                CQueue_Manager[CQueue_UsedChannel].state = CQUEUE_STATE_INIT;
                l_return = CQueue_UsedChannel;
                CQueue_UsedChannel++;
            } else {
                CQueue_Manager[CQueue_UsedChannel].state = 0;
            }
#else
            CQueue_Manager[CQueue_UsedChannel].state = CQUEUE_STATE_INIT;
            l_return = CQueue_UsedChannel;
            CQueue_UsedChannel++;
#endif
            CQueue_ExitCriticalCode();
        }
    }
    return l_return;
}

uint8 CQueue_Uninit(uint16 channel)
{
    uint8 l_return = E_NOT_OK;

    if ((channel < CQUEUE_CHANNEL_MAX)
            && (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK))) {
        CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNINIT;
#if(SYS_OS_NOOS != SYS_OS_TYPE)
        CQueue_Mutex_Delete(channel);
#endif
        l_return = E_OK;
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint16 CQueue_Push(uint16 channel, void *buffer, uint16 len, uint8 fromISR)
#else
uint16 CQueue_Push(uint16 channel, void *buffer, uint16 len)
#endif
{
    uint16 l_return = CQUEUE_ERR_PUSHPOP;
    uint16 l_loop = 0;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != buffer) && (0 != len)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            if ((CQueue_Manager[channel].count + len) <= CQueue_InitCfg[channel].size) {
                for (l_loop = 0; l_loop < len; l_loop++) {
                    switch (CQueue_InitCfg[channel].bufType) {
                    case CQUEUE_BUFTYPE_SINT8:
                        *((sint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((sint8*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_SINT16:
                        *((sint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((sint16*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_SINT32:
                        *((sint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((sint32*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT8:
                        *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((uint8*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT16:
                        *((uint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((uint16*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT32:
                        *((uint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((uint32*)(buffer) + l_loop);
                        break;
                    default:
                        *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].tail) = *((uint8*)(buffer) + l_loop);
                        break;
                    }
                    l_return = CQueue_Manager[channel].tail;
                    CQueue_Manager[channel].tail = (CQueue_Manager[channel].tail + 1) % CQueue_InitCfg[channel].size;
                    CQueue_Manager[channel].count++;
                }
            }
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint16 CQueue_Push2Head(uint16 channel, void *buffer, uint16 len, uint8 fromISR)
#else
uint16 CQueue_Push2Head(uint16 channel, void *buffer, uint16 len)
#endif
{
    uint16 l_return = CQUEUE_ERR_PUSHPOP;
    sint32 l_loop = 0;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != buffer) && (0 != len)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            if ((CQueue_Manager[channel].count + len) <= CQueue_InitCfg[channel].size) {
                for (l_loop = len - 1; l_loop >= 0; l_loop--) {
                    CQueue_Manager[channel].head = (CQueue_Manager[channel].head + CQueue_InitCfg[channel].size - 1) % CQueue_InitCfg[channel].size;
                    CQueue_Manager[channel].count++;
                    switch (CQueue_InitCfg[channel].bufType) {
                    case CQUEUE_BUFTYPE_SINT8:
                        *((sint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((sint8*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_SINT16:
                        *((sint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((sint16*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_SINT32:
                        *((sint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((sint32*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT8:
                        *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((uint8*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT16:
                        *((uint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((uint16*)(buffer) + l_loop);
                        break;
                    case CQUEUE_BUFTYPE_UINT32:
                        *((uint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((uint32*)(buffer) + l_loop);
                        break;
                    default:
                        *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head) = *((uint8*)(buffer) + l_loop);
                        break;
                    }
                }
                l_return = CQueue_Manager[channel].head;
            }
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint16 CQueue_Pop(uint16 channel, void *buffer, uint16 len, uint8 fromISR)
#else
uint16 CQueue_Pop(uint16 channel, void *buffer, uint16 len)
#endif
{
    uint16 l_return = CQUEUE_ERR_PUSHPOP;
    uint16 l_loop = 0;
    uint16 l_count = 0;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != buffer)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            if (0 != len) {
                if (len <= CQueue_Manager[channel].count) {
                    l_count = len;
                }
            } else {
                l_count = CQueue_Manager[channel].count;
            }
            for (l_loop = 0; l_loop < l_count; l_loop++) {
                switch (CQueue_InitCfg[channel].bufType) {
                case CQUEUE_BUFTYPE_SINT8:
                    *((sint8*)(buffer) + l_loop) = *((sint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                case CQUEUE_BUFTYPE_SINT16:
                    *((sint16*)(buffer) + l_loop) = *((sint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                case CQUEUE_BUFTYPE_SINT32:
                    *((sint32*)(buffer) + l_loop) = *((sint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                case CQUEUE_BUFTYPE_UINT8:
                    *((uint8*)(buffer) + l_loop) = *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                case CQUEUE_BUFTYPE_UINT16:
                    *((uint16*)(buffer) + l_loop) = *((uint16*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                case CQUEUE_BUFTYPE_UINT32:
                    *((uint32*)(buffer) + l_loop) = *((uint32*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                default:
                    *((uint8*)(buffer) + l_loop) = *((uint8*)(CQueue_InitCfg[channel].bufPtr) + CQueue_Manager[channel].head);
                    break;
                }
                if (0 != len) {
                    l_return = CQueue_Manager[channel].head;
                }
                CQueue_Manager[channel].head = (CQueue_Manager[channel].head + 1) % CQueue_InitCfg[channel].size;
                CQueue_Manager[channel].count--;
            }
            if (0 == len) {
                l_return = l_loop;
            }
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint8 CQueue_Clear(uint16 channel, uint8 fromISR)
#else
uint8 CQueue_Clear(uint16 channel)
#endif
{
    uint8 l_return = E_NOT_OK;

    if ((channel < CQUEUE_CHANNEL_MAX)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            CQueue_Manager[channel].head = 0;
            CQueue_Manager[channel].tail = 0;
            CQueue_Manager[channel].count = 0;
            l_return = E_OK;
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint8 CQueue_GetLen(uint16 channel, uint16 *len, uint8 fromISR)
#else
uint8 CQueue_GetLen(uint16 channel, uint16 *len)
#endif
{
    uint8 l_return = E_NOT_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != len)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            *len = CQueue_Manager[channel].count;
            l_return = E_OK;
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint8 CQueue_IsEmpty(uint16 channel, uint8 *isEmpty, uint8 fromISR)
#else
uint8 CQueue_IsEmpty(uint16 channel, uint8 *isEmpty)
#endif
{
    uint8 l_return = E_NOT_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != isEmpty)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            *isEmpty = (uint8)((CQueue_Manager[channel].count == 0) ? TRUE : FALSE);
            l_return = E_OK;
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
uint8 CQueue_IsFull(uint16 channel, uint8 *isFull, uint8 fromISR)
#else
uint8 CQueue_IsFull(uint16 channel, uint8 *isFull)
#endif
{
    uint8 l_return = E_NOT_OK;

    if ((channel < CQUEUE_CHANNEL_MAX) && (NULL != isFull)
            && (0 != (CQueue_Manager[channel].state & CQUEUE_STATE_INIT))) {
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Lock(channel, fromISR);
#else
        CQueue_Mutex_Lock(channel);
#endif
#endif
        if (0 == (CQueue_Manager[channel].state & CQUEUE_STATE_LOCK)) {
            CQueue_Manager[channel].state  |=  CQUEUE_STATE_LOCK;
            *isFull = (uint8)((CQueue_Manager[channel].count == CQueue_InitCfg[channel].size) ? TRUE : FALSE);
            l_return = E_OK;
            CQueue_Manager[channel].state  &=  CQUEUE_STATE_UNLOCK;
        }
#if(SYS_OS_NOOS != SYS_OS_TYPE)
#if (STD_OFF == SYS_OS_CONTEXT_AUTOGET)
        CQueue_Mutex_Unlock(channel, fromISR);
#else
        CQueue_Mutex_Unlock(channel);
#endif
#endif
    }
    return l_return;
}

