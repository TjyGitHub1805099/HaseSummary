/*************************************************************************************************************
* author: liqi@hangsheng.com.cn
* 
*************************************************************************************************************/




#ifndef _DIAG_APP_SID_0X31_H
#define _DIAG_APP_SID_0X31_H
#include"Std_Types.h"
#include"diagCommon.h"
#include"sid0x31_cfg.h"

#define ROUTINE_RESULT_CORRECT 0
#define ROUTINE_RESULT_INCORRECT 1

//defined in SRS_flashing_specification_M85_V1.12_20161220.pdf,Pg31
#define ENGINE_SPEED_CONDITIONS_ARE_MET 0
#define ENGINE_SPEED_IS_NOT_ZERO 1
#define VEHICLE_CONDITIONS_ARE_MET 0
#define VEHICLE_SPEED_IS_NOT_STANDING 2
#define IGNITION_CONDTIONS_ARE_MET 0
#define IGNITION_IS_IN_START_POSITION 3
#define VOLTAGE_CONDITIONS_ARE_MET 0
#define UNDER_OR_ABOVE_PROGRAMMING_VOLTAGE 04

#define ROUTINE_STARTED_SUCCESSFULLY 0
#define ROUTINE_IS_BUSY 1
#define ROUTINE_ABNORMAL_STOPPED_BY_ECU 2
#define WRONG_ROUTINE_ID  3
#define ROUTINE_WAS_STOPPED_BY_TESTER 4
#define ROUTINE_WAS_NOT_STARTED 5

#define ROUTINE_STARTED_FAILED 1
#define ROUTINE_DENIED_DISTANCE_BEYOND_254KM 1
#define ROUTINE_DENIED_HAD_CLEAR_100_TIMES 2
#define ROUTINE_DENIED_CURRENT_ODO_IS_0_OR_HAD_RESET 3


typedef enum{
	AVM_CALIBRATION_RESULT = 1,
	AVM_CALIBRATION_ERR_INFOR = 2,
}tAvmIpcMsg;


typedef enum{
	AVM_CALIBRATION_ERR_IDLE,
	AVM_CALIBRATION_ERR_FRONT_CAMERA_FAILED,
	AVM_CALIBRATION_ERR_LEFT_CAMERA_FAILED,
	AVM_CALIBRATION_ERR_REAR_CAMERA_FAILED,
	AVM_CALIBRATION_ERR_RIGHT_CAMERA_FAILED,
	AVM_CALIBRATION_ERR_INPUT_PARA_FAULT,
	AVM_CALIBRATION_ERR_FAILED_TO_OPEN_ORIGINAL_FILE,
}tAvmCalibrationErr;


boolean diagApp0x31ReqHandle(uint8*data, uint16 len);
boolean diagApp0x31StopAllTest(uint8*data, uint16 len);
//void diagApp0x31IpcCallback(uint16 function, uint8 sid, uint8 *rx_data, uint32 len);
void diagApp0x31IpcCallback(uint8 ch,uint8 *rx_data, uint32 len);




#endif
