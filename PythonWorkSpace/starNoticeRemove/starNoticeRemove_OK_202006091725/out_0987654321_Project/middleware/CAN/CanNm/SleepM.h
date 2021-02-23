#ifndef SLEEPMANAGEMENT_H
#define SLEEPMANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*=============[I N T E R F A C E  W I T H  A P P L I C A T I O N  S O F T W A R E]=============*/
/************************************************************************************************/
/*
*							 - - - - - - - - - - - - - - - - - - - - - - - - - -
*							|				APPLICATION SOFTWARE				 |
*							 - - - - - - - - - - - - - - - - - - - - - - - - - - 
*								|											^
*	Master/Slave_SleepCondition	|		   	 - - - - - - - - - -			|	Sleep Management statechar's state
*								|- - - - - >| SLEPP MANAGEMENT	|- - - - - -|
*											 - - - - - - - - - -
*/

/*============================[D E S C R I P T I O N]===========================================*/
/************************************************************************************************/
/*
*	state/modle			COMPLETE(master | slave)	SIMPLIFIED(master | slave)	EXCEPTIONAL(master | slave)
*	HARDWARE_RESET			 	 	o   |   o				  	  o   |   o					  x    |   o
*	------------------------------------------------------------------------------------------------------
*	SLEEP					 	 	o   |   o				  	  o   |   o		  			  x    |   o
*	------------------------------------------------------------------------------------------------------
*	WAKE_UP_TRANSIENT		 	 	o   |   o				  	  x   |   o		 			  x    |   o
*	------------------------------------------------------------------------------------------------------
*	STARTING_CAN_WAKE_UP	 	 	o   |   o			      	  o   |   x		 			  x    |   x
*	------------------------------------------------------------------------------------------------------
*	APPLICATION		   		 	 	o   |   o				  	  o   |   o		 			  x    |   o
*	------------------------------------------------------------------------------------------------------
*	SLEEP_TRANSIENT		    	 	x   |   o			      	  x   |   o		 			  x    |   o
*	------------------------------------------------------------------------------------------------------
*	STARTING_GO_TO_SLEEP	 	 	o   |   x				  	  o   |   x		 			  x    |   x
*	------------------------------------------------------------------------------------------------------
*	
*	cmd								MasterWakeUpSleepCmd		  slaveRefuseToSleep
*	-----------------------------------------------------------------------------------
*	M_GoToSleep						o							  x
*	-----------------------------------------------------------------------------------
*	S_RefuseToSleep					x							  o
*	-----------------------------------------------------------------------------------
*	S_ReadyToSleep					x							  o
*	-----------------------------------------------------------------------------------
*	M_WAKEUP						o							  x
*	-----------------------------------------------------------------------------------
*/

/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

//nds reference file : E_25953NDS05_2
#define NDS_CLUSTER_ID				25953U

#define SLEEPM_H_AR_MAJOR_VERSION   5U
#define SLEEPM_H_AR_MINOR_VERSION   2U
#define SLEEPM_H_AR_PATCH_VERSION   0U

#define SLEEPM_H_SW_MAJOR_VERSION   1U
#define SLEEPM_H_SW_MINOR_VERSION   0U
#define SLEEPM_H_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "SleepM_Cfg.h"
#include "Std_Types.h"
/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/
/* 	Define property of ECU
*	This is one of the ECU on the network that co-ordinates sleep sequence of all ECU that need sleeping.
*	all the active nodes except the Master.
*/
#define SLEEPM_SLAVE 	0
#define SLEEPM_MASTER 	1
#define SLEEPM_PROPERTY	SLEEPM_SLAVE	/* ECU Property select as SLAVE */

/* @[E_25953NDS05_2]-4.7 */
/* 	Define state chart modle of ECU
*	Master implements the 鈥淐omplete鈥� model when at least one Slave can wake up on its own and start a wakeup sequence.
*	Otherwise (i.e. no Slave can wake up on its own and start a wake up sequence), Master implements the鈥淪implified鈥� model.

*	Slave implements the 鈥淐omplete鈥� model when it can wake up on its own and start a wake up sequence.
*	Otherwise (i.e. Slave can only be woken up by (an) other active node(s)), Slave implements the 鈥淪implified鈥�	model.
*	Additionally, if such a Slave does not emit anything on CAN except Diagnostic messages, it can implement the 鈥淓xceptional鈥� model.
*/
#define SLEEPM_COMPLETE		0
#define SLEEPM_SIMPLIFIED	1
#define SLEEPM_EXCEPTIONAL	2
#define SLEEPM_STATECHARTMODEL	SLEEPM_COMPLETE	/* ECU state chart modle select as COMPLETE */

/* time define */
#if ( SLEEPM_PROPERTY == SLEEPM_MASTER )
	/* @[E_25953NDS05_2]-4.6.1 */
	#define M1_WAKEUPTIME					60		/* WakeUp_timeout (ms) */
	#define M2_APPLICATIONTIME				50		/* APPLICATION_timeout (ms) */
	#define M3_SENDSLEEPTIME				1000	/* Send_Sleep_timeout (ms) */
	#define M4_SLEEPTIME					2000	/* SLEEP_timeout (ms) */

#else
	/* @[E_25953NDS05_2]-4.6.2 */
	#define S1_WAKEUPTIME					200		/* WakeUp_timeout (ms) */
	#define S2_APPLICATIONTIME				50		/* APPLICATION_timeout (ms) */
	#define S4_SLEEPTIME					1000	/* SLEEP_timeout (ms) */
	
	#define SA_CYCLETIME					100
	#define SA_NO_ORDER_FROM_MASTER			2000
	#define SA_WRONG_ORDER_FROM_MASTER 		2000
#endif

#define SLEEPM_TIMER_TOLERANCE				2		/* ECU timer tolerace : ms */

/* NDS sleep management pdu data length*/
#define SLEEPM_PDU_LEN						8

/* @[5.591MC_CAN_ID.xlsx]-line150:CMD_MASTER_WAKEUPSLEEP Bit Fields*/
#define CMD_MASTER_WAKEUPSLEEP_BYTE_POS		0x01u/* position:byte1 */
#define CMD_MASTER_WAKEUPSLEEP_MASK         0x03u/* datalen:bit1~bit0 */
#define CMD_MASTER_WAKEUPSLEEP_SHIFT        0u
#define CMD_MASTER_WAKEUPSLEEP_WIDTH        2u
#define CMD_MASTER_WAKEUPSLEEP_DATA(x)      (((uint8_t)(((uint8_t)(x))<<CMD_MASTER_WAKEUPSLEEP_SHIFT))&CMD_MASTER_WAKEUPSLEEP_MASK)

/* @[5.591MC_CAN_ID.xlsx]-line102:CMD_SLAVE_REFUSETOSLEEP Bit Fields*/
#define CMD_SLAVE_REFUSETOSLEEP_BYTE_POS	0x00u/* position:byte0 */
#define CMD_SLAVE_REFUSETOSLEEP_MASK        0xC0u/* datalen:bit7~bit6 */
#define CMD_SLAVE_REFUSETOSLEEP_SHIFT       6u
#define CMD_SLAVE_REFUSETOSLEEP_WIDTH       2u
#define CMD_SLAVE_REFUSETOSLEEP_DATA(x)     (((uint8_t)(((uint8_t)(x))<<CMD_SLAVE_REFUSETOSLEEP_SHIFT))&CMD_SLAVE_REFUSETOSLEEP_MASK)

#define SLEEPM_OS_COUNTER_ID 0

/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/

/* 32 bit data type define */
typedef uint32 TickType;
typedef uint8  SleepManagementType;

/************************************************************************************************/

typedef enum
{
	TIMER_ARRIVE = 0x0u,
	TIMER_EXECUTED = 0x1u,
} TimerReturnType;

typedef struct
{
	boolean  start;
	uint32 remainTime;
	uint32 startTime;
} TimerOutType;

/************************************************************************************************/
/* 	@[E_25953NDS05_2]-4.3.2:Define SleepCondition of ECU 
*	The conditions for each value to be taken are to be specified in ECU Application software specific requirements.
*/
typedef enum
{
	SC_AcceptSleep = 0x0u,
	SC_WakeUp = 0x1u,
} SleepMSleepConditionType;

/************************************************************************************************/
/* 	@[E_25953NDS05_2]-4.4.4 \ 4.4.5:Define MasterWakeUpSleepCmd and slaveRefuseToSleep of ECU */
typedef enum
{
	M_GoToSleep = 0x0u,/* used in MasterWakeUpSleepCmd */
	M_WAKEUP = 0x3u,/* used in MasterWakeUpSleepCmd */
} SleepMMasterSleepCmdType;

typedef enum
{
	S_RefuseToSleep = 0x1u,/* used in slaveRefuseToSleep */
	S_ReadyToSleep = 0x2u,/* used in slaveRefuseToSleep */
} SleepMSlaveSleepCmdType;

/************************************************************************************************/
/* 	@[E_25953NDS05_2]-4.7:Define state chart modle state of ECU */
typedef enum
{
	HARDWARE_RESET = 0x0u,
	SLEEP = 0x1u,
	WAKE_UP_TRANSIENT = 0x2u,
	STARTING_CAN_WAKE_UP = 0x3u,
	APPLICATION = 0x4u,
	SLEEP_TRANSIENT = 0x5u,
	STARTING_GO_TO_SLEEP = 0x6u,
} SleepMChartStateType;

/************************************************************************************************/
/* 	@[E_25953NDS05_2]-4.4.2:Define emission of ECU 
*	OFF:Message Set ([REF 2]) frames transmission is NOT allowed.
*		Frame transmission is authorized only when it is required by this
*		specification (i.e. the concerned statechart).
*		Consequently, only the WakeUp Frame and the CAN frame
*		containing the Sleep Management related parameter (see 鎼�4.4.4.for
*		Master or 鎼�4.4.5. for Slave) can be transmitted
*	ON: Message Set ([REF 2]) frames transmission is allowed.
*		At least the CAN frame containing the Sleep Management related
*		parameter (see 鎼�4.4.4.for Master or 鎼�4.4.5. for Slave) shall be
*/
typedef enum
{
	Emission_ON = 0x0u,
	Emission_OFF = 0x1u,
} SleepMEmissionType;

/************************************************************************************************/
/* NDS_CanSleepManagement configuration */
typedef struct
{
	SleepMSleepConditionType	sleepCondition;/* from application software */
	
	SleepMMasterSleepCmdType	masterWakeUpSleepCmd;/* master send or slave receve the WakeUpSleep cmd*/
	SleepMSlaveSleepCmdType		slaveRefuseToSleep;/* slave send or master receve the RefuseToSleep cmd*/

	TimerOutType	wakeUpTimer;
	TimerOutType	applicationTimer;
	TimerOutType	sleepTimer;
	
	TimerOutType	cycleTimer;
	TimerOutType	noOrderFromMasterTimer;
	TimerOutType 	wrongOrderFromMasterTimer;
		
	#if ( SLEEPM_MASTER == SLEEPM_PROPERTY )
		SleepMTimerOutTimerType	sendSleepTimer;
	#endif
		
	SleepMEmissionType		emissionEn;/* enable related CAN message emission or not*/
	
	SleepMChartStateType	state;
	SleepMChartStateType	prestate;/* management inter state chart and return it to application software*/
	
	uint8 	rxData[SLEEPM_PDU_LEN];/* can receve data buf */
	uint8 	txData[SLEEPM_PDU_LEN];/* can transmit data buf */
	
	boolean  initFlag;
	boolean  interruptFlag;
	boolean  ecuFramesFlag;
	boolean  masterCmdFlag;
	boolean  wrongMasterCmdFlag;

} SleepMInnerChannelType;
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
void SleepM_Init();
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
void SleepM_MainFunction( const SleepManagementType smChannel );
uint8 SleepM_WakeUpAtPowerOn(const SleepManagementType smChannel);

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
SleepMChartStateType AppSoftWare_CurrChartStateGet( const SleepManagementType smChannel );
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
void AppSoftWare_SleepConditonSet( const SleepManagementType smChannel , SleepMSleepConditionType sleepConditon );
/************************************************************************************************/
/*
 * Brief			   SleepM Master Wake Up Sleep Cmd Get.
 * ServiceId		   0x00
 * Sync/Async		   None
 * Reentrancy		   None
 * Param-Name[in]	   None
 * Param-Name[out]	   None
 * Param-Name[in/out]  None
 * Return			   None
 * PreCondition 	   None
 * CallByAPI		   Lower layer
 */
/************************************************************************************************/
void SleepM_MasterWakeUpSleepCmdGet( const SleepManagementType smChannel ,uint8 masterWakeUpSleepCmd );
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
void SleepM_RxIndication( const SleepManagementType smChannel ,uint8 masterWakeUpSleepCmd );
void SleepM_EcuIndication( const SleepManagementType smChannel );

extern void IC_WakeUpFrameCtrl();
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* SLEEPMANAGEMENT_H */

