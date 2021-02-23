#include "HardwareVersion.h"
#include "adc_wrapper_if.h"
#include "trace.h"
#include "BoardDefines.h"

uint8 CircuitVerionConver(uint16 AdVal)
{
	uint8 CircuitVerion = 0xFF;
	volatile uint16 VersionAdVal;
	VersionAdVal = AdVal;
	if((VersionAdVal <= (VERSION_0_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= VERSION_0_LEVEL))
	{
		CircuitVerion = VERSION_0;
	}
	else if((VersionAdVal <= (VERSION_1_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_1_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_1;
	}
	else if((VersionAdVal <= (VERSION_2_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_2_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_2;
	}
	else if((VersionAdVal <= (VERSION_3_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_3_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_3;
	}
	else if((VersionAdVal <= (VERSION_4_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_4_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_4;
	}
	else if((VersionAdVal <= (VERSION_5_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_5_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_5;
	}
	else if((VersionAdVal <= (VERSION_6_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_6_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_6;
	}
	else if((VersionAdVal <= (VERSION_7_LEVEL + VERSION_STEP_LEVEL))&&(VersionAdVal >= (VERSION_7_LEVEL - VERSION_STEP_LEVEL)))
	{
		CircuitVerion = VERSION_7;
	}

	return CircuitVerion;
}

uint8 VariantCtrlConver(uint16 AdVal)
{
	uint8  VariantCtrlVer = 0xFF;
	volatile uint16 VersionAdVal;
	VersionAdVal = AdVal;
	if( (VersionAdVal <= (N1210_LEVEL + VARIANTCTRL_STEP_LEVEL)) &&( (VersionAdVal+VARIANTCTRL_STEP_LEVEL) >= N1210_LEVEL) )
	{
		VariantCtrlVer = N1210;
	}
	else if( (VersionAdVal <= (N1210A_LEVEL + VARIANTCTRL_STEP_LEVEL)) && ( (VersionAdVal+VARIANTCTRL_STEP_LEVEL) >= N1210A_LEVEL ))
	{
		VariantCtrlVer = N1210A;
	}

	return VariantCtrlVer;
}

uint8 getCircuitVerion(void)
{
	uint8 ret = 0xFF;
	uint16 CircuitVerAdVal = 0;
	AdcWrapper_Read(ADC_LOGIC_CH_CIRCUIT_VERSION,&CircuitVerAdVal);
	
	ret = CircuitVerionConver(CircuitVerAdVal);
	
	return ret;	
}

uint8 getVariantCtrlVerion(void)
{
	uint8 ret = 0xFF;
	uint16 VariantCtrlAdVal = 0;
	AdcWrapper_Read(ADC_LOGIC_CH_VARIANT_CTRL,&VariantCtrlAdVal);
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "VariantCtrlAdVal:%d \n\r",VariantCtrlAdVal);
	ret = VariantCtrlConver(VariantCtrlAdVal);
	TracePrintf(INPUT_TRACE, TRACE_NONMASK, "VariantCtrlVersion:%d \n\r",ret);
	return ret;	
}

uint8 getITMasterCtrlVerion(void)
{
	uint8 ret = 0;
	
	ret |= (Port_GetPinLevel(EQ1_DET)&0X01)<<3;
	ret |= (Port_GetPinLevel(EQ2_DET)&0X01)<<2;
	ret |= (Port_GetPinLevel(EQ3_DET)&0X01)<<1;
	ret |= (Port_GetPinLevel(EQ4_DET)&0X01)<<0;
	
	return ret;	
}
