#ifndef _DEA_CODING_H_
#define _DEA_CODING_H_

#include "Std_Types.h"

#define DEA_CODING_DEBUG

#define DEA_CODING_OFF	(0)
#define DEA_CODING_ON	(1)

#define DEA_CODING_DCT	(1)

#define DEA_CODING_VIN_LEN	(7)

#define DEA_CodingTransmissionTypeDefault	(0)	//DCT
#define DEA_CodingFCWDefalut 				(1)	//With
#define DEA_CodingHSADefalut 				(1)	//With
#define DEA_CodingHDCDefalut 				(1)	//With
#define DEA_CodingSBRDefalut 				(0)	//Without
#define DEA_CodingPDCDefalut 				(0)	//with 4 Sensor,no BSD
#define DEA_CodingTPMSDefalut 				(1)	//With
#define DEA_CodingTelematicsSystemDefalut 	(1)	//With
#define DEA_CodingElectricTailGateDefalut 	(1)	//With
#define DEA_CodingStartStopSystemDefalut 	(1)	//With
#define DEA_CodingShiftTipDefalut 			(1)	//With
#define DEA_CodingHUDDefalut 				(1)	//With
#define DEA_CodingWirelessChargingDefalut 	(1)	//With
#define DEA_CodingOATOnOffDefalut 			(1)	//With
#define DEA_CodingIACOnOffDefalut 			(1)	//With
#define DEA_CodingLDWSOnOffDefalut 			(0)	//Without


typedef enum
{
	DEA_CodingWith4SensorNoBSD,
	DEA_CodingWith6SensorNoBSD,
	DEA_CodingWithBSDAnd4Sensor,
	DEA_CodingWithBSDAnd6Sensor,
}DEA_CODING_PDC_ENUM;

void DEA_CodingInit(void);
void DEA_SetCodingUpdateFlag(void);
uint8_t DEA_GetCodingVINValid(void);
uint8_t DEA_GetCodingVIN(uint8_t *vinBuf);
uint8_t DEA_GetCodingTransmissionType(void);
uint8_t DEA_GetCodingFCW(void);
uint8_t DEA_GetCodingHSA(void);	//Hill Start Assist
uint8_t DEA_GetCodingHDC(void);
uint8_t DEA_GetCodingSBR(void);
DEA_CODING_PDC_ENUM DEA_GetCodingPDC(void);
uint8_t DEA_GetCodingTPMS(void);
uint8_t DEA_GetCodingTelematicsSystem(void);
uint8_t DEA_GetCodingElectricTailGate(void);
uint8_t DEA_GetCodingStartStopSystem(void);
uint8_t DEA_GetCodingShiftTip(void);
uint8_t DEA_GetCodingHUD(void);
uint8_t DEA_GetCodingWirelessCharging(void);
uint8_t DEA_GetCodingOATOnOff(void);
uint8_t DEA_GetCodingIACOnOff(void);	//energy regeneration system(IAC) 
uint8_t DEA_GetCodingLDWS(void);
void DEA_SetCodingUpdateFlag(void);
#endif
