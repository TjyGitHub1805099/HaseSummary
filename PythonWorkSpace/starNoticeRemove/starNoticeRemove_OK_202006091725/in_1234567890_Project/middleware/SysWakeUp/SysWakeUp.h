
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :SysWakeUp.h
**          Author: brown
**
**          Date  : 2018-07-23
**          
**          
******************************************************************************/

#ifndef _SYS_WAKE_UP__H_
#define _SYS_WAKE_UP__H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "Std_Types.h"
#include "Cpu.h"

extern volatile boolean gSystemsleeping;
extern volatile boolean gInterruptAwake;

void SysLviWakeUpEnable(void);
void SysLocationWakeUpEnable(void);
void SysCanWakeUpEnable(void);

#if 0
void SysWakeUpEnable(void);
#endif
#ifdef __cplusplus
}
#endif // __cplusplus
#endif







