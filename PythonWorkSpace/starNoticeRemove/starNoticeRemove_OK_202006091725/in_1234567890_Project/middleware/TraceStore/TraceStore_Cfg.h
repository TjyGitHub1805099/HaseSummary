/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : TraceStore_Cfg.h
            Modifly:tan jin yi
**
**          Date  : 2019-12-17
**          
**          
******************************************************************************/

#ifndef TRACE_STORE_IN_SOC_CFG_H
#define TRACE_STORE_IN_SOC_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#define TRACE_STORE_USING		STD_ON

#if (STD_ON == TRACE_STORE_USING )
#define TRACE_STORE_BUFF_LEN 		(2048)//2048byte:used to store trace print data
#define TRACE_STORE_MAIN_START_LOC 	(512)
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRACE_STORE_IN_SOC_CFG_H */
