/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcMutex.h
 * @brief    ##### 
 * @details  this file ###
 * @author  
 * @version  1.0.0
 * @date     4/19/2017
 **************************************************************************/
#ifndef __IPC_MUTEX_H__
#define __IPC_MUTEX_H__

#include "ipcType.h"
#include "Std_Types.h"

//#include "osif.h" 
//typedef mutex_t MUTEX;

boolean spiMutexCreate(uint8 *mutexIndex);
boolean spiMutexLock(uint8 mutexIndex);
boolean spiMutexUnlock(uint8 mutexIndex);

#if  0
void mutexCreate(MUTEX* mutex);


void mutexDestroy (MUTEX *mutex);

void mutexLock(MUTEX  *mutex);

void mutexUnlock(MUTEX *mutex);
#endif

#endif

