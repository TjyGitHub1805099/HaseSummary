/*
* File last modified by:   dukunlin
* File last modified time: 2018-05-09 09:53:02
*/

/**
* @file      BinarySearch.h
* @author    Stephen Du
* @date:     2018-05-09 09:48:13
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-05-09 09:48:13  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _BINARYSEARCH_H_
#define _BINARYSEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
/*============================= START: USER INCLUDED ============================*/

/*============================== END: USER INCLUDE ==============================*/

typedef enum
{
    BSEARCH_BUFTYPE_UINT8,
    BSEARCH_BUFTYPE_UINT16,
    BSEARCH_BUFTYPE_UINT32,
    BSEARCH_BUFTYPE_UINT64,
} BinSearchBufType;

#define BINSEARCH_ERROR 0xFFFF

extern uint16 BinSearch_EquMatch(void* bufPtr, uint16 bufLen, BinSearchBufType bufType, uint32 value);
extern uint16 BinSearch_Interval(void* bufPtr, uint16 bufLen, BinSearchBufType bufType, uint32 value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_BINARYSEARCH_H_
/*============================= END OF FILE: BinarySearch.h ============================*/
