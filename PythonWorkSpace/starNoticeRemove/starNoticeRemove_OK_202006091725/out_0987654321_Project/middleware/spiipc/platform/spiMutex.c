

#include "ipcType.h"
#include "status.h"
#include "osif.h"
#include "Std_Types.h"
#include "SpiIpc.h"
#include "spiMutex.h"
#include "ipcMutex.h"
#include "SpiIpc_cfg.h"

boolean spiMutexCreate(uint8 *mutexIndex)
{
	uint8 i=0;
	boolean returnValue = E_NOT_OK;

	if(mutexIndex == NULL)
		return E_NOT_OK;
	
	for(i=0;i<gSpiIpcGlobalCfg.spiIpcMutexPoolSize;i++)
	{
		if(0 == gSpiIpcMutexHandlePool[i])
		{
			if(OSIF_MutexCreate(&gSpiIpcMutexHandlePool[i]) == STATUS_SUCCESS)
			{
				*mutexIndex = i;
				returnValue = E_OK;
				break;
			}
			else
			{
				returnValue = E_NOT_OK;
			}

		}
	}
	return returnValue;
}

void spiMutexDestroy (uint8 mutexIndex)
{
 	gSpiIpcMutexHandlePool[mutexIndex] = 0;
	
}

boolean spiMutexLock(uint8 mutexIndex)
{
	if(OSIF_MutexLock(&gSpiIpcMutexHandlePool[mutexIndex],OSIF_WAIT_FOREVER)==STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

boolean spiMutexUnlock(uint8 mutexIndex)
{
   if(OSIF_MutexUnlock(&gSpiIpcMutexHandlePool[mutexIndex])==STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

#if   1
void mutexCreate(MUTEX* mutex)
{
    OSIF_MutexCreate(mutex);
}

void mutexDestroy (MUTEX *mutex)
{
    OSIF_MutexDestroy(mutex);
}

void mutexLock(MUTEX  *mutex)
{
    OSIF_MutexLock(mutex,OSIF_WAIT_FOREVER);
}

void mutexUnlock(MUTEX *mutex)
{
    OSIF_MutexUnlock(mutex);
}
#endif

