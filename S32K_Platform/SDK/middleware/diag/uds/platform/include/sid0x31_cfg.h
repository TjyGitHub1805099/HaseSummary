#ifndef _SID_0X31_CFG_H
#define _SID_0X31_CFG_H

#include"sid0x31_if.h"

extern boolean sid0x31Did0x0101DummyVerifyData(const uint8*data,uint16 len);
extern boolean sid0x31Did0x0101DummyCheckPrecondition(const uint8*data,uint16 len);
extern tRoutineCtrlBuffer* sid0x31Did0x0101DummyStartTest(const uint8*data,uint16 len);
extern tRoutineCtrlBuffer* sid0x31Did0x0101DummyStopTest(const uint8*data,uint16 len);
extern tRoutineCtrlBuffer* sid0x31Did0x0101DummyReqResult(const uint8*data,uint16 len);

#endif