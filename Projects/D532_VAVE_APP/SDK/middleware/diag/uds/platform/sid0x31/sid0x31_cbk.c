#include"sid0x31_if.h"
#include"sid0x31_cfg.h"
#include "Task_M_IpcApp_Cfg.h" 
#include "diagAppIpc_cbk.h"
#include "trace.h"
#include "msg_def.h"

extern const tRoutineCtlInfor *gRoutineCtlInfors[];
extern uint8 gRoutineCtrlTotalNum[];
extern uint16 gRespDataLen[];
extern tRoutineCtrlStatusInfor *gRoutinueCtrlStatusArray[];

extern uint8 *gUDSBufferPtr[];


tRoutineCtrlBuffer gRoutineCtrlBuffer = {
	.len = 0,
	.buffer = {1},
};

uint32 gRoutinueCtrlRxReqTick = 0;
boolean gAVM_CalibrationFinish = FALSE;
boolean gAVM_CalibrationInProgress = FALSE;

uint8	gAVM_CalibrationType = 0;	  //0: Factory; 1: After-Sale
//uint8 	gAVM_ErrInfor[2] = {0}; //default camera status 3rd byte: 0=NG
uint8 	gAVM_CalibrationState[AVM_DID_CALIBRATION_STATE_LEN] = {0,0};

boolean gHUD_ResetFinish[6] = {FALSE};
uint8 	gHUD_ResetState[6] = {0};

boolean gHUD_Display_On = FALSE;

boolean sid0x31CheckMainPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return FALSE;
	
	return TRUE;
}

tUdsNRCType sid0x31Check0xDF(tUdsReqInfor *reqInfor)
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

	if(gRoutineCtlInfors[ch]!=NULL)
	{
		if(dataPtr[1] == 1 && dataPtr[2] == 0xDF)
		{
			if(len == 4)
			{
				diagResetBuffer(dataPtr+4, 0, 32);
				did_high_byte = dataPtr[3];
				
				for(i=0;i<gRoutineCtrlTotalNum[ch];i++)
				{
					if(did_high_byte == 0xDF)//get all high byte of didArray
					{
						if((gRoutineCtlInfors[ch][i].routineId & 0xFF00) != 0xFE00)
						{
							byteValue = (uint8)((gRoutineCtlInfors[ch][i].routineId & 0xFF00)>>8);
							matchFlag = TRUE;
						}
					}
					else//get all low byte of didArray
					{
						if(did_high_byte == (uint8)((gRoutineCtlInfors[ch][i].routineId & 0xFF00)>>8))
						{
							byteValue = (uint8)(gRoutineCtlInfors[ch][i].routineId & 0xFF);
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

				//for 0xDFDF
				if(did_high_byte == 0xDF)
				{
					byteNum = 0xDF/8;
					bitNum = 7-byteValue%8;
					//dataPtr[3+byteNum] |= 1<<bitNum;
					dataPtr[4+byteNum] |= 1;
				}

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


//data:byte0:subCtrlType,byte1~2:routineID,byte3~more:routineCtrlOptionRecord
boolean sid0x31Did0x0101DummyVerifyData(const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{

	//check len

	
	return TRUE;
}



boolean sid0x31Did0x0101DummyCheckPrecondition(const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	//if precondition match,return TRUE,else return FALSE
	
	return TRUE;
}

//data:byte0:routineCtrlType,byte1~2:routineDID,byte3~more:routineCtrlOptionRecord
tUdsNRCType sid0x31Did0x0101DummyStartTest(const tUdsReqInfor *reqInfor, const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	//1TODO:if necessary, forward the req to other task
	gRoutineCtrlBuffer.buffer[0] = data[0];//routineCtrlType
	gRoutineCtrlBuffer.buffer[1] = data[1];//did_high
	gRoutineCtrlBuffer.buffer[2] = data[2];//did_low
	gRoutineCtrlBuffer.buffer[3] = 0xFF;//curState

	gRoutineCtrlBuffer.len = 4;

	*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;
}

tUdsNRCType sid0x31Did0x0101DummyStopTest(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	//1TODO:if necessary, forward the req to other task
	//CAUTION:
	//if stop test by uds internally[etg. switch to defaultSession], the data will be NULL and len is 0
	if(data!=0)
	{
		gRoutineCtrlBuffer.buffer[0] = data[0];//routineCtrlType
		gRoutineCtrlBuffer.buffer[1] = data[1];//did_high
		gRoutineCtrlBuffer.buffer[2] = data[2];//did_low
		gRoutineCtrlBuffer.buffer[3] = 0xF0;//result

		gRoutineCtrlBuffer.len = 4;

		*respBufPtr = &gRoutineCtrlBuffer;
	}

	return NRC_NONE;
}

tUdsNRCType sid0x31Did0x0101DummyReqResult(const tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	//1TODO:if necessary, forward the req to other task


	gRoutineCtrlBuffer.buffer[0] = data[0];//routineCtrlType
	gRoutineCtrlBuffer.buffer[1] = data[1];//did_high
	gRoutineCtrlBuffer.buffer[2] = data[2];//did_low
	gRoutineCtrlBuffer.buffer[3] = 0xF1;//result 1
	gRoutineCtrlBuffer.buffer[4] = 0xF2;//result 2
	gRoutineCtrlBuffer.buffer[5] = 0xF3;//result 3

	gRoutineCtrlBuffer.len = 6;
	*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;
}


//Simulation mode calibration of AVM through DA
tUdsNRCType sid0x31Did0x01FEStartAVMCalibrationSIMU(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{

	static request_context_t ctx;
	uint8 uReqData[4];
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;

	gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester

	if(reqInfor->data[3] == 0)
	{
		gAVM_CalibrationFinish = FALSE;
		gAVM_CalibrationInProgress = TRUE;
		gAVM_CalibrationState[0] = 0;	//idle
		gAVM_CalibrationState[1] = 0;	//idle
		gAVM_CalibrationType = (uint8)(ROUTINE_AVM_SIMU_CALIBRATION_REQ&0xFF);	//00: SIMULATION calibration
	}
	uReqData[0] = reqInfor->data[3] + 1;
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 01FE send to AVM via IPC!\r\n");
	
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_DIAGNOSTIC_CTRL_REQ, uReqData, 1); //len = 1

	//send response 71 01 FE 0x 22
	gRoutineCtrlBuffer.buffer[0] = 1;
	gRoutineCtrlBuffer.buffer[1] = 0xFE;
	gRoutineCtrlBuffer.buffer[2] = reqInfor->data[3];
	gRoutineCtrlBuffer.buffer[3] = 0x22;
	gRoutineCtrlBuffer.len = 4;

	*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;


}



//Factory mode calibration of AVM through DA
// data: control type 01, routine iddentifier high byte 02, low byte 05/06
tUdsNRCType sid0x31Did0x0105StartAVMCalibrationFAC(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	static request_context_t ctx;
	uint8 uReqData[4];
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;

	//ctx.channel = IPC_CHANNEL_DIAG;
	//ctx.function_id = AVM_IPC_DIAG_FUNCTION_CALIBRATION;
	gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester
	gAVM_CalibrationFinish = FALSE;
	gAVM_CalibrationInProgress = TRUE;
	gAVM_CalibrationState[0] = 0;	//idle
	gAVM_CalibrationState[1] = 0;	//idle
	gAVM_CalibrationType = (uint8)(ROUTINE_AVM_FAC_CALIBRATION_REQ&0xFF);	//5: factory calibration

	uReqData[0] = 1;
	uReqData[1] = 0x05;
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 0205 send to AVM via IPC!\r\n");
	//ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 2, &ctx, diagApp_AVM_IpcResp); //len = 2
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 2); //len = 2

    //send response 71 01 02 05 22
	gRoutineCtrlBuffer.buffer[0] = 1;
	gRoutineCtrlBuffer.buffer[1] = 2;
	gRoutineCtrlBuffer.buffer[2] = 5;
	gRoutineCtrlBuffer.buffer[3] = 0x22;
	gRoutineCtrlBuffer.len = 4;

	*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;

}

//After sale mode calibration of AVM, send to DA
tUdsNRCType sid0x31Did0x0106StartAVMCalibrationSALE(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	static request_context_t ctx;
	uint8 uReqData[4];

	//ctx.channel = IPC_CHANNEL_DIAG;
	//ctx.function_id = AVM_IPC_CALIBRATION;
	gRoutinueCtrlRxReqTick = reqInfor->reqTick;
	gAVM_CalibrationFinish = FALSE;
	gAVM_CalibrationInProgress = TRUE;
	gAVM_CalibrationState[0] = 0;	//idle
	gAVM_CalibrationState[1] = 0;	//idle
	gAVM_CalibrationType = (uint8)(ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ&0xFF);	//6:after-sale calibration

	uReqData[0] = 1;
	uReqData[1] = 0x06;
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 0206 send to AVM via IPC!\r\n");
	//ipc_gw_send_request(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 2, &ctx, diagApp_AVM_IpcResp);
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_AVM, AVM_IPC_CALIBRATION_REQ, uReqData, 2); //len = 2

	//send response 71 01 02 06 22
	gRoutineCtrlBuffer.buffer[0] = 1;
	gRoutineCtrlBuffer.buffer[1] = 2;
	gRoutineCtrlBuffer.buffer[2] = 6;
	gRoutineCtrlBuffer.buffer[3] = 0x22;
	gRoutineCtrlBuffer.len = 4;

	*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;

}

tUdsNRCType sid0x31AVMCalibrationReqResult(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	gRoutinueCtrlRxReqTick = reqInfor->reqTick;

	//uint16 routineId;
	//routineId = (uint16)((reqInfor->data[2]<<8) | (reqInfor->data[3]));
	if(!gAVM_CalibrationFinish || gAVM_CalibrationType!=reqInfor->data[3])
	{
		TracePrintf(UDS_TRACE, TRACE_INFO, "AVM Calibration not finished!\r\n");
		*respBufPtr = &gRoutineCtrlBuffer;
		if(gAVM_CalibrationInProgress == FALSE)
			return NRC_REQUEST_SEQUENCE_ERROR;
		else
			return NRC_BUSY_REPEAT_REQ;
	}
	else
	{
		gRoutineCtrlBuffer.buffer[0] = 3;
		gRoutineCtrlBuffer.buffer[1] = 2;
		gRoutineCtrlBuffer.buffer[2] = reqInfor->data[3];//5 or 6

		if(gAVM_CalibrationState[0] == AVM_CALIBRATION_STATUS_SUCCESS)
			gRoutineCtrlBuffer.buffer[3] = 0x20;
		else if(gAVM_CalibrationState[0] == AVM_CALIBRATION_STATUS_FAILED)
			gRoutineCtrlBuffer.buffer[3] = 0x21;
		gRoutineCtrlBuffer.len = 4;

		*respBufPtr = &gRoutineCtrlBuffer;

		//sid0x31SetStatus(reqInfor->ch,routineId,ROUTINE_CTRL_TYPE_STOP_ROUTINE);
#if 0
		if(routineId == ROUTINE_AVM_FAC_CALIBRATION_REQ)
			gRoutinueCtrlStatusArray[reqInfor->ch][0].status = ROUTINE_CTRL_STATUS_STOP;
		else if(routineId == ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ)
			gRoutinueCtrlStatusArray[reqInfor->ch][1].status = ROUTINE_CTRL_STATUS_STOP;
#endif
		return NRC_NONE;

	}

}

tUdsNRCType sid0x31AVMCalibrationReqResultSIMU(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	gRoutinueCtrlRxReqTick = reqInfor->reqTick;

	//uint16 routineId;
	//routineId = (uint16)((reqInfor->data[2]<<8) | (reqInfor->data[3]));
	if(!gAVM_CalibrationFinish || gAVM_CalibrationType!=reqInfor->data[3])
	{
		TracePrintf(UDS_TRACE, TRACE_INFO, "AVM Calibration not finished!\r\n");
		*respBufPtr = &gRoutineCtrlBuffer;
		if(gAVM_CalibrationInProgress == FALSE)
			return NRC_REQUEST_SEQUENCE_ERROR;
		else
			return NRC_BUSY_REPEAT_REQ;
	}
	else
	{
		gRoutineCtrlBuffer.buffer[0] = 3;
		gRoutineCtrlBuffer.buffer[1] = 0xFE;
		gRoutineCtrlBuffer.buffer[2] = reqInfor->data[3];//5 or 6

		if(gAVM_CalibrationState[0] == AVM_CALIBRATION_STATUS_SUCCESS)
			gRoutineCtrlBuffer.buffer[3] = 0x20;
		else if(gAVM_CalibrationState[0] == AVM_CALIBRATION_STATUS_FAILED)
			gRoutineCtrlBuffer.buffer[3] = 0x21;
		gRoutineCtrlBuffer.len = 4;

		*respBufPtr = &gRoutineCtrlBuffer;

		//sid0x31SetStatus(reqInfor->ch,routineId,ROUTINE_CTRL_TYPE_STOP_ROUTINE);
#if 0
		if(routineId == ROUTINE_AVM_FAC_CALIBRATION_REQ)
			gRoutinueCtrlStatusArray[reqInfor->ch][0].status = ROUTINE_CTRL_STATUS_STOP;
		else if(routineId == ROUTINE_AVM_AFTERSALE_CALIBRATION_REQ)
			gRoutinueCtrlStatusArray[reqInfor->ch][1].status = ROUTINE_CTRL_STATUS_STOP;
#endif
		return NRC_NONE;

	}

}




boolean sid0x31Did0x010506AVMVerifyData(const uint8*data,uint16 len)
{
	//check len
	if(len > 3)
		return FALSE;
	else
		return TRUE;
}



tUdsNRCType sid0x31HudResetStart(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{

	uint8 uReqData[len+1];
	uint8 uUserId;
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;


	gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester
	uUserId = data[2]-1;
	gHUD_ResetFinish[uUserId] = FALSE;
	gHUD_ResetState[uUserId] = 0;	//idle


	uReqData[0] = 0x31;
	uReqData[1] = 0x01;
	uReqData[2] = 0x03;
	uReqData[3] = data[2];
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 Reset command send to HUD via IPC!\r\n");
	ipc_gw_send_notification_noAck(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_CALIBRATION_OR_RESET_REQ, uReqData, len+1); //len = 4

	//send response 71 01 03 0x 22
	gRoutineCtrlBuffer.buffer[0] = 1;
	gRoutineCtrlBuffer.buffer[1] = 3;
	gRoutineCtrlBuffer.buffer[2] = data[2];
	gRoutineCtrlBuffer.buffer[3] = 0x22;
	gRoutineCtrlBuffer.len = 4;

	*respBufPtr = &gRoutineCtrlBuffer;


	//diagAppIpcCbk_SocNotify_IVI(0x31,uReqData,1); //for test only
	//for test only, should be removed when release!!!
	/*	uint8 rx_data[5];
		rx_data[0] = 0x71;
		rx_data[1] = 1;
		rx_data[2] = 3;
		rx_data[3] = 1;
		rx_data[4] = 20;
		diagApp_HUD_IpcNotifyInd(0x31, rx_data, 5);
	*/
	//
	return NRC_NONE;

}

tUdsNRCType sid0x31HudResetConfirm(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{

	gRoutinueCtrlRxReqTick = reqInfor->reqTick;
	
	uint8 uUserID;
	uUserID = data[2]-1;
	if(!gHUD_ResetFinish[uUserID])
	{
		//TracePrintf(UDS_TRACE, TRACE_INFO, "AVM Calibration not finished!\r\n");
		*respBufPtr = &gRoutineCtrlBuffer;
		return NRC_BUSY_REPEAT_REQ;
	}
	else
	{
		gRoutineCtrlBuffer.buffer[0] = 3;
		gRoutineCtrlBuffer.buffer[1] = 3;
		gRoutineCtrlBuffer.buffer[2] = data[2]; //user 
		gRoutineCtrlBuffer.buffer[3] = gHUD_ResetState[uUserID];

		gRoutineCtrlBuffer.len = 4;
		*respBufPtr = &gRoutineCtrlBuffer;

		return NRC_NONE;
	}

}
tUdsNRCType sid0x31HudCalibrationDisplayStart(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	uint8 uDataLen = (len-3)/2;
	uint8 uReqData_1[uDataLen+4];
	uint8 uReqData_2[uDataLen+4];
	
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;

	static request_context_t ctx;

	gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester

	diagApp_setRxTick(reqInfor->ch, (uint32)reqInfor->reqTick);

	uReqData_1[0] = 0x31;
	uReqData_1[1] = 0x01;
	uReqData_1[2] = 0x03;
	uReqData_1[3] = 0x09;
	diagCpyData(uReqData_1+4, data+3,uDataLen);

	uReqData_2[0] = 0x31;
	uReqData_2[1] = 0x01;
	uReqData_2[2] = 0x03;
	uReqData_2[3] = 0x09;
	diagCpyData(uReqData_2+4, data+3+uDataLen,uDataLen);
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 Calibration Display command send to HUD via IPC!\r\n");
	
	ctx.function_id = HUD_IPC_CALIBRATION_OR_RESET_REQ;
	ctx.channel = IPC_GW_CHANNEL_DIAG_IVI;

	gUDSBufferPtr[reqInfor->ch] = &reqInfor->data[1];

	ipc_gw_send_request(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_CALIBRATION_OR_RESET_REQ, uReqData_1, uDataLen+4, &ctx, diagAppIpcCbk_HUDCalibrationMcuRequest);
	DelayMs(10);
	ipc_gw_send_request(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_CALIBRATION_OR_RESET_REQ, uReqData_2, uDataLen+4, &ctx, diagAppIpcCbk_HUDCalibrationMcuRequest);
	
	gHUD_Display_On = TRUE;

	//For test only, must delete when release
/*
	uint8 respData[5];
	respData[0] = 0x71;
	respData[1] = 1;
	respData[2] = 0x03;
	respData[3] = 0x09;
	respData[4] = 0x30;

	diagAppIpcCbk_HUDCalibrationMcuRequest(&ctx, 0x31, respData,5);
*/
	//

	return NRC_NONE;
}

tUdsNRCType sid0x31HudCalibrationDisplayStatus(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	uint8 uReqData[len+1];
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;

	static request_context_t ctx;


	gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester

	diagApp_setRxTick(reqInfor->ch, (uint32)reqInfor->reqTick);

	uReqData[0] = 0x31;
	uReqData[1] = 0x03;
	uReqData[2] = 0x03;
	uReqData[3] = 0x09;
	TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 Check calibration status command send to HUD via IPC!\r\n");
	
	ctx.function_id = HUD_IPC_CALIBRATION_OR_RESET_REQ;
	ctx.channel = IPC_GW_CHANNEL_DIAG_IVI;

	gUDSBufferPtr[reqInfor->ch] = &reqInfor->data[1];

	ipc_gw_send_request(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_CALIBRATION_OR_RESET_REQ, uReqData, len+1, &ctx, diagAppIpcCbk_HUDCalibrationMcuRequest);
	
	//For test only, must delete when release
		/*
		uint8 respData[5];
		respData[0] = 0x71;
		respData[1] = 0x03;
		respData[2] = 0x03;
		respData[3] = 0x09;
		respData[4] = 0x32;

		diagAppIpcCbk_HUDCalibrationMcuRequest(&ctx, 0x31, respData,5);
		*/
	//
    
	return NRC_NONE;
}

tUdsNRCType sid0x31HudCalibrationDisplayStop(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	uint8 uReqData[len+1];
	tRoutineCtrlBuffer * tmpBuffPtr = NULL;

	static request_context_t ctx;
    if(gHUD_Display_On == TRUE)
    {

		gRoutinueCtrlRxReqTick = reqInfor->reqTick;  //used for checking when response to the requester
		diagApp_setRxTick(reqInfor->ch, (uint32)reqInfor->reqTick);

		uReqData[0] = 0x31;
		uReqData[1] = 0x02;
		uReqData[2] = 0x03;
		uReqData[3] = 0x09;
		TracePrintf(UDS_TRACE, TRACE_INFO, "diag $31 Stop calibration screen command send to HUD via IPC!\r\n");
		
		ctx.function_id = HUD_IPC_CALIBRATION_OR_RESET_REQ;
		ctx.channel = IPC_GW_CHANNEL_DIAG_IVI;

		gUDSBufferPtr[reqInfor->ch] = &reqInfor->data[1];

		ipc_gw_send_request(1, IPC_GW_CHANNEL_DIAG_IVI, HUD_IPC_CALIBRATION_OR_RESET_REQ, uReqData, len+1, &ctx, diagAppIpcCbk_HUDCalibrationMcuRequest);

		gHUD_Display_On = FALSE;
		
		//For test only, must delete when release
		/*
			uint8 respData[5];
			respData[0] = 0x71;
			respData[1] = 0x02;
			respData[2] = 0x03;
			respData[3] = 0x09;
			respData[4] = 0x30;

			diagAppIpcCbk_HUDCalibrationMcuRequest(&ctx, 0x31, respData,5);
		*/
		//
    }
	return NRC_NONE;
}

tUdsNRCType sid0x31HudSelfCheckDTC(tUdsReqInfor *reqInfor,const uint8*data,uint16 len,tRoutineCtrlBuffer** respBufPtr)
{
	//send response 71 01 03 0x20
		gRoutineCtrlBuffer.buffer[0] = 1;
		gRoutineCtrlBuffer.buffer[1] = 3;
		gRoutineCtrlBuffer.buffer[2] = data[2];
		gRoutineCtrlBuffer.buffer[3] = 0x20;
		gRoutineCtrlBuffer.len = 4;
	
		*respBufPtr = &gRoutineCtrlBuffer;

	return NRC_NONE;
}


