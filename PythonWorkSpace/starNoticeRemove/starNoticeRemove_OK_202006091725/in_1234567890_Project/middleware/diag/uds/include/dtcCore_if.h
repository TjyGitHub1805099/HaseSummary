/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _DTC_CORE_IF_H
#define _DTC_CORE_IF_H
#include"dtcCore.h"

extern void dtcCoreInit();
extern void dtcCorePartInit();
extern void dtcCoreChangeUpdateDtcStatusFlag(uint32 ch,boolean enableOrDisable);
extern void dtcCoreMainLoop();
extern boolean dtcCoreRegAppTaskId(uint8 taskid);
extern boolean dtcCoreSetDtcStatus(uint8 ch,uint16 i,tDtcPassOrActive passiveOrActive, uint8*localSnapshot, uint16 len);
extern tUdsNRCType dtcCoreClearDtc(const tUdsReqInfor*reqInfor);
extern tUdsNRCType dtcCoreReadDtcInfor(const tUdsReqInfor*reqInfor);
extern tUdsNRCType sid0x19CheckPreCondition(const uint8*data,uint16 len);
extern boolean dtcCoreCaptureLocalSnapshot(uint8 ch,uint16 dtcIndex,uint8*localSnapshot, uint16 len);
extern boolean dtcCoreCaptureGlobalSnapshot(uint8 ch,uint16 dtcIndex);
extern boolean dtcCoreResetSnapshot(uint8 ch,uint16 dtcIndex);
extern tDtcDataBuffer *dtcCoreGetSnapshot(uint8 ch,uint16 dtcIndex,uint32 snapshotRecordNum);
extern boolean dtcCoreCheckSnapshotRecordNum(uint8 ch,uint32 snapshotRecordNum);
extern boolean dtcCoreStoreSnapshotToNvm(uint8 ch,uint8 innerDtcId);
extern boolean dtcCoreCheckExtDataRecordNum(uint32 extDataRecordNum);
extern tDtcDataBuffer *dtcCoreGetExtendDataRecord(uint8 ch,uint16 dtcIndex,uint32 extDataRecordNum);
extern void dtcCoreResetDTCInfor(uint8 ch,uint8 i, tDTCResetReason reason);
extern void dtcCoreCpyFreezeFrameFromRamToNvm(uint8 ch,uint8 nvmIndex,uint8 ramIndex);
extern uint16 dtcCoreGetExternalReport(const tUdsReqInfor*reqInfor,uint8 **retBuffer);
extern boolean dtcCoreGetIgnOnFlag();
extern void dtcCoreSetMonitorFlag(boolean flag);
extern boolean dtcCoreGetMonitorFlag();
extern void dtcCoreClearAll();



extern uint8 *gRuntimeSnapshotArray[];
extern const tDtcMixInfor *gDtcMixInforArray[];
extern uint8 gDtcSnapshotTypeLen[];


#ifdef ENABLE_STORE_20_PERCENT_DTC_TO_NVM
//extern tDtcStoredInfor **gDtcStoredInforArray;
//extern uint16 *gDtcAvailableSpaceNumInNvmArray;
#else
extern uint8 **gNvmBlockIdListForRuntimeSnapshotArray;
extern uint8 **gNvmBlockIdListForDtcExtInforArray;
#endif

#endif
