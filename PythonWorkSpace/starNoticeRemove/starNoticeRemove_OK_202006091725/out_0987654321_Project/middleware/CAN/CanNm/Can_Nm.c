




 

  



#define NM_C_AR_MAJOR_VERSION   5U
#define NM_C_AR_MINOR_VERSION   2U
#define NM_C_AR_PATCH_VERSION   0U

#define NM_C_SW_MAJOR_VERSION   1U
#define NM_C_SW_MINOR_VERSION   0U
#define NM_C_SW_PATCH_VERSION   0U




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





#define FAULT_ACCOUNT_INSTANCE 2




FaultAccountingChannelType faultAccount[FAULT_ACCOUNT_INSTANCE];



#define CAN_NM_TX_MESSAGE_NUM	3
const CanNmPduType CanNm_TxPduInfo[CAN_NM_TX_MESSAGE_NUM] =
{
	{0x635,0},
	
	{0x602,1},
	{0x5C5,1},
};

#define CAN_NM_RX_MESSAGE_NUM	2
const CanNmPduType CanNm_RxPduInfo[CAN_NM_RX_MESSAGE_NUM] =
{
	{0x35D,0},
	{0x351,0},
};

extern void CanApp_setMsg_0x5e5_CheckSum(Com_SignalIdType SignalId);










static void NM_StartTimer( TimerOutType *  pTimer , uint32 timerTick);
#if 0

static void NM_StopTimer( TimerOutType *  pTimer );
#endif

static TimerReturnType NM_CheckTimer( TimerOutType *  timeOutTimer );






static void NM_StartTimer( TimerOutType *  pTimer , uint32 timerTick)
{
	pTimer->start = TRUE;
	(void)GetCounterValue(0, &pTimer->startTime);
	pTimer->remainTime = timerTick ;
}

#if 0

static void NM_StopTimer( TimerOutType *  pTimer )
{
	pTimer->start = FALSE;
	pTimer->remainTime = 0u;
}
#endif


static TimerReturnType NM_CheckTimer( TimerOutType *  timeOutTimer )
{
	TimerReturnType returnVlu = TIMER_EXECUTED;
	TickType elapsedTick;

	if ( TRUE == timeOutTimer->start )
	{
		
		GetElapsedCounterValue(0,&timeOutTimer->startTime,&elapsedTick);

		
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

		
		if( returnVlu == TIMER_ARRIVE )
		{
			
			timeOutTimer->remainTime = 0x0u;
		}
	}

	return returnVlu;
}




boolean CanNm_FindPduId( PduIdType pduId , CanNmPduType  *pCanNmPdu , uint8 *canInstance )
{
	
	boolean findOut = FALSE ;

	return findOut ;
}



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



void CanNm_BusOffCbk( uint8_t canInstance )
{
	if( canInstance >= FAULT_ACCOUNT_INSTANCE )
	{
		return;
	}

	

	dtcAppReportBusOffState(TRUE);

	
	CanIf_SetControllerMode(canInstance, CANIF_CS_STARTED);
	
	
	

}



void BusOffM_MainFunction( const FaultAccountInstanceType canInstance )
{
	
}



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



void CanNm_SendRefuseToSleepFrame(SleepMSlaveSleepCmdType refuseTosleep)
{
	static uint8 sendOnce = TRUE;
	PduInfoType	pudBuf,pudBuf2;
	
	
	pudBuf.SduDataPtr = Com_IpduBufPtr[IPDU_HND_CLUSTER_GeneralStatus];
	pudBuf.SduLength = Com_TxIpduConfInfo[IPDU_HND_CLUSTER_GeneralStatus - COM_TX_IPDU_START_INDEX ].PduLength;
	
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
	*pudBuf2.SduDataPtr &= 0xF3;
	*pudBuf2.SduDataPtr |= ((refuseTosleep<<2)&0x0C) ;
	
	CanApp_setMsg_0x5e5_CheckSum(SIG_HND_DA_RefusetoSleep);
	
	#endif
	if( sendOnce == TRUE )
	{
		sendOnce = FALSE;
		CanIf_Transmit( ( IPDU_HND_CLUSTER_GeneralStatus - COM_TX_IPDU_START_INDEX ), &pudBuf);
		CanIf_Transmit( ( IPDU_HND_IKEY_Display_SwitchStatus - COM_TX_IPDU_START_INDEX ), &pudBuf2);
	}
}





void StateM_MainFunction( const FaultAccountInstanceType canInstance )
{
	FaultAccountingChannelType *pfaultAccount = &faultAccount[canInstance];
	
	
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
		
	}
	
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
		
	}
}



void CanNm_Init()
{
	SleepM_Init();
}



void CanNm_TestByTrace(tStrArrayPtr strArrayPtr, uint8 arrayNum)
{
	switch( strArrayPtr[0][0] )
	{
		case 'S':
			
		break;

		default:
		break;
	}
}



void CanNm_MainFunction( const FaultAccountInstanceType canInstance )
{
	
	if( canInstance < FAULT_ACCOUNT_INSTANCE )
	{
		
		SleepM_MainFunction(canInstance);
		StateM_MainFunction(canInstance);
	}
}

