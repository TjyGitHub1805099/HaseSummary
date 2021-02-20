#include "DeaCoding.h"
//#include "gsc0x22.h"


uint8_t g_DEA_CodingTransmissionType = 0;
uint8_t g_DEA_CodingFCW = 0;
uint8_t g_DEA_CodingHSA = 0;	//Hill Start Assist
uint8_t g_DEA_CodingHDC = 0;
uint8_t g_DEA_CodingSBR = 0;
DEA_CODING_PDC_ENUM g_DEA_CodingPDC = (DEA_CODING_PDC_ENUM)0;
uint8_t g_DEA_CodingTPMS = 0;
uint8_t g_DEA_CodingTelematicsSystem = 0;
uint8_t g_DEA_CodingElectricTailGate = 0;
uint8_t g_DEA_CodingStartStopSystem = 0;
uint8_t g_DEA_CodingShiftTip = 0;
uint8_t g_DEA_CodingHUD = 0;
uint8_t g_DEA_CodingWirelessCharging = 0;
uint8_t g_DEA_CodingOATOnOff = 0;
uint8_t g_DEA_CodingIACOnOff = 0;	//energy regeneration system(IAC) 
uint8_t g_DEA_CodingLDWSOnOff = 0;
uint8_t g_DEA_CodingVin[DEA_CODING_VIN_LEN]= {0};
uint8_t g_DEA_CodingVinVald = 0;

void DEA_CodingInit(void)
{
#ifndef DEA_CODING_DEBUG	
	uint8_t l_cnt = 0;
	tDidCDS p_cds;
	p_cds = *(sid0x22GetCDS());

	for(l_cnt = 0; l_cnt < DEA_CODING_VIN_LEN; l_cnt++)
	{
		if(p_cds.ShortVIN[l_cnt] == 0xFF)
		{
			break;
		}
	}
	if(l_cnt >= DEA_CODING_VIN_LEN)
	{
		memcpy(g_DEA_CodingVin, p_cds.ShortVIN, DEA_CODING_VIN_LEN);
		g_DEA_CodingVinVald = 1;
	}
	g_DEA_CodingTransmissionType = p_cds.TransmissionType;
	g_DEA_CodingFCW = p_cds.FCWFrontCWandEmergBrakeAid;
	g_DEA_CodingHSA = p_cds.HSAHillStartAssit;	//Hill Start Assist
	g_DEA_CodingHDC = p_cds.HDCHillDecentCtrl;
	g_DEA_CodingSBR = p_cds.SBRRearreminder;
	g_DEA_CodingPDC = p_cds.PDC;
	g_DEA_CodingTPMS = p_cds.TirePressureMonitoringSys;
	g_DEA_CodingTelematicsSystem = p_cds.TelematicsSys;
	g_DEA_CodingElectricTailGate = p_cds.ElectricTailGate;
	g_DEA_CodingStartStopSystem = p_cds.StartStopSys;
	g_DEA_CodingShiftTip = p_cds.ShiftTip;
	g_DEA_CodingHUD = p_cds.HUDHeadUpDisplay;
	g_DEA_CodingWirelessCharging = p_cds.WirelessCharging;
	g_DEA_CodingOATOnOff = p_cds.AmbinenttempInd;
	g_DEA_CodingIACOnOff = p_cds.EnergyRegSys;	//energy regeneration system(IAC) 
	g_DEA_CodingLDWSOnOff = p_cds.LaneDepartureWarningSystem;
#else
	g_DEA_CodingTransmissionType = 1;
	g_DEA_CodingFCW = 1;
	g_DEA_CodingHSA = 1;	//Hill Start Assist
	g_DEA_CodingHDC = 1;
	g_DEA_CodingSBR = 1;
	g_DEA_CodingPDC = DEA_CodingWith4SensorNoBSD;
	g_DEA_CodingTPMS = 1;
	g_DEA_CodingTelematicsSystem = 1;
	g_DEA_CodingElectricTailGate = 1;
	g_DEA_CodingStartStopSystem = 1;
	g_DEA_CodingShiftTip = 1;
	g_DEA_CodingHUD = 1;
	g_DEA_CodingWirelessCharging = 1;
	g_DEA_CodingOATOnOff = 1;
	g_DEA_CodingIACOnOff = 1;	//energy regeneration system(IAC) 
	g_DEA_CodingLDWSOnOff = 1;
	g_DEA_CodingVinVald = 1;
#endif
}

void DEA_SetCodingUpdateFlag(void)
{
#if 0
	uint8_t l_cnt = 0;
	tDidCDS p_cds;
	p_cds = *(sid0x22GetCDS());
	for(l_cnt = 0; l_cnt < DEA_CODING_VIN_LEN; l_cnt++)
	{
		if(p_cds.ShortVIN[l_cnt] == 0xFF)
		{
			break;
		}
	}
	if(l_cnt >= DEA_CODING_VIN_LEN)
	{
		memcpy(g_DEA_CodingVin, p_cds.ShortVIN, DEA_CODING_VIN_LEN);
	}
	if(p_cds.TransmissionType > 1)	//reserved or invalid
	{
		g_DEA_CodingTransmissionType = DEA_CodingTransmissionTypeDefault;
	}
	else
	{
		g_DEA_CodingTransmissionType = p_cds.TransmissionType;
	}

	if(p_cds.FCWFrontCWandEmergBrakeAid > 1)	//reserved or invalid
	{
		g_DEA_CodingFCW = DEA_CodingFCWDefalut;
	}
	else
	{
		g_DEA_CodingFCW = p_cds.FCWFrontCWandEmergBrakeAid;
	}

	
	if(p_cds.HSAHillStartAssit > 1)	//reserved or invalid
	{
		g_DEA_CodingHSA = DEA_CodingHSADefalut;
	}
	else
	{
		g_DEA_CodingHSA = p_cds.HSAHillStartAssit;
	}


	if(p_cds.HDCHillDecentCtrl > 1)	//reserved or invalid
	{
		g_DEA_CodingHDC = DEA_CodingHDCDefalut;
	}
	else
	{
		g_DEA_CodingHDC = p_cds.HDCHillDecentCtrl;
	}

	if(p_cds.SBRRearreminder > 1)	//reserved or invalid
	{
		g_DEA_CodingSBR = DEA_CodingSBRDefalut;
	}
	else
	{
		g_DEA_CodingSBR = p_cds.SBRRearreminder;
	}

	if(p_cds.PDC > 3)	//reserved or invalid
	{
		g_DEA_CodingPDC = (DEA_CODING_PDC_ENUM)DEA_CodingPDCDefalut;
	}
	else
	{
		g_DEA_CodingPDC = (DEA_CODING_PDC_ENUM)p_cds.PDC;
	}

	if(p_cds.TirePressureMonitoringSys > 1)	//reserved or invalid
	{
		g_DEA_CodingTPMS = DEA_CodingTPMSDefalut;
	}
	else
	{
		g_DEA_CodingTPMS = p_cds.TirePressureMonitoringSys;
	}

	if(p_cds.TelematicsSys > 1)	//reserved or invalid
	{
		g_DEA_CodingTelematicsSystem = DEA_CodingTelematicsSystemDefalut;
	}
	else
	{
		g_DEA_CodingTelematicsSystem = p_cds.TelematicsSys;
	}

	if(p_cds.ElectricTailGate > 1)	//reserved or invalid
	{
		g_DEA_CodingElectricTailGate = DEA_CodingElectricTailGateDefalut;
	}
	else
	{
		g_DEA_CodingElectricTailGate = p_cds.ElectricTailGate;
	}

	if(p_cds.StartStopSys > 1)	//reserved or invalid
	{
		g_DEA_CodingTelematicsSystem = DEA_CodingStartStopSystemDefalut;
	}
	else
	{
		g_DEA_CodingTelematicsSystem = p_cds.StartStopSys;
	}

	if(p_cds.ShitTip > 1)	//reserved or invalid
	{
		g_DEA_CodingShiftTip = DEA_CodingShiftTipDefalut;
	}
	else
	{
		g_DEA_CodingShiftTip = p_cds.ShitTip;
	}

	if(p_cds.HUDHeadUpDisplay > 1)	//reserved or invalid
	{
		g_DEA_CodingHUD = DEA_CodingHUDDefalut;
	}
	else
	{
		g_DEA_CodingHUD = p_cds.HUDHeadUpDisplay;
	}

	if(p_cds.WirelessCharging > 1)	//reserved or invalid
	{
		g_DEA_CodingWirelessCharging = DEA_CodingWirelessChargingDefalut;
	}
	else
	{
		g_DEA_CodingWirelessCharging = p_cds.WirelessCharging;
	}

	if(p_cds.AmbinenttempInd > 1)	//reserved or invalid
	{
		g_DEA_CodingOATOnOff = DEA_CodingOATOnOffDefalut;
	}
	else
	{
		g_DEA_CodingOATOnOff = p_cds.AmbinenttempInd;
	}

	if(p_cds.EnergyRegSys > 1)	//reserved or invalid
	{
		g_DEA_CodingIACOnOff = DEA_CodingIACOnOffDefalut;
	}
	else
	{
		g_DEA_CodingIACOnOff = p_cds.EnergyRegSys;
	}

	if(p_cds.LDWS > 1)	//reserved or invalid
	{
		g_DEA_CodingLDWSOnOff = DEA_CodingLDWSOnOffDefalut;
	}
	else
	{
		g_DEA_CodingLDWSOnOff = p_cds.LDWS;
	}
#endif
}

uint8_t DEA_GetCodingVINValid(void)
{
	return g_DEA_CodingVinVald;
}

uint8_t DEA_GetCodingVIN(uint8_t *vinBuf)
{
	memcpy(vinBuf, g_DEA_CodingVin, DEA_CODING_VIN_LEN);
	return 0;
}

uint8_t DEA_GetCodingTransmissionType(void)
{
	return g_DEA_CodingTransmissionType;
}

uint8_t DEA_GetCodingFCW(void)
{
	return g_DEA_CodingFCW;
}

uint8_t DEA_GetCodingHSA(void)	//Hill Start Assist
{
	return g_DEA_CodingHSA;
}

uint8_t DEA_GetCodingHDC(void)
{
	return g_DEA_CodingHDC;
}

uint8_t DEA_GetCodingSBR(void)
{
	return g_DEA_CodingSBR;
}

DEA_CODING_PDC_ENUM DEA_GetCodingPDC(void)
{
	return g_DEA_CodingPDC;
}

uint8_t DEA_GetCodingTPMS(void)
{
	return g_DEA_CodingTPMS;
}

uint8_t DEA_GetCodingTelematicsSystem(void)
{
	return g_DEA_CodingTelematicsSystem;
}

uint8_t DEA_GetCodingElectricTailGate(void)
{
	return g_DEA_CodingElectricTailGate;
}

uint8_t DEA_GetCodingStartStopSystem(void)
{
	return g_DEA_CodingStartStopSystem;
}

uint8_t DEA_GetCodingShiftTip(void)
{
	return g_DEA_CodingShiftTip;
}

uint8_t DEA_GetCodingHUD(void)
{
	return g_DEA_CodingHUD;
}

uint8_t DEA_GetCodingWirelessCharging(void)
{
	return g_DEA_CodingWirelessCharging;
}

uint8_t DEA_GetCodingOATOnOff(void)
{
	return g_DEA_CodingOATOnOff;
}

uint8_t DEA_GetCodingIACOnOff(void)	//energy regeneration system(IAC) 
{
	return g_DEA_CodingIACOnOff;
}

uint8_t DEA_GetCodingLDWS(void)
{
	return g_DEA_CodingLDWSOnOff;
}
