/**
*@file  LcdBacklight_Cfg.h
*@brief This file is a config file, user can change the parameter according to your project.
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
#ifndef _LCDBACKLIGHT_CFG_
#define _LCDBACKLIGHT_CFG_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "flexTimer_pwm5_cfg.h"

#define LCDBL_PWM_CHANNEL INST_FLEXTIMER_PWM5_CFG

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_LCDBACKLIGHT_CFG_
/*=========================== END OF FILE: LcdBacklight_Cfg.h ===========================*/
