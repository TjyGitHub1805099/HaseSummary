
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Spi_IpcCore.h
**          Author: brown
**
**          Date  : 2018-04-23
**          
**          
******************************************************************************/

#ifndef _SPI_IPC_CORE_H_
#define _SPI_IPC_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif 
#include "Std_Types.h"

extern boolean  Spi_MComFLag;

extern void HandleSPI_PwrOnReadDumpData(uint8 HwChannel);
extern void HandleSPI_IPC_Messages(uint8 HwChannel);
#ifdef __cplusplus
}
#endif
#endif

