/******************************************************************************
**          Copyright (c) by HSAE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HSAE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : dsp_global.h
**          Author: Garin Gao
**
**          Date  : 2016-05-10
**          
**          
******************************************************************************/


#ifndef __DSP_GLOBAL_H__
#define __DSP_GLOBAL_H__

#include "Std_Types.h"

#include "Task_Audio_Cfg.h"
#if (STD_ON == TASK_AUDIO_DEBUG_TRACE)
#include "trace.h"
#endif


/******************************************************************************
**    INCLUDES
******************************************************************************/

/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/
#ifndef UINT8
typedef uint8   UINT8;
#endif

#ifndef INT8
typedef sint8     INT8;
#endif

#ifndef UINT16
typedef uint16  UINT16;
#endif

#ifndef INT16
typedef sint16    INT16;
#endif

#ifndef UINT32
typedef uint32    UINT32;
#endif

#ifndef INT32
typedef sint32      INT32;
#endif

/******************************************************************************
**    MACROS
******************************************************************************/

#ifndef  FALSE
#define  FALSE 0
#endif

#ifndef  TRUE
#define  TRUE  1
#endif

#ifndef  NULL
#define  NULL ((void *) 0)
#endif

#ifndef  INVALID_ID
#define INVALID_ID    (-1)
#endif

/******************************************************************************
**    DBG DEFINITIONS
******************************************************************************/
/*define this log level is matching with linux kernel log level*/
#define LOG_LEVEL_INFO    0
#define LOG_LEVEL_WARNING    1
#define LOG_LEVEL_ERRO    2
#define LOG_LEVEL_NONMASK    3

#if (STD_ON == TASK_AUDIO_DEBUG_TRACE)
	#define LOG_OUT(level, format, ...) TracePrintf(SYSTEM_TRACE, level, format, __VA_ARGS__);
#else
	#define LOG_OUT(level, format, ...)
#endif


void sleep_tsk(int ms);

#endif /*__DSP_GLOBAL_H__*/
