#include "oat.h"
#include "dea.h"
#include "alga.h"
//#include "Adc_If.h"
#include "ErrorTypes.h"
#include "DeaCoding.h"
#include "string.h"
#include "MemMap.h"
#include "NvM_if.h"
#include "System_Signal.h"


/**定义温度数据变量**/
int16_t OatTempData_sum = 0;


/**591 project require define**/

OAT_Mode OAT_mode = OAT_Normal;

static IGN_STAT_ENUM Ign_state = IGN_OFF;
static 	DISPLAY_STATE Display_state =  DIS_Fixstate2;

int16_t OatTempData = 0;				//the simple CAN data value
int16_t OatTempValidFlag = 0;			//the value valid

static int16_t l_display_value = 0;			//the display value
static int16_t l_display_OldValue = 0;		//the old display value

BACKRAM_SEC_VAR_UNSPECIFIED int16_t l_display_IgnOffValue;
BACKRAM_SEC_VAR_UNSPECIFIED ExternSTDType s_TimeForOAT;
uint8_t s_RequestTimeUpdateFlag = 0;


/**591 project temp sensor res value**/
const uint16_t g_OAT_ResTable[] = 
{
	28616,
	16497,
	9917,
	6188,
	3992,
	2653,
	1812,
	1268,
	902,
	773,
	661,

};


void Oat_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
	 	OAT_mode = OAT_Normal;
		OatTempValidFlag = 0; //invalid
		OatTempData = 0;
		l_display_IgnOffValue = 0xFF;	//invalid
		memset(&s_TimeForOAT, 0, sizeof(s_TimeForOAT));
		DEA_SetOAT_Valid(OatTempValidFlag);
		OAT_DisplayProc(OatTempData);
	}
}



OAT_InputInfor Get_OatInputIUnfor()
{
	OAT_InputInfor  OatInfor;

	if(DEA_GetOAT_RawValid() == 1) //valid
	{
		OatInfor.u8OatValid = OAT_True;
	}
	else
	{
		OatInfor.u8OatValid = OAT_False;
	}

	if(DEA_GetCanTimeOutStat(IPDU_HND_CLIMATE_ControlData_F) != 0) //timeout
	{
		OatInfor.u8OatTimeoutFlag = OAT_True; //数据超时
	}
	else
	{
		OatInfor.u8OatTimeoutFlag = OAT_False; //数据未超时
	}

	OatInfor.s16OatValue = DEA_GetOAT_RawValue(); //获取温度数值 此处暂时没有处理偏移量

	OatInfor.u8OatAlive = OAT_True;

	return OatInfor;
	
}



//static SAMPLE_DATA_STRU s_oat_data;	/**sample the oat data**/

int16_t g_oat_dis_value = 0;




//funcation 1 for calculating the OAT value
int16_t OAT_Cal_OAT_Temp(uint16_t resValue)	//calculate the tempreture
{
	uint16_t l_cnt = 0;
	int16_t l_temp = 0;
	
	if(resValue > g_OAT_ResTable[0])
	{
		return -400;	//offset is -40 C//the data expand 10 times
	}
	else if(resValue <= g_OAT_ResTable[(sizeof(g_OAT_ResTable)/sizeof(g_OAT_ResTable)[0])-1])
	{
		l_temp = ((int16_t)(sizeof(g_OAT_ResTable)/sizeof(g_OAT_ResTable[0])) - 40 - 1)*10; 
	}
	else
	{
		for(l_cnt = 0; l_cnt < (sizeof(g_OAT_ResTable)/sizeof(g_OAT_ResTable)[0]); l_cnt++)		//search the table for tempreture
		{
			if(resValue > g_OAT_ResTable[l_cnt])
			{
				l_temp = (l_cnt)*10 + (g_OAT_ResTable[l_cnt-1]-resValue)*10/(g_OAT_ResTable[l_cnt-1]-g_OAT_ResTable[l_cnt]);	//search the table for tempreture
				
				if((l_temp%5) < 3)
				{
					l_temp = l_temp - l_temp%5;
				}
				else
				{
					l_temp = l_temp - l_temp%5 + 5;
				}
				
				l_temp -= ( 40 + 1)*10;
				
				break;
			}
		}
	}
	
	return l_temp;
}

//funcation 2 for calculating the OAT value
int16_t OAT_Cal_OAT_Tempreture(uint16_t resValue)		//591 project require	calculate the tempreture
{
	int16_t TempValue;
	uint32_t temp;

	if(resValue == g_OAT_ResTable[0])
	{
		TempValue = 0;
	}
	else if(resValue > g_OAT_ResTable[1])	//16497< x <28616
	{
		temp = ((resValue - g_OAT_ResTable[1])*10*100)/12119;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  0 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[1])
	{
		TempValue = 10;
	}
	else if(resValue > g_OAT_ResTable[2])
	{
		temp = ((resValue - g_OAT_ResTable[2])*10*100)/6580;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  10 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[2])
	{
		TempValue = 20;
	}
	else if(resValue > g_OAT_ResTable[3])
	{
		temp = ((resValue - g_OAT_ResTable[3])*10*100)/3729;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  20 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[3])
	{
		TempValue = 30;
	}
	else if(resValue > g_OAT_ResTable[4])
	{
		temp = ((resValue - g_OAT_ResTable[4])*10*100)/2196;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  30 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[4])
	{
		TempValue = 40;
	}
	else if(resValue > g_OAT_ResTable[5])
	{
		temp = ((resValue - g_OAT_ResTable[5])*10*100)/1339;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  40 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[5])
	{
		TempValue = 50;
	}
	else if(resValue > g_OAT_ResTable[6])
	{
		temp = ((resValue - g_OAT_ResTable[6])*10*100)/841;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  50 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[6])
	{
		TempValue = 60;
	}
	else if(resValue > g_OAT_ResTable[7])
	{
		temp = ((resValue - g_OAT_ResTable[7])*10*100)/544;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  60 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[7])
	{
		TempValue = 70;
	}
	else if(resValue > g_OAT_ResTable[8])
	{
		temp = ((resValue - g_OAT_ResTable[8])*10*100)/366;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  70 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[8])
	{
		TempValue = 80;
	}
	else if(resValue > g_OAT_ResTable[9])
	{
		temp = ((resValue - g_OAT_ResTable[9])*10*100)/129;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  80 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[9])
	{
		TempValue = 85;
	}
	else if(resValue > g_OAT_ResTable[10])
	{
		temp = ((resValue - g_OAT_ResTable[10])*5*100)/112;	//expand 100 times
		temp +=50;	//get the int
		TempValue =  85 + temp/100;
	}
	else if(resValue == g_OAT_ResTable[10])
	{
		TempValue = 90;
	}

	return TempValue;

}

int16_t OAT_GetDisplayValue(void)//获取当前显示的温度值
{
	return g_oat_dis_value;
}

void OAT_SetDisplayValue(int16_t value)//配置仪表显示温度数据
{
	if(l_display_value < -40)
	{
		DEA_SetOAT_FilterValue(-40);
	}
	else if(l_display_value > 60)
	{
		DEA_SetOAT_FilterValue(60);
	}
	else
	{
		DEA_SetOAT_FilterValue(value);
	}
}

void OAT_DisplayProc(int16_t new_oat_value)//配置温度数据变化显示速度
{
	static uint8_t s_timeCnt = 0U;
	int16_t l_display_value;	

	l_display_value = OAT_GetDisplayValue();

	if(new_oat_value < l_display_value) //
	{
		s_timeCnt++;
		if(s_timeCnt > OAT_TIME1) //data down: cycle 2s 591 project require
		{
			s_timeCnt = 0;
			l_display_value -= 1;
			g_oat_dis_value = l_display_value;	//the last time display value
			OAT_SetDisplayValue(l_display_value);//update the display temp value

		}
	}
	else if(new_oat_value > l_display_value) //data up: cycle 2s(at the same time,the vehicle speed shuold >= 20km/h)
	{
		s_timeCnt++;
		if(s_timeCnt > OAT_TIME2)	//data up : cycle 2s	591 project require
		{
			s_timeCnt = 0;
			l_display_value += 5;
			g_oat_dis_value = l_display_value;	//the last time display value
			OAT_SetDisplayValue(l_display_value);//update the display temp value

		}	
	}
}

void OAT_SetRequestTimeUpdate(uint8_t value)
{
	s_RequestTimeUpdateFlag = value;
}

inline uint32_t OAT_MakeTime(uint32_t year, uint32_t mon, uint32_t day, uint32_t hour, uint32_t min, uint32_t sec)
{
    if(0 >= (int32_t) (mon -= 2)) {    /* 1..12 -> 11,12,1..10 */
         mon += 12;      /* Puts Feb last since it has leap day */
         year -= 1;
    }

    return ((((
             (uint32_t) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
             year*365 - 719499
          )*24 + hour /* now have hours */
       )*60 + min /* now have minutes */
    )*60 + sec); /* finally seconds */
}


uint8_t OAT_IgnOffTimeJudge(ExternSTDType time_now, ExternSTDType time_last)
{
	uint32_t time_n;
	uint32_t time_l;
	if(time_now.year < time_last.year)
	{
		return 1;
	}

	time_l = OAT_MakeTime(time_last.year+2000, time_last.month, time_last.day, time_last.hour, time_last.minutes, time_last.second);
	time_n = OAT_MakeTime(time_now.year+2000, time_now.month, time_now.day, time_now.hour, time_now.minutes, time_now.second);

	if(time_n >= time_l)
	{
		if((time_n-time_l) >= SettingTimeForTs)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 1;
	}
}

//funcation 1: CAN data
void OAT_Main(void)
{
	static uint16_t s_rest1 = 0;
	static uint16_t s_rest2 = 0;
	static uint8_t IgnStatForTime = IGN_OFF;
	
	OAT_InputInfor OatInforMation;

	static uint16_t state1_timecnt = 0;
	static uint16_t Fixstate1_timecnt = 0;
	int32_t temp0 = 0;

	static uint16_t cycle_timecnt = 0;
	static uint16_t invalid_cycle_timecnt = 0;
	//static uint16_t add_cnt = 0;

	static uint8_t Oat_Mode = 0;

	static uint16_t test_data = 0;

	int16_t OAT_temp = 0;

	static uint8_t OAT_TimeCnt = 0;

	static TELLTALE_ENUM LowOat_Warn = TELLTALE_OFF;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{	
		if(IgnStatForTime == IGN_OFF)
		{
			IgnStatForTime = IGN_ON;
			s_RequestTimeUpdateFlag = 0;
			System_Signal_Send(SYSTEM_SIG_ID_BC_READRTC, STD_ON, STD_OFF);//re trigger
		}
		
		if(OAT_TimeCnt < 25)
		{
			OAT_TimeCnt++;
		}
		else if(semiActiveOAT_WithGet())
		{
			
			OatInforMation = Get_OatInputIUnfor();//获取温度数据信息
			if((OatInforMation.s16OatValue - 405) >= 0)
			{
				OAT_temp = (OatInforMation.s16OatValue - 405) + 5;
			}
			else
			{
				OAT_temp = (OatInforMation.s16OatValue - 405) - 5;
			}
			
			if(Ign_state == IGN_OFF)
			{
				Ign_state = IGN_ON;

				if((OatInforMation.u8OatValid == OAT_True) && (OatInforMation.u8OatAlive == OAT_True) && (OatInforMation.u8OatTimeoutFlag == OAT_False))
				{
					Oat_Mode = 1;
				}

				Display_state = DIS_Fixstate2;	//update the display state 


				if((OatInforMation.s16OatValue <= MAX_OAT_TEMP) && (OatInforMation.s16OatValue >= MIN_OAT_TEMP)) //限幅处理(expanded 10 times 0~1270)
				{	
					
					l_display_value = OAT_temp/10;// 偏移量-40.5,expand 10 times,四舍五入处理 (传感器的数据值)
					if(s_RequestTimeUpdateFlag == 1)
					{
						if(l_display_value > l_display_IgnOffValue)
						{
							if(OAT_IgnOffTimeJudge(DEA_GetRtcTime(), s_TimeForOAT) == 0)
							{
								l_display_value = l_display_IgnOffValue;
							}
						}
						s_RequestTimeUpdateFlag = 0;
					}
					l_display_OldValue = l_display_value;
					OatTempValidFlag = 1; // valid 向上层发送数据有效标志
				
				}
				else
				{
					l_display_value = 0;
					l_display_OldValue = l_display_value;
					OatTempValidFlag = 0; // invalid	
				}

			}

			if((OatInforMation.u8OatValid == OAT_True) && (OatInforMation.u8OatAlive == OAT_True) && (OatInforMation.u8OatTimeoutFlag == OAT_False))//591 project  OAT CAN handle
			{	
				invalid_cycle_timecnt = 0;
				if(0 == Oat_Mode)	//from timeout to normal
				{
					Oat_Mode = 1;
					
					if((OatInforMation.s16OatValue <= MAX_OAT_TEMP) && (OatInforMation.s16OatValue >= MIN_OAT_TEMP)) //限幅处理(expanded 10 times 0~1270)
					{	
					
						l_display_value = OAT_temp/10;// 偏移量-40.5,expand 10 times,四舍五入处理 (传感器的数据值)
						l_display_OldValue = l_display_value;
					
						OatTempValidFlag = 1; // valid 向上层发送数据有效标志
					
					}
					else
					{
						l_display_value = 0;
						l_display_OldValue = l_display_value;
						OatTempValidFlag = 0; // invalid	
					}

					OAT_SetDisplayValue(l_display_value);	//update the display temp value
					DEA_SetOAT_Valid(OatTempValidFlag); 	//update the display valuevalid
				}
				else
				{
					if((OatInforMation.s16OatValue <= MAX_OAT_TEMP) && (OatInforMation.s16OatValue >= MIN_OAT_TEMP)) //限幅处理(expanded 10 times 0~1270)
					{	
					
						OatTempData = OAT_temp/10;// 偏移量-40.5,expand 10 times,四舍五入处理 (传感器的数据值)
						OatTempValidFlag = 1; // valid 向上层发送数据有效标志
					}
					else
					{
						OatTempData = 0;
						OatTempValidFlag = 0; // invalid 	
					}
										
					switch(Display_state)
					{
						case DIS_Fixstate1:

										//add_cnt = 0;
										l_display_value = l_display_OldValue;	//update the display value
										l_display_OldValue = l_display_value;	//update the old display value
										Fixstate1_timecnt++;
										
										if(DEA_GetFilterVehicleSpeed() > 200)	// >20km/h
										{
											Display_state = DIS_state2;
										}
										else if(l_display_value > OatTempData)
										{
											Display_state = DIS_state4;
										}
										else if(Fixstate1_timecnt >= 300)	//30s
										{
											Display_state = DIS_Fixstate2;
										}

							break;
							
						case DIS_Fixstate2:

										//add_cnt = 0;
										l_display_value = l_display_OldValue;	//update the display value
										l_display_OldValue = l_display_value;	//update the old display value

										if(DEA_GetFilterVehicleSpeed() > 200)	// >20km/h the speed have expanded 10 times
										{
											Display_state = DIS_state1;
										}
										else if(l_display_value > OatTempData)
										{
											Display_state = DIS_state5;
										}
							break;	
							
						case DIS_state1:
										#if 0
										if(OatTempData > l_display_OldValue)
										{
											add_cnt++;
										}

										if(add_cnt >= 1024)
										{
											l_display_value = l_display_OldValue + 1;
											add_cnt = 0;
										}
										#endif 

										l_display_value = l_display_OldValue + (OatTempData - l_display_OldValue + s_rest1)/1024;
										s_rest1 = (OatTempData - l_display_OldValue + s_rest1)%1024;	//get the rest

										
										l_display_OldValue = l_display_value;	//update the old value
										state1_timecnt++;	//start the time cnt
										
										if(DEA_GetFilterVehicleSpeed() <= 200)	//<= 20km/h
										{	
											state1_timecnt = 0;
											Display_state = DIS_Fixstate2;
										}
										else if(l_display_value > OatTempData)
										{	
											state1_timecnt = 0;
											Display_state = DIS_state6;
										}
										else if(state1_timecnt >= 3000)	//5min
										{
											state1_timecnt = 0;
											Display_state = DIS_state2;
										}
							break;
							
						case DIS_state2:
										#if 0
										test_data = DEA_GetRawVehicleSpeed()/10;	//the inicial vechicle speed
										temp0 = (16533 - (187*test_data))/100;	//-283~165
										if(temp0 < 32)
										{
											temp0 = 32;	//32~165
										}

										if(l_display_OldValue < OatTempData )	//OAT rise
										{
											add_cnt++;

											if(add_cnt >= 165)
											{
												add_cnt = 0;
											}
										}

										if(add_cnt >= temp0)
										{
											l_display_value = l_display_OldValue + (add_cnt/temp0);
											add_cnt = 0;
										}
										else
										{
											l_display_value = l_display_OldValue;
										}
										#endif

										test_data = DEA_GetRawVehicleSpeed()/10;	//the inicial vechicle speed
										temp0 = (16533 - (187*test_data))/100;
										if(temp0 <= 32)
										{
											temp0 = 32;
										}
										l_display_value = l_display_OldValue + (OatTempData - l_display_OldValue + s_rest2)/temp0;
										s_rest2 = (OatTempData - l_display_OldValue + s_rest2)%temp0;	//get the rest


										
										l_display_OldValue = l_display_value;

										if(DEA_GetFilterVehicleSpeed() <= 200)	//<= 20km/h
										{
											Display_state = DIS_Fixstate1;
										}
										else if(l_display_value > OatTempData)
										{
											Display_state = DIS_state3;
										}
							
							break;
							
						case DIS_state3:

										//add_cnt = 0;
										if(l_display_OldValue <= OatTempData)
										{
											Display_state = DIS_state2;
										}
										
										l_display_value = OatTempData;
										l_display_OldValue = l_display_value;
										

							break;
							
						case DIS_state4:

										//add_cnt = 0;
										if(l_display_OldValue <= OatTempData)
										{
											Display_state = DIS_Fixstate1;
										}
										
										l_display_value = OatTempData;
										l_display_OldValue = l_display_value;
							break;
							
						case DIS_state5:

										//add_cnt = 0;
										if(l_display_OldValue <= OatTempData)
										{
											Display_state = DIS_Fixstate2;
										}
										
										l_display_value = OatTempData;
										l_display_OldValue = l_display_value;
							break;
							
						case DIS_state6:

										//add_cnt = 0;
										if(l_display_OldValue <= OatTempData)
										{
											Display_state = DIS_state1;
										}
										
										l_display_value = OatTempData;
										l_display_OldValue = l_display_value;
							break;	

						default :
							break;
					}

					//update display cycle 2s
					if(cycle_timecnt < 20)	//less than 2S
					{
						cycle_timecnt++;
					}
					else					//2s cycle time
					{
						cycle_timecnt = 0;	
						OAT_SetDisplayValue(l_display_value);	//update the display temp value
						DEA_SetOAT_Valid(OatTempValidFlag); 	//update the display valuevalid
					}
				}

				if(OatTempValidFlag == 1)
				{
					//warn handle
					if(l_display_value <= 3)	// have expand 10 times		< 3°
					{
						if(LowOat_Warn != TELLTALE_ON)
						{
							LowOat_Warn = TELLTALE_ON;
							IPC_PutAlarmMsg(NORMAL_MSG_ID_Snow, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);
						}
					}
					else if(l_display_value >= 4)	// have expand 10 times		> 4°
					{
						if(LowOat_Warn != TELLTALE_OFF)
						{
							LowOat_Warn = TELLTALE_OFF;
							IPC_PutAlarmMsg(NORMAL_MSG_ID_Snow, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
						}
					}
				}
				else
				{
					if(LowOat_Warn != TELLTALE_OFF)
					{
						LowOat_Warn = TELLTALE_OFF;
						IPC_PutAlarmMsg(NORMAL_MSG_ID_Snow, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
					}
				}
			}
			else	
			{
				if(OatTempValidFlag != 0)
				{
					//update display cycle 2s
					if(invalid_cycle_timecnt < 20)	//less than 2S
					{
						invalid_cycle_timecnt++;
					}
					else					//2s cycle time
					{
						OatTempData = 0;
						//add_cnt = 0;
						Oat_Mode = 0;

						OatTempValidFlag = 0;	//shouw "---"
						
						DEA_SetOAT_Valid(OatTempValidFlag); 	//update the display valuevalid
					}
				}
				
				if(LowOat_Warn != TELLTALE_OFF)
				{
					LowOat_Warn = TELLTALE_OFF;
					IPC_PutAlarmMsg(NORMAL_MSG_ID_Snow, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
				}
			}
		}		
	}
	else
	{
		if(semiActiveOAT_WithGet())
		{
			if(Ign_state == IGN_ON)
			{
				Ign_state = IGN_OFF;
				l_display_OldValue = l_display_value;
				if(OatTempValidFlag == 1)
				{
					l_display_IgnOffValue = l_display_value;
				}
				else
				{
					l_display_IgnOffValue = 0xFF;	//invalid
				}
				OAT_TimeCnt = 0;
				OatTempData = 0;
				//add_cnt = 0;
				Oat_Mode = 0;
				OAT_SetDisplayValue(0);		//update the display temp value
				DEA_SetOAT_Valid(0);		//update the display valuevalid
				if(LowOat_Warn != TELLTALE_OFF)
				{
					LowOat_Warn = TELLTALE_OFF;
					IPC_PutAlarmMsg(NORMAL_MSG_ID_Snow, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
				}
			}

			if(IgnStatForTime == IGN_ON)
			{
				IgnStatForTime = IGN_OFF;
				s_RequestTimeUpdateFlag = 0;
				System_Signal_Send(SYSTEM_SIG_ID_BC_READRTC, STD_ON, STD_OFF);//re trigger
			}
			
			if(s_RequestTimeUpdateFlag == 1)
			{
				s_TimeForOAT = DEA_GetRtcTime();
				s_RequestTimeUpdateFlag = 0;
			}
		}
	}
}


//funcation 2: ResValue data: search the res value table for tempreture 
/**591 project requirement**/
#if 0
uint32_t g_oatres = 0;
void OAT_Main(void)
{
	OAT_InputInfor OatInforMation;
	
	static IGN_STAT_ENUM s_ignStat = IGN_OFF;
	static uint8_t init_display_flag = 0;
	static uint16_t l_sensorErrCnt = 0, l_sensorNormalCnt = 0U;
	static uint8_t l_sensorErrStat = 0;
	uint16_t l_adValue = 0;
	uint16_t l_res_value = 0, l_raw_res_value = 0;
	int16_t l_oatValue = 0;

	uint8_t l_UpdateCnt = 0;

	//if(DEA_GetCodingOATOnOff() == DEA_CODING_ON)
	//{
		if(DEA_GetIgnStat() == IGN_ON)
		{
			l_UpdateCnt++;	//update cycle timecnt
			
			if(s_ignStat == IGN_OFF)//IGN_OFF -> IGN_ON
			{
				s_ignStat = IGN_ON;
				SampleDataReset(&s_oat_data);//reset the data every IGN_ON
			}
			
			if(/*GetBatOatAdVal(&l_adValue) == E_OK*/1)	/**get the oat ADvalue**/
			{
				l_adValue = 661;
				l_raw_res_value = l_adValue; 
				//g_oatres = l_raw_res_value;
				SampleDataAdd(&s_oat_data, l_raw_res_value);


				l_res_value = CalAvgSampleData(&s_oat_data);	/*! average filter */


				//judge the data valid 
				if((l_res_value >= OAT_SENSOR_MIN) && (l_res_value <= OAT_SENSOR_MAX))	/*! valid sensor range is 100 to 18K unit:ohm */
				{
					l_oatValue = OAT_Cal_OAT_Tempreture(l_res_value);	//caculate the temp value buy the ResValue
					DEA_SetOAT_RawValid(1);
					DEA_SetOAT_RawValue(l_oatValue);
					DEA_SetOAT_RawResValid(1U);
					DEA_SetOAT_RawResValue(l_raw_res_value/100);	/**narrow 100 times to the true resvalue**/
				}
				else
				{
					
					DEA_SetOAT_RawValid(0);
					DEA_SetOAT_RawValue(0);
					DEA_SetOAT_RawResValid(0U);
					DEA_SetOAT_RawResValue(0U);
				}
		      }
			 
			  
			#if 0
			if(s_oat_data.Cnt >= 10)
			{
				l_res_value = CalAvgSampleData(&s_oat_data);	/*! average filter */

				
				if((l_res_value >= OAT_SENSOR_MIN) && (l_res_value <= OAT_SENSOR_MAX)) /*! valid sensor range is 100 to 18K unit:ohm */
				{
					l_sensorErrCnt = 0;
					if(l_sensorErrStat == 1U)
					{
						if(l_sensorNormalCnt < OAT_TIME_E)
						{
							l_sensorNormalCnt++;
						}
						else
						{
							l_sensorErrStat = 0;	/* enter normal mode */
							DEA_SetOAT_Valid(1U); 
							DEA_SetOATSigStat(DEA_OAT_SIG_NORMAL);
						}
					}
					else
					{
						l_oatValue = OAT_Cal_OAT_Temp(l_res_value); //reserch for the table to calculate temp value 

						if(init_display_flag == 0)
						{
							init_display_flag = 1U;
							DEA_SetOAT_Valid(1U);
							OAT_SetDisplayValue(l_oatValue);
						}
						else
						{
							OAT_DisplayProc(l_oatValue);
						}
					}
				}
				else		/**not in the valid value range**/
				{
					l_sensorNormalCnt = 0;
					if(l_sensorErrStat == 0)
					{
						if(l_sensorErrCnt < OAT_TIME_E)
						{
							l_sensorErrCnt++;
						}
						else
						{
							l_sensorErrStat = 1U;
							DEA_SetOAT_Valid(0);
							DEA_SetOAT_FilterValue(0);
							if(l_res_value < OAT_SENSOR_MIN)
							{
								DEA_SetOATSigStat(DEA_OAT_SIG_SHORT);
							}
							else 
							{
								DEA_SetOATSigStat(DEA_OAT_SIG_OPEN);
							}
						}
					}
				}

				SampleDataReset(&s_oat_data);
			}
			#endif

			//if(l_UpdateCnt >= 20)	//591 project require 2s cycle
			//{
				l_UpdateCnt = 0;
				OAT_DisplayProc(l_oatValue);
			//}
		}
		else
		{
			if(s_ignStat == IGN_ON)
			{
				s_ignStat = IGN_OFF;
				DEA_SetOAT_RawValid(0U);
				DEA_SetOAT_RawValue(0);
				DEA_SetOAT_RawResValid(0);
				DEA_SetOAT_RawResValue(0);
			}
			OAT_DisplayProc(0);
		}
	//}
}


void OAT_Main(void)
{
	static uint32_t res_value_sum; 
	static uint32_t res_value_cnt;
	static IGN_STAT_ENUM s_ignStat = IGN_OFF;



	uint16_t l_adValue = 0;
	int16_t l_oatValue = 0;
	
	 static uint16_t l_res_value = 0, l_raw_res_value = 0;
	 
	 if(DEA_GetIgnStat() == IGN_ON)
	 {	
		
		
	 	if(s_ignStat == IGN_OFF)//IGN_OFF -> IGN_ON
		{
			s_ignStat = IGN_ON;
			SampleDataReset(&s_oat_data);	//reset the data every IGN_ON
		}

		if(/*GetBatOatAdVal(&l_adValue) == E_OK*/1)	//get the OAT ADvalue
		{
			l_adValue = 20000;
			//judge the data valid 
			if((l_adValue >= OAT_SENSOR_MIN) && (l_adValue <= OAT_SENSOR_MAX))	/*! valid sensor range is 100 to 18K unit:ohm */
			{
				res_value_sum += l_adValue;
				res_value_cnt++;

				if(res_value_cnt >= 12)		//average fillter
				{	
					l_res_value = res_value_sum/res_value_cnt;
					res_value_cnt = 0;
					res_value_sum = 0;
				}

				l_oatValue = OAT_Cal_OAT_Tempreture(l_res_value);	//caculate the temp value buy the ResValue
				l_oatValue -=30;
			}
			
		}
		
		OAT_DisplayProc(l_oatValue);

	 }
	 else
	 {
	 	if(s_ignStat == IGN_ON)
		{
			s_ignStat = IGN_OFF;
			
			res_value_sum = 0;
			res_value_cnt = 0;

		}

		OAT_DisplayProc(0);

	 }
}

#endif

