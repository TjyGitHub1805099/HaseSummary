/**************************
description: UDS session layer
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2016/12/30

change list:
2017/06/02:
only keep one UDS rx buffer for phy and func address
**************************/

#include"uds_if.h"
#include"uds_sid_if.h"
#include "tp_if.h"

#ifdef UDS_WITH_SID0X11
#include"sid0x11_if.h"
#endif

#ifdef UDS_WITH_SID0X10
#include"sid0x10_if.h"
#endif

#ifdef UDS_WITH_SID0X27
#include"sid0x27_if.h"
#endif

#ifndef SW_PHASE_APPLICATION
extern void jmp_app_prog(uint32 addr);
#endif

static boolean udsSetActiveChStatus(uint32 ch,tUdsChStatus status);
static boolean udsSetTimer(uint32 ch,tUdsTimerType timerType);

static void udsResetConnection(uint32 ch);
static void udsBroadcastCnnStatus(boolean status);
static void udsPaddingNRC(uint32 ch,uint8 sid,tUdsNRCType nrc);
static void udsResetActiveChStatus(uint32 ch);
static void udsChangeSessionValue(uint32 ch,uint32 sessionValue);

extern const tDiagSidServiceInfor *gDiagSidService[];
extern const tUdsCfgInfor gUdsCfgInfor;
extern tUdsTimer gUdsSessionTimer[];
extern tUdsChInfor gUdsChInfor[];
extern uint8 sUDSNRCBuffer[][3];
extern uint16 gRespDataLen[];
extern uint32 gUdsRxNewDataTick[];
extern uint8 gSid[];
extern boolean gSuppressPosRespBit[];
extern boolean gRespNRC0x78Flag[];
extern uint8 gDiagRespCode[];
extern tDiagMutex gUdsSidMutex[];
extern boolean gEnableInnerSidFlag;
extern boolean gTpTraceByCanFlag;

#ifdef UDS_WITH_SID0X10
extern uint32 gUdsSessionType[];
#endif

boolean gUdsChSupportFlag[10];//to compliant with DNTC 532 request,any ch may be not supported by dynamic checking EQ PIN

boolean gForwardReqToDiagAppFlag = FALSE;

#define UDS_CH_TMP_RESP_FLAG(ch) gUdsChInfor[ch].tmp_resp_flag
#define UDS_CH_BUFFER(ch)	gUdsChInfor[ch].buffer
#define UDS_CH_STATUS(ch)		gUdsChInfor[ch].chStatus
#define UDS_CH_DATA_LEN(ch)		gUdsChInfor[ch].dataLen
#define UDS_CH_TYPE(ch)		gUdsChInfor[ch].udsChType
#define UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(ch) gUdsChInfor[ch].reqFromFuncAddrFlag

#define UDS_CH_BUFF_MAXLEN(ch)	gUdsChInfor[ch].bufferMaxLen
#define UDS_CH_ID(ch)	gUdsChInfor[ch].udsChType
#define UDS_CH_PRIO(ch)		gUdsChInfor[ch].chPrio

#define UDS_RESP_NEGATIVE 0x7F
#define UDS_CH_MAX 2

static tUdsResetSysType gUdsResetSysType = UDS_NONE_RESET_TYPE;
static boolean gDiagCnnIsActiveFlag = FALSE;
static boolean gUdsInitFinishedFlag = FALSE;


const tUdsCfgInfor *udsGetGlobalCfgInfor()
{
	return &gUdsCfgInfor;
}

boolean udsGetChStatus(uint32 ch,tUdsChStatus *retStatus)
{
	if(ch>gUdsCfgInfor.maxChNum || retStatus == NULL)
		return FALSE;

	*retStatus = UDS_CH_STATUS(ch);
	return TRUE;
}

void udsSetResetSysType(tUdsResetSysType resetTriggerType)
{
	gUdsResetSysType = resetTriggerType;
}

uint32 udsGetResetSysType()
{
	return gUdsResetSysType;
}

boolean udsSetTimer(uint32 ch,tUdsTimerType timerType)
{
	switch(timerType)
	{
	case UDS_TIMER_TYPE_NONE:
		gUdsSessionTimer[ch].interval= 0;
		break;
	case UDS_TIMER_TYPE_P2_SERVER_MAX:
		gUdsSessionTimer[ch].interval = gUdsCfgInfor.p2ServerMaxTimerValue;
		break;
	case UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED:
		gUdsSessionTimer[ch].interval = gUdsCfgInfor.p2ServerMaxEnhancedTimerValue;
		break;
	case UDS_TIMER_TYPE_S3_SERVER:
		gUdsSessionTimer[ch].interval = gUdsCfgInfor.s3ServerTimerValue;
		break;
	default:
		return FALSE;//error
	}

	if(gUdsCfgInfor.udsLogLevel >= UDS_LOG_LEVEL_3)
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"UDS:setTimer:%d\r\n",timerType);
	gUdsSessionTimer[ch].type = timerType;
	gUdsSessionTimer[ch].timeoutFlag = FALSE;
	gUdsSessionTimer[ch].preTick = diagGetCurTick();

	return TRUE;
}

#if 0
boolean udsRxIndication(uint32 ch,uint8* dataPtr, uint16 len)
{
	uint8 sid = 0;
	boolean supressPosResp = FALSE;
	uint8 i = 0;
	uint8 chBusy = 0;
	tUdsNRCType ret = NRC_NONE;
	tUDSAddrType ch_type;

	if( (ch>gUdsCfgInfor.maxChNum) || (ch>0 && len >UDS_CH_BUFF_MAXLEN(ch-1))
		|| gUdsInitFinishedFlag==FALSE ||len ==0||dataPtr == NULL)
		return FALSE;

	sid = dataPtr[0];
	supressPosResp = dataPtr[1]&0x80 ? TRUE : FALSE;
	ch_type = (ch == 0)?UDS_ADDR_TYPE_FUNC:UDS_ADDR_TYPE_PHY;

	if(sid== 0x3E&& supressPosResp== TRUE)
	{
		udsBroadcastCnnStatus(TRUE);
		if(len!=2 || (dataPtr[1] != 0x80))//if NRC,still need to resp
		{
			if(len!=2)
				ret = NRC_INCORRECT_MESSAGE_LENGTH;
			else
				ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;
			// if rx 0x3e 82 with func addr,no NRC will be sent out
			if(ch_type == UDS_ADDR_TYPE_FUNC && ret == NRC_SUB_FUNCTION_NOT_SUPPORTED) 
      			return TRUE;
		}
		#ifdef UDS_WITH_SID0X10
		else
		{
			for(i=0;i<gUdsCfgInfor.maxChNum;i++)
			{
				if((sid0x10GetCurSession(i) != UDS_DEFAULT_SESSION) && (gUdsSessionTimer[i].type==UDS_TIMER_TYPE_S3_SERVER))
				{
					udsSetTimer(i,UDS_TIMER_TYPE_S3_SERVER);
				}
			}
			return TRUE;
		}
		#endif
	}

	if(ret == NRC_NONE && len<8)//SF
	{		
		if(ch_type == UDS_ADDR_TYPE_FUNC)
		{
			for(i=0;i<gUdsCfgInfor.maxChNum;i++)
			{
				if(UDS_CH_STATUS(i)!= UDS_CH_STATUS_IDLE && UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(i) == FALSE)
				{
					ret = NRC_BUSY_REPEAT_REQ;
					chBusy = i;
					break;
				}
			}
		}
	}

	if(ch >0)
		ch -= 1;
	if(ret == NRC_NONE)
	{
		if(len<8)
		{
			if(ch_type == UDS_ADDR_TYPE_FUNC)
			{
				for(i=0;i<gUdsCfgInfor.maxChNum;i++)
				{
					//udsResetConnection(i);
					udsResetActiveChStatus(i);
					diagCpyData(UDS_CH_BUFFER(i),dataPtr,len);
					UDS_CH_DATA_LEN(i) = len;
					UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(i) = TRUE;
					UDS_CH_STATUS(i) = UDS_CH_STATUS_NEW_DATA;
					udsSetTimer(i,UDS_TIMER_TYPE_P2_SERVER_MAX);
				}
			}
		}

		if(ch_type == UDS_ADDR_TYPE_PHY)
		{
			//udsResetConnection(ch);
			udsResetActiveChStatus(ch);
			UDS_CH_DATA_LEN(ch) = len;
			UDS_CH_STATUS(ch) = UDS_CH_STATUS_NEW_DATA;
			diagCpyData(UDS_CH_BUFFER(ch),dataPtr,len);
			udsSetTimer(ch,UDS_TIMER_TYPE_P2_SERVER_MAX);
		}

	}
	else
	{
		if(gUdsCfgInfor.enableRespNrcIfChBusy == TRUE)
		{
			if(ch_type == UDS_ADDR_TYPE_FUNC && ret==NRC_BUSY_REPEAT_REQ)
				ch = chBusy;
			UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
			udsPaddingNRC(ch,sid,ret);
			if(Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3) == FALSE)
			{
				UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
				DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_ERRO,"uds:TP busy,NRC %x,ch phy\r\n",ret);				
				return TRUE;
			}
		}

		UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
		udsPaddingNRC(ch,sid,ret);
		Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3);
	}
	return TRUE;
}
#else
boolean udsRxIndication(uint32 ch, uint8* dataPtr, uint16 len)
{
	uint8 sid = 0;
	boolean supressPosResp = FALSE;
	uint8 i = 0;
	uint8 chBusy = 0;
	tUdsNRCType ret = NRC_NONE;
	tUDSAddrType ch_type;

	if( (ch>gUdsCfgInfor.maxChNum) || (ch>0 && len >UDS_CH_BUFF_MAXLEN(ch-1))
		|| gUdsInitFinishedFlag==FALSE ||len ==0||dataPtr == NULL)
		return FALSE;

	sid = dataPtr[0];
	
	if(ch == 0)
	{
		ch_type = UDS_ADDR_TYPE_FUNC;
	}
	else
	{
		ch_type = UDS_ADDR_TYPE_PHY;
		ch -= 1;
		if(ch<10 && gUdsChSupportFlag[ch] == FALSE)
			return FALSE;
	}
	
	if(sid == 0x3E)
	{
		if((dataPtr[1] != 0) && (dataPtr[1] != 0x80))
			ret = NRC_SUB_FUNCTION_NOT_SUPPORTED;

		if(len != 2)
			ret = NRC_INCORRECT_MESSAGE_LENGTH;

		// if rx 0x3e 82 with func addr,no NRC will be sent out
		if(ch_type == UDS_ADDR_TYPE_FUNC && ret == NRC_SUB_FUNCTION_NOT_SUPPORTED) 
			return TRUE;

		if(dataPtr[1] == 0x80 && len == 2)
			supressPosResp = TRUE;
		
		udsBroadcastCnnStatus(TRUE);

		if(supressPosResp == TRUE)
		{	
			#ifdef UDS_WITH_SID0X10
			for(i=0;i<gUdsCfgInfor.maxChNum;i++)
			{
				if((sid0x10GetCurSession(i) != UDS_DEFAULT_SESSION) && (gUdsSessionTimer[i].type==UDS_TIMER_TYPE_S3_SERVER))
				{
					udsSetTimer(i,UDS_TIMER_TYPE_S3_SERVER);
				}
			}
			#endif
			return TRUE;
		}

		if(ret!=NRC_NONE)
		{
			UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
			udsPaddingNRC(ch, sid, ret);
			Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3);

			return TRUE;
		}
	}
	
	if(ret == NRC_NONE && len<8)//SF
	{		
		if(ch_type == UDS_ADDR_TYPE_FUNC)
		{
			for(i=0;i<gUdsCfgInfor.maxChNum;i++)
			{
				if(gUdsChSupportFlag[i] == FALSE)
					continue;
				if(UDS_CH_STATUS(i)!= UDS_CH_STATUS_IDLE && UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(i) == FALSE)
				{
					ret = NRC_BUSY_REPEAT_REQ;
					chBusy = i;
					break;
				}
			}
		}
	}

	
	if(ret == NRC_NONE)
	{
		if(len<8)
		{
			if(ch_type == UDS_ADDR_TYPE_FUNC)
			{
				for(i=0;i<gUdsCfgInfor.maxChNum;i++)
				{
					if(gUdsChSupportFlag[i] == FALSE)
						continue;
					//udsResetConnection(i);
					udsResetActiveChStatus(i);
					diagCpyData(UDS_CH_BUFFER(i),dataPtr,len);
					UDS_CH_DATA_LEN(i) = len;
					UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(i) = TRUE;
					UDS_CH_STATUS(i) = UDS_CH_STATUS_NEW_DATA;
					udsSetTimer(i,UDS_TIMER_TYPE_P2_SERVER_MAX);
				}
			}
		}

		if(ch_type == UDS_ADDR_TYPE_PHY)
		{
			//udsResetConnection(ch);
			udsResetActiveChStatus(ch);
			UDS_CH_DATA_LEN(ch) = len;
			UDS_CH_STATUS(ch) = UDS_CH_STATUS_NEW_DATA;
			diagCpyData(UDS_CH_BUFFER(ch),dataPtr,len);
			udsSetTimer(ch,UDS_TIMER_TYPE_P2_SERVER_MAX);
		}

	}
	else
	{
		if(gUdsCfgInfor.enableRespNrcIfChBusy == TRUE)
		{
			if(ch_type == UDS_ADDR_TYPE_FUNC && ret==NRC_BUSY_REPEAT_REQ)
				ch = chBusy;
			UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
			udsPaddingNRC(ch,sid,ret);
			if(Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3) == FALSE)
			{
				UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
				DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_ERRO,"uds:TP busy,NRC %x,ch phy\r\n",ret);				
				return TRUE;
			}
		}
	}
	return TRUE;
}
#endif

uint32 udsRxFirstFrameIndication(uint32 ch,uint16 reqBufferLen,uint8** RxBufferPtr)
{
	tReqBufferResult ret = BUFFER_OK;

	//only phy ch can support multiple frames
	if((ch == 0) || (ch>gUdsCfgInfor.maxChNum) || (RxBufferPtr == NULL) || (gUdsInitFinishedFlag==FALSE))
		return BUFFER_WAIT;

	ch -= 1;

	if(ch<10 && gUdsChSupportFlag[ch] == FALSE)
		return BUFFER_WAIT;


	//if(reqBufferLen > (UDS_CH_BUFF_MAXLEN(ch)-7))//dec 7, to make sure there is no data overflow
	if(reqBufferLen > UDS_CH_BUFF_MAXLEN(ch))
	{
		ret = BUFFER_OVERFLOW;
	}
	else
	{
		//udsResetConnection(ch);
		udsResetActiveChStatus(ch);
		UDS_CH_STATUS(ch) = UDS_CH_STATUS_RXING_MULTIPLE_FRAME;
		*RxBufferPtr = UDS_CH_BUFFER(ch);
	}
	
	return ret;

}

void udsCancelRxData(uint32 ch,uint8 reason)
{

}

void udsTxDataConfirmation(uint32 ch,boolean result,uint8 errId)
{
	uint32 resetType = 0;
	
	if(gUdsInitFinishedFlag == FALSE)
		return;

	if(ch >0 && ch <=gUdsCfgInfor.maxChNum)
	{
		ch -= 1;
		if(result == FALSE)
		{
			gTpTraceByCanFlag = FALSE;
			//DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_ERRO,"uds:TxCfm,FAIL to tx.ch(%d),errorId(%d)\r\n",ch,errId);
		}
		if(UDS_CH_TMP_RESP_FLAG(ch) == TRUE)
		{
			UDS_CH_TMP_RESP_FLAG(ch) = FALSE;

			return;
		}

		if(UDS_CH_STATUS(ch) == UDS_CH_STATUS_WAIT_TXCFM)
		{
			if(result == FALSE)
			{
				udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);
			}
			else
			{
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"uds:ch:%d,TxCfm,sid:0x%x\r\n\r\n",ch,gSid[ch]);
				resetType = gUdsResetSysType;
				udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);

				if(gUdsResetSysType != UDS_NONE_RESET_TYPE)//need to reset sys
				{
					#ifdef UDS_WITH_SID0X11
					sid0x11ResetSys(resetType);
					#endif
				}
			}

		}
	}
}

//if we will reset time to NONE,we should set session to DefaultSession first
void udsResetActiveChStatus(uint32 ch)
{

	UDS_CH_TMP_RESP_FLAG(ch) = FALSE;

	if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
	{
		diagMutexLock(gUdsSidMutex[ch]);
		UDS_CH_STATUS(ch) = UDS_CH_STATUS_IDLE;
		gRespDataLen[ch] = 0;
		gDiagRespCode[ch] = NRC_NONE;
		gUdsRxNewDataTick[ch] = 0;
		diagMutexUnlock(gUdsSidMutex[ch]);
	}
	
	UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(ch) = FALSE;
	gRespNRC0x78Flag[ch] = FALSE;
	gSid[ch] = 0;
	gSuppressPosRespBit[ch] = FALSE;
	//gUdsResetSysType = UDS_NONE_RESET_TYPE;
	
	#ifdef UDS_WITH_SID0X10
	if(sid0x10GetCurSession(ch) != UDS_DEFAULT_SESSION)
		udsSetTimer(ch,UDS_TIMER_TYPE_S3_SERVER);
	else
	#endif
	{
		udsSetTimer(ch,UDS_TIMER_TYPE_NONE);
		udsBroadcastCnnStatus(FALSE);
	}
	

}

boolean udsSetActiveChStatus(uint32 ch,tUdsChStatus status)
{

	UDS_CH_STATUS(ch) = status;
	
	if(status == UDS_CH_STATUS_IDLE)
	{
		udsResetActiveChStatus(ch);
	}

	return TRUE;
}

void udsPaddingNRC(uint32 ch,uint8 sid,tUdsNRCType nrc)
{
	sUDSNRCBuffer[ch][0] = UDS_RESP_NEGATIVE;
	sUDSNRCBuffer[ch][1] = sid;
	sUDSNRCBuffer[ch][2] = nrc;

}

void udsChangeSessionValue(uint32 ch,uint32 sessionValue)
{
#ifdef UDS_WITH_SID0X10
	gUdsSessionType[ch] =  sessionValue;
#endif

}

void udsInit()
{
	uint32 ch = 0;
		
	gDiagCnnIsActiveFlag = FALSE;

	for(ch=0;ch<gUdsCfgInfor.maxChNum;ch++)
	{
		if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
			gUdsSidMutex[ch] = diagCreatMutex();
		udsSetTimer(ch,UDS_TIMER_TYPE_NONE);
		UDS_CH_DATA_LEN(ch) = 0;
		UDS_CH_STATUS(ch) = UDS_CH_STATUS_IDLE;
		UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
		gSid[ch] = 0;
		gRespNRC0x78Flag[ch] = FALSE;
		gSuppressPosRespBit[ch] = FALSE;
		gRespDataLen[ch] = 0;
		gDiagRespCode[ch] = NRC_NONE;
		gUdsRxNewDataTick[ch] = 0;
		udsChangeSessionValue(ch,UDS_DEFAULT_SESSION);
		if(ch<10)
			gUdsChSupportFlag[ch] = TRUE;
	}
	
	

	gUdsResetSysType = UDS_NONE_RESET_TYPE;	
	udsSidInit();

	udsCheckIfNeedToUpdateSystem();
	gUdsInitFinishedFlag = TRUE;

}



void udsProcessTimeout(uint32 ch)
{
	uint32 curSysTick = 0;
	
	if(gUdsSessionTimer[ch].type != UDS_TIMER_TYPE_NONE && gUdsSessionTimer[ch].timeoutFlag != TRUE)
	{
		curSysTick = diagGetCurTick();
		if(curSysTick - gUdsSessionTimer[ch].preTick >= gUdsSessionTimer[ch].interval)
		{
			gUdsSessionTimer[ch].timeoutFlag = TRUE;
			gUdsSessionTimer[ch].preTick = 0;
		}
	}

	if(gUdsSessionTimer[ch].timeoutFlag == TRUE)
	{
		switch(gUdsSessionTimer[ch].type)
		{
		case UDS_TIMER_TYPE_P2_SERVER_MAX:
			//Should here reset connection??
			//udsResetConnection(ch);
			udsSetTimer(ch,UDS_TIMER_TYPE_S3_SERVER);
			break;
		case UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED:
			udsResetConnection(ch);
			break;
		case UDS_TIMER_TYPE_S3_SERVER:
			udsResetConnection(ch);
			if(gUdsCfgInfor.udsWorkMode  == UDS_WORK_MODE_IN_BOOTLOADER)
				diagReqSysReset();

			break;
		default:
			break;//1 error
		}
	}

}

void udsResetConnection(uint32 ch)
{
#ifdef UDS_WITH_SID0X10
	sid0x10SetSession(ch,UDS_DEFAULT_SESSION);
#endif
	udsResetActiveChStatus(ch);
}


//status:TRUE--cnn, FALSE--DisCnn
void udsBroadcastCnnStatus(boolean status)
{
	if(gDiagCnnIsActiveFlag != status)
	{
		gDiagCnnIsActiveFlag = status;
		if(gUdsCfgInfor.enableBroadcastUdsCnnStatus == TRUE)
		{	
			if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP)
				udsSendSignal(gUdsCfgInfor.sigId,(uint32)status);
		}
		
	}
}


void udsLoop()
{
	tUdsNRCType respCode = NRC_NONE;
	static uint8 sidIndex = 0;
	uint8 *dataPtr = NULL;	
	uint8 i = 0;
	uint8 ch = 0;
	boolean validSidFlag = FALSE;
	uint8 sidServiceTotaNum = 0;
	tUdsReqInfor reqInfor;

	static boolean sStableDiagGlobalPrecondition = TRUE;
	boolean sCurDiagGlobalPrecondtion = FALSE;
	
	sCurDiagGlobalPrecondtion = diagCheckGlobalPrecondition();
	
	if( sStableDiagGlobalPrecondition != sCurDiagGlobalPrecondtion)
	{
		sStableDiagGlobalPrecondition = sCurDiagGlobalPrecondtion;
		if(sStableDiagGlobalPrecondition == FALSE)
		{
			for(ch=0;ch<gUdsCfgInfor.maxChNum;ch++)
				udsResetActiveChStatus(ch);
			return;
		}
	}

	if(sStableDiagGlobalPrecondition == FALSE)
		return;

	for(ch=0;ch<gUdsCfgInfor.maxChNum;ch++)
	{
		if(ch<10 && gUdsChSupportFlag[ch] == FALSE)
			continue;
		sid0x27SecurityLockTimerTick(ch);

		udsProcessTimeout(ch);			
		
		//start to process new data
		if(UDS_CH_STATUS(ch) == UDS_CH_STATUS_NEW_DATA)
		{
			//udsConnectionInd();
			dataPtr = UDS_CH_BUFFER(ch);

			gSid[ch] = dataPtr[0];
			DIAG_TRACE_VALUE4(UDS_TRACE,TRACE_INFO,"uds:ch:%d,rx SID:0x%x,0x%x,0x%x\r\n",ch,gSid[ch],dataPtr[1],dataPtr[2]);
			sidServiceTotaNum = udsSidGetSupportedSidCount(ch);
			for(i=0;i<sidServiceTotaNum;i++)
			{
				if( gSid[ch]==gDiagSidService[ch][i].sid)
				{
					if(gDiagSidService[ch][i].customerReqSupportFlag == TRUE 
						|| gSid[ch] == SID_INNER_TEST
						|| gEnableInnerSidFlag == TRUE)
					{
						validSidFlag = TRUE;
						sidIndex = i;
					}
					break;
				}
			}

			if(validSidFlag == TRUE)
			{
				if(UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(ch) == TRUE)
				{
					if((UDS_ADDR_TYPE_FUNC & gDiagSidService[ch][sidIndex].addrFilter) == 0)
					{
						udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);
						continue;
					}
				}

				#ifdef UDS_WITH_SID0X10
				if(sid0x10GetCurSession(ch) & gDiagSidService[ch][sidIndex].sessionTypeFilter)
				{
					if(gDiagSidService[ch][sidIndex].supportSuppressPosRespFlag == TRUE)
					{
						gSuppressPosRespBit[ch] = dataPtr[1] & 0x80 ? TRUE:FALSE;
						dataPtr[1] &= 0x7F;
					}
					else
						gSuppressPosRespBit[ch] = FALSE;
					
					gUdsRxNewDataTick[ch] = diagGetCurTick();
					reqInfor.ch = ch;
					reqInfor.reqFromFuncAddrFlag = UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(ch);
					reqInfor.data = dataPtr;
					reqInfor.len = UDS_CH_DATA_LEN(ch);
					reqInfor.reqTick = gUdsRxNewDataTick[ch];
					udsSetActiveChStatus(ch,UDS_CH_STATUS_PENDING);
					udsBroadcastCnnStatus(TRUE);
					respCode = gDiagSidService[ch][sidIndex].hookFunc(&reqInfor);
				}
				else
				#endif
				{
					respCode = NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION;
				}
			}
			else
			{
				respCode = NRC_SERVICE_NOT_SUPPORTED;
			}

			if(respCode != NRC_RESPONSE_PENDING)
				udsSetActiveChStatus(ch,UDS_CH_STATUS_RESP_FINAL_RESULT);
			else
			{
				if(gUdsCfgInfor.enableRespNrc0x78 == TRUE)
				{
					//0x78 always can be sentout no matter supPosRespBit==(TRUE or FALSE)
					if(FALSE == gRespNRC0x78Flag[ch])
					{
						gRespNRC0x78Flag[ch] = TRUE;
						udsPaddingNRC(ch,gSid[ch],NRC_RESPONSE_PENDING);
						DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"uds:ch:%d,resp SID:0x%x,NRC=0x%x\r\n",ch,gSid[ch],respCode);
						Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3);
					}
				}
				udsSetTimer(ch,UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED);
				continue;
			}
		}

		if(UDS_CH_STATUS(ch) == UDS_CH_STATUS_PENDING)
		{
			//check if final result is ready
			//if ready,set ch status to UDS_CH_STATUS_RESP_FINAL_RESULT
			if(gUdsRxNewDataTick[ch] == 0)
			{
				respCode = gDiagRespCode[ch];
				udsSetActiveChStatus(ch,UDS_CH_STATUS_RESP_FINAL_RESULT);
			}
		}
	
		if(UDS_CH_STATUS(ch) == UDS_CH_STATUS_RESP_FINAL_RESULT)
		{
			if(respCode == NRC_NONE)
			{
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"uds:ch:%d,resp SID:0x%x,NRC=0\r\n",ch,gSid[ch]);
				if(gSuppressPosRespBit[ch] == FALSE)
				{
					UDS_CH_BUFFER(ch)[0] = gSid[ch]+0x40;
					udsSetActiveChStatus(ch,UDS_CH_STATUS_WAIT_TXCFM);
					Tp_TxDataRequest(ch+1, UDS_CH_BUFFER(ch), gRespDataLen[ch]+1);
				}
				else
				{
					if(gSid[ch] == SID_ECU_RESET)
					{
						DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:sysSoftReset()\r\n");
						diagReqSysReset();
					}
					//session and securityAccess will be reset by timer S3server
					udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);
				}
			}
			else if(respCode == NRC_STOP_PROCESS) // uds inner error
			{
				udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);
				DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_WARNING,"uds:ch:%d,sid:0x%x,inner error!!!\r\n",ch,gSid[ch]);
			}
			else//other NRCs
			{
				//14229-1-2013, Pg32
				// not resp NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION
				// and NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION for Func address. 
			
				//functional addressing used, the NRC such as 0x11, 0x12, 0x7E, 0x7F, 0x31 can also be
				//suppressed, Other NRC should be sent to the client
			
				//14229-1-2013,pg36:if respond 0x78 by functional addr, all the final NRCs should be responded.
				//if 0x78 has not sent by functional addr yet, but respCode is one of SNS,SFNS,ROOR,[SFNSIAS],[SNSIAS],
				//then these respCodes will not be sent out.
				if((UDS_CH_REQ_FROM_FUNC_ADDR_FLAG(ch) == TRUE)
					&& (gRespNRC0x78Flag[ch] == FALSE)
					&&(respCode == NRC_SERVICE_NOT_SUPPORTED ||
						respCode == NRC_SUB_FUNCTION_NOT_SUPPORTED ||
						respCode == NRC_REQUEST_OUT_OF_RANGE ||
						((gUdsCfgInfor.enableRespNrcSNSIASandSFNSIASforFuncAddrReq == FALSE)&&(
						respCode == NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION 
						||respCode == NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION)
						) ))
				{
					udsSetActiveChStatus(ch,UDS_CH_STATUS_IDLE);
					continue;
				}

				udsPaddingNRC(ch,gSid[ch],respCode);
				udsSetActiveChStatus(ch,UDS_CH_STATUS_WAIT_TXCFM);
				DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"uds:ch:%d,resp SID:0x%x,NRC=0x%x\r\n",ch,gSid[ch],respCode);
				Tp_TxDataRequest(ch+1, sUDSNRCBuffer[ch], 3);

			}
		}

	}
}

