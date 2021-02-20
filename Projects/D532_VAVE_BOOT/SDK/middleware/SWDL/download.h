/**************************
author: liqi@hangsheng.com.cn
version: v1.0.0
**************************/


#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H
#include"std_types.h"
//#include"diagCommon.h"


typedef enum{
	NRC_GENERAL_REJECT = 0x10,
	NRC_SERVICE_NOT_SUPPORTED = 0x11,
	NRC_SUB_FUNCTION_NOT_SUPPORTED = 0x12,	
	NRC_INCORRECT_MESSAGE_LENGTH = 0x13,
	NRC_RESPONSE_TOO_LONG = 0x14,
	NRC_BUSY_REPEAT_REQ = 0x21,
	NRC_CONDITION_NOT_CORRECT = 0x22,
	NRC_REQUEST_SEQUENCE_ERROR = 0x24,
	NRC_REQUEST_OUT_OF_RANGE = 0x31,
	NRC_SECURITY_ACCESS_REQUEST = 0x33,
	NRC_INVALID_KEY = 0x35,
	NRC_EXCEED_NUMBER_OF_ATTEMPTS = 0x36,
	NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED = 0x37,
	NRC_UPLOAD_DOWNLOAD_NOT_ACCEPTED = 0x70,
	NRC_TRANSFER_DATA_SUSPENDED = 0x71,
	NRC_GENERAL_PROGRAMMING_FAILURE = 0x72,
	NRC_WRONG_BLOCK_SEQ_COUNTER = 0x73,
	NRC_RESPONSE_PENDING = 0x78,
	NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION = 0x7E,
	NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION = 0x7F,
	NRC_VOLTAGE_TOO_HIGH = 0x92,
	NRC_VOLTAGE_TOO_LOW = 0x93,
	NRC_STOP_PROCESS = 0xfe,//due to internl error,process needs to be stopped
	NRC_NONE = 0xff//positive resp
}tUdsNRCType;


tUdsNRCType sid0x34VerifyData(const uint8* data, uint16 len);
tUdsNRCType sid0x34CheckProgPreCondition();
tUdsNRCType sid0x36CheckPrecondition();
tUdsNRCType sid0x36MainProcess(const uint8* data, uint16 len);
tUdsNRCType sid0x37MainProcess(const uint8*data, uint16 len);
void sid0x36ResetStatus();

void Download_Loop();
void downloadInit();
void ProTest(void);



#endif
