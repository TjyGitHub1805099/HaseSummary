

/*---------------------------------------------------------------------
 * INCLUDES
 *--------------------------------------------------------------------*/
#include "ipc_gw_adaptor.h"
#include <string.h>
#include "System_Portable.h"

void *ipc_gw_malloc(unsigned int size)
{
    void * ptr = NULL;
    if(size < 20)
    {
        ptr = Mem_Malloc(20);
        return ptr;
    }
    else
    {
        ptr = Mem_Malloc(size);
        return ptr;
    }
}

void *ipc_gw_calloc(unsigned int num, unsigned int size)
{
    unsigned int actual_size;
    void * ptr = NULL;

    actual_size = num*size;
    if(actual_size < 20)
    {
        ptr = Mem_Malloc(20);
        memset(ptr, 0, actual_size);
        return ptr;
    }
    else
    {
        ptr = Mem_Malloc(actual_size);
        memset(ptr, 0, actual_size);
        return ptr;
    }
}

void ipc_gw_free(void *ptr)
{
    Mem_Free (ptr);
}

