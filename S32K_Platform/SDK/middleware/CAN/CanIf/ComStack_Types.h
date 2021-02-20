/********************************************************************
File name: ComStack_Types.h
Author: Stephen Du
Version: V1.0
Timestamp: 2017-02-10 09:54:28
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef COMSTACK_TYPES_H
#define COMSTACK_TYPES_H

/*===============================================================================*/
/*=========================== START: HEAD FILE INCLUDE ==========================*/
/*===============================================================================*/
#include "Std_Types.h"

/*===============================================================================*/
/*============================ END: HEAD FILE INCLUDE ===========================*/
/*===============================================================================*/

/*===============================================================================*/
/*============================== START: TYPE DEFINE =============================*/
/*===============================================================================*/
/*The size of this global type depends on the maximum number of PDUs used within one software module.*/

typedef uint16 PduIdType;

/*The size of this global type depends on the maximum length of PDUs to be sent by an ECU.*/
typedef uint16 PduLengthType;

typedef uint8 *SduDataPtrType;

typedef struct
{
    SduDataPtrType SduDataPtr;
    PduLengthType SduLength;
} PduInfoType;

typedef enum
{
    BUFREQ_OK,           /*Buffer request accomplished successful.*/
    BUFREQ_E_NOT_OK,     /*Buffer request not successful. Buffer cannot be accessed.*/
    BUFREQ_E_BUSY,       /*Temporarily no buffer available. It's up the requestor to retry request for a certain time.*/
    BUFREQ_E_OVFL        /*No Buffer of the required length can be provided.*/
} BufReq_ReturnType;
/*===============================================================================*/
/*=============================== END: TYPE DEFINE ==============================*/
/*===============================================================================*/
/* @req COMTYPE013 */
/* 
 * Variables of this type shall be used to store result of a notification
 * (confirmation or indication). Currently this type is only used for 
 * communication between DCM and TP to enable the notification that an error 
 * has occurred and a dedicated buffer can be unlocked.
 */
typedef uint8 NotifResultType;

/* @req COMTYPE020 */
/* 
 * Variables of this type shall be used to return the bus status evaluated 
 * by a transceiver.
 */
typedef uint8 BusTrcvErrorType;

/* @req COMTYPE026 */
/* 
 * Variables of the type NetworkHandleType shall be used to store the 
 * identifier of a communication channel. 
 */
typedef uint8 NetworkHandleType;

/* @req SWS_COMTYPE_00027 */
/* Variables of this type shall be used to store the state of TP buffer.
 */
typedef enum
{
	TP_DATACONF,
	TP_DATARETRY,
	TP_CONFPENDING
}TpDataStateType;

/* @req SWS_COMTYPE_00037 */
/* Variables of this type shall be used to store the information about
 * Tp buffer handling.
 */
typedef struct
{
	TpDataStateType TpDataState;
	PduLengthType TxTpDataCnt;
}RetryInfoType;

typedef enum
{
    COM_SIG_MOTOR_BIG_ENDIAN = 0,
    COM_SIG_INTEL_LITTLE_ENDIAN,
    COM_SIG_OPAQUE
} Com_SignalEndianessEType;

typedef enum
{
    COM_SIG_BOOLEAN = 0,
    COM_SIG_SINT16,
    COM_SIG_SINT32,
    COM_SIG_SINT8,
    COM_SIG_UINT16,
    COM_SIG_UINT32,
    COM_SIG_UINT8,
    COM_SIG_UINT8_N
} Com_SignalEType;


typedef enum
{
    COM_SIG_TX_PENDING = 0,
    COM_SIG_TX_TRIGGERED
} Com_TransferPropertyEType;

typedef enum
{
    //For uint8[n] type, only Always and never support
    COM_F_ALWAYS = 0,
    COM_F_NEVER,
    COM_F_MASKED_NEW_EQUALS_X,
    COM_F_MASKED_NEW_DIFFERS_X,
    COM_F_MASKED_NEW_EQUALS_MASKED_OLD,
    COM_F_MASKED_NEW_DIFFERS_MASKED_OLD,
    COM_F_NEW_IS_WITHIN,  //Not suit for boolean type
    COM_F_NEW_IS_OUTSIDE, //Not suit for boolean type
    COM_F_NEW_IS_GREATER,
    COM_F_NEW_IS_LESSOREQUAL,
    COM_F_NEW_IS_LESS,
    COM_F_NEW_IS_GREATEROREQUAL,
    COM_F_ONE_EVERY_N
} Com_FilterAlgorithmEType;

typedef struct
{
    /*Multiplicity: 1*/
    Com_FilterAlgorithmEType Algorithm;

    /*Multiplicity:0..1
    Range:-2147483648..2147483647*/
    sint32 Mask;

    /*Multiplicity:0..1
    Range:-2147483648..2147483647*/
    sint32 Max;

    /*Multiplicity:0..1
    Range:-2147483648..2147483647*/
    sint32 Min;

    /*Multiplictiy:0..1
    Range = 0..(ComFilterPeriod-1) */
    uint32 Offset;

    /*This parameter defines the period of the ComFilterAlgorithm ONE_EVERY_N.
    Multiplicity:0..1
    Range:2..4294967295*/
    uint32 Period;

    /*Multiplicity:0..1
    Range:-2147483648..2147483647*/
    sint32 FilterX;
} Com_FilterType;

typedef struct
{
    /*Multiplicity: 1*/
    uint8 StartBit;
    /*Multiplicity: 1*/
    uint8 Length;
    /*Multiplicity: 1*/
    Com_SignalEndianessEType EndianType;  
    /*Multiplicity: 1*/
    Com_SignalEType SignalType;
    /*Multiplicity: 1*/
    uint8 TypeUint8NBytes;
    /*Multiplicity: 0..1*/
    /*
    COM_SIG_TX_PENDING
    COM_SIG_TX_TRIGGERED
    */
    Com_TransferPropertyEType TransferProperty;
    
    /*On sender side the container is used to specify the transmission mode conditions.
        Multiplicity: 0..1*/
    Com_FilterType Filter;
} Com_SigConfType;

typedef struct
{
    uint16 Occurence;
} Com_RunFilterType;

typedef struct
{
    Com_RunFilterType Filter;
} Com_SigRunType;

typedef uint16 Com_SignalIdType;
typedef uint16 Com_SignalGroupIdType;
typedef uint16 Com_IpduGroupIdType;

#define COM_GET_SIG_VALUE 0
#define COM_SET_SIG_VALUE 1

#endif /* end of COMSTACK_TYPES_H */

/*=======[E N D   O F   F I L E]==============================================*/

