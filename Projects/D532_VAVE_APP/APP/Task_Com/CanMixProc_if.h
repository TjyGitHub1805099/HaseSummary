#ifndef _CAN_MIX_PROC_IF_H
#define _CAN_MIX_PROC_IF_H

#include"Std_Types.h"
#include"ComStack_Types.h"

extern void CanMixProc_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);

#endif
