/*============================[R E V I S I O N   H I S T O R Y]=================================*/
/************************************************************************************************/
/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * The integer value 1 is being added or subtracted from a pointer.
 */
 
/* MISRA-C:2004 Rule 19.1,
 * #include statements in a file should only be preceded by other preprocessor directives or comments.
 */
  
/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

#define NM_C_AR_MAJOR_VERSION   5U
#define NM_C_AR_MINOR_VERSION   2U
#define NM_C_AR_PATCH_VERSION   0U

#define NM_C_SW_MAJOR_VERSION   1U
#define NM_C_SW_MINOR_VERSION   0U
#define NM_C_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "Platform_Types.h"
#include "System_Tick.h"
#include "CanIf.h"
#include "Com_If.h"
#include "Com.h"
#include "trace.h"
#include "dtcApp_if.h"
#include "Can_Nm.h"
#include "SleepM.h"
#include "Com_Cfg.h"
#include "main.h"

/*============================[V E R S I O N  C H E C K]========================================*/
/************************************************************************************************/

#if (NM_C_AR_MAJOR_VERSION != NM_H_AR_MAJOR_VERSION)
#error "Nm.c:Mismatch in Specification Major Version"
#endif

#if (NM_C_AR_MINOR_VERSION != NM_H_AR_MINOR_VERSION)
#error "Nm.c:Mismatch in Specification Minor Version"
#endif

#if (NM_C_AR_PATCH_VERSION != NM_H_AR_PATCH_VERSION)
#error "Nm.c:Mismatch in Specification Patch Version"
#endif

#if (NM_C_SW_MAJOR_VERSION != NM_H_SW_MAJOR_VERSION)
#error "Nm.c:Mismatch in Specification Major Version"
#endif

#if (NM_C_SW_MINOR_VERSION != NM_H_SW_MINOR_VERSION)
#error "Nm.c:Mismatch in Specification Minor Version"
#endif


/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/

#define FAULT_ACCOUNT_INSTANCE 2
/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/


FaultAccountingChannelType faultAccount[FAULT_ACCOUNT_INSTANCE];

/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/
#define CAN_NM_TX_MESSAGE_NUM	3
const CanNmPduType CanNm_TxPduInfo[CAN_NM_TX_MESSAGE_NUM] =
{
	{0x635,0},/* inner pdu map num , instance : DA */
	
	{0x602,1},/* METER */
	{0x5C5,1},/* METER */
};

#define CAN_NM_RX_MESSAGE_NUM	2
const CanNmPduType CanNm_RxPduInfo[CAN_NM_RX_MESSAGE_NUM] =
{
	{0x35D,0},/* inner pdu map num , instance : DA */
	{0x351,0},/*  DA */
};

extern void CanApp_setMsg_0x5e5_CheckSum(Com_SignalIdType SignalId);

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/

/*============================[D A T A  M A N A G E M E N T] ===================================*/
/************************************************************************************************/

/*============================[T I M E R  M A N A G E M E N T] =================================*/
/************************************************************************************************/
/* Internal function, used to start timeout timer */
static void NM_StartTimer( TimerOutType *  pTimer , uint32 timerTick);
#if 0
/* Internal function, used to stop timeout timer */
static void NM_StopTimer( TimerOutType *  pTimer );
#endif
/* Internal function, used to check if wakeup timeout timer is timeout */
static TimerReturnType NM_CheckTimer( TimerOutType *  timeOutTimer );
/*============================[S T A T E  M A N A G E M E N T] =================================*/
/************************************************************************************************/

/*============================[L O C A L  F U N C T I O N S  D E F I N I T I O N] ==============*/
/************************************************************************************************/
/* Internal function, used to stop timeout timer */
static void NM_StartTimer( TimerOutType *  pTimer , uint32 timerTick)
{
	pTimer->start = TRUE;
	(void)GetCounterValue(0, &pTimer->startTime);
	pTimer->remainTime = timerTick ;
}

#if 0
/* Internal function, used to stop timeout timer */
static void NM_StopTimer( TimerOutType *  pTimer )
{
	pTimer->start = FALSE;
	pTimer->remainTime = 0u;
}
#endif

/* Internal function, used to check if wakeup timeout timer is timeout */
static TimerReturnType NM_CheckTimer( TimerOutType *  timeOutTimer )
{
	TimerReturnType returnVlu = TIMER_EXECUTED;
	TickType elapsedTick;

	if ( TRUE == timeOutTimer->start )
	{
		//1st : updata start time , caculate elapsed time
		GetElapsedCounterValue(0,&timeOutTimer->startTime,&elapsedTick);

		//2nd : arrive judge
		if ( timeOutTimer->remainTime < elapsedTick )
		{
			returnVlu = TIMER_ARRIVE;
		}
		else
		{
			timeOutTimer->remainTime -= elapsedTick ;
			if( timeOutTimer->remainTime <= NM_TIMER_TOLERANCE )
			{
				returnVlu = TIMER_ARRIVE;
			}
		}

		//3rd : arrived deal
		if( returnVlu == TIMER_ARRIVE )
		{
			//timeOutTimer->start = FALSE;
			timeOutTimer->remainTime = 0x0u;
		}
	}

	return returnVlu;
}

/************************************************************************************************/
/*
 * Brief               This service to findout the pdu id if not in management
 * ServiceId           0x09
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      pduId:Identification of the network through PDU-ID;
 *                     pCanNmPdu:pointer to pdu map
 *                     I-canInstance:can instance
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanNm_RxIndication CanNm_TxConfirmation
 */
/************************************************************************************************/
boolean CanNm_FindPduId( PduIdType pduId , CanNmPduType  *pCanNmPdu , uint8 *canInstance )
{
	//uint8 i;
	boolean findOut = FALSE ;
/*
	CanNmPduType sCanNmPdu ;

	uint16 total_len = 10 ;// sizeof(pCanNmPdu) / sizeof(sCanNmPdu) ;

	for( i =0 ; i < total_len ; i++ )
	{
		if( pduId == pCanNmPdu[i]->canPduId )
		{
			*canInstance = pCanNmPdu[i]->canInstace ;
			findOut = TRUE ;
			break ;
		}
	}
*/
	return findOut ;
}
/************************************************************************************************/
/*
 * Brief               This service  indicates a successful reception of a
 *                     received NM message to the CanNm after passing all
 *                     filters and validation checks.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void CanNm_RxIndication( uint8_t canInstance ,uint8 masterWakeUpSleepCmd )
{
	SleepM_RxIndication(0,masterWakeUpSleepCmd);
}
void CanNm_EcuRxIndication( uint8_t canInstance )
{
	if( canInstance >= FAULT_ACCOUNT_INSTANCE )
	{
		return;
	}
	
	FaultAccountingChannelType *pfaultAccount = &faultAccount[canInstance];
	pfaultAccount->receptionStatus = PRESENT ;

	NM_StartTimer(&pfaultAccount->rxJT1,JT_JUDGMENTTIMER_1);
	NM_StartTimer(&pfaultAccount->rxJT2,JT_JUDGMENTTIMER_2);

	SleepM_EcuIndication(canInstance);
}

/************************************************************************************************/
/*
 * Brief               This service confirms a previous successfully processed
 *                     CAN transmit request.
 * ServiceId           0x01
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
 /************************************************************************************************/
void CanNm_TxConfirmation( uint8_t canInstance )
{
	if( canInstance >= FAULT_ACCOUNT_INSTANCE )
	{
		return;
	}
	FaultAccountingChannelType *pfaultAccount = &faultAccount[canInstance];
	pfaultAccount->transmissionStatus = NOT_MUTE ;

	NM_StartTimer(&pfaultAccount->txJT1,JT_JUDGMENTTIMER_1);
	NM_StartTimer(&pfaultAccount->txJT2,JT_JUDGMENTTIMER_2);
}
/************************************************************************************************/
/*
 * Brief               This service confirms a can driver trasmit buss off processed
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      canInstance:Identification of the CAN channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void CanNm_BusOffCbk( uint8_t canInstance )
{
	if( canInstance >= FAULT_ACCOUNT_INSTANCE )
	{
		return;
	}

	//FaultAccountingChannelType *pfaultAccount =  &faultAccount[canInstance];

	dtcAppReportBusOffState(TRUE);

	//reinit can driver
	CanIf_SetControllerMode(canInstance, CANIF_CS_STARTED);
	
	//NM_StartTimer(&pfaultAccount->JT1,JT_JUDGMENTTIMER_1);
	//NM_StartTimer(&pfaultAccount->JT2,JT_JUDGMENTTIMER_2);

}
/************************************************************************************************/
/*
 * Brief               Main function of the NdsCanNm bus off  management which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      canInstance:Identification of the CAN channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void BusOffM_MainFunction( const FaultAccountInstanceType canInstance )
{
	//	dtcAppReportCanNodeLostFlag(pfaultAccount->receptionStatus, MSG_LOST_FLAG_NUS_OFF);
}
/************************************************************************************************/
/*
 * Brief               Internal function, send out refusetosleep state frame.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
void CanNm_SendWakeUpFrame()
{
	static uint8 sendOnce = TRUE;
	uint8 wakeup = 0 ;
	PduInfoType pudBuf,pudBuf2;
	
	pudBuf.SduDataPtr = Com_IpduBufPtr[IPDU_HND_CLUSTER_WakeUpFrame];
	pudBuf.SduLength = Com_TxIpduConfInfo[IPDU_HND_CLUSTER_WakeUpFrame - COM_TX_IPDU_START_INDEX ].PduLength;
	*pudBuf.SduDataPtr = wakeup ;
	
	pudBuf2.SduDataPtr = Com_IpduBufPtr[IPDU_HND_DA_WakeUpFrame];
	pudBuf2.SduLength = Com_TxIpduConfInfo[IPDU_HND_DA_WakeUpFrame - COM_TX_IPDU_START_INDEX ].PduLength;
	*pudBuf2.SduDataPtr = wakeup ;
	
	if( sendOnce == TRUE )
	{
		sendOnce = FALSE;
		CanIf_Transmit( ( IPDU_HND_CLUSTER_WakeUpFrame - COM_TX_IPDU_START_INDEX ), &pudBuf);
		CanIf_Transmit( ( IPDU_HND_DA_WakeUpFrame - COM_TX_IPDU_START_INDEX ), &pudBuf2);
	}
}
/************************************************************************************************/
/*
 * Brief               Internal function, send out refusetosleep state frame.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
void CanNm_SendRefuseToSleepFrame(SleepMSlaveSleepCmdType refuseTosleep)
{
	static uint8 sendOnce = TRUE;
	PduInfoType	pudBuf,pudBuf2;
	//uint8 l_value = 0;	
	//l_value = refuseTosleep;
	pudBuf.SduDataPtr = Com_IpduBufPtr[IPDU_HND_CLUSTER_GeneralStatus];
	pudBuf.SduLength = Com_TxIpduConfInfo[IPDU_HND_CLUSTER_GeneralStatus - COM_TX_IPDU_START_INDEX ].PduLength;
	//Com_TxSig_CLUSTER_REFUSE(TRUE,&refuseTosleep);
	*pudBuf.SduDataPtr &= 0X3F ;
	*pudBuf.SduDataPtr |= ((refuseTosleep<<6)&0XC0) ;

	#if 0
	pudBuf2.SduDataPtr = Com_IpduBufPtr[IPDU_HND_TCU_DA_information];
	pudBuf2.SduLength = Com_TxIpduConfInfo[IPDU_HND_TCU_DA_information - COM_TX_IPDU_START_INDEX ].PduLength;
	*pudBuf2.SduDataPtr &= 0X3F ;
	*pudBuf2.SduDataPtr |= ((refuseTosleep<<6)&0XC0) ;
	#else
	pudBuf2.SduDataPtr = Com_IpduBufPtr[IPDU_HND_IKEY_Display_SwitchStatus];
	pudBuf2.SduLength = Com_TxIpduConfInfo[IPDU_HND_IKEY_Display_SwitchStatus - COM_TX_IPDU_START_INDEX ].PduLength;
	*pudBuf2.SduDataPtr &= 0xF3;//0X3F ;
	*pudBuf2.SduDataPtr |= ((refuseTosleep<<2)&0x0C) ;
	//Com_SetSignal(SIG_HND_DA_RefusetoSleep, &l_value);
	CanApp_setMsg_0x5e5_CheckSum(SIG_HND_DA_RefusetoSleep);
	
	#endif
	if( sendOnce == TRUE )
	{
		sendOnce = FALSE;
		CanIf_Transmit( ( IPDU_HND_CLUSTER_GeneralStatus - COM_TX_IPDU_START_INDEX ), &pudBuf);
		CanIf_Transmit( ( IPDU_HND_IKEY_Display_SwitchStatus - COM_TX_IPDU_START_INDEX ), &pudBuf2);
	}
}


/************************************************************************************************/
/*
 * Brief               Main function of the NdsCanNm state management which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      canInstance:Identification of the CAN channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void StateM_MainFunction( const FaultAccountInstanceType canInstance )
{
	FaultAccountingChannelType *pfaultAccount = &faultAccount[canInstance];
	//if( AppSoftWare_CurrChartStateGet(0))
	//receptionStatus management
	if( TIMER_ARRIVE == NM_CheckTimer(&pfaultAccount->rxJT1) )
	{
		NM_StartTimer(&pfaultAccount->rxJT1,JT_JUDGMENTTIMER_1);
		if( TIMER_ARRIVE == NM_CheckTimer(&pfaultAccount->rxJT2) )
		{
			pfaultAccount->receptionStatus = CONFIRMED_ABSENT ;
		}
		else
		{
			pfaultAccount->receptionStatus = ABSENT ;
		}
		//	dtcAppReportCanNodeLostFlag(pfaultAccount->receptionStatus, MSG_LOST_FLAG_RX);
	}
	//transmissionStatus management
	if( TIMER_ARRIVE == NM_CheckTimer(&pfaultAccount->txJT1) )
	{
		NM_StartTimer(&pfaultAccount->txJT1,JT_JUDGMENTTIMER_1);
		if( TIMER_ARRIVE == NM_CheckTimer(&pfaultAccount->txJT2) )
		{
			pfaultAccount->transmissionStatus = CONFIRMED_MUTE ;
		}
		else
		{
			pfaultAccount->transmissionStatus = MUTE ;
		}
		//dtcAppReportCanNodeLostFlag(pfaultAccount->transmissionStatus, MSG_LOST_FLAG_TX);
	}
}
/************************************************************************************************/
/*
 * Brief               Init function of the NdsCanNm which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      canInstance:Identification of the CAN channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void CanNm_Init()
{
	SleepM_Init();
}
/*************************************************************************/
/*
 * Brief               test of the CanNm trigger by trace.
 * ServiceId           None
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      tStrArrayPtr strArrayPtr, uint8 arrayNum
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           trace uart interrrupt callback
 */
/*************************************************************************/
void CanNm_TestByTrace(tStrArrayPtr strArrayPtr, uint8 arrayNum)
{
	switch( strArrayPtr[0][0] )
	{
		case 'S':
			//CanNm_MyTest.traceTrigCanNm_PassiveStartUp = TRUE;
		break;

		default:
		break;
	}
}
/************************************************************************************************/
/*
 * Brief               Main function of the NdsCanNm which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      canInstance:Identification of the CAN channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void CanNm_MainFunction( const FaultAccountInstanceType canInstance )
{
	//FaultAccountingChannelType *pfaultAccount;
	if( canInstance < FAULT_ACCOUNT_INSTANCE )
	{
		//pfaultAccount = &faultAccount[canInstance];
		SleepM_MainFunction(canInstance);
		StateM_MainFunction(canInstance);
	}
}

