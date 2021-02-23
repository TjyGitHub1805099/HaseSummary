#include "stdio.h"
#include "hashmap.h"
#include "SPI_CircularQueue.h"
#include "Spi_if.h"
#include "SpiIpc_cfg.h"
#include "SpiIpc.h"
#include "Spi_Ipc_gw_api.h"
#include <string.h>
#include "osif.h"
#include "Spi_IpcCore.h"
#include "portmacro.h"
#include "ipc_gw_adaptor.h"
#include "ipc_gw_core.h"
#include "trace.h"
#include "string.h"
#include "portmacro.h"


#define SPI_REQ_INTERVAL	(200U)

extern TickType_t xTaskGetTickCount( void );

#if    1
error_type register_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel, Spi_notification_callback ntf_cb)
{
	GroupSpi_Ntf_cbs[HwChannel][Schannel] = ntf_cb;
	return IPC_GW_ERR_OK;
}

error_type unregister_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel)
{
	GroupSpi_Ntf_cbs[HwChannel][Schannel] = NULL;
	return IPC_GW_ERR_OK;
}

Spi_notification_callback get_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel)
{
	return (GroupSpi_Ntf_cbs[HwChannel][Schannel]);
}

error_type ipc_gw_register(
    uint32 channel,
    notification_callback ntf_cb,
    request_callback req_cb)
{
#ifdef IPC_PLTF_IS_QNX
    ipc_gw_debug_common("[->IPCGW]<register>(channel):%04d\n\r", channel);
#endif
    return register_callback(channel, ntf_cb, req_cb);
}
#endif

error_type ipc_gw_unregister(
    uint32 channel)
{
    return unregister_callback(channel);
}


boolean GetM_IpcComStatus(void)
{
	return Spi_MComFLag;
}

error_type ipc_gw_send_notification_noAck(
uint8 HwChannel,
uint32 channel,
        uint16 function,
        const uint8 *data,
        uint32 len)
{
    msg_head head;
    uint8 *msg;
    uint16 tx_len;
    IpcCoreErr result;
	uint8 l_reqFlag = 1;
	uint16 qlen = 0;

    
    head.type = IPC_GW_NOTIFY;
    head.sequence_number = 0;
    head.function_id = function;

	if(Spi_MComFLag == FALSE)
	{
		return IPC_GW_ERR_SEND_IPC;
	}
	
    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    
    if(data == NULL || len == 0)
    {
        tx_len  = sizeof(msg_head);
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
        		memcpy(msg, &head, sizeof(msg_head));
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }
    else
    {
        tx_len  = sizeof(msg_head) + len;
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
        		memcpy(msg, &head, sizeof(msg_head));
        		memcpy(msg+sizeof(msg_head), data, len);
		}
		else
		{
		 	return IPC_GW_ERR_SEND_IPC;
		}
    }

    
    #if 1
	SPI_CQueueGlobalLock(HwChannel);
	SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		l_reqFlag = 0;
	}
	result = SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, msg, tx_len);
	SPI_CQueueGlobalUnLock(HwChannel);
	if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
	{
		SpiMasterReq(HwChannel);
		UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
	}
#else
		SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, data_buf, len+4);
		SpiMasterReq(HwChannel);
#endif

    if(E_NOT_OK == result)
    {
        
        FREE(msg);
        return IPC_GW_ERR_SEND_IPC;
    }

    FREE(msg);

    return IPC_GW_ERR_OK;
}     

error_type Ic_Ipc_send_notification
	(
uint8 HwChannel,
    uint8 channel,
   	uint16  function,
    uint16 feature,
    const uint8 *data,
    sint32 len)
	{
		uint16 qlen = 0;
		error_type ret = IPC_GW_ERR_OK;
		uint8 data_buf[SPI_MAX_FRAME_SIZE];
		uint8 l_reqFlag = 1;
	
		if(len > SPI_MAX_DATA_SIZE)
		{
			ret = IPC_GW_ERR_INVALID_MESSAGE;
		}
		else
		{
			data_buf[0] = function;
			data_buf[1] = function>>8;
			data_buf[2] = feature;
			data_buf[3] = feature>>8;
			memcpy(&data_buf[4],data,len);
#if 1
			
			SPI_CQueueGlobalLock(HwChannel);
			SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
			if(qlen > 0)
			{
				l_reqFlag = 0;
			}
			SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, data_buf, len+4);
			
			if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
			{
				SpiMasterReq(HwChannel);
				UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
			}
			SPI_CQueueGlobalUnLock(HwChannel);
#else
			SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, data_buf, len+4);
			SpiMasterReq(HwChannel);
#endif
	
		}
		return ret;
	}


error_type ipc_send_notification(
uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    sint32 len)
{
    msg_head head;
    uint8 *msg;
    uint16 tx_len;
    IpcCoreErr result;
	uint8 l_reqFlag = 1;
	uint16 qlen = 0;

    
    head.type = IPC_GW_NOTIFY;
    head.sequence_number = 0;
    head.function_id = function;

	if(Spi_MComFLag == FALSE)
	{
		return IPC_GW_ERR_SEND_IPC;
	}
	
    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    
    if(data == NULL || len == 0)
    {
        tx_len  = sizeof(msg_head);
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        	memcpy(msg, &head, sizeof(msg_head));
		}
		else
		{
			 return IPC_GW_ERR_MEMORY_ALLOC;
		}
    }
    else
    {
        tx_len  = sizeof(msg_head) + len;
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        	memcpy(msg, &head, sizeof(msg_head));
	        	memcpy(msg+sizeof(msg_head), data, len);
		}
		else
		{
			 return IPC_GW_ERR_MEMORY_ALLOC;
		}
    }

    
    
	SPI_CQueueGlobalLock(HwChannel);
	SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		l_reqFlag = 0;
	}
	result = SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, msg,tx_len);
	SPI_CQueueGlobalUnLock(HwChannel);
	if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
	{
		SpiMasterReq(HwChannel);
		UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
	}
	
    if(E_NOT_OK == result)
    {
        
        FREE(msg);
        return IPC_GW_ERR_QUEUE_FULL;
    }
    FREE(msg);

    return IPC_GW_ERR_OK;
}

error_type ipc_gw_send_request(
	uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    uint32 len,
    const void *context,
    invoke_callback cb)
{
    uint16 sn;
    msg_head head;
    uint8 *msg;
    uint16 tx_len;
    IpcCoreErr result;
	uint8 l_reqFlag = 1;
	uint16 qlen = 0;

	if(Spi_MComFLag == FALSE)
	{
		return IPC_GW_ERR_SEND_IPC;
	}
	
    
    insert_element(context, cb, &sn);
    dump_element();

    
    head.type = IPC_GW_REQ;
    head.sequence_number = sn;
    head.function_id = function;

    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    
    if(data == NULL || len == 0)
    {
        tx_len  = sizeof(msg_head);
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        memcpy(msg, &head, sizeof(msg_head));
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }
    else
    {
        tx_len  = sizeof(msg_head) + len;
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        	memcpy(msg, &head, sizeof(msg_head));
	        	memcpy(msg+sizeof(msg_head), data, len);
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }

    
    
	SPI_CQueueGlobalLock(HwChannel);
	SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		l_reqFlag = 0;
	}
	result = SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, msg,tx_len);
	SPI_CQueueGlobalUnLock(HwChannel);
    if(E_NOT_OK == result)
    {
        
        FREE(msg);
		
        return IPC_GW_ERR_SEND_IPC;
    }
    FREE(msg);
	
	#if   1
	if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
	{
		SpiMasterReq(HwChannel);
		UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
	}
	#endif
	#if   0
    if(E_NOT_OK == result)
    {
        
        FREE(msg);
        return IPC_GW_ERR_SEND_IPC;
    }
    FREE(msg);
	#endif
    return IPC_GW_ERR_OK;
}

error_type ipc_gw_send_request_no_reply(
	uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    uint32 len)
{
    msg_head head;
    uint8 *msg;
    uint16 tx_len;
    IpcCoreErr result;
	uint8 l_reqFlag = 1;
	uint16 qlen = 0;

    
    head.type = IPC_GW_REQ_NO_REPLY;
    head.sequence_number = 0;
    head.function_id = function;

     if(Spi_MComFLag == FALSE)
	{
		return IPC_GW_ERR_SEND_IPC;
	}
	
    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    
    if(data == NULL || len == 0)
    {
        tx_len  = sizeof(msg_head);
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        	memcpy(msg, &head, sizeof(msg_head));
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }
    else
    {
        tx_len  = sizeof(msg_head) + len;
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        	memcpy(msg, &head, sizeof(msg_head));
	        	memcpy(msg+sizeof(msg_head), data, len);
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }

    
    
	SPI_CQueueGlobalLock(HwChannel);
	SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		l_reqFlag = 0;
	}
	result = SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, msg,tx_len);
	SPI_CQueueGlobalUnLock(HwChannel);
	if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
	{
		SpiMasterReq(HwChannel);
		UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
	}
	
    if(E_NOT_OK == result)
    {
        
        FREE(msg);
        return IPC_GW_ERR_SEND_IPC;
    }
    FREE(msg);

    return IPC_GW_ERR_OK;
}

error_type ipc_gw_send_reply(
	uint8 HwChannel,
    request_context_t *context,
    const uint8 *data,
    uint32 len)
{
    msg_head head;
    uint8 *msg;
    uint16 tx_len;
    IpcCoreErr result;
	uint32 channel;
	uint8 l_reqFlag = 1;
	uint16 qlen = 0;

    if(NULL == context || context->sequence_number == 0)
    {
       
        return IPC_GW_ERR_NO_PARAMETER;
    }

	if(Spi_MComFLag == FALSE)
	{
		return IPC_GW_ERR_SEND_IPC;
	}
	
    
    head.type = IPC_GW_RSP;
    head.sequence_number = context->sequence_number;
    head.function_id = context->function_id;
    channel = context->channel;

    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    
    if(data == NULL || len == 0)
    {
        tx_len  = sizeof(msg_head);
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
	        memcpy(msg, &head, sizeof(msg_head));
		}
		else
		{
			  
			 return IPC_GW_ERR_SEND_IPC;
		}
    }
    else
    {
        tx_len  = sizeof(msg_head) + len;
        msg = (uint8 *)MALLOC(tx_len);
		if(msg != NULL)
		{
       	 	memcpy(msg, &head, sizeof(msg_head));
        	memcpy(msg+sizeof(msg_head), data, len);
		}
		else
		{
			 return IPC_GW_ERR_SEND_IPC;
		}
    }

    
	
    
    SPI_CQueueGlobalLock(HwChannel);
	SPI_CQueue_GetTotalLen(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), &qlen);
	if(qlen > 0)
	{
		l_reqFlag = 0;
	}
	result = SPI_CQueue_Push(HwChannel,SPI_CQueueGetTxManagerHandle(HwChannel), channel, msg,tx_len);
	SPI_CQueueGlobalUnLock(HwChannel);
	if(E_NOT_OK == result)
	{
		
		FREE(msg);
		
		return IPC_GW_ERR_SEND_IPC;
	}

    FREE(msg);
    
		#if   1
		if((l_reqFlag != 0) || ((xTaskGetTickCount() - GetReqTickCnt(HwChannel)) > SPI_REQ_INTERVAL))
		{
			SpiMasterReq(HwChannel);
			UpdateReqTickCnt(HwChannel, xTaskGetTickCount());
		}
		#endif

    return IPC_GW_ERR_OK;
}

error_type ipc_gw_receive_msg (
    uint32 channel,
    const uint8* buf,
    uint32 len)
{
    uint8 *data;
    uint16 data_len;
    msg_head head;
    error_type result = IPC_GW_ERR_OK;

    if(len < (uint32)sizeof(msg_head))
    {
        return IPC_GW_ERR_INVALID_MESSAGE;
    }

    
    memcpy(&head, buf, sizeof(msg_head));
    data_len = len - sizeof(msg_head);
    if(data_len <= 0)
    {
        data = NULL;
        data_len = 0;
    }
    else
    {
        data = (uint8 *)MALLOC(data_len);
		if(data != NULL)
		{
        		memcpy(data, buf+sizeof(msg_head), data_len);
		}
		else
		{
		 	return IPC_GW_ERR_SEND_IPC;
		}
    }

    if(is_cpu_big_endian())
    {
        byte_swap(&(head.type), sizeof(msg_type));
        byte_swap(&(head.sequence_number), sizeof(uint16));
        byte_swap(&(head.function_id), sizeof(uint16));
    }

    switch(head.type)
    {
	    case IPC_GW_RSP:
	    {
	        
	        char key[SEQUENCE_NUMBER_CHAR_LEN+1];
	        invoke_element el;
	        error_type ret;
	        sprintf(key, SN_FORMAT_STRING, head.sequence_number);
	        dump_element();
	        ret = remove_element(key, &el);
	        dump_element();

	        
	        if(ret == IPC_GW_ERR_OK)
	        {
	        	if(el.cb!=NULL)
	            	el.cb(el.context, head.function_id, data, data_len);
	        }
	        else
	        {
	            result = IPC_GW_ERR_NO_CALLBACK;
	        }
	    }
	    break;

	    case IPC_GW_REQ:
	    {
	        
	        request_callback callback;
			#if 0
	        request_context_t *context;
	        context = (request_context_t *)MALLOC(sizeof(request_context_t));
	        context->channel = channel;
	        context->sequence_number = head.sequence_number;
	        context->function_id = head.function_id;
			#else
			request_context_t context;
			context.channel = channel;
			context.sequence_number = head.sequence_number;
			context.function_id = head.function_id;

			#endif
	        
	        callback = NULL;
	        get_request_callback(channel, &callback);
	        if(callback)
	        {
	            (*callback)(&context, data, data_len);
	        }
	        else
	        {
	            result = IPC_GW_ERR_NO_CALLBACK;
	        }
	    }
	    break;

	    case IPC_GW_REQ_NO_REPLY:
	    {
	        
	        request_callback callback;
	        request_context_t context;

	        context.channel = channel;
	        context.sequence_number = head.sequence_number;
	        context.function_id = head.function_id;

	        
	        callback = NULL;
	        get_request_callback(channel, &callback);
	        if(callback)
	        {
	            (*callback)(&context, data, data_len);
	        }
	        else
	        {
	            result = IPC_GW_ERR_NO_CALLBACK;
	        }
	    }
	    break;

	    case IPC_GW_NOTIFY:
	    {
	        
	        notification_callback callback;
	        callback = NULL;
	        get_notification_callback(channel, &callback);
	        if(callback)
	        {
	            (*callback)(head.function_id, data, data_len);
	        }
	        else
	        {
	            result = IPC_GW_ERR_NO_CALLBACK;
	        }
	    }
	    break;
	    default:
	        result = IPC_GW_ERR_INVALID_MESSAGE;
	        break;
    }
    FREE(data);
    return result;
}

IpcCoreErr ipcChannelRecv(int channel, const uint8* buf, int len)
{
    error_type ret;

    ret = ipc_gw_receive_msg(channel, buf, len);
    if(IPC_GW_ERR_OK != ret)
    {
        
        return IPC_ERR_OTHER_ERRORS;
    }
    else
    {
        return IPC_ERR_OK;
    }

    return IPC_ERR_OK;

}


void ipc_gw_init()
{
    
    
}

void ipc_gw_init_M()
{
    ipcRegisterChannelRecv((ipcChannelRecvFn)ipcChannelRecv);
    init_core();
}


