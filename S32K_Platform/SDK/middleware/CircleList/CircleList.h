/**
*@file  circleList.h
*@brief Describe here.
*@date    2019-03-25 18:19:28
*@author  liqi@hangsheng.com
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-23  	liqi       First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/

#ifndef _CIRCLE_LIST_H
#define _CIRCLE_LIST_H
#include "std_types.h"
#include "string.h"

typedef struct circleList{
	struct circleList *next;
	uint8 *dataPtr;
}tCircleListItem;

typedef struct{
	tCircleListItem *head;
	tCircleListItem *tail;
	tCircleListItem *array;
	uint8 dataBuffMaxLen;
}tCircleListInfor;

extern void* ListMemMalloc(uint16 len);
extern void ListMemFree(void *addr);

void circleListDeinit(tCircleListInfor *listInfor,uint8 listNum);
boolean circleListInit(tCircleListInfor *listInfor,uint8 itemBufferLen,uint8 listNum);
boolean circleListInsert(tCircleListInfor *listInfor,uint8 *data,uint8 dataLen);
boolean circleListPop(tCircleListInfor *listInfor,uint8 *buff,uint8 buffLen);
void circleListClear(tCircleListInfor *listInfor);
boolean isCircleListEmpty(tCircleListInfor *listInfor);

#endif
