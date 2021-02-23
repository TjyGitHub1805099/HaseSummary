#include "Key.h"
#include "AdKey.h"
#include "PanelKey.h"
#include "System_Signal.h"
#include "trace.h"
#include "Spi_Ipc_gw_api.h"
#include "CanKey.h"


Key gKey;


static void SendKeySignal(uint8 keyState,uint8 keyValue)
{
	if(0 == keyState)
	{
		System_Signal_Send(SYSTEM_SIG_ID_SWC_PRESS,keyValue,STD_OFF);
		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCType:%d Val:%d \n\r",keyState,keyValue);
	}
	else if(1 == keyState)
	{  
		System_Signal_Send(SYSTEM_SIG_ID_SWC_REL,keyValue,STD_OFF);
		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCType:%d Val:%d \n\r",keyState,keyValue);
	}
	else if(2 == keyState)
	{
		System_Signal_Send(SYSTEM_SIG_ID_SWC_HOLD,keyValue,STD_OFF);
		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCType:%d Val:%d \n\r",keyState,keyValue);
	}
	else if(3 == keyState)
	{
		System_Signal_Send(SYSTEM_SIG_ID_SWC_HOLD_REL,keyValue,STD_OFF);
		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCType:%d Val:%d \n\r",keyState,keyValue);
	}

}

static void KeyDecodeEv(uint8 val,uint8 KeyType)
{

	uint8 keyData[2]={0};
	

	SendKeySignal(KeyType,val);
	keyData[0] = KeyType;
	keyData[1] = val;
	if(TRUE == GetM_IpcComStatus())
	{
		ipc_send_notification(1,KEY_IPC_CH,KEY_IPC_FUN_ID,keyData,KEY_DATA_LEN);
		
		#if 0
		switch(val)
		{
			case KEY_STRG_TEL:      
				AVM_KeyDate[0] = 0;
				AVM_KeyDate[1] = KeyType;
				gAvmKeyStatus[AVM_KEY_TYPE_SRVM] = KeyType;
				ipc_send_notification(1,IPC_GW_CHANNEL_AVM,KEY_IPC_FUN_ID,AVM_KeyDate,KEY_DATA_LEN);
				break;
			case KEY_PANEL_HOME:
				AVM_KeyDate[0] = 1;
				AVM_KeyDate[1] = KeyType;
				gAvmKeyStatus[AVM_KEY_TYPE_HOME] = KeyType;
				ipc_send_notification(1,IPC_GW_CHANNEL_AVM,KEY_IPC_FUN_ID,AVM_KeyDate,KEY_DATA_LEN);
				break;
			case KEY_PANEL_BACK:
				AVM_KeyDate[0] = 2;
				AVM_KeyDate[1] = KeyType;
				gAvmKeyStatus[AVM_KEY_TYPE_BACK] = KeyType;
				ipc_send_notification(1,IPC_GW_CHANNEL_AVM,KEY_IPC_FUN_ID,AVM_KeyDate,KEY_DATA_LEN);
				break;
			case KEY_PANEL_AVM:
				AVM_KeyDate[0] = 3;
				AVM_KeyDate[1] = KeyType;
				gAvmKeyStatus[AVM_KEY_TYPE_AVM] = KeyType;
				ipc_send_notification(1,IPC_GW_CHANNEL_AVM,KEY_IPC_FUN_ID,AVM_KeyDate,KEY_DATA_LEN);
				break;
			default:
			    break;

		}
		#endif
			
	}
		
}

void KeyInit(void)
{
	
	
	uint8 i;
	
	gKey.ScanTimer = 0;
	gKey.KeyId = MAX_KEY_CH;
	for(i=0;i< MAX_KEY_CH;i++)
	{
		gKey.State[i] = KEY_STATE_IDLE;
		gKey.WaitTimer[i] = 0;
		gKey.KeyIndex[i] = KEY_NONE;
		gKey.PreKeyIndex[i] = KEY_NONE;
		gKey.KeyVal[i] = 0;
	}
	#if 0
	#ifdef   PANEL_KEY
		PanelUartInit();
	#endif
	#endif
}

static void KeyScanSyc(uint8 ch)
{
	#if 0
	#ifdef PANEL_KEY
		if(AC_PANEL_KEY == ch)
		{
			gKey.KeyIndex[ch] = getPanelkeyVal(gPannelKeyVal);
			
		}
		else if((CAN_STRG_KEY == ch)||(CAN_PINAO_KEY == ch)||(CAN_PANEL_KEY == ch))
		{
			gKey.KeyIndex[ch] = getCankeyVal(ch,gCanSwKeyVal.keyval);
		}
		else
		{
		    gKey.KeyIndex[ch] = AdDataConver(ch,AdkeyFil[ch].temp);
		}
	
	#else
		gKey.KeyIndex[ch] = AdDataConver(ch,AdkeyFil[ch].temp);
	#endif
	#else
		gKey.KeyIndex[ch] = getCankeyVal(ch,gCanSwKeyVal.keyval);
	#endif
	switch(gKey.State[ch])
	{
		case KEY_STATE_IDLE:
		if(gKey.KeyIndex[ch] == KEY_NONE)
		{
			break ;
		}
		gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
		gKey.WaitTimer[ch] = 0;
		gKey.State[ch] = KEY_STATE_JITTER;
		break;
		
		case KEY_STATE_JITTER:
		if(gKey.KeyIndex[ch] == KEY_NONE)
		{
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
			gKey.WaitTimer[ch] = 0;
			gKey.State[ch] = KEY_STATE_IDLE;
		}
		else if(gKey.WaitTimer[ch] >= KEY_JITTER_TIME)
		{
			gKey.KeyId = ch;
			gKey.WaitTimer[ch] = 0;
		#if 0
		#ifdef PANEL_KEY
			if(AC_PANEL_KEY == ch)
			{
				TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>UartData:%d \n\r",gPannelKeyVal);	
			}
			else
			{
		  		TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCKey_AD_Val:%d \n\r",AdkeyFil[ch].temp);	
			}
		#else
			TracePrintf(INPUT_TRACE, TRACE_NONMASK, "[Key-N]>SWCKey_AD_Val:%d \n\r",AdkeyFil[ch].temp);
		#endif
		#endif
			
			KeyDecodeEv(gKey.KeyIndex[ch],PRESS_KEY);
			
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
			gKey.State[ch] = KEY_STATE_PRESS_DOWN;
		}
		else
		{
			gKey.WaitTimer[ch]++;
		}
		break;

		case KEY_STATE_PRESS_DOWN:
		if(gKey.KeyIndex[ch] == KEY_NONE)
		{
			gKey.KeyId = ch;
			KeyDecodeEv(gKey.PreKeyIndex[ch],REL_KEY);
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
			gKey.WaitTimer[ch] = 0;
			gKey.State[ch] = KEY_STATE_IDLE;
		}
		else if(gKey.WaitTimer[ch] >= KEY_PRESS_TIME)
		{
			gKey.WaitTimer[ch] = 0;
			gKey.State[ch] = KEY_STATE_PRESS_DOWN_WAIRT;
			gKey.KeyId = ch;
		}
		else
		{
			gKey.WaitTimer[ch]++;
		}
		break;

		case KEY_STATE_PRESS_DOWN_WAIRT:
		if(gKey.KeyIndex[ch] == KEY_NONE)
		{
			gKey.KeyId = ch;
			KeyDecodeEv(gKey.PreKeyIndex[ch],REL_KEY);
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
			gKey.WaitTimer[ch] = 0;
			gKey.State[ch] = KEY_STATE_IDLE;
		}
		else if(gKey.WaitTimer[ch] >= KEY_LONG_TIME)
		{
			gKey.KeyId = ch;
			gKey.WaitTimer[ch] = 0;
			gKey.State[ch] = KEY_STATE_PRESS_LONG_DOWN;
			KeyDecodeEv(gKey.KeyIndex[ch],HOLD_KEY);
			
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
		}	
		else
		{
			gKey.WaitTimer[ch]++;
		}
		break;

		case KEY_STATE_PRESS_LONG_DOWN:
		if(gKey.KeyIndex[ch] == KEY_NONE)
		{
			gKey.KeyId = ch;
			gKey.State[ch] = KEY_STATE_IDLE;
			KeyDecodeEv(gKey.PreKeyIndex[ch],HOLD_REL_KEY);
			gKey.PreKeyIndex[ch] = gKey.KeyIndex[ch];
			gKey.WaitTimer[ch] = 0;
		}
		
		break;

		default:
		gKey.WaitTimer[ch] = 0;	
		gKey.State[ch] = KEY_STATE_IDLE;
		break;
	}
}

void KeyScanCyc(void)
{

	#if 0
	AdKeyFilterRun(AD_STEERING_A_KEY);
	AdKeyFilterRun(AD_STEERING_B_KEY);
	#endif
	

	#if 0
	if(gKey.ScanTimer < 1)
	{
		gKey.ScanTimer++;
		return;
	}
	
	gKey.ScanTimer = 0;
	
	
	
	
	
	

	#endif
	KeyScanSyc(CAN_STRG_KEY);
	KeyScanSyc(CAN_PINAO_KEY);
	KeyScanSyc(CAN_PANEL_KEY);
	
	
	
}







