#include"circleList.h"

extern void* ListMemMalloc(uint16 len);
extern void ListMemFree(void *addr);
#if 0
void circleListInit(tCircleList *head,uint8 dataBufferLen,uint8 listLen)
{
	uint8 i = 0;
	tCircleList *listItem = head;
	tCircleList *listNext = NULL;
	
	if(listItem!=NULL && dataBufferLen >0 && listLen>0)
	{
		for(i=0;i<listLen;i++)
		{
			listItem->dataPtr = (uint8*)pvPortMalloc(dataBufferLen);
			if(listItem->dataPtr != NULL)
			{
				listNext = (uint8*)pvPortMalloc(sizeof(tCircleList));
				if(listNext!=NULL)
				{
					
				}
			}
		}
	}
}
#else


void circleListDeinit(tCircleListInfor *listInfor,uint8 listNum)
{
	uint8 i = 0;
	if(listInfor && listInfor->array)
	{
		for(i=0;i<listNum;i++)
		{
			if(listInfor->array[i].dataPtr!=NULL)
				ListMemFree(listInfor->array[i].dataPtr);
		}
	}
}


boolean circleListInit(tCircleListInfor *listInfor,uint8 itemBufferLen,uint8 listNum)
{
	uint8 i = 0;
	boolean ret = TRUE;
	
	if(listInfor!=NULL && itemBufferLen >0 && listNum>=2)
	{
		for(i=0;i<listNum;i++)
		{
			if((i+1) < listNum)//not last item
				listInfor->array[i].next = &(listInfor->array[i+1]);
			else
				listInfor->array[i].next = &(listInfor->array[0]);//last item points to the 1th

			listInfor->array[i].dataPtr = NULL;
			listInfor->array[i].dataPtr = (uint8*)ListMemMalloc(itemBufferLen);
			if(listInfor->array[i].dataPtr == NULL)
			{
				circleListDeinit(listInfor,i+1);
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
		listInfor->tail = &listInfor->array[listNum-1];//last one
	}
	return ret;
}

boolean circleListInsert(tCircleListInfor *listInfor,uint8 *data,uint8 dataLen)
{
	boolean ret = FALSE;
	
	if(listInfor && listInfor->head && data
		&& listInfor->head->next
		&& listInfor->tail 
		&& listInfor->head->next != listInfor->tail 
		&& dataLen<=listInfor->dataBuffMaxLen)
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
		&& listInfor->tail->next
		&& listInfor->head 
		&& listInfor->tail->next != listInfor->head 
		&& buffLen>=listInfor->dataBuffMaxLen)
	{
		memcpy(buff,listInfor->tail->dataPtr,buffLen);
		listInfor->tail = listInfor->tail->next;
		ret = TRUE;
	}

	return ret;
}

void circleListClear(tCircleListInfor *listInfor)
{
	listInfor->head = &listInfor->array[0];
	listInfor->tail = &listInfor->array[listInfor->dataBuffMaxLen-1];
}

boolean isCircleListEmpty(tCircleListInfor *listInfor)
{
	boolean ret = TRUE;
	if(listInfor && listInfor->tail && listInfor->head && listInfor->tail->next != listInfor->head)
		ret = FALSE;

	return ret;
}
#endif
