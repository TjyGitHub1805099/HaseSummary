/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcSem.h
 * @brief    ##### 
 * @details  this file ###
 * @author   
 * @version  1.0.0
 * @date     4/20/2017
 **************************************************************************/
#ifndef _IPC_SEM_H_
#define _IPC_SEM_H_

#include "ipcType.h"
#include "Std_Types.h"

boolean spiSemCreate(const uint8 uxInitialCount);
void spiSemPost(uint8 HwChannel);
boolean spiSemWait(uint8 HwChannel);

#endif

