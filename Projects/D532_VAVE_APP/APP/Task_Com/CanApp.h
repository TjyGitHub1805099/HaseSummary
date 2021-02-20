/**
* @file:    CanApp.h
* @brief:   Add your description here for this file.
* @author:  zhangyi
* @date:    2018-09-11 10:40:16
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  zhangyi  2018-09-11 10:40:16  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CANAPP_H_
#define _CANAPP_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "CanApp_Cfg.h"
#include "trace.h"
#include "msg_def.h"
//#include "Task_IPC_Cfg.h"
#include "CanApp_Cbk.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/

/*------------------------------ END: USER INCLUDE ------------------------------*/

void CanApp_Init(void);
void CanApp_Main();
void Can_MsgChangeProcess(uint16 l_ipduId,boolean enableTrace);
void CanApp_IpcRxIndication(const void *ctx, uint8 *data, uint32 len);
extern void CanApp_setMsg_0x5e5_CheckSum(Com_SignalIdType SignalId);
extern void CanApp_sendSpeakerReadysignal(void);
extern void CanApp_TxHand();
extern uint8 Com_SetSignal(Com_SignalIdType SignalId, void *SignalDataPtr);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CANAPP_H_
/*============================= END OF FILE: CanApp.h ============================*/
