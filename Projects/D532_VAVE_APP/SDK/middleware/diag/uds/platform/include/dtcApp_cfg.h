#ifndef _DTC_APP_CFG_H
#define _DTC_APP_CFG_H

#include "dtcCore_cfg.h"

///////////////////////////////////////////////////
#define DTC_APP_DELAY_TIMER 2000
#define JT1_TIMER_VALUE 0	//com has consumed 300ms by MACRO COM_SHORT_PERIODS_TIMEOUT_VALUE
#define JT2_TIMER_VALUE 1700

typedef enum{
	ECU_CATEGORY_NONE = 0,
	ECU_CATEGORY_A = 1,	//Fault monitoring related to Slaves not operating under low battery voltage conditions.
	ECU_CATEGORY_B = 2,	//Fault monitoring related to Master and Slaves with guaranteed operation under low battery voltage conditions.
	ECU_CATEGORY_C = 4,	//Fault monitoring related to nodes that are neither Master nor Slave and with guaranteed operation under low battery voltage conditions.
	ECU_CATEGORY_D = 8,	//Fault monitoring related to nodes that are neither Master nor Slave and not operating under low battery voltage conditions.
}tEcuCategory;

typedef enum{
	ECU_OPT_COND_LEVEL_0 = 1,	//Basis,Stand by (no CANcommunication)
	ECU_OPT_COND_LEVEL_1 = 2,	//Internal Environment,WakeUp/Sleep by CAN 
	ECU_OPT_COND_LEVEL_2 = 4,	//Comfort,Accessory Switch On
	ECU_OPT_COND_LEVEL_3 = 8,	//Traffic,Ignition Switch On (APC)
}tEcuOptCondLevel;

typedef struct{
	uint8 ecu_category;
	uint8 opt_cond_level;
	boolean referToDiagMuxOn;
	boolean faultActive;
	uint16 JT1_timerValue;
	boolean JT1_timerActiveFlag;
	boolean JT1_timeoutFlag;
	uint16 JT2_timerValue;
	boolean JT2_timerActiveFlag;
	boolean JT2_timeoutFlag;
	
	uint32 preTick;
}tDtcAppCanFaultInfor;


#define DTC_APP_RX_CANMSG_TIMEOUT_BIT  	0x80
#define DTC_APP_THIS_CYCLE_TEST_FAILED	0x8000
#define DTC_APP_RX_CANMSG_TIMEOUT_CFM_BIT 0x40
#define VALMAX_VALUE 40
#define DTC_APP_RX_CAN_MSG_FAULT_START_INDEX 0	//BUS OFF
#define DTC_CORE_CAN_FAULT_INDEX 0

#define DTC_APP_FAULT_INDEX_CAN_BUS_OF 0

#define DTC_APP_FAULT_INDEX_CAN_TX_MUTE 1
//=====================================================================================
#define DTC_HIGH_VOLTAGE_THRESHOLD_VALUE 1600             // 16V
#define DTC_HIGH_VOLTAGE_FILTER_THRESHOLD_VALUE 1550      // 15.5V

#define DTC_LOW_VOLTAGE_FILTER_THRESHOLD_VALUE 950        // 9.5V
#define DTC_LOW_VOLTAGE_THRESHOLD_VALUE 900               // 9V


typedef enum{
	DTC_SYS_VOLTAGE_STATUS_LOW,
	DTC_SYS_VOLTAGE_STATUS_NORMAL,
	DTC_SYS_VOLTAGE_STATUS_HIGH,
}tDtcVoltageStatus;

//=====================================================================================



#define IVI_INNER_ID_CAN_BUS_OFF_MASK 1

#define IVI_CAN_TX_MUTE				1

//for ECM
#define IVI_RX_CAN_MSG_MASK_ECM_0X180	1
#define IVI_RX_CAN_MSG_MASK_ECM_0X551   2
#define IVI_RX_CAN_MSG_MASK_ECM_0X1F9	4
#define IVI_RX_CAN_MSG_MASK_ECM_0X160   8


//#define IVI_RX_CAN_MSG_MASK_ECM_0X6E2    4
//#define IVI_RX_CAN_MSG_MASK_ECM_0X580    8
//#define IVI_RX_CAN_MSG_MASK_ECM_0X182    0x10

//FOR ABS_VDC
#define IVI_RX_CAN_MSG_MASK_ABS_0X284      1	//ABS
#define IVI_RX_CAN_MSG_MASK_ABS_0X354      2	//ABC
#define IVI_RX_CAN_MSG_MASK_VDC_0X245      4	//VDC

//for BCM
#define IVI_RX_CAN_MSG_MASK_BCM_0X358      1
//#define IVI_RX_CAN_MSG_MASK_BCM_0X385      2
#define IVI_RX_CAN_MSG_MASK_BCM_0X60D      2
#define IVI_RX_CAN_MSG_MASK_BCM_0X35A      4
#define IVI_RX_CAN_MSG_MASK_BCM_0X35D      8


//for HVAC
#define IVI_RX_CAN_MSG_MASK_HVAC_0X5CB      1


//AT[CVT]
#define IVI_RX_CAN_MSG_MASK_CVT_0X421      1

//STRG
#define IVI_RX_CAN_MSG_MASK_STRG_0X2      1

//EPS
#define IVI_RX_CAN_MSG_MASK_EPS_0X5E4      1

//USM[IPDM]
//#define IVI_RX_CAN_MSG_MASK_USM_0X215      1
#define IVI_RX_CAN_MSG_MASK_USM_0X625      1


//TPMS
#define IVI_RX_CAN_MSG_MASK_TPMS_0X385     1	//[FROM BCM]



//for TCU
//#define IVI_RX_CAN_MSG_MASK_TCU_0X56E      1
#define IVI_RX_CAN_MSG_MASK_TCU_0X563      1


//UPA
#define IVI_RX_CAN_MSG_MASK_UPA_0X57A      1

//BSW
#define IVI_RX_CAN_MSG_MASK_BSW_0X665      1

//VCM
#define IVI_RX_CAN_MSG_MASK_VCM_0X5A9      1
#define IVI_RX_CAN_MSG_MASK_VCM_0X50D      2


//AVM 446
#define IVI_RX_CAN_MSG_MASK_ABD_0X572      1

//IKEY
#define IVI_RX_CAN_MSG_MASK_IKEY_0X351     1


typedef enum{
	//The EXT_ID and INNER_ID correspond one by one.
	//Do not change the order.
	IVI_FAULT_INDEX_BUS_OFF					=0,
	IVI_FAULT_INDEX_TX_MUTE				=1,
	IVI_FAULT_INDEX_RX_CAN_MSG_ECM        ,
	IVI_FAULT_INDEX_RX_CAN_MSG_ABS_VDC    ,
	IVI_FAULT_INDEX_RX_CAN_MSG_BCM       ,
	IVI_FAULT_INDEX_RX_CAN_MSG_HVAC      ,
	IVI_FAULT_INDEX_RX_CAN_MSG_CVT       ,
	IVI_FAULT_INDEX_RX_CAN_MSG_STRG      ,
	IVI_FAULT_INDEX_RX_CAN_MSG_EPS       ,	
	IVI_FAULT_INDEX_RX_CAN_MSG_USM      ,
	IVI_FAULT_INDEX_RX_CAN_MSG_TPMS    	,
	IVI_FAULT_INDEX_RX_CAN_MSG_TCU      ,
	IVI_FAULT_INDEX_RX_CAN_MSG_UPA 		,
	IVI_FAULT_INDEX_RX_CAN_MSG_VCM      ,
	IVI_FAULT_INDEX_RX_CAN_MSG_ABD		,
	
	IVI_FAULT_INDEX_RX_CAN_MSG_IKEY     ,
	IVI_FAULT_INDEX_MAX,
}tIVIFaultIndex;

//============================================

#define IC_INNER_ID_CAN_BUS_OFF_MASK 1

#define IC_CAN_TX_MUTE				1

//for ECM
#define IC_RX_CAN_MSG_MASK_ECM_0X180 1
//#define IC_RX_CAN_MSG_ECM_0X182      2
#define IC_RX_CAN_MSG_MASK_ECM_0X551      2
#define IC_RX_CAN_MSG_MASK_ECM_0X6E2      4
#define IC_RX_CAN_MSG_MASK_ECM_0X580      8



//FOR ABS_VDC
#define IC_RX_CAN_MSG_MASK_ABS_0X284      1
#define IC_RX_CAN_MSG_MASK_ABS_0X354      2
#define IC_RX_CAN_MSG_MASK_VDC_0X245      4

//for BCM
#define IC_RX_CAN_MSG_MASK_BCM_0X358      1
#define IC_RX_CAN_MSG_MASK_BCM_0X35D      2
#define IC_RX_CAN_MSG_MASK_BCM_0X60D      4

//for ACU
#define IC_RX_CAN_MSG_MASK_ACU_0X02A      1

//FOR RADAR
#define IC_RX_CAN_MSG_MASK_RADAR_0X4CC      1
#define IC_RX_CAN_MSG_MASK_RADAR_0X2B7      2

//HVAC
#define IC_RX_CAN_MSG_MASK_HVAC_0X54C      1

//CVT
#define IC_RX_CAN_MSG_MASK_CVT_0X421      1

//ONLY FOR VDC
#define IC_RX_CAN_MSG_MASK_VDC_0X58A      1

//EPS
#define IC_RX_CAN_MSG_MASK_EPS_0X5E4      1


//IPDM
//#define IC_RX_CAN_MSG_MASK_IPDM_0X215      1
#define IC_RX_CAN_MSG_MASK_IPDM_0X625      1

//TPMS
#define IC_RX_CAN_MSG_MASK_TPMS_0X385     1
//#define IC_RX_CAN_MSG_MASK_TPMS_0X5E8     2

//IKEY
#define IC_RX_CAN_MSG_MASK_IKEY_0X351     1

//BSW
#define IC_RX_CAN_MSG_MASK_BSW_0X57A      1
//#define IC_RX_CAN_MSG_MASK_BSW_0X665      2
#define IC_RX_CAN_MSG_MASK_BSW_0X2C8      2

//LENE_CAMERA:
#define IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X4CB 1
#define IC_RX_CAN_MSG_MASK_LANE_CAMERA_0X2B1 2
//SCU
#define IC_RX_CAN_MSG_MASK_SCU_0X11B      1
//LCMR
#define IC_RX_CAN_MSG_MASK_LCMR_0X629      1
//LCML
#define IC_RX_CAN_MSG_MASK_LCML_0X628      1

typedef enum{
	//The EXT_ID and INNER_ID correspond one by one.
	//Do not change the order.
	IC_FAULT_INDEX_BUS_OFF					=0,
	IC_FAULT_INDEX_TX_MUTE				,
	IC_FAULT_INDEX_RX_CAN_MSG_ECM        ,	//180,551,6E2,	580
	IC_FAULT_INDEX_RX_CAN_MSG_ABS_VDC        ,	//ABS:284,354,	VDC:245
	//IC_FAULT_INDEX_RX_CAN_MSG_METER     ,	//IPC:280,2DE,355,5C5,4F2,4F8
	IC_FAULT_INDEX_RX_CAN_MSG_BCM      ,	//358,35D,60D,
	IC_FAULT_INDEX_RX_CAN_MSG_ACU       ,	//AB-SRC(ACU):02A
	IC_FAULT_INDEX_RX_CAN_MSG_RADAR       ,	//ICC(ADAS,FEB):4CC,2B7
	IC_FAULT_INDEX_RX_CAN_MSG_HVAC		,	//54C
	IC_FAULT_INDEX_RX_CAN_MSG_CVT		,	//AT/CVT(TCM):421
	IC_FAULT_INDEX_RX_CAN_MSG_VDC		,	//APB(PKB/E-PKB):58A
	IC_FAULT_INDEX_RX_CAN_MSG_EPS       ,	//5E4
	IC_FAULT_INDEX_RX_CAN_MSG_IPDM      ,	//USM/IPDM:215,625
	IC_FAULT_INDEX_RX_CAN_MSG_TPMS    ,		//385,5E8
	IC_FAULT_INDEX_RX_CAN_MSG_IKEY     ,	//351
	IC_FAULT_INDEX_RX_CAN_MSG_BSW      ,	//UPA(SONAR):57A,2C8
	IC_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,	//LKS/LDP(LDW):4CB,2B1
	//#89	BSW/BUZ 665
	IC_FAULT_INDEX_RX_CAN_MSG_SCU      ,
	IC_FAULT_INDEX_RX_CAN_MSG_LCMR      ,
	IC_FAULT_INDEX_RX_CAN_MSG_LCML      ,
	IC_FAULT_INDEX_MAX,
}tICFaultIndex;


#define AVM_MSG_LOST_STABLE_COUNTER_MAX 5

#define AVM_INNER_ID_CAN_BUS_OFF_MASK 1

#define AVM_CAN_TX_MUTE				1

//for HCM
#define AVM_RX_CAN_MSG_MASK_HCM_0X551      1



//FOR ABS_VDC
#define AVM_RX_CAN_MSG_MASK_ABS_0X284      1
#define AVM_RX_CAN_MSG_MASK_ABS_0X354      2
#define AVM_RX_CAN_MSG_MASK_VDC_0X245      4

//for BCM
#define AVM_RX_CAN_MSG_MASK_BCM_0X358      1
#define AVM_RX_CAN_MSG_MASK_BCM_0X35D      2
#define AVM_RX_CAN_MSG_MASK_BCM_0X60D      4

//for CVT
#define AVM_RX_CAN_MSG_MASK_CVT_0X421      1

//BSW
#define AVM_RX_CAN_MSG_MASK_BSW_0X57A      1
#define AVM_RX_CAN_MSG_MASK_BSW_0X5E9      2

//for STRG
#define AVM_RX_CAN_MSG_MASK_STRG_0X2      1


typedef enum{
	//The EXT_ID and INNER_ID correspond one by one.
	//Do not change the order.
	AVM_FAULT_INDEX_BUS_OFF					=0,
	AVM_FAULT_INDEX_TX_MUTE				,
	//AVM_FAULT_INDEX_RX_CAN_MSG_ECM        ,	//180,551,6E2,	580
	AVM_FAULT_INDEX_RX_CAN_MSG_HCM			,	// 551
	AVM_FAULT_INDEX_RX_CAN_MSG_ABS_VDC        ,	//ABS:284
	//AVM_FAULT_INDEX_RX_CAN_MSG_METER     ,	//IPC:280,2DE,355,5C5,4F2,4F8
	AVM_FAULT_INDEX_RX_CAN_MSG_BCM      ,	//358,60D
	//AVM_FAULT_INDEX_RX_CAN_MSG_ACU       ,	//AB-SRC(ACU):02A
	//AVM_FAULT_INDEX_RX_CAN_MSG_RADAR       ,	//ICC(ADAS,FEB):4CC,2B7
	//AVM_FAULT_INDEX_RX_CAN_MSG_HVAC		,	//54C
	AVM_FAULT_INDEX_RX_CAN_MSG_CVT		,	//AT/CVT(TCM):421
	//DA
	AVM_FAULT_INDEX_RX_CAN_MSG_STRG		,	//STRG: 2
	//AVM_FAULT_INDEX_RX_CAN_MSG_VDC		,	//APB(PKB/E-PKB):58A
	//AVM_FAULT_INDEX_RX_CAN_MSG_EPS       ,	//5E4
	//AVM_FAULT_INDEX_RX_CAN_MSG_IPDM      ,	//USM/IPDM:215,625
	//AVM_FAULT_INDEX_RX_CAN_MSG_TPMS    ,		//385,5E8
	//AVM_FAULT_INDEX_RX_CAN_MSG_IKEY     ,	//351
	AVM_FAULT_INDEX_RX_CAN_MSG_BSW      ,	//UPA(SONAR):57A,5E9
	//AVM_FAULT_INDEX_RX_CAN_MSG_LANE_CAMERA,	//LKS/LDP(LDW):4CB,2B1
	//#89	BSW/BUZ 665
	AVM_FAULT_INDEX_MAX,
}tAVMFaultIndex;

#define AVM_CAN_MSG_STATE_LEN 4



typedef boolean (*tDtcAppHook)(uint8* data, uint8 len);

typedef struct{
	uint32 dtcId;
	uint32 group;
	tDtcAppHook hook;
}tDtcGroupCbkList;



#endif
