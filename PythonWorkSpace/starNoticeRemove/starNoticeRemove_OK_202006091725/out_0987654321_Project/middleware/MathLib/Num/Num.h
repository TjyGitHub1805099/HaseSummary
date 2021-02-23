/**
*@file  Num.h
*@brief Describe here.
*@date    2018-03-23 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-23  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _NUM_H_
#define _NUM_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

typedef uint32 NumType;
#define NUM_E_NOT_FOND 0XFF

extern uint8 Num_CountSettedBit(NumType num);
extern NumType Num_GetLowestSettedBit(NumType num);
extern NumType Num_GetHighstSettedBit(NumType num);
extern uint8 Num_GetLowestSettedBitIndex(NumType num);
extern uint8 Num_GetHighstSettedBitIndex(NumType num);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_NUM_H_
/*=========================== END OF FILE: Num.h ===========================*/
