/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H
#include"download.h"

extern void downloadInit();
extern tUdsNRCType sid0x34VerifyData(const uint8* data, uint16 len);
extern tUdsNRCType sid0x34CheckMainPreCondition();
extern tUdsNRCType sid0x36CheckMainPrecondition();
extern tUdsNRCType sid0x36MainProcess(const uint8* data, uint16 len);
extern tUdsNRCType sid0x37MainProcess(const uint8*data, uint16 len);
extern void downloadResetStatus();



#endif
