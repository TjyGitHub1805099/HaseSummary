/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/

#ifndef _SID_0X22_IF_H
#define _SID_0X22_IF_H

#include"sid0x22.h"

extern void sid0x22Init();
extern const t0x22DidInfo* sid0x22GetDidInfo(uint32 ch, uint16 id);
extern tUdsNRCType sid0x22ParseDidList(uint32 ch, const uint8* data, uint16 len, const t0x22DidBuffer** dids);
extern tUdsNRCType sid0x22CheckPreCondition(const uint8* data, uint16 len);
extern tUdsNRCType sid0x2eCheckPreCondition(const uint8* data, uint16 len);
extern tUdsNRCType sid0x2eWriteDID(uint32 ch, uint8* data, uint16 len);
extern tUdsNRCType sid0x22Check0xDF(tUdsReqInfor *reqInfor);
extern tUdsNRCType sid0x2eCheck0xDF(tUdsReqInfor *reqInfor);

extern const t0x22DidInfo* g0x22DidInforArray[];
extern uint16 g0x22DidTotalNum[];

//==============================================================================================
extern uint8 g_IVI_UdsInnerTestDid[];
//extern uint8 g_IVI_SetupCertificateState[];
extern uint8 g_IVI_SparePartsNumNissan[];
//extern uint8 g_IVI_SparePartsNumRenault[];
extern uint8 g_IVI_DiagIdCode[];
extern uint8 g_IVI_SysSupplierNum[];
extern uint8 g_IVI_EcuSerialNum[];


extern uint8 g_IVI_VehicleManufEcuHwNum[];
extern uint8 g_IVI_SysSupplierEcuSwNum[];
extern uint8 g_IVI_SysSupplierEcuSwVersionNum[];
extern uint8 g_IVI_OperationalReference[];
//==============================================================================================
extern uint8 g_IC_UdsInnerTestDid[];
extern uint8 g_IC_SparePartsNumNissan[];
//extern uint8 g_IC_SparePartsNumRenault[];
extern uint8 g_IC_DiagIdCode[];
extern uint8 g_IC_SysSupplierNum[];
extern uint8 g_IC_SysSupplierEcuSwNum[];
extern uint8 g_IC_SysSupplierEcuSwVersionNum[];
extern uint8 g_IC_LatestSparePartsNum[];

extern boolean gDiagMuxOnCurState;

//==============================================================================================
extern uint8 g_AVM_UdsInnerTestDid[];
//extern uint8 gAvmKeyStatus[];


#endif
