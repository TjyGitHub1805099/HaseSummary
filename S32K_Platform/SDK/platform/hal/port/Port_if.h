
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Port_if.h
**          Author: brown
**
**          Date  : 2018-01-05
**          
**          
******************************************************************************/

#ifndef _PORT_IF_H_
#define _PORT_IF_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#include "Std_Types.h"
#include "BoardDefines.h"


typedef enum
{
	PORT_SET_DIR,
	PORT_SET_VALUE,
	PORT_GET_VALUE
}PortFuncType;

#ifndef PORT_OUTPUT
#define PORT_OUTPUT 1U
#endif
#ifndef PORT_INPUT
#define PORT_INPUT 0U
#endif
#ifndef PORT_HIGH
#define PORT_HIGH 1U
#endif
#ifndef PORT_LOW
#define PORT_LOW 0U
#endif


boolean Port_IocBootMode(PortFuncType func, boolean value);
boolean Port_UdBattAdEn(PortFuncType func, boolean value);
boolean Port_Pmic2Feed5vOn(PortFuncType func, boolean value);
boolean Port_UG5vOn(PortFuncType func, boolean value);
boolean Port_UG3v3On(PortFuncType func, boolean value);
boolean Port_UG1v8On(PortFuncType func, boolean value);
boolean Port_UG1v2On(PortFuncType func, boolean value);
boolean Port_LcdPwrOn(PortFuncType func, boolean value);
boolean Port_BlPwrOn(PortFuncType func, boolean value);
boolean Port_BlEn(PortFuncType func, boolean value);
boolean Port_Charge5vOn(PortFuncType func, boolean value);
boolean Port_Vcc12vEn(PortFuncType func, boolean value);
boolean Port_BtPwrWLAN_On(PortFuncType func, boolean value);
boolean Port_CanStb(PortFuncType func, boolean value);
boolean Port_CanWakeUp(PortFuncType func, boolean value);
boolean Port_CanEn(PortFuncType func, boolean value);
boolean Port_Charge1Ctl1(PortFuncType func, boolean value);
boolean Port_Charge1Ctl2(PortFuncType func, boolean value);
boolean Port_Charge1Ctl3(PortFuncType func, boolean value);
boolean Port_ChargeEn(PortFuncType func, boolean value);
boolean Port_4GRst3V3(PortFuncType func, boolean value);
boolean Port_4GPwrKey3V3(PortFuncType func, boolean value);
boolean Port_4GDtr3V3(PortFuncType func, boolean value);
boolean Port_4GRI3V3(PortFuncType func, boolean value);
boolean Port_AmpSty(PortFuncType func, boolean value);
boolean Port_PmicWdi(PortFuncType func, boolean value);
boolean Port_McuSleepStatus(PortFuncType func, boolean value);
boolean Port_McuWakeUp4G(PortFuncType func, boolean value);


#ifdef __cplusplus
}
#endif // __cplusplus
#endif

