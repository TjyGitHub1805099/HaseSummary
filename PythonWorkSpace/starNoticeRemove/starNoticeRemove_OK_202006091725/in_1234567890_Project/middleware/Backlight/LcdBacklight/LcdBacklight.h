/**
*@file  LcdBacklight.h
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
#ifndef _LCDBACKLIGHT_
#define _LCDBACKLIGHT_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"

typedef enum
{
    LCDBL_MODE_MANUAL,
    LCDBL_MODE_AUTOMATIC,
} LcdBL_ModeEType;

typedef struct
{
    boolean init;
    boolean onoff;
    LcdBL_ModeEType mode;
    uint8 brightness;
    uint8 autoBrightness;
} LcdBacklightType;

extern void LcdBL_Init(void);
extern void LcdBL_Deinit(void);
extern boolean LcdBL_SetOnOff(boolean onoff);
extern boolean LcdBL_GetOnOff(uint8 * onoff);
extern boolean LcdBL_SetBrightness(uint8 brightness);
extern boolean LcdBL_GetBrightness(uint8 * brightness);
extern boolean LcdBL_SetMode(LcdBL_ModeEType mode);
extern boolean LcdBL_GetMode(LcdBL_ModeEType * mode);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_LCDBACKLIGHT_
/*=========================== END OF FILE: LcdBacklight.h ===========================*/
