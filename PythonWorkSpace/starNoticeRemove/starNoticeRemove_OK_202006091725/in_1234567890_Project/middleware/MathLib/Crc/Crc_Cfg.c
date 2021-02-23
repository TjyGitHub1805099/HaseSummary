/**
*@file  Crc_Cfg.c
*@brief Please describle your module here.
*@date    2017-12-22 13:58:22
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
#include "Std_Types.h"
#include "Crc_Cfg.h"
#include "Crc_Common.h"
#include "Crc_Table.h"

const uint8 Crc_MaxChannel = CRC_CH_MAX;

const CrcConfigType CrcConfigList[] =
{
    /***************************** User Configuration Area: Start *****************************/
//CRC_CH_CRC8
    {
        .width = CRC_WIDTH_8BIT,
        .polynomial = 0x1D,
        .initialValue = 0XFF,
        .resultXORValue = 0XFF,
        .inputReflect = FALSE,
        .resultReflect = FALSE,
#if(CRC_8_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_8_Table
#elif(CRC_8_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

//CRC_CH_CRC8H2F
    {
        .width = CRC_WIDTH_8BIT,
        .polynomial = 0X2F,
        .initialValue = 0XFF,
        .resultXORValue = 0XFF,
        .inputReflect = FALSE,
        .resultReflect = FALSE,
#if(CRC_8H2F_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_8H2F_Table
#elif(CRC_8H2F_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

//CRC_CH_CRC16
    {
        .width = CRC_WIDTH_16BIT,
        .polynomial = 0X1021,
        .initialValue = 0XFFFF,
        .resultXORValue = 0X0000,
        .inputReflect = FALSE,
        .resultReflect = FALSE,
#if(CRC_16_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_16_Table
#elif(CRC_16_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

//CRC_CH_CRC32
    {
        .width = CRC_WIDTH_32BIT,
        .polynomial = 0X04C11DB7,
        .initialValue = 0XFFFFFFFF,
        .resultXORValue = 0XFFFFFFFF,
        .inputReflect = TRUE,
        .resultReflect = TRUE,
#if(CRC_32_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_32_Table
#elif(CRC_32_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

//CRC_CH_CRC32P4
    {
        .width = CRC_WIDTH_32BIT,
        .polynomial = 0XF4ACFB13,
        .initialValue = 0XFFFFFFFF,
        .resultXORValue = 0XFFFFFFFF,
        .inputReflect = TRUE,
        .resultReflect = TRUE,
#if(CRC_32P4_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_32P4_Table
#elif(CRC_32P4_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

//CRC_CH_CRC64
    {
        .width = CRC_WIDTH_64BIT,
        .polynomial = 0X42F0E1EBA9EA3693,
        .initialValue = 0XFFFFFFFFFFFFFFFF,
        .resultXORValue = 0XFFFFFFFFFFFFFFFF,
        .inputReflect = TRUE,
        .resultReflect = TRUE,
#if(CRC_64_MODE == CRC_MODE_TABLE)
        .crcTable = Crc_64_Table
#elif(CRC_64_MODE == CRC_MODE_RUNTIME)
        .crcTable = NULL
#endif
    },

    /***************************** User Configuration Area: End ******************************/
};

/*=========================== END OF FILE: Crc_Cfg.c ===========================*/
