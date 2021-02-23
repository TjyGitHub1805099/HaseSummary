/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm_types.c
**          Modifly:brown
**			author: Stephen Du
**          Date  : 2017
**          
**          History:
**          1. Date:2017-06-20
**             Author:weltry wang
**             Modification:
**
******************************************************************************/
/*============================================================================*/

/*======================[R E V I S I O N   H I S T O R Y]=====================*/
/*  <VERSION>    <DATE>    <AUTHOR>    <REVISION LOG>
 *   V1.0.0     20170610  Stephen Du  Initial version
 *
 */
/*============================================================================*/
#ifndef NVM_TYPES_H
#define NVM_TYPES_H

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define NVM_TYPES_H_AR_MAJOR_VERSION 2U
#define NVM_TYPES_H_AR_MINOR_VERSION 3U
#define NVM_TYPES_H_AR_PATCH_VERSION 0U
#define NVM_TYPES_H_SW_MAJOR_VERSION 1U
#define NVM_TYPES_H_SW_MINOR_VERSION 0U
#define NVM_TYPES_H_SW_PATCH_VERSION 7U

/*================================[M A C R O S]===============================*/
#define NVM_API_CONFIG_CLASS_1 1U     /* All API calls belonging to class 1 */
#define NVM_API_CONFIG_CLASS_2 2U     /* All API calls belonging to class 2 */
//#define NVM_API_CONFIG_CLASS_3 3U     /* All API calls belonging to class 3 */
/*Mark: The above three API class define must located before NvM_Cfg.h include*/

/*==============================[I N C L U D E S]=============================*/
#include "NvM_Cfg.h" 
#include "Std_Types.h"

/*================================[M A C R O S]===============================*/
/*
 * This is an asynchronous request result returned by the API service NvM_GetErrorStatus.
 * The availability of an asynchronous request result can be additionally signaled
 * via a callback function.
 */
#define NvM_RequestResultType uint8 /* @req NVM083 NVM470 */

/*
 * The last asynchronous read/write/control request has been finished successfully.
 * This shall be the default value after reset. This status shall have the value 0.
 */
#define NVM_REQ_OK               (NvM_RequestResultType)(0U) /* @req NVM470 */

/*
 * The last asynchronous read/write/control request has been finished unsuccessfully.
 */

#define NVM_REQ_NOT_OK           (NvM_RequestResultType)(1U) /* @req NVM470 */
/*
 * An asynchronous read/write/control request is currently pending.
 */

#define NVM_REQ_PENDING          (NvM_RequestResultType)(2U) /* @req NVM470 */
/*
 * The result of the last asynchronous request NvM_ReadBlock or
 * NvM_ReadAll is a data integrity failure.
 * In case of NvM_ReadBlock the content of the RAM block has changed but has become invalid.
 * The application is responsible to renew and validate the RAM block content.
 */

#define NVM_REQ_INTEGRITY_FAILED (NvM_RequestResultType)(3U) /* @req NVM470 */
/*
 * The referenced block was skipped during execution of NvM_ReadAll or NvM_WriteAll,
 * Dataset NVRAM blocks (NvM_ReadAll) or NVRAM blocks without a permanently configured RAM block.
 */

#define NVM_REQ_BLOCK_SKIPPED    (NvM_RequestResultType)(4U) /* @req NVM470 */
/*
 * The referenced NV block is invalidated.
 */

#define NVM_REQ_NV_INVALIDATED   (NvM_RequestResultType)(5U) /* @req NVM470 */
/*
 * The multi block request NvM_WriteAll was cancelled by calling NvM_CancelWriteAll.
 */

#define NVM_REQ_CANCELLED        (NvM_RequestResultType)(6U) /* @req NVM470 */

/*
 * Development error values are of type uint8.
 */
#define NvM_DevelopErrorType (uint8) /* @req NVM187 */

/*
 * API requests called with wrong block ID.
 */
#define NVM_E_PARAM_BLOCK_ID       NvM_DevelopErrorType(0x0A) /* @req NVM023 */

/*
 * API requests called with wrong block type.
 */
#define NVM_E_PARAM_BLOCK_TYPE     NvM_DevelopErrorType(0x0B) /* @req NVM023 */

/*
 * API requests called with wrong block index.
 */
#define NVM_E_PARAM_BLOCK_DATA_IDX NvM_DevelopErrorType(0x0C) /* @req NVM023 */

/*
 * API requests called with wrong address.
 */
#define NVM_E_PARAM_ADDRESS        NvM_DevelopErrorType(0x0D) /* @req NVM023 */

/*
 * API requests called with wrong block data.
 */
#define NVM_E_PARAM_DATA           NvM_DevelopErrorType(0x0E) /* @req NVM023 */

/*
 * NVRAM manager is still not initialized.
 */
#define NVM_E_NOT_INITIALIZED      NvM_DevelopErrorType(0x14) /* @req NVM023 */

/*
 * API read/write/control request failed because a block with the same ID
 * is already listed or currently in progress.
 */
#define NVM_E_BLOCK_PENDING        NvM_DevelopErrorType(0x15) /* @req NVM023 */

/*
 * NVRAM manager job queue overflow occurred.
 */
#define NVM_E_LIST_OVERFLOW        NvM_DevelopErrorType(0x16) /* @req NVM023 */

/*
 * A write attempt to a write protected NVRAM block was requested.
 */
#define NVM_E_NV_WRITE_PROTECTED   NvM_DevelopErrorType(0x17) /* @req NVM023 */

/*
 * The service is not possible with this block configuration.
 */
#define NVM_E_BLOCK_CONFIG         NvM_DevelopErrorType(0x18) /* @req NVM023 */

/*
 * Value Range:
 * 0..2^(16-@link NVM_DATASET_SELECTION_BITS NVM_DATASET_SELECTION_BITS @endlink)-1
 *
 * Description:
 * Identification of a NVRAM block via a unique block identifier.
 * Reserved NVRAM block IDs:
 *
 * note:
 * 0 -> to derive multi block request results via NvM_GetErrorStatus.
 * 1 -> redundant NVRAM block which holds the configuration ID.
 */
#define NvM_BlockIdType uint16 /* @req NVM471 */

/*
 * Block CRC type enum type define
 */
typedef enum
{
	NVM_CRC16 = 1,
	NVM_CRC32
} NvM_BlockCRCType;

/*
 * Block management type enum type define
 */
typedef enum
{
	NVM_BLOCK_NATIVE = 1,
	NVM_BLOCK_REDUNDANT,
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
	NVM_BLOCK_DATASET
#endif
} NvM_BlockManagementType;

/*
 * Brief    Per block callback routine which shall be called by the NvM module to
 *          copy default data to a RAM block if a ROM block isn't configured.
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK: Callback function has not been processed successfully.
 */
typedef Std_ReturnType (*NvM_InitBlockCallbackType)(void);  /* A pointer to initial callback function.*/

/*
 * Brief    Per block callback routine to notify the upper layer that
 *          an asynchronous single block request has been finished.
 *
 * Return   E_OK: Callback function has been processed successfully
 *          E_NOT_OK: Callback function has not been processed successfully.
 */
typedef Std_ReturnType (*NvM_SingleBlockCallbackType)(uint8 ServiceId, NvM_RequestResultType JobResult);  /* A pointer to single block callback function.*/

/*
 * Common callback routine to notify the upper layer that
 * an asynchronous multi block request has been finished.
 */
typedef void (*NvM_MultiBlockCallbackType)(uint8 ServiceId, NvM_RequestResultType JobResult);  /* A pointer to multi block callback function.*/

/*
 * Function Pointer type define, with void parameter and void return value.
 */
typedef void (*NvM_VoidFuncVoidPtr)(void);

#if(STD_ON==NVM_JOB_PRIORITIZATION)
/*
 * The priority table size
 */
#define NVM_TABLE_SIZE_PRIORITY  17U

/*
 * The job queue table size
 */
#define NVM_TABLE_SIZE_JOB_QUEUE 17U
#endif

/*
 * The CRC space byte size
 */
#define NVM_NV_CRC_MAX_LENGTH 4U

#define NVM_ADMIN_ENQUEUE 0U
#define NVM_ADMIN_RAM_VALID 1U
#define NVM_ADMIN_RAM_CHANGED 2U
#define NVM_ADMIN_NV_WRPRO 3U
#define NVM_ADMIN_NV_REPAIR 4U


#define NVM_BDSC_WRITEBLOCKONCE 0U
#define NVM_BDSC_BLOCKUSECRC 1U
#define NVM_BDSC_BLOCKWRITEPROT 2U
#define NVM_BDSC_CALCRAMBLOCKCRC 3U
#define NVM_BDSC_RESISTANTTOCHANGEDSW 4U
#define NVM_BDSC_SELECTBLOCKFORREADALL 5U
#define NVM_BDSC_SELECTBLOCKFORWRITEALL 6U

#define NVM_ATOMJOB_MAX_DEPTH 4U
#define NVM_READALL_FIRST_BLOCKID 0x01U
#define NVM_WRITEALL_FIRST_BLOCKID 0x02U
#define NVM_QUEUE_EMPTY_COUNT 0U

typedef enum
{
	NVM_SERV_ID_READ_BLOCK,
	NVM_SERV_ID_WRITE_BLOCK,
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if(STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
	NVM_SERV_ID_CALC_CRC,
#endif
	NVM_SERV_ID_RESTORE_BLOCK,
#endif
	NVM_SERV_ID_READ_ALL,
	NVM_SERV_ID_WRITE_ALL,
	NVM_SERV_ID_NONE
} NvM_ServiceIdType;

typedef enum
{
	NVM_ATOMJOB_READBLOCK,
	NVM_ATOMJOB_WRITEBLOCK,
	NVM_ATOMJOB_READROM,
	NVM_ATOMJOB_CALCCRC,
	NVM_ATOMJOB_NONE
} NvM_AtomJobEType;

typedef enum
{
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
	NVM_JOB_TYPE_SINGLE_NORMAL,
#if(STD_ON==NVM_JOB_PRIORITIZATION)
	NVM_JOB_TYPE_SINGLE_IMMED,
	NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED,
#endif
#endif
	NVM_JOB_TYPE_MULTI,
	NVM_JOB_TYPE_NONE
} NvM_JobTypeType;

typedef enum
{
	NVM_JOB_STEP_READ_1ST_NV,
	NVM_JOB_STEP_READ_2ND_NV,
	NVM_JOB_STEP_WRITE_1ST_NV,
	NVM_JOB_STEP_WRITE_2ND_NV,
	NVM_JOB_STEP_ERASE_1ST_NV,
	NVM_JOB_STEP_ERASE_2ND_NV,
	NVM_JOB_STEP_INVALID_1ST_NV,
	NVM_JOB_STEP_INVALID_2ND_NV,
	NVM_JOB_STEP_READ_ROM,
	NVM_JOB_STEP_CALC_CRC_READ_1ST_NV,
	NVM_JOB_STEP_CALC_CRC_READ_2ND_NV,
	NVM_JOB_STEP_CALC_CRC,
	NVM_JOB_STEP_IDLE,
	NVM_JOB_STEP_PENDING,
	NVM_JOB_STEP_CANCEL
} NvM_JobStepType;

typedef enum
{
	NVM_MEMIF_JOB_IDLE,
	NVM_MEMIF_JOB_ASYNC_READY,
	NVM_MEMIF_JOB_ASYNC_PENDING
} NvM_MemIfStateType;

typedef enum
{
	NVM_RUNTIME_NONE,
	NVM_RUNTIME_NOT_SURE,
	NVM_RUNTIME_NORMAL
} NvM_RuntimeType;

typedef struct
{
	uint8 Init;
#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
	uint8 CurActPriTable;
#endif
	NvM_JobTypeType CurrentJobType;
	NvM_RuntimeType RuntimeType;
	NvM_MemIfStateType MemIfJobState;
	NvM_JobStepType JobStep;
} NvM_ModuleType;

typedef struct
{
	uint8 Enqueue;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
	NvM_ServiceIdType ServiceId;
#if(STD_ON==NVM_JOB_PRIORITIZATION)
	NvM_BlockIdType BlockId;
#endif
#endif
	uint8 CancelWriteAll;
	uint8 ID1WriteMark;
	NvM_RequestResultType ReqResult;
} NvM_MultiJobType;

typedef struct
{
	uint8 Count;
	NvM_AtomJobEType ReqId[NVM_ATOMJOB_MAX_DEPTH];
} NvM_AtomJobType;

typedef struct
{
	NvM_BlockIdType BlockId;
	uint8 DeviceId;
	NvM_ServiceIdType ServiceId;
	uint8 Index;
	uint8 NvNum;
	uint8 RomNum;
	uint8 AdminFlagGroup;
	uint8 BDSCFlagGroup;
	uint8 WriteRetryCounter;
	uint8 ESingleReqResult;
	uint8 EMultiReqResult;
	NvM_BlockManagementType ManagementType;
	NvM_BlockCRCType CRCType;
	uint16 BaseNumber;
	uint16 Length;
	uint8* RamAddr; 
	uint8* RomAddr; 
	uint8 CrcFlag;
	uint8* CrcAddr;
	uint16 CrcLength;
	uint32 Crc;
	uint32 TempCrc;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
	NvM_InitBlockCallbackType InitCallback;
#endif
#if (STD_OFF == NVM_POLLING_MODE)
	NvM_SingleBlockCallbackType SingleCallback;
#endif
} NvM_CurRunningType;

typedef struct
{
	/*
	 * Bit 0:Enqueue
	 * Bit 1:RamValid
	 * Bit 2:RamChanged
	 * Bit 3:NvWriteProt
	 * Bit 4:NvRepair
	 * Bit 5:IfWrited
	 */
	uint8 FlagGroup;
	uint8 CurrentIndex;
	NvM_RequestResultType SingleReqResult;
	uint32 Crc;
} NvM_AdminBlockType;

#if(STD_ON==NVM_JOB_PRIORITIZATION)
typedef struct
{
	uint8 HeadIndex;
	uint8 TailIndex;
} NvM_PriTable2QueueType;

typedef struct
{
	NvM_BlockIdType BlockId;
	uint8* DestSrcPtr;
} NvM_ImmedQueueType;
#endif

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
typedef struct
{
	uint8 Count;
	uint8 HeadIndex;
	uint8 TailIndex;
} NvM_RoundRobinQueueManageType;

typedef struct
{
#if(STD_ON==NVM_JOB_PRIORITIZATION)
	uint8 NextIndex;
#endif
	NvM_BlockIdType BlockId;
	NvM_ServiceIdType ServiceId;
	uint8* DestSrcPtr;
} NvM_StandQueueType;
#endif

typedef struct
{
	uint8 NvmNvramDeviceId;
	NvM_BlockManagementType NvmBlockManagementType;
#if (STD_ON==NVM_JOB_PRIORITIZATION)
	uint8 NvmBlockJobPriority;
#endif
	/*
	 * Bit 0:NvmWriteBlockOnce
	 * Bit 1:NvmBlockUseCrc
	 * Bit 2:NvmBlockWriteProt
	 * Bit 3:NvmCalcRamBlockCrc
	 * Bit 4:NvmResistantToChangedSw
	 * Bit 5:NvmSelectBlockForReadall
	 * Bit 6:NvmSelectBlockForWriteall	 
	 */
	uint8 FlagGroup;
	NvM_BlockCRCType NvmBlockCRCType;
	uint16 NvmNvBlockBaseNumber;
	uint16 NvmNvBlockLength;
	uint8 NvmNvBlockNum;
	uint8 NvmRomBlockNum;
	uint8* NvmRamBlockDataAddress;
	//uint8* NvmRomBlockDataAddress;
	NvM_InitBlockCallbackType NvmInitBlockCallback;
	NvM_SingleBlockCallbackType NvmSingleBlockCallback;
} NvM_BlockDescriptorType;

#endif /* End of NVM_TYPES_H*/

/*====================[E N D   O F   F I L E]====================*/

