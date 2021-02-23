/**
*@file  Crc_Cfg.h
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
#ifndef _CRC_CFG_H_
#define _CRC_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

typedef enum
{
    CRC_WIDTH_8BIT,
    CRC_WIDTH_16BIT,
    CRC_WIDTH_32BIT,
    CRC_WIDTH_64BIT
} CrcWidthEType;

typedef struct
{
    CrcWidthEType width;
    uint64 polynomial;
    uint64 initialValue;
    uint64 resultXORValue;
    boolean inputReflect;
    boolean resultReflect;
    const void * crcTable;
} CrcConfigType;

#define CRC_MODE_TABLE (0u)
#define CRC_MODE_RUNTIME (1u)
#define CRC_MODE_HARDWARE (2u)

/***************************** User Configuration Area: Start *****************************/
typedef enum
{
    CRC_CH_CRC8,
    CRC_CH_CRC8H2F,
    CRC_CH_CRC16,
    CRC_CH_CRC32,
    CRC_CH_CRC32P4,
    CRC_CH_CRC64,
    CRC_CH_MAX //Don't remove this element, add your own channel before this line.
} CrcChannelEType;

#define CRC_8_MODE      CRC_MODE_TABLE
#define CRC_8H2F_MODE   CRC_MODE_TABLE
#define CRC_16_MODE     CRC_MODE_TABLE
#define CRC_32_MODE     CRC_MODE_TABLE
#define CRC_32P4_MODE   CRC_MODE_TABLE
#define CRC_64_MODE     CRC_MODE_TABLE

/***************************** User Configuration Area: End ******************************/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CRC_CFG_H_
/*=========================== END OF FILE: Crc_Cfg.h ===========================*/
