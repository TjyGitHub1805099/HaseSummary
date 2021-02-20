/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : 
            Modifly:brown
**
**          Date  : 2017-06-20
**          
**          AutoSAR 3.0
******************************************************************************/
#ifndef  _MEMIF_H_
#define  _MEMIF_H_

#include "Std_Types.h"
#include "Ea.h" 
#include "MemIf_types.h"

//#include "Mem_Cfg.h" 



#ifdef __cplusplus
extern "C" {
#endif


#if(STD_ON == EaSetModeSupported) 
void MemIf_SetMode(MemIf_ModeType Mode); 
#endif

Std_ReturnType MemIf_Read(uint8 DeviceIndex,uint16 BlockNumber, uint16 BlockOffset,uint8* DataBufferPtr, uint16 Length);
Std_ReturnType MemIf_Write(uint8 DeviceIndex, uint16 BlockNumber,uint8* DataBufferPtr); 
void MemIf_Cancel(uint8 DeviceIndex);	
MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex);
MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex);
Std_ReturnType MemIf_InvalidateBlock(uint8 DeviceIndex,uint16 BlockNumber);
Std_ReturnType MemIf_EraseImmediateBlock(uint8 DeviceIndex,uint16 BlockNumber);


#ifdef __cplusplus
}
#endif

#endif

