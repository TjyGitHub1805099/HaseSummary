/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _UDS_IF_H
#define _UDS_IF_H
#include"uds.h"

extern void udsInit();
extern void udsLoop();
extern boolean udsRxIndication(uint32 ch,uint8* dataPtr, uint16 len);
extern uint32 udsRxFirstFrameIndication(uint32 ch,uint16 reqBufferLen,uint8** RxBufferPtr);
extern void udsCancelRxData(uint32 ch,uint8 reason);
extern void udsTxDataConfirmation(uint32 ch,boolean result,uint8 errId);
extern void udsSetResetSysType(tUdsResetSysType resetTriggerType);
extern boolean udsCheckIfNeedToUpdateSystem();
extern void udsSendSignal(uint32 sigId,uint32 value);
extern void udsSendMsg(uint32 taskId,uint8 *data, uint16 len);
extern const tUdsCfgInfor *udsGetGlobalCfgInfor();


#endif
