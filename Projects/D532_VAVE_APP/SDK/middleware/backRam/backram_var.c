#include "backram_var.h"

//Descript:
//1.backram segment define in file
//2.Must be Upgrade Firmware Flag In First valid row
//3.Other variable Append At End

//==========================Upgrade Firmware Flag :start==========================//
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_BootLoaderJumpCmd;  //this var must be the first one, map to bootloader
//==========================Upgrade Firmware Flag :start==========================//

BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_DeaResetMagicWord;
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrRegisterRecord[CRASH_CALL_BACK_DEEP];
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrFlag;

//Here Append .....
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_SysSleepInMainCmd;//add by tjy

