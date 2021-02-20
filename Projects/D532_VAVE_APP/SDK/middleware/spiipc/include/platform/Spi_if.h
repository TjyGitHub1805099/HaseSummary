
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Spi_if.c
**          Author: brown
**
**          Date  : 2018-04-23
**          
**          
******************************************************************************/

#ifndef _SPI_IF_H_
#define _SPI_IF_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

extern void SpiMasterReq(uint8 HwChannel);
extern void SpiMasterAck(uint8 HwChannel);
extern uint8 SpiTransData(uint8 HwChannel,uint8 *pReceiveData, uint8 *pSendData, uint16 Len,boolean WaitFlag);
extern uint8 SpiReadData(uint8 HwChannel,uint8 *pData,uint16 Len,boolean WaitFlag);
extern uint8 SpiSendData(uint8 HwChannel,uint8 *pData,uint16 Len,boolean WaitFlag);
extern uint8 GetCurSpiStatus(uint8 HwChannel);
extern uint8 SpiAbortTransfer(uint8 HwChannel);
extern uint8_t SpiGetLastTransErrorStateByInstance(uint8 instance);
extern void SpiSetLastTransErrorStateByInstance(uint8 instance, uint8 state);
extern void SpiSetLastTransErrorState(uint8 HwChannel, uint8 state);
extern void SpiMasterAck1(uint8 HwChannel);
extern void IncReqWaitCnt(uint8 hwNum);
extern void ResetReqWaitCnt(uint8 hwNum);
extern uint16_t GetReqWaitCnt(uint8 hwNum);
extern void UpdateReqTickCnt(uint8 hwNum, uint32 tick_cnt);
extern uint32 GetReqTickCnt(uint8 hwNum);
extern uint8 SpiIsChMerge(uint8 HwChannel);

#ifdef __cplusplus
}
#endif

#endif












