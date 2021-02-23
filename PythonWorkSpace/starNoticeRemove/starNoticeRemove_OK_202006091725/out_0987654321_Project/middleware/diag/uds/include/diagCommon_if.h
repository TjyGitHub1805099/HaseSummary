/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DIAG_COMMON_IF_H
#define _DIAG_COMMON_IF_H


#include"diagCommon.h"

extern void udsSendSignal(uint32 sigId,uint32 value);
extern void udsSendMsg(uint32 taskId,uint8 *data, uint16 len);
extern boolean DiagConvertToBCDcode8bit(uint8 srcNum, uint8*dstNum);
extern boolean diagCmp(const uint8*data1,const uint8*data2,uint16 len);
extern uint16 diagCpyData(uint8*dst,const uint8*src, uint16 srcLen);
extern boolean diagResetBuffer(uint8* buffer,uint8 padding,uint16 len);

extern boolean diagCheckGlobalPrecondition();
extern boolean diagGetSysVoltage(uint16 *voltage);
extern boolean diagGetAccStatus(uint8* accStatus);
extern boolean diagGetKL15Status(uint8*kl15Status);
extern boolean diagGetKL30Status(uint8*kl30Status);
extern boolean diagGetVehicleSpeed(float *vehicleSpeed);
extern boolean diagGetEngineRPM(uint16 *rpm);
extern boolean diagCheckIfEngineIsRunning();
extern boolean diagCheckIfVehicleIsStatic();
extern boolean diagCheckPrecondition(tDiagStopReason *stopReason);
extern boolean diagCheckPreCondtionGroup2();
extern tDiagSysVoltageStatus diagCheckSysVoltageStatus();
extern void diagSetNvmBlockId(uint16 BlockId,boolean BlockChanged);
extern void diagNvmWriteSync(uint16 BlockId, uint8 * NvM_SrcPtr);
extern void diagDisableComTx();
extern void diagDisableComRx();
extern void diagEnableComTx();
extern void diagEnableComRx();
extern void diagDisableNMTx();
extern void diagDisableNMRx();
extern void diagEnableNMTx();
extern void diagEnableNMRx();


#ifdef SW_PHASE_APPLICATION

extern uint8 * diagMallocMem(uint16 memLen);
extern tDiagMutex diagCreatMutex();
extern boolean diagMutexLock(tDiagMutex mutex);
extern void diagMutexUnlock(tDiagMutex mutex);
extern uint32 diagGetRandomNum();
extern void diagReqSysReset();
extern uint32 diagGetCurTick();

extern boolean diagGetCanTime(tDiagCANTime *curCANTime);
extern boolean diagGetODO(uint32 *odo);
extern boolean diagGetCurrentFilterSpeed(uint16 *currentDisplayedSpeed);
extern boolean diagGetFilterRpm(uint16 *rpm);

#endif


#endif
