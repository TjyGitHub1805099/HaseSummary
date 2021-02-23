/**
*@file  Crc.c
*@brief Please describle your module here.
*@date    2017-12-22 13:57:13
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-12-22  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/

#include "Crc_Common.h"

uint64 (*Crc_CalculateFunc[2])(uint8 channel, uint8 *p_buf, uint8 length, CrcCalcStepEType step, uint64 startValue) = {Crc_CalculateTable, Crc_Calculate};

uint8 Crc_CalculateCRC8(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint8 returnValue = 0u;

#if(CRC_8_MODE != CRC_MODE_HARDWARE)
    returnValue = (uint8)(*Crc_CalculateFunc[CRC_8_MODE])(CRC_CH_CRC8, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

uint8 Crc_CalculateCRC8H2F(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint8 returnValue = 0u;

#if(CRC_8H2F_MODE != CRC_MODE_HARDWARE)
    returnValue = (uint8)(*Crc_CalculateFunc[CRC_8H2F_MODE])(CRC_CH_CRC8H2F, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

uint16 Crc_CalculateCRC16(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint16 returnValue = 0u;

#if(CRC_16_MODE != CRC_MODE_HARDWARE)
    returnValue = (uint16)(*Crc_CalculateFunc[CRC_16_MODE])(CRC_CH_CRC16, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

uint32 Crc_CalculateCRC32(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint32 returnValue = 0u;

#if(CRC_32_MODE != CRC_MODE_HARDWARE)
    returnValue = (uint32)(*Crc_CalculateFunc[CRC_32_MODE])(CRC_CH_CRC32, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

uint32 Crc_CalculateCRC32P4(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint32 returnValue = 0u;

#if(CRC_32P4_MODE != CRC_MODE_HARDWARE)
    returnValue = (uint32)(*Crc_CalculateFunc[CRC_32P4_MODE])(CRC_CH_CRC32P4, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

uint64 Crc_CalculateCRC64(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue)
{
    uint64 returnValue = 0ull;

#if(CRC_64_MODE != CRC_MODE_HARDWARE)
    returnValue = (*Crc_CalculateFunc[CRC_64_MODE])(CRC_CH_CRC64, p_buf, length, step, startValue);
#else
    //call hardware crc api here
#endif
    return (returnValue);
}

/*=========================== END OF FILE: Crc.c ===========================*/
