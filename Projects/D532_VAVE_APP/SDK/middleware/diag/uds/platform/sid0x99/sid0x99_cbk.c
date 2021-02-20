#include"sid0x99_if.h"
#include"MemMap.h"
#include"NvM_Cfg.h"
#include"SWVersion.h"
#include"ipc_hb_with_ivi_if.h"
#include "Com_Cfg.h"
#include"Standby.h"
#include "CanIf.h"
#include "CanIf_Cfg.h"
#include "trace.h"

typedef enum{
	INNER_TEST_REQ_SET_TP_TRACE_BY_CAN_FLAG = 0,
	INNER_TEST_REQ_ENABLE_INNER_SID = 1,
	INNER_TEST_REQ_SOFTWARE_VERSION = 2,
	INNER_TEST_REQ_ENABLE_DTCAPP_TRACE = 3,
	INNER_TEST_REQ_ENABLE_CPU_PAYLOAD_MONITOR = 4,
	INNER_TEST_REQ_GET_CPU_PAYLOAD = 5,
	INNER_TEST_REQ_IPC_STATUS = 6,
	INNER_TEST_REQ_CAN_MSG_LOST_COUNTER = 7,
	INNER_TEST_REQ_CAN_MSG_TX_CFM_FLAG = 8,
	INNER_TEST_REQ_CRASH_REGISTER_RECORD = 9,
	INNER_TEST_REQ_SET_AME_CHANNEL,
	INNER_TEST_REQ_CAN_PDU_DATA,
	INNER_TEST_REQ_SET_TRACE_LEVEL,
}tInnerTestReq;

extern void Tp_SetTraceFlag(boolean flag);
extern void ameSetChannelInfor(uint8 devch);
extern uint16 gRespDataLen[];
extern boolean gEnableInnerSidFlag;
extern boolean gDtcAppTraceSwitchOn;
extern uint32 cpu_rate_x;
extern uint8 gCalCpuPayloadSwitch;
extern uint32 gLostCanMsgCounter;
//extern uint8 gCanMsgTxCfmFlag;
extern uint8 CanApp_Tx_Count[];
extern uint32 gCrashRegisterRecord[];

extern const CanIf_RxPduCfgType CanIf_RxPduCfg_0[];
extern uint8 * const Com_IpduBufPtr[];
extern tTraceModuleInforList gTraceModuleInfoList[];

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


tUdsNRCType sid0x99MainProcess(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 ch = reqInfor->ch;
	uint8 *data = reqInfor->data;
	uint16 len = reqInfor->len;
	uint8 versionLen = 0;
	uint8 versionPtr[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] = {0};
	
	uint8 i =0,k=0;
	uint16 canId = 0;
	uint16 pduId = 0;
	boolean gotCanIdFlag = FALSE;

	//for test only
	//uint8 temp[1] ={20};
	//uint8 temp[3] = {2, 0, 0};
	//uint8 temp_1[3] = {2, 0, 1};
	//for test only

	switch(data[1])
	{
	case INNER_TEST_REQ_SET_TP_TRACE_BY_CAN_FLAG:	
		gEnableTpTraceByCanFlag = (data[2]!=0)?TRUE:FALSE;
		Tp_SetTraceFlag(gEnableTpTraceByCanFlag);
		diagNvmWriteSync(gEnableTpTraceByCanFlagNvMBlockID,&gEnableTpTraceByCanFlag);
		ret = NRC_STOP_PROCESS;//this NRC won't be sent out to tester
		break;
	//for test only
		//diagAppIpcCbk_SocNotify_IVI(101, temp, 1);
		//break;

		//diagApp_AVM_IpcNotifyInd(1, temp, 3);
		//break;
	//for test only
	case INNER_TEST_REQ_ENABLE_INNER_SID:
		if(len == 3)
		{
			if(data[2] == 3)//enable
				gEnableInnerSidFlag = TRUE;
			else
				gEnableInnerSidFlag = FALSE;
		}
		else
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		break;

		//for test only
		//diagApp_AVM_IpcNotifyInd(1, temp_1, 3);
		//break;


		//for test only
	case INNER_TEST_REQ_SOFTWARE_VERSION:
		if(len == 2)
		{
			versionLen = getIocSWVersionRomHex(versionPtr,IOC_SOFTWARE_VERSION_ROM_HEX_LEN);
			diagCpyData(&data[2],versionPtr, versionLen);
			gRespDataLen[ch] = versionLen+1;
		}
		break;
	case INNER_TEST_REQ_ENABLE_DTCAPP_TRACE:
		if(data[2] == 3)//enable
			gDtcAppTraceSwitchOn = TRUE;
		else
			gDtcAppTraceSwitchOn = FALSE;
		gRespDataLen[ch] = 3;
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
	case INNER_TEST_REQ_CAN_MSG_TX_CFM_FLAG:
		data[2] = CanApp_Tx_Count[IPDU_HND_DA_Information - COM_TX_IPDU_START_INDEX];
		gRespDataLen[ch] = 2;
		break;
	case INNER_TEST_REQ_CRASH_REGISTER_RECORD:
		if(len != 2)
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		else
		{
			for(i=0;i<CRASH_CALL_BACK_DEEP;i++)
			{
				data[2+i*4] = (uint8)((gCrashRegisterRecord[i]&0xFF000000)>>24);
				data[3+i*4] = (uint8)((gCrashRegisterRecord[i]&0xFF0000)>>16);
				data[4+i*4] = (uint8)((gCrashRegisterRecord[i]&0xFF00)>>8);
				data[5+i*4] = (uint8)((gCrashRegisterRecord[i]&0xFF));
			}
			gRespDataLen[ch] = 1+4*CRASH_CALL_BACK_DEEP;
			break;
		}
		break;
	case INNER_TEST_REQ_SET_AME_CHANNEL:
		ameSetChannelInfor(data[2]);
		gRespDataLen[ch] = 2;
		break;
	case INNER_TEST_REQ_CAN_PDU_DATA:
		if(len != 4)
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		else
		{
			canId = (uint16)(data[2]<<8 | data[3]);
			
			for(k=0;k<CANIF_RX_PDU_NUM_0;k++)
			{
				if(CanIf_RxPduCfg_0[k].id == canId)
				{
					gotCanIdFlag = TRUE;
					pduId = CanIf_RxPduCfg_0[k].cbkId;

					for(i=0;i<8;i++)
					{
						data[i+2] = Com_IpduBufPtr[pduId][i];
					}
					break;
				}
			}

			if(gotCanIdFlag == TRUE)
				gRespDataLen[ch] = 2+8;
			else
				ret = NRC_REQUEST_OUT_OF_RANGE;
		}
		break;
	case INNER_TEST_REQ_SET_TRACE_LEVEL:
		//99 0xc moduleIndex traceLevel
		if(len != 4)
			ret = NRC_INCORRECT_MESSAGE_LENGTH;
		else
		{
			if(data[2]<MODULE_TRACE_NAME_MAX)//moduleIndex
			{
				gTraceModuleInfoList[data[2]].traceLevel = ((data[3]<= TRACE_ERRO)?data[3]:TRACE_ERRO);
			}
			
			gRespDataLen[ch] = 4;
		}
		break;
	default:
		ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
	}

	return ret;
}
boolean sid0x99_Get_InnerTestFlag()
{

	return gEnableInnerSidFlag;

}


