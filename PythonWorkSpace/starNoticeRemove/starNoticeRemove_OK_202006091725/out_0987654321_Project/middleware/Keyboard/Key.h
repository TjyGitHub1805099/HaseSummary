#ifndef   KEY_H
#define   KEY_H

#include "Std_Types.h"
#include "Task_M_IpcApp_Cfg.h"


/* key IPC parameters*/

#define KEY_IPC_CH      IPC_GW_CHANNEL_KEYSERVICE
#define KEY_IPC_FUN_ID  0
#define KEY_DATA_LEN	2


/* Key state value*/
#define  PRESS_KEY		0
#define	 REL_KEY		1
#define  HOLD_KEY		2
#define  HOLD_REL_KEY	3

#define  KEY_JITTER_TIME          0//3//9
#define  KEY_PRESS_TIME           9 //120
#define  KEY_LONG_TIME            69//29//64//42//180


//#define AD_KEY       1
//#define PANEL_KEY    2
#define CAN_KEY      3

typedef enum
{
	#if 0
    AD_STEERING_A_KEY = 0,
	AD_STEERING_B_KEY,
	
	#ifdef PANEL_KEY 
    	AC_PANEL_KEY,
    #endif
	#endif
	CAN_STRG_KEY,
	CAN_PINAO_KEY,
	CAN_PANEL_KEY,
    MAX_KEY_CH
}keyChannel;

enum
{
	KEY_STATE_IDLE = 0,
	KEY_STATE_JITTER,
	KEY_STATE_PRESS_DOWN,
	KEY_STATE_PRESS_DOWN_WAIRT,
	KEY_STATE_PRESS_LONG_DOWN
}keyState;
	
typedef struct
{
    uint16 WaitTimer[MAX_KEY_CH];
    uint8  ScanTimer;
    uint8  State[MAX_KEY_CH];
    uint8  PreKeyIndex[MAX_KEY_CH];
    uint8  KeyIndex[MAX_KEY_CH];
    uint8  KeyId;
    uint8  KeyVal[MAX_KEY_CH];
}Key;

#if 0
typedef enum
{
	KEY_NONE =0,
	/* Steering_A Key */
	KEY_STRG_SRC, 	   //1
	KEY_STRG_MENU_UP,    //2
	KEY_STRG_MENU_DOWN,  //3
	KEY_STRG_VOICE_REC,	   //4
	KEY_STRG_ENTER,	   //5
			
	/* Steering_B Key */
	KEY_STRG_VOL_DOWN,   //6
	KEY_STRG_VOL_UP,	   //7
	KEY_STRG_TEL,	   //8
	KEY_STRG_BACK,	   //9
	KEY_STRG_DISP,	   //10
	
	/* Panel Key */
	KEY_PANEL_BACK=11, 	   //11
	KEY_PANEL_MENU,    //12
	KEY_PANEL_AVM,  //13
	KEY_PANEL_PWR,	   //14
	KEY_PANEL_VOL_DONW,	   //15
	KEY_PANEL_VOL_UP,	   //16
	KEY_PANEL_HOME,   //17
}KeyVal;
#endif


#if 1
	typedef enum
	{
		KEY_NONE =0,
		/* Steering Key */
		KEY_STRG_UP,	       //1
		KEY_STRG_DOWN,	       //2
		KEY_STRG_LEFT,         //3
		KEY_STRG_RIGHT,        //4
		KEY_STRG_OK,           //5
		KEY_STRG_DISP,	       //6
		KEY_STRG_TEL_ON,	   //7
		KEY_STRG_DVR,	       //8
		KEY_STRG_LKA,	       //9
		/* Pinao Key */
		KEY_PINAO_FR_DEFOG,    //10
		KEY_PINAO_RR_DEFOG,    //11

		/* Panel Key */
		KEY_PANEL_VOICE_UP,	   //12  
		KEY_PANEL_VOICE_DOWN,    //13
		KEY_PANEL_UP,	       //14
		KEY_PANEL_DOWN,	       //15
		KEY_PANEL_SOURCE       //16
	}CanKeyVal;
#endif


#if 0
typedef enum{
	AVM_KEY_TYPE_HOME,
	AVM_KEY_TYPE_BACK,
	AVM_KEY_TYPE_SRVM,
	AVM_KEY_TYPE_AVM,
	AVM_KEY_MAX,
}tAvmKeyType;
#endif

extern void KeyScanCyc(void);
extern void KeyInit(void);

#endif

