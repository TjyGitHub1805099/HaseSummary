/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : Ea.c
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

#include "Std_Types.h" 
#ifndef EA_APIFORAPP_H
#define Ea_APIFORAPP_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************/
/*
 * Function Name       Ea_SetWriteBalance(uint8 State)
 * Brief               Enabled/Disabled balance write 
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      State:STD_ON-Enabled STD_OFF-Disabled
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 */
/*************************************************************************/
void  Ea_SetWriteBalance(uint8 State);

/*************************************************************************/
/*
 * Function Name       Ea_ResetDevice()
 * Brief               Reset EEPROM Block1,it will reset Nvm when reboot
 * Sync/Async          Synchronous
 * Reentrancy          Non-Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              E_OK     : Succeed. 
 *                     E_NOT_OK : write data to EEPROM fialed
 * PreCondition        None
 */
/*************************************************************************/
Std_ReturnType      Ea_ResetDevice();


                                                                                                                                                                                              
#ifdef __cplusplus
}
#endif
#endif

