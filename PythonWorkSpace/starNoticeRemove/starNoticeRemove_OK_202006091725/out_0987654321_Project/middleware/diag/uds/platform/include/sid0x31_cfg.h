#ifndef _SID_0X31_CFG_H
#define _SID_0X31_CFG_H

#include"diagCommon_cfg.h"
#include"sid0x31_if.h"


typedef enum{
	ROUTINE_CTL_DUMMY_REQ = 0x0101,
	ROUTINE_AVM_SIMU_CALIBRATION_REQ = 0xFE00,  //simulation calibration
	ROUTINE_AVM_SIMU_SWITCH_FRONT_REQ = 0xFE01, //simulation switch camera picture
	ROUTINE_AVM_SIMU_SWITCH_REAR_REQ = 0xFE02,  //simulation switch camera picture
	ROUTINE_AVM_SIMU_SWITCH_LEFT_REQ = 0xFE03,  //simulation switch camera picture
	ROUTINE_AVM_SIMU_SWITCH_RIGHT_REQ = 0xFE04,  //simulation switch camera picture	
	ROUTINE_AVM_FAC_CALIBRATION_REQ = 0x0205,  //factory mode AVM calibration
	ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ =0x0206, //after sale mode AVM calibration
	ROUTINE_HUD_RESET_USER_1 = 0x0301,
	ROUTINE_HUD_RESET_USER_2 = 0x0302,
	ROUTINE_HUD_RESET_USER_3 = 0x0303,
	ROUTINE_HUD_RESET_USER_4 = 0x0304,
	ROUTINE_HUD_RESET_USER_5 = 0x0305,
	ROUTINE_HUD_RESET_USER_DEFAULT = 0x0306,
	ROUTINE_HUD_CALIBRATION_DISPLAY = 0x0309,
	ROUTINE_HUD_SELF_CHECK_DTC = 0x030A,	
}tRoutineCtrlID;

typedef enum{
	AVM_CALIBRATION_STATUS_IDLE,
	AVM_CALIBRATION_STATUS_PROGRESSING,
	AVM_CALIBRATION_STATUS_SUCCESS,
	AVM_CALIBRATION_STATUS_FAILED,
}tAvmCalibrationStatus;


extern boolean sid0x31Did0x0101DummyVerifyData(const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern boolean sid0x31Did0x0101DummyCheckPrecondition(const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31Did0x0101DummyStartTest(const tUdsReqInfor *reqInfor, const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31Did0x0101DummyStopTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31Did0x0101DummyReqResult(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);

extern tUdsNRCType sid0x31Did0x01FEStartAVMCalibrationSIMU(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31Did0x0105StartAVMCalibrationFAC(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31Did0x0106StartAVMCalibrationSALE(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31AVMCalibrationReqResult(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31AVMCalibrationReqResultSIMU(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);

extern boolean sid0x31Did0x010506AVMVerifyData(const uint8*data,uint16 len);

extern tUdsNRCType sid0x31HudResetStart(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31HudResetConfirm(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31HudCalibrationDisplayStart(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31HudCalibrationDisplayStatus(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31HudCalibrationDisplayStop(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);
extern tUdsNRCType sid0x31HudSelfCheckDTC(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr);


#endif
