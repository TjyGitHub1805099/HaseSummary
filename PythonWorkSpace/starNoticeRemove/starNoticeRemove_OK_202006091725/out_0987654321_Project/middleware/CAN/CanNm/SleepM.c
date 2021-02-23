




 

  



#define SLEEPM_C_AR_MAJOR_VERSION   5U
#define SLEEPM_C_AR_MINOR_VERSION   2U
#define SLEEPM_C_AR_PATCH_VERSION   0U

#define SLEEPM_C_SW_MAJOR_VERSION   1U
#define SLEEPM_C_SW_MINOR_VERSION   0U
#define SLEEPM_C_SW_PATCH_VERSION   0U




#include "Platform_Types.h"
#include "System_Tick.h"
#include "trace.h"
#include "SleepM.h"
#include "System_Signal.h"
#include "Can_Nm.h"
#include "Com_If.h"
#include "BinSig_Ign_Cfg.h"
#include "BinSig_Acc_Cfg.h"
#include "tpConfig.h"
#include "Delay.h"
#include "main.h"



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





#define SLEEP_MANAGEMENT_INSTANCE	1




SleepMInnerChannelType sSleepM[SLEEP_MANAGEMENT_INSTANCE];

volatile uint8 gSleepM_WakeUp_Flag = FALSE ,gSleepM_WakeUp_Flag2 = FALSE;








void AppSoftWare_SleepConditonSet( const SleepManagementType smChannel , SleepMSleepConditionType sleepConditon );


SleepMChartStateType AppSoftWare_CurrChartStateGet( const SleepManagementType smChannel );




#if 0

static void SleepM_Memcpy(uint8 *  dest, const uint8 *  source, uint8 length );


static void SleepM_Memset(uint8 *  dest, uint8 value, uint8 length );
#endif




static void SleepM_StartTimer( TimerOutType *  pTimer , uint32 timerTick);
#if 0

static void SleepM_StopTimer( TimerOutType *  pTimer );
#endif

static TimerReturnType SleepM_CheckTimer( TimerOutType *  timeOutTimer );








#if 0



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
#endif



static void SleepM_StartTimer( TimerOutType *  pTimer , uint32 remainTime)
{
	pTimer->start = TRUE;
	(void)GetCounterValue(0, &pTimer->startTime);
	pTimer->remainTime = remainTime ;
}
#if 0



static void SleepM_StopTimer( TimerOutType *  pTimer )
{
	pTimer->start = FALSE;
	pTimer->remainTime = 0u;
}
#endif



static TimerReturnType SleepM_CheckTimer( TimerOutType *  timeOutTimer )
{
	TickType elapsedTick;
	TimerReturnType returnVlu = TIMER_ARRIVE;

	if ( TRUE == timeOutTimer->start )
	{
		returnVlu = TIMER_EXECUTED;
		
		GetElapsedCounterValue(0,&timeOutTimer->startTime,&elapsedTick);

		
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

		
		if( returnVlu == TIMER_ARRIVE )
		{
			
			timeOutTimer->remainTime = 0x0u;
		}
	}

	return returnVlu;
}



static void SleepM_CanInit(SleepMInnerChannelType *  innerChannel )
{
	#if ( SLEEPM_SLAVE == SLEEPM_PROPERTY )
		
		innerChannel->slaveRefuseToSleep = S_ReadyToSleep;
		
		
	#else
	#endif
}



static TimerReturnType SleepM_SelfDiag(SleepMInnerChannelType *  innerChannel )
{
	TimerReturnType selfDiagCplt = TIMER_ARRIVE;
	
	return  selfDiagCplt;
}



static void SleepM_SendWakeUpFrame(SleepMInnerChannelType *  innerChannel )
{
	CanNm_SendWakeUpFrame();	
}



static void SleepM_SendRefuseToSleepFrame(SleepMInnerChannelType *  innerChannel )
{
	CanNm_SendRefuseToSleepFrame(innerChannel->slaveRefuseToSleep);
}



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



void AppSoftWare_SleepConditonSet( const SleepManagementType smChannel , SleepMSleepConditionType sleepConditon )
{
	SleepMInnerChannelType *pSleepM = NULL;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	}

	pSleepM->sleepCondition = sleepConditon ;
}



SleepMChartStateType AppSoftWare_CurrChartStateGet( const SleepManagementType smChannel )
{
	SleepMInnerChannelType *pSleepM = NULL;

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	}

	return pSleepM->state;
}



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




#if ( SLEEPM_SLAVE == SLEEPM_PROPERTY )

#if ( SLEEPM_COMPLETE == SLEEPM_STATECHARTMODEL )
void SleepM_CompleteModeManage(SleepMInnerChannelType *pSleepM)
{
	switch( pSleepM->state )
	{
		case HARDWARE_RESET :
		{
			pSleepM->emissionEn = Emission_OFF ;
			
			
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
		
			
			if(( SC_AcceptSleep == pSleepM->sleepCondition )
			 &&( FALSE == pSleepM->interruptFlag ))
			{
				 pSleepM->state = SLEEP;
				 
				 break;
			}
			 
			
			if( TRUE == pSleepM->interruptFlag )
			{
				pSleepM->interruptFlag = FALSE ;
				pSleepM->ecuFramesFlag = FALSE ;
							
				SleepM_CanInit(pSleepM);
				
				SleepM_StartTimer(&pSleepM->wakeUpTimer,S1_WAKEUPTIME);
				 
				pSleepM->state = WAKE_UP_TRANSIENT;
				
				break; 
			}
			
			
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

			
			if( SC_WakeUp == pSleepM->sleepCondition )
			{
				SleepM_SendWakeUpFrame(pSleepM);

				SleepM_StartTimer(&pSleepM->applicationTimer,S2_APPLICATIONTIME);
				 
				pSleepM->state = STARTING_CAN_WAKE_UP;

				break; 
			}
			
			
			if(( SC_AcceptSleep == pSleepM->sleepCondition ) &&
				(( M_WAKEUP == pSleepM->masterWakeUpSleepCmd ) ||(( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->wakeUpTimer) )&&( TRUE == pSleepM->ecuFramesFlag ))) )
			 {
			 	pSleepM->slaveRefuseToSleep = S_ReadyToSleep ;
				SleepM_SendRefuseToSleepFrame(pSleepM);

				SleepM_StartTimer(&pSleepM->cycleTimer,SA_CYCLETIME);

				System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_ON, STD_OFF);

				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);
				#else
				Com_TxStart();
				#endif
				pSleepM->state = APPLICATION;

				break; 
			 }
			
			
			if(( SC_AcceptSleep == pSleepM->sleepCondition ) &&
				( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->wakeUpTimer) ) &&( FALSE == pSleepM->ecuFramesFlag ))
			 {
				System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_OFF, STD_OFF);
				
				pSleepM->ecuFramesFlag = FALSE ;
				pSleepM->interruptFlag = FALSE ;
				
				pSleepM->state = SLEEP;
				
				break; 
			 }
		
		}break;
		case STARTING_CAN_WAKE_UP :
		{
			pSleepM->emissionEn = Emission_OFF ;

			
			if( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->applicationTimer) )
			{
				pSleepM->slaveRefuseToSleep = S_RefuseToSleep ;
				SleepM_SendRefuseToSleepFrame(pSleepM);
				
				SleepM_StartTimer(&pSleepM->cycleTimer,SA_CYCLETIME);
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);
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

			
			if(( ( M_GoToSleep == pSleepM->masterWakeUpSleepCmd ) && ( SC_AcceptSleep == pSleepM->sleepCondition ) ))
			{
				SleepM_StartTimer(&pSleepM->sleepTimer,S4_SLEEPTIME);
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_OFF, STD_OFF);
				#else
				Com_TxStop();
				#endif
				pSleepM->state = SLEEP_TRANSIENT;

				break;
			}

			
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

			
			if( ( M_WAKEUP == pSleepM->masterWakeUpSleepCmd ) || ( SC_WakeUp == pSleepM->sleepCondition ) )
			{
				pSleepM->slaveRefuseToSleep = (SC_WakeUp == pSleepM->sleepCondition)?S_RefuseToSleep:S_ReadyToSleep;
				SleepM_SendRefuseToSleepFrame(pSleepM);
				
				pSleepM->state = APPLICATION;
				#if 0
				System_Signal_Send(SYSTEM_SIG_ID_CAN_SEND_ENABLE, STD_ON, STD_OFF);
				#else
				Com_TxStart();
				#endif
				break;
			}

			
			if( TRUE == pSleepM->ecuFramesFlag )
			{	
				pSleepM->ecuFramesFlag = FALSE;
				
				SleepM_StartTimer(&pSleepM->sleepTimer,S4_SLEEPTIME);
				
				pSleepM->state = SLEEP_TRANSIENT;
				
				break;
			}

			
			if( ( M_WAKEUP != pSleepM->masterWakeUpSleepCmd ) && ( M_GoToSleep != pSleepM->masterWakeUpSleepCmd ) )
			{
				pSleepM->masterWakeUpSleepCmd = M_GoToSleep;
			}

			
			if( ( SC_AcceptSleep == pSleepM->sleepCondition )
				&& ( M_GoToSleep == pSleepM->masterWakeUpSleepCmd )
				&& ( TIMER_ARRIVE == SleepM_CheckTimer(&pSleepM->sleepTimer) ))
			{
				if( TIMER_ARRIVE == SleepM_SelfDiag(pSleepM))
				{
					System_Signal_Send(SYSTEM_SIG_ID_CAN_AWAKE, STD_OFF, STD_OFF);

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



void SleepM_MainFunction( const SleepManagementType smChannel )
{
	SleepMInnerChannelType *pSleepM = NULL;
	
	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
	{
		pSleepM = &sSleepM[smChannel];
	
		
		if( TRUE == pSleepM->initFlag )
		{
			
			#if ( SLEEPM_COMPLETE == SLEEPM_STATECHARTMODEL )
				SleepM_CompleteModeManage(pSleepM);
			#endif
			
			#if ( SLEEPM_SIMPLIFIED == SLEEPM_STATECHARTMODEL )
				SleepM_SimplifiedModeManage(pSleepM);
			#endif

			#if ( SLEEPM_EXCEPTIONAL == SLEEPM_STATECHARTMODEL )
				SleepM_ExceptionalModeManage(pSleepM);
			#endif

			
			#if( STD_ON == SLEEP_MANAGEMENT_DEBUG_TRACE )
				SleepM_StateChangePrintf(pSleepM);
			#endif
		}
	}
}

uint8 SleepM_WakeUpAtPowerOn(const SleepManagementType smChannel)
{
	boolean retVal = E_NOT_OK;
	uint16 i = 1 ,loc_wakeup_count = 0 ,wait_nm_max_time = 100;
	SleepMInnerChannelType *pSleepM = NULL;
	

	if( smChannel < SLEEP_MANAGEMENT_INSTANCE )
		pSleepM = &sSleepM[smChannel];
	else
		return retVal;

	pSleepM->masterWakeUpSleepCmd = M_GoToSleep;
	pSleepM->slaveRefuseToSleep = S_ReadyToSleep;
	pSleepM->ecuFramesFlag = FALSE;

	
	for( i = 0; i < wait_nm_max_time ; i++ )
	{
		DelayMs(1);
		
		if( TRUE == pSleepM->ecuFramesFlag )
			retVal = E_OK;

		
		if( gSleepM_WakeUp_Flag )
		{
			retVal = E_OK;
			pSleepM->masterWakeUpSleepCmd = M_WAKEUP;
			pSleepM->ecuFramesFlag = TRUE;
			break;
		}
		
		
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

void IC_WakeUpFrameCtrl()
{
	static uint8 sendOnce = TRUE;

	
	if(( System_GetTickValue() < 500 ) && ( TRUE == sendOnce ) && ( TRUE == gSleepM_WakeUp_Flag2 ))
	{
		sendOnce = FALSE;
		
		CanNm_SendWakeUpFrame();
		
		TracePrintf(NM_TRACE, TRACE_ERRO, "[NM-I]:0x60d or 0x351 some single arrive send 0x602 MSG\n\r");
	}
}

