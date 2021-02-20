#include "CanApp_TxProc.h"
#if (STD_ON == TASK_CANAPP_DEBUG_TRACE)
#include "trace.h"
#endif

#include "Com_If.h"
#include "dea.h"
//#include "Task_IpcApp_Cfg.h"
#include "Vel.h"
#include "CanApp_PduMsgProc.h"
#include "FuelProc.h"
#include "adc_wrapper_if.h"	 /**AD sample file**/
#include "pin_wrapper_if.h"
#include "dte.h"
#include "BattVoltage.h"
const uint16 s_fuel_gauge_para[][2] =
{
	{ 510,	431},
	{ 910,	608},
	{1170,	689},
	{1400,	745},
	{1630,	791},
	{1860,	830},
	{2090,	863},
	{2320,	892},
	{2550,	917},
	{2780,	939},
	{2805,	941},
	{2830,	943},
};

#define sonar_setting_flag_mask (0x01)
#define its_ldw_flag_mask 		(0x01<<1)
#define its_feb_flag_mask 		(0x01<<2)
#define its_bsw_flag_mask 		(0x01<<3)
#define sonar_cancel_flag_mask 	(0x01<<4)
#define eapm_flag_mask 			(0x01<<5)
#define factory_flag_mask 		(0x01<<6)



static uint16_t s_car_setting_cnt = 0;
static uint8_t s_car_setting_flag = 0;
/*static uint8_t s_sonar_setting_flag = 0; 
static uint8_t s_its_ldw_flag = 0; 
static uint8_t s_its_feb_flag = 0; 
static uint8_t s_its_bsw_flag = 0; 
static uint8_t s_sonar_cancel_flag = 0; 
*/
//static uint8_t s_ECO_FLG=0U;


void CanApp_TxMsgInit(void)
{
	uint8_t l_u8Value = 0;
	//uint8_t l_active_counter = 0;
	//Com_TxSig_ALIV_IC_ODO(CAN_APP_TX_SIG, &l_active_counter);
	boolean l_boolValue;
	l_boolValue = 1;
	Com_TxSig_DC_EXISTANCE(COM_SET_SIG_VALUE, &l_boolValue);
	l_boolValue = 0;
	Com_TxSig_FUEL_FirstW_L(COM_SET_SIG_VALUE, &l_boolValue);

	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);

	l_u8Value = 0x40;
	Com_TxSig_MARKET(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxSonarSetting(uint8_t status)
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= sonar_setting_flag_mask;
	s_car_setting_cnt = 20;
	if(status)
	{
		l_u8Value = 0b101101;
	}
	else
	{
		l_u8Value = 0b101100;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxITS_BSWSetting(uint8_t status)
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= its_bsw_flag_mask;
	s_car_setting_cnt = 20;
	if(status)
	{
		l_u8Value = 0b110;
	}
	else
	{
		l_u8Value = 0b101;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}


void CanApp_TxITS_LDWSetting(uint8_t status)
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= its_ldw_flag_mask;
	s_car_setting_cnt = 20;
	if(status)
	{
		l_u8Value = 0b100;
	}
	else
	{
		l_u8Value = 0b011;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxITS_FEBSetting(uint8_t status)
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= its_feb_flag_mask;
	s_car_setting_cnt = 20;
	if(status == 1U)
	{
		l_u8Value = 0b100100;
		
	}else if(status == 0U)
	{
		l_u8Value = 0b100011;
	}else
	{
		;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxITS_LKASetting(uint8_t status)//yangxl 2019-5-30 10:53:20
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= its_feb_flag_mask;
	s_car_setting_cnt = 20;
	
	if(status == 1U)
	{
		l_u8Value = 0b001000;
		
	}else if(status == 0U)
	{
		l_u8Value = 0b000111;
	}else
	{
		;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxITS_ISSSetting(uint8_t status)//yangxl 2019-5-30 15:17:34
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= its_feb_flag_mask;
	s_car_setting_cnt = 20;
	/*
	if(status)
	{
		l_u8Value = 0b000010;
	}
	else
	{
		l_u8Value = 0b000001;
	}
	*/
	if(status == 1U) //yangxl 2019-11-4 14:26:54
	{
		l_u8Value = 0b000001;
		
	}else if(status == 0U)
	{
		l_u8Value = 0b000010;
	}else
	{
		;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxITS_UPASetting(uint8_t status)//yangxl 2019-11-14 17:05:00
{
	uint8_t l_u8Value = 0;

	if(DEA_GetIgnStat() == IGN_ON) //yangxl 2020-11-30 10:11:19
	{
		s_car_setting_flag |= its_feb_flag_mask;
		s_car_setting_cnt = 20;
		
		if(status == 1U) //yangxl 2019-11-14 17:05:09
		{
			l_u8Value = 0b101101;
			
		}else if(status == 0U)
		{
			l_u8Value = 0b101100;
		}else
		{
			;
		}
		Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
	}
	
}



void CanApp_TxEAPMSetting(uint8_t status)//yangxl 591与532定义不一样
{
	uint8_t l_u8Value = 0;
	
	s_car_setting_flag |= eapm_flag_mask;
	s_car_setting_cnt = 20;
	
	if(status == 1U)
	{
		l_u8Value = 0b100110;
	}else if(status == 0U)
	{
		l_u8Value = 0b100101;
	}else
	{
		;
	}
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void CanApp_TxECOetting(uint8_t status)//yangxl 2019-6-10 14:35:10
{
	uint8_t l_u8Value = 0;

	if(DEA_GetIgnStat() == IGN_ON) //yangxl 2020-9-11 10:15:15
	{
		s_car_setting_flag |= eapm_flag_mask;
		s_car_setting_cnt = 20; //200MS 2019-11-19 15:16:55
		if(status == 1U)
		{
			l_u8Value = 0b1;
			
		}else 
		{
			l_u8Value = 0b0;
		}
		Com_TxSig_ECO_MODE_SW(COM_SET_SIG_VALUE, &l_u8Value);
	}
}


void CanApp_TxSonarCancelRequest(void)
{
	uint8_t l_u8Value = 0;
	s_car_setting_flag |= sonar_cancel_flag_mask;
	s_car_setting_cnt = 20;
	l_u8Value = 0b101111;
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}

void VehicleResettingSetting(void)
{
	uint8_t l_u8Value = 0;
	s_car_setting_flag |= factory_flag_mask;
	s_car_setting_cnt = 20;
	l_u8Value = 0b100010;
	Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
}


#define FUAGE_FLAG
void CanApp_TxMsgProc(void)
{
	static uint8_t s_SIG_HND_EX_TEMP = 0;
	uint16_t l_vel;
	uint32_t l_u32Value;
	uint16_t l_u16Value, l_u16ValueForGauge;
	uint8_t l_u8Value;
	int16_t l_s16Value;
	boolean l_boolValue;
	static uint32_t s_count = 0;
	static IGN_STAT_ENUM s_IgnStatus = IGN_OFF;
	static uint32_t s_BestFuelCnt = 0;
	static uint32_t s_FuelAdSum = 0;
	static uint32_t s_FuelAdCnt = 0;
	static uint32_t s_FuelCapResetCnt = 0;
	
	extern uint8 Com_IpduBuf_0X280[8];
	
	s_count++;
	if(s_count > 100000)
	{
		s_count = 0;
	}

	if(s_car_setting_flag != 0)
	{
		if(s_car_setting_cnt > 0)
		{
			s_car_setting_cnt--;
		}
		else
		{
			l_u8Value = 0;
			Com_TxSig_STAT_CHG_REQ(COM_SET_SIG_VALUE, &l_u8Value);
			Com_TxSig_ECO_MODE_SW(COM_SET_SIG_VALUE, &l_u8Value);
			s_car_setting_flag = 0;
		}
	}

	//Fuel Cap reset F-31-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetFuelCapReset() != 0)
		{
			DEA_SetFuelCapReset(0);
			s_FuelCapResetCnt = 20;	//20*10 // 200ms
		}

		if(s_FuelCapResetCnt > 0)
		{
			s_FuelCapResetCnt--;
			l_boolValue = 1;
		}
		else
		{
			l_boolValue = 0;
		}
		Com_TxSig_CAP_IND_RST_STA(COM_SET_SIG_VALUE, &l_boolValue);
	}

	//MT_R F-1-1-C
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_MT_R();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_RENSWR(COM_SET_SIG_VALUE, &l_boolValue);


	//AT_N F-2-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_MT_N();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_RENSWN(COM_SET_SIG_VALUE, &l_boolValue);


	//Seatbelt F-24-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = GetPinValue_IOC_DR_SAFETY_BELT_WARNING();
	}
	else
	{
		l_boolValue = 0;
	}
	//Com_TxSig_MTSRC_SW(COM_SET_SIG_VALUE, &l_boolValue);	//BUC_SW
	Com_TxSig_BAC_SW(COM_SET_SIG_VALUE, &l_boolValue);	//BAC_SW
	
	//Seatbelt F-24-2
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = GetPinValue_IOC_AS_SAFETY_BELT_WARNING();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_BAC_SW_AS(COM_SET_SIG_VALUE, &l_boolValue);	//BAC_SW_AS



#if 1
	//O_D_OFF F-3-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = GetPinValue_IOC_D_MODE_SW();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_ODOFFSW(COM_SET_SIG_VALUE, &l_boolValue);
#endif
	//M_RANGE F-7-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_M_RANGE();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_MRNG(COM_SET_SIG_VALUE, &l_boolValue);

	
	//other than M_RANGE F-8-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_NOT_M_RANGE();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_NOTMRNG(COM_SET_SIG_VALUE, &l_boolValue);


	//AT_UP F-9-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_AT_SHIFT_UP();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_UPSW(COM_SET_SIG_VALUE, &l_boolValue);

	//AT_DOWN F-10-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_AT_SHIFT_DOWN();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_DOWNSW(COM_SET_SIG_VALUE, &l_boolValue);

#if 0
	//ECO F-37-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		//l_boolValue = GetPinValue_IOC_ECO_MODE_DET(); //cancel ECO MODE SW via H/W
		l_boolValue = s_ECO_FLG; //2019-6-10 14:43:14
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_ECO_MODE_SW(COM_SET_SIG_VALUE, &l_boolValue);
#endif

	//PKB SW Signal F-23-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = GetPinValue_IOC_PKB();
	}
	else
	{
		l_boolValue = 0;
	}
	//CanApp_TxSig_HandBreakSwitch(COM_SET_SIG_VALUE, &l_boolValue);
	Com_TxSig_HandBreakSwitch(COM_SET_SIG_VALUE, &l_boolValue);
	//id 280 transmit pkb with error

	//Brake oil Sw Signal F-27-1
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = GetPinValue_IOC_BRAKE_OIL();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_BRAKE_SYSTEM(COM_SET_SIG_VALUE, &l_boolValue);
	//ID:280 byte7 bit2 waiting


	
	/*********************************************************/
	AdcWrapper_Read(ADC_LOGIC_CH_FUEL_AD, &l_u16Value); //F-25-2-C

	l_u16Value = l_u16Value/4;
	s_FuelAdSum += l_u16Value;
	s_FuelAdCnt++;

	if(s_FuelAdCnt >= 10)
	{
		s_FuelAdSum /= 2;
		s_FuelAdCnt /= 2;
	}

	l_u16Value =  s_FuelAdSum/s_FuelAdCnt;
	DEA_SetFMAD(l_u16Value);
#ifndef FUAGE_FLAG
	Com_TxSig_FMad(COM_SET_SIG_VALUE, &l_u16Value);
#else
	#if 0
	Com_IpduBuf_0X280[6] = l_u16Value&0xFF;
	Com_IpduBuf_0X280[7] &= 0x3F;
	Com_IpduBuf_0X280[7] |= (((l_u16Value>>8)&0x03) << 6);
	#else 
	l_u16Value &= 0x3FF;
	l_u16ValueForGauge = l_u16Value;
	l_u8Value =  (l_u16Value>>8)&0x03;
	l_u16Value = ((l_u16Value&0xFF)<<2)+l_u8Value;
	Com_TxSig_FMad(COM_SET_SIG_VALUE, &l_u16Value);
	#endif
#endif
	/*
	if(l_u16ValueForGauge < 720)
	{
		l_u16Value = (l_u16ValueForGauge*125U)/100-4095/(1023-l_u16ValueForGauge);
	}
	else
	{
		l_u16Value = ((uint32_t)l_u16ValueForGauge*1233U)/1000;
	}
	*/
	

	if((FUEL_GetFuelSensorResNow() >= FUEL_SENSOR_OPEN) || (FUEL_GetFuelSensorResNow() == 0))
	{
		l_u16Value = 1022;
	}
	else if(FUEL_GetFuelSensorResNow() <= FUEL_SENSOR_SHORT)
	{
		l_u16Value = 1;
	}
	else
	{	
		l_u16Value = lookTableTrans(FUEL_GetFuelSensorResNow(), s_fuel_gauge_para, (sizeof(s_fuel_gauge_para)/sizeof(s_fuel_gauge_para[0])));
	}
	DEA_SetFGAUGE(l_u16Value);
	
#if 0
#ifndef FUAGE_FLAG
	Com_TxSig_FGAUGE(COM_SET_SIG_VALUE, &l_u16Value);
#else
	#if 0
	Com_IpduBuf_0X280[1] = l_u16Value&0xFF;
	Com_IpduBuf_0X280[2] &= 0x3F;
	Com_IpduBuf_0X280[2] |= (((l_u16Value>>8)&0x03) << 6);
	#else
	l_u16Value &= 0x3FF;
	l_u8Value = (l_u16Value>>8)&0x03;
	l_u16Value = ((l_u16Value&0xFF)<<2)+l_u8Value;
	Com_TxSig_FGAUGE(COM_SET_SIG_VALUE, &l_u16Value);
	#endif
#endif
#else
	Com_TxSig_FGAUGE(COM_SET_SIG_VALUE, &l_u16Value); //yangxl 2019-6-28 09:48:55
#endif

	//============== DTE =====================
	if(Get_DteState() == DTE_DIS_PRE)
	{
		l_boolValue = 0;
		Com_TxSig_FUEL_SecondW(COM_SET_SIG_VALUE, &l_boolValue);
		Com_TxSig_FUEL_LOW(COM_SET_SIG_VALUE, &l_boolValue);	//low fuel active/deactive
	}
	else if(Get_DteState() == DTE_DIS_SECOND)
	{
		l_boolValue = 1;
		Com_TxSig_FUEL_SecondW(COM_SET_SIG_VALUE, &l_boolValue);
		Com_TxSig_FUEL_LOW(COM_SET_SIG_VALUE, &l_boolValue);	//low fuel active/deactive
	}
	else
	{
		/*if(DEA_GetFuelSigStat() != FuelSigNormal)
		{
			l_boolValue = 1;
			Com_TxSig_FUEL_SecondW(COM_SET_SIG_VALUE, &l_boolValue);
		}
		else*/
		{
			l_boolValue = 0;
			Com_TxSig_FUEL_SecondW(COM_SET_SIG_VALUE, &l_boolValue);
			Com_TxSig_FUEL_LOW(COM_SET_SIG_VALUE, &l_boolValue);	//low fuel active/deactive
		}
	}

	if((Get_DteState() == DTE_DIS_INVALID) || (DEA_GetFuelSigStat() != FuelSigNormal) || (DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 1))
	{
		l_u16Value = 0xFFFF;
	}
	else
	{
		if(DEA_GetDTE() >= 50)
		{
			l_u16Value = DEA_GetDTE()*10;
		}
		else
		{
			l_u16Value = 0xFFFF;
		}
	}

	Com_SendSignal(SIG_HND_DTE, &l_u16Value);

	/******************** Fuel ********************/
	if(FUEL_GetFuelVolumeValid() == 1) //F-25-2-C
	{
		l_u16Value = (uint16_t) (FUEL_GetFuelVol()/10);
	}
	else
	{
		l_u16Value = 0x3FF;
	}
	Com_SendSignal(SIG_HND_FUEL_Remain, &l_u16Value);
#if 0
	//G-6-2 Fuel low
	if(FUEL_GetFuelVolumeValid() == 1)
	{
		if(FUEL_GetFuelLowStatus())
		{
			l_boolValue = 1;
		}
		else
		{
			l_boolValue = 0;
		}
	}
	else
	{
		l_u8Value = 0;
	}
	
	Com_TxSig_FUEL_LOW(COM_SET_SIG_VALUE, &l_boolValue);	//low fuel active/deactive
#endif
	
	/********************* B-4-4 Vehicle Speed output *****************/
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(VEL_vGetVelMode() == VEL_Normal)
		{
			l_u32Value = (uint32_t)DEA_GetRawVehicleSpeedCanOutput();
			//l_vel = (l_u32Value*3387)/3350;
			l_vel = (l_u32Value*351)/351; //2019-8-29 10:07:05
			if(l_vel > 35000)
			{
				l_vel = 35000;
			}
			
			Com_SendSignal(SIG_HND_VSP20_MET, &l_vel);
			Com_SendSignal(SIG_HND_VSP40_MET, &l_vel);
		}
		else
		{
			l_vel = 0;
			Com_SendSignal(SIG_HND_VSP20_MET, &l_vel);
			Com_SendSignal(SIG_HND_VSP40_MET, &l_vel);
		}
	}
	else
	{
		l_vel = 0;
		Com_SendSignal(SIG_HND_VSP20_MET, &l_vel);
		Com_SendSignal(SIG_HND_VSP40_MET, &l_vel);
	}

	DEA_SetVehicleSpeedOutPut(l_vel);/* yangxl 诊断读取车速输出值 B-4-4 */

	if(DEA_GetIgnStat() == IGN_ON)
	{
		//Display speed F-28-1
		if(VEL_vGetVelMode() == VEL_Normal)
		{
			l_u16Value = DEA_GetFilterVehicleSpeed()*10;
		}
		else
		{
			l_u16Value = 0;
		}
		Com_SendSignal(SIG_HND_VS_DISP, &l_u16Value);
	}
	else
	{	
		l_u16Value = 0;
		Com_SendSignal(SIG_HND_VS_DISP, &l_u16Value);
	}
	//Display speed unit F-29-1
	l_boolValue = 0;
	//Com_TxSig_DisplayUnit(COM_SET_SIG_VALUE, &l_boolValue);
	Com_SendSignal(SIG_HND_Display_Unit_355, &l_boolValue);


	//F-36-2 STRT switch

	l_boolValue = DEA_GetHardwareStatus(HARD_ECO_MODE_SW);
	//Com_TxSig_ECO_MODE_SW(COM_SET_SIG_VALUE, &l_boolValue);
	
	//F-39-1
#if 0
	l_u16Value = BattVolt_GetStatus();
	
	if((l_u16Value >= BATTVOLT_STATE_LOW)&&(l_u16Value<=BATTVOLT_STATE_HIGH))
	{
		l_boolValue =1u;
	}else
	{
		l_boolValue =0u;
	}
#endif
	l_boolValue =0u;
	Com_TxSig_B_Circuit_ST(COM_SET_SIG_VALUE, &l_boolValue); //2019-6-11 10:19:32

	//F-49-1-T FR_DEF
	if(DEA_GetIgnStat() == IGN_ON)
	{
		l_boolValue = !GetPinValue_IOC_FR_DEF_SW();
	}
	else
	{
		l_boolValue = 0;
	}
	Com_TxSig_ACFRDEFSW(COM_SET_SIG_VALUE, &l_boolValue);
	
	//F-23-3 E-PKB SW is for hardware

	// Avarage fuel economy ID:2DE
#if 0
	if(DEA_GetAFC_A_Valid())
	{
		if(DEA_GetAFC_A_Value() > 0)
		{
			l_u16Value = 10000/DEA_GetAFC_A_Value();
		}
		else
		{
			l_u16Value = 0;
		}
	}
	else
	{
		l_u16Value = 0;
	}
#else
	if(GetAfcValid())
	{
		if(GetAfcShow() > 0)
		{
			l_u16Value = 10000/GetAfcShow();
		}
		else
		{
			l_u16Value = 0;
		}
	}
	else
	{
		l_u16Value = 0;
	}

#endif
	Com_SendSignal(SIG_HND_AVE_ECO, &l_u16Value);
	
	//
	if(DEA_GetIgnStat() == IGN_ON)
	{
		s_IgnStatus = IGN_ON;
		s_BestFuelCnt = 0;

		l_u16Value = 0x3FF;
		Com_SendSignal(SIG_HND_AVE_Fuel_Single, &l_u16Value);
	}
	else
	{
		if(s_IgnStatus == IGN_ON)
		{
			s_IgnStatus = IGN_OFF;
		}
		if(s_BestFuelCnt < 300) //3S
		{
			s_BestFuelCnt++;
			//l_u16Value = 10000/DEA_GetFilterBestHistoryAfcValue();
			/*
			if(GetHisBestAfcData() > 0u)
			{
				l_u16Value = 10000/GetHisBestAfcData(); //yangxl 2019-8-29 15:47:10
			}else
			{
				l_u16Value = 0x00;
			}
			*/
			if(GetCurrentAFData() > 0u)
			{
				l_u16Value = 10000/GetCurrentAFData(); //yangxl 2019-12-20 19:33:25
			}else
			{
				l_u16Value = 0x3FF;
			}
			
		}
		else
		{
			l_u16Value = 0x3FF;
		}
		Com_SendSignal(SIG_HND_AVE_Fuel_Single, &l_u16Value);
	}


	
	/***************** B-5-1 IC_ODO *******************/
	/*
	if((DEA_GetOdoValid()) && (DEA_GetIgnStat() == IGN_ON))
	{
		l_u32Value = DEA_GetOdo();
	}
	else
	{
		l_u32Value = 0xFFFFFF;
	}
	*/
	if(DEA_GetOdoValid()) //yangxl 2019-12-3 14:59:57
	{
		l_u32Value = DEA_GetOdo();
	}else
	{
		l_u32Value = 0xFFFFFF;
	}
	Com_TxSig_DIS_TOTAL(COM_SET_SIG_VALUE, &l_u32Value);

	/***************** F-41-1 Outside Temp ***************/
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetCanTimeOutStat(IPDU_HND_CLIMATE_ControlData_F) == 0)
		{
			if(DEA_GetOAT_CanRawValue() != 0xFF)
			{
				l_u8Value = (DEA_GetOAT_CanRawValue())/2;
			}
			else
			{
				l_u8Value = 0xFF;
			}
		}
		else
		{
			l_u8Value = 0xFF;
		}
		if(s_SIG_HND_EX_TEMP != l_u8Value)
		{
			s_SIG_HND_EX_TEMP = l_u8Value;
			Com_SendSignal(SIG_HND_EX_TEMP, &s_SIG_HND_EX_TEMP);
		}
	}
	else
	{
		if(s_SIG_HND_EX_TEMP != 0xFF)
		{
			s_SIG_HND_EX_TEMP = 0xFF;
			Com_SendSignal(SIG_HND_EX_TEMP, &s_SIG_HND_EX_TEMP);
		}
	}
#if 0
	/***************** IC_MILEAGE *******************/
	Com_TxSig_MILE_KM(CAN_APP_TX_SIG, &l_u32Value);
#endif

#if 0
	/**************** IC_SERVICE_INTERV ****************/
	if(DEA_GetDistanceValid())
	{
		l_s16Value = DEA_GetMaintainDistance();
	}
	else
	{
		l_s16Value = 32767;	//32767+32768=0xFFFF(invalid value)
	}
	Com_TxSig_ServiceIntervall(CAN_APP_TX_SIG, &l_s16Value);
#endif

	/***************** G-8-1 COM_SET_SIG_VALUE ***************/
	if(DEA_GetIgnStat() == IGN_ON)//yangxl ADD 2019-11-6 14:20:37
	{
		l_boolValue = 1;
	}else
	{
		l_boolValue = 0;
	}
	
	Com_TxSig_DC_EXISTANCE(COM_SET_SIG_VALUE, &l_boolValue);
}
