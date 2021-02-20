#if 0
#ifndef   AD_KEY_H
#define   AD_KEY_H

#include "Std_Types.h"
#include "Task_IpcApp_Cfg.h"

#define  ADKEY_DEBUG

/* key IPC parameters*/

//#define KEY_IPC_CH      IPC_CH_KEY
//#define KEY_IPC_FUN_ID  1
//#define KEY_DATA_LEN	2

//////////////////
#define SWC_STEP_LEVEL       50 //25 //100


/* Steering_A Key AD value*/
#define SWC_SRC_LEVEL  	       0       //0Ω
#define SWC_MENU_UP_LEVEL  	   110     //121Ω
#define SWC_MENU_DOWN_LEVEL    248     //321Ω
#define SWC_TEL_ON_LEVEL  	   429     //723Ω
#define SWC_OK_LEVEL  	       660     //685     //2023Ω


/* Steering_B Key AD value*/
#define SWC_VOL_DOWN_LEVEL 	  0        //0Ω
#define SWC_VOL_UP_LEVEL 	  110      //121Ω
#define SWC_CAMERA_LEVEL 	  248      //321Ω
#define SWC_MODE_LEVEL 		  429      //723Ω
#define SWC_DISP_LEVEL 		  660      //685      //2023Ω

#define KEY_NONE_LEVEL    950  //1000 //4000
#define KEY_LOW_LEVEL     0

#define AD_KEY_CH_NUM     2

typedef struct
{
    unsigned char count;
    uint16  value;
    uint16  new_value;
    uint16  temp;
}AdKeyFilter;


extern AdKeyFilter AdkeyFil[];
extern void AdKeyFilInit(void);
extern void AdKeyFilterRun(uint8 ch);
extern uint8 AdDataConver(uint8 ch,uint16 Val);


#endif
#endif
