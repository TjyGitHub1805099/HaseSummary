/**
* @file:    CircularQueue_Cfg.h
* @brief:   Add your description here for this file.
* @author:  Stephen Du
* @date:    2018-06-20 20:31:04
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  Stephen Du  2018-06-20 20:31:04  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CIRCULARQUEUE_CFG_H_
#define _CIRCULARQUEUE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/

/*------------------------------ END: USER INCLUDE ------------------------------*/

/*------------------------------ START: USER CONFIG -----------------------------*/
#define CQUEUE_CHANNEL_MAX  32

/*-------------------------------- END: USER CONFIG -----------------------------*/

#if (CQUEUE_CHANNEL_MAX >= 255)
#error "Circular queue max channel should less than 256!"
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CIRCULARQUEUE_CFG_H_
/*============================= END OF FILE: CircularQueue_Cfg.h ============================*/
