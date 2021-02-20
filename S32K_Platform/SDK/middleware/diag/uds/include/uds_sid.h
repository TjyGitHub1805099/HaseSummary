/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _UDS_SID_H
#define _UDS_SID_H
#include"diagCommon_if.h"
#include"uds.h"

typedef tUdsNRCType (*tDiagServiceHookFunc)(uint8 ch,uint8 *data,uint16 len);
typedef void (*tDiagServiceStopHookFunc)();

typedef struct{
	tSidType sid;
	tUDSAddrType addrFilter;
	boolean checkSuppressPosRespFlag;
	tDiagServiceHookFunc hookFunc;
	tDiagServiceStopHookFunc stopHookFunc;
	uint32 sessionTypeFilter;//tUdsSessionInnerType
}tDiagSidServiceInfor;

#endif
