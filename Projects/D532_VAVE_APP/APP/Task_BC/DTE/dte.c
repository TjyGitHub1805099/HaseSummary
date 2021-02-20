#include "dte.h"
#include "Dte_Cfg.h"
#include "dea.h"
#include "odo.h"
#include "FuelProc.h"
#include "NvM.h"

#include "odo.h"
#include "MemMap.h"
#include "trace.h"

#include "HardwareVersion.h"


#define DTE_DEBUG
#ifdef DTE_DEBUG
static uint8_t s_dte_debug_flag = 1;
static uint32_t  get_r_afv_last = 0;
static uint32_t  get_r_afc_last = 0;
static uint32_t  dte_last = 0;

#endif

#define FUEL_FACTOR	(10000)	//ul
#define FUEL_SUM_FACTOR	(100000000)	//�?x�?F5E100�?


//#pragma segment BACK_RAM_START
BACKRAM_SEC_VAR_UNSPECIFIED DTE_STRU g_DTE_Var = {0};
//#pragma segment BACK_RAM_END

#if 0
uint8_t g_fuelConsInd = 0;

void DTE_SetFuelConsIndFlag(uint8_t stat)
{
	g_fuelConsInd = stat;
}

uint8_t DTE_GetFuelConsIndFlag(void)
{
	return g_fuelConsInd;
}

void DTE_Init(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		g_DTE_Var.dteValue = 0;
		g_DTE_Var.dteDisplayValid = 0;
		g_DTE_Var.dteDisplayValue = 0;
		g_DTE_Var.dteValid = 0;
		g_DTE_Var.dteMileage = DTE_Cfg_DefaultMillage;
		g_DTE_Var.fuelCons = DTE_Cfg_DefaultCFC;
		g_DTE_Var.dteMileageOffset = ODO_GetOdoOneMeterCnt();
	}
} 

void DTE_FuelConsumptionProc(void)
{
	uint16_t l_fuelCons;
	if(DEA_GetTII_Valid())//閻剚妞傞崰閿嬭ˉ闁插繑婀侀弫锟�?
	{
		l_fuelCons  = DEA_GetTII_Value();//閼惧嘲褰囬惉顒佹閸犻攱琛ラ柌锟�?

		g_DTE_Var.fuelCons += l_fuelCons;//鐎电懓鏌楀▽褰掑櫤鏉╂稖顢戠槐顖氬�?
	}
}

void DTE_MillageProc(uint8_t flag)
{
	if(flag == 0)
	{
		g_DTE_Var.dteMileageOffset = ODO_GetOdoOneMeterCnt();
	}
	else
	{
		if(ODO_GetOdoOneMeterCnt() > g_DTE_Var.dteMileageOffset)
		{
			g_DTE_Var.dteMileage += (ODO_GetOdoOneMeterCnt() - g_DTE_Var.dteMileageOffset);
			g_DTE_Var.dteMileageOffset = ODO_GetOdoOneMeterCnt();
		}
	}
}
volatile uint32_t g_afc = 0;
void DTE_Main(void)
{
	static uint8_t s_init_flag = 0;
	static uint16_t s_timeCnt = 0;
	uint32_t l_afc = 0;
	uint32_t l_fuelVolume; /**fuel vol**/

	if(DEA_GetIgnStat() == IGN_ON)
	{	
		if(s_init_flag == 0U)
		{
			//DTE_MillageProc(0);
			s_init_flag = 1U;
		}

		if(FUEL_GetFuelVolumeValid() == 1)
		{
			s_timeCnt++; 	/**timecnt for refreshing data**/
			
			DTE_FuelConsumptionProc(); /**caculate the fuel has consumed**/
			DTE_MillageProc(1);	/**caculate the distence has passed**/

			/*! mileage above 200km or fuel consumption above 30L  */
			if((g_DTE_Var.dteMileage > DTE_Cfg_MaxHalfMillage) || (g_DTE_Var.fuelCons > DTE_Cfg_MaxHalfFuelCons))	//over flow
			{
				g_DTE_Var.dteMileage /= 2;	//half processing
				g_DTE_Var.fuelCons /= 2;
			}
			
			l_fuelVolume = FUEL_GetFuelVol(); //unit:0.01L /**get current vol**/

			if(l_fuelVolume < (DTE_Cfg_FuelEmptyOffset*100))
			{
				l_fuelVolume = 0;
			}
			else
			{
				l_fuelVolume = l_fuelVolume -(DTE_Cfg_FuelEmptyOffset*100);
			}
			if(g_DTE_Var.dteMileage > 0)
			{
				l_afc = (g_DTE_Var.fuelCons*100/g_DTE_Var.dteMileage);	/* 0.001L/km *//**caculate average fuel consume**/
				if(l_afc > 0)
				{
					g_DTE_Var.dteValue = (l_fuelVolume*1000/l_afc); /**caculate the remain millege**/
				}
				else
				{
					g_DTE_Var.dteValue = (l_fuelVolume*1000/(DTE_Cfg_DefaultCFC/1000));
				}
			}
			else
			{
				g_DTE_Var.dteValue = (l_fuelVolume*1000/(DTE_Cfg_DefaultCFC/1000));
			}

			g_afc = l_afc;
			
			if(g_DTE_Var.dteValue > DTE_Cfg_MaxDisValue) /**limit the remain millege value**/
			{
				g_DTE_Var.dteValue = DTE_Cfg_MaxDisValue;
			}

			if((!g_DTE_Var.dteValid) || (FUEL_GetFillUpStatus() != 0))
			{
				g_DTE_Var.dteDisplayValue = g_DTE_Var.dteValue;
				g_DTE_Var.dteValid = 1U;
			}
			
			if(s_timeCnt >= DTE_Cfg_TimeInterval)	/**display refersh time rearch 30s cycle**/
			{
				s_timeCnt = 0;
				if(g_DTE_Var.dteDisplayValue < g_DTE_Var.dteValue)
				{
					g_DTE_Var.dteDisplayValue += DTE_Cfg_MaxChageStep;	/**display adding step**/
				}
				else if(g_DTE_Var.dteDisplayValue > g_DTE_Var.dteValue)
				{
					g_DTE_Var.dteDisplayValue -= DTE_Cfg_MaxChageStep;	/**display falling step**/
				}
			}

			if(g_DTE_Var.dteDisplayValue < DTE_Cfg_MinDisValue)	//below DTE_Cfg_MinDisValue(50) km show "----"
			{
				g_DTE_Var.dteDisplayValid = 0U;
			}
			else
			{
				g_DTE_Var.dteDisplayValid = 1U;
			}
		}
		else
		{
			g_DTE_Var.dteValid = 0U;
			g_DTE_Var.dteDisplayValid = 0U;
		}

		DEA_SetDTE_Valid(g_DTE_Var.dteDisplayValid);
		DEA_SetDTE(g_DTE_Var.dteDisplayValue);
	}
	else
	{
		if(g_DTE_Var.dteDisplayValue !=0)
		{
			g_DTE_Var.dteDisplayValue = 0;
		}

		g_DTE_Var.dteDisplayValid = 0;

		DEA_SetDTE_Valid(g_DTE_Var.dteDisplayValid);
		DEA_SetDTE(g_DTE_Var.dteDisplayValue);
	}
}

#endif

//591 project cruising distence
/**define the fuel sensor state**/
BACKRAM_SEC_VAR_UNSPECIFIED FUEL_SIG_ENUM Fuel_Sensor;

BACKRAM_SEC_VAR_UNSPECIFIED uint32_t r_afv_last;

BACKRAM_SEC_VAR_UNSPECIFIED DTE_DIS_ENUM Dte_state;
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t  s_DtePowerUpCnt;
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t  s_DtePowerUpFlag;

BACKRAM_SEC_VAR_UNSPECIFIED static uint16_t r_afc;
BACKRAM_SEC_VAR_UNSPECIFIED static uint16_t r_afc_last;
BACKRAM_SEC_VAR_UNSPECIFIED static uint32_t dte_tank_vol_buf[16];
BACKRAM_SEC_VAR_UNSPECIFIED static uint32_t TankVol_avg;
BACKRAM_SEC_VAR_UNSPECIFIED static uint32_t s_Dte_Rn;
BACKRAM_SEC_VAR_UNSPECIFIED static uint32_t s_Dte_FOUT;
static uint32_t s_Dte_RnSum = 0;
static uint32_t s_Dte_RnSum_Int = 0;
static uint32_t s_Dte_RnCnt = 0;



F_VOL_STATE f_vol_state = state_one;

uint8_t first_on = 0;
/*uint8_t refuel_state_for_tank_vol = 0;
uint8_t refuel_state_for_afv = 0;*/
uint8_t refuel_state_for_dte_display = 0;


void DTE_FillUpFunc(void)
{
	uint8_t i;
	
	/*refuel_state_for_tank_vol = 1;
	refuel_state_for_afv = 1;*/
	
	refuel_state_for_dte_display = 1;
	s_Dte_Rn = FUEL_GetFuelVol()*FUEL_FACTOR;	//ul
	s_Dte_FOUT = s_Dte_Rn;	//ul
	TankVol_avg = s_Dte_Rn;	//ul
	r_afv_last = s_Dte_Rn;	//ul
	
	for(i = 0; i < 16; i++)
	{
		dte_tank_vol_buf[i] = s_Dte_Rn;	//incial the data_buf[] value when refuel
	}
	
	s_Dte_RnSum = 0;
	s_Dte_RnSum_Int = 0;
	s_Dte_RnCnt = 0;
	f_vol_state = state_one;
}

//uint8_t getITMasterCtrlVerion1 =0u;

void DTE_Init(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		g_DTE_Var.dteValue = 0;
		g_DTE_Var.dteValueDisplay = 0;
		g_DTE_Var.dteValid = 0;
		r_afv_last = 0;
		TankVol_avg = 0;

		f_vol_state = state_one;
		Dte_state = DTE_DIS_INVALID;
		s_DtePowerUpCnt = 0;
		s_DtePowerUpFlag = 1;
		//getITMasterCtrlVerion1 = getITMasterCtrlVerion();
		if((getITMasterCtrlVerion() == 0b1000) || (getITMasterCtrlVerion() == 0b1010))//yangxl 2019��8��16��14:30:06
		{
			r_afc = DTE_Cfg_DefaultCFC_A;
			r_afc_last = DTE_Cfg_DefaultCFC_A;
		}else
		{
			r_afc = DTE_Cfg_DefaultCFC_B;
			r_afc_last = DTE_Cfg_DefaultCFC_B;
		}
		s_Dte_Rn = 0;
		s_Dte_FOUT = 0;
	}
	
	FUEL_RegisterFillUpFunc(DTE_FillUpFunc);

	DEA_SetDTE_Valid(g_DTE_Var.dteValid);
	DEA_SetDTE(g_DTE_Var.dteValueDisplay);
}

//Get the Dte value show state
uint8_t Get_DteState()
{
	return Dte_state;
}

/*************************/
/**	calculate the R_AFV	**/
/*************************/
//30s
#if 1
uint32_t Get_TankVol(void)		//calculate the avrage Tank_Vol 
{
	uint8_t i;
	uint32_t l_Dte_Dn;
	uint32_t TankVolSum = 0;

	l_Dte_Dn = FUEL_GetFuelVolNow()*FUEL_FACTOR;	//ul
	if(s_Dte_Rn > l_Dte_Dn)
	{
		s_Dte_Rn = s_Dte_Rn-(s_Dte_Rn-l_Dte_Dn)/16;
	}
	else if(s_Dte_Rn < l_Dte_Dn)
	{
		s_Dte_Rn = s_Dte_Rn+(l_Dte_Dn-s_Dte_Rn)/16;
	}


	s_Dte_RnSum += s_Dte_Rn;
	if(s_Dte_RnSum >= FUEL_SUM_FACTOR)	//To prevent overflow
	{
		s_Dte_RnSum_Int += 1;
		s_Dte_RnSum -= FUEL_SUM_FACTOR;
	}
	s_Dte_RnCnt++;
	if(s_Dte_RnCnt >= 150)	//200*(5*30) = 30S
	{
		s_Dte_FOUT = s_Dte_RnSum_Int*(FUEL_SUM_FACTOR/150);
		s_Dte_FOUT += (s_Dte_RnSum_Int*666+500)/1000;
		s_Dte_FOUT += (s_Dte_RnSum)/150;
		s_Dte_RnSum_Int = 0;
		s_Dte_RnSum = 0;
		s_Dte_RnCnt = 0;

		for(i = 15; i >= 1; i--)		//get ride of the most old value
		{
			dte_tank_vol_buf[i] = dte_tank_vol_buf[i - 1];
		}
		dte_tank_vol_buf[0] = s_Dte_FOUT;

		for(i = 0;i < 16; i++)
		{
			TankVolSum += dte_tank_vol_buf[i];
		}	
		TankVol_avg = TankVolSum/16;	//uint: ul
	}
	return TankVol_avg;
}
#else
uint32_t Get_TankVol(void)		//calculate the avrage Tank_Vol of 30s 
{
	uint32_t TankVol = 0;
	uint32_t TankVolSum = 0;
	static uint16_t TimeCnt = 0;

	uint8_t i;
	
	if(FUEL_GetFuelSigStatus() != FuelSigNormal)
	{
		refuel_state_for_tank_vol = 1;
		TankVol_avg = 0;
	}
	else
	{
		if(refuel_state_for_tank_vol != 0)
		{
			refuel_state_for_tank_vol = 0;
			TankVol =  FUEL_GetFuelVol()*10000;	//ul
			for(i = 0; i < 16; i++)
			{
				dte_tank_vol_buf[i] = TankVol;	//incial the data_buf[] value when refuel
			}
			TankVol_avg = TankVol;
		}
		else
		{
			TimeCnt++;
			if(TimeCnt >= 300)	//< 30s 100ms cycle
			{	
				TimeCnt = 0;
				TankVol = FUEL_GetFuelVol() * 10000;	//uint: ul
				for(i = 15; i >= 1; i--)		//get ride of the most old value
				{
					dte_tank_vol_buf[i] = dte_tank_vol_buf[i - 1];
				}
				dte_tank_vol_buf[0] = TankVol;

				for(i = 0;i < 16; i++)
				{
					TankVolSum += dte_tank_vol_buf[i];
				}	
				TankVol_avg = (TankVolSum/16);	//uint: ul
			}
		}
	}

	return TankVol_avg;
}
#endif

//200ms
uint32_t Get_Tim_200ms(uint8_t cal_flag)	//calculate the sum Tim_Vol of 200ms 
{
	static uint32_t TiiSum = 0;
	uint32_t Tim_Vol_200ms = 0;

	TiiSum += DEA_GetTII_Value();	//ul

	if(cal_flag == 1)	//<200ms		100ms cycle
	{
		Tim_Vol_200ms = TiiSum;	//uint: ul
		TiiSum = 0;
	}
	return Tim_Vol_200ms;
}

//30s
uint32_t Get_Tim_30s(uint8_t cal_flag)	//calculate the sum Tim_Vol of 30s
{
	static uint32_t TiiSum = 0;
	uint32_t Tim_Vol_30s = 0;

	
	if(cal_flag == 0)	//< 30s	100ms cycle
	{
		TiiSum += DEA_GetTII_Value();	//ul
	}
	else
	{
		Tim_Vol_30s = TiiSum;	//uint: ul
		TiiSum = 0;
	}

	return Tim_Vol_30s;
}

//200ms
uint32_t Get_F_VOL(uint8_t cal_flag)	//get the F_VOL 	uint: ul
{
	static uint32_t f_vol = 0;
	static uint32_t get_tim_200ms = 0;

	uint32_t tank_vol;
	uint32_t l_afv_last;
	

	l_afv_last = r_afv_last;	//the previous remain fuel vol	uint:ul
	
	if(cal_flag == 0)
	{
		Get_Tim_200ms(0);
	}
	else
	{
		tank_vol = Get_TankVol();			//the tank fuel vol	uint:ul
		get_tim_200ms = Get_Tim_200ms(1);
		
		switch(f_vol_state)	//last_state
		{
			case state_one:	//F_VOL = TIM_VOL200ms
				
						f_vol = get_tim_200ms;

						if((l_afv_last <  (tank_vol*9)/10) && (tank_vol <= DTE_FT_VOL) && (tank_vol >= DTE_ET_VOL))
						{
							f_vol_state = state_five;	//switch to new state
						}
						else if((l_afv_last >  (tank_vol*11)/10) && (tank_vol <= DTE_FT_VOL) && (tank_vol >= DTE_ET_VOL))
						{
							f_vol_state = state_two;	//switch to new state
						}
				break;
				
			case state_two:	//F_VOL = 1.5 * TIM_VOL200ms
				
						f_vol = (15 * get_tim_200ms)/10;

						if((l_afv_last > (tank_vol*13)/10) && (tank_vol <= DTE_FT_VOL) && (tank_vol >= DTE_ET_VOL))
						{
							f_vol_state = state_three;
						}
						else if((l_afv_last < (tank_vol/100)*102) || (tank_vol > DTE_FT_VOL) || (tank_vol < DTE_ET_VOL))
						{
							f_vol_state = state_one;
						}
						
				break;
				
			case state_three:	//F_VOL = 2.5 * TIM_VOL200ms

						f_vol = (25 * get_tim_200ms)/10;

						if((l_afv_last <  (tank_vol*11)/10) || (tank_vol > DTE_FT_VOL) || (tank_vol < DTE_ET_VOL))
						{
							f_vol_state = state_two;
						}
				
				break;
				
			case state_four:	//F_VOL = 0.25 * TIM_VOL200ms
				
						f_vol = (25 * get_tim_200ms)/100;
						
						if((l_afv_last > (tank_vol*9)/10) || (tank_vol > DTE_FT_VOL) || (tank_vol < DTE_ET_VOL))
						{
							f_vol_state = state_five;
						}
				
				break;
				
			case state_five:	//F_VOL = 0.5 * TIM_VOL200ms
					
						f_vol = (5 * get_tim_200ms)/10;

						if((l_afv_last < (tank_vol*7)/10) && (tank_vol <= DTE_FT_VOL) && (tank_vol >= DTE_ET_VOL))
						{
							f_vol_state = state_four;
						}
						else if((l_afv_last > (tank_vol/100)*98) || (tank_vol > DTE_FT_VOL) || (tank_vol < DTE_ET_VOL))
						{
							f_vol_state = state_one;
						}
						
				break;
				
			default:
				break;
		}
	}

	return f_vol;
}

//200ms
uint32_t Get_R_AFV(void)		//calculate the remian fuel vol
{
	uint32_t f_vol = 0;
	static uint8_t timecnt = 0;
	static DEA_FUEL_SIG_ENUM s_FuelStatus = FuelSigNormal;
	
	if((s_FuelStatus == FUEL_GetFuelSigStatus()) && (r_afv_last != 0))		//not refuel or sensor status change
	{
		timecnt++;
		if(timecnt < 2)
		{
			Get_F_VOL(0);
		}
		else
		{
			timecnt = 0;
			f_vol = Get_F_VOL(1);
			if(r_afv_last > f_vol)
			{
				r_afv_last = r_afv_last - f_vol;	//update the r_afv
			}
		}
	}
	else	//refuel
	{
		s_FuelStatus = FUEL_GetFuelSigStatus();
		timecnt = 0;
	}

	return r_afv_last;
}

uint32_t Get_R_AFV_Value(void)		//get the remian fuel vol
{
	return r_afv_last;
}
/*************************/
/**	calculate the R_AFC	**/
/*************************/

//30s
uint32_t Get_Econ_Factor(uint8_t cal_flag)
{
	static uint16_t timecnt = 0;
	static uint32_t speedsum = 0;
	uint32_t speed_avg = 0;

	static uint32_t eco_factor = 0;

	if(cal_flag == 0)	//< 30s
	{
		if(DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 0)
		{
			speedsum += DEA_GetRawVehicleSpeed();	//factor 0.1  unit:km/h
			timecnt++;
		}
	}
	else
	{	
		if(timecnt > 0)
		{
			speed_avg = speedsum/timecnt;
		}
		speedsum = 0;
		timecnt = 0;
		//eco_factor = (speed_avg * 30)/(90 * 3600)
		eco_factor = speed_avg*10/108;		//expand 10000 times	(km/h)
	}
	
	return eco_factor;
}



uint16_t Get_Distance(void)
{
	static uint16_t distence = 0U;
	static uint32_t s_OneMeterCntBak = 0U; 
	uint32_t l_OneMeterCnt = 0U;
 
	l_OneMeterCnt = ODO_GetOdoOneMeterCnt();	//100ms
	if(s_OneMeterCntBak == 0)
	{
		s_OneMeterCntBak = l_OneMeterCnt;
		distence = 0;
	}
	else
	{
		if(l_OneMeterCnt > s_OneMeterCntBak)
		{
			distence = (l_OneMeterCnt - s_OneMeterCntBak);
			s_OneMeterCntBak = l_OneMeterCnt;
		}
		else
		{
			distence = 0;
		}
	}

	return distence;
}


//30s
uint32_t Get_Fuel_Econ(uint8_t cal_flag)
{
	static uint32_t econ_dist = 0;
	uint32_t fuel_econ = 0;
	uint32_t tim_30s = 0;
	
	tim_30s = Get_Tim_30s(cal_flag);	//ul
	econ_dist += Get_Distance();		//m
	
	if(cal_flag == 0)	//< 30s
	{
		//econ_dist += Get_Distance();	//m
	}
	else
	{
		if(econ_dist == 0)
		{
			econ_dist = 1;	//Minimum value of ECON_DSIT is 0.0001km
		}
		fuel_econ = (tim_30s*10)/econ_dist;	//factor:0.01 unit:l/100km (expand 100 times)

		econ_dist = 0;
	}

	return fuel_econ;
}

uint32_t Get_R_AFC_Value(void)
{
	return r_afc;
}

//30s
uint32_t Get_R_AFC(void)
{
	const int32_t c_factor = 10000;
	static uint16_t timecnt = 0;
	static int32_t s_reminder = 0;

	int32_t fuel_econ = 0;
	int32_t econ_factor = 0;
	int32_t temp= 0;

	if(timecnt < 300)
	{
		timecnt++;
		Get_Fuel_Econ(0);
		Get_Econ_Factor(0);	
	}
	else
	{
		fuel_econ = Get_Fuel_Econ(1);
		econ_factor = Get_Econ_Factor(1);	//expand c_factor times
		timecnt = 0;

		if(econ_factor > 0)
		{
			temp = (fuel_econ - (int32_t)r_afc_last)*econ_factor;	//expand c_factor times

			//get the integer
			if(temp > 0)
			{
				s_reminder += temp;
				if(s_reminder > c_factor)
				{
					r_afc = r_afc_last + s_reminder/c_factor;
					s_reminder = s_reminder%c_factor;
				}
				else if(s_reminder == c_factor)
				{
					r_afc = r_afc_last + 1;
					s_reminder = 0;
				}
				else
				{	
					//do nothing
				}
			}
			else if(temp < 0)
			{
				s_reminder += temp;
				if(s_reminder < (-c_factor))
				{
					r_afc = r_afc_last + s_reminder/c_factor;
					s_reminder = s_reminder%c_factor;
				}
				else if(s_reminder == (-c_factor))
				{
					r_afc = r_afc_last - 1;
					s_reminder = 0;
				}
				else
				{	
					//do nothing
				}
			}
			else
			{
				r_afc = r_afc_last;
			}

			//Limiter value of fuel economy {L_AFC} is 2.5 ~ 80 [L/100 km]
			if(r_afc >= 8000)
			{
				r_afc = 8000;
			}

			if(r_afc <= 250)
			{
				r_afc = 250;
			}
			r_afc_last = r_afc;
		}
	}

	return r_afc;	//factor 0.01 unit:L/100km expand 100 times
}

uint32_t Get_DteDisplay(uint32_t sample,uint32_t display,uint8_t step)
{
	 if(abs(sample - display) > 3)
	 {	
		 if(sample > display)
		 {
		 	display += step;
		 }
		 else
		 {
		 	display -= step;
		 }
		 return display;
	 }
	 else
	 {
	 	return sample;
	 }
}

void DTE_SetDebugFlag(uint8_t stat)
{
	s_dte_debug_flag = stat;
}

void DTE_Main(void)
{

	static uint16_t displaytime_cnt = 0;

	static uint32_t  get_r_afv = 0;
	static uint32_t  get_r_afc = 0;

	static uint8_t FuelSensorState = 0;
	static FUEL_SIG_ENUM s_FuelStatus = FuelSigNormal;

	DTE_DIS_ENUM l_Dte_state = Dte_state;

	if(DEA_GetIgnStat() == IGN_ON)
	{	
		Fuel_Sensor = FUEL_GetFuelSigStatus();	//get the fuel sensor status

		if(Fuel_Sensor != FuelSigNormal)	//fuel sensor not normal
		{
			if(FuelSensorState != 1)
			{
				FuelSensorState = 1;
				memset(&g_DTE_Var,0,sizeof(g_DTE_Var));
				s_FuelStatus = FUEL_GetFuelSigStatus();
			}
			l_Dte_state = DTE_DIS_INVALID;	//data show "--"

			if(s_DtePowerUpCnt < 30*10)	//During 30sec from first IGN is ON after BAT is ON, 闁炽儻鎷�?----闁炽儻鎷�?is shown on display as DTE
			{
				s_DtePowerUpCnt++;
			}
		}
		else 
		{
			get_r_afv = Get_R_AFV();
			if(get_r_afv > FUEL_FULL_VOL*10*1000)	//ul
			{
				get_r_afv = FUEL_FULL_VOL*10*1000;
			}
			
			get_r_afc = Get_R_AFC();
			FuelSensorState = 0;
			if(s_DtePowerUpCnt < 30*10)	//During 30sec from first IGN is ON after BAT is ON, 闁炽儻鎷�?----闁炽儻鎷�?is shown on display as DTE
			{
				g_DTE_Var.dteValueDisplay = 0;
				s_DtePowerUpCnt++;
			}
			else 
			{
				//if(get_r_afv < 8840000)	//uint: ul
				if(get_r_afv < 7340000)	//uint: ul  //yangxl 2019-11-11 15:45:39
				{
					l_Dte_state = DTE_DIS_SECOND;	//data show "--"
				}
				else
				{
					if(l_Dte_state != DTE_DIS_SECOND)
					{
						if(1 == FUEL_GetFuelLowStatus())	//low fuel
						{
							l_Dte_state = DTE_DIS_PRE;	//data to blinking
						}
						else 
						{
							l_Dte_state = DTE_DIS_NORMAL;	//normal show
						}
					}
					else
					{
						if(0 == FUEL_GetFuelLowStatus())	//low fuel
						{
							l_Dte_state = DTE_DIS_NORMAL;	//normal show
						}
					}
				}

				if(s_DtePowerUpFlag == 1)
				{
					if(get_r_afv > (DTE_DR + DTE_SM))
					{
						g_DTE_Var.dteValue = (get_r_afv - DTE_DR - DTE_SM)/(get_r_afc*100);	//uint: km
						g_DTE_Var.dteValid = 1;
					}
					else
					{
						g_DTE_Var.dteValue = 0;
						g_DTE_Var.dteValid = 0;
					}
					g_DTE_Var.dteValueDisplay = g_DTE_Var.dteValue;
					s_DtePowerUpFlag = 0;
					//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_BC]:DTE:s_DtePowerUpFlag = 1u \r\n");//yangxl test
#ifdef DTE_DEBUG
					IPC_UpdateDTEState(get_r_afc,get_r_afv,g_DTE_Var.dteValueDisplay,g_DTE_Var.dteValue);
#endif
				}
				else
				{
					if(displaytime_cnt < 300 )	//<= Tcal:30s
					{
						displaytime_cnt++;
					}
					else	//> 30s
					{
						//DTE = (R_AFV - DTE_DR - DTE_SM)/R_AFC * 100(km)
						displaytime_cnt = 0;
						if(get_r_afv > (DTE_DR + DTE_SM))
						{
							g_DTE_Var.dteValue = (get_r_afv - DTE_DR - DTE_SM)/(get_r_afc*100);	//uint: km
							g_DTE_Var.dteValid = 1;
							g_DTE_Var.dteValueDisplay = Get_DteDisplay(g_DTE_Var.dteValue, g_DTE_Var.dteValueDisplay, STEPUNIT);
						}
						else
						{
							g_DTE_Var.dteValue = 0;
							g_DTE_Var.dteValid = 0;
							g_DTE_Var.dteValueDisplay = 0;
						}
						//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_BC]:DTE:display30timeflg = 1u \r\n");//yangxl test
#ifdef DTE_DEBUG
						IPC_UpdateDTEState(get_r_afc,get_r_afv,g_DTE_Var.dteValueDisplay,g_DTE_Var.dteValue);
#endif
					}

					if((1 == refuel_state_for_dte_display) || (s_FuelStatus != FUEL_GetFuelSigStatus())) //refuel
					{
						//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_BC]:DTE:refuelflg = %d \r\n",refuel_state_for_dte_display);//yangxl test
						//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_BC]:DTE:s_FuelStatus = %d FuelStatus = %d \r\n",s_FuelStatus,FUEL_GetFuelSigStatus());//yangxl test
						//TracePrintf(COM_TRACE, TRACE_NONMASK, "[Task_BC]:DTE:speed = %d \r\n",DEA_GetFilterVehicleSpeed()/10);//yangxl test
#ifdef DTE_DEBUG
						IPC_UpdateDTEState(get_r_afc,get_r_afv,g_DTE_Var.dteValueDisplay,g_DTE_Var.dteValue);
#endif
						refuel_state_for_dte_display = 0;
						s_FuelStatus = FUEL_GetFuelSigStatus();
						if(get_r_afv > (DTE_DR + DTE_SM))
						{
							g_DTE_Var.dteValue = (get_r_afv - DTE_DR - DTE_SM)/(get_r_afc*100);	//uint: km
							g_DTE_Var.dteValid = 1;
							g_DTE_Var.dteValueDisplay = g_DTE_Var.dteValue;
						}
						else
						{
							g_DTE_Var.dteValue = 0;
							g_DTE_Var.dteValid = 0;
							g_DTE_Var.dteValueDisplay = 0;
						}
					}
				}

				if(g_DTE_Var.dteValueDisplay >= 50)
				{
					DEA_SetDTE(g_DTE_Var.dteValueDisplay);
				}
				else
				{
					g_DTE_Var.dteValueDisplay = 0;
					DEA_SetDTE(g_DTE_Var.dteValueDisplay);
					l_Dte_state = DTE_DIS_SECOND;
				}
			}
		}
		
		DEA_SetDTE_Valid(g_DTE_Var.dteValid);
		Dte_state = l_Dte_state;
	}
	else
	{
		displaytime_cnt = 0;
		FuelSensorState = 0;
		/*refuel_state_for_tank_vol = 0;
		refuel_state_for_afv = 0;*/
		refuel_state_for_dte_display = 0;
#ifdef DTE_DEBUG
		DTE_SetDebugFlag(0);
#endif
	}

#ifdef DTE_DEBUG
	if(s_dte_debug_flag)
	{
		if((get_r_afc != get_r_afc_last) || (get_r_afv/10000 != get_r_afv_last) || (dte_last != g_DTE_Var.dteValue))
		{
			get_r_afc_last = get_r_afc;
			get_r_afv_last = get_r_afv/10000;
			dte_last = g_DTE_Var.dteValue;
			TracePrintf(0, TRACE_NONMASK, "[Task_BC]:afv:%d afc:%d dte:%d\r\n", get_r_afv, get_r_afc, g_DTE_Var.dteValue);
		}
	}
#endif
	
}

