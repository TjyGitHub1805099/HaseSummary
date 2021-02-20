#ifndef _TP_IF_H
#define _TP_IF_H
#include"tpTypes.h"

extern void Tp_Init();
extern void Tp_MainLoop();

//for app layer
extern boolean Tp_TxDataRequest(uint32 ch, uint8*data, uint16 len);

//for can driver
extern void Tp_RxIndication(uint32 RxPduId, const void* PduInfoPtr);
extern void Tp_TxConfirmation(uint32 TxPduId);
extern uint32 Tp_GetCurTick();
#endif
