/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: BinSig_Ign_Cfg.h
Author: Stephen Du
Version: V2.0
Timestamp: 2018-10-23 10:38:12
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
#ifndef _BINSIG_IGN_CFG_H_
#define _BINSIG_IGN_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "BoardDefines.h"

#define BINSIG_IGN_MODULE_ACTIVE STD_ON

#if(STD_ON == BINSIG_IGN_MODULE_ACTIVE)
#define BINSIG_IGN_DEBUG_TRACE STD_ON

#define BINSIG_IGN_ON STD_LOW
#define BINSIG_IGN_OFF STD_HIGH

#define BINSIG_IGN_FILTER_TIME (10U)

#define BINSIG_IGN_GET_PORT_VALUE Port_GetPinLevel(IGN_DET)
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*=========================== END OF FILE: BinSig_Ign_Cfg.h ===========================*/
