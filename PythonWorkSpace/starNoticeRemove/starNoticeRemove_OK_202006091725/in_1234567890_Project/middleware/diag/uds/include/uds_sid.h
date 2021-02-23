/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/



#ifndef _UDS_SID_H
#define _UDS_SID_H
#include"diagCommon_if.h"
#include"uds.h"

typedef tUdsNRCType (*tDiagServiceHookFunc)(tUdsReqInfor*reqInfor);
typedef void (*tDiagServiceStopHookFunc)(uint32 ch);

typedef struct{
	boolean customerReqSupportFlag;
	tSidType sid;
	tUDSAddrType addrFilter;
	boolean supportSuppressPosRespFlag;
	tDiagServiceHookFunc hookFunc;
	tDiagServiceStopHookFunc stopHookFunc;
	uint32 sessionTypeFilter;//tUdsSessionInnerType
}tDiagSidServiceInfor;

#endif
