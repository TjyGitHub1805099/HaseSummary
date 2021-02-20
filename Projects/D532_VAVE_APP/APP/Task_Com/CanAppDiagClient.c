#include "dea.h"
#include "CanAppDiagClient.h"
#include "CanIf_Cfg.h"


CAN_APP_DIAG_CODING_READ_ENUM CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
uint8_t CanAppDiagSessionSwitchOkFlag = 0;
uint8_t CanAppDiagReadWriteOkFlag = 0;
uint8_t CanAppDiagTimerOutCnt = 0;
uint8_t CanAppDiagTryCnt = 0;

uint8_t CanAppDiagWriteRequestFlag = 0;	//VehicleReset;
uint8_t CanAppDiagWriteRequestValue = 0;

uint8_t CanAppDiagReadWriteRequestSend = 0;
uint8_t CanAppDiagReadWriteSessionSwSend = 0;

uint8_t CanAppDiagWriteSessionSwFlag = 0;

uint8_t CarBodySetInProgressFlag = 0;
uint16_t CanAppDiagTimerCnt = 0;

#if 0
void CanAppDiagRxProc(uint8_t *data, uint16_t len)
{
	uint8 data_buf[8] = {0};
	if(len <= sizeof(data_buf))
	{
		memcpy(data_buf, data, len);
		switch(data_buf[1])	//SID
		{
			case 0x50:
				if(data_buf[2] == 0x03) //yangxl 2019-7-18 15:32:53
				{
					CanAppDiagSessionSwitchOkFlag = 1;
				}
				else
				{
					CanAppDiagSessionSwitchOkFlag = 2;	//invalid
				}
				break;
			case 0x62:
				if(data_buf[2] == 0x12)
				{
					switch(data_buf[3])
					{
						case 0x01:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiLightSensitivityFlagGet() == 0)
								{
									semiLightSensitivityFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b11 && SysSettingLightSensitivityGet() != data_buf[4])
							{
								SysSettingLightSensitivityUpdate(data_buf[4]);
							}
							break;
						case 0x04:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_DELAY_TIMER)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiLightOffDayFlagGet() == 0)
								{
									semiLightOffDayFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b111 && SysSettingLightOffDayGet() != data_buf[4])
							{
								SysSettingLightOffDayUpdate(data_buf[4]);
							}
							break;
						case 0x10:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiSelectiveUnlockFlagGet() == 0)
								{
									semiSelectiveUnlockFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b01 && SysSettingSelectiveUnlockGet() != data_buf[4])
							{
								SysSettingSelectiveUnlockUpdate(data_buf[4]);
							}
							break;
						case 0x21:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiWiperWithSpeedFlagGet() == 0)
								{
									semiWiperWithSpeedFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b01 && SysSettingWiperWithSpeedGet() != data_buf[4])
							{
								SysSettingWiperWithSpeedUpdate(data_buf[4]);
							}
							break;
						default:
							break;
					}
				}
				break;
			case 0x6E:
				if(data_buf[2] == 0x12)
				{
					switch(data_buf[3])
					{
						case 0x01:
							if(CanAppDiagWriteRequestFlag == LightSensitivity)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingLightSensitivityUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x04:
							if(CanAppDiagWriteRequestFlag == LightOffDay)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingLightOffDayUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x10:
							if(CanAppDiagWriteRequestFlag == SelectiveUnlock)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingSelectiveUnlockUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x21:
							if(CanAppDiagWriteRequestFlag == WiperWithSpeed)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingWiperWithSpeedUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x1F:
							if(CanAppDiagWriteRequestFlag == VehicleReset)
							{
								CanAppDiagWriteRequestFlag = 0;
								CanAppDiagWriteRequestValue = 0;
								CanAppDiagWriteSessionSwFlag = 0;
								CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
							}
							break;
						default:
							break;
					}
				}
				break;
			case 0x7F:
			default:
				if((CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_SW_SESSION) || (CanAppDiagReadWriteSessionSwSend == 1))
				{
					CanAppDiagSessionSwitchOkFlag = 2;	//invalid
				}
				else
				{	
					CanAppDiagReadWriteOkFlag = 2; //invalid
				}
				break;
		}
	}
}
#else
void CanAppDiagRxProc(uint8_t *data, uint16_t len)
{
	uint8 data_buf[8] = {0};
	if(len <= sizeof(data_buf))
	{
		memcpy(data_buf, data, len);
		switch(data_buf[1])	//SID
		{
			case 0x50:
				if(data_buf[2] == 0x03) //yangxl 2019-7-18 15:32:53
				{
					CanAppDiagSessionSwitchOkFlag = 1;
				}
				else
				{
					CanAppDiagSessionSwitchOkFlag = 2;	//invalid
				}
				break;
			case 0x62:
				if(data_buf[2] == 0x12)
				{
					switch(data_buf[3])
					{
						case 0x01:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiLightSensitivityFlagGet() == 0)
								{
									semiLightSensitivityFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b11 && SysSettingIntensityOfLightGet() != data_buf[4])
							{
								SysSettingIntensityOfLightUpdate(data_buf[4]);
							}
							break;
						case 0x04:
							if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_DELAY_TIMER)
							{
								CanAppDiagReadWriteOkFlag = 1;
								if(semiLightOffDayFlagGet() == 0)
								{
									semiLightOffDayFlagSet(1);
								}
							}
							if(data_buf[4] <= 0b111 && SysSettingTurnOffLightsDelayGet() != data_buf[4])
							{
								SysSettingTurnOffLightsDelayUpdate(data_buf[4]);
							}
							break;
						case 0x09:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION)
								{
									CanAppDiagReadWriteOkFlag = 1;
									
									if(semiAutoFoldFunctionFlagGet() == 0)
									{
										semiAutoFoldFunctionFlagSet(1);
									}
									
								}
								if((data_buf[4] <= 0b01) && (SysSettingRearviewMirrorFoldsAutomaticallyGet() != data_buf[4]))
								{
									SysSettingRearviewMirrorFoldsAutomaticallyUpdate(data_buf[4]);
								}
								break;
							}
						case 0x12:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiAutoLockFunctionCustomizeFlagGet() == 0)
									{
										semiAutoLockFunctionCustomizeFlagSet(1);
									}
								}
								if((data_buf[4] <= 0b01) && (SysSettingAutomaticLockingGet() != data_buf[4]))
								{
									SysSettingAutomaticLockingUpdate(data_buf[4]);
								}
								break;
							}
						case 0x13:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiLockAutoWindowUpFlagGet() == 0)
									{
										semiLockAutoWindowUpFlagSet(1);
									}
								}
								if((data_buf[4] <= 0b01) && (SysSettingAutomaticClosingWindowGet() != data_buf[4]))
								{
									SysSettingAutomaticClosingWindowUpdate(data_buf[4]);
								}
								break;
							}
						case 0x14:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiAutoUnlockFunctionCustomizeFlagGet() == 0)
									{
										semiAutoUnlockFunctionCustomizeFlagSet(1);
									}
								}
								if((data_buf[4] <= 0b01) && (SysSettingAutomaticUnlockGet() != data_buf[4]))
								{
									SysSettingAutomaticUnlockUpdate(data_buf[4]);
								}
								break;
							}
						case 0x17:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_ANSWER_BACK)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiAnswerBackFlagGet() == 0)
									{
										semiAnswerBackFlagSet(1);
									}
								}
								if((data_buf[4] <= 0b01) && (SysSettingCarLockPromptGet() != data_buf[4]))
								{
									SysSettingCarLockPromptUpdate(data_buf[4]);
								}
								break;
							}
						case 0x22:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_RAIN_SENSOR)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiRainSensorFlagGet() == 0)
									{
										semiRainSensorFlagSet(1);
									}
								}
								/*
								if((data_buf[4] <= 0b01) && (SysSettingCarLockPromptGet() != data_buf[4]))
								{
									SysSettingCarLockPromptSet(data_buf[4]);
								}
								*/
								break;
							}
						case 0x23:
							{
								if(CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP)
								{
									CanAppDiagReadWriteOkFlag = 1;
									if(semiWiperWithWipingDripFlagGet() == 0)
									{
										semiWiperWithWipingDripFlagSet(1);
									}
								}
								
								if((data_buf[4] <= 0b01) && (SysSettingBrushAwayTheRainGet() != data_buf[4]))
								{
									SysSettingBrushAwayTheRainUpdate(data_buf[4]);
								}
								
								break;
							}
						default:
							break;
					}
				}
				break;
			case 0x6E:
				if(data_buf[2] == 0x12)
				{
					switch(data_buf[3])
					{
						case 0x01:
							if(CanAppDiagWriteRequestFlag == LightSensitivity)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingIntensityOfLightUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x04:
							if(CanAppDiagWriteRequestFlag == LightOffDay)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingTurnOffLightsDelayUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x09:
							if(CanAppDiagWriteRequestFlag == AutoFoldFunction)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingRearviewMirrorFoldsAutomaticallyUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x12:
							if(CanAppDiagWriteRequestFlag == AutoLockFunctionCustomize)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingAutomaticLockingUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x13:
							if(CanAppDiagWriteRequestFlag == LockAutoWindowUp)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingAutomaticClosingWindowUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x14:
							if(CanAppDiagWriteRequestFlag == AutoUnlockFunctionCustomize)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingAutomaticUnlockUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x17:
							if(CanAppDiagWriteRequestFlag == AnswerBack)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingCarLockPromptUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x22:
							if(CanAppDiagWriteRequestFlag == RainSensor)
							{
								CanAppDiagReadWriteOkFlag = 1;
								//SysSettingWiperWithSpeedUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x23:
							if(CanAppDiagWriteRequestFlag == WiperWithWipingDrip)
							{
								CanAppDiagReadWriteOkFlag = 1;
								SysSettingBrushAwayTheRainUpdate(CanAppDiagWriteRequestValue);
							}
							break;
						case 0x1F:
							if(CanAppDiagWriteRequestFlag == VehicleReset)
							{
								CanAppDiagWriteRequestFlag = 0;
								CanAppDiagWriteRequestValue = 0;
								CanAppDiagWriteSessionSwFlag = 0;
								CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
							}
							break;
						default:
							break;
					}
				}
				break;
			case 0x7F:
				{
					switch(CanAppDiagWriteRequestFlag)
					{
						case CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING:
						{
							semiLightSensitivityFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_DELAY_TIMER:
						{
							semiLightOffDayFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION:
						{
							semiAutoFoldFunctionFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION:
						{
							semiAutoLockFunctionCustomizeFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP:
						{
							semiLockAutoWindowUpFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION:
						{
							semiAutoUnlockFunctionCustomizeFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_ANSWER_BACK:
						{
							semiAnswerBackFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_RAIN_SENSOR:
						{
							semiRainSensorFlagSet(0);
							break;
						}
						case CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP:
						{
							semiWiperWithWipingDripFlagSet(0);
							break;
						}
						default:
							break;
					}
				}
			default:
				if((CanAppDiagCodingReadFlag == CAN_APP_DIAG_CODING_READ_SW_SESSION) || (CanAppDiagReadWriteSessionSwSend == 1))
				{
					CanAppDiagSessionSwitchOkFlag = 2;	//invalid
				}
				else
				{	
					CanAppDiagReadWriteOkFlag = 2; //invalid
				}
				break;
		}
	}
}

#endif

void CanAppDiagClientSwitchSession(void)
{
	PduInfoType l_pdu;
	uint8 data_buf[8] = {0};

	data_buf[0] = 2;
	data_buf[1] = 0x10;
	data_buf[2] = 0x03;//yangxl 2019-7-18 15:32:53

	l_pdu.SduDataPtr = data_buf;
	l_pdu.SduLength = 8;
	CanIf_Transmit(CANIF_TX_PDU_CFG_CBK_ID_0, &l_pdu);
}

void CanAppDiagClientReadRequest(uint8_t B1, uint8_t B2)
{
	PduInfoType l_pdu;
	uint8 data_buf[8] = {0};

	data_buf[0] = 3;
	data_buf[1] = 0x22;
	data_buf[2] = B1;
	data_buf[3] = B2;

	l_pdu.SduDataPtr = data_buf;
	l_pdu.SduLength = 8;
	CanIf_Transmit(CANIF_TX_PDU_CFG_CBK_ID_0, &l_pdu);
}

void CanAppDiagClientWriteRequest(uint8_t B1, uint8_t B2, uint8_t value)
{
	PduInfoType l_pdu;
	uint8 data_buf[8] = {0};

	data_buf[0] = 4;
	data_buf[1] = 0x2E;
	data_buf[2] = B1;
	data_buf[3] = B2;
	data_buf[4] = value;

	l_pdu.SduDataPtr = data_buf;
	l_pdu.SduLength = 8;
	CanIf_Transmit(CANIF_TX_PDU_CFG_CBK_ID_0, &l_pdu);
}

void CarBodySetInProgressFlagSet(uint8_t value)
{
	CarBodySetInProgressFlag = value;
	IPC_SetCarBodySetInProgress(value);
	//SysSettingCarBodySetInProgressSet(value);
}

void CanAppDiagWriteRequestFlagSet(uint8_t falg, uint8_t value)
{
	if(DEA_GetCAN_MASK() != 1u) //DiagMuxOn != 1, STOP communication  yangxl 2020-4-28 16:43:18
	{
		return ;
	}
	
	if(CarBodySetInProgressFlag == 0)
	{
		CanAppDiagWriteRequestFlag = falg;
		CanAppDiagWriteRequestValue = value;
	}
}

void CanAppDiagFlagReset(void)
{
	CanAppDiagReadWriteOkFlag = 0;
	CanAppDiagReadWriteRequestSend = 0;
	CanAppDiagTryCnt = 0;
	CanAppDiagTimerCnt = 0;
}

uint8_t CanAppDiagClientReadFunc(uint16_t *pTimerCnt, uint8_t B1, uint8_t B2, CAN_APP_DIAG_CODING_READ_ENUM next_state)
{
	if(*pTimerCnt < (CAN_APP_DIAG_TOUT/CAN_APP_DIAG_TASK_PERIOD))
	{
		if(next_state == CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING) //now is CAN_APP_DIAG_CODING_READ_SW_SESSION
		{
			if(CanAppDiagReadWriteSessionSwSend == 0)
			{
				CanAppDiagClientSwitchSession();
				CanAppDiagReadWriteSessionSwSend = 1;
			}
			else if(CanAppDiagSessionSwitchOkFlag == 2)
			{
				if(CanAppDiagTryCnt < 2)
				{
					CanAppDiagTryCnt++;
					CanAppDiagClientSwitchSession();
					(*pTimerCnt) = 0;
					CanAppDiagSessionSwitchOkFlag = 0;
					CanAppDiagTimerOutCnt = 0;
				}
				else
				{
					CanAppDiagReadWriteSessionSwSend = 0;
					return 2;
				}
			}
			else if(CanAppDiagSessionSwitchOkFlag == 1)
			{
				(*pTimerCnt) = 0;
				CanAppDiagSessionSwitchOkFlag = 0;
				CanAppDiagTryCnt = 0;
				CanAppDiagTimerOutCnt = 0;
				CanAppDiagCodingReadFlag = next_state;
				CanAppDiagReadWriteSessionSwSend = 0;
			}
			else
			{
				(*pTimerCnt)++;
			}
		}
		else
		{
			if(CanAppDiagReadWriteRequestSend == 0)
			{
				CanAppDiagClientReadRequest(B1, B2);
				CanAppDiagReadWriteRequestSend = 1;
			}
			else if(CanAppDiagReadWriteOkFlag == 2)
			{
				if(CanAppDiagTryCnt < 3)
				{
					CanAppDiagTryCnt++;
					if(next_state != CAN_APP_DIAG_CODING_READ_COMPLETE)
					{
						CanAppDiagClientReadRequest(B1, B2);
						(*pTimerCnt) = 0;
						CanAppDiagReadWriteOkFlag = 0;
						CanAppDiagTimerOutCnt = 0;
					}
				}
				else
				{
					CanAppDiagReadWriteSessionSwSend = 0;
					return 2;
				}
			}
			else if(CanAppDiagReadWriteOkFlag == 1)
			{
				(*pTimerCnt) = 0;
				CanAppDiagReadWriteOkFlag = 0;
				CanAppDiagTryCnt = 0;
				CanAppDiagTimerOutCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagCodingReadFlag = next_state;
			}
			else
			{
				(*pTimerCnt)++;
			}
		}
		return 0;
	}
	else
	{
		CanAppDiagReadWriteSessionSwSend = 0;
		if(CanAppDiagTimerOutCnt < 1)
		{
			(*pTimerCnt) = 0;
			CanAppDiagTimerOutCnt++;
			return 0;
		}
		else
		{
			CanAppDiagTimerOutCnt = 0;
			return 1;
		}

	}
}

uint8_t CanAppDiagClientWriteFunc(uint8_t State, uint16_t *pTimerCnt, uint8_t B1, uint8_t B2, uint8_t value)
{
	if(*pTimerCnt < (CAN_APP_DIAG_TOUT/CAN_APP_DIAG_TASK_PERIOD))
	{
		if(State == SwitchSession)
		{
			if(CanAppDiagReadWriteSessionSwSend == 0)
			{
				CanAppDiagClientSwitchSession();
				CanAppDiagReadWriteSessionSwSend = 1;
			}
			else if(CanAppDiagSessionSwitchOkFlag == 2)
			{
				if(CanAppDiagTryCnt < 2)
				{
					CanAppDiagTryCnt++;
					if(State != DiagWriteNone)
					{
						CanAppDiagClientSwitchSession();
						(*pTimerCnt) = 0;
						CanAppDiagSessionSwitchOkFlag = 0;
						CanAppDiagTimerOutCnt = 0;
					}
				}
				else
				{
					CanAppDiagReadWriteSessionSwSend = 0;
					return 2;
				}
			}
			else if(CanAppDiagSessionSwitchOkFlag == 1)
			{
				(*pTimerCnt) = 0;
				CanAppDiagSessionSwitchOkFlag = 0;
				CanAppDiagTryCnt = 0;
				CanAppDiagTimerOutCnt = 0;
				CanAppDiagWriteSessionSwFlag = 1;
				CanAppDiagReadWriteSessionSwSend = 0;
			}
			else
			{
				(*pTimerCnt)++;
			}
		}
		else 
		{
			if(CanAppDiagReadWriteRequestSend == 0)
			{
				CanAppDiagClientWriteRequest(B1, B2, value);
				CanAppDiagReadWriteRequestSend = 1;
			}
			else if(CanAppDiagReadWriteOkFlag == 2)
			{
				if(CanAppDiagTryCnt < 3)
				{
					CanAppDiagTryCnt++;
					if(State != DiagWriteNone)
					{
						CanAppDiagClientWriteRequest(B1, B2, value);
						(*pTimerCnt) = 0;
						CanAppDiagReadWriteOkFlag = 0;
						CanAppDiagTimerOutCnt = 0;
					}
				}
				else
				{
					CanAppDiagReadWriteSessionSwSend = 0;
					return 2;
				}
			}
			else if(CanAppDiagReadWriteOkFlag == 1)
			{
				(*pTimerCnt) = 0;
				CanAppDiagReadWriteOkFlag = 0;
				CanAppDiagTryCnt = 0;
				CanAppDiagTimerOutCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			else
			{
				(*pTimerCnt)++;
			}
		}
		return 0;
	}
	else
	{
		CanAppDiagReadWriteSessionSwSend = 0;
		if(CanAppDiagTimerOutCnt < 1)
		{
			(*pTimerCnt) = 0;
			CanAppDiagTimerOutCnt++;
			return 0;
		}
		else
		{
			CanAppDiagTimerOutCnt = 0;
			return 1;
		}
	}
}

#if 0
uint8_t CanAppDiagClientWriteProcess(uint8_t Flag, uint16_t *pTimerCnt)
{
	uint8_t ret = E_OK;
	switch(Flag)
	{
		case SwitchSession:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0, 0, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case LightSensitivity:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x01, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case LightOffDay:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x04, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case SelectiveUnlock:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x10, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case WiperWithSpeed:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x21, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case VehicleReset:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x1F, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		default:
			ret = E_NOT_OK;
			break;
	}
	return ret;
}
#else
uint8_t CanAppDiagClientWriteProcess(uint8_t Flag, uint16_t *pTimerCnt)
{
	uint8_t ret = E_OK;
	switch(Flag)
	{
		case SwitchSession:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0, 0, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagWriteSessionSwFlag = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case LightSensitivity:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x01, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case LightOffDay:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x04, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case AutoFoldFunction:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x09, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case AutoLockFunctionCustomize:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x12, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case LockAutoWindowUp:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x13, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case AutoUnlockFunctionCustomize:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x14, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case AnswerBack:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x17, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case RainSensor:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x22, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case WiperWithWipingDrip:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x23, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		case VehicleReset:
			if(CanAppDiagClientWriteFunc(Flag, pTimerCnt, 0x12, 0x1F, CanAppDiagWriteRequestValue) != 0)
			{
				*pTimerCnt = 0;
				CanAppDiagReadWriteRequestSend = 0;
				CanAppDiagWriteRequestFlag = 0;
			}
			break;
		default:
			ret = E_NOT_OK;
			break;
	}
	return ret;
}

#endif

#if 0
void CanAppDiagClientProc(void)
{
	static IGN_STAT_ENUM s_IgnStatus = IGN_OFF;
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnStatus != IGN_ON)
		{
			CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
			
			s_IgnStatus = IGN_ON;	
		}
		
		if(CanAppDiagCodingReadFlag < CAN_APP_DIAG_CODING_READ_COMPLETE)
		{
			CarBodySetInProgressFlagSet(1);
			switch(CanAppDiagCodingReadFlag)
			{
				case CAN_APP_DIAG_CODING_READ_INIT:
					CanAppDiagTimerCnt = 0;
					CanAppDiagTimerOutCnt = 0;
					CanAppDiagReadWriteOkFlag = 0;
					CanAppDiagSessionSwitchOkFlag = 0;
					CanAppDiagReadWriteRequestSend = 0;
					CanAppDiagReadWriteSessionSwSend = 0;
					CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_SW_SESSION;
					break;
				case CAN_APP_DIAG_CODING_READ_SW_SESSION:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0, 0, CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING) != 0)
					{
						CanAppDiagTimerCnt = 0;
						CanAppDiagReadWriteOkFlag = 0;
						CanAppDiagReadWriteRequestSend = 0;
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE;	//stop read process
					}
					break;
				case CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x01, CAN_APP_DIAG_CODING_READ_DELAY_TIMER) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_DELAY_TIMER;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_DELAY_TIMER:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x04, CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_SELECT_UNLOCK:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x10, CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_WIPER_WITH_SPEED:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x21, CAN_APP_DIAG_CODING_READ_COMPLETE) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE;	//
					}
					break;
				default:
					CanAppDiagFlagReset();
					CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE;	//stop read process
					break;
			}
		}
		else
		{
			if(CanAppDiagWriteRequestFlag != 0)
			{	
				CarBodySetInProgressFlagSet(1);
				if(CanAppDiagWriteSessionSwFlag == 0)
				{
					CanAppDiagClientWriteProcess(SwitchSession, &CanAppDiagTimerCnt);
				}
				else
				{
					CanAppDiagClientWriteProcess(CanAppDiagWriteRequestFlag, &CanAppDiagTimerCnt);
				}
			}
			else
			{
				CarBodySetInProgressFlagSet(0);
				CanAppDiagWriteSessionSwFlag = 0;
			}
		}
	}
	else
	{
		if(s_IgnStatus != IGN_OFF)
		{
			s_IgnStatus = IGN_OFF;
			semiCarAssistSettingReset();
			CarBodySetInProgressFlagSet(0);
		}
	}
}
#else
void CanAppDiagClientProc(void)
{
	static IGN_STAT_ENUM s_IgnStatus = IGN_OFF;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnStatus != IGN_ON)
		{
			CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
			
			s_IgnStatus = IGN_ON;	
		}
		
		if(0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_GeneralStatus))
		{
			if(DEA_GetCAN_MASK() != 1u) // //DiagMuxOn != 1, STOP communication  yangxl 2020-6-2 14:35:06
			{
				CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
				return ;
			}
		}else
		{
			CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_INIT;
			return ;
		}
		
		
		if(CanAppDiagCodingReadFlag < CAN_APP_DIAG_CODING_READ_COMPLETE)
		{
			CarBodySetInProgressFlagSet(1);
			switch(CanAppDiagCodingReadFlag)
			{
				case CAN_APP_DIAG_CODING_READ_INIT:
					CanAppDiagTimerCnt = 0;
					CanAppDiagTimerOutCnt = 0;
					CanAppDiagReadWriteOkFlag = 0;
					CanAppDiagSessionSwitchOkFlag = 0;
					CanAppDiagReadWriteRequestSend = 0;
					CanAppDiagReadWriteSessionSwSend = 0;
					CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_SW_SESSION;
					break;
				case CAN_APP_DIAG_CODING_READ_SW_SESSION:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0, 0, CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING) != 0)
					{
						CanAppDiagTimerCnt = 0;
						CanAppDiagReadWriteOkFlag = 0;
						CanAppDiagReadWriteRequestSend = 0;
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE;	//stop read process
					}
					break;
				case CAN_APP_DIAG_CODING_READ_AUTO_LIGHT_TIMING:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x01, CAN_APP_DIAG_CODING_READ_DELAY_TIMER) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_DELAY_TIMER;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_DELAY_TIMER:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x04, CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_AUTOFOLD_FUNCTION:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x09, CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_AUTO_LOCK_FUNCTION:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x12, CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP;	//
					}
					break;
				case CAN_APP_DIAG_CODING_READ_LOCK_AUTO_WINDOWUP:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x13, CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION; //
					}
					break;
				case CAN_APP_DIAG_CODING_READ_AUTO_UNLOCK_FUNCTION:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x14, CAN_APP_DIAG_CODING_READ_ANSWER_BACK) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_ANSWER_BACK; //
					}
					break;
				case CAN_APP_DIAG_CODING_READ_ANSWER_BACK:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x17, CAN_APP_DIAG_CODING_READ_RAIN_SENSOR) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_RAIN_SENSOR; //
					}
					break;
				case CAN_APP_DIAG_CODING_READ_RAIN_SENSOR:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x22, CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP; //
					}
					break;
				case CAN_APP_DIAG_CODING_READ_WIPER_WITH_WIPING_DRIP:
					if(CanAppDiagClientReadFunc(&CanAppDiagTimerCnt, 0x12, 0x23, CAN_APP_DIAG_CODING_READ_COMPLETE) != 0)
					{
						CanAppDiagFlagReset();
						CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE; //
					}
					break;

				default:
					CanAppDiagFlagReset();
					CanAppDiagCodingReadFlag = CAN_APP_DIAG_CODING_READ_COMPLETE;	//stop read process
					break;
			}
		}
		else
		{
			if(CanAppDiagWriteRequestFlag != 0)
			{	
				CarBodySetInProgressFlagSet(1);
				if(CanAppDiagWriteSessionSwFlag == 0)
				{
					CanAppDiagClientWriteProcess(SwitchSession, &CanAppDiagTimerCnt);
				}
				else
				{
					CanAppDiagClientWriteProcess(CanAppDiagWriteRequestFlag, &CanAppDiagTimerCnt);
				}
			}
			else
			{
				CarBodySetInProgressFlagSet(0);
				CanAppDiagWriteSessionSwFlag = 0;
			}
		}
	}
	else
	{
		if(s_IgnStatus != IGN_OFF)
		{
			s_IgnStatus = IGN_OFF;
			semiCarAssistSettingReset();
			CarBodySetInProgressFlagSet(0);
		}
	}
}

#endif
