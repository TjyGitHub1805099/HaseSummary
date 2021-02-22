/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: BinSig_Reverse_Cfg.h
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
#ifndef _BINSIG_REVERSE_CFG_H_
#define _BINSIG_REVERSE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "BoardDefines.h"

#define BINSIG_REVERSE_MODULE_ACTIVE STD_ON

#if(STD_ON == BINSIG_REVERSE_MODULE_ACTIVE)
#define BINSIG_REVERSE_DEBUG_TRACE STD_ON

#define BINSIG_REVERSE_ON STD_HIGH
#define BINSIG_REVERSE_OFF STD_LOW

#define BINSIG_REVERSE_FILTER_TIME (10U)

#define BINSIG_REVERSE_GET_PORT_VALUE Port_GetPinLevel(REVERSE_DET)
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: BinSig_Reverse_Cfg.h ===========================*/