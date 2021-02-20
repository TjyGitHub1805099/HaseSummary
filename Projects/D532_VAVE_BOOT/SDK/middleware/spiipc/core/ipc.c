
#include <stdio.h>
#include <string.h>

#include "ipc.h"
#include "SpiIpc.h"



ipcChannelRecvFn gOutputCallback = NULL;

void ipcRegisterChannelRecv(ipcChannelRecvFn output_callback)
{
    gOutputCallback = output_callback;
}

void ipcUnregisterChannelRecv(void)
{
    gOutputCallback = NULL;
}

