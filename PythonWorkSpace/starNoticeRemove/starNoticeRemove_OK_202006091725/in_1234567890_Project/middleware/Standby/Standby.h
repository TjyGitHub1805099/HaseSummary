
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Port_if.h
**          Author: brown
**
**          Date  : 2018-06-22
**          
**          
******************************************************************************/

#ifndef _STANDBY__H_
#define _STANDBY__H_
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "Std_Types.h"
#include "Cpu.h"
#include "backram_var.h"

//#define CRASH_CALL_BACK_DEEP 8
//#define CRASH_HISTORY_FLAG 0x13579246

#define cpu_data_t uint32
#define cpu_addr_t uint32

typedef struct fault_cpu_frame_st {
    cpu_data_t r0;
    cpu_data_t r1;
    cpu_data_t r2;
    cpu_data_t r3;
    cpu_data_t r12;
    cpu_data_t lr;
    cpu_data_t pc;
    cpu_data_t spsr;
} fault_cpu_frame_t;

typedef struct fault_exc_frame_st {
    fault_cpu_frame_t cpu_frame;
} fault_exc_frame_t;


//=================================sys sleep in main=========================
#define SYS_SLEEP_IN_MAIN	STD_ON

#if(STD_ON == SYS_SLEEP_IN_MAIN)
#include "MemMap.h"
extern BACKRAM_START_SEC_VAR_UNSPECIFIED uint32 g_SysSleepInMainCmd;
extern void reInitGpioPin(void);
#endif

void SysStandbyClockInit(void);
void SysEntryStandby(void);
extern void DisableIrq(void);
extern void SysLvi2DetEnable(void);

extern NVM_SEC_VAR_UNSPECIFIED uint32 gCrashRegisterRecord[];


#ifdef __cplusplus
}
#endif // __cplusplus
#endif
