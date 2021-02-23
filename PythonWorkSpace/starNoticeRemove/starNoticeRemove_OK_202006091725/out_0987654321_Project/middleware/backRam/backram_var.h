#ifndef __BACKRAM_VAR__
#define __BACKRAM_VAR__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "MemMap.h"
#include "Standby.h"

#define CRASH_CALL_BACK_DEEP 8
#define CRASH_HISTORY_FLAG 0x13579246

extern BACKRAM_START_SEC_VAR_UNSPECIFIED volatile uint32 g_BootLoaderJumpCmd;  //this var must be the first one, map to bootloader
extern BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_DeaResetMagicWord;
extern BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrRegisterRecord[CRASH_CALL_BACK_DEEP];
extern BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_defaultIsrFlag;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_LCDBACKLIGHT_
