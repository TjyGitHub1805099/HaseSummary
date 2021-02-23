/**
* @file:    ame_Cbk.h
* @brief:   Add your description here for this file.
* @author:  qiweifeng
* @date:    2018-09-29 17:40:29
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  qiweifeng  2018-09-29 17:40:29  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _AME_CBK_H_
#define _AME_CBK_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "ame.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "sid0x22.h"
#include "sid0x2f.h"
#include "tpTypes.h"
#include "Spi_Ipc_gw_api.h"

/*------------------------------ END: USER INCLUDE ------------------------------*/
uint32 ameParseDevChannel(uint32 function);
tUdsNRCType ameRWParseDidList(uint32 devCh, const uint8* data, uint16 len, const t0x22DidBuffer** dids);
t0x22DidInfo* ameRWGetDidInfo(uint32 devCh, uint16 did);
tIOCtrlInfo* ameIOControlGetDidInfo(uint32 devCh, uint16 did);


/**************************************************** for tp **********************************************************/
boolean ameRxIndication(uint32 ch, uint8* newDataPtr, uint16 len);
tReqBufferResult ameRxFirstFrameIndication(uint32 ch, uint16 reqBufferLen, uint8** RxBufferPtr);
void ameCancelRxData(uint32 ch, uint8 reason);
void ameTxDataConfirmation(uint32 ch, boolean result, uint8 errId);


/**************************************************** for ipc *********************************************************/
/*
void ameIpcCbk_McuReadSocReply(request_context_t* ctx, uint16 function, uint8* rx_data, uint32 len);
void ameIpcCbk_McuWriteSocReply(request_context_t* ctx, uint16 function, uint8* rx_data, uint32 len);
void ameIpcCbk_McuCtrlSocReply(request_context_t* ctx, uint16 function, uint8* rx_data, uint32 len);
*/
void ameIpcCbk_McuRequestSocReply(request_context_t* ctx, uint16 function, uint8* rx_data, uint32 len);


/**************************************************** for ipc *********************************************************/
void ameIpcCbk_SocReadMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);
void ameIpcCbk_SocWriteMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);
void ameIpcCbk_SocCtrlMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);


/**********************************************************************************************************************/
tUdsNRCType ameReadData(uint32 devCh, uint8* data, uint16 len);
tUdsNRCType ameWriteData(uint32 devCh, uint8* data, uint16 len);
tUdsNRCType ameIOControl(uint32 devCh, uint8* data, uint16 len);
tUdsNRCType ameSecurityAccess(uint32 devCh, uint8* data, uint16 len);





#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_AME_CBK_H_
/*============================= END OF FILE: ame_Cbk.h ============================*/
