/********************************************************************
File name: Com.h
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
#ifndef COM_H
#define COM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*===============================================================================*/
/*=========================== START: HEAD FILE INCLUDE ==========================*/
/*===============================================================================*/
#include "Com_Cfg.h"
#include "Com_If.h"
#include "ComStack_Types.h" 

/*===============================================================================*/
/*============================= END: HEAD FILE INCLUDE ==========================*/
/*===============================================================================*/

/*===============================================================================*/
/*============================== START: MACRO DEFINE ============================*/
/*===============================================================================*/
#define COM_MODULE_ID  COM_TRACE //((uint8)50U)
#define Det_ReportError(a,b,c,d) //TracePrintValue(a,b,#c,d)

/******************************* COM ERROR CODE **********************************/
#define COM_SERVICE_NOT_AVAILABLE   ((uint8)0x80U)
#define COM_BUSY                    ((uint8)0x81U)
// #define COM_E_ID                    ((uint8)0x82U)
// #define COM_E_LENGTH                ((uint8)0x83U)
// #define COM_E_LIMIT                 ((uint8)0x84U)
// #define COM_E_NOMSG                 ((uint8)0x85U)
// #define COM_E_TIME                  ((uint8)0x86U)

/******************************* COM SERIVCE ID **********************************/
#define COMSERVICEID_SIGNALENDIANTRANSFER       ((uint8)0x1C)
#define COMSERVICEID_INVALIDATESIGNALGROUP      ((uint8)0x1B)
#define COMSERVICEID_MAINFUNCTIONROUTESIGNALS   ((uint8)0x1A)
#define COMSERVICEID_MAINFUNCTIONTX             ((uint8)0x19)
#define COMSERVICEID_MAINFUNCTIONRX             ((uint8)0x18)
#define COMSERVICEID_TRIGGERIPDUSEND            ((uint8)0x17)
#define COMSERVICEID_INVALIDATESHADOWSIGNAL     ((uint8)0x16)
#define COMSERVICEID_TXCONFIRMATION             ((uint8)0x15)
#define COMSERVICEID_RXINDICATION               ((uint8)0x14)
#define COMSERVICEID_TRIGGERTRANSMIT            ((uint8)0x13)
#define COMSERVICEID_ERRORGETSERVICEID          ((uint8)0x11)
#define COMSERVICEID_INVALIDATESIGNAL           ((uint8)0x10)
#define COMSERVICEID_RECEIVESHADOWSIGNAL        ((uint8)0x0F)
#define COMSERVICEID_RECEIVESIGNALGROUP         ((uint8)0x0E)
#define COMSERVICEID_SENDSIGNALGROUP            ((uint8)0x0D)
#define COMSERVICEID_UPDATESHADOWSIGNAL         ((uint8)0x0C)
#define COMSERVICEID_RECEIVESIGNAL              ((uint8)0x0B)
#define COMSERVICEID_SENDSIGNAL                 ((uint8)0x0A)
#define COMSERVICEID_GETVERSIONINFO             ((uint8)0x09)
#define COMSERVICEID_GETCONFIGURATIONID         ((uint8)0x08)
#define COMSERVICEID_GETSTATUS                  ((uint8)0x07)
#define COMSERVICEID_ENABLERECEPTIONDM          ((uint8)0x06)
#define COMSERVICEID_DISABLERECEPTIONDM         ((uint8)0x05)
#define COMSERVICEID_IPDUGROUPSTOP              ((uint8)0x04)
#define COMSERVICEID_IPDUGROUPSTART             ((uint8)0x03)
#define COMSERVICEID_DEINIT                     ((uint8)0x02)
#define COMSERVICEID_INIT                       ((uint8)0x01)

/*===============================================================================*/
/*============================= END: MACRO DEFINE ===============================*/
/*===============================================================================*/

/*===============================================================================*/
/*============================ START: TYPE DEFINE ===============================*/
/*===============================================================================*/
/************************** FUNCTION POINT TYPE DEFINE ***************************/
typedef void (*Type1FuncPtrType)(uint32 data);
/******************************* ENUM TYPE DEFINE ********************************/
typedef enum
{
    COM_TX_RX_MODE_DIRECT = 0,
    COM_TX_RX_MODE_MIXED,
    COM_TX_RX_MODE_PERIODIC,
} Com_TxRxModeEType;

typedef enum{
	COM_TXRX_CBK_IND_NONE = 0,
	COM_TXRX_CBK_IND_RX_TIMEOUT_SHORT_PERIODS,
	COM_TXRX_CBK_IND_RX_TIMEOUT_LONG_PERIODS,
}Com_TxRxCbkIndTyep;
/****************************** STRUCT TYPE DEFINE *******************************/
typedef struct
{
	Com_StatusType ComStatus;
} Com_ManagementType;

typedef struct
{
    uint32 Timeout;
} Com_RxIpduRunType;

typedef struct
{
    uint32 PreSendTime;
    uint32 RepeatCounter;
    uint32 Timeout;
} Com_TxIpduRunType;
/*===============================================================================*/
/*============================== END: TYHPE DEFINE ==============================*/
/*===============================================================================*/

/*===============================================================================*/
/*======================= START: CONFIGURATION TYPE DEFINE ======================*/
/*===============================================================================*/
typedef struct
{
    uint8 PduLength;
    uint8 QueueDepth;
    /*
    COM_TX_RX_MODE_DIRECT,
    COM_TX_RX_MODE_PERIODIC,
    COM_TX_RX_MODE_MIXED
    */
    Com_TxRxModeEType RxMode;
    uint32 Period;
    uint32 Timeout;
    uint32 FirstOffset;
    uint32 FirstTimeout;
} Com_RxIpduConfType;

typedef struct
{
    uint8 PduLength;
    PduIdType CanIfPduId;
    /*
    COM_TX_RX_MODE_DIRECT,
    COM_TX_RX_MODE_PERIODIC,
    COM_TX_RX_MODE_MIXED
    */
    Com_TxRxModeEType TxMode;
    uint32 Period;
    uint32 Timeout;
    uint32 FirstOffset;
    uint32 FirstTimeout;
    uint32 MinimumDelayTime;
    uint32 RepetitionPeriod;
    uint8 NumberOfRepetitions;
} Com_TxIpduConfType;

typedef struct{
	uint8 startFlag:1;
	uint8 firstDM:1;
}tCom_SimGlobalFlag;

typedef struct{
	uint8 rxDM:1;
	uint8 rxDMChecked:1;
	uint8 rxBufChange:1;
}tCom_SimRxFlag;

typedef struct{
	uint8 txDMRun:1;
	uint8 txCycleEvent:1;
	uint8 txTrigEvent:1;
	uint8 txOffsetTrig:1;
}tCom_SimTxFlag;


/*===============================================================================*/
/*========================= END: CONFIGURATION TYPE DEFINE ======================*/
/*===============================================================================*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*============================== END OF FILE: Com.h =============================*/

