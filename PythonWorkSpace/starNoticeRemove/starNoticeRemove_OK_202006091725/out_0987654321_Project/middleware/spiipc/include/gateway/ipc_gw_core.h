#ifndef __IPC_GW_CORE_H
#define __IPC_GW_CORE_H

//#include "hashmap.h"
#include "msg_def.h"
#include "ipcMutex.h"
#include "SpiIpc.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct
{
    char key[SEQUENCE_NUMBER_CHAR_LEN+1];
    const void *context;
    invoke_callback cb;
} invoke_element;

typedef struct
{
    char key[CHANNEL_NUMBER_CHAR_LEN+1];
    notification_callback notification_cb;
    request_callback request_cb;
} registry_element;

//#include "osif.h"
//typedef mutex_t MUTEX;

extern void init_core();
extern void deinit_core();
extern error_type insert_element(const void *ctx, invoke_callback cb, uint16 *seq_number);
extern error_type remove_element(char*key, invoke_element * out);
extern void dump_element(void);
//extern hmap_t get_map(void);

extern error_type register_callback(uint32 channel, notification_callback ntf_cb, request_callback req_cb);
extern error_type unregister_callback(uint32 channel);
extern error_type insert_registry(uint32 channel, notification_callback ntf_cb, request_callback req_cb);
extern error_type remove_registry(char*key, registry_element * out);
extern void dump_registry(void);
//extern hmap_t get_channle_map(void);

extern void get_notification_callback(uint32 channel, notification_callback *cb);
extern void get_request_callback(uint32 channel, request_callback *cb);
extern int is_cpu_big_endian();
extern void byte_swap(void *word, int len);

#if defined(__cplusplus)
}
#endif

#endif

