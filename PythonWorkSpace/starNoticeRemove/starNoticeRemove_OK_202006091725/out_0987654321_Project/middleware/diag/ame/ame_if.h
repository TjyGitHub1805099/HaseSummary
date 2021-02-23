/**
* @file:    ame_if.h
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
#ifndef _AME_IF_H_
#define _AME_IF_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "tpTypes.h"
#include "msg_def.h"

/*------------------------------ END: USER INCLUDE ------------------------------*/



/**************************************************** for tp ****************************************************************/
extern boolean ameRxIndication(uint32 ch, uint8* newDataPtr, uint16 len);
extern tReqBufferResult ameRxFirstFrameIndication(uint32 ch, uint16 reqBufferLen, uint8** RxBufferPtr);
extern void ameCancelRxData(uint32 ch, uint8 reason);
extern void ameTxDataConfirmation(uint32 ch, boolean result, uint8 errId);


/**************************************************** for ipc ***************************************************************/
extern void ameIpcCbk_SocReadMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);
extern void ameIpcCbk_SocWriteMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);
extern void ameIpcCbk_SocCtrlMcuReply(request_context_t* ctx, uint8* rx_data, uint16 len);






#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_AME_IF_H_
/*============================= END OF FILE: ame_if.h ============================*/
