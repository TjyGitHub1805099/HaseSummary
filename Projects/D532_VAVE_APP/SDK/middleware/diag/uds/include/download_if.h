/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H
#include"download.h"

extern void downloadInit();
extern tUdsNRCType sid0x34VerifyData(tUdsReqInfor *reqInfor);
extern tUdsNRCType sid0x34CheckMainPreCondition();
extern tUdsNRCType sid0x36CheckMainPrecondition();
extern tUdsNRCType sid0x36MainProcess(tUdsReqInfor *reqInfor);
extern tUdsNRCType sid0x37MainProcess(tUdsReqInfor *reqInfor);
extern void downloadResetStatus(uint32 ch);
extern tUdsNRCType sid0x31IpcUpdater();



#endif
