#ifndef _UDS_SID_IF_H
#define _UDS_SID_IF_H
#include"uds_if.h"
#include"uds_sid.h"

extern tUdsNRCType diagSid0x99InnerTest(tUdsReqInfor*reqInfor);
extern tUdsNRCType diagSid0x10SessionControl(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x11EcuReset(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x14ClearDTC(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x19ReadDtc(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x2fIOControl(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x22ReadDataByDID(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x23ReadMemByAddr(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x27SecurityAccess(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x28CommunicationControl(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x2eWriteDataByDID(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x31RoutineControl(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x34RequestDownload(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x35RequestUpload(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x36TransferData(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x37RequestTransferExit(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x3eTesterPresent(tUdsReqInfor *reqInfor);
extern tUdsNRCType diagSid0x85DTCSetting(tUdsReqInfor*reqInfor);

//you should consider big/little endian when you call this api
extern tUdsNRCType diagSidResp(tUdsRespInfor *respInfor);

extern uint8 udsSidGetSupportedSidCount(uint8 ch);
extern void udsSidInit();

#endif
