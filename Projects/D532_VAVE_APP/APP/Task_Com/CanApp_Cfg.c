/**
* @file:    CanApp_Cfg.c
* @brief:   Add your description here for this file.
* @author:  zhangyi
* @date:    2018-09-11 10:40:16
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  zhangyi  2018-09-11 10:40:16  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "Std_Types.h"
#include "CanApp_Cfg.h"

#include "Com_If.h"


/*----------------------------- START: USER INCLUDED ----------------------------*/
//uint8 CanApp_Tx_NeedCount[COM_TX_IPDU_MAX_NUM] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3};
//uint8 CanApp_Tx_NeedCount[COM_TX_IPDU_MAX_NUM] = {1, 1, 1, 3, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 1, 3, 1, 3, 1};
uint8 CanApp_Tx_NeedCount[COM_TX_IPDU_MAX_NUM] = {1, 1, 1, 3, 1, 1, 1, 3, 3, 3, 3, 3, 1, 3, 1, 3, 1, 3, 1};



void Byte_to_Float(float32 *f,uint8 *byte,uint32 startbit)
{
  FloatLongType fl;
  fl.ldata = 0;
  fl.ldata = byte[startbit + 3];
  fl.ldata = (fl.ldata<<8) | byte[startbit+2];
  fl.ldata = (fl.ldata<<8)| byte[startbit+1];
  fl.ldata = (fl.ldata<<8)| byte[startbit];
  *f = fl.fdata;
}

/*------------------------------ END: USER INCLUDE ------------------------------*/


/*============================= END OF FILE: CanApp_Cfg.c ============================*/
