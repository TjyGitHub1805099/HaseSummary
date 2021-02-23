/**
*@file  PriQueue_Cfg.c
*@brief Describe here.
*@date    2018-03-29 15:10:14
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2018-03-29  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "Std_Types.h"
#include "PriQueue.h"

/*===============================================================================*/
/*============================= START: USER INCLUDED ============================*/
/*===============================================================================*/
//#include "example.h"

/*===============================================================================*/
/*============================== END: USER INCLUDE ==============================*/
/*===============================================================================*/
const uint8 PriQueue_MaxChannel = PRIQUEUE_CH_MAX;
PriQueueManageType PriQueueManage[PRIQUEUE_CH_MAX];
/*===============================================================================*/
/*========================== START: USER CONFIGURATION ==========================*/
/*===============================================================================*/
PriQueue_BufDef(Dtc, PRIQUEUE_LENGTH_DTC);

const PriQueueCfgType PriQueueCfg[] = //don't change this line.
{
    {PRIQUEUE_LENGTH_DTC, PriQueue_GetFlagBufName(Dtc), &PriQueue_GetHeadBufName(Dtc), PriQueue_GetNodeBufName(Dtc)},
};
/*===============================================================================*/
/*============================ END: USER CONFIGURATION ==========================*/
/*===============================================================================*/

/*========================= END OF FILE: PriQueue_Cfg.c =========================*/
