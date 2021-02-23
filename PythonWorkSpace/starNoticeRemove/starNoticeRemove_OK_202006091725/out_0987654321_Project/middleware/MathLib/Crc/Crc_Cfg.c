
#include "Std_Types.h"
#include "Crc_Cfg.h"
#include "Crc_Common.h"
#include "Crc_Table.h"

const uint8 Crc_MaxChannel = CRC_CH_MAX;

const CrcConfigType CrcConfigList[] =
{
    

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

    
};


