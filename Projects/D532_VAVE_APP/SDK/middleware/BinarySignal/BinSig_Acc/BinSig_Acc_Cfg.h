/*!Warning!:Please don't modify this file,this is generated by tools!*/
/********************************************************************
File name: BinSig_Acc_Cfg.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-03-02 16:55:47
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
#ifndef _BINSIG_ACC_CFG_H_
#define _BINSIG_ACC_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "pin_wrapper_if.h"

#define BINSIG_ACC_MODULE_ACTIVE STD_ON

#if(STD_ON == BINSIG_ACC_MODULE_ACTIVE)
#define BINSIG_ACC_DEBUG_TRACE STD_OFF

#define BINSIG_ACC_ON STD_LOW
#define BINSIG_ACC_OFF STD_HIGH

#define BINSIG_ACC_FILTER_TIME (10U)

//#define BINSIG_ACC_GET_PORT_VALUE    PORT_READ_PIN_VALUE(PORTGP_ACC_ON_OR_OFF,PINNUM_ACC_ON_OR_OFF)
#define BINSIG_ACC_GET_PORT_VALUE		GetPinValue_ACC()
#endif
#ifdef __cplusplus
}
#endif // __cplusplus

#endif