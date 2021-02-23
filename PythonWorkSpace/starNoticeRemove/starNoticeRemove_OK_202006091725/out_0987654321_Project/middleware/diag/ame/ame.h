/**
* @file:    ame.h
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
#ifndef _AME_H_
#define _AME_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include "Std_Types.h"

/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "ame_cfg.h"
#include "ame_cbk.h"
#include "diagCommon.h"
#include "sid0x22_if.h"
#include "sid0x22_cfg.h"
#include "msg_def.h"

/*------------------------------ END: USER INCLUDE ------------------------------*/
typedef struct{
	uint8  buffer[AME_TX_BUFF_LEN];
	uint16 curDataLen;
}tAmeTxBuffer;


typedef struct{
	uint32 ameChType;
	boolean ameNewCmdFlag;
	
}tAmeChInfor;


tAmeTxBuffer* ameGetTxBuffer();
uint16 ameAppFillTxBuffer(uint8* data, uint16 len);
uint16 ameAppReveFillTxBuffer(uint8* data, uint16 len);

void ameAppResetTxBuffer();
boolean ameAppTxDataToSoc(request_context_t* ctx, uint16 did, uint8* data, uint16 len);


boolean ameParseNewMsg();
boolean ameAppTxData(uint32 ch, uint8* data, uint16 len);









#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_AME_H_
/*============================= END OF FILE: ame.h ============================*/
