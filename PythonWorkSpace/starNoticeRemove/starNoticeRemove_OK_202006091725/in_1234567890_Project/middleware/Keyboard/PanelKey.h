
#if 0 

#ifndef   PANEL_KEY_H
#define   PANEL_KEY_H

#include "Std_Types.h"
#include "uart_acpanel_cfg.h"
#include "Task_IpcApp_Cfg.h"
#include "Task_M_IpcApp_Cfg.h"

#define ADKEY_DEBUG
#define PANEL_HW_CHANNEL (INST_UART_ACPANEL_CFG)
#define PANEL_KEY_RX_BUFF_LEN (16U)
#define PANEL_RX_QUEUE_BUFF_LEN (32U)
#define PANEL_KEY_VAL_MASK    ( ~(1U<<7))


/* Panel Key  value*/
#define PANEL_BACK_VAL  	  (1U<<0)
#define PANEL_MENU_VAL  	  (1U<<1)     
#define PANEL_AVM_VAL		  (1U<<2)   
#define PANEL_PWR_VAL  	      (1U<<3)  
#define PANEL_VOL_DOWN_VAL    (1U<<4)    
#define PANEL_VOL_UP_VAL      (1U<<5)
#define PANEL_HOME_VAL        (1U<<6)

#if 1
extern uint8 gPannelKeyVal;
extern void PanelUartInit(void);
extern uint8 getPanelkeyVal(uint8 Val);

#endif

#endif

#endif

