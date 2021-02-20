/**
* @file:    CanApp_Cfg.h
* @brief:   Add your description here for this file.
* @author:  zhangyi
* @date:    2018-09-11 10:40:16
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  zhangyi  2018-09-11 10:40:16  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CANAPP_CFG_H_
#define _CANAPP_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "Com_Cfg.h"
#include "CanApp_Cfg.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/

/*------------------------------ END: USER INCLUDE ------------------------------*/
#define CanAppTxInitValue    0
#define CanAppTxTirggerValue 1
#define CanAppTxDefaultValue 2
#define HANDLE_RECVMSG_TEMPSIZE 128 //15

#define CanApp_10MS_DETECTION 1
#define CanApp_50MS_DETECTION 5
#define CanApp_100MS_DETECTION 10

typedef union
{
  float32 fdata;
  unsigned long ldata;
}FloatLongType;

typedef struct {
	uint8 flag;
	uint8 offset;
	uint8 num;
	uint8 lenght;
	uint32 sysTick;
	uint8 send_type;
	uint16 buf[50];
}CanApp_Tx_TextNum;


extern void Byte_to_Float(float32 *f, uint8 *byte, uint32 startbit);
extern uint32 System_GetTickValue(void);





#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CANAPP_CFG_H_
/*============================= END OF FILE: CanApp_Cfg.h ============================*/
