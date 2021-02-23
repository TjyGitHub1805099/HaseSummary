#include "sid0x2f_if.h"
#include "FanCtrl.h"
#include "sid0x2f_cfg.h"
#include "PowerSupply.h"
#include "System_Signal.h"
#include "diagAppIpc_cbk.h"
#include "Task_M_IpcApp_Cfg.h"
#include "msg_def.h"
#include "NvM_Cfg.h" 
#include "NvM_if.h" 
#include "amp_control_api.h"
#include "Delay.h"
#include <string.h>
#include "Spi_Ipc_gw_api.h"
#include "uds_cfg.h"
#include "main.h"

extern void Com_RxStop();






extern void diagApp_setRxTick(uint8 ch, uint32 tick);

tIOCtrlBuffer gIOCtrlStateBuffer = {
	.len = 0,
	.buffer = {1},
};

boolean sid0x2fCheckMainPreCondition()
{
	
	if(diagCheckPreCondtionGroup2() == FALSE)
		return FALSE;
	return TRUE;
}


tUdsNRCType sid0x2fCheck0xDF(tUdsReqInfor *reqInfor)
{
	tUdsNRCType ret = NRC_NONE;
	uint8 did_high_byte = 0;
	uint8 i=0;
	uint8 byteValue = 0;
	uint8 byteNum = 0;
	uint8 bitNum = 0;
	uint8 ch = reqInfor->ch;
	uint8* dataPtr = reqInfor->data;
	uint16 len = reqInfor->len;
	boolean matchFlag = FALSE;

	if(gIOControlInfoArray[ch] != NULL)
	{
		if(dataPtr[1] == 0xDF && dataPtr[3] == 0x3)
		{
			if(len == 4)
			{
				diagResetBuffer(dataPtr+4, 0, 32);
				did_high_byte = dataPtr[3];
				
				for(i=0; i<gIOCtrlDidNum[ch]; i++)
				{
					if(did_high_byte == 0xDF)   
					{
						byteValue = (uint8)((gIOControlInfoArray[ch][i].did & 0xFF00)>>8);
						matchFlag = TRUE;
					}
					else   
					{
						if(did_high_byte == (uint8)((gIOControlInfoArray[ch][i].did & 0xFF00)>>8))
						{
							byteValue = (uint8)(gIOControlInfoArray[ch][i].did & 0xFF);
							matchFlag = TRUE;
						}
					}

					if(matchFlag == TRUE)
					{
						byteNum = byteValue/8;
						bitNum = 7-byteValue%8;
						dataPtr[4+byteNum] |= 1<<bitNum;
						matchFlag = FALSE;
					}
				}
				
				byteNum = 0xDF/8;
				bitNum = 7-byteValue%8;
				dataPtr[3+byteNum] |= 1<<bitNum;

				gRespDataLen[ch] = 35;
			}
			else
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
		}
		else
			ret = NRC_REQUEST_OUT_OF_RANGE;

	}
	else
		ret = NRC_STOP_PROCESS;

	return ret;
}


boolean sid0x2fCheckSpecificPrecondition(uint32 ch,const tIOCtrlInfo* ioctrlInfor)
{
	boolean ret = TRUE;

	if(ioctrlInfor)
	{
		
		switch(ioctrlInfor->did)
		{
		case IOCTRL_ID_DUMMY_TEST:
			
		#if 0
			if precondition NOT match
				ret = FALSE;
		#endif
			break;
		#if 0
		case IOCT_ID_MORE
			
			break;
		#endif
		default:
			break;
		}

	}

	
	return ret;
}




boolean sid0x2fDid0X0101DummyVerifyPara(const uint8*data, uint16 len)
{
	boolean ret = TRUE;
#if 0
	uint8 ioCtrlPara;
	uint8 ioCtrlState1;
	uint8 ioCtrlState2;

	
	if(len!=4)
		ret = FALSE;
	
	ioCtrlState1 = data[2];
	
	
	
	
#endif
	return ret;
}




tUdsNRCType sid0x2fDid0X0101DummyCheckExtraDataOrState(const uint8*data, uint16 len)
{
	tUdsNRCType ret = NRC_NONE;

	
	#if 0
	
	
	uint16 did = 0;
	const tIOCtrlStatus* ioCtrlStatus;

	did = (uint16)(data[0]<<8|data[1]);
	if((ioCtrlStatus=sid0x2fGetIoCtrlStatus(did))!=NULL)
	{
		if(ioCtrlStatus->status == IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT
			&& (data[2]==IOCONTROL_PARA_TYPE_SHORT_TERM_ADJUSTMENT))
			return NRC_REQUEST_SEQUENCE_ERROR;
	}
	
	#endif
	
	return ret;
}



const tIOCtrlBuffer* sid0x2fDid0X0101DummyStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len)
{
	uint8 ioCtrlPara;
	uint8 state1;
	

	if(data )
	{
		ioCtrlPara = data[2];
		state1 = data[3];
		
	
		
		
	
		gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
		gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
		gIOCtrlStateBuffer.buffer[2] = ioCtrlPara;
		SetFanSpeed(state1);
		
		gIOCtrlStateBuffer.buffer[3] = 0x11;

		gIOCtrlStateBuffer.len = 4;

		

		return &gIOCtrlStateBuffer;
	}
	else
		return NULL;
}


const tIOCtrlBuffer* sid0x2fDid0X0101DummyStopTest(const tUdsReqInfor *reqInfor,tIOControlParameterType req)
{
	uint8 curState = 0;

	

	if(req == IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU
		|| req == IOCONTROL_PARA_TYPE_RESET_TO_DEFAULT_OR_INIT_VALUE)
	{
		
	}
	else if(req == IOCONTROL_PARA_TYPE_FREEZE_CURRENT_STATE)
	{
		
	}
	
	gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
	gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
	gIOCtrlStateBuffer.buffer[2] = req;

	
	gIOCtrlStateBuffer.buffer[3] = curState;

	gIOCtrlStateBuffer.len = 4;

	return &gIOCtrlStateBuffer;
}




const tIOCtrlBuffer* sid0x2fDidAutoTestStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len)
{
	static request_context_t ctx;
	uint8 reqData[32];
	
	uint16 io_did = 0;
	uint8 fanSpeedSet;
	uint16 fanSpeedBack;

	
	io_did = data[0] <<8 | data[1];

	
	diagApp_setRxTick(reqInfor->ch, (uint32)reqInfor->reqTick);

	if(io_did == IOCTRL_ID_SET_UUID)
	{
		memset(&socSerialNum[0],' ',20);
		memcpy(&socSerialNum[0],data+5,len-5);
		NvM_WriteBlock(socSerialNumNvMBlockID,&socSerialNum[0]);

		gIOCtrlStateBuffer.buffer[0] = 0x07;
		gIOCtrlStateBuffer.buffer[1] = 0x06;
		gIOCtrlStateBuffer.buffer[2] = 0x03;
		gIOCtrlStateBuffer.buffer[3] = 0x00;
		gIOCtrlStateBuffer.len = 4;

		return &gIOCtrlStateBuffer;
	}

	if(io_did == IOCTRL_ID_CLEAR_UUID)
	{
		memset(&socSerialNum[0],' ',20);
		NvM_WriteBlock(socSerialNumNvMBlockID,&socSerialNum[0]);

		gIOCtrlStateBuffer.buffer[0] = 0x07;
		gIOCtrlStateBuffer.buffer[1] = 0x07;
		gIOCtrlStateBuffer.buffer[2] = 0x03;
		gIOCtrlStateBuffer.buffer[3] = 0x00;
		gIOCtrlStateBuffer.len = 4;

		return &gIOCtrlStateBuffer;
	}

	if(io_did == IOCTRL_ID_CALIBRATE_AMP)
	{
		api_amp_exec_lc_filter_diagnostics();

		DelayMs(2000);

		gIOCtrlStateBuffer.buffer[0] = 0x08;
		gIOCtrlStateBuffer.buffer[1] = 0x01;
		gIOCtrlStateBuffer.buffer[2] = 0x03;
		if(api_amp_get_lc_filter_result() == 0)
			gIOCtrlStateBuffer.buffer[3] = 0x00;
		else
			gIOCtrlStateBuffer.buffer[3] = 0x01;

		gIOCtrlStateBuffer.len = 4;

		return &gIOCtrlStateBuffer;
	}

	if(io_did == IOCTRL_ID_SET_FAN_SPEED)
	{
		fanSpeedSet = data[3];
		SetFanSpeed(fanSpeedSet);
		DelayMs(100);
		fanSpeedBack = GetFanFreq();

		gIOCtrlStateBuffer.buffer[0] = 0x07;
		gIOCtrlStateBuffer.buffer[1] = 0x0F;
		gIOCtrlStateBuffer.buffer[2] = 0x03;
		gIOCtrlStateBuffer.buffer[3] = 0x00;
		gIOCtrlStateBuffer.buffer[4] = fanSpeedBack;
		gIOCtrlStateBuffer.len = 5;

		return &gIOCtrlStateBuffer;
	}

	else
	{
		gUDSBufferPtr[reqInfor->ch] = &reqInfor->data[1];

		ctx.function_id = IC_IPC_AUTOTEST_CTRL;
		ctx.channel = IPC_GW_CHANNEL_SYSTEM;
		diagCpyData(reqData, (const uint8*)data, len);
		ipc_gw_send_request(1, IPC_GW_CHANNEL_SYSTEM, IC_IPC_AUTOTEST_CTRL, reqData, len, &ctx, diagAppIpcCbk_AutoTestMcuRequest);

		
		
#if 0
		respData[0] = 0x01;
		respData[1] = 0x01;
		respData[2] = 0x03;
		respData[3] = 0x00;
		
		diagAppIpcCbk_AutoTestMcuRequest(&ctx, 14, respData, 4);
#endif
		
		return NULL;
	}
}



boolean sid0x2fDid0XF11FVerifyPara(const uint8*data, uint16 len)
{
	boolean ret = TRUE;
	
	
	

	
	if(len!=6)
		ret = FALSE;
	
	
	
	if(data[3] != 0xF1 || data[4] != 0x1F)
		ret = FALSE;
	
	
	

	return ret;
}



const tIOCtrlBuffer* sid0x2fDid0XF11FStartTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len)
{
	uint8 ioCtrlPara;
	
	

	if(data )
	{
		ioCtrlPara = data[2];
		
		
	
		
		
	
		gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
		gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
		gIOCtrlStateBuffer.buffer[2] = ioCtrlPara;
		Com_RxStop();
		
		Soc_Power_Onoff(STD_OFF);
		
		
		
		gIOCtrlStateBuffer.buffer[3] = 0x11;

		gIOCtrlStateBuffer.len = 4;

		

		return &gIOCtrlStateBuffer;
	}
	else
		return NULL;
}


const tIOCtrlBuffer* sid0x2fDid0XF11FStopTest(const tUdsReqInfor *reqInfor,tIOControlParameterType req)
{
	uint8 curState = 0;

	

	if(req == IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU
		|| req == IOCONTROL_PARA_TYPE_RESET_TO_DEFAULT_OR_INIT_VALUE)
	{
		
	}
	else if(req == IOCONTROL_PARA_TYPE_FREEZE_CURRENT_STATE)
	{
		
	}

	SysResetWrapper(GENERAL_TEST);
	
	gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
	gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
	gIOCtrlStateBuffer.buffer[2] = req;

	
	gIOCtrlStateBuffer.buffer[3] = curState;

	gIOCtrlStateBuffer.len = 4;

	return &gIOCtrlStateBuffer;
}

