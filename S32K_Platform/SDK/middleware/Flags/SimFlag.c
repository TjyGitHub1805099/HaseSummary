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
#include "Std_Types.h"
#include "System_Portable.h"
/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/
void SimFlag_SetFlagState(uint8 *bufPtr, uint16 index, boolean trueFalse)
{
    uint16 l_byteIndex = 0;
    uint16 l_bitIndex = 0;

    if (NULL_PTR != bufPtr)
    {
        EnterCriticalCode();
        l_byteIndex = (index >> 3);
        l_bitIndex  = (index & 7U);
        if (TRUE == trueFalse)
        {
            *(bufPtr + l_byteIndex) |= (uint8)(1 << l_bitIndex);
        }
        else
        {
            *(bufPtr + l_byteIndex) &= (uint8)(~(1 << l_bitIndex));
        }
        ExitCriticalCode();
    }
}

boolean SimFlag_GetFlagState(uint8 *bufPtr, uint16 index)
{
    uint16 l_byteIndex = 0;
    uint16 l_bitIndex = 0;
    boolean l_returnValue = 0;

    if (NULL_PTR != bufPtr)
    {
        EnterCriticalCode();
        l_byteIndex = (index >> 3);
        l_bitIndex  = (index & 7U);
        l_returnValue = (*(bufPtr + l_byteIndex) >> l_bitIndex) & 0x01;
        ExitCriticalCode();
    }
    return (l_returnValue);
}

/*============================= END OF FILE: SimFlag.c ============================*/
