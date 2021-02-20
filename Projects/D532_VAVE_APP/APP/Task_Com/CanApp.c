/**
* @file:    CanApp.c
* @brief:   Add your description here for this file.
* @author:  zhangyi
* @date:    2018-09-11 10:40:16
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  zhangyi  2018-09-11 10:40:16  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include <string.h>


#include"CanApp.h"
#include "Com_If.h"
#include "SimFlag.h"
#include "Com_Cfg.h"
#include "Spi_Ipc_gw_api.h"
#include "NvM_if.h"
#include "NvM_Cfg.h"
#include "MemMap.h"

#include "CircularQueue.h"
#include "Task_M_IpcApp_Cfg.h" 
#include "trace.h"

/* Gps time need  */
#include "ExternRTC.h"

/*----------------------------- START: USER INCLUDED ----------------------------*/

/*------------------------------ END: USER INCLUDE ------------------------------*/
#define debugger 0

#define DA_RX_MSG_NUM 42
#define CanApp_TIMEOUT_ID 0xFFFF


/**
*@par Service ID[hex]
*0x00
*@par Sync/Async
*Synchronous
*@par Reentrancy
*Non Reentrant
*@param[in] None
*@param[inout] None
*@param[out] None
*@retval None
*
*@brief Service for resetting all internal variables.
*/
/* @req REQ001*/


static uint8 CanApp_Status_Req = FALSE;
uint16 CanAppChannel = CQUEUE_ERR_CHANNEL;
uint16 CanAppCountChannel = CQUEUE_ERR_CHANNEL;

uint16 CanApp_Recv_Id[HANDLE_RECVMSG_TEMPSIZE];
volatile uint8 CanApp_RecvHandle[HANDLE_RECVMSG_TEMPSIZE][4];


extern const PduIdType Com_SigId2IpduId[];
extern uint8 * const Com_IpduBufPtr[];
extern uint8 CanApp_Tx_Count[COM_TX_IPDU_MAX_NUM];
static uint8 CanApp_Main_Count = 0;
extern uint8 CanApp_Tx_NeedCount[COM_TX_IPDU_MAX_NUM]; 
extern uint8 Com_IpduBuf_0X358[8];

boolean CanApp_txPendingFlag[COM_TX_SIGNAL_MAX_NUM] = {0};

boolean g_CanMsgRxTimeoutFlag = FALSE;
uint8 g_CanAppPduTimeOutFlag[8] ={0,0,0,0,0,0,0,0};

const uint8 Da_Rx_Msg_PduID[DA_RX_MSG_NUM] = 
{
	IPDU_HND_LWSOUT_M2,                  IPDU_HND_ECM_TorqueControl_RN1,    IPDU_HND_ECM_TorqueControl_N1,    IPDU_HND_ECM_SystemControl_1f9,   IPDU_HND_USM_Fast_Frame,
	IPDU_HND_Brake_StabilityControl_RN1, IPDU_HND_Front_Wheel_Speed_Frame,  IPDU_HND_ITS_InfoStatus_N1,       IPDU_HND_ITS_InfoStatus_N2,       IPDU_HND_SRR_Left,
	IPDU_HND_SRR_Right,                  IPDU_HND_SMART_bcmMSG_PUSH,        IPDU_HND_IKEY_352,                IPDU_HND_Brake_GeneralData,       IPDU_HND_BCMMSG1,    
	IPDU_HND_BCM_SystemControl_2,        IPDU_HND_BCM_SystemControl,        IPDU_HND_TPMS_GeneralStatus_BCM,  IPDU_HND_ATCVT_to_MMI_RN1,        IPDU_HND_ITS_InfoStatus_N3_7b,   
	IPDU_HND_ITS_InfoStatus_N2_1,        IPDU_HND_ECM_GeneralStatus,        IPDU_HND_DCM_GeneralStatus_5,     IPDU_HND_PBD_Status,              IPDU_HND_SONAR_GeneralStatus_8,  
	IPDU_HND_ECM_to_ITM,                 IPDU_HND_EPB_SystemControl,        IPDU_HND_DVR_Request,             IPDU_HND_AMP_5b9,                 IPDU_HND_AMP_Version_5c2,         
	IPDU_HND_S_BOX_ControlData_SetPM,    IPDU_HND_S_BOX_ControlData_Seat,   IPDU_HND_S_BOX_ControlData_enjoy, IPDU_HND_S_BOX_ControlData_AC,    IPDU_HND_EPS_GeneralStatus,   
	IPDU_HND_LCM_5E6,                    IPDU_HND_LCM_5E7,                  IPDU_HND_APA_HMI_system,          IPDU_HND_DVR_STS,                 IPDU_HND_BCM_GeneralStatus,  
	IPDU_HND_USM_GeneralStatus,          IPDU_HND_BSW_GeneralStatus_3,
};

ExternSTDType gps_time_struct;

void CanApp_Init(void)
{	
	uint16 l_loop;
	uint8 InitValue = CanAppTxInitValue;
	for (l_loop = 0; l_loop < COM_TX_SIGNAL_MAX_NUM; l_loop++) {
			if (CanApp_Tx_Matrix[l_loop] != NULL) {
				CanApp_Tx_Matrix[l_loop](1, &InitValue);
			}
	}
	ExtrRtc_StdTimeGet(&gps_time_struct);
	CanAppChannel = CQueue_Init(CQUEUE_BUFTYPE_UINT16, CanApp_Recv_Id, HANDLE_RECVMSG_TEMPSIZE);

}
static void CanAppReportPduStatus()
{
	error_type l_errCode = IPC_GW_ERR_OK;
	uint8 counter = 0;
	static uint8 s_Index = 0;
	uint8 reportIndex = 0;
	if (CanApp_Status_Req == TRUE)
	{
		while((counter < 21) && (s_Index < DA_RX_MSG_NUM))
		{
			reportIndex = Da_Rx_Msg_PduID[s_Index];
			l_errCode = ipc_send_notification(1,IPC_GW_CHANNEL_CANAPP, reportIndex, Com_IpduBufPtr[reportIndex],8);
		
			TracePrintf(CANAPP_TRACE, TRACE_INFO, "tx PduId[%d] to SOC,ret=%d\r\n", reportIndex,l_errCode);
			counter++;
			s_Index++;	
		}
		
		if(DA_RX_MSG_NUM == s_Index)
		{
			s_Index = 0;
			CanApp_Status_Req = FALSE;
		}
	}


}

static void CanAppReportPduTimeOutStatus(void)
{
	static uint16 timeout_counter = 0;
	if (timeout_counter++ % 100 == 0)
	{
		if(TRUE == g_CanMsgRxTimeoutFlag)
		{
			ipc_send_notification(1,IPC_GW_CHANNEL_CANAPP, CanApp_TIMEOUT_ID, &g_CanAppPduTimeOutFlag[0],8);
		    g_CanMsgRxTimeoutFlag = FALSE;
		}
	}
}

void CanApp_Main()
{
	uint16 l_txCanAppIdLoop = CQUEUE_ERR_PUSHPOP;
	uint16 l_CountTimeLoop = CQUEUE_ERR_PUSHPOP;
	uint16 l_txIndex = 0xFFFF;
	boolean vaildvalue = CanAppTxTirggerValue;
		
	CanAppReportPduStatus();
	CanApp_TxHand();
	CanAppReportPduTimeOutStatus();

	#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
    l_txCanAppIdLoop = CQueue_Pop(CanAppChannel, &l_txIndex, 1, STD_OFF);
	#else
    l_txCanAppIdLoop = CQueue_Pop(CanAppChannel, &l_txIndex, 1);
	#endif
    while (l_txCanAppIdLoop != CQUEUE_ERR_PUSHPOP) {
		if (CanApp_Tx_Matrix[l_txIndex] != NULL) {
			CanApp_Tx_Matrix[l_txIndex](l_txCanAppIdLoop, &vaildvalue);
		}
		#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
    	l_txCanAppIdLoop = CQueue_Pop(CanAppChannel, &l_txIndex, 1, STD_OFF);
		#else
    	l_txCanAppIdLoop = CQueue_Pop(CanAppChannel, &l_txIndex, 1);
		#endif
    }
}	



void Can_MsgChangeProcess(uint16 l_ipduId,boolean enableTrace)
{
	error_type l_errCode = IPC_GW_ERR_OK;
	uint8 l_msgLen = 0xFF;
	uint8 l_tempBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint32 rcvedTime = 0xFFFFFFFF;
	l_msgLen = Com_CopyIpduRawData(l_ipduId, &l_tempBuf[0], &rcvedTime);
	if (l_msgLen <= 8 && l_msgLen != 0) {
		l_errCode = ipc_send_notification(1,IPC_GW_CHANNEL_CANAPP, l_ipduId, &l_tempBuf[0],l_msgLen);
		if(enableTrace == TRUE)
			TracePrintf(CANAPP_TRACE, TRACE_INFO, "tx PduId[%d] {0x%x,0x%x} to Soc,ret=%d \r\n", l_ipduId,*((uint32*)&l_tempBuf[0]),*((uint32*)&l_tempBuf[4]),l_errCode);
	}
}

void CanApp_IpcRxIndication(const void *ctx, uint8 *data, uint32 len)
{
	uint16 l_queueIndex = CQUEUE_ERR_PUSHPOP;
	uint16 MsgId = CQUEUE_ERR_PUSHPOP;
	uint16 realMsgId = CQUEUE_ERR_PUSHPOP;
	uint8 window_data[4] = {0xf,0xf,0xf,0xf} ;
	
	//request_context_t *context = (request_context_t*)ctx;
	if ((NULL == (data && ctx)) || len <3) {
		#if debugger
		TracePrintf(CANAPP_TRACE, TRACE_WARNING, "recv data is Error!\r\n");
		#endif
		return;
	}
	MsgId = *(uint16 *)(data + 1);
	realMsgId = MsgId - COM_TX_SIGNAL_START_INDEX;
	switch (MsgId) {
	case 0xFFFF:
		CanApp_Status_Req = TRUE;
		g_CanMsgRxTimeoutFlag = TRUE;
		break;
	case 0xFFFE:
		//request by fuyuanli ensure voice ctl window,add by @2020-1-6
		window_data[0] = ((*(uint32*)&data[3]) >> 0 ) & 0xf ;
		window_data[1] = ((*(uint32*)&data[3]) >> 4 ) & 0xf ;
		window_data[2] = ((*(uint32*)&data[3]) >> 8 ) & 0xf ;
		window_data[3] = ((*(uint32*)&data[3]) >>12 ) & 0xf ;

		Com_SetSignal(SIG_HND_FL_WindowSW, &window_data[0]);
		Com_SetSignal(SIG_HND_FR_WindowSW, &window_data[1]);
		Com_SetSignal(SIG_HND_RL_WindowSW, &window_data[2]);
		if( 0xffff == ((*(uint32*)&data[3])&0xffff) )//invaild data not send to CAN BUS
		{
			Com_SetSignal(SIG_HND_RR_WindowSW, &window_data[3]);
		}
		else
		{
			Com_SendSignal(SIG_HND_RR_WindowSW, &window_data[3]);//triger send 3 frame
		}

		break;

	default:
		//TracePrintf(CANAPP_TRACE, TRACE_WARNING, "recv data len: %d!\r\n", len);
		if(len < 7)break;
		len --;
		if((len % 6) != 0)break;
		data++;
		while (len > 0) {


			MsgId = *(uint16 *)(data);
			realMsgId = MsgId - COM_TX_SIGNAL_START_INDEX;
			if (COM_TX_SIGNAL_MAX_NUM <= realMsgId) {
				TracePrintf(CANAPP_TRACE, TRACE_INFO, "recv id Erroe  id: %d!\r\n", MsgId);
				break;
			}
			TracePrintf(CANAPP_TRACE,TRACE_INFO,"signal[%d]:0x%x from SOC\r\n",MsgId,*(uint32*)&data[2]);
			#if ((SYS_OS_NOOS != SYS_OS_TYPE)&&(STD_OFF == SYS_OS_CONTEXT_AUTOGET))
    		l_queueIndex = CQueue_Push(CanAppChannel, &realMsgId, 1, STD_OFF);
			#else
    		l_queueIndex = CQueue_Push(CanAppChannel, &realMsgId, 1);
			#endif
			if(l_queueIndex!= CQUEUE_ERR_PUSHPOP)
			{
				memcpy((uint8 *)&CanApp_RecvHandle[l_queueIndex][0], &data[2], 4);
				TracePrintf(CANAPP_TRACE,TRACE_INFO,"push Queue\r\n");
			}
			len  -= 6;
			data += 6;
		}
		break;
	}
}

void CanApp_setMsg_0x5e5_CheckSum(Com_SignalIdType SignalId)
{
	uint16 l_pduId = Com_SigId2IpduId[SignalId];
	uint8 l_checkSum_L = 0;
	uint8 l_checkSum_H = 0;
	uint8 l_checkSum = *Com_IpduBufPtr[l_pduId];
	uint8 i = 0;
	for(i=1;i<7;i++)
	{
		l_checkSum = l_checkSum ^ (*(Com_IpduBufPtr[l_pduId]+i));
	}
	l_checkSum_L = (l_checkSum ^ ((*(Com_IpduBufPtr[l_pduId]+i)) & 0x0F)) & 0x0F;
	l_checkSum_H = (l_checkSum & 0xF0)>>4;
	l_checkSum = l_checkSum_L ^ l_checkSum_H;
	Com_SetSignal(SIG_HND_check_sum, &l_checkSum);	
}

void CanApp_sendSpeakerReadysignal(void)
{
	uint16 l_queueIndex = CQUEUE_ERR_PUSHPOP;
	uint16 realMsgId = CQUEUE_ERR_PUSHPOP;
	uint8 l_data = 1;
	realMsgId = SIG_HND_TCU_SpeakerReady - COM_TX_SIGNAL_START_INDEX;
	// This code is not executed when the air conditioning app is up, so it will not conflict with ipc 
	l_queueIndex = CQueue_Push(CanAppChannel, &realMsgId, 1, STD_OFF);
	if(l_queueIndex!= CQUEUE_ERR_PUSHPOP)
	{
		memcpy((uint8 *)&CanApp_RecvHandle[l_queueIndex][0], &l_data, 1);
	}

}

static void CanApp_GpsTimeUpdate(void)
{
	ExtrRtc_StdTimeGet(&gps_time_struct);
	Com_SetSignal(SIG_HND_YEAR, &gps_time_struct.year);
	Com_SetSignal(SIG_HND_MONTH, &gps_time_struct.month);
	Com_SetSignal(SIG_HND_DAY, &gps_time_struct.day);
	Com_SetSignal(SIG_HND_HOUR, &gps_time_struct.hour);
	Com_SetSignal(SIG_HND_MINUTE, &gps_time_struct.minutes);
	Com_SetSignal(SIG_HND_SECOND, &gps_time_struct.second);
}


void CanApp_TxHand()
{
	static uint8 gps_counter = 0;
	/*
	 * Gps time update
	 */
	if (gps_counter++ % 25 == 0)
		CanApp_GpsTimeUpdate();

}

/*============================= END OF FILE: CanApp.c ============================*/
