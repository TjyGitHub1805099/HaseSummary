#ifndef _TP_IF_H
#define _TP_IF_H
#include"tpTypes.h"
#include "ComStack_Types.h"
extern void Tp_Init();
extern void Tp_MainLoop();

//for app layer
extern boolean Tp_TxDataRequest(uint32 ch, uint8*data, uint16 len);

//for can driver
extern void Tp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
extern void Tp_TxConfirmation(PduIdType TxPduId);
extern uint32 Tp_GetCurTick();
#endif
