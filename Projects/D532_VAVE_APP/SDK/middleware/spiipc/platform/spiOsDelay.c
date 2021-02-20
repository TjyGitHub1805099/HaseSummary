

#include "spiOsDelay.h"
#include "ipcType.h"
#include "status.h"
#include "osif.h" 


void spiOsDelay(uint32 Delay)
{
	OSIF_TimeDelay(Delay);
}




