/**
*@file  System_Portable.h
*@brief Please describle your module here.
*@date    2017-12-22 15:44:22
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-12-22  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _SYSTEM_PORTALBE_H_
#define _SYSTEM_PORTALBE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define SYS_OS_NOOS (0U)
#define SYS_OS_FREERTOS (1U)
#define SYS_OS_AUTOSAR (2U)
#define SYS_OS_UCOS (3U)

#define SYS_HW_VER_S00 (0U)
#define SYS_HW_VER_S01 (1U)
#define SYS_HW_VER_P00 (2U)
#define SYS_HW_VER_P01 (3U)

#define SYS_OS_TYPE SYS_OS_FREERTOS
#define SYS_HW_VERSION SYS_HW_VER_S00

#if (SYS_OS_TYPE == SYS_OS_FREERTOS)
#include "FreeRTOS.h"
#define EnterCriticalCode() portENTER_CRITICAL()
#define ExitCriticalCode() portEXIT_CRITICAL()
#define Mem_Malloc(wantedSize) pvPortMalloc(wantedSize)
#define Mem_Free(bufPtr) vPortFree(bufPtr)
#else
#define EnterCriticalCode() while(0)
#define ExitCriticalCode() while(0)
#define Mem_Malloc(wantedSize) while(0)
#define Mem_Free(bufPtr) while(0)

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_SYSTEM_PORTALBE_H_
/*=========================== END OF FILE: System_Portable.h ===========================*/
