/**
*@file  PriQueue_Cfg.h
*@brief Describe here.
*@date    2018-03-23 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-23  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _PRIQUEUE_CFG_H_
#define _PRIQUEUE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*===============================================================================*/
/*============================= START: USER INCLUDED ============================*/
/*===============================================================================*/
//#include "example.h"

/*===============================================================================*/
/*============================== END: USER INCLUDE ==============================*/
/*===============================================================================*/

/*===============================================================================*/
/*========================== START: USER CONFIGURATION ==========================*/
/*===============================================================================*/
#define PRIQUEUE_DEBUG_TRACE STD_OFF

#define PRIQUEUE_LENGTH_DTC 5

typedef enum _PriQueue_ChannelEType
{
    PRIQUEUE_CH_DTC,
    PRIQUEUE_CH_MAX
} PriQueue_ChannelEType;

/*===============================================================================*/
/*============================ END: USER CONFIGURATION ==========================*/
/*===============================================================================*/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_PRIQUEUE_CFG_H_
/*=========================== END OF FILE: PriQueue_Cfg.h ===========================*/
