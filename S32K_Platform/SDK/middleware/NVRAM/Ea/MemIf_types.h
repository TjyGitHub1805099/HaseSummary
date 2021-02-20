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
#ifndef  _EemIF_TYPE_H_
#define  _EemIF_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Platform_Types.h"
#include "Std_Types.h"


typedef enum  
{	
	MEMIF_JOB_OK,
	MEMIF_JOB_FAILED,
	MEMIF_JOB_PENDING,
	MEMIF_JOB_CANCELED,
	MEMIF_BLOCK_INCONSISTENT,
	MEMIF_BLOCK_INVALID,
}MemIf_JobResultType; 

typedef enum  
{	
	MEMIF_MODE_SLOW = 0,
	MEMIF_MODE_FAST = 1,
}MemIf_ModeType; 

typedef enum  
{	
	MEMIF_UNINIT,	
	MEMIF_IDLE,
 	MEMIF_BUSY,		
	MEMIF_BUSY_INTERNAL,
}MemIf_StatusType;


#ifdef __cplusplus
}
#endif

#endif






