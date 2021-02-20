#include"CanMixProc_if.h"
#include "CanAppDiagClient.h"

void CanMixProc_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
	if(RxPduId == 0)
	{
		CanAppDiagRxProc(PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
	}
}
