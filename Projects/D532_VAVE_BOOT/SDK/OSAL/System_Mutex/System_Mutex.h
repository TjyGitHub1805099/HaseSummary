/********************************************************************
File name: System_Mutex.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-06-19 11:31:56
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
#ifndef  _SYSTEM_MUTEX_H_
#define  _SYSTEM_MUTEX_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "FreeRTOS.h"
#include "semphr.h"

typedef SemaphoreHandle_t SystemMutexType;

/*
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*@param[inout] None
*@param[out] SystemMutexType: if this value equal NULL, that means create mutex failed.
*
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*Service to create a mutex.
*/
extern uint8 System_Mutex_Create(SystemMutexType* mutexHandle);

/*
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] handle
*Pointer point to mutex handle.
*
*@param[inout] None
*@param[out] None
*
*@retval E_OK: Request successed.
*@retval E_NOT_OK: Request failed.
*
*Service to delete a mutex.
*/
extern uint8 System_Mutex_Delete(SystemMutexType handle);

/*
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] handle
*Pointer point to mutex handle.
*
*@param[in] fromISR
*STD_ON: Set this value if you call the function in interrupt context.
*STD_OFF: Set this value if you call the function except in interrupt context(exp:Task env. default value).
*
*@param[inout] None
*@param[out] None
*
*@retval E_OK: Lock successed.
*@retval E_NOT_OK: Lock failed.
*
*Service to get the mutex to suspend the current context.
*/
extern boolean System_Mutex_Lock(SystemMutexType handle, boolean fromISR);

/*
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] handle
*Pointer point to mutex handle.
*
*@param[in] fromISR
*STD_ON: Set this value if you call the function in interrupt context.
*STD_OFF: Set this value if you call the function except in interrupt context(exp:Task env. default value).
*
*@param[inout] None
*
*@param[out] None
*
*@retval E_OK: Unlock successed.
*@retval E_NOT_OK: Unlock failed.
*
*Service to release the mutex.
*/
extern boolean System_Mutex_Unlock(SystemMutexType handle, boolean fromISR);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: System_Mutex.h ===========================*/
