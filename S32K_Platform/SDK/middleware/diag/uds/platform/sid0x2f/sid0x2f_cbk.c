#include"sid0x2f_if.h"

tIOCtrlBuffer gIOCtrlStateBuffer = {
	.len = 0,
	.buffer = {1},
	};

boolean sid0x2fCheckMainPreCondition()
{
	//2TODO: general check. this depends on project requirement
	if(diagCheckPreCondtionGroup2() == FALSE)
		return FALSE;
	return TRUE;
}


boolean sid0x2fCheckSpecificPrecondition(const tIOCtrlInfo* ioctrlInfor)
{
	boolean ret = TRUE;

	if(ioctrlInfor)
	{
		//2TODO: 2. special check,depends on project requirement
		switch(ioctrlInfor->did)
		{
		case IOCTRL_ID_DUMMY_TEST:
			//1TODO:check test precondition for this DID
		#if 0
			if precondition NOT match
				ret = FALSE;
		#endif
			break;
		#if 0
		case IOCT_ID_MORE//add more DIDs here
			//check precondition
			break;
		#endif
		default:
			break;
		}

	}

	
	return ret;
}



//para: data: byte0~1: DID, byte2:ioCtrlPara, byte3~more:ioCtrlState[if appear]
boolean sid0x2fDid0X0101DummyVerifyPara(const uint8*data, uint16 len)
{
	boolean ret = TRUE;
	uint8 ioCtrlPara;
	uint8 ioCtrlState1;
	uint8 ioCtrlState2;

	//1.check if len is valid
	if(len!=4)
		ret = FALSE;
	//2.parse state
	ioCtrlState1 = data[2];
	
	//parse rest stats if more
	
	//3.parse mask if apply

	return ret;
}


//this cbk can be NULL if you have no special logic for this ioInputOutput
//para: data: byte0~1: DID, byte2:ioCtrlPara, byte3~more:ioCtrlState[if appear]
tUdsNRCType sid0x2fDid0X0101DummyCheckExtraDataOrState(const uint8*data, uint16 len)
{
	tUdsNRCType ret = NRC_NONE;

	//if you want to check extra data or state,you can add you logic here.
	#if 0
	//example,if customer req to return NRC_REQUEST_SEQUENCE_ERROR if this ioCtrl test had been started,
	//but yet not stopted,we still req to start again
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


//para: data:byte0~byte1: DID, byte2:ioCtrlPara[returnControlToEcu or shortAdjuectment], byte3~more:ioCtrlState
const tIOCtrlBuffer* sid0x2fDid0X0101DummyStartTest(const uint8*data,uint16 len)
{
	uint8 ioCtrlPara;
	uint8 state1;
	uint8 state2;

	if(data )
	{
		ioCtrlPara = data[2];
		state1 = data[3];
		state2 = data[4];
	
		//1TODO:start the test here directly  if test doesn't pend UDS task
		//1TODO:else, forward these states to another task
	
		gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
		gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
		gIOCtrlStateBuffer.buffer[2] = ioCtrlPara;

		//1TODO:set the runtime state,here is dummy data
		gIOCtrlStateBuffer.buffer[3] = 0x11;
		gIOCtrlStateBuffer.buffer[4] = 0x22;
		gIOCtrlStateBuffer.buffer[5] = 0x33;
		gIOCtrlStateBuffer.buffer[6] = 0x44;

		gIOCtrlStateBuffer.len = 7;

		//1TODO:if can't trigger test successfully, return NULL

		return &gIOCtrlStateBuffer;
	}
	else
		return NULL;
}

//para: data:byte0~byte1: DID, byte2:ioCtrlPara[returnControlToEcu or shortAdjuectment], byte3~more:ioCtrlState
const tIOCtrlBuffer* sid0x2fDid0X0101DummyStopTest(tIOControlParameterType req)
{
	uint8 curState = 0;

	//2TODO: forward the req to other task if necessary

	if(req == IOCONTROL_PARA_TYPE_RETURN_CONTROL_TO_ECU
		|| req == IOCONTROL_PARA_TYPE_RESET_TO_DEFAULT_OR_INIT_VALUE)
	{
		//2TODO:set the curState to default value
	}
	else if(req == IOCONTROL_PARA_TYPE_FREEZE_CURRENT_STATE)
	{
		//1TODO: keep current state
	}
	
	gIOCtrlStateBuffer.buffer[0] = (uint8)((IOCTRL_ID_DUMMY_TEST & 0xFF00)>>8);
	gIOCtrlStateBuffer.buffer[1] = (uint8)(IOCTRL_ID_DUMMY_TEST & 0xFF);
	gIOCtrlStateBuffer.buffer[2] = req;

	//1TODO:return the runtime state
	gIOCtrlStateBuffer.buffer[3] = curState;

	gIOCtrlStateBuffer.len = 4;

	return &gIOCtrlStateBuffer;
}



