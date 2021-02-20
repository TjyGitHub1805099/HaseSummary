
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "trace.h"
#include "status.h"

#include "Std_Types.h"
#include "System_Signal.h"
#include "Spi_Ipc_gw_api.h"
#include "System_Module_Cfg.h"
#include "System_Mutex.h"
#include "System_Tick.h"
#include "CircularQueue.h"

#include "AC_CTL.h"
#include "PanelKey.h"
#include "Key.h"

#if (STD_ON == SYS_MODULE_ADD_UART)
#include "lpuart_hw_access.h"
#include "uart_pal.h"
//#include "uart0_pal_cfg.h"
#include "uart_acpanel_cfg.h"
#endif

#if 1
boolean gPanelUartInitFlag = FALSE;
static boolean gPanelRxNewDataFlag = FALSE;
uint32 PanelCQueueCh = 0;

SystemMutexType panel_mutex;

uint8 gPanelKeyRxBuffer[PANEL_KEY_RX_BUFF_LEN] = {1};
uint8 gPanelRxQueueBuffer[PANEL_RX_QUEUE_BUFF_LEN] = {1};
uint8 gRxBuffer[PANEL_RX_QUEUE_BUFF_LEN] = {1};
uint8 gPannelKeyVal = 0;


#if (STD_ON == SYS_MODULE_ADD_UART)
void panel_rx_cbk(void *driverState, uart_event_t event, void *userData);
#endif


void PanelUartInit(void)
{
	status_t l_status = STATUS_ERROR;
	memset(gRxBuffer,0,PANEL_RX_QUEUE_BUFF_LEN);
	
	if (FALSE == gPanelUartInitFlag)
	{
		#if (STD_ON == SYS_MODULE_ADD_UART)
		//uart_acpanel_cfg_Config0.baudRate = 19200;//modified : because the bps changed by panle
		uart_acpanel_cfg_Config0.baudRate = 38400;//modified : because the bps changed by panle
		uart_acpanel_cfg_Config0.rxCallback = panel_rx_cbk;
		uart_acpanel_cfg_Config0.transferType = UART_USING_INTERRUPTS;
		l_status =UART_Init(PANEL_HW_CHANNEL,&uart_acpanel_cfg_Config0);
		#endif
#if 1
		if(STATUS_SUCCESS == l_status)
		{
			//gTraceNameCount = sizeof(gTraceModuleInfoList)/sizeof(tTraceModuleInforList);
			//System_Mutex_Create(&panel_mutex);
			if((PanelCQueueCh=CQueue_Init(CQUEUE_BUFTYPE_UINT8,gPanelRxQueueBuffer,PANEL_RX_QUEUE_BUFF_LEN)) != CQUEUE_ERR_CHANNEL )
			{
				gPanelUartInitFlag = TRUE;
			#if (STD_ON == SYS_MODULE_ADD_UART)
				LPUART_DRV_ReceiveData(PANEL_HW_CHANNEL,(const uint8*)gPanelKeyRxBuffer,PANEL_KEY_RX_BUFF_LEN);
			#endif
			}
			//else
			//	TracePrintf(TRACE_TRACE, TRACE_INFO, "FAIL TO INIT PANEL MODULE!!!\r\n");
		}
#endif
	}
}

uint8 Panel_LPUART_DRV_GetFifoRxCount(uint32 uartHwInstance)
{
	LPUART_Type * const s_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;
	uint8 waterRxCount = 0;
	
	waterRxCount = (uint8)((s_lpuartBase[uartHwInstance]->WATER & LPUART_WATER_RXCOUNT_MASK)>>24);
	return waterRxCount;
}

void Panel_LPUART_DRV_GetData(uint32 uartHwInstance,uint8*rxDataBuffPtr)
{
	LPUART_Type * const s_lpuartBase[LPUART_INSTANCE_COUNT] = LPUART_BASE_PTRS;

	*rxDataBuffPtr = (uint8)s_lpuartBase[uartHwInstance]->DATA;
}


void panel_rx_cbk(void *driverState, uart_event_t event, void *userData)
{
	uint8 rxCount = 0;
	//uint16 realDataLen = 0;
	uint8 dataLen = 0 ;
	//static uint32 invalidFrameCounter = 0;
	lpuart_state_t *lpuartState = (lpuart_state_t*)driverState;
	uint8 i = 0;
	static uint32 invalidIntCounter = 0;
	uint8 *rxBufPtr = lpuartState->rxBuff;
	
	if(event == UART_EVENT_RX_FULL || event == UART_EVENT_END_TRANSFER)
	{
		rxCount = Panel_LPUART_DRV_GetFifoRxCount(PANEL_HW_CHANNEL);
		if(rxCount>0)
		{
			for(i=0;i<rxCount;i++)
			{
				Panel_LPUART_DRV_GetData(PANEL_HW_CHANNEL,rxBufPtr);
				rxBufPtr++;
			}

			if(CQueue_Push(PanelCQueueCh,gPanelKeyRxBuffer,rxCount,STD_ON) != CQUEUE_ERR_PUSHPOP)
			{
				if(event == UART_EVENT_END_TRANSFER)
				{
					memset(gPanelKeyRxBuffer,0,PANEL_KEY_RX_BUFF_LEN);
					//ipcSemPost(gIpcRxSem);
					gPanelRxNewDataFlag = TRUE;
				}
			}

		}
		else if(event == UART_EVENT_END_TRANSFER)
		{
			memset(gPanelKeyRxBuffer,0,PANEL_KEY_RX_BUFF_LEN);
			//ipcSemPost(gIpcRxSem);
			gPanelRxNewDataFlag = TRUE;
		}
		else
			invalidIntCounter++;
		if(gPanelRxNewDataFlag == TRUE)
		{
			memset(gRxBuffer,0,PANEL_RX_QUEUE_BUFF_LEN);
			gPanelRxNewDataFlag = FALSE;
			dataLen = CQueue_Pop(PanelCQueueCh,gRxBuffer,CQUEUE_POP_ALL,STD_OFF);

			gPannelKeyVal = gRxBuffer[11] ;
			//if( gPannelKeyVal == 0)
			//{
			//	i=0;
			//}
			//changed by tjy : used in ac panel control
			if(( dataLen == AC_FRAME_END ) && ( gRxBuffer[AC_DATA_LEN] == (dataLen-4) ))
			{
				acFrame.newData = TRUE ;
				memcpy(&acFrame.rxData[0],&gRxBuffer[0],dataLen);
			}
		}
	}

}

uint8 getPanelkeyVal(uint8 Val)
{
	uint8 key_data = KEY_NONE;//KEY_PANEL_NONE;
    uint8 key_val = 0;
	key_val = Val & PANEL_KEY_VAL_MASK;
    if(key_val != 0)
    {
		switch(key_val)
	    {
			case PANEL_BACK_VAL:
				key_data = KEY_PANEL_BACK;
				break;
			case PANEL_MENU_VAL:
				key_data = KEY_PANEL_MENU;
				break;
			case PANEL_AVM_VAL:
				key_data = KEY_PANEL_AVM;
				break;
			case PANEL_PWR_VAL:
				key_data = KEY_PANEL_PWR;
				break;
			case PANEL_VOL_DOWN_VAL:
				key_data = KEY_PANEL_VOL_DONW;
				break;
			case PANEL_VOL_UP_VAL:
				key_data = KEY_PANEL_VOL_UP;
				break;
			case PANEL_HOME_VAL:
				key_data = KEY_PANEL_HOME;
				break;
			default:
				key_data = KEY_NONE;
				break;
		}
	}
	else
	{
		key_data = KEY_NONE;
	}
		
	return key_data;
}

#endif

#endif

