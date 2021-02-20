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

#define SLEEPM_C_AR_MAJOR_VERSION   5U
#define SLEEPM_C_AR_MINOR_VERSION   2U
#define SLEEPM_C_AR_PATCH_VERSION   0U

#define SLEEPM_C_SW_MAJOR_VERSION   1U
#define SLEEPM_C_SW_MINOR_VERSION   0U
#define SLEEPM_C_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "Platform_Types.h"
#include "System_Tick.h"
#include "trace.h"
#include "SleepM.h"
#include "System_Signal.h"
#include "Can_Nm.h"

#include "BinSig_Ign_Cfg.h"
#include "BinSig_Acc_Cfg.h"
/*============================[V E R S I O N  C H E C K]========================================*/
/************************************************************************************************/

#if (SLEEPM_C_AR_MAJOR_VERSION != SLEEPM_H_AR_MAJOR_VERSION)
#error "SleepM.c:Mismatch in Specification Major Version"
#endif

#if (SLEEPM_C_AR_MINOR_VERSION != SLEEPM_H_AR_MINOR_VERSION)
#error "SleepM.c:Mismatch in Specification Minor Version"
#endif

#if (SLEEPM_C_AR_PATCH_VERSION != SLEEPM_H_AR_PATCH_VERSION)
#error "SleepM.c:Mismatch in Specification Patch Version"
#endif

#if (SLEEPM_C_SW_MAJOR_VERSION != SLEEPM_H_SW_MAJOR_VERSION)
#error "SleepM.c:Mismatch in Specification Major Version"
#endif

#if (SLEEPM_C_SW_MINOR_VERSION != SLEEPM_H_SW_MINOR_VERSION)
#error "SleepM.c:Mismatch in Specification Minor Version"
#endif


/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/

#define SLEEP_MANAGEMENT_INSTANCE	1

/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/

SleepMInnerChannelType sSleepM[SLEEP_MANAGEMENT_INSTANCE];

volatile uint8 gSleepM_WakeUp_Flag2 = FALSE;

/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/

/* Application function, used to set SleepCondition */
void AppSoftWare_SleepConditonSet( const SleepManagementType smChannel , SleepMSleepConditionType sleepConditon );

/* Application function, used to get current chart state */
SleepMChartStateType AppSoftWare_CurrChartStateGet( const SleepManagementType smChannel );


/*============================[D A T A  M A N A G E M E N T] ===================================*/
/************************************************************************************************/

/* Internal function, used to copy data */
static void SleepM_Memcpy(uint8 *  dest, const uint8 *  source, uint8 length );

/* Internal function, used to reset data */
static void SleepM_Memset(uint8 *  dest, uint8 value, uint8 length );

/*============================[T I M E R  M A N A G E M E N T] =================================*/
/************************************************************************************************/

/* Internal function, used to start timeout timer */
static void SleepM_StartTimer( TimerOutType *  pTimer , uint32 timerTick);

/* Internal function, used to stop timeout timer */
static void SleepM_StopTimer( TimerOutType *  pTimer );

/* Internal function, used to check if wakeup timeout timer is timeout */
static TimerReturnType SleepM_CheckTimer( TimerOutType *  timeOutTimer );

/*============================[S T A T E  M A N A G E M E N T] =================================*/
/************************************************************************************************/


/*============================[L O C A L  F U N C T I O N S  D E F I N I T I O N] ==============*/
/************************************************************************************************/


/************************************************************************************************/
/*
 * Brief               Internal function, used to reset data.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *dest:data copy from
 * Param-Name[in]      *source:data copy to
 * Param-Name[in]      length:data copy length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
static void SleepM_Memcpy( uint8 *  dest, const uint8 *  source, uint8 length )
{
	if( ( NULL != dest ) && ( NULL != source ) )
	{
		while (length > 0u)
		{
			length--;
			*dest = *source;
			dest++;
			source++;
		}
	}
}
/************************************************************************************************/
/*
 * Brief               Internal function, used to reset data.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *dest:data set to
 * Param-Name[in]      value:need set value
 * Param-Name[in]      length:data set length
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Layer
 */
/************************************************************************************************/
static void SleepM_Memset( uint8 *  dest, uint8 value, uint8 length )
{
	if( NULL != dest )
	{
		while (length > 0u)
		{
			length--;
			*dest = value;
			dest++;
		}
	}
}
/************************************************************************************************/
/*
 * Brief               Internal function, used to stop timeout timer.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *timeOutTimer:point sleep timer struct
 * Param-Name[in]      remainTime:remain Time
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static void SleepM_StartTimer( TimerOutType *  pTimer , uint32 remainTime)
{
	pTimer->start = TRUE;
	(void)GetCounterValue(0, &pTimer->startTime);
	pTimer->remainTime = remainTime ;
}
/************************************************************************************************/
/*
 * Brief               Internal function, used to stop timeout timer.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *timeOutTimer:point sleep timer struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static void SleepM_StopTimer( TimerOutType *  pTimer )
{
	pTimer->start = FALSE;
	pTimer->remainTime = 0u;
}
/************************************************************************************************/
/*
 * Brief               Internal function, used to check if wakeup timeout timer is timeout.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *timeOutTimer:point sleep timer struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static TimerReturnType SleepM_CheckTimer( TimerOutType *  timeOutTimer )
{
	TickType elapsedTick;
	TimerReturnType returnVlu = TIMER_ARRIVE;

	if ( TRUE == timeOutTimer->start )
	{
		returnVlu = TIMER_EXECUTED;
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
			if( timeOutTimer->remainTime <= SLEEPM_TIMER_TOLERANCE )
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
 * Brief               Internal function, CAN init.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *innerChannel:point sleep management struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static void SleepM_CanInit(SleepMInnerChannelType *  innerChannel )
{
	#if ( SLEEPM_SLAVE == SLEEPM_PROPERTY )
		/* @[E_25953NDS05_2]-4.5.1 */
		innerChannel->slaveRefuseToSleep = S_ReadyToSleep;
		//this line conflict when CAN BUS interrupt frame received ,which which inculde masterWakeUpCmd = M_WakeUp 
		//innerChannel->masterWakeUpSleepCmd = M_GoToSleep;
	#else
	#endif
}
/************************************************************************************************/
/*
 * Brief               Internal function, self diag.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *innerChannel:point sleep management struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static TimerReturnType SleepM_SelfDiag(SleepMInnerChannelType *  innerChannel )
{
	TimerReturnType selfDiagCplt = TIMER_ARRIVE;
	
	return  selfDiagCplt;
}
/************************************************************************************************/
/*
 * Brief               Internal function, send out wakeup frame.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *innerChannel:point sleep management struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static void SleepM_SendWakeUpFrame(SleepMInnerChannelType *  innerChannel )
{
	CanNm_SendWakeUpFrame();	
}
/************************************************************************************************/
/*
 * Brief               Internal function, send out refusetosleep state frame.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *innerChannel:point sleep management struct
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
static void SleepM_SendRefuseToSleepFrame(SleepMInnerChannelType *  innerChannel )
{
	CanNm_SendRefuseToSleepFrame(innerChannel->slaveRefuseToSleep);
}
/************************************************************************************************/
/*
 * Brief               Main function of the NdsCanSm initial.
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
extern boolean gSoftWareResetFlag;
void SleepM_Init()
{
	SleepManagementType smChannel ;	
	SleepMInnerChannelType *pSleepM = NULL;
	
	for( smChannel = 0  ; smChannel < SLEEP_MANAGEMENT_INSTANCE ; smChannel++ )
	{
		pSleepM = &sSleepM[smChannel];
		
		pSleepM->state = (TRUE == gSoftWareResetFlag)?SLEEP:HARDWARE_RESET ;
		pSleepM->prestate = pSleepM->state;
		pSleepM->emissionEn = Emission_OFF ;
		pSleepM->sleepCondition = SC_AcceptSleep ;
		pSleepM->slaveRefuseToSleep = S_ReadyToSleep ;
		pSleepM->masterWakeUpSleepCmd = M_GoToSleep ;
		
		pSleepM->ecuFramesFlag = FALSE ;
		pSleepM->interruptFlag = FALSE;
		
		pSleepM->initFlag = TRUE ;
	}
}
/************************************************************************************************/
/*
 * Brief               software set sleep management condition.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *pSleepM:point sleep management struct
 * Param-Name[in]      sleepConditon:sleep condition from application software 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
void AppSoftWare_SleepConditonSet( const SleepManagementType smChannel , SleepMSleepConditionType sleepConditon )
{
	SleepMInnerChannelType *pSleepM = NULL;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	}

	pSleepM->sleepCondition = sleepConditon ;
}
/************************************************************************************************/
/*
 * Brief               software get sleep management state.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *pSleepM:point sleep management struct
 * Param-Name[out]     SleepMChartStateType ,state of sleep management state 
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Up Software layer
 */
/************************************************************************************************/
SleepMChartStateType AppSoftWare_CurrChartStateGet( const SleepManagementType smChannel )
{
	SleepMInnerChannelType *pSleepM = NULL;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	}

	return pSleepM->state;
}
/************************************************************************************************/
/*
 * Brief               ECU Frame Deal.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      smChannel:sleep management channel
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           CanNm_Rxindication
 */
/************************************************************************************************/
void SleepM_RxIndication( const SleepManagementType smChannel ,uint8 masterWakeUpSleepCmd )
{
	SleepMInnerChannelType *pSleepM = NULL;
	
	if( smChannel >= SLEEP_MANAGEMENT_INSTANCE )
	{
		return;
	}
	pSleepM = &sSleepM[smChannel];

	pSleepM->masterCmdFlag = TRUE;
	pSleepM->masterWakeUpSleepCmd = masterWakeUpSleepCmd;
}
void SleepM_EcuIndication( const SleepManagementType smChannel )
{
	SleepMInnerChannelType *pSleepM = NULL;
	
	if( smChannel >= SLEEP_MANAGEMENT_INSTANCE )
	{
		return;
	}
	pSleepM = &sSleepM[smChannel];

	pSleepM->interruptFlag = TRUE;
	pSleepM->ecuFramesFlag = TRUE;
}

/************************************************************************************************/
/*
 * Brief               sleep management fuction.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_Nm
 */
/************************************************************************************************/
#if ( SLEEPM_SLAVE == SLEEPM_PROPERTY )

#if ( SLEEPM_COMPLETE == SLEEPM_STATECHARTMODEL )
void SleepM_CompleteModeManage(SleepMInnerChannelType *pSleepM)
{
	#if (STD_ON == MAIN_SM_STATE_USE_TIMER)
		//used for fix some bug :add at 2020-6-5
		//when NM MSG wake up IT-MASTER,some SM time(S1) not match
		static boolean s_check_application_state_in_main = TRUE;
		if(TRUE == s_check_application_state_in_main)
		{
			s_check_application_state_in_main = FALSE;
			//if SM state was APPLICATION shoud send SYSTEM_SIG_ID_CAN_AWAKE to PM
			if(APPLICATION == pSleepM->state)
			{
				System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_ON, STD_OFF);//inform TASK_PM CAN_BUS ask for ECU wake
				Com_TxStart();
			}
		}
	#endif	
	//========================

	switch( pSleepM->state )
	{
		case HARDWARE_RESET :
		{
			pSleepM->emissionEn = Emission_OFF ;
			
			// jump to WAKE_UP_TRANSIENT
			if( TIMER_ARRIVE == SleepM_SelfDiag(pSleepM))
			{
				SleepM_CanInit(pSleepM);
				
				SleepM_StartTimer(&pSleepM->wakeUpTimer,S1_WAKEUPTIME);
				
				pSleepM->state = WAKE_UP_TRANSIENT;
				
				break; 
			}
			
		}break;
		case SLEEP :
		{
			pSleepM->emissionEn = Emission_OFF ;
		
			// jump to SLEEP
			if(( SC_AcceptSleep == pSleepM->sleepCondition )
			 &&( FALSE == pSleepM->interruptFlag ))
			{
				 pSleepM->state = SLEEP;
				 
				 break;
			}
			 
			// jump to WAKE_UP_TRANSIENT
			if( TRUE == pSleepM->interruptFlag )
			{
				pSleepM->interruptFlag = FALSE ;
				pSleepM->ecuFramesFlag = FALSE ;
							
				SleepM_CanInit(pSleepM);
				
				SleepM_StartTimer(&pSleepM->wakeUpTimer,S1_WAKEUPTIME);
				 
				pSleepM->state = WAKE_UP_TRANSIENT;
				
				break; 
			}
			
			// jump to STARTING_CAN_WAKE_UP
			if( SC_WakeUp == pSleepM->sleepCondition )
			{
				SleepM_CanInit(pSleepM);

				SleepM_SendWakeUpFrame(pSleepM);
				 
				SleepM_StartTimer(&pSleepM->applicationTimer,S2_APPLICATIONTIME);

				pSleepM->state = STARTING_CAN_WAKE_UP;

				break; 
			}
			
		}break;
		case WAKE_UP_TRANSIENT :
		{
			pSleepM->emissionEn = Emission_OFF ;

			// jump to STARTING_CAN_WAKE_UP
			if( SC_WakeUp == pSleepM->sleepCondition )
			{
				SleepM_SendWakeUpFrame(pSleepM);

				SleepM_StartTimer(&pSleepM->applicationTimer,S2_APPLICATIONTIME);
				 
				pSleepM->state = STARTING_CAN_WAKE_UP;

				break; 
			}
			
			// jump to APPLICATION
			if(( SC_AcceptSleep == pSleepM->sleepCondition ) &&
				(( M_WAKEUP == pSleepM->masterWakeUpSleepCmd ) ||(( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->wakeUpTimer) )&&( TRUE == pSleepM->ecuFramesFlag ))) )
			 {
			 	pSleepM->slaveRefuseToSleep = S_ReadyToSleep ;
				SleepM_SendRefuseToSleepFrame(pSleepM);

				SleepM_StartTimer(&pSleepM->cycleTimer,SA_CYCLETIME);

				System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_ON, STD_OFF);//inform TASK_PM CAN_BUS ask for ECU wake

				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);//inform TASK_COM can to send app data
				#else
				Com_TxStart();
				#endif
				pSleepM->state = APPLICATION;

				break; 
			 }
			
			// jump to SLEEP
			if(( SC_AcceptSleep == pSleepM->sleepCondition ) &&
				( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->wakeUpTimer) ) &&( FALSE == pSleepM->ecuFramesFlag ))
			 {
				System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_OFF, STD_OFF);//inform TASK_PM CAN_BUS ask to sleep
				
				pSleepM->ecuFramesFlag = FALSE ;
				pSleepM->interruptFlag = FALSE ;
				
				pSleepM->state = SLEEP;
				
				break; 
			 }
		
		}break;
		case STARTING_CAN_WAKE_UP :
		{
			pSleepM->emissionEn = Emission_OFF ;

			// jump to APPLICATION
			if( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->applicationTimer) )
			{
				pSleepM->slaveRefuseToSleep = S_RefuseToSleep ;
				SleepM_SendRefuseToSleepFrame(pSleepM);
				
				SleepM_StartTimer(&pSleepM->cycleTimer,SA_CYCLETIME);
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);//inform TASK_COM can to send app data
				#else
				Com_TxStart();
				#endif
				pSleepM->state = APPLICATION;
				
				break;
			}
			
		}break;
		case APPLICATION :
		{
			pSleepM->emissionEn = Emission_ON ;
			/* @[E_25953NDS05_2]-4.4.6 */ /* @[E_25953NDS05_2]-5.3.4.2 */
			if( TRUE == pSleepM->masterCmdFlag )
			{
				pSleepM->masterCmdFlag = FALSE;
				SleepM_StartTimer(&pSleepM->noOrderFromMasterTimer,SA_NO_ORDER_FROM_MASTER);

				/* @[E_25953NDS05_2]-5.3.4.3 */
				if( ( M_WAKEUP != pSleepM->masterWakeUpSleepCmd ) && ( M_GoToSleep != pSleepM->masterWakeUpSleepCmd ) )
				{
					if( TRUE != pSleepM->wrongMasterCmdFlag )
					{
						SleepM_StartTimer(&pSleepM->wrongOrderFromMasterTimer,SA_WRONG_ORDER_FROM_MASTER);
					}
					pSleepM->wrongMasterCmdFlag = TRUE;
					if( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->wrongOrderFromMasterTimer) )
					{
						pSleepM->masterWakeUpSleepCmd = M_GoToSleep ;
					}
				}
				else
				{
					pSleepM->wrongMasterCmdFlag = FALSE;
				}

			}
			else if( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->noOrderFromMasterTimer) )
			{
				pSleepM->masterWakeUpSleepCmd = M_GoToSleep ;
			}

			// jump to SLEEP_TRANSIENT
			if(( ( M_GoToSleep == pSleepM->masterWakeUpSleepCmd ) && ( SC_AcceptSleep == pSleepM->sleepCondition ) ))
			{
				SleepM_StartTimer(&pSleepM->sleepTimer,S4_SLEEPTIME);
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_OFF, STD_OFF);//inform TASK_COM can not send app data
				#else
				Com_TxStop();
				#endif
				pSleepM->state = SLEEP_TRANSIENT;

				break;
			}

			// jump to APPLICATION
			if( ( M_WAKEUP == pSleepM->masterWakeUpSleepCmd ) || ( SC_WakeUp == pSleepM->sleepCondition ) )
			{
				pSleepM->state = APPLICATION;
				if( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->cycleTimer) )
				{
					SleepM_StartTimer(&pSleepM->cycleTimer,SA_CYCLETIME);
					
					pSleepM->slaveRefuseToSleep = (SC_WakeUp == pSleepM->sleepCondition)?S_RefuseToSleep:S_ReadyToSleep;
					SleepM_SendRefuseToSleepFrame(pSleepM);
				}
				break;
			}

		}break;
		case SLEEP_TRANSIENT :
		{
			pSleepM->emissionEn = Emission_OFF ;

			// jump to APPLICATION
			if( ( M_WAKEUP == pSleepM->masterWakeUpSleepCmd ) || ( SC_WakeUp == pSleepM->sleepCondition ) )
			{
				pSleepM->slaveRefuseToSleep = (SC_WakeUp == pSleepM->sleepCondition)?S_RefuseToSleep:S_ReadyToSleep;
				SleepM_SendRefuseToSleepFrame(pSleepM);
				/* @[E_25953NDS05_2]-4.4.6 */
				pSleepM->state = APPLICATION;
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);//inform TASK_COM can not send app data
				#else
				Com_TxStart();
				#endif
				break;
			}

			// jump to SLEEP_TRANSIENT
			if( TRUE == pSleepM->ecuFramesFlag )
			{	
				pSleepM->ecuFramesFlag = FALSE;
				
				SleepM_StartTimer(&pSleepM->sleepTimer,S4_SLEEPTIME);
				
				pSleepM->state = SLEEP_TRANSIENT;
				
				break;
			}

			//ECU Frames stoped
			if( ( M_WAKEUP != pSleepM->masterWakeUpSleepCmd ) && ( M_GoToSleep != pSleepM->masterWakeUpSleepCmd ) )
			{
				pSleepM->masterWakeUpSleepCmd = M_GoToSleep;
			}

			// jump to SLEEP
			if( ( SC_AcceptSleep == pSleepM->sleepCondition )
				&& ( M_GoToSleep == pSleepM->masterWakeUpSleepCmd )
				&& ( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->sleepTimer) ))
			{
				if( TIMER_ARRIVE == SleepM_SelfDiag(pSleepM))
				{
					System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_OFF, STD_OFF);//inform TASK_PM can to sleep

					pSleepM->ecuFramesFlag = FALSE ;
					pSleepM->interruptFlag = FALSE ;

					pSleepM->state = SLEEP;

					break;
				}
			}
		}break;
		case STARTING_GO_TO_SLEEP :
		{}break;
		default :
		{}break;
	}

	if(pSleepM->state == SLEEP || pSleepM->state == SLEEP_TRANSIENT)
		Tp_SetTraceFlag(FALSE);
	else
		Tp_SetTraceFlag(TRUE);
}
#endif

#if ( SLEEPM_SIMPLIFIED == SLEEPM_STATECHARTMODEL )
void SleepM_SimplifiedModeManage(SleepMInnerChannelType *pSleepM)
{
	switch( pSleepM->state )
	{
		case HARDWARE_RESET :
		{}break;
		case SLEEP :
		{}break;
		case WAKE_UP_TRANSIENT :
		{}break;
		case APPLICATION :
		{}break;
		case SLEEP_TRANSIENT :
		{}break;
		default :
		{}break;
	}
}
#endif	

#if ( SLEEPM_EXCEPTIONAL == SLEEPM_STATECHARTMODEL )
void SleepM_ExceptionalModeManage(SleepMInnerChannelType *pSleepM)
{
	switch( pSleepM->state )
	{
		case HARDWARE_RESET :
		{}break;
		case SLEEP :
		{}break;
		case WAKE_UP_TRANSIENT :
		{}break;
		case APPLICATION :
		{}break;
		case SLEEP_TRANSIENT :
		{}break;
		default :
		{}break;
	}
}
#endif

#endif
/************************************************************************************************/
/*
 * Brief               printf sleep management state .
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      *pSleepM:point sleep management struct 
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           SleepM_MainFunction
 */
/************************************************************************************************/
void SleepM_StateChangePrintf( SleepMInnerChannelType *pSleepM )
{
	if( pSleepM->prestate == pSleepM->state )
	{
		return;
	}

	char stringP[0x40]="[NM-I]:SM-";
	switch(pSleepM->prestate)
	{
		case 	HARDWARE_RESET:strcat(stringP,"Reset");break;
		case 	SLEEP:strcat(stringP,"Sleep");break;
		case 	WAKE_UP_TRANSIENT:strcat(stringP,"WakeUpTransient");break;
		case 	STARTING_CAN_WAKE_UP:strcat(stringP,"StartCanWakeUp");break;
		case	APPLICATION:strcat(stringP,"Application");break;
		case	SLEEP_TRANSIENT:strcat(stringP,"SleepTransient");break;
		case 	STARTING_GO_TO_SLEEP:strcat(stringP,"StartGoToSleep");break;
		default:break;
	}
	strcat(stringP,"2");
	switch(pSleepM->state)
	{
		case 	HARDWARE_RESET:strcat(stringP,"Reset");break;
		case 	SLEEP:strcat(stringP,"Sleep");break;
		case 	WAKE_UP_TRANSIENT:strcat(stringP,"WakeUpTransient");break;
		case 	STARTING_CAN_WAKE_UP:strcat(stringP,"StartCanWakeUp");break;
		case	APPLICATION:strcat(stringP,"Application");break;
		case	SLEEP_TRANSIENT:strcat(stringP,"SleepTransient");break;
		case 	STARTING_GO_TO_SLEEP:strcat(stringP,"StartGoToSleep");break;
		default:break;
	}
	strcat(stringP,"\r\n");
	TracePrintf(NM_TRACE, TRACE_NONMASK, stringP);
	pSleepM->prestate = pSleepM->state;
}
/************************************************************************************************/
/*
 * Brief               Main function of the NdsCanSm which processes the algorithm.
 * ServiceId           0x00
 * Sync/Async          None
 * Reentrancy          None
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Lower layer
 */
/************************************************************************************************/
void SleepM_MainFunction( const SleepManagementType smChannel )
{
	SleepMInnerChannelType *pSleepM = NULL;
	
	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	
		/* 1st:cansm init finished*/
		if( TRUE == pSleepM->initFlag )
		{
			/* 2nd:cansm state management*/
			#if ( SLEEPM_COMPLETE == SLEEPM_STATECHARTMODEL )
				SleepM_CompleteModeManage(pSleepM);
			#endif
			
			#if ( SLEEPM_SIMPLIFIED == SLEEPM_STATECHARTMODEL )
				SleepM_SimplifiedModeManage(pSleepM);
			#endif

			#if ( SLEEPM_EXCEPTIONAL == SLEEPM_STATECHARTMODEL )
				SleepM_ExceptionalModeManage(pSleepM);
			#endif

			/* 3rd:cansm state printf */
			#if( STD_ON == SLEEP_MANAGEMENT_DEBUG_TRACE )
				SleepM_StateChangePrintf(pSleepM);
			#endif
		}
	}
}

#if (STD_ON != MAIN_SM_STATE_USE_TIMER)
uint8 SleepM_WakeUpAtPowerOn(const SleepManagementType smChannel)
{
	boolean retVal = E_NOT_OK;
	uint16 i = 1 ,loc_wakeup_count = 0 ,wait_nm_max_time = 100;//100ms wait nm frame from can interruput
	SleepMInnerChannelType *pSleepM = NULL;
	uint8 msaterWakeUpCmd = 0 ;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
		pSleepM = &sSleepM[smChannel];
	else
		return retVal;

	pSleepM->masterWakeUpSleepCmd = M_GoToSleep;
	pSleepM->slaveRefuseToSleep = S_ReadyToSleep;
	pSleepM->ecuFramesFlag = FALSE;

	//judge sys wake up by what:nm frame or loc_wake_up(ign or acc)
	for( i = 0; i < wait_nm_max_time ; i++ )
	{
		DelayMs(1);
		//if ecu frame in ,if timer > s1(200),send nm frame to CAN BUS
		if( TRUE == pSleepM->ecuFramesFlag )
			retVal = E_OK;

		//can nm frame arrived
		if( gSleepM_WakeUp_Flag )
		{
			retVal = E_OK;
			pSleepM->masterWakeUpSleepCmd = M_WAKEUP;
			pSleepM->ecuFramesFlag = TRUE;
			break;
		}
		
		//check ign or acc
		if(( BINSIG_IGN_ON ==  BINSIG_IGN_GET_PORT_VALUE )
			|| ( BINSIG_ACC_ON ==  BINSIG_ACC_GET_PORT_VALUE ))
		{
			loc_wakeup_count++;
			if( loc_wakeup_count > 10 )
			{
				retVal = E_OK;
				pSleepM->slaveRefuseToSleep = S_RefuseToSleep;
				SleepM_SendWakeUpFrame(pSleepM);
				break;
			}
		}
	}

	if( E_OK == retVal )
		CanNm_SendRefuseToSleepFrame(pSleepM->slaveRefuseToSleep);
	
	return retVal;
}
#else
uint8 SleepM_WakeUpAtPowerOn(const SleepManagementType smChannel)
{
	boolean retVal = E_NOT_OK;
	uint8 timer_diff = 10;
	static uint16 WakeUp_timer = 0,APPLICATION_timer=0;
	static uint8 s_WakeUp_timer_en =FALSE,s_APPLICATION_timer_en =FALSE;
	static uint16 s_loc_wakeup_count = 0;
	SleepMInnerChannelType *pSleepM = NULL;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];;
		retVal = E_OK;
	}
	else
	{
		return retVal;
	}

	//SM timer count
	if(s_WakeUp_timer_en)
		WakeUp_timer +=timer_diff;
	if(s_APPLICATION_timer_en)
		APPLICATION_timer +=timer_diff;

	//ign and acc judge
	if(( BINSIG_IGN_ON ==  BINSIG_IGN_GET_PORT_VALUE )
		|| ( BINSIG_ACC_ON ==  BINSIG_ACC_GET_PORT_VALUE ))
	{
		s_loc_wakeup_count++;
		if(s_loc_wakeup_count == 2)//3*timer_diff ms filter
		{
			pSleepM->slaveRefuseToSleep = S_RefuseToSleep;
		}
	}
	else
	{
		if(s_loc_wakeup_count > 0)
		{
			s_loc_wakeup_count--;
		}
		if(s_loc_wakeup_count == 0)
		{
			pSleepM->slaveRefuseToSleep = S_ReadyToSleep;
		}
	}

	//SM state jump
	switch(pSleepM->state)
	{
		case HARDWARE_RESET:
		{
			s_WakeUp_timer_en = TRUE;
			pSleepM->state = WAKE_UP_TRANSIENT;
		}break;
		case SLEEP:
		{
			if(TRUE == pSleepM->interruptFlag)
			{
				s_WakeUp_timer_en = TRUE;
				pSleepM->state = WAKE_UP_TRANSIENT;
			}
			else if(S_RefuseToSleep==pSleepM->slaveRefuseToSleep)
			{
				SleepM_SendWakeUpFrame(pSleepM);
				s_APPLICATION_timer_en = TRUE;
				pSleepM->state = STARTING_CAN_WAKE_UP;
			}

		}break;
		case WAKE_UP_TRANSIENT:
		{
			if(S_RefuseToSleep==pSleepM->slaveRefuseToSleep)
			{
				SleepM_SendWakeUpFrame(pSleepM);
				s_APPLICATION_timer_en = TRUE;
				pSleepM->state = STARTING_CAN_WAKE_UP;
			}
			else if(S_ReadyToSleep==pSleepM->slaveRefuseToSleep)
			{
				if((M_WAKEUP==pSleepM->masterWakeUpSleepCmd)
					|| ((WakeUp_timer>=S1_WAKEUPTIME) && ( TRUE == pSleepM->ecuFramesFlag )))
				{
					CanNm_SendRefuseToSleepFrame(pSleepM->slaveRefuseToSleep);
					pSleepM->state = APPLICATION;
				}
				else if((WakeUp_timer>=S1_WAKEUPTIME) && ( FALSE == pSleepM->ecuFramesFlag ))
				{
					pSleepM->state = SLEEP;
				}
			}

		}break;
		case STARTING_CAN_WAKE_UP:
		{
			if(APPLICATION_timer>=S2_APPLICATIONTIME)
			{
				CanNm_SendRefuseToSleepFrame(pSleepM->slaveRefuseToSleep);
				pSleepM->state = APPLICATION;
			}

		}break;
		case APPLICATION:{}break;
		case SLEEP_TRANSIENT:{}break;
		case STARTING_GO_TO_SLEEP:{}break;
		default:break;
	}

	return retVal;
}
#endif
void IC_WakeUpFrameCtrl()
{
	static uint8 sendOnce = TRUE;

	//used to solve at system wakeup 0x60d or 0x351 some single arrive ,sys need send 0x602 MSG ,add by tjy @2019-12-25
	if(( System_GetTickValue() < 500 ) && ( TRUE == sendOnce ) && ( TRUE == gSleepM_WakeUp_Flag2 ))
	{
		sendOnce = FALSE;
		//
		CanNm_SendWakeUpFrame();
		
		TracePrintf(NM_TRACE, TRACE_ERRO, "[NM-I]:0x60d or 0x351 some single arrive send 0x602 MSG\n\r");
	}
}

