#include "backram_var.h"







BACKRAM_START_SEC_VAR_UNSPECIFIED volatile uint32 g_BootLoaderJumpCmd;  


BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_DeaResetMagicWord;
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrRegisterRecord[CRASH_CALL_BACK_DEEP];
BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrFlag;


BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_SysSleepInMainCmd;

