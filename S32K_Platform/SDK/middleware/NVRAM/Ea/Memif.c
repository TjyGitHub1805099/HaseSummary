
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
**          
******************************************************************************/

#include "Memif.h"
#include "Mem_Cfg.h" 
#include "ea_if.h"


#if(1 == DeviceIndexAllNumbers)
	Std_ReturnType MemIf_Read(uint8 DeviceIndex,uint16 BlockNumber, uint16 BlockOffset,uint8* DataBufferPtr, uint16 Length)
	{
		return Ea_Read(BlockNumber,BlockOffset,DataBufferPtr,Length);
		
	}
	Std_ReturnType MemIf_Write(uint8 DeviceIndex, uint16 BlockNumber,uint8* DataBufferPtr)
	{
		
		return Ea_Write(BlockNumber,DataBufferPtr);
		
	}
	void MemIf_Cancel(uint8 DeviceIndex)
	{

	}
	MemIf_StatusType MemIf_GetStatus(uint8 DeviceIndex)
	{
		return Ea_GetStatus();
	}
	MemIf_JobResultType MemIf_GetJobResult(uint8 DeviceIndex)
	{
		return Ea_GetJobResult();
	}
	Std_ReturnType MemIf_InvalidateBlock(uint8 DeviceIndex,uint16 BlockNumber)
	{
		return Ea_InvalidateBlock(BlockNumber);
	}
	Std_ReturnType MemIf_EraseImmediateBlock(uint8 DeviceIndex,uint16 BlockNumber)
	{
		return Ea_EraseImmediateBlock(BlockNumber);
	}
#endif










