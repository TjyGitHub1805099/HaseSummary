

#include "osif.h"
#include "SpiIpc.h"
#include "ipcType.h"
#include "status.h"
#include "SpiIpc_cfg.h"

#ifdef IPC_USE_RTOS
boolean spiSemCreate(const uint8 uxInitialCount)
{
	boolean returnValue = E_NOT_OK;
	uint8 i = 0;
	
	for(i = 0; i < gSpiIpcGlobalCfg.spiIpcSemPoolSize; i++)
	{
		if(0 == gSpiSemPool[i])
		{
			if(STATUS_SUCCESS == OSIF_SemaCreate(&gSpiSemPool[i], uxInitialCount))
			{
				returnValue = E_OK;
			}
		}
	}
	return (returnValue);
}

void spiSemPost(uint8 HwChannel)
{
	OSIF_SemaPost(&gSpiSemPool[HwChannel]);
}

boolean spiSemWait(uint8 HwChannel)
{
	boolean returnValue = E_NOT_OK;
	
	if(STATUS_SUCCESS ==  OSIF_SemaWait(&gSpiSemPool[HwChannel], portMAX_DELAY))
	{
		returnValue = E_OK;
	}
	return returnValue;
}

#endif

