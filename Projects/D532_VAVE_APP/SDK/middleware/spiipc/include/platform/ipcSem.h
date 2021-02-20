

#ifndef _IPC_SEM_H_
#define _IPC_SEM_H_

#include "ipcCorePlatformConfig.h"

#ifdef IPC_PLTF_IS_S32

#include "osif.h" 
typedef semaphore_t IpcSemHandle;

#endif



IpcSemHandle  ipcSemCreate(void);
void          ipcSemDestroy(IpcSemHandle handle);

void          ipcSemPost(IpcSemHandle handle);
void          ipcSemWait(IpcSemHandle handle);





#endif

