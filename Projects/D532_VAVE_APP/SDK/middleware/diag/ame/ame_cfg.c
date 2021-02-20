/**
* @file:    ame_Cfg.c
* @brief:   Add your description here for this file.
* @author:  qiweifeng
* @date:    2018-09-29 17:40:29
*
* @copyright None
* @warning   All rights reserved.
*
* @par Version Histroy
<pre><b>
Version:   Author:       Date&&Time:      Revision Log: </b>
 V1.0.0  qiweifeng  2018-09-29 17:40:29  First Creat
When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#include "Std_Types.h"
/*----------------------------- START: USER INCLUDED ----------------------------*/
#include "ame_cbk.h"
#include "ame_cfg.h"
/*------------------------------ END: USER INCLUDE ------------------------------*/



uint8 gUnlockAmeKey[UNLOCK_AME_DATA_LEN] = {0xB0,0xC1,0xDF,0x2E,0x5A,0x46,0x73,0x15,0x69,0xA8};



#if 0
/*************************************************************************************************
* Before writing real data, you need to write the following 3 arrays.
*************************************************************************************************/
const uint8 array1[] = {0x31,0x32,0x33,0x34};   
const uint8 array2[] = {0x35,0x36,0x37,0x38};   
const uint8 array3[] = {0x39,0x40,0x41,0x42};


uint16 gArray1len = sizeof(array1);
uint16 gArray2len = sizeof(array2);
uint16 gArray3len = sizeof(array3);

#endif


/************************************************************************************************/
const tAmeSubServiceFnCbk gAmeSubServiceFnCbk[] = {
	{SID_READ_DATA_BY_DID,  ameReadData},
	{SID_WRITE_DATA_BY_DID, ameWriteData},
	{SID_IOCONTROL,         ameIOControl},
	{SID_SECURITY_ACCESS,   ameSecurityAccess},
};


const t0x22DidInfo gAmeRWDidInfo[] = {
	/*This is the ame did list*/
};


/*
const tAmeIOCtrlDidInfo gAmeIOCtrlDidInfo[]={
	
	{AME_DID_TUNER, TRUE, FALSE, AME_DID_TUNER_LEN, NULL},	
	{AME_DID_BT,    TRUE, FALSE, AME_DID_BT_LEN,    NULL},	
	{AME_DID_WIFI,  TRUE, TRUE,  AME_DID_WIFI_LEN,  NULL},	
	{AME_DID_MEDIA, TRUE, FALSE, AME_DID_MEDIA_LEN, NULL},	
	{AME_DID_AUDIO, TRUE, FALSE, AME_DID_AUDIO_LEN, NULL},	
	{AME_DID_NAVI,  TRUE, FALSE, AME_DID_NAVI_LEN,  NULL},	
};

*/


//uint16 gAmeIOCtrlDidTotalNum = sizeof(gAmeIOCtrlDidInfo)/sizeof(tAmeIOCtrlDidInfo);
uint16 gAmeSubServiceFnCbkTotalNum = sizeof(gAmeSubServiceFnCbk)/sizeof(tAmeSubServiceFnCbk);









/*============================= END OF FILE: ame_Cfg.c ============================*/
