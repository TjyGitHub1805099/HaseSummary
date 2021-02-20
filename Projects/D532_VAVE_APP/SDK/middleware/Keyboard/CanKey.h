#ifndef   CAN_KEY_H
#define   CAN_KEY_H

#if 0
typedef union CanKey
{
	uint16 keyval;
	struct
	{
		//steering key
		uint16 Strg_Up	                :1;	
		uint16 Strg_Down	            :1;  
		uint16 Strg_Left                :1;	
		uint16 Strg_Right               :1;  
		uint16 Strg_Ok	                :1;
		uint16 Strg_Disp                :1; 
		uint16 Strg_Tel_On		        :1;
		//uint16 Response_error_left	    :1;
		uint16 Strg_Dvr		            :1;
		uint16 Strg_Lka		            :1;
		//uint16 Response_error_right		:1;
		//pinao key
		uint16 Pinao_Fr_Defog          :1;
		uint16 Pinao_Rr_Defog          :1;
		//panel key
		uint16 Panel_Voice_Up           :1;			
		uint16 Panel_Voice_Down         :1;	
		uint16 Panel_Up                 :1;	
		uint16 Panel_Down              :1;
		uint16 Panel_Source             :1;
	}key;
}CanKeyType;


//#define CAN_KEY_VAL_MASK    (0xFFFF)


#define CAN_STRG_KEY_MASK    (0x1FF)
#define CAN_PINAO_KEY_MASK   (3U<<9)
#define CAN_PANEL_KEY_MASK   (0xF800)


/* Steering Key value*/
#define STRG_UP_VAL  	               (1U<<0)
#define STRG_DOWN_VAL   	           (1U<<1)     
#define STRG_LEFT_VAL		           (1U<<2)   
#define STRG_RIGHT_VAL  	           (1U<<3)  
#define STRG_OK_VAL                    (1U<<4)    
#define STRG_DISP_VAL                  (1U<<5)
#define STRG_TEL_ON_VAL                (1U<<6)
//#define Response_error_left_VASTRG     (1U<<7)
#define STRG_DVR_VAL                   (1U<<7)
#define STRG_LKA_VAL                   (1U<<8)
//#define Response_error_right         (1U<<10)
/* PINAO Key value*/
#define PINAO_FR_DEFOG_VAL             (1U<<9)
#define PINAO_RR_DEFOG_VAL             (1U<<10)
/* Panel Key value*/
#define PANEL_VOICE_UP_VAL             (1U<<11)
#define PANEL_VOICE_DOWN_VAL           (1U<<12)
#define PANEL_UP_VAL                   (1U<<13)
#define PANEL_DOWN_VAL                 (1U<<14)
#define PANEL_SOURCE_VAL               (1U<<15)
#else

typedef union CanKey
{
	uint16 keyval;
	struct
	{
		//panel key
		uint16 Panel_Source	            :1;	
		uint16 Panel_Down	            :1;  
		uint16 Panel_Up                 :1;	
		uint16 Panel_Voice_Down         :1;  
		uint16 Panel_Voice_Up	        :1;
		//pinao key
		uint16 Pinao_Rr_Defog           :1; 
		uint16 Pinao_Fr_Defog		    :1;
		//steering key 
		uint16 Strg_Lka		            :1;
		uint16 Strg_Dvr		            :1;
		uint16 Strg_Tel_On              :1;
		uint16 Strg_Disp                :1;
		uint16 Strg_Ok                  :1;			
		uint16 Strg_Right               :1;	
		uint16 Strg_Left                :1;	
		uint16 Strg_Down                :1;
		uint16 Strg_Up                  :1;
	}key;
}CanKeyType;

#define CAN_STRG_KEY_MASK    (0xFF80)
#define CAN_PINAO_KEY_MASK   (0x60)
#define CAN_PANEL_KEY_MASK   (0x1F)

/* Steering Key value*/
#define STRG_UP_VAL  	               (1U<<15)//(1U<<0)
#define STRG_DOWN_VAL   	           (1U<<14)//(1U<<1)     
#define STRG_LEFT_VAL		           (1U<<13)//(1U<<2)   
#define STRG_RIGHT_VAL  	           (1U<<12)//(1U<<3)  
#define STRG_OK_VAL                    (1U<<11)//(1U<<4)    
#define STRG_DISP_VAL                  (1U<<10)//(1U<<5)
#define STRG_TEL_ON_VAL                (1U<<9)//(1U<<6)
//#define Response_error_left_VASTRG     (1U<<7)
#define STRG_DVR_VAL                   (1U<<8)//(1U<<7)
#define STRG_LKA_VAL                   (1U<<7)//(1U<<8)
//#define Response_error_right         (1U<<10)
/* PINAO Key value*/
#define PINAO_FR_DEFOG_VAL             (1U<<6)//(1U<<9)
#define PINAO_RR_DEFOG_VAL             (1U<<5)//(1U<<10)
/* Panel Key value*/
#define PANEL_VOICE_UP_VAL             (1U<<4)//(1U<<11)
#define PANEL_VOICE_DOWN_VAL           (1U<<3)//(1U<<12)
#define PANEL_UP_VAL                   (1U<<2)//(1U<<13)
#define PANEL_DOWN_VAL                 (1U<<1)//(1U<<14)
#define PANEL_SOURCE_VAL               (1U<<0)//(1U<<15)
#endif

extern CanKeyType gCanSwKeyVal;
extern uint8 getCankeyVal(uint8 ch,uint16 Val);
extern void getCanSwkeySigVal(void);

#endif

