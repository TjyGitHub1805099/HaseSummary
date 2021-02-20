#ifndef _SWDL_FLAG_H_
#define _SWDL_FLAG_H_

#include "Std_Types.h"
#include "MemMap.h"

#define SWDL_TYPE_CAN 0X10
#define SWDL_TYPE_USB 0X20

#define SWDL_TARGET_IOC 0X01
#define SWDL_TARGET_SOC 0X04
#define SWDL_TARGET_APP 0X0A

#define SWDL_FLAG_START_ADDR MEM_SWDL_FLAG_START_ADDR

#define SWDL_APP_VALID_FLAG_START_ADDR MEM_APP_VALID_FLAG_START_ADDR
#define SWDL_APP_VALID_FLAG_SIZE MEM_APP_VALID_FLAG_SIZE

#define SWDL_APP_START_ADDR MEM_APP_START_ADDR
#define SWDL_CHIP_MAX_ADDR MEM_CHIP_MAX_ADDR

extern uint64 Swdl_GetFlagSecSize(void);
extern Std_ReturnType Swdl_SetUpdateFlag(uint8 type);
extern Std_ReturnType Swdl_ClearUpdateFlag(void);
extern Std_ReturnType Swdl_GetUpdateFlag(uint8 *pFlag);
extern Std_ReturnType Swdl_IsAppValid(void);


#endif

