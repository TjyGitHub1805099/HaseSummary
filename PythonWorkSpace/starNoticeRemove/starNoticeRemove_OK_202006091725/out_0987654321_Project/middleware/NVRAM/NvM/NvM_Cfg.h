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

#define NVM_BLOCK_NUM_ALL_DF          144U

#define NVM_MAX_LENGTH_NV_BLOCK    40U

//Nvm BlockID Define
#define  NvM_ConfigureIdNvMBlockID    1U
#define  TestIDNvMBlockID    2U
#define  g_odo_OdoNvMBlockID    3U
#define  g_odo_backNvMBlockID    4U
#define  g_odo_paraNvMBlockID    5U
#define  g_VEL_CodingNvMBlockID    6U
#define  g_VEL_LT_ConfigNvMBlockID    7U
#define  g_SERV_CodingNvMBlockID    8U
#define  g_Unit_CodingNvMBlockID    9U
#define  g_sys_settingNvMBlockID    10U
#define  gEnableTpTraceByCanFlagNvMBlockID    11U
#define  socSerialNumNvMBlockID    12U
#define  g_IVI_VINNvMBlockID    13U
#define  gIVI_PKI_CERTIFICATENvMBlockID    14U
#define  g_odo_back1NvMBlockID    15U
#define  gIVICanFaultBitFlagArrayNvMBlockID    16U
#define  gIcCanFaultBitFlagArrayNvMBlockID    17U
#define  gRuntimeSnapshot_IVI_0_NvMBlockID    18U
#define  gRuntimeSnapshot_IVI_1_NvMBlockID    19U
#define  gRuntimeSnapshot_IVI_2_NvMBlockID    20U
#define  gRuntimeSnapshot_IVI_3_NvMBlockID    21U
#define  gRuntimeSnapshot_IVI_4_NvMBlockID    22U
#define  gRuntimeSnapshot_IVI_5_NvMBlockID    23U
#define  gRuntimeSnapshot_IVI_6_NvMBlockID    24U
#define  gRuntimeSnapshot_IVI_7_NvMBlockID    25U
#define  gRuntimeSnapshot_IVI_8_NvMBlockID    26U
#define  gRuntimeSnapshot_IVI_9_NvMBlockID    27U
#define  gRuntimeSnapshot_IVI_10_NvMBlockID    28U
#define  gRuntimeSnapshot_IVI_11_NvMBlockID    29U
#define  gRuntimeSnapshot_IVI_12_NvMBlockID    30U
#define  gRuntimeSnapshot_IVI_13_NvMBlockID    31U
#define  gRuntimeSnapshot_IVI_14_NvMBlockID    32U
#define  gRuntimeSnapshot_IVI_15_NvMBlockID    33U
#define  gDtcStatusAndExtInfor_IVI_0_NvMBlockID    34U
#define  gDtcStatusAndExtInfor_IVI_1_NvMBlockID    35U
#define  gDtcStatusAndExtInfor_IVI_2_NvMBlockID    36U
#define  gDtcStatusAndExtInfor_IVI_3_NvMBlockID    37U
#define  gDtcStatusAndExtInfor_IVI_4_NvMBlockID    38U
#define  gDtcStatusAndExtInfor_IVI_5_NvMBlockID    39U
#define  gDtcStatusAndExtInfor_IVI_6_NvMBlockID    40U
#define  gDtcStatusAndExtInfor_IVI_7_NvMBlockID    41U
#define  gDtcStatusAndExtInfor_IVI_8_NvMBlockID    42U
#define  gDtcStatusAndExtInfor_IVI_9_NvMBlockID    43U
#define  gDtcStatusAndExtInfor_IVI_10_NvMBlockID    44U
#define  gDtcStatusAndExtInfor_IVI_11_NvMBlockID    45U
#define  gDtcStatusAndExtInfor_IVI_12_NvMBlockID    46U
#define  gDtcStatusAndExtInfor_IVI_13_NvMBlockID    47U
#define  gDtcStatusAndExtInfor_IVI_14_NvMBlockID    48U
#define  gDtcStatusAndExtInfor_IVI_15_NvMBlockID    49U
#define  gRuntimeSnapshot_IC_0_NvMBlockID    50U
#define  gRuntimeSnapshot_IC_1_NvMBlockID    51U
#define  gRuntimeSnapshot_IC_2_NvMBlockID    52U
#define  gRuntimeSnapshot_IC_3_NvMBlockID    53U
#define  gRuntimeSnapshot_IC_4_NvMBlockID    54U
#define  gRuntimeSnapshot_IC_5_NvMBlockID    55U
#define  gRuntimeSnapshot_IC_6_NvMBlockID    56U
#define  gRuntimeSnapshot_IC_7_NvMBlockID    57U
#define  gRuntimeSnapshot_IC_8_NvMBlockID    58U
#define  gRuntimeSnapshot_IC_9_NvMBlockID    59U
#define  gRuntimeSnapshot_IC_10_NvMBlockID    60U
#define  gRuntimeSnapshot_IC_11_NvMBlockID    61U
#define  gRuntimeSnapshot_IC_12_NvMBlockID    62U
#define  gRuntimeSnapshot_IC_13_NvMBlockID    63U
#define  gRuntimeSnapshot_IC_14_NvMBlockID    64U
#define  gDtcStatusAndExtInfor_IC_0_NvMBlockID    65U
#define  gDtcStatusAndExtInfor_IC_1_NvMBlockID    66U
#define  gDtcStatusAndExtInfor_IC_2_NvMBlockID    67U
#define  gDtcStatusAndExtInfor_IC_3_NvMBlockID    68U
#define  gDtcStatusAndExtInfor_IC_4_NvMBlockID    69U
#define  gDtcStatusAndExtInfor_IC_5_NvMBlockID    70U
#define  gDtcStatusAndExtInfor_IC_6_NvMBlockID    71U
#define  gDtcStatusAndExtInfor_IC_7_NvMBlockID    72U
#define  gDtcStatusAndExtInfor_IC_8_NvMBlockID    73U
#define  gDtcStatusAndExtInfor_IC_9_NvMBlockID    74U
#define  gDtcStatusAndExtInfor_IC_10_NvMBlockID    75U
#define  gDtcStatusAndExtInfor_IC_11_NvMBlockID    76U
#define  gDtcStatusAndExtInfor_IC_12_NvMBlockID    77U
#define  gDtcStatusAndExtInfor_IC_13_NvMBlockID    78U
#define  gDtcStatusAndExtInfor_IC_14_NvMBlockID    79U
#define  gRuntimeSnapshot_AVM_0_NvMBlockID    80U
#define  gRuntimeSnapshot_AVM_1_NvMBlockID    81U
#define  gRuntimeSnapshot_AVM_2_NvMBlockID    82U
#define  gRuntimeSnapshot_AVM_3_NvMBlockID    83U
#define  gRuntimeSnapshot_AVM_4_NvMBlockID    84U
#define  gRuntimeSnapshot_AVM_5_NvMBlockID    85U
#define  gRuntimeSnapshot_AVM_6_NvMBlockID    86U
#define  gRuntimeSnapshot_AVM_7_NvMBlockID    87U
#define  gRuntimeSnapshot_AVM_8_NvMBlockID    88U
#define  gRuntimeSnapshot_AVM_9_NvMBlockID    89U
#define  gDtcStatusAndExtInfor_AVM_0_NvMBlockID    90U
#define  gDtcStatusAndExtInfor_AVM_1_NvMBlockID    91U
#define  gDtcStatusAndExtInfor_AVM_2_NvMBlockID    92U
#define  gDtcStatusAndExtInfor_AVM_3_NvMBlockID    93U
#define  gDtcStatusAndExtInfor_AVM_4_NvMBlockID    94U
#define  gDtcStatusAndExtInfor_AVM_5_NvMBlockID    95U
#define  gDtcStatusAndExtInfor_AVM_6_NvMBlockID    96U
#define  gDtcStatusAndExtInfor_AVM_7_NvMBlockID    97U
#define  gDtcStatusAndExtInfor_AVM_8_NvMBlockID    98U
#define  gDtcStatusAndExtInfor_AVM_9_NvMBlockID    99U
#define  gAvmCanFaultBitFlagArrayNvMBlockID    100U
#define  gpsInfoFromSocNvMBlockID    101U
#define  rtcStoredDataNvMBlockID    102U
#define  gWarningLampLightingRecords_0_NvMBlockID    103U
#define  gWarningLampLightingRecords_5_NvMBlockID    104U
#define  gWarningLampLightingRecords_10_NvMBlockID    105U
#define  gWarningLampLightingRecords_15_NvMBlockID    106U
#define  gWarningLampLightingRecords_20_NvMBlockID    107U
#define  gWarningLampLightingRecords_25_NvMBlockID    108U
#define  gWarningLampLightingRecords_30_NvMBlockID    109U
#define  gWarningLampLightingRecords_35_NvMBlockID    110U
#define  gWarningLampLightingRecords_40_NvMBlockID    111U
#define  gWarningLampLightingRecords_45_NvMBlockID    112U
#define  gWarningLampLightingRecords_50_NvMBlockID    113U
#define  gWarningLampLightingRecords_55_NvMBlockID    114U
#define  g_AVM_ProductionLineNvMBlockID    115U
#define  gCrashRegisterRecordNvMBlockID    116U
#define  gRuntimeSnapshot_HUD_0_NvMBlockID    117U
#define  gRuntimeSnapshot_HUD_1_NvMBlockID    118U
#define  gRuntimeSnapshot_HUD_2_NvMBlockID    119U
#define  gRuntimeSnapshot_HUD_3_NvMBlockID    120U
#define  gRuntimeSnapshot_HUD_4_NvMBlockID    121U
#define  gRuntimeSnapshot_HUD_5_NvMBlockID    122U
#define  gRuntimeSnapshot_HUD_6_NvMBlockID    123U
#define  gRuntimeSnapshot_HUD_7_NvMBlockID    124U
#define  gRuntimeSnapshot_HUD_8_NvMBlockID    125U
#define  gRuntimeSnapshot_HUD_9_NvMBlockID    126U
#define  gRuntimeSnapshot_HUD_10_NvMBlockID    127U
#define  gRuntimeSnapshot_HUD_11_NvMBlockID    128U
#define  gRuntimeSnapshot_HUD_12_NvMBlockID    129U
#define  gRuntimeSnapshot_HUD_13_NvMBlockID    130U
#define  gDtcStatusAndExtInfor_HUD_0_NvMBlockID    131U
#define  gDtcStatusAndExtInfor_HUD_1_NvMBlockID    132U
#define  gDtcStatusAndExtInfor_HUD_2_NvMBlockID    133U
#define  gDtcStatusAndExtInfor_HUD_3_NvMBlockID    134U
#define  gDtcStatusAndExtInfor_HUD_4_NvMBlockID    135U
#define  gDtcStatusAndExtInfor_HUD_5_NvMBlockID    136U
#define  gDtcStatusAndExtInfor_HUD_6_NvMBlockID    137U
#define  gDtcStatusAndExtInfor_HUD_7_NvMBlockID    138U
#define  gDtcStatusAndExtInfor_HUD_8_NvMBlockID    139U
#define  gDtcStatusAndExtInfor_HUD_9_NvMBlockID    140U
#define  gDtcStatusAndExtInfor_HUD_10_NvMBlockID    141U
#define  gDtcStatusAndExtInfor_HUD_11_NvMBlockID    142U
#define  gDtcStatusAndExtInfor_HUD_12_NvMBlockID    143U
#define  gDtcStatusAndExtInfor_HUD_13_NvMBlockID    144U
/*** The above lines is generated by tool ***/
/*** Generation data:2020-02-17 11:10 ***/
/*** End , DO NOT MODIFY THIS TEXT!!! ***/

//#define SELF_DEBUG
#endif /* End of NVM_CFG_H*/

/*====================[E N D   O F   F I L E]====================*/
