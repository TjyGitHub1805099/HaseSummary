/********************************************************************
File name: VirtualSig.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-01-22 18:20:35
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _VIRTUALSIG_H_
#define _VIRTUALSIG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	
#include "Std_Types.h"
#include "VirtualSig_Cfg.h"

typedef enum
{
	VIRTUALSIG_F_ALWAYS,
	VIRTUALSIG_F_DIFFER
}VirtualSig_FilterEType;

typedef struct
{
	boolean Type;
	uint16 TaskId;
	uint32 SpecValue;
} VirtualSig_RegTaskInfoType;

typedef struct
{
    uint8 Filter;
    uint16 RegMaxNum;
	VirtualSig_RegTaskInfoType *RegTaskInfoPtr;
} VirtualSig_CfgInfoType;

typedef struct
{
	uint32 Status;
	uint16 RegCount;
	VirtualSig_RegTaskInfoType *RegTaskInfo;
} VirtualSig_RuntimeInfoType;

/**
*@par Service ID[hex]
*0X0
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] moduleOffset
*Module offset value signed by System Signal module.
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Call this function to initialize the module.
*You do not need to call this function after you registered to system signal module.
*/
extern void VirtualSig_Init(uint16 moduleOffset);

/**
*@par Service ID[hex]
*0X1
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] taskId
*Task identifier which want to receive this signal.
*
*@param[inout] None
*
*@param[out] None
*
*@retval E_OK: Subscribe successed.
*@retval E_NOT_OK: Subscribe failed due to max number limited or module not initialize.
*
*Call this function to subscribe if you want to receive this signal event in every change.
*/
extern boolean VirtualSig_Subscribe(uint16 signalId, uint16 taskId);

/**
*@par Service ID[hex]
*0X2
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] taskId
*Task identifier which want to receive this signal.
*
*@param[in] specValue
*Specific value which want to receive.
*
*@param[inout] None
*
*@param[out] None
*
*@retval E_OK: Subscribe successed.
*@retval E_NOT_OK: Subscribe failed due to max number limited or module not initialize.
*
*Call this function to subscribe if you want to receive this signal event when it equal specified.
*/
extern boolean VirtualSig_SubSpecValue(uint16 signalId, uint16 taskId, uint32 specValue);

/**
*@par Service ID[hex]
*0X3
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[inout] None
*
*@param[out] status
*Only the retval equal E_OK,then this status value is right.
*
*@retval E_OK: Get status successed.
*@retval E_NOT_OK: Get status failed due to max number limited or module not initialize.
*
*Call this function to get signal status.
*/
extern boolean VirtualSig_GetStatus(uint16 signalId,uint32* status);

/**
*@par Service ID[hex]
*0X4
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] signalId
*Signal identifier which you want to Subscribe/receive.
*
*@param[in] status
*Signal status value.
*
*@param[inout] None
*
*@param[out] status
*
*@retval E_OK: Set status success.
*@retval E_NOT_OK: Set status failed due to not initialize or status not changed.

*@retval None
*
*Call this function to set signal status.
*/
extern boolean VirtualSig_SetStatus(uint16 signalId, uint32 status, boolean fromISR);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: VirtualSig.h ===========================*/

