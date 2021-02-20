/******************************************************************************
**          Copyright (c) by HSAE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HSAE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : 
**          Author: Zhu Qiang
**
**          Date  : 2018-05-17
**          
**          
******************************************************************************/

#ifndef __DSP_DRIVER_TYPE_CONVERTER_H__
#define __DSP_DRIVER_TYPE_CONVERTER_H__

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include <stdbool.h>

/******************************************************************************
**    MACROS
******************************************************************************/

bool dspChannelToString(int32_t channel, char** literalChannel);
bool dspInputToString(int32_t input, char** literalInput);

#endif /*__DSP_DRIVER_TYPE_CONVERTER_H__*/
