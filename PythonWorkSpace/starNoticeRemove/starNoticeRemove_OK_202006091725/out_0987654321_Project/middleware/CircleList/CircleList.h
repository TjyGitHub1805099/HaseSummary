#ifndef _CIRCLE_LIST_H
#define _CIRCLE_LIST_H
#include "Std_Types.h"
#include "System_Portable.h"

typedef struct circleList{
	struct circleList *next;
	uint8 *dataPtr;
}tCircleListItem;

typedef struct{
	tCircleListItem *head;
	tCircleListItem *tail;
	tCircleListItem *array;
	uint8 itemBufferLen;
	uint16 listNum;
}tCircleListInfor;

extern void* ListMemMalloc(uint16 len);
extern void ListMemFree(void *addr);

void circleListDeinit(tCircleListInfor *listInfor);
boolean circleListInit(tCircleListInfor *listInfor,uint8 itemBufferLen,uint16 listNum);
boolean circleListInsert(tCircleListInfor *listInfor,uint8 *data,uint8 dataLen);
boolean circleListPop(tCircleListInfor *listInfor,uint8 *buff,uint8 buffLen);
void circleListClear(tCircleListInfor *listInfor);
boolean isCircleListEmpty(tCircleListInfor *listInfor);

#endif
