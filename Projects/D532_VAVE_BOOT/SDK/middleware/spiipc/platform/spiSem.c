/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcSemFreeRTOS.c
 * @brief    ##### 
 * @details  this file ###
 * @author   
 * @version  1.0.0
 * @date     4/20/2017
 **************************************************************************/
//#include "ipcSem.h"
#include "osif.h"
#include "SpiIpc.h"
#include "ipcType.h"
#include "status.h"

extern semaphore_t gSpiSemPool[];
extern const SpiIpcGlobalStruct gSpiIpcGlobalCfg;

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
	status_t Flag;
	Flag = OSIF_SemaPost(&gSpiSemPool[HwChannel]);
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

