/**
*@file  FlagGroup.c
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
#include <string.h>
#include "FlagGroup.h"
#include "Num.h"
#include "Crc.h"
#include "System_Portable.h"

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
#include <stdio.h>
#endif

/*
#define FLAGGROUP_BUF_DEF(size,name) #if((size)<=64)\
uint8 name[((size)-1)/8+8]\
#elif((size)<=256)\
uint16 name[((size)-1)/16+6]\
#else\
uint32 name[((size)-1)/32+5]\
#endif
*/

/*
*Memory map: To improve effiency, Crc only calc size.
*_______________________________________________________________________
*|  CCR   | AllSet | AnySet | size + count  | data0  | data1  | ...... |
*| uint32 | uint32 | uint32 | uint16+uint16 | uint32 | uint32 | uint32 |
************************************************************************
*/

uint32* FlagGroup_Init(uint32* bufPtr, uint16 size)
{
    uint32* l_returnValue = NULL_PTR;
    uint32* l_dataPtr = bufPtr + 4;
    uint8 l_byteLoop = 0;
    uint8 l_byteSettedBit = 0;
    uint16 l_sumBits = 0;
    uint16 l_maxBit = 0;
    uint32 l_crc = 0;
    uint32 l_maxByteCont = 0, l_maxByteMask = 0;
    uint8 l_maxByte = 0;
    boolean l_resetBuf = FALSE;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_Init(size:%d,buf:%d)\n", size, bufPtr);
#endif

    if ((0 != size) && (1024 >= size) && (NULL_PTR != bufPtr))
    {
        l_maxBit = (uint16)((bufPtr[3] & 0xFFFF0000) >> 16);
        l_crc = Crc_CalculateCRC32((uint8*)(&l_maxBit), 2, CRC_STEP_SINGLE, 0);
        if ((l_crc != *bufPtr) || (size != l_maxBit))
        {
            l_resetBuf = TRUE;
        }
        else
        {
            l_maxByte = ((size - 1) >> 5) + 1;
            l_maxByteCont = size & 31;
            l_maxByteMask = (1 << l_maxByteCont) - 1;
            if (0 != l_maxByteMask)
            {
                l_dataPtr[l_maxByte - 1] &= l_maxByteMask;
            }
            for (l_byteLoop = 0; l_byteLoop < l_maxByte; l_byteLoop++)
            {
                l_byteSettedBit = Num_CountSettedBit(l_dataPtr[l_byteLoop]);
                if ((l_byteLoop == (l_maxByte - 1)) && (0 != l_maxByteMask))
                {
                    if (((0 == l_byteSettedBit) && (0 != (bufPtr[2] & (1 << l_byteLoop))))
                            || ((0 != l_byteSettedBit) && (0 == (bufPtr[2] & (1 << l_byteLoop))))
                            || ((l_maxByteCont == l_byteSettedBit) && (0 == (bufPtr[1] & (1 << l_byteLoop))))
                            || ((l_maxByteCont != l_byteSettedBit) && (0 != (bufPtr[1] & (1 << l_byteLoop)))))
                    {
                        break;
                    }
                }
                else
                {
                    if (((0 == l_byteSettedBit) && (0 != (bufPtr[2] & (1 << l_byteLoop))))
                            || ((0 != l_byteSettedBit) && (0 == (bufPtr[2] & (1 << l_byteLoop))))
                            || ((32 == l_byteSettedBit) && (0 == (bufPtr[1] & (1 << l_byteLoop))))
                            || ((32 != l_byteSettedBit) && (0 != (bufPtr[1] & (1 << l_byteLoop)))))
                    {
                        break;
                    }
                }
                l_sumBits += l_byteSettedBit;
            }
            if ((l_byteLoop != l_maxByte) || (l_sumBits != (bufPtr[3] & 0xFFFF)))
            {
                l_resetBuf = TRUE;
            }
        }
        if (TRUE == l_resetBuf)
        {
            l_maxBit = size;
            l_crc = Crc_CalculateCRC32((uint8*)(&l_maxBit), 2, CRC_STEP_SINGLE, 0);
            EnterCriticalCode();
            memset(bufPtr, 0, ((((size) - 1) >> 5) + 5) << 2);
            bufPtr[0] = l_crc;
            bufPtr[1] = 0;
            bufPtr[2] = 0;
            bufPtr[3] = l_maxBit << 16;
            ExitCriticalCode();
        }
        l_returnValue = &bufPtr[4];
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d all:%d any:%d count%d\n", l_returnValue, bufPtr[1], bufPtr[2], bufPtr[3]);
#endif
    return (l_returnValue);
}

boolean FlagGroup_ClearAll(uint32* handlePtr)
{
    boolean l_returnValue = E_NOT_OK;
    uint8 l_byteSize = 0;
    uint16 l_size = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_ClearAll(hand:%d)\n", handlePtr);
#endif
    if (NULL_PTR != handlePtr)
    {
        EnterCriticalCode();
        l_size = (handlePtr[-1] & 0xFFFF0000) >> 16;
        l_byteSize = ((l_size - 1) >> 5) + 1;
        memset(handlePtr, 0, l_byteSize << 2);
        *(handlePtr - 1) &= 0xFFFF0000;
        *(handlePtr - 2) = 0;
        *(handlePtr - 3) = 0;
        ExitCriticalCode();
        l_returnValue = E_OK;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d\n", l_returnValue);
#endif
    return (l_returnValue);
}

uint16 FlagGroup_GetSize(const uint32* handlePtr)
{
    uint16 l_returnValue = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_GetSize(hand:%d)\n", handlePtr);
#endif
    if (NULL_PTR != handlePtr)
    {
        l_returnValue = (handlePtr[-1] & 0xFFFF0000) >> 16;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d\n", l_returnValue);
#endif
    return (l_returnValue);
}

boolean FlagGroup_SetFlagState(uint32* handlePtr, uint16 index, boolean trueFalse)
{
    boolean l_returnValue = E_NOT_OK;
    uint8 l_Element = 0, l_ElementBit = 0;
    uint8 l_maxByte = 0;
    uint16 l_maxBit = 0;
    uint32 l_maxByteCont = 0, l_maxByteMask = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_SetFlagState(hand:%d,index:%d,truefalse:%d)\n", handlePtr, index, trueFalse);
#endif
    if ((NULL_PTR != handlePtr) && (index < ((handlePtr[-1] & 0xFFFF0000) >> 16)))
    {
        l_Element = index >> 5;
        l_ElementBit  = index & 31;

        EnterCriticalCode();
        l_maxBit = (handlePtr[-1] & 0xFFFF0000) >> 16;
        l_maxByte = ((l_maxBit - 1) >> 5) + 1;
        l_maxByteCont = l_maxBit & 31;
        l_maxByteMask = (1 << l_maxByteCont) - 1;
        if (0 != l_maxByteMask)
        {
            handlePtr[l_maxByte - 1] &= l_maxByteMask;
        }
        if (TRUE == trueFalse)
        {
            if (!(handlePtr[l_Element] & (0x01 << l_ElementBit)))
            {
                handlePtr[l_Element] |= (0x01 << l_ElementBit);
                handlePtr[-1] += 1;
            }
            handlePtr[-2] |= (0x01 << l_Element);
            if ((l_Element == (l_maxByte - 1)) && (0 != l_maxByteMask))
            {
                if (l_maxByteMask == handlePtr[l_Element])
                {
                    handlePtr[-3] |= (0x01 << l_Element);
                }
            }
            else
            {
                if (0xFFFFFFFF == handlePtr[l_Element])
                {
                    handlePtr[-3] |= (0x01 << l_Element);
                }
            }
        }
        else
        {
            if (handlePtr[l_Element] & (0x01 << l_ElementBit))
            {
                handlePtr[l_Element] &= (~(0x01 << l_ElementBit));
                handlePtr[-1] -= 1;
            }
            if (0 == handlePtr[l_Element])
            {
                handlePtr[-2] &= (~(0x01 << l_Element));
            }
            handlePtr[-3] &= (~(0x01 << l_Element));
        }
        ExitCriticalCode();
        l_returnValue = E_OK;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d\n", l_returnValue);
#endif
    return (l_returnValue);
}

boolean FlagGroup_GetFlagState(const uint32* handlePtr, uint16 index, boolean* trueFalse)
{
    boolean l_returnValue = E_NOT_OK;
    uint8 l_Element = 0, l_ElementBit = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_GetFlagState(hand:%d,index:%d)\n", handlePtr, index);
#endif
    if ((NULL_PTR != handlePtr) && (index < ((handlePtr[-1] & 0xFFFF0000) >> 16)))
    {
        l_Element = index >> 5;
        if (0 == (handlePtr[-2] & (0x01 << l_Element)))
        {
            *trueFalse = FALSE;
        }
        else
        {
            l_ElementBit  = index & 31;
            if (0 == (handlePtr[l_Element] & (0x01 << l_ElementBit)))
            {
                *trueFalse = FALSE;
            }
            else
            {
                *trueFalse = TRUE;
            }
        }
        l_returnValue = E_OK;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d,truefalse:%d\n", l_returnValue, *trueFalse);
#endif
    return (l_returnValue);
}

boolean FlagGroup_CountClearedFlag(const uint32* handlePtr, uint16* num)
{
    boolean l_returnValue = E_NOT_OK;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_CountClearedFlag(hand:%d)\n", handlePtr);
#endif
    if (NULL_PTR != handlePtr)
    {
        *num = ((handlePtr[-1] & 0xFFFF0000) >> 16) - (handlePtr[-1] & 0xFFFF);
        l_returnValue = E_OK;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d,num:%d\n", l_returnValue, *num);
#endif
    return (l_returnValue);
}

boolean FlagGroup_CountSettedFlag(const uint32* handlePtr, uint16* num)
{
    boolean l_returnValue = E_NOT_OK;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_CountSettedFlag(hand:%d)\n", handlePtr);
#endif
    if (NULL_PTR != handlePtr)
    {
        *num = handlePtr[-1] & 0xFFFF;
        l_returnValue = E_OK;
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d,num:%d\n", l_returnValue, *num);
#endif
    return (l_returnValue);
}

boolean FlagGroup_GetSettedFlag(const uint32* handlePtr, FlagGroupModeEType mode, uint16* index)
{
    boolean l_returnValue = E_NOT_OK;
    uint8 l_Element = 0, l_ElementBit = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_GetSettedFlag(hand:%d,mode:%d)\n", handlePtr, mode);
#endif
    if (NULL_PTR != handlePtr)
    {
        if (FLAGGROUP_MODE_LOWEST == mode)
        {
            l_Element = Num_GetLowestSettedBitIndex(handlePtr[-2]);
            if (NUM_E_NOT_FOND != l_Element)
            {
                l_ElementBit = Num_GetLowestSettedBitIndex(handlePtr[l_Element]);
                if (((handlePtr[-1] & 0xFFFF0000) >> 16) > ((l_Element << 5) + l_ElementBit))
                {
                    *index = (l_Element << 5) + l_ElementBit;
                    l_returnValue = E_OK;
                }
            }
        }
        else
        {
            l_Element = Num_GetHighstSettedBitIndex(handlePtr[-2]);
            if (NUM_E_NOT_FOND != l_Element)
            {
                l_ElementBit = Num_GetHighstSettedBitIndex(handlePtr[l_Element]);
                if (((handlePtr[-1] & 0xFFFF0000) >> 16) > ((l_Element << 5) + l_ElementBit))
                {
                    *index = (l_Element << 5) + l_ElementBit;
                    l_returnValue = E_OK;
                }
            }
        }
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d,index:%d\n", l_returnValue, *index);
#endif
    return (l_returnValue);
}

boolean FlagGroup_GetClearedFlag(const uint32* handlePtr, FlagGroupModeEType mode, uint16* index)
{
    boolean l_returnValue = E_NOT_OK;
    uint8 l_Element = 0, l_ElementBit = 0;
    uint8 l_maxByte = 0;
    uint16 l_maxBit = 0;
    uint32 l_maxByteCont = 0, l_maxByteMask = 0;

#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\nFlagGroup_GetClearedFlag(hand:%d,mode:%d)\n", handlePtr, mode);
#endif
    if (NULL_PTR != handlePtr)
    {
        l_maxBit = (handlePtr[-1] & 0xFFFF0000) >> 16;
        if (FLAGGROUP_MODE_LOWEST == mode)
        {
            l_Element = Num_GetLowestSettedBitIndex(~handlePtr[-3]);
            if (NUM_E_NOT_FOND != l_Element)
            {
                l_ElementBit = Num_GetLowestSettedBitIndex(~handlePtr[l_Element]);
                if (l_maxBit > ((l_Element << 5) + l_ElementBit))
                {
                    *index = (l_Element << 5) + l_ElementBit;
                    l_returnValue = E_OK;
                }
            }
        }
        else
        {
            l_maxByte = ((l_maxBit - 1) >> 5) + 1;
            l_maxByteCont = l_maxBit & 31;
            l_maxByteMask = (1 << l_maxByteCont) - 1;

            l_Element = Num_GetHighstSettedBitIndex(~(~((1 << l_maxByte) - 1) | handlePtr[-3]));
            if (NUM_E_NOT_FOND != l_Element)
            {
                if ((l_Element == (l_maxByte - 1)) && (0 != l_maxByteMask))
                {
                    l_ElementBit = Num_GetHighstSettedBitIndex(~(handlePtr[l_Element] | (~l_maxByteMask)));
                }
                else
                {
                    l_ElementBit = Num_GetHighstSettedBitIndex(~(handlePtr[l_Element]));
                }
                if (l_maxBit > ((l_Element << 5) + l_ElementBit))
                {
                    *index = (l_Element << 5) + l_ElementBit;
                    l_returnValue = E_OK;
                }
            }
        }
    }
#if (STD_ON==FLAGGROUP_DEBUG_TRACE)
    printf("\treturn:%d,index:%d\n", l_returnValue, *index);
#endif
    return (l_returnValue);
}

/*=========================== END OF FILE: FlagGroup.c ===========================*/
