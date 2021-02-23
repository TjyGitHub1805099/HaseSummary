

#include "device_registers.h"

#include "lpspi_slave_driver.h"
#include "spi_pal.h"
#include "pins_driver.h"
#include "BoardDefines.h"

#include "SPI_CircularQueue.h"
#include "SpiIpc_cfg.h"
#include "osif.h"
#include "spi_pal.h"
#include "status.h"
#include "BoardDefines.h"
#include "spi_ipc_pal_cfg.h"
#include "spi_M_ipc_pal_cfg.h"
#include "portmacro.h"

#ifndef SPI_TRANS_ERROR_CHECK
#error "SPI_TRANS_ERROR_CHECK not defined, please update .cproject file"
#endif
 
#define SPI_HW_IDEX				1
#define SPI_HW_M_IDEX			0

#define SPI_TIMEOUT							500
#define SPI_CHANNEL_NUM0			    	7
#define SPI_CHANNEL_NUM1			    	IPC_GW_CHANNEL_MAX
#define SPI_MutexSize		sizeof(SpiIpcGlobalStruct)

SpiCqMutex gSpiIpcCQueueMutexIndex[HW_SPI_IPC_MAX_NUM];
mutex_t gSpiIpcMutexHandlePool[HW_SPI_IPC_MAX_NUM*SPI_MutexSize];
semaphore_t gSpiSemPool[HW_SPI_IPC_MAX_NUM];

uint8 LastTransErrorState[HW_SPI_IPC_MAX_NUM] = {0,0};
uint16 ReqWaitCnt[HW_SPI_IPC_MAX_NUM] = {0,0};
volatile uint32 ReqTickCnt[HW_SPI_IPC_MAX_NUM] = {0,0};
uint8 SpiReadWriteTmpBuf[SPI_MAX_FRAME_SIZE];	

void SpiSlaveInit0(void)
{
	SPI_SlaveInit(SPI_HW_IDEX,&spi_ipc_pal_SlaveConfig0);
}

void SpiSlaveInit1(void)
{
	SPI_SlaveInit(SPI_HW_M_IDEX,&spi_M_ipc_pal_cfg_SlaveConfig0);
}


const spi_initfun  spi_initGroup[HW_SPI_IPC_MAX_NUM]={
	SpiSlaveInit0,
	SpiSlaveInit1
};


void IncReqWaitCnt(uint8 hwNum)
{
	ReqWaitCnt[hwNum]++;
}

void ResetReqWaitCnt(uint8 hwNum)
{
	ReqWaitCnt[hwNum] = 0;
}

uint16 GetReqWaitCnt(uint8 hwNum)
{
	return ReqWaitCnt[hwNum];
}


void UpdateReqTickCnt(uint8 hwNum, uint32 tick_cnt)
{
	ReqTickCnt[hwNum] = tick_cnt;
}

uint32 GetReqTickCnt(uint8 hwNum)
{
	return ReqTickCnt[hwNum];
}

void HwReqMaster0(void)
{
	static boolean state = 0;
	portDISABLE_INTERRUPTS();
	if(state == 0)
		PINS_DRV_ClearPins(PORTGP_SPI_IPC_REQ,(1 << PINNUM_SPI_IPC_REQ));
	else
		PINS_DRV_SetPins(PORTGP_SPI_IPC_REQ,(1 << PINNUM_SPI_IPC_REQ));
	state = !state;
	portENABLE_INTERRUPTS();
}

void HwAckMaster0(void)
{
	static boolean state = 0;
	portDISABLE_INTERRUPTS();
	if(state == 0)
		PINS_DRV_ClearPins(PORTGP_SPI_IPC_ACK,(1 << PINNUM_SPI_IPC_ACK));
	else
		PINS_DRV_SetPins(PORTGP_SPI_IPC_ACK,(1 << PINNUM_SPI_IPC_ACK));
	state = !state;
	portENABLE_INTERRUPTS();
}
#if 1
void HwReqMaster1(void)
{
	static boolean state = 0;
	portDISABLE_INTERRUPTS();
	if(state == 0)
		PINS_DRV_ClearPins(PORTGP_SPI_M_IPC_REQ,(1<<PINNUM_SPI_M_IPC_REQ));
	else
		PINS_DRV_SetPins(PORTGP_SPI_M_IPC_REQ,(1<<PINNUM_SPI_M_IPC_REQ));
	state = !state;
	portENABLE_INTERRUPTS();
}
void HwAckMaster1(void)
{
	static boolean state = 0;
	portDISABLE_INTERRUPTS();
	if(state == 0)
		PINS_DRV_ClearPins(PORTGP_SPI_M_IPC_ACK,(1<<PINNUM_SPI_M_IPC_ACK));
	else
		PINS_DRV_SetPins(PORTGP_SPI_M_IPC_ACK,(1<<PINNUM_SPI_M_IPC_ACK));
	state = !state;
	portENABLE_INTERRUPTS();
}
#else
void HwReqMaster1(void)
{
	uint8 count;
	portDISABLE_INTERRUPTS();
	PINS_DRV_ClearPins(PORTGP_SPI_M_IPC_REQ,(1<<PINNUM_SPI_M_IPC_REQ));
	for(count=0;count<50;count++)
	{
		__asm("nop");
	}
	 PINS_DRV_SetPins(PORTGP_SPI_M_IPC_REQ,(1<<PINNUM_SPI_M_IPC_REQ));
	 portENABLE_INTERRUPTS();
}
uint16 Ack1Count=0;
void HwAckMaster1(void)
{
	uint8 count;

#if    1
	portDISABLE_INTERRUPTS();
	PINS_DRV_ClearPins(PORTGP_SPI_M_IPC_ACK,(1<<PINNUM_SPI_M_IPC_ACK));
	for(count=0;count<50;count++)
	{
		__asm("nop");
	}
	 PINS_DRV_SetPins(PORTGP_SPI_M_IPC_ACK,(1<<PINNUM_SPI_M_IPC_ACK));
	 portENABLE_INTERRUPTS();
	 Ack1Count++;
#endif
}
#endif

const spi_ReqAckfun spiReq[HW_SPI_IPC_MAX_NUM]={
	HwReqMaster0,
	HwReqMaster1,
};

const spi_ReqAckfun spiAck[3]={
	HwAckMaster1,
	HwAckMaster0,
	NULL
};

Spi_notification_callback  Spi_Ntf_cbs0[SPI_CHANNEL_NUM0]={NULL};

Spi_notification_callback  *GroupSpi_Ntf_cbs[HW_SPI_IPC_MAX_NUM]={
Spi_Ntf_cbs0,
NULL
};

uint8 HwSpiTransBlock0(uint8 *pReceieBuf,uint8 *pSendBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;
	
	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, pSendBuf, pReceieBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, pSendBuf, pReceieBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

uint8 HwSpiTransBlock1(uint8 *pReceieBuf,uint8 *pSendBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;
	
	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, pSendBuf, pReceieBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, pSendBuf, pReceieBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}



uint8 HwSpiRxBlock0(uint8 *pBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;
	
	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, SpiReadWriteTmpBuf, pBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, SpiReadWriteTmpBuf, pBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}


uint8 HwSpiRxBlock1(uint8 *pBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;
	
	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, SpiReadWriteTmpBuf, pBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, SpiReadWriteTmpBuf, pBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}


uint8 HwSpiTxBlock0(uint8 *pBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;

	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, pBuf, SpiReadWriteTmpBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_IDEX, pBuf, SpiReadWriteTmpBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

uint8 HwSpiTxBlock1(uint8 *pBuf,uint16 Len,boolean WaitFlag)
{
	status_t Flag;
	
	if(WaitFlag == STD_ON)
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, pBuf, SpiReadWriteTmpBuf, Len, OSIF_WAIT_FOREVER);
	}
	else
	{
		Flag = LPSPI_DRV_SlaveTransferBlocking(SPI_HW_M_IDEX, pBuf, SpiReadWriteTmpBuf, Len, SPI_TIMEOUT);
	}
	if(Flag == STATUS_SUCCESS)
	{
		return E_OK;
	}
	else
	{
		return E_NOT_OK;
	}
}

const spi_Transfun SpiTransDataGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiTransBlock0,
	HwSpiTransBlock1
};


const spi_TxRxfun SpiReadDataGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiRxBlock0,
	HwSpiRxBlock1
};

const spi_TxRxfun SpiSendDataGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiTxBlock0,
	HwSpiTxBlock1	
};

uint8 HwSpiGetStatus0(void)
{
	status_t SpiStatus;

	SpiStatus = LPSPI_DRV_SlaveGetTransferStatus(SPI_HW_IDEX, NULL);

	if(SpiStatus == STATUS_SUCCESS)
	{
		return SPI_STATUS_SUCESS;
	}
	else if(SpiStatus == STATUS_BUSY)
	{
		return SPI_STATUS_BUSY;
	}
	else
	{
		return SPI_STATUS_ERROR;
	}
}

uint8 HwSpiGetStatus1(void)
{
	status_t SpiStatus;

	SpiStatus = LPSPI_DRV_SlaveGetTransferStatus(SPI_HW_M_IDEX, NULL);

	if(SpiStatus == STATUS_SUCCESS)
	{
		return SPI_STATUS_SUCESS;
	}
	else if(SpiStatus == STATUS_BUSY)
	{
		return SPI_STATUS_BUSY;
	}
	else
	{
		return SPI_STATUS_ERROR;
	}
}


const spi_GetStatus  SpiGetStatusGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiGetStatus0,
	HwSpiGetStatus1
};

uint8 HwSpiAbortTransfer0(void)
{
	return LPSPI_DRV_SlaveEndTransfer(SPI_HW_IDEX);
}

uint8 HwSpiAbortTransfer1(void)
{
	return LPSPI_DRV_SlaveEndTransfer(SPI_HW_M_IDEX);
}

const spi_AbortTransfer SpiAbortTransferGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiAbortTransfer0,
	HwSpiAbortTransfer1
};

const uint8 spi_HwChannelToInstance[HW_SPI_IPC_MAX_NUM]=
{
	SPI_HW_IDEX,
	SPI_HW_M_IDEX
};

uint8 spi_GetLastTransErrorState0(void)
{
	return LastTransErrorState[SPI_HW_IPC_NUM0];
}

uint8 spi_GetLastTransErrorState1(void)
{
	return LastTransErrorState[SPI_HW_IPC_NUM1];
}


const spi_GetLastTransErrorState spi_GetLastTransErrorStateGroup[HW_SPI_IPC_MAX_NUM]={
	spi_GetLastTransErrorState0,
	spi_GetLastTransErrorState1
};


void spi_SetLastTransErrorState0(uint8 state)
{
	LastTransErrorState[SPI_HW_IPC_NUM0] = state;
}

void spi_SetLastTransErrorState1(uint8 state)
{
	LastTransErrorState[SPI_HW_IPC_NUM1] = state;
}

const spi_SetLastTransErrorState spi_SetLastTransErrorStateGroup[HW_SPI_IPC_MAX_NUM]={
	spi_SetLastTransErrorState0,
	spi_SetLastTransErrorState1
};

SPI_IPC_MERGE_CH_TYPE HwSpiGetChMergeType0(void)
{
	return SPI_IPC_SINGLE_CH;
}

SPI_IPC_MERGE_CH_TYPE HwSpiGetChMergeType1(void)
{
	return SPI_IPC_SINGLE_CH;
}

const spi_GetChMergeType SpiGetChMergeTypeGroup[HW_SPI_IPC_MAX_NUM]={
	HwSpiGetChMergeType0,
	HwSpiGetChMergeType1
};

#if (SPI_IPC_QUEUE_DYNAMIC == 0)
uint8 SPI_TxQueueBuf0[SPI_CHANNEL_NUM0][SPI_CQUEUE_TXCH_SIZE];
uint8 SPI_RxQueueBuf0[SPI_CHANNEL_NUM0][SPI_CQUEUE_RXCH_SIZE];
uint8 SPI_TxQueueBuf1[SPI_CHANNEL_NUM1][SPI_CQUEUE_TXCH_SIZE];
uint8 SPI_RxQueueBuf1[SPI_CHANNEL_NUM1][SPI_CQUEUE_RXCH_SIZE];

uint8 *SPI_TxQueueGroup[HW_SPI_IPC_MAX_NUM]={
(uint8 *)SPI_TxQueueBuf0,
(uint8 *)SPI_TxQueueBuf1
};

uint8 *SPI_RxQueueGroup[HW_SPI_IPC_MAX_NUM]={
(uint8*)SPI_RxQueueBuf0,	
(uint8*)SPI_RxQueueBuf1
};

#else
uint16 SPI_QueueSize0[SPI_CHANNEL_NUM0][2] = {

{1,							2},	
{3,							4},	
{SPI_CQUEUE_TXCH_SIZE, 		SPI_CQUEUE_RXCH_SIZE},		
{SPI_CQUEUE_TXCH_SIZE, 		SPI_IPC_QUEUE_MIN_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	 	SPI_CQUEUE_RXCH_SIZE},		
{SPI_IPC_QUEUE_MIN_SIZE, 	SPI_CQUEUE_RXCH_SIZE},		
{SPI_IPC_QUEUE_MIN_SIZE,	SPI_IPC_QUEUE_MIN_SIZE},	
};

uint16 SPI_QueueSize1[SPI_CHANNEL_NUM1][2] = {

{5,						6},	
{7,						8},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
{SPI_CQUEUE_TXCH_SIZE,	SPI_CQUEUE_RXCH_SIZE},	
};

SPI_QueueSizeGroupType SPI_QueueSizeGroup[HW_SPI_IPC_MAX_NUM]={
SPI_QueueSize0,
SPI_QueueSize1
};

#endif

SPI_CQueue_ManagerType SPI_TxQueue_Manager0[SPI_CHANNEL_NUM0];
SPI_CQueue_ManagerType SPI_RxQueue_Manager0[SPI_CHANNEL_NUM0];
SPI_CQueue_ManagerType SPI_TxQueue_Manager1[SPI_CHANNEL_NUM1];
SPI_CQueue_ManagerType SPI_RxQueue_Manager1[SPI_CHANNEL_NUM1];	

SPI_CQueue_ManagerType *SPI_TxQueueM_Group[HW_SPI_IPC_MAX_NUM]={
SPI_TxQueue_Manager0,
SPI_TxQueue_Manager1
};

SPI_CQueue_ManagerType *SPI_RxQueueM_Group[HW_SPI_IPC_MAX_NUM]={
SPI_RxQueue_Manager0,
SPI_RxQueue_Manager1
};


const SpiIpcChCfg SpiIpcUserCfg[HW_SPI_IPC_MAX_NUM]={
#if   1
{SPI_HW_IPC_NUM0,SPI_HW_IDEX,SPI_CHANNEL_NUM0,SPI_CQUEUE_TXCH_SIZE,SPI_CQUEUE_RXCH_SIZE},
{SPI_HW_IPC_NUM1,SPI_HW_M_IDEX,SPI_CHANNEL_NUM1,SPI_CQUEUE_TXCH_SIZE,SPI_CQUEUE_RXCH_SIZE},
#else
{SPI_HW_IPC_NUM0,SPI_HW_M_IDEX,SPI_CHANNEL_NUM0,SPI_CQUEUE_TXCH_SIZE,SPI_CQUEUE_RXCH_SIZE},
#endif
};

const SpiIpcGlobalStruct gSpiIpcGlobalCfg = {
	.spiHwMaxCh = HW_SPI_IPC_MAX_NUM,
	.spiIpcMutexPoolSize = HW_SPI_IPC_MAX_NUM*SPI_MutexSize,
	.spiIpcSemPoolSize = HW_SPI_IPC_MAX_NUM,
};


