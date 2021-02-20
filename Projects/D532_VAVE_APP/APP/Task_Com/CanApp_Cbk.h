/**
* @file:    CanApp_Cbk.h
* @brief:   Add your description here for this file.
* @author:  zhangyi
* @date:    2018-10-22 16:30:15
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  zhangyi  2018-10-22 16:30:15  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CANAPP_CBK_H_
#define _CANAPP_CBK_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include"CanApp.h"
#include "CanApp_Cfg.h"
#include "Com_Cfg.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/

/*------------------------------ END: USER INCLUDE ------------------------------*/
typedef void (*CanAppTxProc)(uint16 l_txCanAppIdLoop, void *value);
extern const CanAppTxProc CanApp_Tx_Matrix[];
extern volatile uint8 CanApp_RecvHandle[HANDLE_RECVMSG_TEMPSIZE][4];




#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CANAPP_CBK_H_
/*============================= END OF FILE: CanApp_Cbk.h ============================*/
