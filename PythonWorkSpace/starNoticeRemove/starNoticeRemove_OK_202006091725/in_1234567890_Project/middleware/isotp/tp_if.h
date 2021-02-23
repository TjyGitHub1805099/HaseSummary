#ifndef _TP_IF_H
#define _TP_IF_H
#include "tpTypes.h"
#include "ComStack_Types.h"
#include "CircleList.h"

extern void Tp_Init();
extern void Tp_MainLoop();

//for app layer
extern boolean Tp_TxDataRequest(uint32 ch, uint8*data, uint16 len);

//for can driver
extern void Tp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
extern void Tp_TxConfirmation(PduIdType TxPduId);
extern uint32 Tp_GetCurTick();

extern SqQueue tpSQueue[];
extern const tTpCfgInfor gTpCfgInfor;
extern const tSessionLayerWrapper gUpperLayerWrapper[];
extern const uint8 *tpRxBuffer[];
extern const uint8 *tpTxBuffer[];
extern uint8 *gUpperRxBufHeader[];
extern uint8 *gUpperRxBufPtr[];
extern tTpTxDataInfor gUpperTxDataInfor[];
extern tTpChInfor gTpChInfor[];
extern tTpTimerSTmin gTimerSTmin[];
extern tCircleListInfor gTpRxCirlceListInfor[];
extern uint8 gTxcfmCh[];

#endif
