/*
* File last modified by:   dukunlin
* File last modified time: 2018-04-28 15:00:27
*/

/**
* @file      SimFlag.h
* @author    Stephen Du
* @date:     2018-04-28 15:00:27
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-04-28 15:00:27  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _SIMFLAG_H_
#define _SIMFLAG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/
#define SIMFLAG_ALLFLAGS 0XFFFFU
#define SIMFLAG_ERROR 0XFF

extern void SimFlag_Init(uint8 *bufPtr, uint16 maxBits, boolean initValue);
extern void SimFlag_SetFlagState(uint8 *bufPtr, uint16 index, boolean trueFalse);
extern boolean SimFlag_GetFlagState(uint8 *bufPtr, uint16 index);

/**
 * Use this macro to define buffer quickly.
 */
#define SimFlag_BufDef(name,size) uint8 name[(((size)-1)>>3)+3]


/**
 * Use this macro to get buffer name.
 */
//#define SimFlag_GetBufName(name) SimFlag_Buf_##name

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_SIMFLAG_H_
/*============================= END OF FILE: SimFlag.h ============================*/
