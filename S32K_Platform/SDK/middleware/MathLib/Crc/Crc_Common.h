/**
*@file  Crc_Common.h
*@brief Please describle your module here.
*@date    2017-12-22 13:58:05
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
#ifndef _CRC_COMMON_H_
#define _CRC_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "Crc_Cfg.h"

typedef enum
{
    CRC_STEP_SINGLE,
    CRC_STEP_MULTI_START,
    CRC_STEP_MULTI_CONTI,
    CRC_STEP_MULTI_END
} CrcCalcStepEType;

/*
*When step==CRC_STEP_SINGLE or step==CRC_STEP_MULTI_START, "startValue" will be ignore.
*When step==CRC_STEP_MULTI_CONTI or step==CRC_STEP_MULTI_END, "startValue" should be the previous returned CRC value.
*/
extern uint64 Crc_Calculate(uint8 channel, uint8 *p_buf, uint8 length, CrcCalcStepEType step, uint64 startValue);
extern uint64 Crc_CalculateTable(uint8 channel, uint8 *p_buf, uint8 length, CrcCalcStepEType step, uint64 startValue);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CRC_H_
/*=========================== END OF FILE: Crc_Common.h ===========================*/
