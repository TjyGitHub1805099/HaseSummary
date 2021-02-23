#ifndef __MESSAGE_DEFINE_H
#define __MESSAGE_DEFINE_H

/**
* @file msg_def.h
* @ingroup system
* @author Wuwen Jiang
* Message definition for service
*/

#include "Platform_Types.h"
//#include "ipc_gw_adaptor.h"
#define SEQUENCE_NUMBER_MAX_VALUE 65534
#define SEQUENCE_NUMBER_CHAR_LEN 8
#define SN_FORMAT_STRING "%06d"
#define CHANNEL_NUMBER_CHAR_LEN 3
#define CHANNEL_FORMAT_STRING "%03d"
#define RESERVED_CHANNEL_COUNT  1

typedef enum 
{
    IPC_GW_REQ = 0,
    IPC_GW_REQ_NO_REPLY,
    IPC_GW_RSP,
    IPC_GW_NOTIFY
}msg_type;

typedef enum 
{
	IPC_GW_ERR_OK = 0, /*success*/
	IPC_GW_ERR_INVALID_MESSAGE,/*IPC Message content is invalid*/
	IPC_GW_ERR_MEMORY_ALLOC, /*Allocate memory failed*/
	IPC_GW_ERR_SEND_IPC, /*Send IPC message but get errors*/
	IPC_GW_ERR_HASH_MAP, /*Operate the inside hash map error*/
	IPC_GW_ERR_NO_CALLBACK, /*Can't find the callback function*/
	IPC_GW_ERR_NO_PARAMETER, /*The parameter is NULL when reply*/
	IPC_GW_ERR_QUEUE_FULL
}error_type;

typedef struct
{
	uint16 type;
	uint16 function_id;
	uint16 sequence_number;
}msg_head;

typedef struct
{
	uint16 type;
	uint16 sequence_number;
	uint16 function_id;
	uint16 packet_feature;
	uint16 msg_len;
}msg_Ic_head;

typedef struct
{
	uint32 channel;
	uint16 function_id;
	uint16 sequence_number;
}request_context_t;

//typedef void (*invoke_callback)(const void *ctx, uint16 function, uint8 *rx_data,uint32 len);
typedef void (*notification_callback)(uint16 function, uint8 *rx_data, uint32 len);
typedef void (*request_callback)(const void *ctx, uint8 *rx_data, uint32 len);

typedef void (*sdl_callback)(uint8 *rx_data, uint32 len);

#endif /* __MESSAGE_DEFINE_H */

