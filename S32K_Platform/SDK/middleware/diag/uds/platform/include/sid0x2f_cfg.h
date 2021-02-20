#ifndef _SID_0X2F_CFG_H
#define _SID_0X2F_CFG_H

#include"sid0x2f_if.h"


extern const tIOCtrlBuffer* sid0x2fDid0X0101DummyStartTest(const uint8*data,uint16 len);
extern const tIOCtrlBuffer* sid0x2fDid0X0101DummyStopTest(tIOControlParameterType req);
extern boolean sid0x2fDid0X0101DummyVerifyPara(const uint8*data, uint16 len);
extern tUdsNRCType sid0x2fDid0X0101DummyCheckExtraDataOrState(const uint8*data, uint16 len);
#endif