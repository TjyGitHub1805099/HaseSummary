/**
*@file  KeyBacklight_Cfg.h
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
#ifndef _KEYBACKLIGHT_CFG_
#define _KEYBACKLIGHT_CFG_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "flexTimer_pwm3_cfg.h"

#define KEYBL_PWM_CHANNEL INST_FLEXTIMER_PWM3_CFG

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //_KEYBACKLIGHT_CFG_
/*=========================== END OF FILE: KeyBacklight_Cfg.h ===========================*/
