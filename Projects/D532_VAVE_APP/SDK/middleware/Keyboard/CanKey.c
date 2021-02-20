#include "PanelKey.h"
#include "Key.h"
#include "CanKey.h"
#include "Com_If.h"


CanKeyType gCanSwKeyVal;
extern uint8 * const Com_IpduBufPtr[];

#if 0
static uint16 ReverseBit(uint16 data)
{
	uint16 l_retVal = data;

	//15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
	l_retVal = (l_retVal << 8) | (l_retVal >> 8);               //(1)7 6 5 4 3 2 1 0 15 14 13 12 11 10 9  8
	l_retVal = ((l_retVal << 4)&0xf0f0)|((l_retVal>>4)&0x0f0f); //(2)3 2 1 0 7 6 5 4 11 10 9  8  15 14 13 12
	l_retVal = ((l_retVal << 2)&0xcccc)|((l_retVal>>2)&0x3333); //(3)1 0 3 2 5 4 7 6 9  8  11 10 13 12 15 14
	l_retVal = ((l_retVal << 1)&0xaaaa)|((l_retVal>>1)&0x5555); //(4)0 1 2 3 4 5 6 7 8  9  10 11 12 13 14 15 
	return l_retVal;
}
#endif

#if 1
uint8 getCankeyVal(uint8 ch,uint16 Val)
{
	uint16 key_data = KEY_NONE;//KEY_PANEL_NONE;
    uint16 key_val = 0;
	if(CAN_STRG_KEY == ch)
	{
		key_val = Val & CAN_STRG_KEY_MASK;
		switch(key_val)
	    {
			case STRG_UP_VAL:
				key_data = KEY_STRG_UP;
				break;
			case STRG_DOWN_VAL:
				key_data = KEY_STRG_DOWN;
				break;
			case STRG_LEFT_VAL:
				key_data = KEY_STRG_LEFT;
				break;
			case STRG_RIGHT_VAL:
				key_data = KEY_STRG_RIGHT;
				break;
			case STRG_OK_VAL:
				key_data = KEY_STRG_OK;
				break;
			case STRG_DISP_VAL:
				key_data = KEY_STRG_DISP;
				break;	
			case STRG_TEL_ON_VAL:
				key_data = KEY_STRG_TEL_ON;
				break;
			case STRG_DVR_VAL:
				key_data = KEY_STRG_DVR;
				break;
			case STRG_LKA_VAL:
				key_data = KEY_STRG_LKA;
				break;
			
			default:
				key_data = KEY_NONE;
				break;
		}	
	}
	else if(CAN_PINAO_KEY == ch)
	{
		key_val = Val & CAN_PINAO_KEY_MASK;
		switch(key_val)
	    {
			case PINAO_FR_DEFOG_VAL:
				key_data = KEY_PINAO_FR_DEFOG;
				break;
			case PINAO_RR_DEFOG_VAL:
				key_data = KEY_PINAO_RR_DEFOG;
				break;	
			default:
				key_data = KEY_NONE;
				break;
		}
		
	}
	else if(CAN_PANEL_KEY == ch)
	{
		key_val = Val & CAN_PANEL_KEY_MASK;
		switch(key_val)
	    {
			case PANEL_VOICE_UP_VAL:
				key_data = KEY_PANEL_VOICE_UP;
				break;
			case PANEL_VOICE_DOWN_VAL:
				key_data = KEY_PANEL_VOICE_DOWN;
				break;
			case PANEL_UP_VAL:
				key_data = KEY_PANEL_UP;
				break;
			case PANEL_DOWN_VAL:
				key_data = KEY_PANEL_DOWN;
				break;
			case PANEL_SOURCE_VAL:
				key_data = KEY_PANEL_SOURCE;
				break;
			default:
				key_data = KEY_NONE;
				break;
		}
		
	}
		
	return key_data;
}
#endif

#if 0
void getCanSwkeySigVal(void)
{
	boolean l_keySigVal = 0;
	/*steering key*/
	Com_RxSig_GetL_SW_UP(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Up = l_keySigVal;
	
	Com_RxSig_GetL_SW_DOWN(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Down = l_keySigVal;
	
	Com_RxSig_GetL_SW_LEFT(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Left = l_keySigVal;
	
	Com_RxSig_GetL_SW_RIGHT(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Right = l_keySigVal;
	
	Com_RxSig_GetL_SW_OK(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Ok = l_keySigVal;
	
	Com_RxSig_GetL_SW_DISP(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Disp = l_keySigVal;
	
	Com_RxSig_GetL_SW_TEL_ON(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Tel_On = l_keySigVal;

	Com_RxSig_GetR_SW_DVR(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Dvr = l_keySigVal;
	
	Com_RxSig_GetR_SW_LKA(&l_keySigVal);
	gCanSwKeyVal.key.Strg_Lka = l_keySigVal;
	
	/*pinao key*/
	Com_RxSig_GetPinao_FR_DEFOGSW(&l_keySigVal);
	gCanSwKeyVal.key.Pinao_Fr_Defog = l_keySigVal;
	
    Com_RxSig_GetPinao_RR_DEFOGSW(&l_keySigVal);
	gCanSwKeyVal.key.Pinao_Rr_Defog = l_keySigVal;
	
	/*panel key*/
	Com_RxSig_GetVOICE_UP(&l_keySigVal);
	gCanSwKeyVal.key.Panel_Voice_Up =l_keySigVal;
	
	Com_RxSig_GetVOICE_DOWN(&l_keySigVal);
	gCanSwKeyVal.key.Panel_Voice_Down =l_keySigVal;
	
	Com_RxSig_GetUP_5C8(&l_keySigVal);
	gCanSwKeyVal.key.Panel_Up = l_keySigVal;
	
	Com_RxSig_GetDOWN_5C8(&l_keySigVal);
	gCanSwKeyVal.key.Panel_Down = l_keySigVal;
	
	Com_RxSig_GetSOURCE_5C8(&l_keySigVal);
	gCanSwKeyVal.key.Panel_Source = l_keySigVal;
	
}
#else

void getCanSwkeySigVal(void)
{
	uint8 temp[3] = {0,0,0};
	uint16 keyval = 0;
	uint8 *keydat = Com_IpduBufPtr[IPDU_HND_S_BOX_ControlData_SetPM];
	temp[0] = keydat[3];
	temp[1] = keydat[4];
	temp[2] = keydat[5];
	keyval = temp[0] & 0xFE;
	keyval |= (temp[1] >> 7);
	keyval = keyval << 8;
	keyval |= (((temp[1] << 1) & 0x80)|((temp[1] & 0x1F) << 2));   
	keyval = keyval | (temp[2] >> 6);
	gCanSwKeyVal.keyval = keyval;
}

#endif



