/**************************
author: Yi
version: v2.0.0
notes:
	1.this is a interface layer to CAN application according current architect
	2.multiple channals app supported may be aware of configuration,especially the number of MB, rx fifo mask
	individual mask should be checked by datasheet
	3.to run correctly can moudule,flexcan module gernerated code was fixed by manually.
	4.dma transfering in fifo is not surrported , adding related code in FLEXCAN_CompleteRxMessageFifoData if necessary.
**************************/
#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_


//#include <null.h>
#include "ComStack_Types.h" 
#include "flexcan_driver.h"

#define CAN_CH_MAX_NUM 	3

#define CAN_CHN_CFG_0					0
#define CAN_CHN_CFG_1					1
//#define CAN_CHN_CFG_2					2


//===================================================================================
#define CANIF_TX_PDU_CFG_CBK_ID_0       13           // IC diagnosis for other modules
#define CANIF_TX_PDU_CFG_CBK_ID_1       15 //20      //PHY1   IC
#define CANIF_TX_PDU_CFG_CBK_ID_2       17 //21      //PHY2   IVI
#define CANIF_TX_PDU_CFG_CBK_ID_3       18 //22      //PHY3   AVM
#define CANIF_TX_PDU_CFG_CBK_ID_4       14 //23      //PHY3   HUD
#define CANIF_TX_PDU_CFG_CBK_ID_5       16 //                 AME

//===================================================================================
#define CANIF_RX_PDU_CFG_CBK_ID_1       40      //PHY1   IC
#define CANIF_RX_PDU_CFG_CBK_ID_2       41      //PHY2   IVI
#define CANIF_RX_PDU_CFG_CBK_ID_3       43      //PHY3   AVM
#define CANIF_RX_PDU_CFG_CBK_ID_4       44      //PHY3   HUD
#define CANIF_RX_PDU_CFG_CBK_ID_5       45      //       AME


#define CANIF_RX_PDU_CFG_CBK_ID_0       42      //FUNC
//===================================================================================


#define CANIF_USE_RXFIFO STD_OFF


#ifdef CAN_CHN_CFG_0
#define CANIF_RX_FILTER_NUM_0 22
#define CANIF_TX_PDU_NUM_0 8  //7
#define CANIF_RX_PDU_NUM_0 37 //31 //35 //34 //33
#endif

#ifdef CAN_CHN_CFG_1
#define CANIF_RX_FILTER_NUM_1 12//20
#define CANIF_TX_PDU_NUM_1 11 //16
#define CANIF_RX_PDU_NUM_1 15 //47 //45 //46 //42
#endif

#ifdef CAN_CHN_CFG_2
#define CANIF_RX_FILTER_NUM_2 0
#define CANIF_TX_PDU_NUM_2 0
#define CANIF_RX_PDU_NUM_2 0
#endif



#define CANIF_TX_PDU_NUM (CANIF_TX_PDU_NUM_1 + CANIF_TX_PDU_NUM_0)//CAN TX NUM TOTAL



#endif

