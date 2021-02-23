/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm.h
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
 *   V1.0.0     20170000     SDu       Initial version
 *
 */
/*============================================================================*/
#ifndef NVM_H
#define NVM_H

/*====================[I N C L U D E S]====================*/
#include "NvM_Types.h"

/*===================[V E R S I O N  I N F O R M A T I O N]===================*/
#define NVM_MODULE_ID 20
#define NVM_H_AR_MAJOR_VERSION 2U
#define NVM_H_AR_MINOR_VERSION 3U
#define NVM_H_AR_PATCH_VERSION 0U
#define NVM_H_SW_MAJOR_VERSION 1U
#define NVM_H_SW_MINOR_VERSION 0U
#define NVM_H_SW_PATCH_VERSION 3U
#define NVM_VENDOR_API_INFIX   0U

#if (STD_ON == NVM_VERSION_INFO_API)
/*************************************************************************/
/*
 * Brief               This service returns the version information of
 *                     this module
 * ServiceId           0x0f
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     Versioninfo: Pointer to where to store the version
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/

#if (STD_ON == NVM_DEV_ERROR_DETECT)
#define NvM_GetVersionInfo(VersionInfo) \
    do{\
        if (NULL_PTR == (VersionInfo))\
        { \
            Det_ReportError(NVM_MODULE_ID, 0, 2, NVM_E_PARAM_DATA);\
        }\
        else\
        {\
            (VersionInfo)->vendorID = 0; \
            (VersionInfo)->moduleID = NVM_MODULE_ID; \
            (VersionInfo)->instanceID = 0u; \
            (VersionInfo)->sw_major_version = NVM_H_SW_MAJOR_VERSION; \
            (VersionInfo)->sw_minor_version = NVM_H_SW_MINOR_VERSION; \
            (VersionInfo)->sw_patch_version = NVM_H_SW_PATCH_VERSION; \
        }\
    }while(0)
#else
#define NvM_GetVersionInfo(VersionInfo) \
    do{\
        (VersionInfo)->vendorID = 0; \
        (VersionInfo)->moduleID = NVM_MODULE_ID; \
        (VersionInfo)->instanceID = 0u; \
        (VersionInfo)->sw_major_version = NVM_H_SW_MAJOR_VERSION; \
        (VersionInfo)->sw_minor_version = NVM_H_SW_MINOR_VERSION; \
        (VersionInfo)->sw_patch_version = NVM_H_SW_PATCH_VERSION; \
    }while(0)
#endif

#endif/* STD_ON == NVM_VERSION_INFO_API */

#endif /* End of NVM_H*/

/*====================[E N D   O F   F I L E]====================*/

