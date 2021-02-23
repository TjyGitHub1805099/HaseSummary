#include "CircleList.h"
#include <string.h>

extern void* ListMemMalloc(uint16 len);
extern void ListMemFree(void *addr);



void circleListDeinit(tCircleListInfor *listInfor)
{
	uint16 i = 0;
	if(listInfor && listInfor->array)
	{
		for(i=0;i<listInfor->listNum;i++)
		{
			if(listInfor->array[i].dataPtr!=NULL)
				ListMemFree(listInfor->array[i].dataPtr);
		}
	}
}


boolean circleListInit(tCircleListInfor *listInfor,uint8 itemBufferLen,uint16 listNum)
{
	uint16 i = 0;
	boolean ret = TRUE;
	
	if(listInfor && listInfor->array && itemBufferLen >0 && listNum>=2)
	{
		listInfor->listNum = listNum;
		for(i=0;i<listNum;i++)
			listInfor->array[i].dataPtr = NULL;
		
		for(i=0;i<listNum;i++)
		{
			if((i+1) < listNum)//not last item
				listInfor->array[i].next = &(listInfor->array[i+1]);
			else
				listInfor->array[i].next = &(listInfor->array[0]);//last item points to the 1th

			
			listInfor->array[i].dataPtr = (uint8*)ListMemMalloc(itemBufferLen);
			if(listInfor->array[i].dataPtr == NULL)
			{
				circleListDeinit(listInfor);
				ret = FALSE;
				break;
			}
		}
		
	}
	else
		ret = FALSE;
	
	if(ret == TRUE)
	{
		listInfor->head = &listInfor->array[0];
		listInfor->tail = listInfor->head;
		listInfor->itemBufferLen = itemBufferLen;
	}
	return ret;
}


boolean circleListInsert(tCircleListInfor *listInfor,uint8 *data,uint8 dataLen)
{
	boolean ret = FALSE;
	
	if(listInfor && listInfor->head && data
		&& listInfor->head->next
		&& listInfor->tail 
		&&(listInfor->head == listInfor->tail
		|| listInfor->head->next != listInfor->tail) 
		&& dataLen<=listInfor->itemBufferLen)
	{
		memcpy(listInfor->head->dataPtr,data,dataLen);
		listInfor->head = listInfor->head->next;
		ret = TRUE;
	}
	else
		ret = FALSE;

	return ret;
}


boolean circleListPop(tCircleListInfor *listInfor,uint8 *buff,uint8 buffLen)
{
	boolean ret = FALSE;
	
	if(listInfor && listInfor->tail && buff
		&& listInfor->head 
		&& listInfor->head != listInfor->tail
		&& buffLen>=listInfor->itemBufferLen)
	{
		memcpy(buff,listInfor->tail->dataPtr,listInfor->itemBufferLen);
		listInfor->tail = listInfor->tail->next;
		ret = TRUE;
	}

	return ret;
}

void circleListClear(tCircleListInfor *listInfor)
{
	listInfor->head = &listInfor->array[0];
	listInfor->tail = listInfor->head;
}

boolean isCircleListEmpty(tCircleListInfor *listInfor)
{
	boolean ret = TRUE;
	if(listInfor && listInfor->tail && listInfor->head && listInfor->tail != listInfor->head)
		ret = FALSE;

	return ret;
}

