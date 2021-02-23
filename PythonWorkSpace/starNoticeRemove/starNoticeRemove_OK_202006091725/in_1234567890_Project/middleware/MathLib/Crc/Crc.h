/**
*@file  Crc.h
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
#ifndef _CRC_H_
#define _CRC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "Crc_Common.h"

/*
*When step==CRC_STEP_SINGLE or step==CRC_STEP_MULTI_START, "startValue" will be ignore.
*When step==CRC_STEP_MULTI_CONTI or step==CRC_STEP_MULTI_END, "startValue" should be the previous crc value.
*/
extern uint8 Crc_CalculateCRC8(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);
extern uint8 Crc_CalculateCRC8H2F(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);
extern uint16 Crc_CalculateCRC16(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);
extern uint32 Crc_CalculateCRC32(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);
extern uint32 Crc_CalculateCRC32P4(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);
extern uint64 Crc_CalculateCRC64(uint8 * p_buf, uint16 length, CrcCalcStepEType step, uint64 startValue);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CRC_H_
/*=========================== END OF FILE: Crc.h ===========================*/
