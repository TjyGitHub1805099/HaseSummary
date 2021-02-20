
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Spi_if.c
**          Author: brown
**
**          Date  : 2018-04-23
**          
**          
******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "Spi_if.h"
#include "SpiIpc_cfg.h"
#include "SpiIpc.h"

extern const spi_initfun spi_initGroup[];

extern const spi_ReqAckfun spiReq[];
extern const spi_ReqAckfun spiAck[];
extern const spi_TxRxfun SpiReadDataGroup[];
extern const spi_Transfun SpiTransDataGroup[];
extern const spi_TxRxfun SpiSendDataGroup[];
extern const spi_GetStatus SpiGetStatusGroup[];
extern const spi_AbortTransfer SpiAbortTransferGroup[];
extern const spi_GetLastTransErrorState spi_GetLastTransErrorStateGroup[];
extern const spi_SetLastTransErrorState spi_SetLastTransErrorStateGroup[];
extern uint8 spi_HwChannelToInstance[];
extern const spi_GetChMergeType SpiGetChMergeTypeGroup[];

void Spi_Init(uint8 HwChannel)
{
	(*spi_initGroup[HwChannel])();
}

void SpiMasterReq(uint8 HwChannel)
{
	(*spiReq[HwChannel])();
}

void SpiMasterAck(uint8 HwChannel)
{
	//(*spiAck[HwChannel])();
}

void SpiMasterAck1(uint8 HwChannel)
{
	(*spiAck[HwChannel])();
}

uint8 SpiReadData(uint8 HwChannel,uint8 *pData,uint16 Len,boolean WaitFlag)
{
	return ((*SpiReadDataGroup[HwChannel])(pData, Len, WaitFlag));
}

uint8 SpiTransData(uint8 HwChannel,uint8 *pReceiveData, uint8 *pSendData, uint16 Len,boolean WaitFlag)
{
	return ((*SpiTransDataGroup[HwChannel])(pReceiveData, pSendData, Len, WaitFlag));
}


uint8 SpiSendData(uint8 HwChannel,uint8 *pData,uint16 Len,boolean WaitFlag)
{
	return ((*SpiSendDataGroup[HwChannel])(pData, Len, WaitFlag));
}

uint8 GetCurSpiStatus(uint8 HwChannel)
{
	return ((*SpiGetStatusGroup[HwChannel])());
}

uint8 SpiAbortTransfer(uint8 HwChannel)
{
	return ((*SpiAbortTransferGroup[HwChannel])());
}

uint8 SpiGetLastTransErrorStateByInstance(uint8 instance)
{
	if(instance <= HW_SPI_IPC_MAX_NUM)
	{
		return ((*spi_GetLastTransErrorStateGroup[instance])());
	}
	else
	{
		return 0;
	}
}

void SpiSetLastTransErrorState(uint8 HwChannel, uint8 state)
{
	(*spi_SetLastTransErrorStateGroup[spi_HwChannelToInstance[HwChannel]])(state);
}

void SpiSetLastTransErrorStateByInstance(uint8 instance, uint8 state)
{
	if(instance <= HW_SPI_IPC_MAX_NUM)
	{
		(*spi_SetLastTransErrorStateGroup[instance])(state);
	}
}



uint8 SpiIsChMerge(uint8 HwChannel)
{
	if((*SpiGetChMergeTypeGroup[HwChannel])() == SPI_IPC_MULTI_CH)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

