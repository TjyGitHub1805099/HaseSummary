/*
* File last modified by:   dukunlin
* File last modified time: 2018-05-09 10:39:17
*/

/**
* @file      BinarySearch.c
* @author    Stephen Du
* @date:     2018-05-09 09:48:13
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-05-09 09:48:13  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "BinarySearch.h"

/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/

uint16 BinSearch_EquMatch(void* bufPtr, uint16 bufLen, BinSearchBufType bufType, uint32 value)
{
    uint8* l_8bufPtr = (uint8*)bufPtr;
    uint16* l_16bufPtr = (uint16*)bufPtr;
    uint32* l_32bufPtr = (uint32*)bufPtr;
    uint64* l_64bufPtr = (uint64*)bufPtr;
    uint16 l_lowIndex = 0u;
    uint16 l_highIndex = bufLen - 1;
    uint16 l_midIndex = 0u;
    uint16 l_returnValue = BINSEARCH_ERROR;
    uint8 l_seqType = 0u;

    if ((NULL_PTR != bufPtr) && (bufLen > 0))
    {
        switch (bufType)
        {
        case BSEARCH_BUFTYPE_UINT16:
            l_8bufPtr = l_8bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_16bufPtr[l_lowIndex] < l_16bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (bufLen < 3)
                {
                    if (value == l_16bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_16bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_16bufPtr[l_lowIndex] <= value) && (value <= l_16bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_16bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_16bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_16bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value > l_16bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (bufLen < 3)
                {
                    if (value == l_16bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_16bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_16bufPtr[l_lowIndex] >= value) && (value >= l_16bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_16bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_16bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_16bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value < l_16bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;

        case BSEARCH_BUFTYPE_UINT32:
            l_8bufPtr = l_8bufPtr;
            l_16bufPtr = l_16bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_32bufPtr[l_lowIndex] < l_32bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (bufLen < 3)
                {
                    if (value == l_32bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_32bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_32bufPtr[l_lowIndex] <= value) && (value <= l_32bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_32bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_32bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_32bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value > l_32bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (bufLen < 3)
                {
                    if (value == l_32bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_32bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_32bufPtr[l_lowIndex] >= value) && (value >= l_32bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_32bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_32bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_32bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value < l_32bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;

        case BSEARCH_BUFTYPE_UINT64:
            l_8bufPtr = l_8bufPtr;
            l_16bufPtr = l_16bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_seqType = (l_64bufPtr[l_lowIndex] < l_64bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (bufLen < 3)
                {
                    if (value == l_64bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_64bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_64bufPtr[l_lowIndex] <= value) && (value <= l_64bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_64bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_64bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_64bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value > l_64bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (bufLen < 3)
                {
                    if (value == l_64bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_64bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_64bufPtr[l_lowIndex] >= value) && (value >= l_64bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_64bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_64bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_64bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value < l_64bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;

        default:
            l_16bufPtr = l_16bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_8bufPtr[l_lowIndex] < l_8bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (bufLen < 3)
                {
                    if (value == l_8bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_8bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_8bufPtr[l_lowIndex] <= value) && (value <= l_8bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_8bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_8bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_8bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value > l_8bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (bufLen < 3)
                {
                    if (value == l_8bufPtr[l_lowIndex])
                    {
                        l_returnValue = l_lowIndex;
                    }
                    else if (value == l_8bufPtr[l_highIndex])
                    {
                        l_returnValue = l_highIndex;
                    }
                }
                else if ((l_8bufPtr[l_lowIndex] >= value) && (value >= l_8bufPtr[l_highIndex]))
                {
                    while (l_lowIndex <= (l_highIndex - 2))
                    {
                        if (value == l_8bufPtr[l_lowIndex])
                        {
                            l_returnValue = l_lowIndex;
                            break;
                        }
                        else if (value == l_8bufPtr[l_highIndex])
                        {
                            l_returnValue = l_highIndex;
                            break;
                        }
                        else
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if (value == l_8bufPtr[l_midIndex])
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            else if (value < l_8bufPtr[l_midIndex])
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    return (l_returnValue);
}

uint16 BinSearch_Interval(void* bufPtr, uint16 bufLen, BinSearchBufType bufType, uint32 value)
{
    uint8* l_8bufPtr = (uint8*)bufPtr;
    uint16* l_16bufPtr = (uint16*)bufPtr;
    uint32* l_32bufPtr = (uint32*)bufPtr;
    uint64* l_64bufPtr = (uint64*)bufPtr;
    uint16 l_lowIndex = 0u;
    uint16 l_highIndex = bufLen - 1;
    uint16 l_midIndex = 0u;
    uint16 l_returnValue = BINSEARCH_ERROR;
    uint8 l_seqType = 0u;

    if ((NULL_PTR != bufPtr) && (bufLen > 0))
    {
        switch (bufType)
        {
        case BSEARCH_BUFTYPE_UINT16:
            l_8bufPtr = l_8bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_16bufPtr[l_lowIndex] < l_16bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (value <= l_16bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value > l_16bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_16bufPtr[l_midIndex] < value) && (value <= l_16bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_16bufPtr[l_midIndex - 1] < value) && (value <= l_16bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_16bufPtr[l_midIndex] < value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (value >= l_16bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value < l_16bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_16bufPtr[l_midIndex] > value) && (value >= l_16bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_16bufPtr[l_midIndex - 1] > value) && (value >= l_16bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_16bufPtr[l_midIndex] > value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }

            break;

        case BSEARCH_BUFTYPE_UINT32:
            l_8bufPtr = l_8bufPtr;
            l_16bufPtr = l_16bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_32bufPtr[l_lowIndex] < l_32bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (value <= l_32bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value > l_32bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_32bufPtr[l_midIndex] < value) && (value <= l_32bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_32bufPtr[l_midIndex - 1] < value) && (value <= l_32bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_32bufPtr[l_midIndex] < value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (value >= l_32bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value < l_32bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_32bufPtr[l_midIndex] > value) && (value >= l_32bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_32bufPtr[l_midIndex - 1] > value) && (value >= l_32bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_32bufPtr[l_midIndex] > value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;

        case BSEARCH_BUFTYPE_UINT64:
            l_8bufPtr = l_8bufPtr;
            l_16bufPtr = l_16bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_seqType = (l_64bufPtr[l_lowIndex] < l_64bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (value <= l_64bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value > l_64bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_64bufPtr[l_midIndex] < value) && (value <= l_64bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_64bufPtr[l_midIndex - 1] < value) && (value <= l_64bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_64bufPtr[l_midIndex] < value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (value >= l_64bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value < l_64bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_64bufPtr[l_midIndex] > value) && (value >= l_64bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_64bufPtr[l_midIndex - 1] > value) && (value >= l_64bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_64bufPtr[l_midIndex] > value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;

        default:
            l_16bufPtr = l_16bufPtr;
            l_32bufPtr = l_32bufPtr;
            l_64bufPtr = l_64bufPtr;
            l_seqType = (l_8bufPtr[l_lowIndex] < l_8bufPtr[l_highIndex]) ? 0 : 1;
            if (0 == l_seqType)
            {
                if (value <= l_8bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value > l_8bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_8bufPtr[l_midIndex] < value) && (value <= l_8bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_8bufPtr[l_midIndex - 1] < value) && (value <= l_8bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_8bufPtr[l_midIndex] < value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            else
            {
                if (value >= l_8bufPtr[l_lowIndex])
                {
                    l_returnValue = 0;
                }
                else if (value < l_8bufPtr[l_highIndex])
                {
                    l_returnValue = l_highIndex + 1;
                }
                else
                {
                    if (3 > bufLen)
                    {
                        l_returnValue = l_highIndex;
                    }
                    else
                    {
                        while (l_lowIndex <= l_highIndex)
                        {
                            l_midIndex = (l_lowIndex + l_highIndex) >> 1;
                            if ((l_8bufPtr[l_midIndex] > value) && (value >= l_8bufPtr[l_midIndex + 1]))
                            {
                                l_returnValue = l_midIndex + 1;
                                break;
                            }
                            else if ((l_8bufPtr[l_midIndex - 1] > value) && (value >= l_8bufPtr[l_midIndex]))
                            {
                                l_returnValue = l_midIndex;
                                break;
                            }
                            if (l_8bufPtr[l_midIndex] > value)
                            {
                                l_lowIndex = l_midIndex;
                            }
                            else
                            {
                                l_highIndex = l_midIndex;
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    return (l_returnValue);
}

/*============================= END OF FILE: BinarySearch.c ============================*/
