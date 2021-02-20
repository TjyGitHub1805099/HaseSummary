/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm_cfg.h
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
 *   V1.0.0     20170610  Stephen Du   Initial version
 *
 */
/*============================================================================*/
#ifndef NVM_CFG_H
#define NVM_CFG_H

/*====================[I N C L U D E S]====================*/
#include "Std_Types.h"



/*====================[V E R S I O N  I N F O R M A T I O N]====================*/
#define NVM_CFG_H_AR_MAJOR_VERSION 2U
#define NVM_CFG_H_AR_MINOR_VERSION 3U
#define NVM_CFG_H_AR_PATCH_VERSION 0U
#define NVM_CFG_H_SW_MAJOR_VERSION 1U
#define NVM_CFG_H_SW_MINOR_VERSION 0U
#define NVM_CFG_H_SW_PATCH_VERSION 5U

/*====================[M A C R O S]====================*/

//Now only support NVM_API_CONFIG_CLASS_1 and NVM_API_CONFIG_CLASS_2
#define  NVM_API_CONFIG_CLASS  NVM_API_CONFIG_CLASS_2
#define NVM_DEM_ERROR_DETECT STD_OFF


#define  NVM_COMPILED_CONFIG_ID  0x5AA5U

#define  NVM_CRC_NUM_OF_BYTES  0x0020U

#define  NVM_DATASET_SELECTION_BITS  0x0002U

#define  NVM_DEV_ERROR_DETECT  STD_OFF

#define  NVM_DRV_MODE_SWITCH  STD_OFF

#define  NVM_DYNAMIC_CONFIGURATION  STD_ON

#define  NVM_JOB_PRIORITIZATION  STD_OFF

#define  NVM_MAX_NUM_OF_WRITE_RETRIES  0x03U

#define  NVM_MULTI_BLOCK_CALLBACK  NULL_PTR

#define  NVM_POLLING_MODE  STD_OFF

#define  NVM_SET_RAM_BLOCK_STATUS_API  STD_ON

#define  NVM_SIZE_IMMEDIATE_JOB_QUEUE  0x01U

#define  NVM_SIZE_STANDARD_JOB_QUEUE  0x0Fu

#define  NVM_VERSION_INFO_API  STD_ON

#define NVM_INCLUDE_DEM STD_OFF
#define NVM_INCLUDE_DET STD_OFF

#if((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
//The total number of different priority
#define NVM_TOTAL_NUM_DIFF_PRI 10U
#endif

/*** Start , DO NOT MODIFY THIS TEXT!!! ***/
/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
/*** ***********************The below lines is generated by tool*********************** ***/
extern const uint8 NVM_BLOCK_NUM_ALL;

#define NVM_BLOCK_NUM_ALL_DF          2U

#define NVM_MAX_LENGTH_NV_BLOCK    2U

//Nvm BlockID Define
#define  NvM_ConfigureIdNvMBlockID    1U
#define  TestIDNvMBlockID    2U
/*** The above lines is generated by tool ***/
/*** Generation data:2018-09-19 16:49 ***/
/*** End , DO NOT MODIFY THIS TEXT!!! ***/

//#define SELF_DEBUG
#endif /* End of NVM_CFG_H*/

/*====================[E N D   O F   F I L E]====================*/