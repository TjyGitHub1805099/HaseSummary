/**
*@file  CircularQueue_Cfg.h
*@brief Containing the entire or parts of Circular Queue code.
*@date    2017-10-26 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-10-26  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _CIRCULAR_QUEUE_CFG_H_
#define _CIRCULAR_QUEUE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

typedef enum
{
    CQUEUE_CHANNEL_0,
    CQUEUE_CHANNEL_1,
    CQUEUE_CHANNEL_2,
    CQUEUE_CHANNEL_3,
    CQUEUE_CHANNEL_4,
    CQUEUE_CHANNEL_5,
    CQUEUE_CHANNEL_MAX,
} CQueue_Channel_EType;

#define CQUEUE_CH_CANPDUCHANGE CQUEUE_CHANNEL_5
#define CQUEUE_CH_TRACE CQUEUE_CHANNEL_0

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_CIRCULAR_QUEUE_CFG_H_
/*=========================== END OF FILE: CircularQueue_Cfg.h ===========================*/
