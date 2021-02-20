/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Ea_cfg.h
**          Modifly:brown
**			author: weltry wang
**          Date  : 2017-06-20
**          
**          History:
**          1. Date:2018-02-26
**             Author:weltry wang
**             Modification:
**
******************************************************************************/





#ifndef NvM_IF_H
#define NvM_IF_H
#ifdef __cplusplus
}
#endif

#include "NvM.h"

#include "NvM_Cfg.h"






/*====================[P R O T O T Y P E S]====================*/


 
void NvM_ResetForWriteAll();

/*************************************************************************/
/*
 * Brief               Service for resetting all internal variables
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_Init(void);

/*************************************************************************/
/*
 * Brief               Service to cancel a running NvM_WriteAll request
 * ServiceId           0x0a
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request shall only be used by the ECU state manager
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_CancelWriteAll(void);

/*************************************************************************/
/*
 * Brief               Initiates a multi block read request
 * ServiceId           0x0c
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request may be triggered only by the ECU state manager at system startup
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_ReadAll(void);

/*************************************************************************/
/*
 * Brief               Initiates a multi block write request
 * ServiceId           0x0d
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        This request must only be triggered by the ECU state manager at shutdown of the system
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_WriteAll(void);

/*************************************************************************/
/*
 * Brief               Service for performing the processing of the NvM jobs
 * ServiceId           0x0e
 * Timing              VARIABLE_CYCLIC
 * Param-Name[in]      None
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_MainFunction(void);

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
/*************************************************************************/
/*
 * Brief               Service for setting the DataIndex of a dataset NVRAM block
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                            A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                   DataIndex: Index position (association) of a NV/ROM block.
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex);

/*************************************************************************/
/*
 * Brief               Service for getting the currently set DataIndex of a dataset NVRAM block
 * ServiceId           0x02
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   DataIndexPtr: Pointer to where to store the current dataset index (0..255)
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8 * DataIndexPtr);

/*************************************************************************/
/*
 * Brief               Service to copy the data of the NV block to its corresponding RAM block
 * ServiceId           0x06
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   NvM_DstPtr: Pointer to the RAM data block
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                   E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, uint8 * NvM_DstPtr);

/*************************************************************************/
/*
 * Brief               Service to copy the data of the RAM block to its corresponding NV block
 * ServiceId           0x07
 * Sync/Async          Asynchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                   NvM_SrcPtr: Pointer to the RAM data block
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                   E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, uint8 * NvM_SrcPtr);


/*************************************************************************/
/*
 * Brief               Service to restore the default data to its corresponding RAM block
 * ServiceId           0x08
 * Sync/Async          Asynchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              E_OK: request has been accepted
 *                   E_NOT_OK: request has not been accepted
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, uint8 * NvM_DestPtr);

/*************************************************************************/
/*
 * Brief               Service to read the block dependent error/status information
 * ServiceId           0x04
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 * Param-Name[out]   RequestResultPtr: Pointer to where to store the request result
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_GetErrorStatus(NvM_BlockIdType BlockId, uint8 * RequestResultPtr);

#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/*************************************************************************/
/*
 * Brief               Service for setting the RAM block status of an NVRAM block
 * ServiceId           0x05
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The block identifier uniquely identifies one NVRAM block descriptor.
 *                               A NVRAM block descriptor contains all needed information about a single NVRAM block.
 *                   BlockChanged: TRUE: Validate the RAM block and mark block as changed
 *                                 FALSE: Invalidate the RAM block and mark block as unchanged
 * Param-Name[out]   None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged);
extern NvM_JobTypeType NvM_GetCurrentJobType(void);
#endif
#endif







#ifdef __cplusplus
}
#endif
#endif

