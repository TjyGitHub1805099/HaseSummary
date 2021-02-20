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
#ifndef _ADC_WRAPPER_CFG_H_
#define _ADC_WRAPPER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Std_Types.h"

#define PLATFORM_S32K_FOR_ADC
#include "S32K148.h"
#define ADC_INSTANCE_MAX 2

#ifdef MCU_S32K144
#define ADC_CH_MAX  16
#elif defined MCU_S32K146
#define ADC_CH_MAX  24
#elif defined MCU_S32K148
#define ADC_CH_MAX 32
#endif



#if 1

//ADC 0
#define ADC_PHY_CH_INDEX_VARIANT_CTRL       0
#define ADC_PHY_CH_INDEX_TUNER_ANT_PWR   	1
#define ADC_PHY_CH_INDEX_GPS_ANT_PWR	    2
#define ADC_PHY_CH_INDEX_MB_TEMP	        3
#define ADC_PHY_CH_INDEX_CIRCUIT_VERSION	4
#define ADC_PHY_CH_INDEX_STEERING_B_IN	    5
#define ADC_PHY_CH_INDEX_STEERING_A_IN	    6


//ADC 1
#define ADC_PHY_CH_INDEX_AMP_TEMP           0
#define ADC_PHY_CH_INDEX_FUEL_PWR           1
#define ADC_PHY_CH_INDEX_FUEL_AD 	        2
#define ADC_PHY_CH_INDEX_UDBATT_AD	        3

#else
//ADC 0
#define ADC_PHY_CH_INDEX_FUEL_PWR  0
#define ADC_PHY_CH_INDEX_FUEL_AD 	1
#define ADC_PHY_CH_INDEX_UDBATT_AD	2
#define ADC_PHY_CH_INDEX_AMP_TEMP	3
#define ADC_PHY_CH_INDEX_NTC	4
#define ADC_PHY_CH_INDEX_STEERING_A_IN	5
#define ADC_PHY_CH_INDEX_STEERING_B_IN	6


//ADC 1
#define ADC_PHY_CH_INDEX_UDBATT_AD_1 0
#endif

#if 1
typedef enum {

	ADC_LOGIC_CH_VARIANT_CTRL = 0,
	ADC_LOGIC_CH_TUNER_ANT_PWR = 1,
	ADC_LOGIC_CH_GPS_ANT_PWR = 2,
	ADC_LOGIC_CH_MB_TEMP = 3,
	ADC_LOGIC_CH_CIRCUIT_VERSION = 4,
	ADC_LOGIC_CH_STEERING_B_IN = 5,
	ADC_LOGIC_CH_STEERING_A_IN = 6,

	ADC_LOGIC_CH_AMP_TEMP = 7,
	ADC_LOGIC_CH_FUEL_PWR = 8,
	ADC_LOGIC_CH_FUEL_AD = 9,
	ADC_LOGIC_CH_UDBATT_AD = 10,

}tAdcLogicCh;
#else
typedef enum {
	ADC_LOGIC_CH_FUEL_PWR = 0,
	ADC_LOGIC_CH_FUEL_AD = 1,
	ADC_LOGIC_CH_UDBATT_AD = 2,
	ADC_LOGIC_CH_AMP_TEMP = 3,
	ADC_LOGIC_CH_NTC = 4,
	ADC_LOGIC_CH_STEERING_A_IN = 5,
	ADC_LOGIC_CH_STEERING_B_IN = 6,
	ADC_LOGIC_CH_UDBATT_AD_1 = 7,
}tAdcLogicCh;
#endif

#ifdef __cplusplus
}
#endif

#endif  

