


#ifndef IPC_MUTEX_H
#define IPC_MUTEX_H


//#include "ipcCorePlatformConfig.h"


//#ifdef IPC_PLTF_IS_SH2

//#include "kernel.h"
//#include "kernel_api.h"

//typedef  ER_ID  MUTEX;


//#elif defined IPC_PLTF_IS_S32

#include "osif.h"
typedef mutex_t MUTEX;

//#else

//#include "../../platform/common_posix/mutex.h"

//typedef  MUTEX  IPC_MUTEX;

//#endif


void mutexCreate(MUTEX* mutex);


void mutexDestroy (MUTEX *mutex);

void mutexLock(MUTEX  *mutex);

void mutexUnlock(MUTEX *mutex);



#endif


