

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "ipcCorePlatformConfig.h"
#include "hashmap.h"
#include "ipc_gw_core.h"
#include "spiMutex.h"
#include "ipcMutex.h"
#include "ipc_gw_adaptor.h"

static int g_is_bigendian = 0;
static hmap_t g_ctx_map = NULL;
static hmap_t g_channel_map = NULL;
static MUTEX ctx_map_mutext;
 hmap_t get_map(void);
 hmap_t get_channle_map(void);
 
#define CORE_ELEMENT

void ipc_gw_debug_common(
    const char *fmt,
    uint32 para)
{
	(void)fmt;
	(void)para;
}


static sint32 iter_elem(void* el, void *arg)
{
	(void)el;
	(void)arg;
    //invoke_element *elem = (invoke_element *) el;
    //printf("key=%s; context=%d, callback=%d\n\r", elem->key, (uint32)elem->context, (uint32)elem->cb);
    return 0;
}

static sint32 free_elem(void* el, void *arg)
{
	(void)arg;
    invoke_element *elem = (invoke_element *) el;
    //printf("free elem =%s\n\r", elem->key);
    FREE(elem);
    return 0;
}

static uint16 create_unique_number(void)
{
    static uint16 seq_number = 0;

    if(seq_number > SEQUENCE_NUMBER_MAX_VALUE)
    {
        seq_number = 1;
    }
    else
    {
        seq_number++ ;
    }

    return seq_number;
}

error_type insert_element(const void *ctx, invoke_callback cb, uint16 *seq_number)
{
    uint32 ret;
    uint16 sn;
    invoke_element *elem;
    /*create element.*/
    elem = (invoke_element *)MALLOC(sizeof(invoke_element));
    if(NULL == elem)
    {
        return IPC_GW_ERR_MEMORY_ALLOC;
    }
    sn = create_unique_number();
    sprintf(elem->key, SN_FORMAT_STRING, sn);
    elem->context = ctx;
    elem->cb= cb ;
    *seq_number = sn;

    /*insert element to hashmap.*/
    mutexLock(&ctx_map_mutext);
    ret= hashmap_put(get_map(), elem->key, elem);
    mutexUnlock(&ctx_map_mutext);
    if(ret != HMAP_S_OK)
    {
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]insert invoke_element failed : %d\n\r", ret);
#endif
        return IPC_GW_ERR_HASH_MAP;
    }

    return IPC_GW_ERR_OK;
}

error_type remove_element(char*key, invoke_element * out)
{
    uint32 ret;
    void_ptr p;
    invoke_element *el;
    /*remove element from hashmap.*/
    mutexLock(&ctx_map_mutext);
    ret = hashmap_remove(get_map(), (char*)key, &p);
    mutexUnlock(&ctx_map_mutext);
    if(ret != HMAP_S_OK)
    {
        (*out).context = NULL;
        (*out).cb= NULL;
        memset((*out).key, 0, SEQUENCE_NUMBER_CHAR_LEN);
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]remove invoke_element failed : %d\n\r", ret);
#endif
        return IPC_GW_ERR_HASH_MAP;
    }

    /*fill the output value and free the element.*/
    el = (invoke_element *)p;
    (*out).context = el->context;
    (*out).cb= el->cb;
    memset((*out).key, 0, SEQUENCE_NUMBER_CHAR_LEN);
    memcpy((*out).key, el->key, strlen(el->key));
    free_elem(el, 0);

    return IPC_GW_ERR_OK;
}

void dump_element(void)
{
    hashmap_iterate(get_map(), iter_elem, 0);
}

#define CORE_REGISTRY
static sint32 iter_registry(void* el, void *arg)
{
	(void)el;
	(void)arg;
    //registry_element *elem = (registry_element *) el;
    //printf("key=%s; notification_cb=%d, request_cb=%d\n\r", elem->key, (uint32)elem->notification_cb, (uint32)elem->request_cb);
    return 0;
}

static sint32 free_registry(void* el, void *arg)
{
	(void)arg;
    registry_element *elem = (registry_element *) el;
    //printf("free elem =%s\n\r", elem->key);
    FREE(elem);
    return 0;
}

error_type insert_registry(uint32 channel, notification_callback ntf_cb, request_callback req_cb)
{
    uint32 ret;
    /*create element.*/
    registry_element *elem;
    elem = (registry_element *)MALLOC(sizeof(registry_element));
    if(NULL == elem)
    {
        return IPC_GW_ERR_MEMORY_ALLOC;
    }
    sprintf(elem->key, CHANNEL_FORMAT_STRING, channel);
    elem->notification_cb = ntf_cb;
    elem->request_cb= req_cb;

    /*insert element to hashmap.*/
    ret = hashmap_put(get_channle_map(), elem->key, elem);
    if(ret != HMAP_S_OK)
    {
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]insert insert_registry failed : %d\n\r", ret);
#endif
        return IPC_GW_ERR_HASH_MAP;
    }

    return IPC_GW_ERR_OK;
}

error_type remove_registry(char*key, registry_element * out)
{
    uint32 ret;
    void_ptr p;
    registry_element *el;
    /*remove element from hashmap.*/
    ret = hashmap_remove(get_channle_map(), (char*)key, &p);
    if(ret != HMAP_S_OK)
    {
        (*out).notification_cb = NULL;
        (*out).request_cb= NULL;
        memset((*out).key, 0, CHANNEL_NUMBER_CHAR_LEN);
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]remove remove_registry failed : %d\n\r", ret);
#endif
        return IPC_GW_ERR_HASH_MAP;
    }

    /*fill the output value and free the element.*/
    el = (registry_element *)p;
    (*out).notification_cb = el->notification_cb;
    (*out).request_cb = el->request_cb;
    memset((*out).key, 0, CHANNEL_NUMBER_CHAR_LEN);
    memcpy((*out).key, el->key, strlen(el->key));
    free_registry(el, 0);

    return IPC_GW_ERR_OK;
}

void dump_registry(void)
{
    hashmap_iterate(get_channle_map(), iter_registry, 0);
}

error_type register_callback(uint32 channel, notification_callback ntf_cb, request_callback req_cb)
{
    return insert_registry(channel, ntf_cb, req_cb);
}

error_type unregister_callback(uint32 channel)
{
    char key[CHANNEL_NUMBER_CHAR_LEN+1];
    registry_element el;
    sprintf(key, CHANNEL_FORMAT_STRING, channel);
    return remove_registry(key, &el);
    //dump_registry();
}

#define CORE_COMMON
static int check_endian()
{
    unsigned i = 1;
    char *c;
    c = (char*)&i;
    return (c[0] == 1 ? 0 : 1);
}

void init_core()
{
    g_is_bigendian = check_endian();
    g_ctx_map = hashmap_create();
    g_channel_map = hashmap_create();
    mutexCreate(&ctx_map_mutext);

#ifdef IPC_PLTF_IS_QNX
    ipc_gw_debug_common("[ipcgw core]init ipc gateway core success. It is big endian:%d.\n\r", g_is_bigendian);
#endif
}

void deinit_core()
{
    hashmap_destroy(g_channel_map, free_elem, 0);
    hashmap_destroy(g_ctx_map, free_elem, 0);
    mutexDestroy(&ctx_map_mutext);

#ifdef IPC_PLTF_IS_QNX
    ipc_gw_debug_common("[ipcgw core]deinit ipc gateway core success. ctx:%d\n\r", (uint32)g_ctx_map);
#endif
}

hmap_t get_map(void)
{
    return g_ctx_map;
}

hmap_t get_channle_map(void)
{
    return g_channel_map;
}


void get_notification_callback(uint32 channel, notification_callback *cb)
{
    uint32 ret;
    char key[CHANNEL_NUMBER_CHAR_LEN+1];
    void_ptr p;
    registry_element *el;

    sprintf(key, CHANNEL_FORMAT_STRING, channel);
    ret = hashmap_get(get_channle_map(), (const char *)&key, &p);
    if(ret != HMAP_S_OK)
    {
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]get callback failed : %d", ret);
#endif
        return ;
    }

    el = (registry_element *)p;
    *cb = el->notification_cb;
}

void get_request_callback(uint32 channel, request_callback *cb)
{
    uint32 ret;
    char key[CHANNEL_NUMBER_CHAR_LEN+1];
    void_ptr p;
    registry_element *el;

    sprintf(key, CHANNEL_FORMAT_STRING, channel);
    ret = hashmap_get(get_channle_map(), (const char *)&key, &p);
    if(ret != HMAP_S_OK)
    {
#ifdef IPC_PLTF_IS_QNX
        ipc_gw_debug_common("[ipcgw core]get callback failed : %d", ret);
#endif
        return ;
    }

    el = (registry_element *)p;
    *cb = el->request_cb;
}

int is_cpu_big_endian()
{
    return g_is_bigendian;
}

void byte_swap(void *word, int len)
{
    int i;
    char c, *w;
    w = (char*)word;
    for(i=0; i<len/2; i++)
    {
        c = w[i];
        w[i] = w[len-1-i];
        w[len-1-i] = c;
    }
}


