
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  :Port_if.c
**          Author: brown
**
**          Date  : 2018-01-05
**          
**          
******************************************************************************/


#include "Port_if.h"
#include "S32K146.h"


boolean Port_IocBootMode(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_IOC_BOOT_MODE,(1 << PORT_PIN_IOC_BOOT_MODE), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_IOC_BOOT_MODE) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_IOC_BOOT_MODE, (1 << PORT_PIN_IOC_BOOT_MODE));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_IOC_BOOT_MODE, (1 << PORT_PIN_IOC_BOOT_MODE));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_IOC_BOOT_MODE);
	}
	return(ReturnValue);
}
pins_channel_type_t  TestPin;
boolean Port_UdBattAdEn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_UDBATT_AD_EN,(1 << PORT_PIN_UDBATT_AD_EN), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//TestPin = PINS_DRV_GetPinsDirection(PORT_UDBATT_AD_EN);
		//if(PINS_DRV_GetPinsDirection(PORT_UDBATT_AD_EN) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_UDBATT_AD_EN, (1 << PORT_PIN_UDBATT_AD_EN));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_UDBATT_AD_EN, (1 << PORT_PIN_UDBATT_AD_EN));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_UDBATT_AD_EN);
	}
	return(ReturnValue);
}	

boolean Port_Pmic2Feed5vOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_PMIC2_FEED_5V0_ON,(1 << PORT_PIN_PMIC2_FEED_5V0_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_PMIC2_FEED_5V0_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_PMIC2_FEED_5V0_ON, (1 << PORT_PIN_PMIC2_FEED_5V0_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_PMIC2_FEED_5V0_ON, (1 << PORT_PIN_PMIC2_FEED_5V0_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_PMIC2_FEED_5V0_ON);
	}
	return(ReturnValue);
}	

boolean Port_PmicPwrOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_PMIC_PWRON,(1 << PORT_PIN_PMIC_PWRON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_PMIC_PWRON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_PMIC_PWRON, (1 << PORT_PIN_PMIC_PWRON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_PMIC_PWRON, (1 << PORT_PIN_PMIC_PWRON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_PMIC_PWRON);
	}
	return(ReturnValue);
}	

boolean Port_UG5vOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_UG5V_ON,(1 << PORT_PIN_UG5V_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_UG5V_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_UG5V_ON, (1 << PORT_PIN_UG5V_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_UG5V_ON, (1 << PORT_PIN_UG5V_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_UG5V_ON);
	}
	return(ReturnValue);
}	

boolean Port_UG3v3On(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_UG3V3_ON,(1 << PORT_PIN_UG3V3_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_UG3V3_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_UG3V3_ON, (1 << PORT_PIN_UG3V3_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_UG3V3_ON, (1 << PORT_PIN_UG3V3_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_UG3V3_ON);
	}
	return(ReturnValue);
}	

boolean Port_UG1v8On(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_UG1V8_ON,(1 << PORT_PIN_UG1V8_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_UG1V8_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_UG1V8_ON, (1 << PORT_PIN_UG1V8_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_UG1V8_ON, (1 << PORT_PIN_UG1V8_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_UG1V8_ON);
	}
	return(ReturnValue);
}

boolean Port_UG1v2On(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_UG1V2_ON,(1 << PORT_PIN_UG1V2_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_UG1V2_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_UG1V2_ON, (1 << PORT_PIN_UG1V2_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_UG1V2_ON, (1 << PORT_PIN_UG1V2_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_UG1V2_ON);
	}
	return(ReturnValue);
}

boolean Port_LcdPwrOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_LCD_PWR_ON,(1 << PORT_PIN_LCD_PWR_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_LCD_PWR_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_LCD_PWR_ON, (1 << PORT_PIN_LCD_PWR_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_LCD_PWR_ON, (1 << PORT_PIN_LCD_PWR_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_LCD_PWR_ON);
	}
	return(ReturnValue);
}

boolean Port_BlPwrOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_BL_PWR_ON,(1 << PORT_PIN_BL_PWR_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_BL_PWR_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_BL_PWR_ON, (1 << PORT_PIN_BL_PWR_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_BL_PWR_ON, (1 << PORT_PIN_BL_PWR_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_BL_PWR_ON);
	}
	return(ReturnValue);
}

boolean Port_BlEn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_BL_EN,(1 << PORT_PIN_BL_EN), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_BL_EN) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_BL_EN, (1 << PORT_PIN_BL_EN));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_BL_EN, (1 << PORT_PIN_BL_EN));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_BL_EN);
	}
	return(ReturnValue);
}

boolean Port_Charge5vOn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CHARGE_5V_ON,(1 << PORT_PIN_CHARGE_5V_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CHARGE_5V_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CHARGE_5V_ON, (1 << PORT_PIN_CHARGE_5V_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CHARGE_5V_ON, (1 << PORT_PIN_CHARGE_5V_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CHARGE_5V_ON);
	}
	return(ReturnValue);
}

boolean Port_Vcc12vEn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_VCC12VOUT_EN,(1 << PORT_PIN_VCC12VOUT_EN), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_VCC12VOUT_EN) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_VCC12VOUT_EN, (1 << PORT_PIN_VCC12VOUT_EN));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_VCC12VOUT_EN, (1 << PORT_PIN_VCC12VOUT_EN));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_VCC12VOUT_EN);
	}
	return(ReturnValue);
}

boolean Port_BtPwrWLAN_On(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_PWR_BT_WLAN_ON,(1 << PORT_PIN_PWR_BT_WLAN_ON), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_PWR_BT_WLAN_ON) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_PWR_BT_WLAN_ON, (1 << PORT_PIN_PWR_BT_WLAN_ON));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_PWR_BT_WLAN_ON, (1 << PORT_PIN_PWR_BT_WLAN_ON));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_PWR_BT_WLAN_ON);
	}
	return(ReturnValue);
}

boolean Port_CanStb(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CAN_STB,(1 << PORT_PIN_CAN_STB), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CAN_STB) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CAN_STB, (1 << PORT_PIN_CAN_STB));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CAN_STB, (1 << PORT_PIN_CAN_STB));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CAN_STB);
	}
	return(ReturnValue);
}

boolean Port_CanWakeUp(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CAN_WAKE,(1 << PORT_PIN_CAN_WAKE), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CAN_WAKE) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CAN_WAKE, (1 << PORT_PIN_CAN_WAKE));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CAN_WAKE, (1 << PORT_PIN_CAN_WAKE));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CAN_WAKE);
	}
	return(ReturnValue);
}

boolean Port_CanEn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CAN_EN,(1 << PORT_PIN_CAN_EN), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CAN_EN) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CAN_EN, (1 << PORT_PIN_CAN_EN));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CAN_EN, (1 << PORT_PIN_CAN_EN));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CAN_EN);
	}
	return(ReturnValue);
}

boolean Port_Charge1Ctl1(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CHARGE1_CTL1,(1 << PORT_PIN_CHARGE1_CTL1), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CHARGE1_CTL1) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CHARGE1_CTL1, (1 << PORT_PIN_CHARGE1_CTL1));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CHARGE1_CTL1, (1 << PORT_PIN_CHARGE1_CTL1));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CHARGE1_CTL1);
	}
	return(ReturnValue);
}

boolean Port_Charge1Ctl2(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CHARGE1_CTL2,(1 << PORT_PIN_CHARGE1_CTL2), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CHARGE1_CTL2) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CHARGE1_CTL2, (1 << PORT_PIN_CHARGE1_CTL2));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CHARGE1_CTL2, (1 << PORT_PIN_CHARGE1_CTL2));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CHARGE1_CTL2);
	}
	return(ReturnValue);
}

boolean Port_Charge1Ctl3(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CHARGE1_CTL3,(1 << PORT_PIN_CHARGE1_CTL3), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CHARGE1_CTL3) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CHARGE1_CTL3, (1 << PORT_PIN_CHARGE1_CTL3));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CHARGE1_CTL3, (1 << PORT_PIN_CHARGE1_CTL3));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CHARGE1_CTL3);
	}
	return(ReturnValue);
}
 
boolean Port_ChargeEn(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_CHARGE1_EN,(1 << PORT_PIN_CHARGE1_EN), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_CHARGE1_EN) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_CHARGE1_EN, (1 << PORT_PIN_CHARGE1_EN));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_CHARGE1_EN, (1 << PORT_PIN_CHARGE1_EN));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_CHARGE1_EN);
	}
	return(ReturnValue);
}

boolean Port_4GRst3V3(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_4G_RST_3V3,(1 << PORT_PIN_4G_RST_3V3), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_4G_RST_3V3) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_4G_RST_3V3, (1 << PORT_PIN_4G_RST_3V3));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_4G_RST_3V3, (1 << PORT_PIN_4G_RST_3V3));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_4G_RST_3V3);
	}
	return(ReturnValue);
}

boolean Port_4GPwrKey3V3(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_4G_PWRKEY_3V3,(1 << PORT_PIN_4G_PWRKEY_3V3), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_4G_PWRKEY_3V3) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_4G_PWRKEY_3V3, (1 << PORT_PIN_4G_PWRKEY_3V3));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_4G_PWRKEY_3V3, (1 << PORT_PIN_4G_PWRKEY_3V3));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_4G_PWRKEY_3V3);
	}
	return(ReturnValue);
}

boolean Port_4GDtr3V3(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_4G_DTR_3V3,(1 << PORT_PIN_4G_DTR_3V3), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_4G_DTR_3V3) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_4G_DTR_3V3, (1 << PORT_PIN_4G_DTR_3V3));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_4G_DTR_3V3, (1 << PORT_PIN_4G_DTR_3V3));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_4G_DTR_3V3);
	}
	return(ReturnValue);
}

boolean Port_4GRI3V3(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_4G_RI_3V3,(1 << PORT_PIN_4G_RI_3V3), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_4G_RI_3V3) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_4G_RI_3V3, (1 << PORT_PIN_4G_RI_3V3));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_4G_RI_3V3, (1 << PORT_PIN_4G_RI_3V3));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_4G_RI_3V3);
	}
	return(ReturnValue);
}

boolean Port_AmpSty(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_AMP_STB,(1 << PORT_PIN_AMP_STB), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_AMP_STB) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_AMP_STB, (1 << PORT_PIN_AMP_STB));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_AMP_STB, (1 << PORT_PIN_AMP_STB));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_AMP_STB);
	}
	return(ReturnValue);
}

boolean Port_PmicWdi(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_IOC_PMIC_WDI,(1 << PORT_PIN_IOC_PMIC_WDI), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_IOC_PMIC_WDI) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_IOC_PMIC_WDI, (1 << PORT_PIN_IOC_PMIC_WDI));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_IOC_PMIC_WDI, (1 << PORT_PIN_IOC_PMIC_WDI));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_IOC_PMIC_WDI);
	}
	return(ReturnValue);
}

boolean Port_McuSleepStatus(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_MCU_SLEEP_STATUS,(1 << PORT_PIN_MCU_SLEEP_STATUS), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_MCU_SLEEP_STATUS) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_MCU_SLEEP_STATUS, (1 << PORT_PIN_MCU_SLEEP_STATUS));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_MCU_SLEEP_STATUS, (1 << PORT_PIN_MCU_SLEEP_STATUS));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_MCU_SLEEP_STATUS);
	}
	return(ReturnValue);
}

boolean Port_McuWakeUp4G(PortFuncType func, boolean value)
{
	boolean ReturnValue = E_OK;

	if(func == PORT_SET_DIR)
	{
		PINS_DRV_SetPinDirection(PORT_MCU_WAKEUP_4G,(1 << PORT_PIN_MCU_WAKEUP_4G), (pins_level_type_t) value);
	}
	else if(func == PORT_SET_VALUE)
	{
		//if(PINS_DRV_GetPinsDirection(PORT_MCU_WAKEUP_4G) == PORT_OUTPUT)
		//{
			if(value == PORT_HIGH)
			{
				PINS_DRV_SetPins(PORT_MCU_WAKEUP_4G, (1 << PORT_PIN_MCU_WAKEUP_4G));
			}
			else
			{
				PINS_DRV_ClearPins(PORT_MCU_WAKEUP_4G, (1 << PORT_PIN_MCU_WAKEUP_4G));
			}
		//}
		//else
		//{
		//	ReturnValue = E_NOT_OK;
		//}
	}
	else
	{
		ReturnValue = (boolean)PINS_DRV_ReadPins(PORT_MCU_WAKEUP_4G);
	}
	return(ReturnValue);
}






