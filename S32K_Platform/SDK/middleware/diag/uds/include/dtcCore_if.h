/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _DTC_CORE_IF_H
#define _DTC_CORE_IF_H
#include"dtcCore.h"

extern void dtcCoreInit();
extern void dtcCorePartInit();
extern void dtcCoreChangeUpdateDtcStatusFlag(boolean enableOrDisable);
extern void dtcCoreMainLoop();
extern boolean dtcCoreRegAppTaskId(uint8 taskid);
extern boolean dtcCoreSetDtcStatus(uint16 i,tDtcPassOrActive passiveOrActive, uint8*localSnapshot, uint16 len);
extern tUdsNRCType dtcCoreClearDtc(const uint8 *data, uint16 len);
extern tUdsNRCType dtcCoreReadDtcInfor(uint8 *data, uint16 len);
extern tUdsNRCType sid0x19CheckPreCondition(const uint8*data,uint16 len);
extern boolean dtcCoreCaptureLocalSnapshot(uint16 dtcIndex,uint8*localSnapshot, uint16 len);
extern boolean dtcCoreCaptureGlobalSnapshot(uint16 dtcIndex);
extern boolean dtcCoreResetSnapshot(uint16 dtcIndex);
extern const tDtcDataBuffer *dtcCoreGetSnapshot(uint16 dtcIndex,uint32 snapshotRecordNum);
extern boolean dtcCoreCheckSnapshotRecordNum(uint32 snapshotRecordNum);
extern boolean dtcCoreStoreSnapshotToNvm(uint8 innerDtcId);
extern boolean dtcCoreCheckExtDataRecordNum(uint32 extDataRecordNum);
extern const tDtcDataBuffer *dtcCoreGetExtendDataRecord(uint16 dtcIndex,uint32 extDataRecordNum);

#endif
