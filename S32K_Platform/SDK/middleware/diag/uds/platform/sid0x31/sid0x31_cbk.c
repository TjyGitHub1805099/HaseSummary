#include"sid0x31_if.h"

tRoutineCtrlBuffer gRoutineCtrlBuffer = {
	.len = 0,
	.buffer = {1},
	};

boolean sid0x31CheckMainPreCondition()
{
	if(diagCheckSysVoltageStatus() != DIAG_SYS_VOLTAGE_STATUS_NORMAL)
		return FALSE;
	
	return TRUE;
}

//data:byte0:subCtrlType,byte1~2:routineID,byte3~more:routineCtrlOptionRecord
boolean sid0x31Did0x0101DummyVerifyData(const uint8*data,uint16 len)
{

	//check len

	
	return TRUE;
}

boolean sid0x31Did0x0101DummyCheckPrecondition(const uint8*data,uint16 len)
{
	//if precondition match,return TRUE,else return FALSE
	
	return TRUE;
}

//data:byte0:routineCtrlType,byte1~2:routineDID,byte3~more:routineCtrlOptionRecord
tRoutineCtrlBuffer* sid0x31Did0x0101DummyStartTest(const uint8*data,uint16 len)
{
	//1TODO:if necessary, forward the req to other task
	gRoutineCtrlBuffer.buffer[0] = data[0];//routineCtrlType
	gRoutineCtrlBuffer.buffer[1] = data[1];//did_high
	gRoutineCtrlBuffer.buffer[2] = data[2];//did_low
	gRoutineCtrlBuffer.buffer[3] = 0xFF;//curState

	gRoutineCtrlBuffer.len = 4;
	return &gRoutineCtrlBuffer;
}

tRoutineCtrlBuffer* sid0x31Did0x0101DummyStopTest(const uint8*data,uint16 len)
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

		return &gRoutineCtrlBuffer;
	}

	return 0;	
}

tRoutineCtrlBuffer* sid0x31Did0x0101DummyReqResult(const uint8*data,uint16 len)
{
	//1TODO:if necessary, forward the req to other task


	gRoutineCtrlBuffer.buffer[0] = data[0];//routineCtrlType
	gRoutineCtrlBuffer.buffer[1] = data[1];//did_high
	gRoutineCtrlBuffer.buffer[2] = data[2];//did_low
	gRoutineCtrlBuffer.buffer[3] = 0xF1;//result 1
	gRoutineCtrlBuffer.buffer[4] = 0xF2;//result 2
	gRoutineCtrlBuffer.buffer[5] = 0xF3;//result 3

	gRoutineCtrlBuffer.len = 6;

	return &gRoutineCtrlBuffer;
}

