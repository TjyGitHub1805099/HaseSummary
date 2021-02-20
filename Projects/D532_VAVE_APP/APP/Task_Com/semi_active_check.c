#include "semi_active_check.h"
#include "MemMap.h"
#include "pin_wrapper_if.h"

#include "HardwareVersion.h"



BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveDisplayUnit;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveLanguage;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveTPMSWith;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveService;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveOATWith;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveClockWithoutNavi_Audio;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveNAVI_AUDIO;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_BSW;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_LDW;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_FEB;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveAT_MT;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveIdleStopFunction;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveE_PKB_And_AVH;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveIBA_OFF;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveIndirectIllumination;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveRearSeat;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveSLIP;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveI_KEY;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveOAT_With;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiSonar;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiLED_Headlamp;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiEAPM;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiSelectiveUnlock;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiWiperWithSpeed;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiLightSensitivity;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiLightOffDay;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiAutoFoldFunction;			//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiAutoLockFunctionCustomize;	//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiLockAutoWindowUp;			//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiAutoUnlockFunctionCustomize;//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiAnswerBack;					//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiRainSensor;					//yangxl 2019-7-22 16:01:03
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiWiperWithWipingDrip;		//yangxl 2019-7-22 16:01:03

BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_ACC; //yangxl add H-22-4-C 2019-7-25 10:49:46
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_LKA; //yangxl add H-22-4-C 2019-7-25 10:49:46

BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveShiftByWire;//yangxl add H-10-1-2 2019-11-26 15:05:56












uint8_t semiCount[semiMaxIndex] = {0};

/***************yangxl***************/
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_TJP;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t semiActiveITS_ICC;


/***************yangxl***************/



uint8_t semiActiveFlag = 0;

void SemiActiveCheckInit(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		semiActiveDisplayUnit = 0;
		semiActiveLanguage = 0;
		semiActiveTPMSWith = 0;
		semiActiveService = 0;
		semiActiveOATWith = 0;
		semiActiveClockWithoutNavi_Audio = 0;
		semiActiveNAVI_AUDIO = 0;
		semiActiveITS_BSW = 0;
		semiActiveITS_LDW = 0;
		semiActiveITS_FEB = 0;
		semiActiveAT_MT = 0;
		semiActiveIdleStopFunction = 0;
		semiActiveE_PKB_And_AVH = 0x03;		//0x03:The E-PKB and AVH  function is unavailable
		semiActiveIBA_OFF = 0;
		semiActiveIndirectIllumination = 0;
		semiActiveRearSeat = 0;
		semiActiveSLIP = 0;
		semiActiveI_KEY = 0;
		semiActiveOAT_With = 0;
		semiSonar = 0;
		semiLED_Headlamp = 0;
		semiEAPM = 0;
		semiSelectiveUnlock = 0;
		semiWiperWithSpeed = 0;
		semiLightSensitivity = 0;
		semiLightOffDay = 0;	
		semiActiveITS_TJP = 0u; /* yangxl */
		semiActiveITS_ICC = 0u;
		
		semiAutoFoldFunction = 0u;			//yangxl 2019-7-22 16:01:03
 		semiAutoLockFunctionCustomize = 0u;	//yangxl 2019-7-22 16:01:03
 		semiLockAutoWindowUp = 0u;			//yangxl 2019-7-22 16:01:03
 		semiAutoUnlockFunctionCustomize = 0u;//yangxl 2019-7-22 16:01:03
 		semiAnswerBack = 0u;					//yangxl 2019-7-22 16:01:03
		semiRainSensor = 0u;				//yangxl 2019-7-22 16:01:03
		semiWiperWithWipingDrip = 0u;		//yangxl 2019-7-22 16:01:03

		semiActiveITS_ACC = 0u;
		semiActiveITS_LKA = 0u;
		semiActiveShiftByWire = 0u; //yangxl 2019-11-26 15:07:15
	}
	semiActiveFlag = 1;
}


uint8_t semiActiveDisplayUnitGet(void)
{
	return semiActiveDisplayUnit;
}

uint8_t semiActiveLanguageGet(void)
{
	return semiActiveLanguage;
}
	
uint8_t semiActiveTPMSWithGet(void)
{
	return semiActiveTPMSWith;
}

uint8_t semiActiveServiceGet(void)
{
	return semiActiveService;
}

uint8_t semiActiveOATWithGet(void)
{
	return semiActiveOATWith;
}

uint8_t semiActiveClockWithoutNavi_AudioGet(void)
{
	return semiActiveClockWithoutNavi_Audio;
}

uint8_t semiActiveNAVI_AUDIOGet(void)
{
	return semiActiveNAVI_AUDIO;
}

uint8_t semiActiveITS_BSWGet(void)
{
	return semiActiveITS_BSW;
}

uint8_t semiActiveITS_LDWGet(void)
{
	return semiActiveITS_LDW;
}

uint8_t semiActiveITS_FEBGet(void)
{
	return semiActiveITS_FEB;
}

uint8_t semiActiveAT_MTGet(void)
{
	return semiActiveAT_MT;
}

uint8_t semiActiveIdleStopFunctionGet(void)
{
	return semiActiveIdleStopFunction;
}

uint8_t semiActiveE_PKB_And_AVHGet(void)
{
	return semiActiveE_PKB_And_AVH;
}

uint8_t semiActiveIBA_OFFGet(void)
{
	return semiActiveIBA_OFF;
}

uint8_t semiActiveIndirectIlluminationGet(void)
{
	return semiActiveIndirectIllumination;
}

uint8_t semiActiveRearSeatGet(void)
{
	return semiActiveRearSeat;
}


uint8_t semiActiveSLIPGet(void)
{
	return semiActiveSLIP;
}

uint8_t semiActiveI_KEYGet(void)
{
	return semiActiveI_KEY;
}

uint8_t semiActiveOAT_WithGet(void)
{
	return semiActiveOAT_With;
}

uint8_t semiSonarGet(void)
{
	return semiSonar;
}

uint8_t semiLED_HeadlampFlagGet(void)
{
	return semiLED_Headlamp;
}

uint8_t semiEAPMFlagGet(void)
{
	return semiEAPM;
}

uint8_t semiSelectiveUnlockFlagGet(void)
{
	return semiSelectiveUnlock;
}

uint8_t semiWiperWithSpeedFlagGet(void)
{
	return semiWiperWithSpeed;
}

uint8_t semiLightSensitivityFlagGet(void)
{
	return semiLightSensitivity;
}

uint8_t semiLightOffDayFlagGet(void)
{
	return semiLightOffDay;
}

uint8_t semiAutoFoldFunctionFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiAutoFoldFunction;
}
uint8_t semiAutoLockFunctionCustomizeFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiAutoLockFunctionCustomize;
}
uint8_t semiLockAutoWindowUpFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiLockAutoWindowUp;
}
uint8_t semiAutoUnlockFunctionCustomizeFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiAutoUnlockFunctionCustomize;
}
uint8_t semiAnswerBackFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiAnswerBack;
}
uint8_t semiRainSensorFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiRainSensor;
}
uint8_t semiWiperWithWipingDripFlagGet(void) //yangxl 2019-7-22 16:18:37
{
	return semiWiperWithWipingDrip;
}

uint8_t semiActiveITS_ACCGet(void) //yangxl 2019-7-25 10:51:55
{
	return semiActiveITS_ACC;
}

uint8_t semiActiveITS_LKAGet(void) //yangxl 2019-7-25 10:52:00
{
	return semiActiveITS_LKA;
}

uint8_t semiActiveShiftByWireGet(void) //yangxl 2019-11-26 15:07:53
{
	return semiActiveShiftByWire;
}




void semiSelectiveUnlockFlagSet(uint8_t value)
{
	if(value)
	{
		semiSelectiveUnlock = value;
		semiActiveUpdateFlagSet(1);
	}
}

void semiWiperWithSpeedFlagSet(uint8_t value)
{
	if(value)
	{
		semiWiperWithSpeed = value;
		semiActiveUpdateFlagSet(1);
	}
}

void semiLightSensitivityFlagSet(uint8_t value)
{
	//if(value)
	{
		semiLightSensitivity = value;
		semiActiveUpdateFlagSet(1);
	}
}

void semiLightOffDayFlagSet(uint8_t value)
{
	//if(value)
	{
		semiLightOffDay = value;
		semiActiveUpdateFlagSet(1);
	}
}

void semiAutoFoldFunctionFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiAutoFoldFunction = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiAutoLockFunctionCustomizeFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiAutoLockFunctionCustomize = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiLockAutoWindowUpFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiLockAutoWindowUp = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiAutoUnlockFunctionCustomizeFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiAutoUnlockFunctionCustomize = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiAnswerBackFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiAnswerBack = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiRainSensorFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiRainSensor = value;
		semiActiveUpdateFlagSet(1);
	}
}
void semiWiperWithWipingDripFlagSet(uint8_t value)//yangxl 2019-7-22 16:22:27
{
	//if(value)
	{
		semiWiperWithWipingDrip = value;
		semiActiveUpdateFlagSet(1);
	}
}



void semiCarAssistSettingReset(void)
{
	semiSelectiveUnlock = 0;
	semiWiperWithSpeed = 0;
	semiLightSensitivity = 0;
	semiLightOffDay = 0;
	
	semiAutoFoldFunction = 0u;			//yangxl 2019-7-22 16:01:03
	semiAutoLockFunctionCustomize = 0u;	//yangxl 2019-7-22 16:01:03
	semiLockAutoWindowUp = 0u;			//yangxl 2019-7-22 16:01:03
	semiAutoUnlockFunctionCustomize = 0u;//yangxl 2019-7-22 16:01:03
	semiAnswerBack = 0u;					//yangxl 2019-7-22 16:01:03
	semiRainSensor = 0u;				//yangxl 2019-7-22 16:01:03
	semiWiperWithWipingDrip = 0u;		//yangxl 2019-7-22 16:01:03
	
	semiActiveUpdateFlagSet(1);
}

uint8_t semiActiveUpdateFlagGet(void)
{
	return semiActiveFlag;
}

uint8_t semiActiveITS_TJPGet(void) /*yangxl */
{
	return semiActiveITS_TJP;
}
/*
void semiActiveITS_TJPSet(uint8_t value)
{
	semiActiveITS_TJP = value;
}
*/

uint8_t semiActiveITS_ICCGet(void) /*yangxl */
{
	return semiActiveITS_ICC;
}
/*
void semiActiveITS_ICCSet(uint8_t value)
{
	semiActiveITS_ICC = value;
}
*/

void semiActiveUpdateFlagSet(uint8_t value)
{
	semiActiveFlag = value;
}

void SemiActiveCheckProc(void)
{
	uint8_t l_value = 0;
	uint8_t l_semiActiveFlag  = 0;
	uint8_t l_IPDU_HND_SONAR_GeneralStatus_8 = 0;
	static uint32_t s_IndirectIlluminationCnt = 0;
	static uint16_t s_LED_HeadLampCnt = 0;
	static uint8_t s_SemiActiveE_PKB_And_AVH = 0x03;
	
	if(semiActiveITS_BSW == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_SRR_Left))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_SRR_Left, 0);
			semiCount[semiActiveITS_BSWIndex]++;
			if(semiCount[semiActiveITS_BSWIndex] >= 3)
			{
				semiActiveITS_BSW = 1;
				l_semiActiveFlag = 1;
			}
		}
	}
		
	if(semiActiveITS_LDW == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N3_7b))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N3_7b, 0);
			semiCount[semiActiveITS_LDWIndex]++;
			if(semiCount[semiActiveITS_LDWIndex] >= 3)
			{
				semiActiveITS_LDW = 1;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(semiActiveITS_FEB == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N2_1))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N2_1, 0);
			semiCount[semiActiveITS_FEBIndex]++;
			if(semiCount[semiActiveITS_FEBIndex] >= 3)
			{
				semiActiveITS_FEB = 1;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(semiActiveAT_MT == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ATCVT_to_MMI_RN1))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ATCVT_to_MMI_RN1, 0);
			semiCount[semiActiveAT_MTIndex]++;
			if(semiCount[semiActiveAT_MTIndex] >= 3)
			{
				semiActiveAT_MT = 1;
				l_semiActiveFlag = 1;
			}
		}
	}

	if((getITMasterCtrlVerion() >= 0b1110) || (getITMasterCtrlVerion() == 0b1100)) //yangxl ³µÁ¾ÅäÖÃÅÐ¶Ï 2019-9-24 11:19:18
	{
		if(semiActiveIdleStopFunction == 0)
		{
			if(DEA_GetCanReceiveIndStat(IPDU_HND_ECCSMSG8_OBD_IS))
			{
				DEA_SetCanReceiveIndStat(IPDU_HND_ECCSMSG8_OBD_IS, 0);
				Com_RxSig_GetISSAP(&l_value);
				if(l_value)
				{
					semiCount[semiActiveIdleStopFunctionIndex]++;
					if(semiCount[semiActiveIdleStopFunctionIndex] >= 3)
					{
						semiActiveIdleStopFunction = 1;
						l_semiActiveFlag = 1;
					}
				}
				else
				{
					semiCount[semiActiveIdleStopFunctionIndex] = 0;
				}
			}
		}
	}else
	{
		semiActiveIdleStopFunction = 0;
	}
	

#if 1
	if(semiActiveTPMSWith != 2)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_TPMS_GeneralStatus_BCM))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_TPMS_GeneralStatus_BCM, 0);
			Com_RxSig_GetD_STYLE(&l_value);
			if(l_value != 0x02)
			{
				semiCount[semiActiveTPMSWithIndex2]=0u;
				if(semiCount[semiActiveTPMSWithIndex1] < 3u)
				{
					semiCount[semiActiveTPMSWithIndex1]++;
				}else
				{
					if(semiActiveTPMSWith != 1)
					{
						semiActiveTPMSWith = 1;
						l_semiActiveFlag = 1;
					}
				}
				/*
				if(semiCount[semiActiveTPMSWithIndex1] >= 3u)
				{
					if(semiActiveTPMSWith != 1)
					{
						semiActiveTPMSWith = 1;
						l_semiActiveFlag = 1;
					}
				}
				*/

			}
			else
			{
				semiCount[semiActiveTPMSWithIndex1]=0u;
				semiCount[semiActiveTPMSWithIndex2]++;
				if(semiCount[semiActiveTPMSWithIndex2] >= 3u)
				{
					semiActiveTPMSWith = 2;
					l_semiActiveFlag = 1;
				}

			}
		}
	}
#else
	if(semiActiveTPMSWith != 1)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_TPMS_GeneralStatus_BCM))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_TPMS_GeneralStatus_BCM, 0);
			semiCount[semiActiveTPMSWithIndex]++;
			if(semiCount[semiActiveTPMSWithIndex] >= 3)
			{
				semiActiveTPMSWith = 1;
				l_semiActiveFlag = 1;
			}
		}
	}
#endif

	if(semiActiveE_PKB_And_AVH == 0x03)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_EPB_SystemControl))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_EPB_SystemControl, 0);
			Com_RxSig_GetEPB_CONG_IND(&l_value);
			if(l_value != 0x03)
			{
				if(s_SemiActiveE_PKB_And_AVH == l_value)
				{
					semiCount[semiActiveE_PKB_And_AVHIndex]++;
					if(semiCount[semiActiveE_PKB_And_AVHIndex] >= 3)
					{
						semiActiveE_PKB_And_AVH = l_value;
						l_semiActiveFlag = 1;
					}
				}
				else
				{
					s_SemiActiveE_PKB_And_AVH = l_value;
					semiCount[semiActiveE_PKB_And_AVHIndex] = 1;
				}
			}
			else
			{
				semiCount[semiActiveE_PKB_And_AVHIndex] = 0;
			}
		}
	}

	if(semiActiveRearSeat == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ACU_GeneralStatus_6))
		{	
			DEA_SetCanReceiveIndStat(IPDU_HND_ACU_GeneralStatus_6, 0);
			do
			{
				Com_RxSig_GetSecond_ROW_L_BELT_STA(&l_value);
				if((l_value == 0b01) || (l_value == 0b10))
				{
					semiCount[semiActiveRearSeatIndex]++;
					if(semiCount[semiActiveRearSeatIndex] >= 3)
					{
						semiActiveRearSeat = 1;
						l_semiActiveFlag = 1;
					}
					break;
				}

				Com_RxSig_GetSecond_ROW_C_BELT_STA(&l_value);
				if((l_value == 0b01) || (l_value == 0b10))
				{
					semiCount[semiActiveRearSeatIndex]++;
					if(semiCount[semiActiveRearSeatIndex] >= 3)
					{
						semiActiveRearSeat = 1;
						l_semiActiveFlag = 1;
					}
					break;
				}

				Com_RxSig_GetSecond_ROW_R_BELT_STA(&l_value);
				if((l_value == 0b01) || (l_value == 0b10))
				{
					semiCount[semiActiveRearSeatIndex]++;
					if(semiCount[semiActiveRearSeatIndex] >= 3)
					{
						semiActiveRearSeat = 1;
						l_semiActiveFlag = 1;
					}
					break;
				}
			}while(0);
		}
	}

	if(semiActiveSLIP == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_Brake_StabilityControl_RN1))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_Brake_StabilityControl_RN1, 0);
			semiCount[semiActiveSLIPIndex]++;
			if(semiCount[semiActiveSLIPIndex] >= 10)
			{
				semiActiveSLIP = 1;
				l_semiActiveFlag = 1;
			}
		}
		else if(DEA_GetCanReceiveIndStat(IPDU_HND_Brake_GeneralData))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_Brake_GeneralData, 0);
			if(DEA_GetVDC_LAMP() == 0x01)
			{
				semiCount[semiActiveSLIPIndex]++;
				if(semiCount[semiActiveSLIPIndex] >= 5)
				{
					semiActiveSLIP = 1;
					l_semiActiveFlag = 1;
				}
			}
		}
	}

	if(semiActiveI_KEY == 0)
	{
	
		if(DEA_GetCanReceiveIndStat(IPDU_HND_SMART_bcmMSG_PUSH))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_SMART_bcmMSG_PUSH, 0);
			semiCount[semiActiveI_KEYIndex]++;
			if(semiCount[semiActiveI_KEYIndex] >= 3)
			{
				semiActiveI_KEY = 1;
				l_semiActiveFlag = 1;
			}
		}
		
	}

	if(semiActiveIndirectIllumination == 0)
	{
		if(GetPinValue_ILLUMINATION_SEMI())
		{
			if(s_IndirectIlluminationCnt < 50) //50ms
			{
				s_IndirectIlluminationCnt++;
			}
			else
			{
				semiActiveIndirectIllumination = 1;
				l_semiActiveFlag = 1;
			}
		}
		else
		{
			s_IndirectIlluminationCnt = 0;
		}
	}

	if(semiActiveOAT_With == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_CLIMATE_ControlData_F))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_CLIMATE_ControlData_F, 0);
			Com_ReceiveSignal(SIG_HND_OutsideTemp_AD, &l_value);	//591 project OAT
			if((l_value != 0) && (l_value != 0xFF))
			{
				semiActiveOAT_With = 1;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(DEA_GetCanReceiveIndStat(IPDU_HND_SONAR_GeneralStatus_8))
	{
		DEA_SetCanReceiveIndStat(IPDU_HND_SONAR_GeneralStatus_8, 0);
		l_IPDU_HND_SONAR_GeneralStatus_8 = 1;
	}

	if(semiSonar == 0)
	{
		if(l_IPDU_HND_SONAR_GeneralStatus_8)
		{
			semiCount[semiSonarIndex]++;
			if(semiCount[semiSonarIndex] >= 3)
			{
				semiSonar = 2;
				semiCount[semiSonarIndex] = 0;
				l_semiActiveFlag = 1;
			}
		}
	}
	else if(semiSonar == 2)	//rear only
	{
		if(l_IPDU_HND_SONAR_GeneralStatus_8)
		{
			semiCount[semiSonarIndex]++;
			if(semiCount[semiSonarIndex] >= 3)
			{
				semiSonar = 1;
				//semiEAPM = 1;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(semiEAPM == 0)
	{
		if(l_IPDU_HND_SONAR_GeneralStatus_8)
		{
			Com_RxSig_GetSETTEING_EAPM(&l_value);
			if(l_value == 0b10)
			{
				semiCount[semiEAPMIndex]++;
				if(semiCount[semiEAPMIndex] >= 3)
				{
					semiEAPM = 1;
					l_semiActiveFlag = 1;
				}
			}
		}
	}

	if(semiLED_Headlamp == 0)
	{
		if((GetPinValue_IOC_LED_HEAD_LAMP_L() == 0) || (GetPinValue_IOC_LED_HEAD_LAMP_R() == 0))
		{
			if(s_LED_HeadLampCnt < 10) //100ms
			{	
				s_LED_HeadLampCnt++;
			}
			else
			{
				semiLED_Headlamp = 1;
				l_semiActiveFlag = 1;
			}
		}
		else
		{
			s_LED_HeadLampCnt = 0;
		}
	}

#if 0
	if(semiEAPM == 0)
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_SONAR_GeneralStatus_8))
		{
			semiEAPM = 1;
			l_semiActiveFlag = 1;
		}
	}
#endif
	
	/* Function_H-22-2 */
	if(semiActiveITS_TJP == 0u) /* yangxl */
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N3_7b))		
		{
			//Com_RxSig_GetSETTEING_Steering_Assist(&l_value);
			if((l_value == 0b01) || (l_value == 0b10))
			{
				semiActiveITS_TJP = 1u;
				l_semiActiveFlag = 1u;
			}
		}
	}
	if(semiActiveITS_ICC == 0u)  /* yangxl */
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N2_1))		
		{
			//Com_RxSig_GetRequest_Cruise_Display(&l_value);
			if(l_value == 0b01)
			{
				semiActiveITS_ICC = 1u;
				l_semiActiveFlag = 1u;				
			}
		}
	}

	if(semiActiveITS_ACC == 0U) /* yangxl */
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N2_1))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N2_1, 0u);
			semiCount[semiActiveITS_ACCIndex]++;
			if(semiCount[semiActiveITS_ACCIndex] >= 3u)
			{
				semiActiveITS_ACC = 1u;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(semiActiveITS_LKA == 0U) /* yangxl */
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N3_7b))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ITS_InfoStatus_N3_7b, 0u);
			semiCount[semiActiveITS_LKAIndex]++;
			if(semiCount[semiActiveITS_LKAIndex] >= 3u)
			{
				semiActiveITS_LKA = 1u;
				l_semiActiveFlag = 1;
			}
		}
	}

	if(semiActiveShiftByWire == 0U) /* yangxl */
	{
		if(DEA_GetCanReceiveIndStat(IPDU_HND_ShBW_GeneralStatus))
		{
			DEA_SetCanReceiveIndStat(IPDU_HND_ShBW_GeneralStatus, 0u);
			semiCount[semiActiveShiftByWireIndex]++;
			if(semiCount[semiActiveShiftByWireIndex] >= 3u)
			{
				semiActiveShiftByWire = 1u;
				l_semiActiveFlag = 1;
			}
		}
	}


	if(l_semiActiveFlag)
	{
		semiActiveUpdateFlagSet(1);
	}
}

