

#ifndef __IPC_GATEWAY_ADAPTOR_H_
#define __IPC_GATEWAY_ADAPTOR_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "Std_Types.h"

extern void *ipc_gw_malloc(unsigned int size);
extern void *ipc_gw_calloc(unsigned int num, unsigned int size);
extern void ipc_gw_free(void *ptr);


#define MALLOC  ipc_gw_malloc
#define CALLOC  ipc_gw_calloc
#define FREE    ipc_gw_free


#if defined(__cplusplus)
}
#endif

#endif /* __IPC_GATEWAY_ADAPTOR_H_ */

