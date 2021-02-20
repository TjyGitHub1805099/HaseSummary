#include "System_Portable.h"
#include "System_Portable.h"
#include "System_Mutex.h"

extern volatile boolean gSystemInitDoneFlag;
static SystemMutexType sNvmMainMutexHdl = 0;

void EnterCritical()
{
	EnterCriticalCode();
}

void ExitCritical()
{
	ExitCriticalCode();
}

void Nvm_MainMutexInit()
{
  if(sNvmMainMutexHdl == 0)
    System_Mutex_Create(&sNvmMainMutexHdl);
}


void Nvm_UnlockMainMutex()
{
  if(TRUE == gSystemInitDoneFlag && sNvmMainMutexHdl !=0)
    System_Mutex_Unlock(sNvmMainMutexHdl, STD_OFF);
}

void Nvm_LockMainMutex()
{
  if(TRUE == gSystemInitDoneFlag && sNvmMainMutexHdl !=0)
    System_Mutex_Lock(sNvmMainMutexHdl, STD_OFF);
}


