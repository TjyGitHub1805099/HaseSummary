/********************************************************************
File name: System_Mutex.c
Author: Stephen Du
Version: V1.0
Timestamp: 2017-06-19 11:31:28
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
#include "System_Mutex.h"

uint8 System_Mutex_Create(SystemMutexType* mutexHandle)
{
    uint8 l_returnValue = E_NOT_OK;
    SystemMutexType l_mutex = NULL;

    if (NULL != mutexHandle) {
        l_mutex = xSemaphoreCreateMutex();
        if (NULL != l_mutex) {
            *mutexHandle = l_mutex;
            l_returnValue = E_OK;
        }
    }
    return l_returnValue;
}

uint8 System_Mutex_Delete(SystemMutexType handle)
{
    uint8 l_returnValue = E_OK;

    if (NULL != handle) {
        vSemaphoreDelete(handle);
    }

    return l_returnValue;
}

boolean System_Mutex_Lock(SystemMutexType handle, boolean fromISR)
{
    boolean returnValue = E_NOT_OK;

    if (NULL != handle) {
        if (STD_ON == fromISR) {
            if (pdPASS == xSemaphoreTakeFromISR(handle, NULL)) {
                returnValue = E_OK;
            }
        } else {
            if (pdPASS == xSemaphoreTake( handle, portMAX_DELAY )) {
                returnValue = E_OK;
            }
        }
    }
    return (returnValue);
}

boolean System_Mutex_Unlock(SystemMutexType handle, boolean fromISR)
{
    boolean returnValue = E_NOT_OK;

    if (NULL != handle) {
        if (STD_ON == fromISR) {
            if (pdPASS == xSemaphoreGiveFromISR(handle, NULL)) {
                returnValue = E_OK;
            }
        } else {
            if (pdPASS == xSemaphoreGive(handle)) {
                returnValue = E_OK;
            }
        }
    }
    return (returnValue);
}

/*=========================== END OF FILE: System_Mutex.c ===========================*/
