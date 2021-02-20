/********************************************************************
Copyright (C),
File name: Pwm.h
Author: Stephen Du
Version: V1.0
Date: 10/18/2013
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
#ifndef _ERRORTYPES_H_
#define _ERRORTYPES_H_

typedef enum
{
	PWM_E_SUCCESS,
	PWM_E_PARAM_CONFIG = 0X10,
	PWM_E_UNINIT,
	PWM_E_PARAM_CHANNEL,
	PWM_E_FREQUENCE_UNCHANGEABLE,
	PWM_E_ALREADY_INITIALIZED,
	PWM_E_PARAM_POINTER,
	PWM_E_NOT_DISENGAGED,
	PWM_E_POWER_STATE_NOT_SUPPORTED,
	PWM_E_TRANSITION_NOT_POSSIBLE,
	PWM_E_PERIPHERAL_NOT_PREPARED,
	PWM_E_DUTY_VALUE_ERR,
	PWM_E_FREQUENCE_VALUE_ERR
}Pwm_ErrorType;

typedef enum
{
	SPI_E_SUCCESS,
    SPI_E_PARAM_CHANNEL = 0X0A,
	SPI_E_PARAM_BAUD,
	SPI_E_PARAM_LENGTH,
	SPI_E_PARAM_UNINIT,
	SPI_E_PARAM_POINTER,
	SPI_E_ALREADY_INITIALIZED,
    SPI_E_BUSY,
    SPI_E_NOT_RESPONSE
}Spi_ErrorType;

typedef enum
{
	ADC_E_SUCCESS,
	ADC_E_PARAM_CONFIG = 0X10,
	ADC_E_UNINIT,
	ADC_E_PARAM_CHANNEL,
	ADC_E_DATA_INVALID,
	ADC_E_ALREADY_INITIALIZED,
	ADC_E_PARAM_POINTER
}ADC_ErrorType;

typedef enum
{
	MODULE_E_SUCCESS,
	MODULE_E_PARAM_CONFIG = 0X10,
	MODULE_E_UNINIT,
	MODULE_E_ALREADY_INITIALIZED,
	MODULE_E_PARAM_POINTER
}Module_ErrorType;

#endif
