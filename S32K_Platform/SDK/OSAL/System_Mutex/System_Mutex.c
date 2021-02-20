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

SystemMutexType System_Mutex_Create(void)
{
	return (xSemaphoreCreateMutex());
}

boolean System_Mutex_Lock(SystemMutexType handle, boolean fromISR)
{
	boolean returnValue = E_NOT_OK;

	if (NULL != handle)
	{
		if (STD_ON == fromISR)
		{
			if (pdPASS == xSemaphoreTakeFromISR(handle, NULL))
			{
				returnValue = E_OK;
			}
		}
		else
		{
			if (pdPASS == xSemaphoreTake( handle, portMAX_DELAY ))
			{
				returnValue = E_OK;
			}
		}
	}
	return (returnValue);
}

boolean System_Mutex_Unlock(SystemMutexType handle, boolean fromISR)
{
	boolean returnValue = E_NOT_OK;

	if (NULL != handle)
	{
		if (STD_ON == fromISR)
		{
			if (pdPASS == xSemaphoreGiveFromISR(handle, NULL))
			{
				returnValue = E_OK;
			}
		}
		else
		{
			if (pdPASS == xSemaphoreGive(handle))
			{
				returnValue = E_OK;
			}
		}
	}
	return (returnValue);
}

/*=========================== END OF FILE: System_Mutex.c ===========================*/
