/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Nvm_cbk.c
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
 *   V1.0.0     20170615  Stephen Du   Initial version
 *
 */
/*============================================================================*/
#ifndef NVM_CBK_H
#define NVM_CBK_H

/*====================[V E R S I O N  I N F O R M A T I O N]====================*/
#define NVM_CBK_H_AR_MAJOR_VERSION 2U
#define NVM_CBK_H_AR_MINOR_VERSION 3U
#define NVM_CBK_H_AR_PATCH_VERSION 0U
#define NVM_CBK_H_SW_MAJOR_VERSION 1U
#define NVM_CBK_H_SW_MINOR_VERSION 0U
#define NVM_CBK_H_SW_PATCH_VERSION 3U

/**
*Function to be used by the underlying memory abstraction to signal end of job without error.
*/
/*************************************************************************/
/*
 * Brief               Function to be used by the underlying memory abstraction to signal end of job without error
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Non Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up layer
 */
/*************************************************************************/
extern void NvM_JobEndNotification(void);

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
extern void NvM_JobErrorNotification(void);

extern void Nvm_LockMainMutex();
extern void Nvm_UnlockMainMutex();
#endif /* End of NVM_CBK_H*/
/*====================[E N D   O F   F I L E]====================*/

