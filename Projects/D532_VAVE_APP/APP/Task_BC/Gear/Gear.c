#define   IOC_PLATFORM

//#include "Task_CanApp_Cfg.h"
#if (STD_ON == TASK_CANAPP_DEBUG_TRACE)
#include "trace.h"
#endif

#include "dea.h"
//#include "Task_IpcApp_Cfg.h"
//#include "CanApp_PduChanged.h"

#include "Gear.h"
//#include "gsc0x22.h"
#include "NvM.h"
#include "string.h"
#include "MemMap.h"
#include "NvM_if.h"



uint8_t GearData = 0;		/** gear value**/
uint8_t GearDataValid = 0; /**gear value valid**/

Grar_SHOW2 Grar_ATSHOW2 = Show_OFF2;
Grar_MTSHOW Grar_ATSHOW3 = Show_OFF;


/**the frame 0x421 recived judge flag**/
NVM_SEC_VAR_UNSPECIFIED uint8_t SignalRecived = 0;
NVM_SEC_VAR_UNSPECIFIED uint8_t FirstMode = 0;


Gear_Infor Gear_RawInfor;

GEAR_MODE  Gear_Mode = METER_OFF;
Grar_MTSHOW  Gear_MTMode = Show_OFF;


Grar_SHOW2 Get_GearAtShow2()
{
	return Grar_ATSHOW2;
}

Grar_MTSHOW Get_GearAtShow3()
{
	return Grar_ATSHOW3;
}

GEAR_MODE Get_GearMode()
{
	return Gear_Mode;
}

Grar_MTSHOW Get_GearMTMode()
{
	return Gear_MTMode;
}



#if 0
/**传统车-自动档/手动档**/
extern void GearModeSet(uint8 inData);
extern void GearShiftModeSet(uint8 inData);
extern void GearGearSet(uint8 inData);
extern void GearEngineMode(uint8 inData);

static GEAR_Nvm GearNvmData;

uint8 GearWorkMode = GEAR_RUN_OFF; 
uint8 GearType = GEAR_DCT;

void SetGearType(uint8 Type)
{
	GearNvmData.Type = (GEAR_TYPE)Type;
}

GEAR_TYPE GetGearType(void)
{
	GearNvmData.Type = GEAR_DCT;
	return ((GEAR_TYPE)GearNvmData.Type);
}

void SetGearShiftMode(uint8 Mode)
{
	GearNvmData.Type = (GEAR_SHIFT_MODE)Mode;
}

GEAR_SHIFT_MODE GetGearShiftMode(void)
{
	GearNvmData.ShiftMode = SHIFT_NONE;
	return ((GEAR_SHIFT_MODE)GearNvmData.ShiftMode);
}

//////////////////////////////////////////////////////
static void Gear_DctProc(void) 
{
	uint8_t l_u8GearValue = 0;
	uint8_t l_u8GearEngineMode = 0;
	
	if(DEA_GetTransType() == 0x01)
	{
		GearModeSet(0x2);	//AT manual mode
	}
	else
	{
		GearModeSet(0x1);	//AT mode
	}
	
	if(DEA_GetTransGearValid())
	{
		l_u8GearValue = DEA_GetTransGearIO();
		if(l_u8GearValue <= 6)
		{
			GearGearSet(l_u8GearValue);
		}
		else if(l_u8GearValue == 0x0D)
		{
			GearGearSet(IPC_GEAR_N);	//N gear
		}
		else if(l_u8GearValue == 0x0E)
		{
			GearGearSet(IPC_GEAR_R);	//R gear
		}
		else
		{
			GearGearSet(0x00);
		}
	}
	else
	{
		GearGearSet(0x00);
	}

	if(DEA_GetTransCntrlValid())
	{
		l_u8GearEngineMode = DEA_GetTransCntrl();
		if(l_u8GearEngineMode <= 2)
		{
			GearEngineMode(l_u8GearEngineMode);
		}
		else
		{
			GearEngineMode(0x00);
		}
	}
	else
	{
		GearEngineMode(0x00);
	}
}

static void Gear_MtProc(void) 
{
	uint8_t l_u8EngEstGear = 0;
	uint8_t l_u8Value = 0;
	uint8_t l_GearUpFlag = 0;

	GearModeSet(0x0);	//MT mode
	Com_RxSig_GetEngRecmndUpshftIOv(&l_u8Value);
	if(!l_u8Value)
	{
		Com_RxSig_GetEngRecmndUpshftIO(&l_u8Value);
		if(l_u8Value)
		{
			l_GearUpFlag = 1;
		}
	}

	if(l_GearUpFlag)
	{
		GearShiftModeSet(1);	//Gear up
	}
	else
	{
		Com_RxSig_GetEngRecmndDwnshftIOv(&l_u8Value);
		if(!l_u8Value)
		{
			Com_RxSig_GetEngRecmndDwnshftIO(&l_u8Value);
			if(l_u8Value)
			{
				GearShiftModeSet(2);	//Gear Down
			}
			else
			{
				GearShiftModeSet(0);
			}
		}
	}

	Com_RxSig_GetEngEstGear(&l_u8EngEstGear);
	if(l_u8EngEstGear <= 6)
	{
		GearGearSet(l_u8EngEstGear);
	}
	else
	{
		GearGearSet(0x00);
	}
}


void GEAR_GearProc(void)
{
	if(DEA_GetIgnStat() == IGN_ON)
	{
		switch(GearWorkMode)
		{
			case GEAR_RUN_OFF:
				GearGearSet(0x00);
				GearModeSet(0x00);
				GearShiftModeSet(0x00);
				GearEngineMode(0x00);
				GearWorkMode = GEAR_CHECK_MODE;
				break;

			case GEAR_CHECK_MODE:
				if(GetGearType() == GEAR_DCT)
				{
					GearType = GEAR_DCT;
				}
				else
				{
					GearType = GEAR_MT;
				}
				GearWorkMode = GERA_RUN_ON;
				break;

			case GERA_RUN_ON:
				if(GearType == GEAR_DCT)
				{
					Gear_DctProc();
				}
				else
				{
					Gear_MtProc();
				}
				break;

			default:
				GearWorkMode = GEAR_RUN_OFF;
				break;
		}
	}
}
#endif


void Gear_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		GearDataValid = 0;
		GearData = 0;

		Gear_MTMode = Show_OFF;
		Gear_Mode = METER_OFF;

		Grar_ATSHOW2 = Show_OFF2;
		Grar_ATSHOW3 = Show_OFF;
		
		DEA_SetFilterGearValueValid(GearDataValid);
		DEA_SetFilterGearValue(GearData);
	}
}



uint8_t GetGearValue()
{
	if(DEA_GetGearErrorValueStat() == 0)
	{

		Gear_RawInfor.u8GearValue = DEA_GetRawGearValue();
	}
	else
	{
		Gear_RawInfor.u8GearValue = 0x0F;	//invalid
	}

	return Gear_RawInfor.u8GearValue;
}


uint8_t GetGearTm_Type()
{
	if(0 == DEA_GetRawGearATCheakValue())
	{
		Gear_RawInfor.u8GearTm_Type = 0;
	}
	else if(1 == DEA_GetRawGearATCheakValue())
	{
		Gear_RawInfor.u8GearTm_Type = 1;
	}

	return Gear_RawInfor.u8GearTm_Type;
}


uint8_t GetGearBlink()
{
	if(1 == DEA_GetRawGearBlinkValue())
	{
		Gear_RawInfor.u8GearBlinkValue = 1;
	}
	else
	{
		Gear_RawInfor.u8GearBlinkValue = 0;
	}
	
	return Gear_RawInfor.u8GearBlinkValue;
}


#if 0
uint8_t GtGearIndication()
{
	if(1 == DEA_GetRawGearIdicateAllow())
	{
		Gear_RawInfor.u8GearIndicateAllow = 1;
	}
	else
	{
		Gear_RawInfor.u8GearIndicateAllow = 0;
	}

	return Gear_RawInfor.u8GearIndicateAllow;
}
#endif


void Gear_MAIN()
{
	//static uint8_t GearLast = 0;
	//static uint8_t AT_OFFcnt = 0;
	
	int8_t temp_data = 0;
	
	uint8_t temp_gearvalue = 0;
	uint8_t temp_gearblink = 0;
	uint8_t temp_gearindicate = 0;
	//uint8_t temp_gearTmType = 0;

	int8_t temp_MT = 0;
	uint16_t Rspeed = 0;
	uint16_t Vspeed = 0;
	uint16_t Apost = 0;
	if(DEA_GetIgnStat() == IGN_ON)
	{
		
		if(FirstMode != 1)
		{
			SignalRecived = DEA_GetGearDataRecive();
		}
		
		//AT
		temp_gearvalue = GetGearValue();	//get the CAN data value
		temp_gearblink = GetGearBlink();
	    //temp_gearindicate= GtGearIndication();
		//temp_gearTmType = GetGearTm_Type();	//get the cheak gearAT

		//MT
		Rspeed = DEA_GetFilterEngineRevolution();
		Vspeed = DEA_GetFilterVehicleSpeed()/10;
		Apost = (DEA_GetGearRawSensorValue() * 125/1000) * 80;
		/**	temp_MT = ......**/							//caculate the MT gear

		if(SignalRecived == 1)	// recive CAN DATA ID : 0X421
		{
			FirstMode = 1;
			if(DEA_GetCanTimeOutStat(IPDU_HND_ATCVT_to_MMI_RN1) == 0)	//not timeout
			{
				//if(0 == temp_gearTmType)	// Tm_Type = 0;
				{

					if((0 != temp_gearvalue) &&(temp_gearvalue <= 23)) 	//valid range
					{
						temp_data = temp_gearvalue;
						GearDataValid = 1;
						Gear_Mode = METER_ON;	//data number section on
					}
					else
					{
						temp_data = 0x0F;	//invalid value
						GearDataValid = 0;
						Gear_Mode = METER_OFF;	//data number section off
					}

					if(METER_OFF != Gear_Mode)
					{
						if(1 == temp_gearblink)	//require blink
						{
							Gear_Mode = METER_BLINK;	//blink
						}
						else
						{
							Gear_Mode = METER_ON;	//on
						}
					}

					switch(temp_data)
					{
						case 1:
								GearData = 0x09;		//P
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
							
						case 2:
								GearData = 0x0A;		//R		
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
							
						case 3:
								GearData = 0x0B;		//N
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
							
						case 4:
								GearData = 0x0C;		//D			
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
							
						case 5:
								GearData = 0x0E;		//S
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
							
						case 6:
								GearData = 0x0D;		//L							
								//GearLast = GearData;	
								Grar_ATSHOW2 = Show_OFF2;
							break;
								
						case 8:
								GearData = 0x01;		// 1
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
									
						case 9:
								GearData = 0x02;		// 2	
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
									
						case 10:
								GearData = 0x03;		// 3
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
								break;
									
						case 11:
								GearData = 0x04;		// 4	
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
									
						case 12:
								GearData = 0x05;		// 5
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_OFF2;
							break;
									
						case 13:
								GearData = 0x06;		// 6					
								//GearLast = GearData;	
								Grar_ATSHOW2 = Show_OFF2;
								break;
								
						case 16:
								GearData = 0x01;		// 1
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
							break;
											
						case 17:
								GearData = 0x02;		// 2	
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
							break;
											
						case 18:
								GearData = 0x03;		// 3
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
								break;
											
						case 19:
								GearData = 0x04;		// 4	
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
							break;
											
						case 20:
								GearData = 0x05;		// 5
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
							break;
											
						case 21:
								GearData = 0x06;		// 6					
								//GearLast = GearData;	
								Grar_ATSHOW2 = Show_M;
								break;
								
						case 22:
								GearData = 0x07;		// 7
								//GearLast = GearData;
								Grar_ATSHOW2 = Show_M;
							break;
													
						case 23:
								GearData = 0x08;		// 8				
								//GearLast = GearData;		
								Grar_ATSHOW2 = Show_M;
								break;
						default:
								GearData = 0x0F;	//invalid		
								Grar_ATSHOW2 = Show_OFF;
							break;
					}
					
#if 0
					if((1 == temp_data) || 
						(2 == temp_data) || 
						(3 == temp_data) || 
						(4 == temp_data) || 
						(6 == temp_data))
					{
						if(temp_MT == 0)
						{
							Grar_ATSHOW3 = Show_OFF;	//off
						}
						else if(temp_MT > 0)
						{
							Grar_ATSHOW3 = Show_UP;	//up
						}
						else if(temp_MT < 0)
						{
							Grar_ATSHOW3 = Show_DOWN;	//down
						}
					}
					else if(5 == temp_data)
					{
						Grar_ATSHOW3 = Show_S;	//s
					}
					else
					{
						Grar_ATSHOW3 = Show_OFF;	//off
					}
#else
				#if 0 //yangxl 2019-7-24 18:59:37
					if(5 == temp_data)
					{
						Grar_ATSHOW3 = Show_S;	//s
					}
					else 
					{
						if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0)
						{
							if(DEA_GetSHIFT_IND() == 0b01)
							{
								Grar_ATSHOW3 = Show_UP;	//up
							}
							else if(DEA_GetSHIFT_IND() == 0b10)
							{
								Grar_ATSHOW3 = Show_DOWN;	//down
							}
							else
							{
								Grar_ATSHOW3 = Show_OFF;	//off
							}
						}
						else
						{
							Grar_ATSHOW3 = Show_OFF;	//off
						}
					}
				#else
					Grar_ATSHOW3 = Show_OFF;	//off
				#endif
#endif
				}
				/*
				else
				{
					Gear_Mode = METER_OFF;
					Grar_ATSHOW2 = Show_OFF2;
					Grar_ATSHOW3 = Show_OFF;
				}
				*/
			}
			else	//timeout
			{
				#if 0
				if(AT_OFFcnt < 200)	//< 2s	timeout for 2s
				{
					AT_OFFcnt++;
					GearData = GearLast;
				}
				else
				{
					AT_OFFcnt = 0;
					Gear_Mode = ATMETER_OFF;
				}
				#endif
				Gear_Mode = METER_OFF;
				Grar_ATSHOW2 = Show_OFF2;
				Grar_ATSHOW3 = Show_OFF;
			}
		}
		else					// MT GEAR (not recive CAN DATA ID : 0X421)
		{	
		#if 0 //yangxl 2019-7-24 18:59:37
			if((DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0) && 
				(DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame) == 0))
			{
				if(0 == temp_MT)
				{
					Gear_MTMode = Show_OFF;
				}
				else
				{	if(temp_MT > 0)
					{
						if((2 == temp_MT) || 
							(3 == temp_MT) || 
							(4 == temp_MT) || 
							(5 == temp_MT) || 
							(6 == temp_MT))
						{
							temp_data = temp_MT;	
							GearDataValid = 1;

							Gear_MTMode = Show_UP;
						}
						else
						{
							temp_data = 0x0F;	//invalid value
							GearDataValid = 0;
						}
					}
					else
					{
						if((-2 == temp_MT) || 
							(-3 == temp_MT) || 
							(-4 == temp_MT) || 
							(-5 == temp_MT))
						{
							temp_data = temp_MT;	
							GearDataValid = 1;

							Gear_MTMode = Show_DOWN;
						}
						else
						{
							temp_data = 0x0F;	//invalid value
							GearDataValid = 0;
						}
					}
					
					switch(temp_data)
					{
						case 2:
								GearData = 0x02;	//Gear 2
							break;
							
						case 3:
								GearData = 0x03;	//Gear 3
							break;
							
						case 4:
								GearData = 0x04;	//Gear 4
							break;
							
						case 5:
								GearData = 0x05;	//Gear 5
							break;
							
						case 6:
								GearData = 0x06;	//Gear 6
							break;
							
						case -2:
								GearData = 0x02;	//Gear 2
							break;
							
						case -3:
								GearData = 0x03;	//Gear 3
							break;
							
						case -4:
								GearData = 0x04;	//Gear 4
							break;
							
						case -5:
								GearData = 0x05;	//Gear 5
							break;
			
						default:
								GearData = 0x0F;	//invalid
							break;

					}
		
				}
			}
			else
			{
				Gear_MTMode = Show_OFF;
			}
		
		#else
			if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN1) == 0)
			{
				if(DEA_GetSHIFT_IND() == 0b01)
				{
					Grar_ATSHOW3 = Show_UP; //up
				}
				else if(DEA_GetSHIFT_IND() == 0b10)
				{
					Grar_ATSHOW3 = Show_DOWN;	//down
				}
				else
				{
					Grar_ATSHOW3 = Show_OFF;	//off
				}
				GearDataValid = 1;
			}
			else
			{
				Grar_ATSHOW3 = Show_OFF;	//off
			}
			if(DEA_GetCanTimeOutStat(IPDU_HND_ECM_to_ITM) == 0u)//yangxl 2019-8-21 09:31:09
			{
				if((DEA_GetGSINXTGP() > 0b000) && (DEA_GetGSINXTGP() < 0b111))
				{
					GearData = DEA_GetGSINXTGP();
					Gear_Mode = METER_ON;
				}else
				{
					Gear_Mode = METER_OFF;
				}
			}else
			{
				GearData = 0x00;
				Gear_Mode = METER_OFF;
			}
			Grar_ATSHOW2 = Show_OFF;
			
		#endif
		}
		DEA_SetFilterGearValueValid(GearDataValid);
		DEA_SetFilterGearValue(GearData);

	}
	else
	{
		
		if(GearData != 0x00)
		{
			GearData = 0x00;
		}

		Gear_MTMode = Show_OFF;
		Gear_Mode = METER_OFF;

		Grar_ATSHOW2 = Show_OFF2;
		Grar_ATSHOW3 = Show_OFF;
		
		GearDataValid = 0;

		DEA_SetFilterGearValueValid(GearDataValid);
		DEA_SetFilterGearValue(GearData);

	}
}



