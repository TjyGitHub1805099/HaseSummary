#include "CircleList.h"
#include "Std_Types.h"
#include "System_Portable.h"



void* ListMemMalloc(uint16 len)
{
	return Mem_Malloc(len);
	
}

void ListMemFree(void *addr)
{
	Mem_Free(addr);
}