/**
*@file  keyBacklight.h
*@brief This file provide the function prototype.
*@date    2017-12-26 15:29:28
*@author  Stephen Du
*
*@copyright None
*@warning All rights reserved.
*
*@par Version Histroy
<pre><b>
Version:    Date:       Author:    Revision Log: </b>
 V1.0.0   2017-12-26  Stephen Du   First Creat
 When you update, please do not forgot to del me and add your info at here.
</pre>
*/
#ifndef _KEYBACKLIGHT_H_
#define _KEYBACKLIGHT_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

typedef enum
{
    KEYBL_MODE_MANUAL,
    KEYBL_MODE_AUTOMATIC,
} KeyBL_ModeEType;

extern void keyBL_Init(void);
extern void keyBL_Deinit(void);
extern boolean keyBL_SetOnOff(boolean onoff);
extern boolean keyBL_GetOnOff(uint8 * onoff);
extern boolean keyBL_SetBrightness(uint8 brightness);
extern boolean keyBL_GetBrightness(uint8 * brightness);
extern boolean keyBL_SetMode(KeyBL_ModeEType mode);
extern boolean keyBL_GetMode(KeyBL_ModeEType * mode);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_KEYBACKLIGHT_H_
/*=========================== END OF FILE: keyBacklight.h ===========================*/
