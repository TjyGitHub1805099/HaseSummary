/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _SID_0X2F_H
#define _SID_0X2F_H
#include"diagCommon_if.h"
#include"uds_sid.h"

#define IOCONTROL_RESP_TIMEOUT_VALUE 5000
#define SID_0X2F_RESP_TMP_BUFFER_LEN 40
#define IOCTRL_BUFFER_LEN 8	//maybe this len will be changed according to CRS

typedef enum{
	IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU = 0,
	IOCONTROL_PARA_TYPE_RESET_TO_DEFAULT_OR_INIT_VALUE = 1,
	IOCONTROL_PARA_TYPE_FREEZE_CURRENT_STATE = 2,
	IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT =3
}tIOControlParameterType;

typedef struct{
	boolean timeoutFlag;
	uint16 value;
}tTimer;

#define IO_CTRL_BUFF_LEN 32

typedef struct{
	uint8 len;
	uint8 buffer[IO_CTRL_BUFF_LEN];
}tIOCtrlBuffer;

typedef struct{
	uint32 did;
	tIOControlParameterType status;
}tIOCtrlStatus;

typedef boolean (*tIOCtrlVerifyCbk)(const uint8 *data, uint16 len);
typedef const tIOCtrlBuffer* (*tIOCtrlStartCbk)(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len);
typedef const tIOCtrlBuffer* (*IOCtrlStopCbk)(const tUdsReqInfor *reqInfor,tIOControlParameterType req);

typedef struct{
	uint32 did;
	//uint32 sesstionFilter;
	boolean getRespSyncFlag;
	uint32 securLevelFilter;//tInnerSecurityLevel
	tIOCtrlVerifyCbk verifyParaCbk;
	tIOCtrlVerifyCbk checkExtraDataOrState;
	tIOCtrlStartCbk startTestCbk;
	IOCtrlStopCbk stopTestCbk;
}tIOCtrlInfo;

typedef struct{

}tSid0x2fCfgInfor;


#endif
