/*
* File last modified by:   dukunlin
* File last modified time: 2018-05-09 10:22:40
*/

/**
* @file      SimFlag.c
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
#include "string.h"
#include "Std_Types.h"
#include "SimFlag.h"
#include "System_Portable.h"
/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/
void SimFlag_Init(uint8 *bufPtr, uint16 maxBits, boolean initValue)
{
	*(uint16 *)bufPtr=(maxBits < SIMFLAG_ALLFLAGS) ? maxBits : 0;
	memset((uint8*)&bufPtr[2],0XFF*initValue,((maxBits-1)>>3)+1);
}

void SimFlag_SetFlagState(uint8 *bufPtr, uint16 index, boolean trueFalse)
{
    uint16 l_byteIndex = 0;
    uint16 l_bitIndex = 0;

    if (NULL_PTR != bufPtr){
		if(index == SIMFLAG_ALLFLAGS){
			EnterCriticalCode();
			memset((uint8*)&bufPtr[2],trueFalse,((*(uint16*)(&bufPtr[0])-1)>>3)+1);
			ExitCriticalCode();
		} else if (index <=*(uint16*)(&bufPtr[0])){
			EnterCriticalCode();
	        l_byteIndex = (index >> 3) + 2;
	        l_bitIndex  = (index & 7U);
	        if (TRUE == trueFalse){
	            *(bufPtr + l_byteIndex) |= (uint8)(1 << l_bitIndex);
	        } else {
	            *(bufPtr + l_byteIndex) &= (uint8)(~(1 << l_bitIndex));
	        }
	        ExitCriticalCode();
		} else {
			// do nothing
		}
    }
}

uint8 SimFlag_GetFlagState(uint8 *bufPtr, uint16 index)
{
    uint16 l_byteIndex = 0;
    uint16 l_bitIndex = 0;
    uint8 l_returnValue = SIMFLAG_ERROR;

    if ((NULL_PTR != bufPtr)&&(index <=*(uint16*)(&bufPtr[0])))
    {
        EnterCriticalCode();
        l_byteIndex = (index >> 3)+2;
        l_bitIndex  = (index & 7U);
        l_returnValue = (*(bufPtr + l_byteIndex) >> l_bitIndex) & 0x01;
        ExitCriticalCode();
    }
    return (l_returnValue);
}

/*============================= END OF FILE: SimFlag.c ============================*/
