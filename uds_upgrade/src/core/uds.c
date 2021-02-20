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

static boolean udsSetActiveChStatus(tActiveChStatus status);
static boolean udsSetTimer(tUdsTimerType timerType);

static void udsResetConnection();
static void udsBroadcastCnnStatus(boolean status);
static void udsPaddingNRC(uint8 ch,uint8 sid,tUdsNRCType nrc);
static void udsResetActiveChStatus();
static void udsResendData();
static void udsChangeSessionValue(uint32 sessionValue);

extern const tDiagSidServiceInfor gDiagSidService[];
extern const tUdsCfgInfor gUdsCfgInfor;
extern uint8 gUdsRxTxBuffer[];
#ifdef UDS_WITH_SID0X10
extern uint32 gUdsSessionType;
#endif
uint16 gRespDataLen = 0;
boolean gForwardReqToDiagAppFlag = FALSE;

#define UDS_CH_TMP_RESP_FLAG(ch) gUdsChInfor[ch].tmp_resp_flag
#define UDS_CH_BUFFER(ch)	gUdsChInfor[ch].buffer
#define UDS_CH_STATUS(ch)		gUdsChInfor[ch].chStatus
#define UDS_CH_DATA_LEN(ch)		gUdsChInfor[ch].dataLen

#define UDS_CH_BUFF_MAXLEN(ch)	gUdsChInfor[ch].bufferMaxLen
#define UDS_CH_ID(ch)	gUdsChInfor[ch].chId
#define UDS_CH_PRIO(ch)		gUdsChInfor[ch].chPrio

#define UDS_RESP_NEGATIVE 0x7F
#define UDS_CH_MAX 2

static uint8 sUDSNRCBuffer[UDS_CH_MAX][3];
static tUdsResetSysType gUdsResetSysType = UDS_NONE_RESET_TYPE;
static boolean gDiagCnnIsActiveFlag = FALSE;
static uint8 gResendCounter = 0;
static tUdsTimer gUdsSessionTimer;
static uint8 gSid;
static boolean gSuppressPosRespBit = FALSE;
static tActiveChInfor gActiveChInfor;
static boolean gRespNRC0x78Flag = FALSE;
static boolean gUdsInitFinishedFlag = FALSE;

static tUdsChInfor gUdsChInfor[UDS_CH_MAX] = {
		{TP_CHANNEL_UDS_PHY,CH_STATUS_IDLE,FALSE,2,0,0,(uint8*)gUdsRxTxBuffer},
		{TP_CHANNEL_UDS_FUNC,CH_STATUS_IDLE,FALSE,1,0,0,(uint8*)gUdsRxTxBuffer}
};


const tUdsCfgInfor *udsGetGlobalCfgInfor()
{
	return &gUdsCfgInfor;
}


void udsSetResetSysType(tUdsResetSysType resetTriggerType)
{
	gUdsResetSysType = resetTriggerType;
}

uint32 udsGetResetSysType()
{
	return gUdsResetSysType;
}

boolean udsSetTimer(tUdsTimerType timerType)
{
	switch(timerType)
	{
	case UDS_TIMER_TYPE_NONE:
		gUdsSessionTimer.interval= 0;
		break;
	case UDS_TIMER_TYPE_P2_SERVER_MAX:
		gUdsSessionTimer.interval = gUdsCfgInfor.p2ServerMaxTimerValue;
		break;
	case UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED:
		gUdsSessionTimer.interval = gUdsCfgInfor.p2ServerMaxEnhancedTimerValue;
		break;
	case UDS_TIMER_TYPE_S3_SERVER:
		gUdsSessionTimer.interval = gUdsCfgInfor.s3ServerTimerValue;
		break;
	default:
		return FALSE;//error
	}

	if(gUdsCfgInfor.udsLogLevel >= UDS_LOG_LEVEL_3)
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"UDS:setTimer:%d\r\n",timerType);
	gUdsSessionTimer.type = timerType;
	gUdsSessionTimer.timeoutFlag = FALSE;
	gUdsSessionTimer.preTick = diagGetCurTick();

	return TRUE;
}

boolean udsRxIndication(uint32 ch,uint8* dataPtr, uint16 len)
{
	uint8 sid = 0;
	boolean supressPosResp = FALSE;
	static uint8 tmpNrcBuffer[3] = {0};
	
	if(dataPtr == NULL || (ch!=TP_CHANNEL_UDS_FUNC&&ch!=TP_CHANNEL_UDS_PHY)||len ==0 || len >UDS_CH_BUFF_MAXLEN(ch) || gUdsInitFinishedFlag==FALSE)
		return FALSE;
	
	if(diagCheckGlobalPrecondition() == FALSE)
		return FALSE;
	sid = dataPtr[0];
	supressPosResp = dataPtr[1]&0x80 ? TRUE : FALSE;
	
	if(UDS_CH_STATUS(ch)!= CH_STATUS_IDLE)//same channel is busy,we will ingore new data
	{
		if(gUdsCfgInfor.enableRespNrcIfChBusy == TRUE)
		{
			if(ch == TP_CHANNEL_UDS_PHY)
			{
				UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
				tmpNrcBuffer[0] = UDS_RESP_NEGATIVE;
				tmpNrcBuffer[1] = dataPtr[0];
				tmpNrcBuffer[2] = NRC_BUSY_REPEAT_REQ;
				Tp_TxDataRequest(ch, tmpNrcBuffer, 3);
			}
		}
		return FALSE;
	}
	else if(sid== 0x3E&& supressPosResp== TRUE)
	{
		//if(ch ==TP_CHANNEL_UDS_FUNC)
		{
			if(len!=2 || (dataPtr[1] != 0x80))
			{		
				tmpNrcBuffer[0] = UDS_RESP_NEGATIVE;
				tmpNrcBuffer[1] = 0x3e;
				if(len!=2)
					tmpNrcBuffer[2] = NRC_INCORRECT_MESSAGE_LENGTH;
				else
					tmpNrcBuffer[2] = NRC_SUB_FUNCTION_NOT_SUPPORTED;
				// if rx 0x3e 82 with func addr,no NRC will be sent out
				if(ch ==TP_CHANNEL_UDS_FUNC && tmpNrcBuffer[2] == NRC_SUB_FUNCTION_NOT_SUPPORTED) 
          			return TRUE;
				UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
				if(Tp_TxDataRequest(ch, tmpNrcBuffer, 3) == FALSE)
				{
					UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
					//maybe we should create a tx buffer list in TP, if tp is busy
					DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"uds:TP busy,NRC %x ,ch FUNC\r\n",NRC_BUSY_REPEAT_REQ);
					return TRUE;
				}

			}
			#ifdef UDS_WITH_SID0X10
			else if((sid0x10GetCurSession() != UDS_DEFAULT_SESSION) && (gUdsSessionTimer.type==UDS_TIMER_TYPE_S3_SERVER)/* && gActiveChInfor.chId != ACTIVE_CH_NONE*/)
			{
				udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
				udsBroadcastCnnStatus(TRUE);
			}
			#endif
		}
		return TRUE;
	}

		
	if(len<8)//SF
	{
		if(gActiveChInfor.chId != TP_CHANNEL_MAX) // busy now
		{
			if(gActiveChInfor.priority >= UDS_CH_PRIO(ch))
			{
				if(UDS_CH_ID(ch) == TP_CHANNEL_UDS_PHY && (dataPtr[1]&&0x80 !=0x80))
				{
					UDS_CH_TMP_RESP_FLAG(ch) = TRUE;
					tmpNrcBuffer[0] = UDS_RESP_NEGATIVE;
					tmpNrcBuffer[1] = dataPtr[0];
					tmpNrcBuffer[2] = NRC_BUSY_REPEAT_REQ;
					if(Tp_TxDataRequest(ch, tmpNrcBuffer, 3) == FALSE)
					{
						//maybe we should create a tx buffer list in TP, if tp is busy
						DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"uds:TP busy,NRC %x,ch phy\r\n",NRC_BUSY_REPEAT_REQ);
						return TRUE;
					}
				}
				else// ignore new req.
				{
					return TRUE;
				}
			}
			else //reset previous connection,Accept new data
			{
				udsResetConnection();
				diagCpyData(UDS_CH_BUFFER(ch),dataPtr,len);
			}
		}
		else
			diagCpyData(UDS_CH_BUFFER(ch),dataPtr,len);
	}
	
	UDS_CH_STATUS(ch) = CH_STATUS_BUSY;
	UDS_CH_DATA_LEN(ch) = len;
	
	gActiveChInfor.chId = ch;
	
	if(ch == TP_CHANNEL_UDS_FUNC)
		gActiveChInfor.addrType = UDS_ADDR_TYPE_FUNC;
	else
		gActiveChInfor.addrType = UDS_ADDR_TYPE_PHY;

	gActiveChInfor.priority = UDS_CH_PRIO(ch);
	gActiveChInfor.chStatus = ACTIVE_CH_STATUS_NEW_DATA;
	gActiveChInfor.buffer = UDS_CH_BUFFER(ch);
	gActiveChInfor.dataLen = len;
	udsSetTimer(UDS_TIMER_TYPE_P2_SERVER_MAX);
	udsBroadcastCnnStatus(TRUE);
	
	return TRUE;
}

uint32 udsRxFirstFrameIndication(uint32 ch,uint16 reqBufferLen,uint8** RxBufferPtr)
{
	tReqBufferResult ret = BUFFER_OK;

	if((ch >= UDS_CH_MAX) || (RxBufferPtr == NULL) || (gUdsInitFinishedFlag==FALSE))
		return BUFFER_WAIT;


	//if(reqBufferLen > (UDS_CH_BUFF_MAXLEN(ch)-7))//dec 7, to make sure there is no data overflow
	if(reqBufferLen > UDS_CH_BUFF_MAXLEN(ch))
	{
		ret = BUFFER_OVERFLOW;
	}
	else
	{
		
		if(gActiveChInfor.chId != TP_CHANNEL_MAX)
		{
			if(gActiveChInfor.priority >= UDS_CH_PRIO(ch))
			{
				ret = BUFFER_OVERFLOW;//BUFFER_WAIT;
			}
			else //reset previous connection,Accept new data
			{
				udsBroadcastCnnStatus(FALSE);
				udsResetConnection();
				UDS_CH_STATUS(ch) = CH_STATUS_IDLE;
				*RxBufferPtr = UDS_CH_BUFFER(ch);
				ret = BUFFER_OK;
			}
		}
		else
		{
			udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
			*RxBufferPtr = UDS_CH_BUFFER(ch);
			gActiveChInfor.dataLen = 0;
			UDS_CH_STATUS(ch) = CH_STATUS_IDLE;
			ret = BUFFER_OK;
		}
		
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

	if(ch>=UDS_CH_MAX)
	{
		//DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_ERRO,"uds:TxCfm,ERROR,ch(%d) value exceed\r\n",ch);
		return;
	}

	if(result == FALSE)
		DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"uds:TxCfm,FAIL to tx.ch(%d),errorId(%d)\r\n",ch,errId);

	if(UDS_CH_TMP_RESP_FLAG(ch) == TRUE)
	{
		UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
		
		return;
	}
	
	if(ch == gActiveChInfor.chId)
	{
		if(result == FALSE)
		{
			udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
		}
		else if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_WAIT_FOR_TXCFM)
		{
			if(gUdsResetSysType != UDS_NONE_RESET_TYPE)
			{
				//华晨M85需求:SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf, Pg31
				resetType = gUdsResetSysType;
				udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);//gUdsResetSysType will be reset to UDS_NONE_RESET_TYPE;
				#ifdef UDS_WITH_SID0X11
				sid0x11ResetSys(resetType);
				#endif
			}
			else if(UDS_CH_STATUS(ch) == CH_STATUS_WAIT_NRC_0X78_TXCFM)
			{
				udsSetTimer(UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED);
				udsSetActiveChStatus(ACTIVE_CH_STATUS_BUSY);
				UDS_CH_STATUS(ch) = CH_STATUS_BUSY;
			}
			else
			{
				DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:TxCfm DONE!\r\n\r\n");
				udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
			}
		}
		else//this should not happen
		{
			DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"uds:TxCfm,ERROR,ch=%d,errId=%d\r\n",ch,errId);
		}
			

	}
	else
	{
		UDS_CH_STATUS(ch) = CH_STATUS_IDLE;
	}

}

//if we will reset time to NONE,we should set session to DefaultSession first
void udsResetActiveChStatus()
{
	gForwardReqToDiagAppFlag = FALSE;
	gRespDataLen = 0;
	gUdsResetSysType = UDS_NONE_RESET_TYPE;
	gRespNRC0x78Flag = FALSE;
	gSuppressPosRespBit = FALSE;
	if(gActiveChInfor.chId != TP_CHANNEL_MAX)
	{
		UDS_CH_STATUS(gActiveChInfor.chId) = CH_STATUS_IDLE;
		UDS_CH_TMP_RESP_FLAG(gActiveChInfor.chId) = FALSE;
	}
	gResendCounter = 0;

	gActiveChInfor.chId= TP_CHANNEL_MAX;
	gActiveChInfor.addrType = UDS_ADDR_TYPE_NONE;
	gActiveChInfor.priority = 0;
	gActiveChInfor.chStatus = ACTIVE_CH_STATUS_IDLE;
	gActiveChInfor.dataLen = 0;

	#ifdef UDS_WITH_SID0X10
	if(sid0x10GetCurSession() != UDS_DEFAULT_SESSION)
		udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
	else
	#endif
		udsSetTimer(UDS_TIMER_TYPE_NONE);
	
}

boolean udsSetActiveChStatus(tActiveChStatus status)
{

	if(status == ACTIVE_CH_STATUS_IDLE)
	{
		udsResetActiveChStatus();
	}
	else
	{
		gActiveChInfor.chStatus = status;
		if(status == ACTIVE_CH_STATUS_WAIT_FOR_TXCFM)
		{
			#ifdef UDS_WITH_SID0X10
			if(sid0x10GetCurSession() != UDS_DEFAULT_SESSION)
			#endif
				udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
		}
	}

	return TRUE;
}

void udsPaddingNRC(uint8 ch,uint8 sid,tUdsNRCType nrc)
{
	sUDSNRCBuffer[ch][0] = UDS_RESP_NEGATIVE;
	sUDSNRCBuffer[ch][1] = sid;
	sUDSNRCBuffer[ch][2] = nrc;

}

void udsChangeSessionValue(uint32 sessionValue)
{
#ifdef UDS_WITH_SID0X10
	gUdsSessionType =  sessionValue;
#endif

}

void udsInit()
{
	uint8 ch = 0;
		
	gDiagCnnIsActiveFlag = FALSE;

	udsSetTimer(UDS_TIMER_TYPE_NONE);
	
	gSid = 0;
	gForwardReqToDiagAppFlag = FALSE;

	gActiveChInfor.chId = TP_CHANNEL_MAX;
	gActiveChInfor.chStatus = ACTIVE_CH_STATUS_IDLE;
	gActiveChInfor.priority = 0;
	gActiveChInfor.buffer = NULL;
	gActiveChInfor.dataLen = 0;

	gRespDataLen = 0;
	gResendCounter = 0;
	gRespNRC0x78Flag = FALSE;
	gSuppressPosRespBit = FALSE;
	
	for(ch=0;ch<UDS_CH_MAX;ch++)
	{
		UDS_CH_DATA_LEN(ch) = 0;
		UDS_CH_STATUS(ch) = CH_STATUS_IDLE;
		UDS_CH_TMP_RESP_FLAG(ch) = FALSE;
		gUdsChInfor[ch].bufferMaxLen = gUdsCfgInfor.udsRxTxBufferLen;
	}

	gUdsResetSysType = UDS_NONE_RESET_TYPE;
	//gUdsSessionType =  UDS_DEFAULT_SESSION;
	udsChangeSessionValue(UDS_DEFAULT_SESSION);
	udsSidInit();

	udsCheckIfNeedToUpdateSystem();
	gUdsInitFinishedFlag = TRUE;

}



void udsProcessTimeout()
{
	
	if(gUdsSessionTimer.timeoutFlag == TRUE)
	{
		switch(gUdsSessionTimer.type)
		{
		case UDS_TIMER_TYPE_P2_SERVER_MAX:
			//if uds can't rx txCfm,but currentSession is defaultSession
			udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);			
			//gRespNRC0x78Flag = TRUE;//disable reTx 0x78
			break;
		case UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED:
		#if 0
			udsResetConnection();
		#else
			udsSetTimer(UDS_TIMER_TYPE_P2_SERVER_MAX_ENHANCED);
			gRespNRC0x78Flag = FALSE;//allow to reTx 0x78
		#endif
			break;
		case UDS_TIMER_TYPE_S3_SERVER:
			if(gUdsCfgInfor.udsWorkMode  == UDS_WORK_MODE_IN_BOOTLOADER)
			{
				//华晨M85需求:SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf, Pg31
				//SRS_flashing_specification_M85_V1.12_20161220.pdf Pg10
				udsResetConnection();
				diagReqSysReset();
			}
			else
			{
				udsResetConnection();
			}
			break;
		default:
			break;//1 error
		}
	}

}

void udsResetConnection()
{
#ifdef UDS_WITH_SID0X10
	sid0x10SetSession(UDS_DEFAULT_SESSION);
#endif
	udsResetActiveChStatus();
}


//status:TRUE--cnn, FALSE--DisCnn
void udsBroadcastCnnStatus(boolean status)
{
	if(gDiagCnnIsActiveFlag != status)
	{
		gDiagCnnIsActiveFlag = status;
		if(gUdsCfgInfor.enableBroadcastUdsCnnStatus == TRUE)
		{	
			if(gUdsCfgInfor.udsWorkMode == UDS_WORK_MODE_IN_APP && gUdsCfgInfor.sigId >0)
				udsSendSignal(gUdsCfgInfor.sigId,(uint32)status);
		}
		
	}
}


void udsResendData()
{
	uint8 *dataPtr = NULL;
	uint16 len = 0;
	
	//if timer P2server is time out before gResendCounter==0, operation for RESEND will be stopped
	gResendCounter--;
	if(gResendCounter >0)
	{
		if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_RESEND_POSITIVE_DATA)
		{
			dataPtr = gActiveChInfor.buffer;
			len = gRespDataLen+1;
		}
		else if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_RESEND_NRC_DATA)
		{
			dataPtr = sUDSNRCBuffer[gActiveChInfor.chId];
			len = 3;
		}
		
		if(Tp_TxDataRequest(gActiveChInfor.chId, dataPtr, len) == FALSE)
		{
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:TP busy\r\n");
		}
		else
		{
			DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:resend data, done!\r\n");
			udsSetActiveChStatus(ACTIVE_CH_STATUS_WAIT_FOR_TXCFM);
		}
	}
	else
	{
		DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:TP busy,reSendcounter exceed!!!\r\n");
		udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
	}

}


void udsLoop()
{
	uint8 activeChId = 0;
	tUdsNRCType respCode = NRC_NONE;
	uint32 curSysTick = 0;
	static uint8 sidIndex = 0;
	uint8 *dataPtr = NULL;	
	uint8 i = 0;
	boolean validSidFlag = FALSE;
	uint8 sidServiceTotaNum = 0;
	static boolean sStableDiagGlobalPrecondition = TRUE;
	boolean sCurDiagGlobalPrecondition = FALSE;
	
	sCurDiagGlobalPrecondition = diagCheckGlobalPrecondition();
	
	if( sStableDiagGlobalPrecondition != sCurDiagGlobalPrecondition)
	{
		sStableDiagGlobalPrecondition = sCurDiagGlobalPrecondition;
		if(sStableDiagGlobalPrecondition == FALSE)
		{
			udsResetActiveChStatus();
			return;
		}
	}

	if(sStableDiagGlobalPrecondition == FALSE)
		return;
	#ifdef UDS_WITH_SID0X27	
	sid0x27SecurityLockTimerTick();
	#endif
	
	if(gUdsSessionTimer.type != UDS_TIMER_TYPE_NONE && gUdsSessionTimer.timeoutFlag != TRUE)
	{
		curSysTick = diagGetCurTick();
		if(curSysTick - gUdsSessionTimer.preTick >= gUdsSessionTimer.interval)
		{
			gUdsSessionTimer.timeoutFlag = TRUE;
			gUdsSessionTimer.preTick = 0;
		}
	}
	else
	{
		udsBroadcastCnnStatus(FALSE);
	}
		

	udsProcessTimeout();
	activeChId = gActiveChInfor.chId;
	if(activeChId == TP_CHANNEL_MAX)
		return;
	//start to process new data
	if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_NEW_DATA)
	{
		//udsConnectionInd();
		dataPtr = gActiveChInfor.buffer;

		gSid = dataPtr[0];
		DIAG_TRACE_VALUE3(UDS_TRACE,TRACE_INFO,"uds:rx SID:0x%x,0x%x,0x%x\r\n",gSid,dataPtr[1],dataPtr[2]);
		sidServiceTotaNum = udsSidGetSupportedSidCount();
		for(i=0;i<sidServiceTotaNum;i++)
		{
			if( gSid==gDiagSidService[i].sid)
			{
				validSidFlag = TRUE;
				sidIndex = i;
				break;
			}
		}

		if(validSidFlag == TRUE)
		{
			if((gActiveChInfor.addrType & gDiagSidService[sidIndex].addrFilter) == 0)
			{
				udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
				return;
			}

			gSuppressPosRespBit = FALSE;

			#ifdef UDS_WITH_SID0X10
			if(sid0x10GetCurSession() & gDiagSidService[sidIndex].sessionTypeFilter)
			{
				if(gDiagSidService[sidIndex].checkSuppressPosRespFlag == TRUE)
				{
					//dataPtr[1] &= 0x7F;
					gSuppressPosRespBit = dataPtr[1] & 0x80 ? TRUE:FALSE;
				}

				udsSetActiveChStatus(ACTIVE_CH_STATUS_BUSY);
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

		if(respCode != NRC_NONE)
		{
			if (gUdsCfgInfor.enableRespNrcSNSIASandSFNSIASforFuncAddrReq == FALSE)
			{
				//standard 14229 protocl is request as following:
				if(activeChId == TP_CHANNEL_UDS_PHY)
					udsSetActiveChStatus(ACTIVE_CH_STATUS_RESP_NRC);
				else// not resp for Func address. 14229-1-2013, Pg32
				{
					udsResetActiveChStatus();
				}
			}
			else
			{
				// customer requirement: if rx Functional addr req, NRC: SNSIAS and SFNSIAS should be resp
				if(activeChId == TP_CHANNEL_UDS_PHY || (activeChId == TP_CHANNEL_UDS_FUNC && respCode == NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION))
					udsSetActiveChStatus(ACTIVE_CH_STATUS_RESP_NRC);
				else
				{
					udsResetActiveChStatus();
				}

			}
		}
	}


	if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_BUSY)
	{
		respCode = gDiagSidService[sidIndex].hookFunc(activeChId,gActiveChInfor.buffer,gActiveChInfor.dataLen);

		if(respCode == NRC_NONE)
		{
			DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"uds:resp SID:0x%x,NRC=0\r\n",gSid);
			if(gSuppressPosRespBit == FALSE)
			{
				gActiveChInfor.buffer[0] = gSid+0x40;
				//P2_SERVER is active now
				udsSetActiveChStatus(ACTIVE_CH_STATUS_WAIT_FOR_TXCFM);

				if(Tp_TxDataRequest(activeChId, gActiveChInfor.buffer, gRespDataLen+1) == FALSE)
				{
					DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:TP busy,resend posResp later\r\n");
					udsSetActiveChStatus(ACTIVE_CH_STATUS_RESEND_POSITIVE_DATA);
					gResendCounter = gUdsCfgInfor.udsResendCounter;
				}
				else
				{
					//if(sid0x10GetCurSession() != UDS_DEFAULT_SESSION)
						//udsSetTimer(UDS_TIMER_TYPE_S3_SERVER);
				}

			}
			else
			{
				if(gSid == SID_ECU_RESET)
				{
					DIAG_TRACE_TXT(UDS_TRACE,TRACE_INFO,"uds:sysSoftReset()\r\n");
					
					diagReqSysReset();
				}
				//session and securityAccess will be reset by timer S3server
				udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
			}
		}
		else if(respCode == NRC_STOP_PROCESS) // uds inner error
		{
			udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
		}
		else//other NRCs
		{
			if(respCode == NRC_RESPONSE_PENDING)
			{
				if(gUdsCfgInfor.enableRespNrc0x78 == TRUE)
				{
					//0x78 always can be sentout no matter supPosRespBit==(TRUE or FALSE)

					if(FALSE == gRespNRC0x78Flag)
					{
						gRespNRC0x78Flag = TRUE;

						UDS_CH_STATUS(activeChId) = CH_STATUS_WAIT_NRC_0X78_TXCFM;
						udsSetActiveChStatus(ACTIVE_CH_STATUS_RESP_NRC);
						//before here, P2_SERVER_MAX is active yet,after here, P2_SERVER is active
						//if we can't get txCfm,we will reset all after P2server is time out.
						udsSetTimer(UDS_TIMER_TYPE_P2_SERVER_MAX);
					}
				}
			}
			else
			{
				//华晨M8:SRS_basic_diagnostic_System_specification_M85_V1.13_20170117.pdf, Pg33
				//.What recommend is that when
				//functional addressing used, the NRC such as 0x11, 0x12, 0x7E, 0x7F, 0x31 can also be
				//suppressed, Other NRC should be sent to the client
			
				//14229-1-2013,pg36:if respond 0x78 by functional addr, all the final NRCs should be responded.
				//if 0x78 has not sent by functional addr yet, but respCode is one of SNS,SFNS,ROOR,[SFNSIAS],[SNSIAS],
				//then these respCodes will not be sent out.
				if((activeChId == TP_CHANNEL_UDS_FUNC)
					&& (gRespNRC0x78Flag == FALSE)
					&&(respCode == NRC_SERVICE_NOT_SUPPORTED ||
						respCode == NRC_SUB_FUNCTION_NOT_SUPPORTED ||
						respCode == NRC_REQUEST_OUT_OF_RANGE ||
						((gUdsCfgInfor.enableRespNrcSNSIASandSFNSIASforFuncAddrReq == FALSE)&&(
						respCode == NRC_SUB_FUNCTION_NOT_SUPPORTED_IN_ACTIVE_SESSION 
						||respCode == NRC_SERVICE_NOT_SUPPORED_IN_ACTIVE_SESSION)
						) ))
				{
					udsSetActiveChStatus(ACTIVE_CH_STATUS_IDLE);
				}
				else
				{
					//if other conditions,all the NRC will be sent out no matter suppressPosRespBit is true or false
					udsSetActiveChStatus(ACTIVE_CH_STATUS_RESP_NRC);
				}
			}

		}
	}

	if(gActiveChInfor.chStatus == ACTIVE_CH_STATUS_RESP_NRC)
	{
		//at this moment,P2server is active
		udsPaddingNRC(activeChId,gSid,respCode);
		udsSetActiveChStatus(ACTIVE_CH_STATUS_WAIT_FOR_TXCFM);
		DIAG_TRACE_VALUE2(UDS_TRACE,TRACE_INFO,"uds:resp SID:0x%x,NRC=0x%x\r\n",gSid,respCode);
		if(Tp_TxDataRequest(activeChId, sUDSNRCBuffer[activeChId], 3) == FALSE)
		{
			DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"uds:TP busy,resend NRC %x later\r\n",respCode);
			udsSetActiveChStatus(ACTIVE_CH_STATUS_RESEND_NRC_DATA);
			gResendCounter = gUdsCfgInfor.udsResendCounter;
		}
	}

	//if timer P2server is time out before gResendCounter==0, operation for RESEND will be stopped
	if((gActiveChInfor.chStatus == ACTIVE_CH_STATUS_RESEND_NRC_DATA) 
		|| (gActiveChInfor.chStatus == ACTIVE_CH_STATUS_RESEND_POSITIVE_DATA))
	{	
		DIAG_TRACE_VALUE1(UDS_TRACE,TRACE_INFO,"uds:resend for SID:0x%x\r\n",gSid);
		udsResendData();
	}

}

