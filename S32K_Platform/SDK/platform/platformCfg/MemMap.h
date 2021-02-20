#ifndef _MEMMAP_H_
#define _MEMMAP_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define MEM_BOOTLOADER_VER_ADDR 0X71000

#define MEM_BOOTLOADER_VER_LEN 2

#define MEM_APP_START_ADDR 0xA0000
#define MEM_APP_MAIN_ENTRY_ADDR 0xA0400
#define MEM_APP_VALID_FLAG_START_ADDR 0xA0404
#define MEM_APP_VALID_FLAG_SIZE 4
#define MEM_SWDL_FLAG_START_ADDR 0X4000
#define MEM_CHIP_MAX_ADDR 0xFFBFF

//#define APPVALID_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED #pragma segment CONST=SEC_APPVALID
//#define APPVALID_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED #pragma segment CONST

#define APPVALID_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#define APPVALID_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED

#define APPINFO_START_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED
#define APPINFO_STOP_SEC_VAR_FAST_POWER_ON_INIT_UNSPECIFIED

#define NVM_START_SEC_VAR_UNSPECIFIED
#define NVM_STOP_SEC_VAR_UNSPECIFIED

/*
 * Example:
 * Step 1: #include "MemMap.h"
 * Step 2: NVM_VAR_DEFINE(uint32,nvmVarTest)=0x11223344;
 * Warning: All variable must initial.
 * */
#define NVM_VAR_DEFINE(type,name) type name __attribute__ ((section(".nvmSection")))

#define SWDL_START_SEC_VAR_FAST_POWER_ON_NOINIT_UNSPECIFIED
#define SWDL_STOP_SEC_VAR_FAST_POWER_ON_NOINIT_UNSPECIFIED

#ifdef __cplusplus
}
#endif // __cplusplus

#endif

