/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm_cfg.c
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

/*====================[V E R S I O N  I N F O R M A T I O N]====================*/

/*====================[I N C L U D E S]====================*/
#include "NvM_Types.h"
#include "NvM.h"  
#include "MemMap.h"
#include "NvM_Cfg.h"

/* FlagGroup:
 * Bit 0:NvmWriteBlockOnce
 * Bit 1:NvmBlockUseCrc
 * Bit 2:NvmBlockWriteProt
 * Bit 3:NvmCalcRamBlockCrc
 * Bit 4:NvmResistantToChangedSw
 * Bit 5:NvmSelectBlockForReadall
 * Bit 6:NvmSelectBlockForWriteall 
 */
#define NVM_WRITE_BLOCK_ONCE 		1
#define NVM_BLOCK_USE_CRC    		2
#define NVM_BLOCK_WRITE_PROT   		4
#define NVM_CALC_RAM_BLOCK_CRC 		8
#define NVM_RESISTANT2CHANGEDSW 	16
#define NVM_SELCT_BLOCK_4READALL	32
#define NVM_SELCT_BLOCK_4WRITEALL	64

NVM_SEC_VAR_UNSPECIFIED uint8 TestID = 0X55u;
NVM_SEC_VAR_UNSPECIFIED uint16 NvM_ConfigureId = NVM_COMPILED_CONFIG_ID;


/*** Start , DO NOT MODIFY THIS TEXT!!! ***/
/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
/*** ***********************The below lines is generated by tool*********************** ***/
const uint8 NVM_BLOCK_NUM_ALL = 2;
uint8 NvM_NvDataBuffer[NVM_MAX_LENGTH_NV_BLOCK + NVM_NV_CRC_MAX_LENGTH];
NvM_AdminBlockType NvM_AdminBlock[NVM_BLOCK_NUM_ALL_DF + 1]={0};

const NvM_BlockDescriptorType NvM_BlockDescriptor[NVM_BLOCK_NUM_ALL_DF + 1] =
{
    {0,/*it not used,start with block1*/
        NVM_BLOCK_NATIVE,
    #if ((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
        255,
    #endif
        0,NVM_CRC16,0,0,0,0,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR 
    },
    {/*Block 1*/
        0, /* NvmNvramDeviceId */
        NVM_BLOCK_NATIVE,    /* NvmBlockManagementType */
        #if ((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
        0,/* NvmBlockJobPriority */
        #endif
        NVM_SELCT_BLOCK_4READALL|NVM_SELCT_BLOCK_4WRITEALL|NVM_CALC_RAM_BLOCK_CRC|NVM_BLOCK_USE_CRC|0,
        NVM_CRC16,/* NvmBlockCRCType */
        1,/* NvmNvBlockBaseNumber */
        2,/* NvmNvBlockLength */
        1,/*NvmNvBlockNum*/
        0,/* NvmRomBlockNum */
        (uint8*)&NvM_ConfigureId,   /* NvmRamBlockDataAddress */
        NULL_PTR, /* NvmInitBlockCallback */
        NULL_PTR/* NvmSingleBlockCallback */
    },
    {/*Block 2*/
        0, /* NvmNvramDeviceId */
        NVM_BLOCK_REDUNDANT,    /* NvmBlockManagementType */
        #if ((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
        0,/* NvmBlockJobPriority */
        #endif
        NVM_SELCT_BLOCK_4READALL|NVM_SELCT_BLOCK_4WRITEALL|NVM_CALC_RAM_BLOCK_CRC|NVM_BLOCK_USE_CRC|0,
        NVM_CRC16,/* NvmBlockCRCType */
        2,/* NvmNvBlockBaseNumber */
        2,/* NvmNvBlockLength */
        2,/*NvmNvBlockNum*/
        0,/* NvmRomBlockNum */
        (uint8*)&TestID,   /* NvmRamBlockDataAddress */
        NULL_PTR, /* NvmInitBlockCallback */
        NULL_PTR/* NvmSingleBlockCallback */
    },

};
/*** Generation data:2018-09-19 16:49 ***/
/*** The above lines is generated by tool ***/
/*** End , DO NOT MODIFY THIS TEXT!!! ***/

