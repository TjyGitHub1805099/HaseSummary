/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm.c
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
 *   V1.0.0     20170000   Stephen Du  Initial version
 *
 */
/*============================================================================*/

/* @req NVM076 */
/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define NVM_C_AR_MAJOR_VERSION 2U
#define NVM_C_AR_MINOR_VERSION 3U
#define NVM_C_AR_PATCH_VERSION 0U
#define NVM_C_SW_MAJOR_VERSION 1U
#define NVM_C_SW_MINOR_VERSION 0U
#define NVM_C_SW_PATCH_VERSION 6U

/*====================[I N C L U D E S]====================*/
#include "NvM.h"      /* @req NVM077 */
#include "Memif.h"    /* @req NVM077 */
//#include "SchM_NvM.h" /* @req NVM077 */
#include "System_Module_Cfg.h"
#if(STD_ON == NVM_INCLUDE_DEM)
#include "Dem.h"      /* @req NVM077 */
#endif

#include "Crc.h"      /* @req NVM077 */

#if(STD_ON == NVM_INCLUDE_DET)
/*This is an optionally*/
#include "Det.h"      /* @req NVM077 */
#endif
//#include "System_Portable.h"
#if (STD_ON == SYS_MODULE_ADD_WDG)
//#include "wdg_pal.h"
//#include "wdog_pal_cfg.h"
#endif

extern void EnterCritical();
extern void ExitCritical();
/*====================[F U N C  D E C L A R A T I O N]====================*/
static void  NvM_SetByteBitState(uint8 * BytePtr, uint8 SetBit, uint8 OnOff);
static uint8  NvM_GetByteBitState(uint8 ByteNum, uint8 GetBit);
static void  NvM_SetJobOverFlag(NvM_RequestResultType SingleReqResult, uint8 CrcUpdate);

static Std_ReturnType NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8 * DestSrcPtr);
static Std_ReturnType NvM_JobDequeue(void);

static void NvM_AtomJobReq(NvM_AtomJobEType AtomJobReqId);
static Std_ReturnType NvM_MultiJobDispatch(void);

static void NvM_RunTimeHandle(void);
static void NvM_CopyCurBlockInfo(void);
static Std_ReturnType NvM_MemCpy(const uint8 * SrcAddrPtr, uint8 * DstAddrPtr, uint32 Length);

static void NvM_MemIfReadOk(void);
static void NvM_MemIfReadFailed(void);
static void NvM_MemIfWriteOk(void);
static void NvM_MemIfWriteFailed(void);
static void NvM_MemIfPending(void);
static void NvM_MemIfCancelled(void);
static void NvM_MemIfInconsistent(void);
static void NvM_MemIfInvalid(void);

static void NvM_Inner_ReadRom(void);
static void NvM_Inner_CalcCrc(void);
static void NvM_Inner_ReadBlock(void);
static void NvM_Inner_WriteBlock(void);
static void NvM_Inner_ReadAll(void);
static void NvM_Inner_WriteAll(void);

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if(STD_ON==NVM_JOB_PRIORITIZATION)
static Std_ReturnType NvM_Get16Bits1stSettedBit(uint16 Number, uint8 * SettedBitPtr);
static Std_ReturnType NvM_GetHighestPriJob(uint8 * JobQueueIndexPtr);
static Std_ReturnType NvM_GetQueueSpace(NvM_JobTypeType JobType, uint8 * JobQueueIndexPtr);
static void NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex);
#endif
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
static void NvM_Inner_SetRamStatus(void);
#endif
static void NvM_Inner_RestoreBlockDefaults(void);
#endif

/*====================[V A R I A N T S]====================*/

extern const uint8 NVM_BLOCK_NUM_ALL;
extern const NvM_BlockDescriptorType NvM_BlockDescriptor[];
extern uint32 __NVMSEC_RAMROM_OFFSET;

static NvM_ModuleType NvM_Module =
{
    STD_OFF,
#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
    0,
#endif
    NVM_JOB_TYPE_NONE,
    NVM_RUNTIME_NONE,
    NVM_MEMIF_JOB_IDLE,
    NVM_JOB_STEP_IDLE
};

static NvM_MultiJobType NvM_MultiJob =
{
    STD_OFF,
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    NVM_SERV_ID_NONE,
#if(STD_ON==NVM_JOB_PRIORITIZATION)
    1,
#endif
#endif
    STD_OFF,
    STD_OFF,
    NVM_REQ_OK
};

static NvM_AtomJobType NvM_AtomJob =
{
    0,
    {
        NVM_ATOMJOB_NONE,
        NVM_ATOMJOB_NONE,
        NVM_ATOMJOB_NONE,
        NVM_ATOMJOB_NONE
    }
};

static NvM_CurRunningType NvM_CurRunning =
{
    0,
    0,
    NVM_SERV_ID_NONE,
    0,
    0,
    0,
    0,
    0,
    0,
    NVM_REQ_OK,
    NVM_REQ_OK,
    NVM_BLOCK_NATIVE,
    NVM_CRC16,
    0,
    0,
    NULL_PTR,
    NULL_PTR,
    0,
    NULL_PTR,
    0,
    0,
    0,
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    NULL_PTR,
#endif
#if (STD_OFF == NVM_POLLING_MODE)
    NULL_PTR
#endif
};

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if(STD_ON==NVM_JOB_PRIORITIZATION)
static NvM_RoundRobinQueueManageType NvM_ImmedQueueManage = {0x00u, 0x00u, 0x00u};
#else
static NvM_RoundRobinQueueManageType NvM_StandQueueManage = {0x00u, 0x00u, 0x00u};
#endif
#endif
static NvM_MultiBlockCallbackType NvmMultiBlockCallback = NVM_MULTI_BLOCK_CALLBACK;

static const NvM_VoidFuncVoidPtr NvM_MemIfAsyncFuncTable[6][2] =
{
    {
        &NvM_MemIfReadOk,
        &NvM_MemIfWriteOk,
    },
    {
        &NvM_MemIfReadFailed,
        &NvM_MemIfWriteFailed,
    },
    {
        &NvM_MemIfPending,
        &NvM_MemIfPending,
    },
    {
        &NvM_MemIfCancelled,
        &NvM_MemIfCancelled,
    },
    {
        &NvM_MemIfInconsistent,
        &NvM_MemIfInconsistent,
    },
    {
        &NvM_MemIfInvalid,
        &NvM_MemIfInvalid,
    }
};

static const NvM_VoidFuncVoidPtr NvM_Inner_ServFuncMatrix[] =
{
    &NvM_Inner_ReadBlock,
    &NvM_Inner_WriteBlock,
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
    &NvM_Inner_SetRamStatus,
#endif
    &NvM_Inner_RestoreBlockDefaults,
#endif
    &NvM_Inner_ReadAll,
    &NvM_Inner_WriteAll
};

static const NvM_VoidFuncVoidPtr NvM_AtomJobFuncMatrix[] =
{
    &NvM_Inner_ReadBlock,
    &NvM_Inner_WriteBlock,
    &NvM_Inner_ReadRom,
    &NvM_Inner_CalcCrc
};

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
static uint16 NvM_PriorityTable[2][NVM_TABLE_SIZE_PRIORITY] =
{
    {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000
    },
    {
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000
    }
};
#endif

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if(STD_ON==NVM_JOB_PRIORITIZATION)
static NvM_PriTable2QueueType NvM_PriTable2Queue[2][NVM_TOTAL_NUM_DIFF_PRI];
static NvM_ImmedQueueType NvM_ImmedQueue[NVM_SIZE_IMMEDIATE_JOB_QUEUE];
#endif
static NvM_StandQueueType NvM_StandQueue[NVM_SIZE_STANDARD_JOB_QUEUE];
#endif

//static NvM_AdminBlockType NvM_AdminBlock[NVM_BLOCK_NUM_ALL + 1];
extern NvM_AdminBlockType NvM_AdminBlock[];


#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
static uint16 NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE];
#endif

//static uint8 NvM_NvDataBuffer[NVM_MAX_LENGTH_NV_BLOCK + NVM_NV_CRC_MAX_LENGTH];

extern uint8 NvM_NvDataBuffer[];

#define MemIf_READ() \
    if (NVM_CRC16 == NvM_CurRunning.CRCType) \
    { \
        l_Length = NvM_CurRunning.Length + 2; \
    } \
    else if(NVM_CRC32 == NvM_CurRunning.CRCType) \
    { \
        l_Length = NvM_CurRunning.Length + 4; \
    } \
    else \
    { \
        l_Length = NvM_CurRunning.Length + 1; \
    } \
    if (E_OK == MemIf_Read(NvM_CurRunning.DeviceId, l_BlockNum, 0, NvM_NvDataBuffer, l_Length)) \
    { \
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING; \
    }

#define MemIf_WRITE() \
    if (E_OK == MemIf_Write(NvM_CurRunning.DeviceId, l_BlockNum, NvM_NvDataBuffer)) \
    { \
        NvM_CurRunning.WriteRetryCounter--; \
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING; \
    }

#define MemIf_ERASE() \
    if (E_OK == MemIf_EraseImmediateBlock(NvM_CurRunning.DeviceId, l_BlockNum)) \
    { \
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING; \
    }

#define MemIf_INVALID() \
    if (E_OK == MemIf_InvalidateBlock(NvM_CurRunning.DeviceId, l_BlockNum)) \
    { \
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_PENDING; \
    }

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
#define NVM_MEMIF_REQ(Type) \
    if (NVM_MEMIF_JOB_ASYNC_READY == NvM_Module.MemIfJobState) \
    { \
        if ((NVM_JOB_STEP_CANCEL == NvM_Module.JobStep) && (NVM_JOB_TYPE_MULTI != NvM_Module.CurrentJobType)) \
        { \
            NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF); \
        } \
        else if (MEMIF_IDLE == MemIf_GetStatus(NvM_CurRunning.DeviceId)) \
        { \
            if (NVM_JOB_STEP_##Type##_2ND_NV == NvM_Module.JobStep) \
            { \
                NvM_CurRunning.Index += 1U; \
                l_BlockNum = NvM_CurRunning.BaseNumber + 1U;\
            } \
            else \
            { \
				l_BlockNum = NvM_CurRunning.BaseNumber; \
			} \
            MemIf_##Type() \
            else \
            { \
                NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF); \
            } \
        } \
        else \
        { \
        } \
    }
#else
#define NVM_MEMIF_REQ(Type) \
    if (NVM_MEMIF_JOB_ASYNC_READY == NvM_Module.MemIfJobState) \
    { \
        if (MEMIF_IDLE == MemIf_GetStatus(NvM_CurRunning.DeviceId)) \
        { \
            if (NVM_JOB_STEP_##Type##_2ND_NV == NvM_Module.JobStep) \
            { \
                NvM_CurRunning.Index += 1U; \
                l_BlockNum = NvM_CurRunning.BaseNumber + 1U;\
            } \
            else \
            { \
				l_BlockNum = NvM_CurRunning.BaseNumber; \
			} \
            MemIf_##Type() \
            else \
            { \
                NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF); \
            } \
        } \
    }
#endif


#define NVM_SET_MEMIF_JOB_READY(Step) \
    NvM_Module.JobStep = NVM_JOB_STEP_##Step##_NV; \
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY

/*====================[F U N C T I O N   I M P L E M E N T A T I O N S]====================*/
/*************************************************************************/
/*
 * Brief               Set one bit of byte to 0 or 1
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      SetBit: Which bit to be setted
 *                     OnOff:  STD_ON means setted to 1
 *                             STD_OFF means setted to 0
 * Param-Name[out]     None
 * Param-Name[in/out]  BytePtr: Pointer to the Byte number to be setted
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void  NvM_SetByteBitState(uint8 * BytePtr, uint8 SetBit, uint8 OnOff)
{
    if (STD_ON == OnOff)
    {
        (*BytePtr) |= (uint8)(0x01U << SetBit);
    }
    else
    {
        (*BytePtr) &= (uint8)(~(uint8)(0x01U << SetBit));
    }
    return;
}

/*************************************************************************/
/*
 * Brief               Get the specfied bit status of byte
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      ByteNum: The source byte number to be getted
 *                     GetBit:  Which bit state you want to get
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              0: Means this bit equal 0
 *                       1: Means this bit equal 1
 * PreCondition        None
 */
/*************************************************************************/
static uint8 NvM_GetByteBitState(uint8 ByteNum, uint8 GetBit)
{
    uint8 l_Number = 0U;

    if (0U != (ByteNum & (uint8)(0x01U << GetBit)))
    {
        l_Number = 1;
    }
    return (l_Number);
}

/*************************************************************************/
/*
 * Brief               Copy specified length data from one memory to another
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      SrcAddrPtr: The source memory address
 *                     Length: The length to be copied
 * Param-Name[out]     DstAddrPtr: The destination memory address
 * Param-Name[in/out]  None
 * Return              E_OK: This operation has processed successfully
 *                     E_NOT_OK: This operation has error, any parameter illegal
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_MemCpy(const uint8 * SrcAddrPtr, uint8 * DstAddrPtr, uint32 Length)
{
    uint32 l_LengthLoop = 0;
    Std_ReturnType l_ReturnValue = E_NOT_OK;
#ifdef SELF_DEBUG
    return (E_OK);
#endif

    if ((NULL_PTR != SrcAddrPtr) && (NULL_PTR != DstAddrPtr) && (0U != Length))
    {
        /* Parameter is ok, copy data*/
        for (; l_LengthLoop < Length; l_LengthLoop++)
        {
            *(DstAddrPtr + l_LengthLoop) = *(SrcAddrPtr + l_LengthLoop);
        }
        l_ReturnValue = E_OK;
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Send a atom job request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      AtomJobReqId: Atom job ID
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void  NvM_AtomJobReq(NvM_AtomJobEType AtomJobReqId)
{
#ifdef SELF_DEBUG
    printf("Send atom Req: %d\r\n", AtomJobReqId);
#endif
    NvM_AtomJob.Count++;
    NvM_AtomJob.ReqId[NvM_AtomJob.Count] = AtomJobReqId;
    return;
}

/*************************************************************************/
/*
 * Brief               Detect module runtime type
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void  NvM_RunTimeHandle(void)
{
    if ((NVM_REQ_NOT_OK == NvM_AdminBlock[NVM_READALL_FIRST_BLOCKID].SingleReqResult)
            || (NVM_COMPILED_CONFIG_ID != *(uint16 *)(NvM_BlockDescriptor[NVM_READALL_FIRST_BLOCKID].NvmRamBlockDataAddress)))
    {
#if (STD_OFF == NVM_DYNAMIC_CONFIGURATION)
        NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
#else
        /* the runtime need more information to charge*/
        NvM_Module.RuntimeType = NVM_RUNTIME_NOT_SURE;
        NvM_MultiJob.ID1WriteMark = STD_ON; /*need check whether set this flag at here*/
#endif
    }
    else
    {
        NvM_Module.RuntimeType = NVM_RUNTIME_NORMAL;
    }
    return;
}

/*************************************************************************/
/*
 * Brief               Copy current block infomation to global variant
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void  NvM_CopyCurBlockInfo(void)
{
    NvM_CurRunning.ManagementType = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmBlockManagementType;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType)
    {
        NvM_AdminBlock[NvM_CurRunning.BlockId].CurrentIndex = 0;
    }
#else
    NvM_AdminBlock[NvM_CurRunning.BlockId].CurrentIndex = 0;
#endif
    NvM_CurRunning.Index = NvM_AdminBlock[NvM_CurRunning.BlockId].CurrentIndex;
    NvM_CurRunning.AdminFlagGroup = NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup;
    NvM_CurRunning.Crc = NvM_AdminBlock[NvM_CurRunning.BlockId].Crc;
    NvM_CurRunning.ESingleReqResult = NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult;

    NvM_CurRunning.WriteRetryCounter = NVM_MAX_NUM_OF_WRITE_RETRIES;

    NvM_CurRunning.DeviceId = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmNvramDeviceId;
    NvM_CurRunning.NvNum = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmNvBlockNum;
    NvM_CurRunning.RomNum = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRomBlockNum;
    NvM_CurRunning.BDSCFlagGroup = NvM_BlockDescriptor[NvM_CurRunning.BlockId].FlagGroup;
    NvM_CurRunning.CRCType = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmBlockCRCType;
    NvM_CurRunning.BaseNumber = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmNvBlockBaseNumber;
    NvM_CurRunning.Length = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmNvBlockLength;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType)
    {
        NvM_CurRunning.RomAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress - (uint32)(&__NVMSEC_RAMROM_OFFSET); //NvmRomBlockDataAddress
    }
    else if (NvM_CurRunning.NvNum <= NvM_CurRunning.Index)
    {
        NvM_CurRunning.RomAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress - (uint32)(&__NVMSEC_RAMROM_OFFSET) + ((NvM_CurRunning.Index - NvM_CurRunning.NvNum) * NvM_CurRunning.Length);
    }
    else
    {
        /* None will not happen */
    }
#else
    NvM_CurRunning.RomAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress - (uint32)(&__NVMSEC_RAMROM_OFFSET); //NvmRomBlockDataAddress
#endif
#if (NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    NvM_CurRunning.InitCallback = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmInitBlockCallback;
#endif
#if (STD_OFF == NVM_POLLING_MODE)
    NvM_CurRunning.SingleCallback = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmSingleBlockCallback;
#endif
    return;
}

/*************************************************************************/
/*
 * Brief               Multiple type job dispatch
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: There has a job need process
 *                     E_NOT_OK: There is no job need process
 * PreCondition        None
 */
/*************************************************************************/
#if((NVM_API_CONFIG_CLASS_1==NVM_API_CONFIG_CLASS)||(STD_OFF==NVM_JOB_PRIORITIZATION))
static Std_ReturnType  NvM_MultiJobDispatch(void)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

    if (NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId)
    {
        NvM_CurRunning.BlockId++;
        if (NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId)
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /*switch the low layer process mode to slow mode after job over*/
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJob.ReqResult = NVM_REQ_NOT_OK;
            }
            else
            {
                NvM_MultiJob.ReqResult = NVM_REQ_OK;
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;

#if (STD_OFF == NVM_POLLING_MODE)
            if (NULL_PTR != NvmMultiBlockCallback)
            {
                /* call the callback function to notify the up layer after job processing is over */
                (*NvmMultiBlockCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_MultiJob.ReqResult);
            }
#endif
        }
        else
        {
            /* pending the job */
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    else
    {
        NvM_CurRunning.BlockId++;
        if ((NVM_WRITEALL_FIRST_BLOCKID == NvM_CurRunning.BlockId)
                || ((NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId) && (STD_OFF == NvM_MultiJob.ID1WriteMark)))
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /*switch the low layer process mode to slow mode after job over*/
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJob.ReqResult = NVM_REQ_NOT_OK;
            }
            else
            {
                NvM_MultiJob.ReqResult = NVM_REQ_OK;
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;

#if (STD_OFF == NVM_POLLING_MODE)
            if (NULL_PTR != NvmMultiBlockCallback)
            {
                /* call the callback function to notify the up layer after job processing is over */
                (*NvmMultiBlockCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_MultiJob.ReqResult);
            }
#endif
        }
        else if ((NVM_BLOCK_NUM_ALL < NvM_CurRunning.BlockId)
                 && (STD_ON == NvM_MultiJob.ID1WriteMark))
        {
            /* pending the job and clear the write flag */
            NvM_CurRunning.BlockId = 1;
            NvM_MultiJob.ID1WriteMark = STD_OFF;
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            *(uint16*)NvM_CurRunning.RamAddr = NVM_COMPILED_CONFIG_ID;
            NvM_SetByteBitState(&NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
        else
        {
            /* pending the job */
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Put a job to the job queue to be processing
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The job's block ID
 *                     ServiceId: The job's service ID
 *                     DestSrcPtr: Pointer to the temperary buffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The job enqueue success
 *                     E_NOT_OK: The job queue is full or some error happened
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8 * DestSrcPtr)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    uint8 l_Index = 0;

    //EnterCriticalCode();
	EnterCritical();
    if (NVM_SIZE_STANDARD_JOB_QUEUE > NvM_StandQueueManage.Count)
    {
        l_Index = NvM_StandQueueManage.TailIndex;
        NvM_StandQueue[l_Index].BlockId = BlockId;
        NvM_StandQueue[l_Index].ServiceId = ServiceId;
        NvM_StandQueue[l_Index].DestSrcPtr = DestSrcPtr;
        NvM_StandQueueManage.TailIndex++;
        /* protect the tailindex is overflow, when this happened, reset it*/
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= NvM_StandQueueManage.TailIndex)
        {
            NvM_StandQueueManage.TailIndex = 0;
        }
        NvM_StandQueueManage.Count++;
        if ((NVM_SERV_ID_READ_ALL == ServiceId)
                || (NVM_SERV_ID_WRITE_ALL == ServiceId))
        {
            /* set multiple type job flag */
            NvM_MultiJob.Enqueue = STD_ON;
            NvM_MultiJob.ReqResult = NVM_REQ_PENDING;
        }
        else
        {
            /* set single type job flag */
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
            NvM_AdminBlock[BlockId].SingleReqResult = NVM_REQ_PENDING;
        }
        l_ReturnValue = E_OK;
    }
	ExitCritical();
#else
    /* the api class is class 1, put job to global variant directly, no need queue */
   // EnterCriticalCode();
    EnterCritical();
    NvM_CurRunning.BlockId = BlockId;
    NvM_MultiJob.Enqueue = STD_ON;
    NvM_MultiJob.ReqResult = NVM_REQ_PENDING;
    NvM_CurRunning.ServiceId = ServiceId;
    NvM_CurRunning.RamAddr = DestSrcPtr;
    l_ReturnValue = E_OK;
	ExitCritical();
#endif
    //ExitCriticalCode();
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Get a job from the job queue to process
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Get job successful
 *                     E_NOT_OK: Get job error or the queue is empty
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_JobDequeue(void)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    uint8 l_Index = 0;

    if (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
    {
        /* multiple type job dispatch */
        l_ReturnValue = NvM_MultiJobDispatch();
    }
    else if (0 < NvM_StandQueueManage.Count)
    {
        //EnterCriticalCode();
        EnterCritical();
        /* get the queue current head index */
        l_Index = NvM_StandQueueManage.HeadIndex;
        NvM_CurRunning.ServiceId = NvM_StandQueue[l_Index].ServiceId;
        NvM_MultiJob.ServiceId = NvM_CurRunning.ServiceId;
        if ((NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId)
                || (NVM_SERV_ID_WRITE_ALL == NvM_CurRunning.ServiceId))
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            if (NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId)
            {
                NvM_CurRunning.BlockId = NVM_READALL_FIRST_BLOCKID;
            }
            else
            {
                NvM_CurRunning.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
            }
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /* switch the low layer process mode to fast mode before multiple type job start */
            MemIf_SetMode(MEMIF_MODE_FAST);
#endif
        }
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
            NvM_CurRunning.BlockId = NvM_StandQueue[l_Index].BlockId;
            if (NULL_PTR != NvM_StandQueue[l_Index].DestSrcPtr)
            {
                /* the parameter is null, using the default configered ram address */
                NvM_CurRunning.RamAddr = NvM_StandQueue[l_Index].DestSrcPtr;
            }
            else
            {
                /* there is a temperary parameter in, so the ram address will keep up with it */
                NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            }
        }
        NvM_CopyCurBlockInfo();
        /* set the queue management flag */
        NvM_StandQueueManage.HeadIndex++;
        if (NVM_SIZE_STANDARD_JOB_QUEUE <= NvM_StandQueueManage.HeadIndex)
        {
            /* prevent the index overflow, when it happen, need reset it */
            NvM_StandQueueManage.HeadIndex = 0;
        }
        NvM_StandQueueManage.Count--;
        //ExitCriticalCode();
        ExitCritical();
        l_ReturnValue = E_OK;
    }
    else
    {
        /* the job queue is empty */
        l_ReturnValue = E_NOT_OK;
    }
#else
    if (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
    {
        /* multiply type job dispatch */
        l_ReturnValue = NvM_MultiJobDispatch();
    }
    else if (STD_ON == NvM_MultiJob.Enqueue)
    {
        EnterCriticalCode();
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
        if (NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId)
        {
            NvM_CurRunning.BlockId = NVM_READALL_FIRST_BLOCKID;
        }
        else
        {
            NvM_CurRunning.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
        }
        /* multiple type job did not have temprary parameter, so using default ram address*/
        NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
        NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
        ExitCriticalCode();
#if (STD_ON == NVM_DRV_MODE_SWITCH)
        MemIf_SetMode(MEMIF_MODE_FAST);
#endif
        /* copy current block information to global variant to process */
        NvM_CopyCurBlockInfo();
        l_ReturnValue = E_OK;
    }
    else
    {
        /* there is no job need process or the flag set error happend */
        l_ReturnValue = E_NOT_OK;
    }
#endif
    return (l_ReturnValue);
}

#endif

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
/*************************************************************************/
/*
 * Brief               Find the first setted bit of a given 16 bits source number
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      Number: The source number
 * Param-Name[out]     SettedBitPtr: Which bit is the lowest setted bit
 * Param-Name[in/out]  None
 * Return              E_OK: The first setted bit is fond, see the output parameter
 *                     E_NOT_OK: Did not find that bit, the source number equal 0
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_Get16Bits1stSettedBit(uint16 Number, uint8 * SettedBitPtr)
{
    uint8 l_BitsLoop = 0x00u;
    uint8 l_BaseBit = 0x00u;
    uint16 l_BaseNum = 0x0000u;
    Std_ReturnType l_ReturnValue = E_OK;

    if (0x0000 == Number)
    {
        /* the number equal 0, can not find the setted bit */
        l_ReturnValue = E_NOT_OK;
    }
    else if (0x100 > Number)
    {
        /* the first setted bit between 0 and 3 */
        if (0x10 > Number)
        {
            l_BaseNum = 0x01u;
            l_BaseBit = 0u;
        }
        /* the first setted bit between 4 and 7 */
        else
        {
            l_BaseNum = 0x10u;
            l_BaseBit = 4u;
        }
    }
    else
    {
        /* the first setted bit between 8 and 11 */
        if (0x1000 > Number)
        {
            l_BaseNum = 0x100u;
            l_BaseBit = 8u;
        }
        /* the first setted bit between 12 and 15 */
        else
        {
            l_BaseNum = 0x1000u;
            l_BaseBit = 12u;
        }
    }
    if (E_OK == l_ReturnValue)
    {
        do
        {
            if (0 != (l_BaseNum & Number))
            {
                *SettedBitPtr = l_BaseBit + l_BitsLoop;
                break;
            }
            l_BaseNum = l_BaseNum << 1;
            l_BitsLoop++;
        }
        while (4 > l_BitsLoop);
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Get the available job queue space
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      JobType: The job type
 * Param-Name[out]     JobQueueIndexPtr: Pointer to the space which store the job index
 * Param-Name[in/out]  None
 * Return              E_OK: The valid space is fond
 *                     E_NOT_OK: The queue is full
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_GetQueueSpace(NvM_JobTypeType JobType, uint8 * JobQueueIndexPtr)
{
    uint8 l_JobQueueGroup = 0x00u;
    uint8 l_JobQueueRdyBit = 0x00u;
    uint8 l_JobQueueIndex = 0xFFu;
    Std_ReturnType l_ReturnValue = E_OK;

    if (NVM_JOB_TYPE_SINGLE_IMMED == JobType)
    {
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.Count)
        {
            /* Job queue overflow */
            l_ReturnValue = E_NOT_OK;
        }
        else
        {
            EnterCriticalCode();
            *JobQueueIndexPtr = NvM_ImmedQueueManage.TailIndex;
            NvM_ImmedQueueManage.TailIndex++;
            if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.TailIndex)
            {
                /* prevent index overflow, reset it */
                NvM_ImmedQueueManage.TailIndex = 0;
            }
            NvM_ImmedQueueManage.Count++;
            ExitCriticalCode();
        }
    }
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1], &l_JobQueueGroup);
        if ((E_NOT_OK == l_ReturnValue) || (((NVM_SIZE_STANDARD_JOB_QUEUE - 1) >> 4) < l_JobQueueGroup))
        {
            if (E_NOT_OK != l_ReturnValue)
            {
                /* Initial error happened, Correct the mistakes */
                NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] &= (uint16)(~(0x01u << l_JobQueueGroup));
                l_ReturnValue = E_NOT_OK;
            }
            /* Job queue overflow */
        }
        else
        {
            l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_QueueSpaceTalbe[l_JobQueueGroup], &l_JobQueueRdyBit);
            l_JobQueueIndex = (l_JobQueueGroup << 4) + l_JobQueueRdyBit;
            if ((E_NOT_OK == l_ReturnValue) || (NVM_SIZE_STANDARD_JOB_QUEUE <= l_JobQueueIndex))
            {
                /* Flag error happened, Correct the mistakes */
                if (E_NOT_OK == l_ReturnValue)
                {
                    NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] &= (uint16)(~(0x01u << l_JobQueueGroup));
                }
                else
                {
                    /* Initial error happened, Correct the mistakes */
                    l_ReturnValue = E_NOT_OK;
                }
            }
            else
            {
                EnterCriticalCode();
                *JobQueueIndexPtr = l_JobQueueIndex;
                /* clear the job queue space table, mark it as not available */
                NvM_QueueSpaceTalbe[l_JobQueueGroup] &= (uint16)(~(0x01u << l_JobQueueRdyBit));
                if (0 == NvM_QueueSpaceTalbe[l_JobQueueGroup])
                {
                    /* the group's space are all in using, clear the group flag */
                    NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] &= (uint16)(~(0x01u << l_JobQueueGroup));
                }
                ExitCriticalCode();
            }
        }
    }
    else
    {
        /* None will not happen */
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Release the space which indicated by the index
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      JobType: The job type
 * Param-Name[out]     JobQueueIndex: The job queue index which to be release
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_RelQueueSpace(NvM_JobTypeType JobType, uint8 JobQueueIndex)
{
    uint8 l_QueueLowBits = 0xFFu;
    uint8 l_QueueHighBits = 0xFFu;

    EnterCriticalCode();
    if ((NVM_JOB_TYPE_SINGLE_IMMED == JobType) || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == JobType))
    {
        NvM_ImmedQueueManage.HeadIndex++;
        if (NVM_SIZE_IMMEDIATE_JOB_QUEUE <= NvM_ImmedQueueManage.HeadIndex)
        {
            /* round robin queue, prvent index overflow, reset it */
            NvM_ImmedQueueManage.HeadIndex = 0;
        }
        NvM_ImmedQueueManage.Count--;
    }
    else if (NVM_JOB_TYPE_SINGLE_NORMAL == JobType)
    {
        /* set job queue table flag */
        l_QueueLowBits = JobQueueIndex & 0X0FU;
        l_QueueHighBits = (uint8)((JobQueueIndex & 0XF0U) >> 4U);
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] |= (uint16)(0X01U << l_QueueHighBits);
        NvM_QueueSpaceTalbe[l_QueueLowBits] |= (uint16)(0X01U << l_QueueLowBits);
    }
    else
    {
        /* None Will not happen */
    }
    ExitCriticalCode();
    return;
}

/*************************************************************************/
/*
 * Brief               Get the highest priority job
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     JobQueueIndexPtr: Pointer to the space which store the job index
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_GetHighestPriJob(uint8 * JobQueueIndexPtr)
{
    uint8 l_PriGroup = 0xFFu;
    uint8 l_PriRdyBit = 0xFFu;
    uint8 l_PriHighest = 0xFFu;
    uint8 l_CurActPriTable = 0;
    uint8 l_SamePriHeadIndex = 0;
    Std_ReturnType l_ReturnValue = E_OK;

    l_CurActPriTable = NvM_Module.CurActPriTable;
    l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1], &l_PriGroup);
    if (E_OK == l_ReturnValue)
    {
        if (NVM_TABLE_SIZE_PRIORITY < (l_PriGroup + 2))
        {
            /* Initial error happened, Correct the mistakes */
            NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1] &= (uint16)(~(0x01u << l_PriGroup));
            l_ReturnValue = E_NOT_OK;
        }
        else
        {
            l_ReturnValue = NvM_Get16Bits1stSettedBit(NvM_PriorityTable[l_CurActPriTable][l_PriGroup], &l_PriRdyBit);
            if (E_NOT_OK == l_ReturnValue)
            {
                /* Flag error happened, Correct the mistakes */
                NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1] &= (uint16)(~(0x01u << l_PriGroup));
            }
            else
            {
                l_PriHighest = (l_PriGroup << 4) + l_PriRdyBit;
                if (NVM_TOTAL_NUM_DIFF_PRI <= l_PriHighest)
                {
                    /* Flag error happened, Correct the mistakes */
                    NvM_PriorityTable[l_CurActPriTable][l_PriGroup] &= (uint16)(~(0x01u << l_PriRdyBit));
                    l_ReturnValue = E_NOT_OK;
                }
                else
                {
                    l_SamePriHeadIndex = NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex;
                    EnterCriticalCode();
                    *JobQueueIndexPtr = l_SamePriHeadIndex;
                    if (NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex == NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].TailIndex)
                    {
                        /* This is the last element, clear priority table flag */
                        NvM_PriorityTable[l_CurActPriTable][l_PriGroup] &= (uint16)(~(0x01u << l_PriRdyBit));
                        if (0 == NvM_PriorityTable[l_CurActPriTable][l_PriGroup])
                        {
                            NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1] &= (uint16)(~(0x01u << l_PriGroup));
                        }
                    }
                    else
                    {
                        NvM_PriTable2Queue[l_CurActPriTable][l_PriHighest].HeadIndex = NvM_StandQueue[l_SamePriHeadIndex].NextIndex;
                        NvM_StandQueue[l_SamePriHeadIndex].NextIndex = l_SamePriHeadIndex;
                    }
                    ExitCriticalCode();
                }
            }
        }
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Multiple type job dispatch
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: There has a job need process
 *                     E_NOT_OK: There is no job need process
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_MultiJobDispatch(void)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

    if (NVM_SERV_ID_READ_ALL == NvM_MultiJob.ServiceId)
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != NvM_Module.CurrentJobType) || (1 != NvM_MultiJob.BlockId))
        {
            NvM_MultiJob.BlockId++;
        }
        if (NVM_BLOCK_NUM_ALL < NvM_MultiJob.BlockId)
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /* switch low layer mode to slow mode after multiple type job is processed over */
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            if (0 != NvM_PriorityTable[1 - NvM_Module.CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1])
            {
                /* switch the current active priority table */
                NvM_Module.CurActPriTable = 1 - NvM_Module.CurActPriTable;
            }
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJob.ReqResult = NVM_REQ_NOT_OK;
            }
            else
            {
                NvM_MultiJob.ReqResult = NVM_REQ_OK;
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;

#if (STD_OFF == NVM_POLLING_MODE)
            if (NULL_PTR != NvmMultiBlockCallback)
            {
                /* if the callback func is registered and the switch is open, call it */
                (*NvmMultiBlockCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_MultiJob.ReqResult);
            }
#endif
        }
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
            NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    else
    {
        NvM_MultiJob.BlockId++;
        if ((2 == NvM_MultiJob.BlockId)
                || ((NVM_BLOCK_NUM_ALL < NvM_MultiJob.BlockId) && (STD_OFF == NvM_MultiJob.ID1WriteMark)))
        {
#if (STD_ON == NVM_DRV_MODE_SWITCH)
            /* switch low layer mode to slow mode after multiple type job is processed over */
            MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
            if (0 != NvM_PriorityTable[1 - NvM_Module.CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1])
            {
                /* switch the current active priority table */
                NvM_Module.CurActPriTable = 1 - NvM_Module.CurActPriTable;
            }
            NvM_MultiJob.Enqueue = STD_OFF;
            if (NVM_REQ_NOT_OK == NvM_CurRunning.EMultiReqResult)
            {
                NvM_MultiJob.ReqResult = NVM_REQ_NOT_OK;
            }
            else
            {
                NvM_MultiJob.ReqResult = NVM_REQ_OK;
            }
            NvM_CurRunning.EMultiReqResult = NVM_REQ_OK;
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;

#if (STD_OFF == NVM_POLLING_MODE)
            if (NULL_PTR != NvmMultiBlockCallback)
            {
                /* if the callback func is registered and the switch is open, call it */
                (*NvmMultiBlockCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_MultiJob.ReqResult);
            }
#endif
        }
        else if (NVM_BLOCK_NUM_ALL >= NvM_MultiJob.BlockId)
        {
            /* copy the current job information to global variant to process */
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
            NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
            NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
        else
        {
            /* copy the current job information to global variant to process */
            NvM_MultiJob.BlockId = 1;
            NvM_MultiJob.ID1WriteMark = STD_OFF;
            NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
            NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
            NvM_CopyCurBlockInfo();
            l_ReturnValue = E_OK;
        }
    }
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Put a job to the job queue to be processing
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      BlockId: The job's block ID
 *                     ServiceId: The job's service ID
 *                     DestSrcPtr: Pointer to the temperary buffer
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: The job enqueue success
 *                     E_NOT_OK: The job queue is full or some error happened
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_JobEnqueue(NvM_BlockIdType BlockId, NvM_ServiceIdType ServiceId, uint8 * DestSrcPtr)
{
    uint8 l_PriLowBits;
    uint8 l_PriHighBits;
    uint8 l_CurActPriTable = 0U;
    uint8 l_JobPri = 0xFFu;
    uint8 l_AddJobQueueIndex = 0U;
    NvM_JobTypeType l_JobType = NVM_JOB_TYPE_NONE;
    Std_ReturnType l_ReturnValue = E_OK;

    EnterCriticalCode();
    if ((NVM_SERV_ID_READ_ALL == ServiceId)
            || (NVM_SERV_ID_WRITE_ALL == ServiceId))
    {
        NvM_MultiJob.Enqueue = STD_ON;
        NvM_MultiJob.ServiceId = ServiceId;
        NvM_MultiJob.ReqResult = NVM_REQ_PENDING;
    }
    else if ((NVM_BLOCK_NUM_ALL >= BlockId) && (0 < BlockId)
             && (STD_OFF == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE)))
    {
        l_JobPri = NvM_BlockDescriptor[BlockId].NvmBlockJobPriority;
        if ((0 == l_JobPri) && (NVM_SERV_ID_WRITE_BLOCK == ServiceId))
        {
            l_JobType = NVM_JOB_TYPE_SINGLE_IMMED;
        }
        else
        {
            l_JobType = NVM_JOB_TYPE_SINGLE_NORMAL;
        }
        if (E_NOT_OK == NvM_GetQueueSpace(l_JobType, &l_AddJobQueueIndex))
        {
            /* Job queue overflow */
            l_ReturnValue = E_NOT_OK;
        }
        else
        {
            if (NVM_JOB_TYPE_SINGLE_IMMED == l_JobType)
            {
                /* store the job information to queue and set the job state as pending */
                NvM_ImmedQueue[l_AddJobQueueIndex].BlockId = BlockId;
                NvM_ImmedQueue[l_AddJobQueueIndex].DestSrcPtr = DestSrcPtr;
                NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
                NvM_AdminBlock[BlockId].SingleReqResult = NVM_REQ_PENDING;
            }
            else if (NVM_JOB_TYPE_SINGLE_NORMAL == l_JobType)
            {
                l_PriLowBits = l_JobPri & 0X0FU;
                l_PriHighBits = (uint8)((l_JobPri & 0XF0U) >> 4);
                if (STD_ON == NvM_MultiJob.Enqueue)
                {
                    /* if there is a multiple type job in queue, need change the active priotity table */
                    l_CurActPriTable = 1 - NvM_Module.CurActPriTable;
                }
                else
                {
                    l_CurActPriTable = NvM_Module.CurActPriTable;
                }
                /* set the priority table group flag */
                NvM_PriorityTable[l_CurActPriTable][NVM_TABLE_SIZE_PRIORITY - 1] |= (uint16)(0x01u << l_PriHighBits);
                if (0 != (NvM_PriorityTable[l_CurActPriTable][l_PriHighBits] & (0X01u << l_PriLowBits)))
                {
                    /* add the new job to the queue's tail */
                    NvM_StandQueue[NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].TailIndex].NextIndex = l_AddJobQueueIndex;
                }
                else
                {
                    /* set the priority table's corresponding bit */
                    NvM_PriorityTable[l_CurActPriTable][l_PriHighBits] |= (uint16)(0X01u << l_PriLowBits);
                    NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].HeadIndex = l_AddJobQueueIndex;
                }
                /* store the new job information to queue and set the block status as pending */
                NvM_PriTable2Queue[l_CurActPriTable][l_JobPri].TailIndex = l_AddJobQueueIndex;
                NvM_StandQueue[l_AddJobQueueIndex].BlockId = BlockId;
                NvM_StandQueue[l_AddJobQueueIndex].ServiceId = ServiceId;
                NvM_StandQueue[l_AddJobQueueIndex].NextIndex = l_AddJobQueueIndex;
                NvM_StandQueue[l_AddJobQueueIndex].DestSrcPtr = DestSrcPtr;
                NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE, STD_ON);
                NvM_AdminBlock[BlockId].SingleReqResult = NVM_REQ_PENDING;
            }
            else
            {
                /* None will not happen*/
            }
        }
    }
    else
    {
        /* Invalid BlockId or Job has already enqueued */
        l_ReturnValue = E_NOT_OK;
    }
    ExitCriticalCode();
    return (l_ReturnValue);
}

/*************************************************************************/
/*
 * Brief               Get a job from the job queue to process
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK: Get job successful
 *                     E_NOT_OK: Get job error or the queue is empty
 * PreCondition        None
 */
/*************************************************************************/
static Std_ReturnType NvM_JobDequeue(void)
{
    uint8 l_JobQueueIndex;
    Std_ReturnType l_ReturnValue = E_OK;

    if (NVM_QUEUE_EMPTY_COUNT != NvM_ImmedQueueManage.Count)
    {
        EnterCriticalCode();
        if ((NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType) || (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType))
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED;
            //NvM_MultiJob.BlockId = NvM_CurRunning.BlockId;
            //NvM_MultiJob.ServiceId = NvM_CurRunning.ServiceId;
        }
        else
        {
            NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_IMMED;
        }
        NvM_CurRunning.BlockId = NvM_ImmedQueue[NvM_ImmedQueueManage.HeadIndex].BlockId;
        NvM_CurRunning.ServiceId = NVM_SERV_ID_WRITE_BLOCK;
        if (NULL_PTR != NvM_ImmedQueue[NvM_ImmedQueueManage.HeadIndex].DestSrcPtr)
        {
            NvM_CurRunning.RamAddr = NvM_ImmedQueue[NvM_ImmedQueueManage.HeadIndex].DestSrcPtr;
        }
        else
        {
            NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
        }
        ExitCriticalCode();
        NvM_RelQueueSpace(NvM_Module.CurrentJobType, NvM_ImmedQueueManage.HeadIndex);
        NvM_CopyCurBlockInfo();
    }
    else
    {
        if ((NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType)
                || (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType))
        {
            if (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED == NvM_Module.CurrentJobType)
            {
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
                NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
                NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
            }
            /* switch low layer mode to slow mode after multiple type job is processed over */
            l_ReturnValue = NvM_MultiJobDispatch();
        }
        else
        {
            /* get the highest priority from the priority table */
            l_ReturnValue = NvM_GetHighestPriJob(&l_JobQueueIndex);
            EnterCriticalCode();
            if (E_OK == l_ReturnValue)
            {
                /* transfer the job need processing to global variant */
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_SINGLE_NORMAL;
                NvM_CurRunning.BlockId = NvM_StandQueue[l_JobQueueIndex].BlockId;
                NvM_CurRunning.ServiceId = NvM_StandQueue[l_JobQueueIndex].ServiceId;
                if (NULL_PTR != NvM_StandQueue[l_JobQueueIndex].DestSrcPtr)
                {
                    NvM_CurRunning.RamAddr = NvM_StandQueue[l_JobQueueIndex].DestSrcPtr;
                }
                else
                {
                    NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
                }
                /* after transfer the job to global variant, need release the space */
                NvM_RelQueueSpace(NvM_Module.CurrentJobType, l_JobQueueIndex);
                NvM_CopyCurBlockInfo();
            }
            else if (STD_ON == NvM_MultiJob.Enqueue)
            {
                NvM_Module.CurrentJobType = NVM_JOB_TYPE_MULTI;
                NvM_CurRunning.ServiceId = NvM_MultiJob.ServiceId;
                if (NVM_SERV_ID_READ_ALL == NvM_MultiJob.ServiceId)
                {
                    NvM_MultiJob.BlockId = NVM_READALL_FIRST_BLOCKID;
                }
                else
                {
                    NvM_MultiJob.BlockId = NVM_WRITEALL_FIRST_BLOCKID;
                }
                NvM_CurRunning.BlockId = NvM_MultiJob.BlockId;
                NvM_CurRunning.RamAddr = NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress;
                NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_PENDING;
                NvM_CopyCurBlockInfo();
#if (STD_ON == NVM_DRV_MODE_SWITCH)
                /* switch the low layer process mode to fast mode before multiple type job start */
                MemIf_SetMode(MEMIF_MODE_FAST);
#endif
                l_ReturnValue = E_OK;
            }
            else
            {
                l_ReturnValue = E_NOT_OK;
            }
            ExitCriticalCode();
        }
    }
    return (l_ReturnValue);
}
#endif

/*************************************************************************/
/*
 * Brief               Set the flag after job process over
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      SingleReqResult: Job process result
 *                     CrcUpdate: Update the crc field or not
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_SetJobOverFlag(NvM_RequestResultType SingleReqResult, uint8 CrcUpdate)
{
#ifdef SELF_DEBUG
    printf("Job over: %d\r\n", SingleReqResult);
#endif
    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
    NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup = NvM_CurRunning.AdminFlagGroup;
    if (STD_ON == CrcUpdate)
    {
        NvM_AdminBlock[NvM_CurRunning.BlockId].Crc = NvM_CurRunning.Crc;
    }
    NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = SingleReqResult;
#if (STD_OFF == NVM_POLLING_MODE)
    if (NULL_PTR != NvM_CurRunning.SingleCallback)
    {
        (*NvM_CurRunning.SingleCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult);
    }
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
    NvM_AtomJob.Count = 0;
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer process the read job success
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfReadOk(void)
{
#ifdef SELF_DEBUG
    printf("memif return: read ok\r\n");
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_BLOCKUSECRC))
    {
        NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
        if (NVM_JOB_STEP_READ_1ST_NV == NvM_Module.JobStep)
        {
            NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC_READ_1ST_NV;
        }
        else
        {
            NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC_READ_2ND_NV;
        }
        NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
    }
    else
    {
        if (E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr, NvM_CurRunning.Length))
        {
            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = (uint16)((NvM_NvDataBuffer[NvM_CurRunning.Length]) | (NvM_NvDataBuffer[NvM_CurRunning.Length + 1] << 8));

            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = (uint32)((NvM_NvDataBuffer[NvM_CurRunning.Length]) | (NvM_NvDataBuffer[NvM_CurRunning.Length + 1] << 8) |
                                              (NvM_NvDataBuffer[NvM_CurRunning.Length + 2] << 16)   | (NvM_NvDataBuffer[NvM_CurRunning.Length + 3] << 24));
            }
            else
            {
                NvM_CurRunning.Crc = NvM_NvDataBuffer[NvM_CurRunning.Length];
            }
            if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
            {
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
            }
            NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
        }
        else
        {
            NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
        }
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer process the read job failed
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfReadFailed(void)
{
#ifdef SELF_DEBUG
    printf("memif return: read failed\n");
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        /* @req NVM305 NVM359 NVM361*/
#if (STD_ON == NVM_DEM_ERROR_DETECT)
        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
    }
    else if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
             || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#else
    if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
            || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#endif
    {
        /* @req NVM305 NVM359 NVM361*/
        NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_NOT_OK;
#if (STD_ON == NVM_DEM_ERROR_DETECT)

        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else
    {
        /* @req NVM199 */
#ifdef SELF_DEBUG
        printf("ready to read 2nd block\n");
#endif
        NVM_SET_MEMIF_JOB_READY(READ_2ND);
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer is processing
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfPending(void)
{
    /*do nothing, just waiting*/
    /*do not remove the function*/
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer job is cancelled
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfCancelled(void)
{
#ifdef SELF_DEBUG
    printf("memif return: cancelled\n");
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
    {
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF);
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
    }
    NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer block is inconsistent
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfInconsistent(void)
{
#ifdef SELF_DEBUG
    printf("memif return: inconsistent\n");
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        /* @req NVM358*/
#if (STD_ON == NVM_DEM_ERROR_DETECT)

        Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_SetJobOverFlag(NVM_REQ_INTEGRITY_FAILED, STD_OFF);
    }
    else if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
             || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#else
    if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
            || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#endif
    {
        /* @req NVM358*/
        NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (STD_ON == NVM_DEM_ERROR_DETECT)

        Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else
    {
#ifdef SELF_DEBUG
        printf("ready to read 2nd block\n");
#endif
        NVM_SET_MEMIF_JOB_READY(READ_2ND);
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer block is invalid
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfInvalid(void)
{
#ifdef SELF_DEBUG
    printf("memif return: invalid\n");
#endif
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
    {
        if ((NVM_SERV_ID_READ_BLOCK == NvM_CurRunning.ServiceId) || (NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId))
        {
            if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_WRITEBLOCKONCE))
            {
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO, STD_ON);
            }
        }
        NvM_SetJobOverFlag(NVM_REQ_NV_INVALIDATED, STD_OFF);
    }
    else if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
             || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#else
    if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
            || (NVM_JOB_STEP_READ_2ND_NV == NvM_Module.JobStep))
#endif
    {
        if ((NVM_SERV_ID_READ_BLOCK == NvM_CurRunning.ServiceId) || (NVM_SERV_ID_READ_ALL == NvM_CurRunning.ServiceId))
        {
            if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_WRITEBLOCKONCE))
            {
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO, STD_ON);
            }
        }
        NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_NV_INVALIDATED;
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    else
    {
        NVM_SET_MEMIF_JOB_READY(READ_2ND);
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer process the write job success
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfWriteOk(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
    {
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
    }
    if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_WRITEBLOCKONCE))
    {
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO, STD_ON);
    }
    if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
            && (NVM_JOB_STEP_WRITE_1ST_NV == NvM_Module.JobStep))
    {
        NvM_CurRunning.ESingleReqResult = NVM_REQ_OK;
        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_OFF);
        NVM_SET_MEMIF_JOB_READY(WRITE_2ND);
    }
    else
    {
        NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The low layer process the write job failed
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_MemIfWriteFailed(void)
{
    NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
    if (0U < NvM_CurRunning.WriteRetryCounter)
    {
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_ASYNC_READY;
    }
    else if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType)
             && (NVM_JOB_STEP_WRITE_1ST_NV == NvM_Module.JobStep))
    {
        NvM_CurRunning.WriteRetryCounter = NVM_MAX_NUM_OF_WRITE_RETRIES;
        NVM_SET_MEMIF_JOB_READY(WRITE_2ND);
    }
    else
    {
        if ((NVM_BLOCK_REDUNDANT == NvM_CurRunning.ManagementType) && (NVM_REQ_OK == NvM_CurRunning.ESingleReqResult))
        {
            NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
        }
        else
        {
            NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_ON);
            /* @req NVM213 NVM296*/
#if (STD_ON == NVM_DEM_ERROR_DETECT)
            Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        }
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The module internal function to process read rom request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_ReadRom(void)
{
#ifdef SELF_DEBUG
    printf("Enter NvM_Inner_ReadRom Func\n");
#endif
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
        {
            NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
            NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
        }
        NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
    }
    else
    {
        if (E_OK == NvM_MemCpy(NvM_CurRunning.RomAddr, NvM_CurRunning.RamAddr, NvM_CurRunning.Length))
        {
            if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_CALCRAMBLOCKCRC))
            {
                NvM_CurRunning.CrcAddr = NvM_CurRunning.RamAddr;
                //NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                //NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
                if (NVM_READALL_FIRST_BLOCKID == NvM_CurRunning.BlockId)
                {
                    NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_ON);
                }
                else
                {
                   // NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
                   NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_ON);
                }
            }
            else
            {
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
                }
               // NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
               NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_ON);
            }
        }
        else
        {
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
            if ((NVM_SERV_ID_RESTORE_BLOCK == NvM_CurRunning.ServiceId)
                    && (NULL_PTR != NvM_CurRunning.InitCallback))
            {
                (*NvM_CurRunning.InitCallback)();
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
                }
                NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
            }
            else
#endif
            {
                NvM_SetJobOverFlag(NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult, STD_OFF);
            }
        }
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The module internal function to calculate crc
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_CalcCrc(void)
{
    uint16 l_CrcLength = 0U;
#ifdef SELF_DEBUG
    printf("Enter NvM_Inner_CalcCrc Func\n");
#endif

    if (0U == NvM_CurRunning.CrcFlag)
    {
        NvM_CurRunning.CrcFlag = 1U;
        NvM_CurRunning.CrcLength = NvM_CurRunning.Length;
        if (NVM_CRC_NUM_OF_BYTES < NvM_CurRunning.CrcLength)
        {
            l_CrcLength = NVM_CRC_NUM_OF_BYTES;

            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_START, 0);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_START, 0);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
            else
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_START, 0);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
        }
        else
        {
            l_CrcLength = NvM_CurRunning.CrcLength;

            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_SINGLE, 0);
            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_SINGLE, 0);
            }
            else
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_SINGLE, 0);
            }
            NvM_CurRunning.CrcFlag = 0U;
            if (0 != NvM_AtomJob.Count)
            {
                NvM_AtomJob.Count--;
            }
        }
    }
    else
    {
        if (NVM_CRC_NUM_OF_BYTES < NvM_CurRunning.CrcLength)
        {
            l_CrcLength = NVM_CRC_NUM_OF_BYTES;

            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_CONTI, NvM_CurRunning.Crc);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_CONTI, NvM_CurRunning.Crc);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
            else
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_CONTI, NvM_CurRunning.Crc);
                NvM_CurRunning.CrcLength -= l_CrcLength;
                NvM_CurRunning.CrcAddr += l_CrcLength;
            }
        }
        else
        {
            l_CrcLength = NvM_CurRunning.CrcLength;

            if (NVM_CRC16 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC16(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_END, NvM_CurRunning.Crc);
            }
            else if (NVM_CRC32 == NvM_CurRunning.CRCType)
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC32(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_END, NvM_CurRunning.Crc);
            }
            else
            {
                NvM_CurRunning.Crc = Crc_CalculateCRC8(NvM_CurRunning.CrcAddr, l_CrcLength, CRC_STEP_MULTI_END, NvM_CurRunning.Crc);
            }
            NvM_CurRunning.CrcFlag = 0U;
            if (0 != NvM_AtomJob.Count)
            {
                NvM_AtomJob.Count--;
            }
        }
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The module internal function to process read block request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_ReadBlock(void)
{
    uint16 l_BlockNum;
    uint16 l_Length;
    MemIf_JobResultType l_ReturnValue = MEMIF_JOB_PENDING;
    uint32 l_TempCrc;
#ifdef SELF_DEBUG
    printf("Enter NvM_Inner_ReadBlock Func\n");
#endif



//step3
    if ((NVM_JOB_STEP_CALC_CRC_READ_1ST_NV == NvM_Module.JobStep) || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
    {
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            l_TempCrc = (uint16)((NvM_NvDataBuffer[NvM_CurRunning.Length]) | (NvM_NvDataBuffer[NvM_CurRunning.Length + 1] << 8));
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            l_TempCrc = (uint32)((NvM_NvDataBuffer[NvM_CurRunning.Length]) | (NvM_NvDataBuffer[NvM_CurRunning.Length + 1] << 8) |
                                 (NvM_NvDataBuffer[NvM_CurRunning.Length + 2] << 16)   | (NvM_NvDataBuffer[NvM_CurRunning.Length + 3] << 24));
        }
        else
        {
            l_TempCrc = NvM_NvDataBuffer[NvM_CurRunning.Length];
        }
        if (NvM_CurRunning.Crc == l_TempCrc)
        {
            if (E_OK == NvM_MemCpy(NvM_NvDataBuffer, NvM_CurRunning.RamAddr, NvM_CurRunning.Length))
            {
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
                }
                if (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR, STD_ON);
                }
                NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
            }
            else
            {
                NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
            }
        }
        else
        {
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
            if (NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
            {
                /* @req NVM203 NVM294*/
#if (STD_ON == NVM_DEM_ERROR_DETECT)

                Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
                NvM_SetJobOverFlag(NVM_REQ_INTEGRITY_FAILED, STD_ON);
            }
            else if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
                     || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
#else
            if ((NVM_BLOCK_NATIVE == NvM_CurRunning.ManagementType)
                    || (NVM_JOB_STEP_CALC_CRC_READ_2ND_NV == NvM_Module.JobStep))
#endif
            {
                /* @req NVM203 NVM294*/
                NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_INTEGRITY_FAILED;
#if (STD_ON == NVM_DEM_ERROR_DETECT)

                Dem_ReportErrorStatus(NVM_E_INTEGRITY_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
                NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                NvM_AtomJobReq(NVM_ATOMJOB_READROM);
            }
            else
            {
#ifdef SELF_DEBUG
                printf("Read 2ND block\n");
#endif
                NVM_SET_MEMIF_JOB_READY(READ_2ND);
            }
        }
    }

    //step2
    NVM_MEMIF_REQ(READ)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
#ifdef SELF_DEBUG
        printf("Wait memif return the result\n");
#endif
        l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        (*NvM_MemIfAsyncFuncTable[l_ReturnValue][NVM_SERV_ID_READ_BLOCK])();
    }

    //step1
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
        if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType) && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
        {
            NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
            NvM_AtomJobReq(NVM_ATOMJOB_READROM);
        }
        else
#endif
        {
#ifdef SELF_DEBUG
            printf("ready to read 1st block\n");
#endif
            NVM_SET_MEMIF_JOB_READY(READ_1ST);
        }
    }

    return;
}

/*************************************************************************/
/*
 * Brief               The module internal function to process write block request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_WriteBlock(void)
{
    uint16 l_BlockNum;
    MemIf_JobResultType l_ReturnValue = MEMIF_JOB_PENDING;

    NVM_MEMIF_REQ(WRITE)
    if (NVM_MEMIF_JOB_ASYNC_PENDING == NvM_Module.MemIfJobState)
    {
        l_ReturnValue = MemIf_GetJobResult(NvM_CurRunning.DeviceId);
        (*NvM_MemIfAsyncFuncTable[l_ReturnValue][NVM_SERV_ID_WRITE_BLOCK])();
    }


    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        if (NVM_CRC16 == NvM_CurRunning.CRCType)
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length]  = (uint8)(NvM_CurRunning.Crc);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 1] = (uint8)(NvM_CurRunning.Crc >> 8);
        }
        else if (NVM_CRC32 == NvM_CurRunning.CRCType)
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length]   = (uint8)(NvM_CurRunning.Crc);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 1] = (uint8)(NvM_CurRunning.Crc >> 8);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 2] = (uint8)(NvM_CurRunning.Crc >> 16);
            NvM_NvDataBuffer[NvM_CurRunning.Length + 3] = (uint8)(NvM_CurRunning.Crc >> 24);
        }
        else
        {
            NvM_NvDataBuffer[NvM_CurRunning.Length] = (uint8)(NvM_CurRunning.Crc);
        }
        NVM_SET_MEMIF_JOB_READY(WRITE_1ST);
    }

    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        if (E_OK == NvM_MemCpy(NvM_CurRunning.RamAddr, NvM_NvDataBuffer, NvM_CurRunning.Length))
        {
            if (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_BLOCKUSECRC))
            {
                NvM_CurRunning.CrcAddr = NvM_NvDataBuffer;
                NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
            }
            else
            {
                NVM_SET_MEMIF_JOB_READY(WRITE_1ST);
            }
        }
        else
        {
            NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
        }
    }



    return;
}

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
/*************************************************************************/
/*
 * Brief               The module internal function to process set ram status request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_SetRamStatus(void)
{
    if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
    {
        NvM_SetJobOverFlag(NVM_REQ_OK, STD_ON);
    }
    else
    {
        NvM_CurRunning.CrcAddr = NvM_CurRunning.RamAddr;
        NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
        NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
    }
    return;
}
#endif

/*************************************************************************/
/*
 * Brief               The module internal function to process restore block default request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_RestoreBlockDefaults(void)
{
    if (NVM_JOB_STEP_PENDING == NvM_Module.JobStep)
    {
        NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
        NvM_AtomJobReq(NVM_ATOMJOB_READROM);
    }
    return;
}
#endif

/*************************************************************************/
/*
 * Brief               The module internal function to process read all block request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_ReadAll(void)
{
    uint8 l_ResistentChangedSw;

    if (NVM_READALL_FIRST_BLOCKID == NvM_CurRunning.BlockId)
    {
        NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
    }
    else if (NVM_READALL_FIRST_BLOCKID < NvM_CurRunning.BlockId)
    {
        if ((NVM_READALL_FIRST_BLOCKID + 1U) == NvM_CurRunning.BlockId)
        {
            NvM_RunTimeHandle();
        }
        if (NVM_JOB_STEP_CALC_CRC == NvM_Module.JobStep)
        {
            if (NvM_CurRunning.Crc != NvM_CurRunning.TempCrc)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
                return;
            }
            else
            {
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
                }
                NvM_SetJobOverFlag(NVM_REQ_OK, STD_OFF);
                return;
            }
        }
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
        if ((STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_SELECTBLOCKFORREADALL))
                && (STD_OFF == NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED))
                && (NVM_BLOCK_DATASET != NvM_CurRunning.ManagementType))
#else
        if ((STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_SELECTBLOCKFORREADALL))
                && (STD_OFF == NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED)))
#endif
        {
            l_ResistentChangedSw = NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_RESISTANTTOCHANGEDSW);
            if ((NVM_RUNTIME_NOT_SURE == NvM_Module.RuntimeType)
                    && (STD_OFF == l_ResistentChangedSw))
            {
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
                }
                NvM_Module.JobStep = NVM_JOB_STEP_READ_ROM;
                NvM_AtomJobReq(NVM_ATOMJOB_READROM);
            }
            else
            {
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
                if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                {
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF);
                    NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
                }
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
#else
                if ((STD_ON == NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID))
                        && (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_CALCRAMBLOCKCRC)))
                {
                    if (NvM_CurRunning.RamAddr == NvM_BlockDescriptor[NvM_CurRunning.BlockId].NvmRamBlockDataAddress)
                    {
                        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF);
                        NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
                    }
                    NvM_CurRunning.TempCrc = NvM_CurRunning.Crc;
                    NvM_Module.JobStep = NVM_JOB_STEP_CALC_CRC;
                    NvM_AtomJobReq(NVM_ATOMJOB_CALCCRC);
                }
                else
                {
                    NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                    NvM_AtomJobReq(NVM_ATOMJOB_READBLOCK);
                }
#endif
            }
        }
        else
        {
            /* @req NVM287*/
            NvM_SetJobOverFlag(NVM_REQ_BLOCK_SKIPPED, STD_OFF);
        }

    }
    else
    {
        /*error block id, do nothing*/
    }
    return;
}

/*************************************************************************/
/*
 * Brief               The module internal function to process write block request
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
static void NvM_Inner_WriteAll(void)
{
    uint8 l_RamValid;
    uint8 l_RamChanged;
    uint8 l_NvProt;
    uint8 l_NvRepair;

    if (STD_ON == NvM_MultiJob.CancelWriteAll)
    {
        while (NVM_BLOCK_NUM_ALL >= NvM_CurRunning.BlockId)
        {
            /* @req NVM287*/
            NvM_SetByteBitState(&NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_ENQUEUE, STD_OFF);
            NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup = NvM_CurRunning.AdminFlagGroup;
            NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult = NVM_REQ_CANCELLED;
            NvM_CurRunning.BlockId++;
#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
            NvM_MultiJob.BlockId = NvM_CurRunning.BlockId;
#endif
        }
        if (STD_ON == NvM_MultiJob.ID1WriteMark)
        {
            NvM_AdminBlock[1].SingleReqResult = NVM_REQ_CANCELLED;
        }
#if (STD_ON == NVM_DRV_MODE_SWITCH)
        MemIf_SetMode(MEMIF_MODE_SLOW);
#endif
        /* @req NVM237*/
#if (STD_ON == NVM_DEM_ERROR_DETECT)

        Dem_ReportErrorStatus(NVM_E_REQ_FAILED, DEM_EVENT_STATUS_FAILED);
#endif
        NvM_MultiJob.CancelWriteAll = STD_OFF;
        NvM_MultiJob.Enqueue = STD_OFF;
        NvM_MultiJob.ReqResult = NVM_REQ_CANCELLED;
#if (STD_OFF == NVM_POLLING_MODE)
        if (NULL_PTR != NvmMultiBlockCallback)
        {
            (*NvmMultiBlockCallback)((uint8)(NvM_CurRunning.ServiceId), NvM_MultiJob.ReqResult);
        }
#endif
        NvM_Module.CurrentJobType = NVM_JOB_TYPE_NONE;
        NvM_Module.MemIfJobState = NVM_MEMIF_JOB_IDLE;
        NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
    }
    else
    {
        l_RamValid = NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_VALID);
        l_RamChanged = NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_RAM_CHANGED);
        l_NvProt = NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_WRPRO);
        l_NvRepair = NvM_GetByteBitState(NvM_CurRunning.AdminFlagGroup, NVM_ADMIN_NV_REPAIR);

        if ((((STD_ON == l_RamValid) && (STD_ON == l_NvRepair)) ||
                ((STD_ON == l_RamValid) && (STD_ON == l_RamChanged) && (STD_OFF == l_NvProt))) &&
                (STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_SELECTBLOCKFORWRITEALL)))
        {
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
            if ((NVM_BLOCK_DATASET == NvM_CurRunning.ManagementType)
                    && (NvM_CurRunning.NvNum <= NvM_CurRunning.Index))
            {
                NvM_SetJobOverFlag(NVM_REQ_NOT_OK, STD_OFF);
            }
            else
#endif
            {
                NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
                NvM_AtomJobReq(NVM_ATOMJOB_WRITEBLOCK);
            }
        }
        else
        {
            NvM_SetJobOverFlag(NVM_REQ_BLOCK_SKIPPED, STD_OFF);
        }
    }
    return;
}
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
/* @req NVM447 NVM399 NVM400 NVM192 */
void  NvM_Init(void)
{
    uint16 l_BlockIdLoop = 0u;
#if(((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&STD_ON==NVM_JOB_PRIORITIZATION))
    uint8 l_LoopCount = 0u;
    uint8 l_ModLoop = 0u;
    uint8 l_ModLoopCount = 0u;
#endif

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
    l_LoopCount = (NVM_SIZE_STANDARD_JOB_QUEUE - 1) >> 4;
    l_ModLoopCount = (NVM_SIZE_STANDARD_JOB_QUEUE - 1) % 16;
    NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] = 0x0000u;
    NvM_QueueSpaceTalbe[l_LoopCount] = 0x0000u;
    for (l_BlockIdLoop = 0; l_BlockIdLoop <= l_LoopCount; l_BlockIdLoop++)
    {
        if (l_BlockIdLoop < l_LoopCount)
        {
            NvM_QueueSpaceTalbe[l_BlockIdLoop] = 0xFFFFu;
        }
        else
        {
            for (l_ModLoop = 0; l_ModLoop <= l_ModLoopCount; l_ModLoop++)
            {
                NvM_QueueSpaceTalbe[l_LoopCount] |= (0x0001u << l_ModLoop);
            }
        }
        NvM_QueueSpaceTalbe[NVM_TABLE_SIZE_JOB_QUEUE - 1] |= (0x0001u << l_BlockIdLoop);
    }
#endif
    for (l_BlockIdLoop = 1; l_BlockIdLoop <= NVM_BLOCK_NUM_ALL; l_BlockIdLoop++)
    {
        NvM_AdminBlock[l_BlockIdLoop].FlagGroup = 0;
        NvM_AdminBlock[l_BlockIdLoop].CurrentIndex = 0; /* @req NVM192 */
        NvM_AdminBlock[l_BlockIdLoop].SingleReqResult = NVM_REQ_NOT_OK;
        if (STD_ON == NvM_GetByteBitState(NvM_BlockDescriptor[l_BlockIdLoop].FlagGroup, NVM_BDSC_BLOCKWRITEPROT))
        {
            NvM_SetByteBitState(&NvM_AdminBlock[l_BlockIdLoop].FlagGroup, NVM_ADMIN_NV_WRPRO, STD_ON);
        }
        else
        {
            NvM_SetByteBitState(&NvM_AdminBlock[l_BlockIdLoop].FlagGroup, NVM_ADMIN_NV_WRPRO, STD_OFF);
        }
    }
    NvM_Module.Init = STD_ON;
    return;
}





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
/* @req NVM448 NVM027 NVM191 NVM401 */
void  NvM_SetDataIndex(NvM_BlockIdType BlockId, uint8 DataIndex)
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 1, NVM_E_NOT_INITIALIZED);
        return;
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 1, NVM_E_PARAM_BLOCK_ID);
        return;
    }

    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 1, NVM_E_BLOCK_PENDING);
        return;
    }

    if (NVM_BLOCK_DATASET != NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 1, NVM_E_PARAM_BLOCK_TYPE);
        return;
    }
    else if ((NvM_BlockDescriptor[BlockId].NvmNvBlockNum + NvM_BlockDescriptor[BlockId].NvmRomBlockNum) <= DataIndex)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 1, NVM_E_PARAM_BLOCK_DATA_IDX);
        return;
    }
#endif
#if(NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId].NvmBlockManagementType) /* @req NVM264 */
    {
        NvM_AdminBlock[BlockId].CurrentIndex = DataIndex; /* @req NVM014 */
    }
    return;
#endif
}

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
/* @req NVM027 NVM191 NVM402 */
void  NvM_GetDataIndex(NvM_BlockIdType BlockId, uint8 * DataIndexPtr) /* @req NVM449 */
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 2, NVM_E_NOT_INITIALIZED);
        return;
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 2, NVM_E_PARAM_BLOCK_ID);
        return;
    }

    if (NVM_BLOCK_DATASET != NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 2, NVM_E_PARAM_BLOCK_TYPE);
        return;
    }

    if (NULL_PTR == DataIndexPtr)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 2, NVM_E_PARAM_DATA);
        return;
    }
#endif
#if(NVM_API_CONFIG_CLASS_1 != NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
    {
        *DataIndexPtr = NvM_AdminBlock[BlockId].CurrentIndex; /* @req NVM021 */
    }
    else
#endif
    {
        *DataIndexPtr = 0; /* @req NVM265 */
    }
    return;
}
#endif

#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
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
/* @req NVM027 NVM191 NVM404 */
void NvM_GetErrorStatus(NvM_BlockIdType BlockId, uint8 * RequestResultPtr) /* @req NVM451 */
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 4, NVM_E_NOT_INITIALIZED);
        return;
    }

    if (NVM_BLOCK_NUM_ALL < BlockId)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 4, NVM_E_PARAM_BLOCK_ID);
        return;
    }

    if (NULL_PTR == RequestResultPtr)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 4, NVM_E_PARAM_DATA);
        return;
    }
#endif

    /*multi type job is running now.*/
    if (0U == BlockId)
    {
        /*multi job is running but not execute over,return the latest single job result*/
        if (NVM_REQ_PENDING == NvM_MultiJob.ReqResult)
        {
            if ((NVM_JOB_STEP_IDLE != NvM_Module.JobStep)
                    && (1U < NvM_CurRunning.BlockId))
            {
                *RequestResultPtr = NvM_AdminBlock[NvM_CurRunning.BlockId - 1U].SingleReqResult; /* @req NVM015 */
            }
            else
            {
                *RequestResultPtr = NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult; /* @req NVM015 */
            }
        }
        else
        {
            /*
             *multi type job is running over,but not have any other type job is pending,
             *return the last multi type job common result.
             */
            *RequestResultPtr = NvM_MultiJob.ReqResult; /* @req NVM015 */
        }
    }
    else
    {
        /*single type job is running*/
        *RequestResultPtr = NvM_AdminBlock[BlockId].SingleReqResult; /* @req NVM015 */
    }
    return;
}

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
/* @req NVM027 NVM191 NVM407 NVM408 NVM409 */
void NvM_SetRamBlockStatus(NvM_BlockIdType BlockId, boolean BlockChanged) /* @req NVM453 */
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 5, NVM_E_NOT_INITIALIZED);
        return;
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 5, NVM_E_PARAM_BLOCK_ID);
        return;
    }

    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 5, NVM_E_BLOCK_PENDING);
        return;
    }
#endif

    if ((STD_ON != NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
            && (NULL_PTR != NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress)) /* @req NVM240 */
    {
        if (STD_ON == BlockChanged)
        {
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_VALID, STD_ON); /* @req NVM241 NVM406 */
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
            if (STD_ON == NvM_GetByteBitState(NvM_BlockDescriptor[BlockId].FlagGroup, NVM_BDSC_CALCRAMBLOCKCRC))
            {
                /* @req NVM121 */
                //if (E_NOT_OK == NvM_JobEnqueue(BlockId, NVM_SERV_ID_CALC_CRC, NULL_PTR))
                //{
                /*Reserved*/
                //}
            }
        }
        else
        {
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF); /* @req NVM405 */
        }
        /*please ref state diagram to complete this func*/
    }
    return;
}
#endif

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
/* @req NVM027 NVM191 NVM409 */
Std_ReturnType NvM_ReadBlock(NvM_BlockIdType BlockId, uint8 * NvM_DstPtr) /* @req NVM454 */
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 6, NVM_E_NOT_INITIALIZED);
        return (l_ReturnValue);
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 6, NVM_E_PARAM_BLOCK_ID);
        return (l_ReturnValue);
    }
    /* @req NVM196 */
    if ((NULL_PTR == NvM_DstPtr) && (NULL_PTR == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 6, NVM_E_PARAM_ADDRESS);
        return (l_ReturnValue);
    }
    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 6, NVM_E_BLOCK_PENDING);
        return (l_ReturnValue);
    }
#endif
    l_ReturnValue = NvM_JobEnqueue(BlockId, NVM_SERV_ID_READ_BLOCK, NvM_DstPtr); /* @req NVM195 */

#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (E_NOT_OK == l_ReturnValue)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 6, NVM_E_LIST_OVERFLOW);
        return (l_ReturnValue);
    }
#endif
    if (E_OK == l_ReturnValue)
    {
        if ((NULL_PTR == NvM_DstPtr) || (NvM_DstPtr == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
        {
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF); /* @req NVM198 */
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
        }
    }
    return (l_ReturnValue);
}

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
/* @req NVM027 NVM191 NVM409 */
Std_ReturnType NvM_WriteBlock(NvM_BlockIdType BlockId, uint8 * NvM_SrcPtr)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_NOT_INITIALIZED);
        return (l_ReturnValue);
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_PARAM_BLOCK_ID);
        return (l_ReturnValue);
    }
    /* @req NVM197 */
    if ((NULL_PTR == NvM_SrcPtr) && (NULL_PTR == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_PARAM_ADDRESS);
        return (l_ReturnValue);
    }
    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_BLOCK_PENDING);
        return (l_ReturnValue);
    }
    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_NV_WRPRO))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_NV_WRITE_PROTECTED);
        return (l_ReturnValue);
    }
#endif
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if ((NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
            && (NvM_BlockDescriptor[BlockId].NvmNvBlockNum <= NvM_AdminBlock[BlockId].CurrentIndex))
    {
        l_ReturnValue = E_NOT_OK;
    }
    else if (STD_OFF == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_NV_WRPRO))
#else
    if (STD_OFF == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_NV_WRPRO))
#endif
    {
        l_ReturnValue = NvM_JobEnqueue(BlockId, NVM_SERV_ID_WRITE_BLOCK, NvM_SrcPtr); /* @req NVM195 */
#if(STD_ON == NVM_DEV_ERROR_DETECT)
        if (E_NOT_OK == l_ReturnValue)
        {
            Det_ReportError(NVM_MODULE_ID, 0, 7, NVM_E_LIST_OVERFLOW);
            return (l_ReturnValue);
        }
#endif
    }
    else
    {
        l_ReturnValue = E_NOT_OK; /* @req NVM217 */
    }

    if (E_OK == l_ReturnValue)
    {
#if(STD_ON==NVM_JOB_PRIORITIZATION)
        if ((0 == NvM_BlockDescriptor[BlockId].NvmBlockJobPriority)
                && (NVM_JOB_STEP_IDLE != NvM_Module.JobStep)
                && (NVM_JOB_TYPE_SINGLE_IMMED != NvM_Module.CurrentJobType)
                && (NVM_JOB_TYPE_MULTI_GAP_SINGLE_IMMED != NvM_Module.CurrentJobType))
        {
            NvM_Module.JobStep = NVM_JOB_STEP_CANCEL;
            MemIf_Cancel(NvM_CurRunning.DeviceId);
        }
#endif
        /*please ref state diagram to complete this func page39*/
#if (STD_ON == NVM_SET_RAM_BLOCK_STATUS_API)
        /* @req NVM344 */
        /*treat the current block as valid and changed.*/
        if ((NULL_PTR == NvM_SrcPtr) || (NvM_SrcPtr == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
        {
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_VALID, STD_ON);
            NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_ON);
        }
#endif
    }
    return (l_ReturnValue);
}
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
/* @req NVM027 NVM191 NVM409 */
Std_ReturnType NvM_RestoreBlockDefaults(NvM_BlockIdType BlockId, uint8 * NvM_DestPtr)
{
    Std_ReturnType l_ReturnValue = E_NOT_OK;

#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_NOT_INITIALIZED);
        return (l_ReturnValue);
    }

    if ((NVM_BLOCK_NUM_ALL < BlockId) || (0 == BlockId))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_PARAM_BLOCK_ID);
        return (l_ReturnValue);
    }
    /* @req NVM197 */
    if ((NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
            && (NvM_BlockDescriptor[BlockId].NvmNvBlockNum >= NvM_AdminBlock[BlockId].CurrentIndex))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_PARAM_ADDRESS);
        return (l_ReturnValue);
    }
    if ((NULL_PTR == NvM_DestPtr) && (NULL_PTR == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_BLOCK_CONFIG);
        return (l_ReturnValue);
    }
    if (STD_ON == NvM_GetByteBitState(NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_ENQUEUE))
    {
        Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_BLOCK_PENDING);
        return (l_ReturnValue);
    }
#endif
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if (NVM_BLOCK_DATASET == NvM_BlockDescriptor[BlockId].NvmBlockManagementType)
    {
        if ((NvM_AdminBlock[BlockId].CurrentIndex < (NvM_BlockDescriptor[BlockId].NvmNvBlockNum + NvM_BlockDescriptor[BlockId].NvmRomBlockNum))
                && (NvM_BlockDescriptor[BlockId].NvmNvBlockNum <= NvM_AdminBlock[BlockId].CurrentIndex))
        {
            l_ReturnValue = E_OK;
        }
    }
    else if (NULL_PTR != NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress) // NvmRomBlockDataAddress
#else
    if (NULL_PTR != NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress) // NvmRomBlockDataAddress
#endif
    {
        l_ReturnValue = E_OK;
    }
    else
    {
        if (NULL_PTR != NvM_BlockDescriptor[BlockId].NvmInitBlockCallback)
        {
            l_ReturnValue = E_OK;
        }
    }
    if (E_OK == l_ReturnValue)
    {
        l_ReturnValue = NvM_JobEnqueue(BlockId, NVM_SERV_ID_RESTORE_BLOCK, NvM_DestPtr); /* @req NVM195 */
#if(STD_ON == NVM_DEV_ERROR_DETECT)
        if (E_NOT_OK == l_ReturnValue)
        {
            Det_ReportError(NVM_MODULE_ID, 0, 8, NVM_E_LIST_OVERFLOW);
            return (l_ReturnValue);
        }
#endif
        if (E_OK == l_ReturnValue)
        {
            if ((NULL_PTR == NvM_DestPtr) || (NvM_DestPtr == NvM_BlockDescriptor[BlockId].NvmRamBlockDataAddress))
            {
                NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_VALID, STD_OFF);
                NvM_SetByteBitState(&NvM_AdminBlock[BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED, STD_OFF);
            }
        }
    }
    return (l_ReturnValue);
    /*please ref state diagram to complete this func*/
}
#endif

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
/* @req NVM027 NVM191 NVM409 */
void  NvM_CancelWriteAll(void)
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 0x0A, NVM_E_NOT_INITIALIZED);
        return;
    }
#endif
#if(NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)
    if ((STD_ON == NvM_MultiJob.Enqueue)
            && (NVM_SERV_ID_WRITE_ALL == NvM_MultiJob.ServiceId))
#else
    if ((STD_ON == NvM_MultiJob.Enqueue)
            && (NVM_SERV_ID_WRITE_ALL == NvM_CurRunning.ServiceId))
#endif
    {
        NvM_MultiJob.CancelWriteAll = STD_ON;
    }
    return;
}

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
/* @req NVM027 NVM191 NVM409 */
void  NvM_ReadAll(void)
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 0x0C, NVM_E_NOT_INITIALIZED);
        return;
    }
#endif

    if (STD_ON != NvM_MultiJob.Enqueue)
    {
        if (E_NOT_OK == NvM_JobEnqueue(0, NVM_SERV_ID_READ_ALL, NULL_PTR))
        {
            /*This will not happen*/
        }
    }
    return;
}

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
/* @req NVM027 NVM191 NVM409 */
void  NvM_WriteAll(void)
{
#if(STD_ON == NVM_DEV_ERROR_DETECT)
    if (STD_ON != NvM_Module.Init)
    {
        Det_ReportError(NVM_MODULE_ID, 0, 0x0D, NVM_E_NOT_INITIALIZED);
        return;
    }
#endif
    if (STD_ON != NvM_MultiJob.Enqueue)
    {
        if (E_NOT_OK == NvM_JobEnqueue(0, NVM_SERV_ID_WRITE_ALL, NULL_PTR))
        {
            /*This will not happen*/
        }
    }
    return;
}
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
void NvM_MainFunction(void)
{
    if (MEMIF_UNINIT == Ea_GetStatus())
        //if(MEMIF_UNINIT==MemIf_GetStatus(NvM_CurRunning.DeviceId))
    {
        return;
    }
    do
    {
        //#if (STD_ON == SYS_MODULE_ADD_WDG)
        //WDG_Refresh(INST_WDOG_PAL_CFG);
        //#endif
        while (NVM_JOB_STEP_IDLE != NvM_Module.JobStep)
        {
            if (0U == NvM_AtomJob.Count)
            {
                (*NvM_Inner_ServFuncMatrix[NvM_CurRunning.ServiceId])();
            }
            else
            {
                (*NvM_AtomJobFuncMatrix[NvM_AtomJob.ReqId[NvM_AtomJob.Count]])();
                if (   (NVM_JOB_STEP_IDLE  == NvM_Module.JobStep)
                        && (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
                        && (NVM_REQ_OK         != NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult))
                {
                    NvM_CurRunning.EMultiReqResult = NVM_REQ_NOT_OK;
                }
            }
        }
        if (E_OK == NvM_JobDequeue())
        {
            NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
            if (NVM_SERV_ID_NONE <= NvM_CurRunning.ServiceId)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
            }
        }
        else
        {
            break;
        }
    }
    while (1);
}




/*************************************************************************/
/*
 * Brief               Function to be used by the underlying memory abstraction to signal end of job without error
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
void NvM_JobEndNotification(void)
{
    return;
}

/*************************************************************************/
/*
 * Brief               Function to be used by the underlying memory abstraction to signal end of job with error
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
void NvM_JobErrorNotification(void)
{
    return;
}

NvM_JobTypeType NvM_GetCurrentJobType(void)
{
    return (NvM_Module.CurrentJobType);
}

void NvM_ResetForWriteAll()
{
    if (   (NVM_SERV_ID_WRITE_BLOCK == NvM_CurRunning.ServiceId)
            && (STD_OFF == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_SELECTBLOCKFORREADALL)))
    {
        NvM_JobEnqueue(NvM_CurRunning.BlockId, NvM_CurRunning.ServiceId, NvM_CurRunning.RamAddr);
    }
    NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
    NvM_AtomJob.Count  = 0;
    while (1)
    {
        if (E_OK == NvM_JobDequeue())
        {
            if (   (NVM_SERV_ID_WRITE_BLOCK != NvM_CurRunning.ServiceId)
                    || ((STD_ON == NvM_GetByteBitState(NvM_CurRunning.BDSCFlagGroup, NVM_BDSC_SELECTBLOCKFORWRITEALL))
                        && (STD_ON  == NvM_GetByteBitState(NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup, NVM_ADMIN_RAM_VALID))
                        && (STD_ON  == NvM_GetByteBitState(NvM_AdminBlock[NvM_CurRunning.BlockId].FlagGroup, NVM_ADMIN_RAM_CHANGED))))
            {
                continue;
            }
            NvM_Module.JobStep = NVM_JOB_STEP_PENDING;
            if (NVM_SERV_ID_NONE <= NvM_CurRunning.ServiceId)
            {
                NvM_Module.JobStep = NVM_JOB_STEP_IDLE;
            }
        }
        else
        {
            break;
        }

        while (NVM_JOB_STEP_IDLE != NvM_Module.JobStep)
        {
            if (0U == NvM_AtomJob.Count)
            {
                (*NvM_Inner_ServFuncMatrix[NvM_CurRunning.ServiceId])();
            }
            else
            {
                (*NvM_AtomJobFuncMatrix[NvM_AtomJob.ReqId[NvM_AtomJob.Count]])();
                if (   (NVM_JOB_STEP_IDLE  == NvM_Module.JobStep)
                        && (NVM_JOB_TYPE_MULTI == NvM_Module.CurrentJobType)
                        && (NVM_REQ_OK         != NvM_AdminBlock[NvM_CurRunning.BlockId].SingleReqResult))
                {
                    NvM_CurRunning.EMultiReqResult = NVM_REQ_NOT_OK;
                }
            }
        }
    }
    return ;
}


/*====================[E N D  O F   F I L E]====================*/

