/**************************
description: UDS sid list
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30
**************************/

#include"uds_sid_cfg.h"
#include"uds_sid_if.h"

#ifdef UDS_WITH_SID0X10
#include"sid0x10_if.h"
#endif

#ifdef UDS_WITH_SID0X11
#include"sid0x11_if.h"
#endif

#ifdef UDS_WITH_SID0X28
#include"sid0x28_if.h"
#endif

#ifdef UDS_WITH_DOWNLOAD
#include"download_if.h"
#endif

#ifdef UDS_WITH_SID0X27
#include"sid0x27_if.h"
#endif

#ifdef UDS_WITH_SID0X2F
#include"sid0x2f_if.h"
#endif

#ifdef UDS_WITH_SID0X31
#include"sid0x31_if.h"
#endif

#ifdef UDS_WITH_SID0X22
#include"sid0x22_if.h"
#endif

#ifdef UDS_WITH_DTC
#include"sid0x14_if.h"
#include"sid0x85_if.h"
#endif

const tDiagSidServiceInfor gDiagSidService[] = {
#if 1
#ifdef UDS_WITH_SID0X10
	{SID_SESSION_CONTROL,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,TRUE,diagSid0x10SessionControl,0,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
#endif
#ifdef UDS_WITH_SID0X11
	{SID_ECU_RESET,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,TRUE,diagSid0x11EcuReset,0,/*UDS_DEFAULT_SESSION|*/UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#endif
#ifdef SW_PHASE_APPLICATION
#ifdef UDS_WITH_DTC
	{SID_CLEAR_DTC,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,FALSE,diagSid0x14ClearDTC,0,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
	{SID_READ_DTC,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,FALSE,diagSid0x19ReadDtc,0,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
	{SID_CONTROL_DTC_SETTING,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,TRUE,diagSid0x85DTCSetting,sid0x85ResetStatus,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#endif
#endif
#ifdef UDS_WITH_SID0X22
	{SID_READ_DATA_BY_DID,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,FALSE,diagSid0x22ReadDataByDID,0,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
	{SID_WRITE_DATA_BY_DID,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x2eWriteDataByDID,0,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
#endif
//	{SID_READ_MEMORY_BY_ADDRESS,UDS_ADDR_TYPE_PHY,FALSE,NULL,NULL,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
//	{SID_READ_SCALING_DATA_BY_DID,UDS_ADDR_TYPE_PHY,FALSE,NULL,NULL,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#ifdef UDS_WITH_SID0X27
	{SID_SECURITY_ACCESS,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x27SecurityAccess,sid0x27ResetStatus,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
#endif
#ifdef UDS_WITH_SID0X28
	{SID_COMMUNICATION_CONTROL,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,TRUE,diagSid0x28CommunicationControl,sid0x28ResetStatus,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#endif
//	{SID_READ_DATA_BY_PERIODIC_DID,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
//	{SID_DYNAMICALLY_DEFINE_DATA_DID,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#ifdef SW_PHASE_APPLICATION
#ifdef UDS_WITH_SID0X2F
	{SID_IOCONTROL,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x2fIOControl,sid0x2fStopAllTest,UDS_EXTENDED_DIAG_SESSION},
#endif
#endif
//	{SID_WRITE_MEMORY_BY_ADDRESS,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
#ifdef UDS_WITH_SID0X31
	{SID_ROUTINE_CONTROL,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,FALSE,diagSid0x31RoutineControl,sid0x31StopAllTest,UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
#endif
#ifdef UDS_WITH_DOWNLOAD
	{SID_REQUEST_DOWNLOAD,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x34RequestDownload,downloadResetStatus,UDS_PROGRAMMING_SESSION},
	{SID_REQUEST_UPLOAD,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x35RequestUpload,downloadResetStatus,UDS_PROGRAMMING_SESSION},
	{SID_TRANSFER_DATA,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x36TransferData,downloadResetStatus,UDS_PROGRAMMING_SESSION},
	{SID_REQUEST_TRANSFER_EXIT,UDS_ADDR_TYPE_PHY,FALSE,diagSid0x37RequestTransferExit,downloadResetStatus,UDS_PROGRAMMING_SESSION},
#endif
//	{SID_REQUEST_FILE_TRANSFER,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
	{SID_TESTER_PRESENT,UDS_ADDR_TYPE_PHY|UDS_ADDR_TYPE_FUNC,TRUE,diagSid0x3eTesterPresent,0,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION|UDS_CODING_SESSION},
//	{SID_ACCESS_TIMING_PARAMETER,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
//	{SID_SECURED_DATA_TRANSMISSION,UDS_DEFAULT_SESSION|UDS_EXTENDED_DIAG_SESSION|UDS_PROGRAMMING_SESSION},
//	{SID_RESPONSE_ON_EVENT,},
//	{SID_LINK_CONTROL}
#endif
};

static uint8 gDiagSidServiceTotalNum = 0;

uint8 udsSidGetSupportedSidCount()
{
	return gDiagSidServiceTotalNum;
}


void udsSidInit()
{
	gDiagSidServiceTotalNum = sizeof(gDiagSidService)/sizeof(const tDiagSidServiceInfor);

#ifdef UDS_WITH_SID0X28
	sid0x28Init();
#endif

#ifdef UDS_WITH_SID0X10
	sid0x10Init();
#endif

#ifdef UDS_WITH_SID0X11
	sid0x11Init();
#endif

#ifdef UDS_WITH_SID0X27
	sid0x27Init();
#endif
#ifdef UDS_WITH_SID0X22
	sid0x22Init();
#endif
#ifdef UDS_WITH_SID0X31
	sid0x31Init();
#endif
#ifdef UDS_WITH_DOWNLOAD
	downloadInit();
#endif
#ifdef UDS_WITH_SID0X2F
	sid0x2fInit();
#endif
	
#ifdef UDS_WITH_DTC
	sid0x85Init();
#endif

}

