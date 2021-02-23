/********************************************************************
File name: Adc_If.h
Author: Stephen Du
Version: V1.0
Date: 11/09/2016
Description: 
Others: 
Function List: 
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#ifndef _ADC_WRAPPER_IF_H_
#define _ADC_WRAPPER_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adc_wrapper.h"
#include "adc_wrapper_cfg.h"

boolean AdcWrapper_Init(uint8 adcInstanceIndex,const void* adcCfg);
boolean AdcWrapper_Deinit(uint8 adcInstanceIndex);
boolean AdcWrapper_ConvLoop(uint8 adcInstanceIndex);
boolean AdcWrapper_Read(uint32 ch,uint16* result);
boolean AdcWrapper_Stop(uint8 adcInstanceIndex);
boolean AdcWrapper_StopGroup(uint8 adcInstanceIndex,uint32 groupIndex);

#ifdef __cplusplus
}
#endif

#endif  

