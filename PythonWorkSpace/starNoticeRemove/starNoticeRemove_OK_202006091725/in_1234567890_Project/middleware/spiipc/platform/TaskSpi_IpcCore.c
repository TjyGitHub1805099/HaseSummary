#include "System_Task.h"
#include "System_Signal.h"
#include "Spi_if.h"
#include "TaskSpi_IpcCore.h"
#include "Spi_IpcCore.h"
#include "SpiIpc_cfg.h"
#include "SpiIpc.h"
#include "spiSem.h"
#include "SPI_CircularQueue.h"
#include "Spi_Ipc_gw_api.h"
#include "osif.h"
#include "ipc_gw_core.h"
#include "dea.h"
#include "trace.h"
#include "SpiIpc_cfg.h"



extern ipcChannelRecvFn gOutputCallback;
boolean StartSend;
void Task_SpiCoreRead(void * pvParameters)
{
	(void)pvParameters;
	Spi_Init(SPI_HW_IPC_NUM0);
	vTaskDelay(pdMS_TO_TICKS(10));
	while (1)
	{
		HandleSPI_IPC_Messages(SPI_HW_IPC_NUM0);
			/*===============================================================================*/
			/*================================ END: USER CODE ===============================*/
			/*===============================================================================*/
#if (STD_ON == TASK_SPIAPP_STACK_MONITOR)
			l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_SPIAPP_DEBUG_TRACE)
			TracePrintf(SPIAPP_TRACE, TRACE_WARNING,  FILE_ID, LINE_NUM, "[Task_SpiApp-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
	}
}

void Task_SpiCoreRead_M(void * pvParameters)
{
	(void)pvParameters;
	Spi_Init(SPI_HW_IPC_NUM1);
	vTaskDelay(pdMS_TO_TICKS(10));
	while (1)
	{
		/*if(BACKRAM_IPC_FLAG == IPC_MAGIC_VALUE)
		{
			HandleSPI_IPC_Messages_Old(SPI_HW_IPC_NUM1);
		}
		else*/
		{
			HandleSPI_IPC_Messages(SPI_HW_IPC_NUM1);
		}
		/*===============================================================================*/
		/*================================ END: USER CODE ===============================*/
		/*===============================================================================*/
#if (STD_ON == TASK_SPIAPP_STACK_MONITOR)
		l_stackSize = uxTaskGetStackHighWaterMark(NULL);
#if (STD_ON == TASK_SPIAPP_DEBUG_TRACE)
		TracePrintf(SPIAPP_TRACE, TRACE_WARNING,  FILE_ID, LINE_NUM, "[Task_SpiApp-W]:Stack Size:%d\r\n",l_stackSize);
#endif
#endif
	}
}


#ifdef   SPI_IPC_DEBUG
uint32 SpiCH1Count=0;
uint32 SpiCH2Count=0;
uint32 SpiCH3Count=0;
uint32 SpiCH4Count=0;



void Spi_CbCh0(uint16 function, uint16 feature, uint8 *rx_data, sint32 len)
{
	SpiCH1Count++;
}
void Spi_CbCh1(uint16 function, uint16 feature, uint8 *rx_data, sint32 len)
{
	SpiCH2Count++;
}

void Spi_CbCh2(uint16 function, uint16 feature, uint8 *rx_data, sint32 len)
{
	SpiCH3Count++;
	//Ic_Ipc_send_notification(SPI_HW_IPC_NUM0,2,function,feature, rx_data,len);
}

void Spi_CbCh3(uint16 function, uint16 feature, uint8 *rx_data, sint32 len)
{
	SpiCH4Count++;
	//Ic_Ipc_send_notification(SPI_HW_IPC_NUM0,3,function,feature,rx_data,len);
}

void IPC_Test_callback(const void *ctx, uint8 *rx_data, uint32 len)
{
#if    1
	uint8 buff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

   // SYSTIM  l_curTime;
    uint16 curTime[2];

    request_context_t *l_ctx = (request_context_t*)ctx;
	
    switch(l_ctx->function_id) {
	
		case 101:
		   StartSend = 1;
		   ipc_gw_send_reply(1,ctx,rx_data,len);
		//TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Task_Spi]:Reply = 6\r\n");
			break;


		case 102:
			StartSend = 0;
			ipc_gw_send_reply(1,ctx,rx_data,len);
			break;
    }
#endif
}

//void IPC_Notf_callback(uint16 function, uint8 *rx_data, uint32 len)
void IPC_Notf_callback(uint16 function, uint8 *rx_data, uint32 len)	
{
#if    1
	uint8 buff[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

   // SYSTIM  l_curTime;
    uint16 curTime[2];

    //request_context_t *l_ctx = (request_context_t*)ctx;
	
    switch(function) {
		case 7:
		//StartSend = 1;
		//TracePrintf(GENERAL_TRACE, TRACE_WARNING, "[Task_Spi]:Reply = 6\r\n");
			break;

		case 102:
		//	StartSend = 0;
			break;
    }
#endif
}

#endif


void Task_SpiCoreRx0(void * pvParameters)
{
	uint8 ch = 0;
	uint16 len = 0;
	uint16 function = 0;
	uint16 feature = 0;
	uint8  HwSpiBuf0[SPI_MAX_FRAME_SIZE]={0};
	(void)pvParameters;

	ipc_gw_init();
	Spi_notification_callback callback = NULL;
	vTaskDelay(2 / portTICK_RATE_MS);

#ifdef   SPI_IPC_DEBUG
	register_Spi_ipc_callback(0,0, Spi_CbCh0);
	register_Spi_ipc_callback(0,1, Spi_CbCh1);
	register_Spi_ipc_callback(0,2, Spi_CbCh2);
	register_Spi_ipc_callback(0,3, Spi_CbCh3);
#endif

	while(1)
	{
		if(spiSemWait(SPI_HW_IPC_NUM0)==0)
		{
		#if   1
			for(ch = 0; ch < SpiIpcUserCfg[SPI_HW_IPC_NUM0].ChannelNum; ch++)
			{
				len = 0;
				if(SPI_CQueue_GetLen((uint8)SPI_HW_IPC_NUM0,SPI_CQueueGetRxManagerHandle(SPI_HW_IPC_NUM0), ch, &len) == E_OK)
				{
					if(len > 0)
					{
						len = SPI_CQueue_Pop(SPI_HW_IPC_NUM0,SPI_CQueueGetRxManagerHandle(SPI_HW_IPC_NUM0), ch, HwSpiBuf0, sizeof(HwSpiBuf0));
						callback = get_Spi_ipc_callback(SPI_HW_IPC_NUM0,ch);
						if(callback != NULL)
						{
							function =  HwSpiBuf0[1];
							function <<= 8;
							function += HwSpiBuf0[0];
							
							feature =  HwSpiBuf0[3];
							feature <<= 8;
							feature += HwSpiBuf0[2];
							callback(function, (uint16)feature, ((uint8 *)HwSpiBuf0+4), (len-4));
							break;
						}
					}
				}
			}
		#endif
		}
	}
}


void Task_SpiCoreRx0_M(void * pvParameters)
{
	uint8 ch = 0;
	uint16 len = 0;
	//uint16 function = 0;
	//uint16 feature = 0;
	uint8  HwSpiBufM[SPI_MAX_FRAME_SIZE]={0};
	(void)pvParameters;

	ipcChannelRecvFn callback = NULL;
	vTaskDelay(2 / portTICK_RATE_MS);

#ifdef   SPI_IPC_DEBUG
	ipc_gw_register(2, IPC_Notf_callback, IPC_Test_callback);
#endif

	while(1)
	{
		if(spiSemWait(SPI_HW_IPC_NUM1)==0)
		{
		#if    1
			for(ch = 0; ch < SpiIpcUserCfg[SPI_HW_IPC_NUM1].ChannelNum; ch++)
			{
				len = 0;
				if(SPI_CQueue_GetLen((uint8)SPI_HW_IPC_NUM1,SPI_CQueueGetRxManagerHandle(SPI_HW_IPC_NUM1), ch, &len) == E_OK)
				{
					if(len > 0)
					{
						len = SPI_CQueue_Pop(SPI_HW_IPC_NUM1, SPI_CQueueGetRxManagerHandle(SPI_HW_IPC_NUM1), ch, HwSpiBufM, sizeof(HwSpiBufM));
						callback = gOutputCallback;
						if(callback != NULL)
						{
							gOutputCallback (ch, HwSpiBufM, len);
							break;
						}
					}
				}
			}
	   #endif
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////



