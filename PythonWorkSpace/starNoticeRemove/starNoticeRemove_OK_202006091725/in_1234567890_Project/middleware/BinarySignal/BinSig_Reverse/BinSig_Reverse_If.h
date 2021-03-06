/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: BinSig_Reverse_If.h
Author: Stephen Du
Version: V2.0
Timestamp: 2019-01-17 17:19:13
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
#ifndef _BINSIG_REVERSE_IF_H_
#define _BINSIG_REVERSE_IF_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

#define BINSIG_REVERSE_INACTIVE (0U)
#define BINSIG_REVERSE_ACTIVE (1U)

/**
*@par Service ID[hex]
*0X0
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Reentrant
*@param[in] None
*
*@param[inout] None
*
*@param[out] None
*
*@retval None
*
*Scanf the signal periodically and do the debouncer.
*/
extern void BinSig_Reverse_Scan(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: BinSig_Reverse_If.h ===========================*/
