
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :LptmrWakeUp.h
**          Author: huang
**
**          Date  : 2020-06-16
**          
**          
******************************************************************************/

#ifndef _LPTMR_WAKEUP__H_
#define _LPTMR_WAKEUP__H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "lpTmr1.h"
#include "BoardDefines.h"

void LptmrWakeUpInit(void);
void WakeUpSourceDiasbleIRQ(void);
void LptmrStartCounter(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif







