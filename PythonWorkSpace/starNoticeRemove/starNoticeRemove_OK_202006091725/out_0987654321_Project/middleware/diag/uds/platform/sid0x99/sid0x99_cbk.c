#include "sid0x99_if.h"
#include "uds.h"
#include "MemMap.h"
#include "NvM_Cfg.h" 
#include "SWVersion.h"
#include "ipc_hb_with_ivi_if.h"
#include "Delay.h"
#include <string.h>
#include "Spi_Ipc_gw_api.h"
#include "usb_swdl.h"
#include "uds_cfg.h"
#include "dtcApp_if.h"
#include "main.h"
typedef enum{
	INNER_TEST_REQ_SET_TP_TRACE_BY_CAN_FLAG = 0,
	INNER_TEST_REQ_ENABLE_INNER_SID,
	INNER_TEST_REQ_ENABLE_UPGRADE_BY_UDS,
	INNER_TEST_REQ_UDS_WORK_MODE,
	INNER_TEST_REQ_ECU_NAME, 
	
	INNER_TEST_REQ_SOFTWARE_VERSION,
	INNER_TEST_REQ_ENABLE_DTCAPP_TRACE,
	INNER_TEST_REQ_ENABLE_CPU_PAYLOAD_MONITOR,
	INNER_TEST_REQ_GET_CPU_PAYLOAD,
	INNER_TEST_REQ_IPC_STATUS,
	INNER_TEST_REQ_CAN_MSG_LOST_COUNTER,
	INNER_TEST_REQ_IC_SOC_STATUS,
	
	
}tInnerTestReq;

extern void Tp_SetTraceFlag(boolean flag);

extern uint8_t IPC_GetConnectedStatus(void);
const uint8 gUpgradeByUdsKey[10] = {0,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};



const uint8 gUdsEcuName[16] = "N534";


NVM_SEC_VAR_UNSPECIFIED uint8 gEnableTpTraceByCanFlag = FALSE;

boolean sid0x99_GetEnableTpTraceByCanFlag()
{
	return gEnableTpTraceByCanFlag;
}

void sid0x99_SetEnableTpTraceByCanFlag(boolean flag)
{
	if(gEnableTpTraceByCanFlag != flag)
	{
		gEnableTpTraceByCanFlag = flag;
		diagNvmWriteSync(gEnableTpTraceByCanFlagNvMBlockID,&gEnableTpTraceByCanFlag);
	}
}

void sid0x99Init()
{

}

tUdsNRCType sid0x99CheckPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return NRC_CONDITION_NOT_CORRECT;
	else
		return NRC_NONE;
}

boolean sid0x99_Get_InnerTestFlag()
{
	return gEnableInnerSidFlag;
}


tUdsNRCType sid0x99MainProcess(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 ch = reqInfor->ch;
	uint8 *data = reqInfor->data;
	uint16 len = reqInfor->len;
	uint8 versionLen = 0;
	uint8 versionPtr[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] = {0};
	uint8 i = 0;
	boolean invalidKeyFlag = FALSE;
	uint8 udsEcuNameLen = 0;
	
	
	switch(data[1])
	{
	case INNER_TEST_REQ_SET_TP_TRACE_BY_CAN_FLAG:
		gEnableTpTraceByCanFlag = (data[2]!=0)?TRUE:FALSE;
		Tp_SetTraceFlag(gEnableTpTraceByCanFlag);
		diagNvmWriteSync(gEnableTpTraceByCanFlagNvMBlockID,&gEnableTpTraceByCanFlag);
		ret = NRC_STOP_PROCESS;
		break;
	case INNER_TEST_REQ_ENABLE_INNER_SID:
		if(len == 3)
		{
			if(data[2] == 3)
				gEnableInnerSidFlag = TRUE;
			else
				gEnableInnerSidFlag = FALSE;
			gRespDataLen[ch] = 1;
		}
		else
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;
	case INNER_TEST_REQ_ENABLE_UPGRADE_BY_UDS:
		if(gEnableInnerSidFlag == TRUE)
		{
			if(len == 12)
			{
				for(i=0;i<10;i++)
				{
					if(data[i+2] != gUpgradeByUdsKey[i])
					{
						invalidKeyFlag = TRUE;
						break;
					}
				}

				if(invalidKeyFlag == TRUE)
					ret = NRC_REQUEST_OUT_OF_RANGE;
				else
				{
					SetUsbUpgradeFlag(STD_ON);
					gRespDataLen[ch] = 1;
				}
			}
		}
		else
			ret = NRC_CONDITION_NOT_CORRECT;
		break;
	case INNER_TEST_REQ_UDS_WORK_MODE:
		if(len == 2)
		{
			data[2] = UDS_WORK_MODE_IN_APP;
			gRespDataLen[ch] = 2;
		}
		else
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;
	case INNER_TEST_REQ_ECU_NAME:
		if(len == 2)
		{
			udsEcuNameLen = strlen((const char*)gUdsEcuName);
			for(i=0;i<udsEcuNameLen;i++)
				data[2+i] = gUdsEcuName[i];
			gRespDataLen[ch] = udsEcuNameLen+1;
		}
		else
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;

	case INNER_TEST_REQ_SOFTWARE_VERSION:
		if(len == 2)
		{
			versionLen = getIocSWVersionRomHex(versionPtr,IOC_SOFTWARE_VERSION_ROM_HEX_LEN);
			diagCpyData(&data[2],(const uint8*)versionPtr, versionLen);
			gRespDataLen[ch] = versionLen+1;
		}
		break;
	case INNER_TEST_REQ_ENABLE_DTCAPP_TRACE:
		if(data[2] == 3)
			gDtcAppTraceSwitchOn = TRUE;
		else
			gDtcAppTraceSwitchOn = FALSE;
		break;

	case INNER_TEST_REQ_ENABLE_CPU_PAYLOAD_MONITOR:
		gCalCpuPayloadSwitch = (data[2]!=0)?TRUE:FALSE;
		gRespDataLen[ch] = 2;
		break;

	case INNER_TEST_REQ_GET_CPU_PAYLOAD:
		data[2] = (uint8)((cpu_rate_x&0xFF000000)>>24);
		data[3] = (uint8)((cpu_rate_x&0xFF0000)>>16);
		data[4] = (uint8)((cpu_rate_x&0xFF00)>>8);
		data[5] = (uint8)((cpu_rate_x&0xFF));
		gRespDataLen[ch] = 5;
		break;
	case INNER_TEST_REQ_CAN_MSG_LOST_COUNTER:
		data[2] = (uint8)((gLostCanMsgCounter&0xFF000000)>>24);
		data[3] = (uint8)((gLostCanMsgCounter&0xFF0000)>>16);
		data[4] = (uint8)((gLostCanMsgCounter&0xFF00)>>8);
		data[5] = (uint8)((gLostCanMsgCounter&0xFF));
		gRespDataLen[ch] = 5;
		break;
	
	case INNER_TEST_REQ_IPC_STATUS:
		data[2] = GetM_IpcComStatus();
		data[3] = get_ipc_hb_with_ivi_status();
		gRespDataLen[ch] = 3;
		break;

	case INNER_TEST_REQ_IC_SOC_STATUS:
		data[2] = IPC_GetConnectedStatus();
		gRespDataLen[ch] = 2;
		break;
#if 0
	case INNER_TEST_TEMP:
		reqData[0] = 2;
		reqData[1] = 0;
		reqData[2] = 0;
		diagApp_AVM_IpcNotifyInd(1, reqData, 3);

		break;
	case INNER_TEST_TEMP_1:
		reqData[0] = 2;
		reqData[1] = 0;
		reqData[2] = 1;
		diagApp_AVM_IpcNotifyInd(1, reqData, 3);

		break;
#endif
	default:
		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}
