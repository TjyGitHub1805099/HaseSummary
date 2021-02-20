#ifndef _UDS_SID_IF_H
#define _UDS_SID_IF_H
#include"uds_if.h"
#include"uds_sid.h"

extern tUdsNRCType diagSid0x99InnerTest(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x10SessionControl(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x11EcuReset(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x14ClearDTC(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x19ReadDtc(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x2fIOControl(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x22ReadDataByDID(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x27SecurityAccess(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x28CommunicationControl(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x2eWriteDataByDID(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x31RoutineControl(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x34RequestDownload(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x35RequestUpload(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x36TransferData(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x37RequestTransferExit(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x3eTesterPresent(uint8 ch,  uint8 *data,uint16 len);
extern tUdsNRCType diagSid0x85DTCSetting(uint8 ch,  uint8 *data,uint16 len);

//you should consider big/little endian when you call this api
extern boolean diagSidResp(boolean dataFromSocFlag,boolean finishFlag,uint8 respCode,const uint8 *respData, uint16 len);

extern uint8 udsSidGetSupportedSidCount();
extern void udsSidInit();

#endif
