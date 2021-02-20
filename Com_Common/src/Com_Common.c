/*
* File last modified by:   dukunlin
* File last modified time: 2018-05-14 11:55:19
*/

/**
* @file      Com_Common.c
* @author    Stephen Du
* @date:     2018-04-28 12:02:11
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-04-28 12:02:11  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "ComStack_Types.h"

/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/
extern const Com_SigConfType Com_SigConfInfo[];
extern Com_SigRunType Com_SigRunInfo[];

const static uint8 ByteHighBitsMask[8] =
{
    1u, 3u, 7u, 15u, 31u, 63u, 127u, 255u
};

static boolean Com_GetIntelLittleTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 firstByteActiveBit = 0u;
    uint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByte = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) >> 3;
    stopByteBit = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) & 7;
    if (startByte == stopByte)
    {
        getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[stopByteBit]) >> (startByteBit);
    }
    else
    {
        firstByteActiveBit = 8 - startByteBit;
        for (loop = startByte; loop <= stopByte; loop++)
        {
            if (loop == startByte)
            {
                getValue = *(SrcBuffer + loop) >> (startByteBit);
            }
            else if (loop == stopByte)
            {
                getValue +=  (*(SrcBuffer + loop) & ByteHighBitsMask[stopByteBit]) << (((stopByte - startByte - 1) << 3) + firstByteActiveBit);
            }
            else
            {
                getValue += *(SrcBuffer + loop) << (((loop - startByte - 1) << 3) + firstByteActiveBit);
            }
        }
    }
    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1)))
        {
            getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
        }
    }
    *((uint32 *)GetValue) = (uint32)getValue;
    return (E_OK);
}

static boolean Com_SetIntelLittleTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 firstByteActiveBit = 0u;
    uint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByte = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) >> 3;
    stopByteBit = (Com_SigConfInfo[SignalId].StartBit + Com_SigConfInfo[SignalId].Length - 1) & 7;
    if (startByte == stopByte)
    {
        *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[stopByteBit - startByteBit] << startByteBit));
        *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << startByteBit);
    }
    else
    {
        firstByteActiveBit = 8 - startByteBit;
        for (loop = startByte; loop <= stopByte; loop++)
        {
            if (loop == startByte)
            {
                *(SrcBuffer + loop) &= (uint8)((1 << startByteBit) - 1);
                *(SrcBuffer + loop) |= (uint8)((newValue & ((1 << (firstByteActiveBit)) - 1)) << startByteBit);
                newValue = newValue >> firstByteActiveBit;
            }
            else if (loop == stopByte)
            {
                *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[stopByteBit]));
                *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[stopByteBit]);
            }
            else
            {
                *(SrcBuffer + loop) = (uint8)(newValue & 255);
                newValue = newValue >> 8;
            }
        }
    }
    return (E_OK);
}

#ifndef COM_GENCODE_FROM_XML
static boolean Com_GetMotorBigTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByteBit = (startByteBit + 9 - (Com_SigConfInfo[SignalId].Length & 7)) & 7;
    stopByte = startByte + ((Com_SigConfInfo[SignalId].Length + 6 - startByteBit) >> 3);

    if (startByte == stopByte)
    {
        getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[startByteBit]) >> stopByteBit;
    }
    else
    {
        lastByteActiveBit = 8 - stopByteBit;
        for (loop = stopByte; loop >= startByte; loop--)
        {
            if (loop == stopByte)
            {
                getValue = *(SrcBuffer + loop) >> stopByteBit;
            }
            else if (loop == startByte)
            {
                getValue += (*(SrcBuffer + loop) & ByteHighBitsMask[startByteBit]) << (((stopByte - startByte - 1) << 3) + lastByteActiveBit);
            }
            else
            {
                getValue += *(SrcBuffer + loop) << (((stopByte - loop - 1) << 3) + lastByteActiveBit);
            }
        }
    }

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1)))
        {
            getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
        }
    }
    *((uint32 *)GetValue) = (uint32)getValue;
    return (E_OK);
}

static boolean Com_SetMotorBigTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }

    startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
    startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
    stopByteBit = (startByteBit + 9 - (Com_SigConfInfo[SignalId].Length & 7)) & 7;
    stopByte = startByte + ((Com_SigConfInfo[SignalId].Length + 6 - startByteBit) >> 3);

    if (startByte == stopByte)
    {
        *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[startByteBit - stopByteBit] << stopByteBit));
        *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << stopByteBit);
    }
    else
    {
        lastByteActiveBit = 8 - stopByteBit;
        for (loop = stopByte; loop >= startByte; loop--)
        {
            if (loop == stopByte)
            {
                *(SrcBuffer + loop) &= (uint8)((1 << stopByteBit) - 1);
                *(SrcBuffer + loop) |= (uint8)((newValue & ((1 << lastByteActiveBit) - 1)) << stopByteBit);
                newValue = newValue >> lastByteActiveBit;
            }
            else if (loop == startByte)
            {
                *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[startByteBit]));
                *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[startByteBit]);
            }
            else
            {
                *(SrcBuffer + loop) = (uint8)(newValue & 255);
                newValue = newValue >> 8;
            }
        }
    }
    return (E_OK);
}
#else
static boolean Com_GetMotorBigTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 *GetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    sint32 getValue = 0xFFFFFFFF;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }
    else
    {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
        stopByte = startByte - ((Com_SigConfInfo[SignalId].Length + startByteBit - 1) >> 3);
        stopByteBit = (startByteBit + Com_SigConfInfo[SignalId].Length - 1) & 7;

        if (startByte == stopByte)
        {
            getValue = (*(SrcBuffer + startByte) & ByteHighBitsMask[stopByteBit]) >> startByteBit;
        }
        else
        {
            lastByteActiveBit = 8 - startByteBit;
            for (loop = startByte ; loop >= stopByte; loop--)
            {
                if (loop == startByte)
                {
                    getValue = *(SrcBuffer + loop) >> startByteBit;
                }
                else if (loop == stopByte)
                {
                    getValue += (*(SrcBuffer + loop) & ByteHighBitsMask[stopByteBit]) << (((startByte - loop - 1) << 3) + lastByteActiveBit);
                }
                else
                {
                    getValue += *(SrcBuffer + loop) << (((startByte - loop - 1) << 3) + lastByteActiveBit);
                }
            }
        }
        if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
                || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
                || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
        {
            if (getValue & ((uint32)1 << (Com_SigConfInfo[SignalId].Length - 1)))
            {
                getValue |= (~(((uint32)1 << Com_SigConfInfo[SignalId].Length) - 1));
            }
        }
        *((uint32 *)GetValue) = (uint32)getValue;
    }
    return (E_OK);
}

static boolean Com_SetMotorBigTypeSignal(uint16 SignalId, uint8 *SrcBuffer, uint32 SetValue)
{
    uint8 startByte = 0u;
    uint8 startByteBit = 0u;
    uint8 stopByte = 0u;
    uint8 stopByteBit = 0u;
    uint8 lastByteActiveBit = 0u;
    sint8 loop = 0u;
    uint32 newValue = SetValue;

    if ((0 == Com_SigConfInfo[SignalId].Length)
            || (32 < Com_SigConfInfo[SignalId].Length)
            || (COM_SIG_UINT8_N == Com_SigConfInfo[SignalId].SignalType))
    {
        return (E_NOT_OK);
    }
    else
    {
        startByte = Com_SigConfInfo[SignalId].StartBit >> 3;
        startByteBit = Com_SigConfInfo[SignalId].StartBit & 7;
        stopByte = startByte - ((Com_SigConfInfo[SignalId].Length + startByteBit - 1) >> 3);
        stopByteBit = (startByteBit + Com_SigConfInfo[SignalId].Length - 1) & 7;

        if (startByte == stopByte)
        {
            *(SrcBuffer + startByte) &= (uint8)(~(ByteHighBitsMask[stopByteBit - startByteBit] << startByteBit));
            *(SrcBuffer + startByte) |= (uint8)((newValue & ByteHighBitsMask[Com_SigConfInfo[SignalId].Length - 1]) << startByteBit);
        }
        else
        {
            lastByteActiveBit = 8 - startByteBit;
            for (loop = startByte ; loop >= stopByte; loop--)
            {
                if (loop == startByte)
                {
                    *(SrcBuffer + loop) &= (uint8)((1 << startByteBit) - 1);
                    *(SrcBuffer + loop) |= ((newValue & ByteHighBitsMask[lastByteActiveBit - 1]) << startByteBit);
                    newValue = newValue >> lastByteActiveBit;
                }
                else if (loop == stopByte)
                {
                    *(SrcBuffer + loop) &= (uint8)(~(ByteHighBitsMask[stopByteBit]));
                    *(SrcBuffer + loop) |= (uint8)(newValue & ByteHighBitsMask[stopByteBit]);
                }
                else
                {
                    *(SrcBuffer + loop) = (uint8)(newValue & 255);
                    newValue = newValue >> 8;
                }
            }
        }
    }
    return (E_OK);
}
#endif

boolean Com_SigFilter_Type1(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_ALWAYS
    SignalId = SignalId;
    OldValue = OldValue;
    NewValue = NewValue;
    return (TRUE);
}

boolean Com_SigFilter_Type2(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEVER
    SignalId = SignalId;
    OldValue = OldValue;
    NewValue = NewValue;
    return (FALSE);
}

boolean Com_SigFilter_Type3(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_EQUALS_X
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.FilterX) == (sint32)(NewValue & Com_SigConfInfo[SignalId].Filter.Mask))
    {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type4(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_DIFFERS_X
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.FilterX) != (sint32)(NewValue & Com_SigConfInfo[SignalId].Filter.Mask))
    {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type5(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_EQUALS_MASKED_OLD
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.Mask & OldValue) == (NewValue & Com_SigConfInfo[SignalId].Filter.Mask))
    {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type6(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_MASKED_NEW_DIFFERS_MASKED_OLD
    boolean l_ReturnValue = FALSE;

    if ((Com_SigConfInfo[SignalId].Filter.Mask & OldValue) != (NewValue & Com_SigConfInfo[SignalId].Filter.Mask))
    {
        l_ReturnValue = TRUE;
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type7(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_WITHIN   not suilt for boolean type
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if ((Com_SigConfInfo[SignalId].Filter.Min < (sint32)NewValue)
                || ((sint32)NewValue < Com_SigConfInfo[SignalId].Filter.Max))
        {
            l_ReturnValue = TRUE;
        }
    }
    else if (COM_SIG_BOOLEAN != Com_SigConfInfo[SignalId].SignalType)
    {
        if ((Com_SigConfInfo[SignalId].Filter.Min < NewValue) || (NewValue < Com_SigConfInfo[SignalId].Filter.Max))
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        //do nothing,to match MISRA C rules
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type8(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_OUTSIDE   not suilt for boolean type
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType)
            || (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if (((sint32)NewValue < Com_SigConfInfo[SignalId].Filter.Min) || (Com_SigConfInfo[SignalId].Filter.Max < (sint32)NewValue))
        {
            l_ReturnValue = TRUE;
        }
    }
    else if (COM_SIG_BOOLEAN != Com_SigConfInfo[SignalId].SignalType)
    {
        if ((NewValue < Com_SigConfInfo[SignalId].Filter.Min) || (Com_SigConfInfo[SignalId].Filter.Max < NewValue))
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        //do nothing,to match MISRA C rules
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type9(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_GREATER
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if ((sint32)OldValue < (sint32)NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        if (OldValue < NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type10(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_LESSOREQUAL
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if ((sint32)OldValue >= (sint32)NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        if (OldValue >= NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type11(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_LESS
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if ((sint32)OldValue > (sint32)NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        if (OldValue > NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type12(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_NEW_IS_GREATEROREQUAL
    boolean l_ReturnValue = FALSE;

    if ((COM_SIG_SINT8 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT16 == Com_SigConfInfo[SignalId].SignalType) ||
            (COM_SIG_SINT32 == Com_SigConfInfo[SignalId].SignalType))
    {
        if ((sint32)OldValue <= (sint32)NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    else
    {
        if (OldValue <= NewValue)
        {
            l_ReturnValue = TRUE;
        }
    }
    return (l_ReturnValue);
}

boolean Com_SigFilter_Type13(uint16 SignalId, uint32 OldValue, uint32 NewValue)
{
    //COM_F_ONE_EVERY_N
    boolean l_ReturnValue = FALSE;

    Com_SigRunInfo[SignalId].Filter.Occurence++;
    if (Com_SigRunInfo[SignalId].Filter.Occurence == Com_SigConfInfo[SignalId].Filter.Offset)
    {
        l_ReturnValue = TRUE;
    }
    if (Com_SigRunInfo[SignalId].Filter.Occurence == Com_SigConfInfo[SignalId].Filter.Period)
    {
        Com_SigRunInfo[SignalId].Filter.Occurence = 0;
    }
    return (l_ReturnValue);
}

boolean (*Com_GetSigProcess[])(uint16 SignalId, uint8* SrcBuffer, uint32 *GetValue) =
{
    Com_GetMotorBigTypeSignal, Com_GetIntelLittleTypeSignal
};

boolean (*Com_SetSigProcess[])(uint16 SignalId, uint8* SrcBuffer, uint32 SetValue) =
{
    Com_SetMotorBigTypeSignal, Com_SetIntelLittleTypeSignal
};

boolean (*Com_SigFilter[])(uint16 SignalId, uint32 OldValue, uint32 NewValue) =
{
    Com_SigFilter_Type1,//COM_F_ALWAYS,
    Com_SigFilter_Type2,//COM_F_NEVER
    Com_SigFilter_Type3,//COM_F_MASKED_NEW_EQUALS_X
    Com_SigFilter_Type4,//COM_F_MASKED_NEW_DIFFERS_X
    Com_SigFilter_Type5,//COM_F_MASKED_NEW_EQUALS_MASKED_OLD
    Com_SigFilter_Type6,//COM_F_MASKED_NEW_DIFFERS_MASKED_OLD
    Com_SigFilter_Type7,//COM_F_NEW_IS_WITHIN
    Com_SigFilter_Type8,//COM_F_NEW_IS_OUTSIDE
    Com_SigFilter_Type9,//COM_F_NEW_IS_GREATER
    Com_SigFilter_Type10,//COM_F_NEW_IS_LESSOREQUAL
    Com_SigFilter_Type11,//COM_F_NEW_IS_LESS
    Com_SigFilter_Type12,//COM_F_NEW_IS_GREATEROREQUAL
    Com_SigFilter_Type13 //COM_F_ONE_EVERY_N
};

/*============================= END OF FILE: Com_Common.c ============================*/
