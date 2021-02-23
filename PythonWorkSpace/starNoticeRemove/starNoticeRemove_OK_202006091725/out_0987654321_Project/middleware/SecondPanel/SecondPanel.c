

#include "SecondPanel.h"
#include "lpuart_driver.h"
#include "trace.h"


uint8 g_cPanelRxRawDataBuffer[PANEL_RX_RAW_DATA_BUFF_LEN];
uint8 g_cUartPanelDeviceInitDone = FALSE;
uint16 g_sSecondPanelSendDataFlag = 0xffff;  



uint8 gUartDriverRawDataBuffer[PANEL_RX_RAW_DATA_BUFF_LEN];
uint8 gUartRxFrameLen = 0;

SecondPanelPara stSecondPanelPara =
{
	.OutPM25Value = 100,
	.InPM25Value = 100,
	.OutTemp = 25,
	.InTemp = 25,
	.SoundWare = 0,
	.LightnessValue = SecondPanel_Pix100,
	.stTimeListValue.hour = 0,
	.stTimeListValue.minutes = 0,
};

void UartPanelRxCbk(void *driverState, uart_event_t event, void *userData)
{
	uint8 rxCount = 0;


	
	uint8 i = 0;
	uint8 *rxBufPtr = gUartDriverRawDataBuffer;
















	if(event == UART_EVENT_RX_FULL || event == UART_EVENT_END_TRANSFER)
	{
		while((rxCount = trace_LPUART_DRV_GetFifoRxCount(PANEL_UART_CH))>0)
		{
			rxBufPtr = gUartDriverRawDataBuffer;
			for(i=0; i<rxCount; i++)
			{
				trace_LPUART_DRV_GetData(PANEL_UART_CH,rxBufPtr);
				rxBufPtr++;
			}








			rxCount = 0;
		}











	}
}


uint8 UART2Init(void)
{
	status_t l_status = STATUS_ERROR;
	boolean ret = FALSE;




		uart_acpanel_cfg_Config0.baudRate = 9600;
		uart_acpanel_cfg_Config0.rxCallback = UartPanelRxCbk;
		uart_acpanel_cfg_Config0.transferType = UART_USING_INTERRUPTS;
		l_status = UART_Init(PANEL_UART_CH, &uart_acpanel_cfg_Config0);

		if(l_status == STATUS_SUCCESS)
		{

			ret = TRUE;


				g_cUartPanelDeviceInitDone = TRUE;
				LPUART_DRV_ReceiveData(PANEL_UART_CH, g_cPanelRxRawDataBuffer, PANEL_RX_RAW_DATA_BUFF_LEN);

		}

	return ret;
}


void SecondPanelInit(void)
{
	if(UART2Init() != E_NOT_OK)  
	{
		ErrorHandler();
	}
}

void ErrorHandler(void)
{



}


uint8 GetSecondPanelPM25Value(uint8 cType, uint8 *cData)
{
	int nData = 0;

	switch(cType)
	{
	case SecondPanel_Outside:
		nData = *((int *)cData);
		if((nData < 0) || (nData >= 1000))
		{
			return E_NOT_OK;
		}		
		stSecondPanelPara.OutPM25Value = nData;
		if(stSecondPanelPara.OutPM25Value > 1000)
		{
			stSecondPanelPara.OutPM25Value = 999;
		}		
		SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsidePM25);
		break;

	case SecondPanel_Inside:
		nData = *((short *)cData);
		if((nData < 0) || (nData >= 1000))
		{
			return E_NOT_OK;
		}
		stSecondPanelPara.InPM25Value = nData;
		if(stSecondPanelPara.InPM25Value > 1000)
		{
			stSecondPanelPara.InPM25Value = 999;
		}		
		SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsidePM25);
		break;

	default:
		break;
	}

	return E_OK;
}


uint8 GetSecondPanelTemp(uint8 cType, uint8 *cData)
{
	int nData = 0;
	int nSulData = 0;

	switch(cType)
	{
	case SecondPanel_Outside:
		nData = *((int *)cData);
		if(nData < 0)
		{
			nSulData = ~nData + 1;
		}
		else 
		{
			nSulData = nData;
		}
		
		if(nSulData >= 100)
		{
			return E_NOT_OK;
		}
		stSecondPanelPara.OutTemp = nData;
		SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsideTemp);
		break;

	case SecondPanel_Inside:
		nData = *((char *)cData);
		if(nData < 0)
		{
			nSulData = ~nData + 1;
		}
		else 
		{
			nSulData = nData;
		}
		
		if(nSulData >= 100)
		{
			return E_NOT_OK;
		}
		stSecondPanelPara.InTemp = nData;
		SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsideTemp);
		break;

	default:
		break;
	}

	return E_OK;
}


uint8 GetSecondPanelSoundWare(uint8 cData)
{
	
		
	stSecondPanelPara.SoundWare = cData;
	if(SoundWare_TurnOn == stSecondPanelPara.SoundWare)
	{
		SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWareEnable);
	}
	if(SoundWare_TurnOff == stSecondPanelPara.SoundWare)
	{
		ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWareEnable);
	}	

	SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWare);
	
	if((E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWareEnable)) && (SoundWare_TurnOn != stSecondPanelPara.SoundWare))
	{
		
		
	}
	return E_OK;
}


uint8 GetSecondPanelLightnessValue(uint8 cData)
{
	stSecondPanelPara.LightnessValue = cData;
	SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidLightness);
	
	return E_OK;
}


uint8 GetSecondPanelRtcTimeValue(void)
{
	ExternSTDType stRtcTime;

	if(E_NOT_OK == GetRtcTime(&stRtcTime))
	{
		return E_NOT_OK;
	}

	stSecondPanelPara.stTimeListValue.hour = stRtcTime.hour;
	stSecondPanelPara.stTimeListValue.minutes = stRtcTime.minutes;

	SetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidRtcTime);
	
	return E_OK;
}


uint8 SendDataToSecondPanel(void)
{
	static uint8 i = 0;
	uint8 j = 0;
	static uint8 cCycleUpdateDataFlag = 0;  

	if(E_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidEnable))  
	{
		switch (i)
		{
			case 0:
			case 1:
			case 2:
				UART2SendDataFromSwitch(SecondPanel_TurnOn);
				i++;
				return E_OK;
				break;
				
			case 3:
				GetSecondPanelSoundWare(SoundWare_TurnOn);
				i++;
				return E_OK;
				break;
				
			default:
				break;
		}

		if(cCycleUpdateDataFlag >= CYCLE_UPADATE_DATA_NUM)
		{
			cCycleUpdateDataFlag = 0;
		}
		switch (cCycleUpdateDataFlag)
		{
			case 0:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsidePM25))
				{
					UART2SendDataFromPM25(SecondPanel_Outside);
				}
				cCycleUpdateDataFlag++;
				break;
				
			case 1:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsidePM25))
				{
					UART2SendDataFromPM25(SecondPanel_Inside);
				}
				cCycleUpdateDataFlag++;
				break;

			case 2:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidRtcTime))
				{
					UART2SendDataFromRtcTime();
				}
				cCycleUpdateDataFlag++;
				break;
				
			case 3:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWare))
				{
					UART2SendDataFromSoundWare();
				}
				cCycleUpdateDataFlag++;
				break;

			case 4:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidLightness))
				{
					UART2SendDataFromLightness();
				}
				cCycleUpdateDataFlag++;
				break;

			case 5:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsideTemp))
				{
					UART2SendDataFromTemp(SecondPanel_Outside);
				}
				cCycleUpdateDataFlag++;
				break;

			case 6:
				if(E_NOT_OK == GetFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsideTemp))
				{
					UART2SendDataFromTemp(SecondPanel_Inside);
				}
				cCycleUpdateDataFlag++;
				break;

			default:
				break;
		}
	}
	else
	{
		for(j=0; j<3; j++)
		{
			UART2SendDataFromSwitch(SecondPanel_TurnOff);
		}
		i = 0;
	}
	
	return E_OK;
}


void TemperatureCoding(uint8 cType, int nSrcData, uint8 *cDstData)
{
	int nData = 0;
	uint8 cData = 0;

	switch(cType)
	{
	case SecondPanel_Outside:
		cDstData[0] = 0x54;
		break;

	case SecondPanel_Inside:
		cDstData[0] = 0x74;
		break;

	default:
		break;
	}

	if(nSrcData < 0)
	{
		cDstData[1] = 0x2d;
		nData = ~nSrcData + 1;
	}
	else 
	{
		cDstData[1] = 0x20;
		nData = nSrcData;
	}

	if(nData >= 100)
	{
		return;
	}

	cData = ((uint8)(nData & 0xff))/10;
	if(0 == cData)
	{
		cDstData[2] = 0x20;
	}
	else
	{
		cDstData[2] = 0x30 + cData;
	}

	cDstData[3] = ((uint8)(nData & 0xff))%10 + 0x30;

	cDstData[4] = '\r';
}


void PM25Coding(uint8 cType, int nSrcData, uint8 *cDstData)
{
	uint8 cData = 0;
	uint8 cTempFlag = 0;

	switch(cType)
	{
	case SecondPanel_Outside:
		cDstData[0] = 0x45;
		break;

	case SecondPanel_Inside:
		cDstData[0] = 0x65;
		break;

		default:
		break;
	}

	if((nSrcData < 0) || (nSrcData > 1000))
	{
		return;
	}

	cData = nSrcData/100;
	if(0 == cData)
	{
		cDstData[1] = 0x20;
		cTempFlag = 1;
	}
	else
	{
		cDstData[1] = 0x30 + cData;
	}

	cData = (nSrcData%100)/10;
	if((0 == cData) && (1 == cTempFlag))
	{
		cDstData[2] = 0x20;
	}
	else
	{
		cDstData[2] = 0x30 + cData;
	}

	cDstData[3] = (nSrcData%100)%10 + 0x30;

	cDstData[4] = '\r';
}


void SoundWareCoding(uint8 SrcData, uint8 *cDstData)
{
	uint8 cData = 0;
	uint8 cTempFlag = 0;

	switch(SrcData)
	{
	case SoundWare_TurnOn:  
		cDstData[0] = 0x6f;
		cDstData[1] = 0x70;
		cDstData[2] = 0x65;
		cDstData[3] = 0x6e;
		break;








		default:
		cDstData[0] = 0x73;

	

		cData = SrcData/100;
		if(0 == cData)
		{
			cDstData[1] = 0x20;
			cTempFlag = 1;
		}
		else
		{
			cDstData[1] = 0x30 + cData;
		}

		cData = (SrcData%100)/10;
		if((0 == cData) && (1 == cTempFlag))
		{
			cDstData[2] = 0x20;
		}
		else
		{
			cDstData[2] = 0x30 + cData;
		}

		cDstData[3] = (SrcData%100)%10 + 0x30;
		break;
	}

	cDstData[4] = '\r';
}


void LightnessCoding(uint8 SrcData, uint8 *cDstData)
{
	uint8 cData = 0;
	uint8 cTempFlag = 0;

	cDstData[0] = 0x70;



	cData = SrcData/100;
	if(0 == cData)
	{
		cDstData[1] = 0x20;
		cTempFlag = 1;
	}
	else
	{
		cDstData[1] = 0x30 + cData;
	}

	cData = (SrcData%100)/10;
	if((0 == cData) && (1 == cTempFlag))
	{
		cDstData[2] = 0x20;
	}
	else
	{
		cDstData[2] = 0x30 + cData;
	}

	cDstData[3] = (SrcData%100)%10 + 0x30;

	cDstData[4] = '\r';
}


void SecondPanelSwitchCoding(uint8 cType, uint8 *cDstData)
{
	cDstData[0] = 0x6f;

	switch(cType)
	{
	case SecondPanel_TurnOn:
		cDstData[1] = 0x6e;
		cDstData[2] = 0x20;
		cDstData[3] = 0x20;
		break;

	case SecondPanel_TurnOff:
		cDstData[1] = 0x66;
		cDstData[2] = 0x66;
		cDstData[3] = 0x20;
		break;

	default:
		break;
	}

	cDstData[4] = '\r';
}


void RtcTimeCoding(void *SrcData, uint8 *cDstData)
{
	uint8 cData = 0;

	if(NULL == SrcData)
	{
		return;
	}

	TimeList *tPtr = (TimeList *)SrcData;

	
	cData = tPtr->hour/10;
	cDstData[0] = 0x30 + cData;

	cData = tPtr->hour%10;
	cDstData[1] = 0x30 + cData;

	
	cData = tPtr->minutes/10;
	cDstData[2] = 0x30 + cData;

	cData = tPtr->minutes%10;
	cDstData[3] = 0x30 + cData;

	cDstData[4] = '\r';
}

uint8 UART2SendDataFromTemp(uint8 cType)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	switch(cType)
	{
	case SecondPanel_Outside:
		TemperatureCoding(cType, stSecondPanelPara.OutTemp, cSendData);
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsideTemp);
		break;

	case SecondPanel_Inside:
		TemperatureCoding(cType, stSecondPanelPara.InTemp, cSendData);	
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsideTemp);
		break;

		default:
		break;
	}
	
	return E_OK;
}

uint8 UART2SendDataFromPM25(uint8 cType)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	switch(cType)
	{
	case SecondPanel_Outside:
		PM25Coding(cType, stSecondPanelPara.OutPM25Value, cSendData);
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidOutsidePM25);
		break;

	case SecondPanel_Inside:
		PM25Coding(cType, stSecondPanelPara.InPM25Value, cSendData);
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidInsidePM25);
		break;

		default:
		break;
	}
	return E_OK;
}

uint8 UART2SendDataFromSoundWare(void)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	SoundWareCoding(stSecondPanelPara.SoundWare, cSendData);
	LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
	ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidSoundWare);
	
	return E_OK;
}

uint8 UART2SendDataFromLightness(void)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	LightnessCoding(stSecondPanelPara.LightnessValue, cSendData);
	LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);	
	ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidLightness);
	
	return E_OK;
}

uint8 UART2SendDataFromSwitch(uint8 cType)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	switch(cType)
	{
	case SecondPanel_TurnOn:
		SecondPanelSwitchCoding(cType, cSendData);	
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		break;

	case SecondPanel_TurnOff:
		SecondPanelSwitchCoding(cType, cSendData);	
		LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
		break;

	default:
		break;
	}

	return E_OK;
}

uint8 UART2SendDataFromRtcTime(void)
{
	uint8 cSendData[SEND_DATA_CODING_LEN];

	RtcTimeCoding(&stSecondPanelPara.stTimeListValue, cSendData);	
	LPUART_DRV_SendDataPolling(PANEL_UART_CH, &cSendData[0], SEND_DATA_CODING_LEN);
	ClearFlagBit(&g_sSecondPanelSendDataFlag, SecondPanel_ValidRtcTime);

	return E_OK;
}


uint8 SetFlagBit(uint16 *sSrc, uint8 cBit)
{
	uint16 sData = *(uint16 *)sSrc;

	if(NULL == sSrc)
	{
		return E_NOT_OK;
	}

	sData |= 0x0001 << cBit;
	*sSrc = sData;

	return E_OK;	
}


uint8 GetFlagBit(uint16 *sSrc, uint8 cBit)
{
	uint16 sData = *(uint16 *)sSrc;
	uint16 cTempData = 0;

	if(NULL == sSrc)
	{
		return E_OK;
	}

	cTempData = sData >> cBit;
	cTempData &= 0x0001;

	return cTempData;	
}


uint8 ClearFlagBit(uint16 *sSrc, uint8 cBit)
{
	uint16 sData = *(uint16 *)sSrc;

	if(NULL == sSrc)
	{
		return E_NOT_OK;
	}
	
	sData &= ~(0x0001 << cBit);
	*sSrc = sData;

	return E_OK;
}











