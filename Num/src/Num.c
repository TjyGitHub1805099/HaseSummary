/**
*@file  Num.c
*@brief Describe here.
*@date    2018-03-23 15:29:14
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

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
#include "Num.h"

uint8 Num_CountSettedBit(NumType num)
{
    uint8 l_count = 0u;

    for (l_count = 0u; num; num = num & (num - 1))
    {
        l_count++;
    }
    return l_count;
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
NumType Num_GetLowestSettedBit(NumType num)
{
    NumType l_returnValue = NUM_E_NOT_FOND;
    if (0 != num)
    {
        l_returnValue = num & ((~num) + 1);
    }
    return (l_returnValue);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
NumType Num_GetHighstSettedBit(NumType num)
{
    NumType l_returnValue = NUM_E_NOT_FOND;

    if (0 != num)
    {
        while (num)
        {
            l_returnValue = num;
            num = num & (num - 1);
        }
    }
    return l_returnValue;
}
/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 Num_GetLowestSettedBitIndex(NumType num)
{
    uint8 l_returnValue = NUM_E_NOT_FOND;

    if (0 != num)
    {
        //l_returnValue = Num_CountSettedBit(num ^ (num - 1));//start from 1
        // l_returnValue = Num_CountSettedBit((num & (~(num - 1))) - 1);//start from 0
        //l_returnValue = Num_CountSettedBit((num & ((~num) + 1)) - 1);//start from 0
        l_returnValue = Num_CountSettedBit(num ^ (num - 1)) - 1;//start from 0
    }
    return (l_returnValue);
}

/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
uint8 Num_GetHighstSettedBitIndex(NumType num)
{
    uint8 l_returnValue = NUM_E_NOT_FOND;
    NumType l_tempValue = 0U;

    if (0 != num)
    {
        l_tempValue = Num_GetHighstSettedBit(num);
        if (NUM_E_NOT_FOND != l_tempValue)
        {
            l_returnValue = Num_CountSettedBit(l_tempValue - 1);//start from 0
        }
    }
    return (l_returnValue);
}
/*=========================== END OF FILE: Num.c ===========================*/
