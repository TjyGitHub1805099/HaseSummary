/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcSemFreeRTOS.c
 * @brief    ##### 
 * @details  this file ###
 * @author   
 * @version  1.0.0
 * @date     4/20/2017
 **************************************************************************/
#ifndef _TASK_IPC_SEM_H_
#define _TASK_IPC_SEM_H_
#include "osif.h" 
#include "status.h"
#include "Std_Types.h"

boolean Task_IpcSemCreate(void);

void Task_IpcSemPost(void);

boolean Task_IpcSemWait(uint16_t timeout);

#endif

