/**
*@file  CircularQueue.h
*@brief Containing the entire or parts of Circular Queue code.
*@date    2017-10-26 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-10-26  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _IPC_CIRCULAR_QUEUE_H_
#define _IPC_CIRCULAR_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"


typedef enum
{
    SPI_EVENT_TYPE_TIMER = 0,
    SPI_EVENT_TYPE_RECV_DATA,    //  recv HEARTBEAT or ACK
    SPI_EVENT_TYPE_SEND_ACK,
    SPI_EVENT_TYPE_SEND_DATA,              //  send RELIABLE data
    SPI_EVENT_TYPE_SEND_DATA_UNRELIABLE    //  send UNRELIABLE data
} SpiEventType;



typedef struct {
	uint8 init;
	uint16 head;
	uint16 tail;
	uint16 size;
	uint16 FrameCount;
	uint8 *buf;
} SPI_CQueue_ManagerType;



//uint8 IPC_RxTxQueue_Init(void);


extern uint8 SPI_CQueue_Init(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 size);
extern uint8 SPI_CQueue_PushFront(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 len);
extern uint8 SPI_CQueue_Push(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 len);
extern uint16 SPI_CQueue_Pop(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 buffer_size);
extern uint16 SPI_CQueue_PopMulti(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *buffer, uint16 buffer_size);
extern uint16 SPI_CQueue_PopMultiChannel(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 *buffer, uint16 buffer_size);
extern uint8 SPI_CQueue_GetTotalLen(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint16 *len);
extern uint8 SPI_CQueue_GetLen(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint16 *len);
extern uint8 SPI_CQueue_IsEmpty(uint8 HwChannel,SPI_CQueue_ManagerType *SPI_CQueue_Manager, uint8 channel, uint8 *isEmpty);

extern void SPI_CQueueGlobalLock(uint8 HwChannel);

extern void SPI_CQueueGlobalUnLock(uint8 HwChannel);

extern SPI_CQueue_ManagerType * SPI_CQueueGetTxManagerHandle(uint8 HwChannel);
extern uint8 SPI_RxTxQueue_Init(void);
extern SPI_CQueue_ManagerType * SPI_CQueueGetRxManagerHandle(uint8 HwChannel);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CIRCULAR_QUEUE_H_
/*=========================== END OF FILE: CircularQueue.h ===========================*/

