/***************************************************************************
 * @copyright Copyright(C)  All rights reserved  2016-2026
 *   Hangsheng Automotive Electrical Co.,Ltd.
 * 
 * @file     ipcTypes.h
 * @brief    ##### 
 * @details  this file ###
 * @author  
 * @version  1.0.0
 * @date     3/24/2017
 **************************************************************************/

#ifndef _CORE_IPC_TYPE_H_
#define _CORE_IPC_TYPE_H_
	 	 
#define IPC_USE_RTOS

typedef void* PrioQueueHandle;
	 
//#include "osif.h" 

#ifdef IPC_PLTF_IS_LINUX
    #include "ipcTypeLinux.h"
#endif    
	 
#ifdef IPC_PLTF_IS_SH2
    #include "ipcTypeSH2.h"
#endif
	 
#ifdef IPC_PLTF_IS_QNX
    #include "ipcTypeQnx.h"
#endif

//#include "UartIpc_if.h"
	 
	 
	 
#endif

