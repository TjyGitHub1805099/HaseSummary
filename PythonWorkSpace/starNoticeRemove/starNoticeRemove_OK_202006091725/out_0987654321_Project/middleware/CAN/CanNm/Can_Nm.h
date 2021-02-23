#ifndef CAN_NETWPRKMANAGEMENT_H
#define CAN_NETWPRKMANAGEMENT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*=============[I N T E R F A C E  W I T H  A P P L I C A T I O N  S O F T W A R E]=============*/
/************************************************************************************************/
#include "trace.h"

/*============================[D E S C R I P T I O N]===========================================*/
/************************************************************************************************/

/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

//nds reference file : E_25953NDS06_2
#define NDS_CLUSTER_ID				25953U

#define NM_H_AR_MAJOR_VERSION   5U
#define NM_H_AR_MINOR_VERSION   2U
#define NM_H_AR_PATCH_VERSION   0U

#define NM_H_SW_MAJOR_VERSION   1U
#define NM_H_SW_MINOR_VERSION   0U
#define NM_H_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "SleepM.h"

/*============================[M A C R O  D E F I N I T I O N]==================================*/
/************************************************************************************************/

/* @[E_25953NDS05_2]-4.4.6 */
/* 	sleep management parameters reception monitoring out this time no active reception
	should set masterWakeUpSleepCmd = M_GoToSleep,slaveRefuseToSleepCmd = S_ReadyToSleep */
#define JT_JUDGMENTTIMER_1					200
#define JT_JUDGMENTTIMER_2					2000
#define NM_TIMER_TOLERANCE					5		/* ECU timer tolerace : ms */
/*============================[T Y P E  D E F I N I T I O N]====================================*/
/************************************************************************************************/

typedef uint8  FaultAccountInstanceType;

typedef uint16 CanNmPduIdType;
typedef uint8  CanNmPduInstanceType;

/************************************************************************************************/
/* 	@[E_25953NDS06_2]
Table 9-2 : Relationship with Transmission status and fault status bit sent to diagnosis tool */
typedef enum
{
	NOT_MUTE = 0x0u,
	MUTE = 0x2u,
	CONFIRMED_MUTE = 0x3u,
} TransmissionStatusType;
/************************************************************************************************/
/* 	@[E_25953NDS06_2]
Table 9-3 : Relationship with Reception status and fault status bit sent to diagnosis tool */
typedef enum
{
	PRESENT = 0x0u,
	ABSENT = 0x2u,
	CONFIRMED_ABSENT = 0x3u,
} ReceptionStatusType;
/************************************************************************************************/
/* 	@[E_25953NDS06_2]
Table 9-4 : Relationship with BusOff status and fault status bit sent to diagnosis tool */
typedef enum
{
	BUS_ON = 0x0u,
	BUS_OFF = 0x2u,
	CONFIRMED_BUS_OFF = 0x3u,
} BusOffStatusType;
/************************************************************************************************/
/* NDS_StationManagement configuration */
typedef struct
{
	TransmissionStatusType	transmissionStatus;/* Transmission Status */
	ReceptionStatusType		receptionStatus;/* Reception Status */
	BusOffStatusType		busOffStatus;/* BusOff Status */
	
	TimerOutType txJT1,rxJT1;
	TimerOutType txJT2,rxJT2;
	
} FaultAccountingChannelType;

typedef struct
{
	CanNmPduIdType 		 canPduId;/* can pdu inner map */
	CanNmPduInstanceType canInstace;/* can pdu instance */
} CanNmPduType;
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
void CanNm_TestByTrace(tStrArrayPtr strArrayPtr, uint8 arrayNum);
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
void CanNm_Init();
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
void CanNm_MainFunction( const FaultAccountInstanceType canInstance );

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
void CanNm_RxIndication( uint8_t canInstance ,uint8 masterWakeUpSleepCmd );
void CanNm_EcuRxIndication( uint8_t canInstance );
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
void CanNm_TxConfirmation( uint8_t canInstance );
/************************************************************************************************/
/*
 * Brief			   This service confirms a can driver trasmit buss off processed
 * ServiceId		   0x00
 * Sync/Async		   Synchronous
 * Reentrancy		   Reentrant
 * Param-Name[in]	   canInstance:Identification of the CAN channel
 * Param-Name[out]	   None
 * Param-Name[in/out]  None
 * Return			   None
 * PreCondition 	   None
 * CallByAPI		   Lower layer
 */
/************************************************************************************************/
void CanNm_BusOffCbk( uint8_t canInstance );
/************************************************************************************************/
/*
 * Brief			   Can nm  Master Wake Up Sleep Cmd Get.
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
void CanNm_MasterWakeUpSleepCmdSet( const SleepManagementType smChannel ,uint8 masterWakeUpSleepCmd );
/************************************************************************************************/
/*
 * Brief			   Internal function, send out refusetosleep state frame.
 * ServiceId		   0x00
 * Sync/Async		   None
 * Reentrancy		   None
 * Param-Name[in]	   None
 * Param-Name[out]	   None
 * Param-Name[in/out]  None
 * Return			   None
 * PreCondition 	   None
 * CallByAPI		   SleepM_MainFunction
 */
/************************************************************************************************/
void CanNm_SendWakeUpFrame();
/************************************************************************************************/
/*
 * Brief			   Internal function, send out refusetosleep state frame.
 * ServiceId		   0x00
 * Sync/Async		   None
 * Reentrancy		   None
 * Param-Name[in]	   None
 * Param-Name[out]	   None
 * Param-Name[in/out]  None
 * Return			   None
 * PreCondition 	   None
 * CallByAPI		   SleepM_MainFunction
 */
/************************************************************************************************/
void CanNm_SendRefuseToSleepFrame(SleepMSlaveSleepCmdType refuseTosleep);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* CAN_NETWPRKMANAGEMENT_H */

