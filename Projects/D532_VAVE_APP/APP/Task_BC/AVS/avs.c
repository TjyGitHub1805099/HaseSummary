//#include "Tt.h"
#include "avs_cfg.h"
#include "odo.h"
//#include "BackRam.h"
#include "dea.h"
#include "MemMap.h"


//place it to non-volatile ram
//#pragma segment BACK_RAM_START
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_RefTimeA = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_RefTimeB = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 TravelTimeA = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 TravelTimeB = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint16_t AVS_TimecntA = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint16_t AVS_TimecntB = 0;




/**SKON_s106 project**/
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t AVS_TripTimeA = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32_t AVS_TripTimeB = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t AVS_RefTimeAValid = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t AVS_RefTimeBValid = 0;
/**/
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_MillageA = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_MillageB = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_MillageA_Offset = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_MillageB_Offset = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_TripA = 0; 
BACKRAM_SEC_VAR_UNSPECIFIED uint32 AVS_TripB = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t AVS_TripAValid = 0;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t AVS_TripBValid = 0;
//#pragma segment BACK_RAM_END


//#pragma segment DATA=RAM
static boolean AVS_Initialized = FALSE;
//static uint32 AVS_Base_Time;
//static uint32 AVS_Now_Time;
uint8_t AVS_VcuReadyFlag = 0;


//extern uint32_t GetTravelTime(void);

static void AVS_MillageProc(uint32_t *pAVS_Millage, uint32_t *pAVS_MillageOffset, uint8_t flag);

/**SKON_s106 project Vcu_Ready signal**/

uint8_t  AVS_VcuReadyJudge()
{
	Vcu_InputInfor AVS_VcuInfor;
	
	if(DEA_GetRawVcuReadyErrStat() == 0)
	{
		AVS_VcuInfor.u8VcuValid = AVS_True; //avlid
	}
	else
	{
		AVS_VcuInfor.u8VcuValid = AVS_False; //invalid
	}
/*
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_vcuCha_info) != 0)
	{
		AVS_VcuInfor.u8VcuTimeoutFlag = AVS_True; //timeout
	}
	else
	{
		AVS_VcuInfor.u8VcuTimeoutFlag = AVS_False; //not timeout
	}
*/
	AVS_VcuInfor.u8VcuAlive = AVS_True; //Alive is normal

	AVS_VcuInfor.u8VcuValue = DEA_GetRawVcuReadyValue();

	if((AVS_VcuInfor.u8VcuValid == AVS_True) && 
		(AVS_VcuInfor.u8VcuTimeoutFlag == AVS_False) && 
		(AVS_VcuInfor.u8VcuAlive == AVS_True) &&
		(AVS_VcuInfor.u8VcuValue == AVS_True))	
	{
		 AVS_VcuReadyFlag = AVS_True;
	}
	else
	{
		AVS_VcuReadyFlag = AVS_False;
	}

	return AVS_VcuReadyFlag;
	
}




/********************************/

void AVS_ResetTripAAVS()
{
	if(AVS_Initialized == TRUE)
	{
		AVS_TripA = 0;
		AVS_RefTimeA = 0;
		AVS_MillageA = 0;
		TravelTimeA =0uL;
		AVS_TimecntA = 0;
		AVS_TripAValid = 0;
		AVS_MillageProc(NULL, &AVS_MillageA_Offset, 0);
	}
}


void AVS_ResetTripBAVS()
{
	if(AVS_Initialized == TRUE)
	{
		AVS_TripB = 0; 
		AVS_RefTimeB = 0; 
		AVS_MillageB = 0;
		TravelTimeB =0uL;
		AVS_TimecntB = 0;
		AVS_TripBValid = 0;
		AVS_MillageProc(NULL, &AVS_MillageB_Offset, 0);
	}
}

static void AVS_UpdateAVSRefTime(void)
{
	AVS_RefTimeA++;
	AVS_RefTimeB++;
	TravelTimeA++;
	TravelTimeB++;
}

static void AVS_MillageProc(uint32_t *pAVS_Millage, uint32_t *pAVS_MillageOffset, uint8_t flag)
{
	if(flag == 0)
	{
		*pAVS_MillageOffset = ODO_GetOdoOneMeterCnt();
	}
	else
	{
		if(ODO_GetOdoOneMeterCnt() > *pAVS_MillageOffset)
		{
			*pAVS_Millage += (ODO_GetOdoOneMeterCnt() - *pAVS_MillageOffset);
			*pAVS_MillageOffset = ODO_GetOdoOneMeterCnt();
		}
	}
}


void AVS_vInit(void)
{
	if(AVS_Initialized == FALSE)
	{
		AVS_Initialized = TRUE;
		if(DEA_GetSysRstType() == RST_IRRST)//AVS_ReadAVSFromNVRAM(&AVS_TripA, &AVS_TripB) != 0)
		{
			AVS_RefTimeA = 0;
			AVS_RefTimeB = 0;
			AVS_MillageA = 0;
			AVS_MillageB = 0;
			AVS_MillageA_Offset = 0;
			AVS_MillageB_Offset = 0;
			AVS_TripA = 0; 
			AVS_TripB = 0;

			TravelTimeA = 0UL;
			TravelTimeB = 0UL;
			AVS_TimecntA = 0u;
			AVS_TimecntB = 0u;

			AVS_TripAValid = 0u;
			AVS_TripBValid = 0u;

			AVS_MillageProc(NULL, &AVS_MillageA_Offset, 0);
			AVS_MillageProc(NULL, &AVS_MillageB_Offset, 0);
		}

		DEA_SetAVSA(AVS_TripA);
		DEA_SetAVSB(AVS_TripB);	
	}
}


//Depend the TT module, and ODO module
void AVS_vMain(void)
{
	if(AVS_Initialized == TRUE)
	{
		if(DEA_GetIgnStat() == IGN_ON)
		{
			if(ODO_IsMillageSingalValid() && (DEA_GetRawVehicleSpeedErrorStat() == 0))
			{

#if 0
				if(( DEA_GetFilterEngineRevolution()) && (DEA_GetRawVehicleSpeed() > AVS_CAR_MOVING_THRESHOLD))	/**EngineRevolution !=0,VehicleSpeed > threshold**/
				{
					AVS_MillageProc(&AVS_MillageA, &AVS_MillageA_Offset, 1);
					AVS_MillageProc(&AVS_MillageB, &AVS_MillageB_Offset, 1);

					AVS_UpdateAVSRefTime(); //record the time cycle:100ms
					counter++;	  			//591 project require 30s change the display value
					
					if(counter >= 300)	//30s have already come (591 project require)
					{	
						counter = 0;	//per 30s update
						if(AVS_MillageA <= AVS_MAX_S)	//over 10w km not update again (591 project require)
						{
        					/**AVS_A**/
        					if((AVS_MillageA >= AVS_MAXDIS) ||(AVS_RefTimeA >= AVS_MAXDIS)) //overflow
        					{
        						AVS_MillageA = AVS_MillageA/2;
        						AVS_RefTimeA = AVS_RefTimeA/2;
        					}
        					
        					if(AVS_MillageA > AVS_DISPLAY_MILLAGE_THRESHOLD)	//distance passed over 500 meter
        					{	
        						if(AVS_RefTimeA != 0)
        						{
        							AVS_TripA = (AVS_MillageA*(36*10)+(AVS_RefTimeA*5)) / (AVS_RefTimeA*10); //just get the intege part
        							AVS_TripAValid = 1;
        							if(AVS_TripA > AVS_MAX_VALUE)
        							{
        								AVS_TripA = AVS_MAX_VALUE;
        							}
        
        							AVS_TripTimeA = AVS_RefTimeA/600; //分钟		((x*100ms)/1000*60)(min)
        							AVS_RefTimeAValid = 1;
        						}
        					}
        					else
        					{
        						AVS_TripAValid = 0;
        						AVS_TripA = 0;
        
        						AVS_TripTimeA = 0;
        						AVS_RefTimeAValid = 0;
        						
        					}
        
        					/**AVS_B**/
        					if((AVS_MillageB >= AVS_MAXDIS) ||(AVS_RefTimeB >= AVS_MAXDIS)) //overflow
        					{
        						AVS_MillageB = AVS_MillageB/2;
        						AVS_RefTimeB = AVS_RefTimeB/2;
        					}
        
        					if(AVS_MillageB > AVS_DISPLAY_MILLAGE_THRESHOLD)
        					{	
        						if(AVS_RefTimeB != 0)
        						{
        							AVS_TripB = (AVS_MillageB*(36*10)+(AVS_RefTimeB*5)) / (AVS_RefTimeB*10); //just get the intege part
        							AVS_TripBValid = 1;
        							if(AVS_TripB > AVS_MAX_VALUE)
        							{
        								AVS_TripB = AVS_MAX_VALUE;
        							}
        
        							AVS_TripTimeB = AVS_RefTimeB/600; //分钟	((x*100ms)/1000*60)(min)
        							AVS_RefTimeBValid = 1;
        						}
        					}
        					else
        					{
        						AVS_TripBValid = 0;
        						AVS_TripB = 0;
        
        						AVS_TripTimeB = 0;
        						AVS_RefTimeBValid = 0;
        					}
						}
					}
				}
				else
				{
					AVS_MillageProc(NULL, &AVS_MillageA_Offset, 0);
					AVS_MillageProc(NULL, &AVS_MillageB_Offset, 0);
				}
				DEA_SetAVSA_Valid(AVS_TripAValid);
				DEA_SetAVSA(AVS_TripA);		//AVS_tripA
				DEA_SetAVSB_Valid(AVS_TripBValid);
				DEA_SetAVSB(AVS_TripB);		//AVS_tripB

				
				DEA_SetTripTimeA(AVS_TripTimeA);	//AVS_TripTimeA
				DEA_SetTripTimeAValid(AVS_RefTimeAValid);

				DEA_SetTripTimeB(AVS_TripTimeB);	//AVS_TripTimeB
				DEA_SetTripTimeBValid(AVS_RefTimeBValid);
#else
				if(0 == DEA_GetCanTimeOutStat(IPDU_HND_Front_Wheel_Speed_Frame))
				{
					AVS_MillageProc(&AVS_MillageA, &AVS_MillageA_Offset, 1);
					AVS_MillageProc(&AVS_MillageB, &AVS_MillageB_Offset, 1);
					AVS_UpdateAVSRefTime(); //record the time cycle:100ms
				}else
				{
					AVS_MillageProc(NULL, &AVS_MillageA_Offset, 0);
					AVS_MillageProc(NULL, &AVS_MillageB_Offset, 0);
					AVS_TimecntA = 0;	//per 30s update
					AVS_TimecntB = 0;	//per 30s update
				}
				
				
			#if 0	
				AVS_TimecntA++;
				if(AVS_TimecntA >= 300u)	//30s have already come (591 project require)
				{	
					c
					if((AVS_MillageA <= AVS_MAX_S)&&(AVS_RefTimeA<=AVS_MAX_T))
					{
						if(AVS_MillageA > AVS_DISPLAY_MILLAGE_THRESHOLD)	//distance passed over 500 meter
    					{	
    						if(AVS_RefTimeA != 0)
    						{
    							//AVS_TripA = (AVS_MillageA*(36*10)+(AVS_RefTimeA*5)) / (AVS_RefTimeA*10); //just get the intege part
    							AVS_TripA = AVS_MillageA*36/AVS_RefTimeA; 
    							AVS_TripAValid = 1;
    							if(AVS_TripA > AVS_MAX_VALUE)
    							{
    								AVS_TripA = AVS_MAX_VALUE;
    							}
    
    							AVS_TripTimeA = AVS_RefTimeA/600; //分钟		((x*100ms)/1000*60)(min)
    							AVS_RefTimeAValid = 1;
    						}
    					}
    					else
    					{
    						AVS_TripAValid = 0;
    						AVS_TripA = 0;
    
    						AVS_TripTimeA = 0;
    						AVS_RefTimeAValid = 0;
    						
    					}
					}

				}

				AVS_TimecntB++;
				if(AVS_TimecntB >= 300u)	//30s have already come (591 project require)
				{	
					AVS_TimecntB = 0;	//per 30s update
					if((AVS_MillageB <= AVS_MAX_S)&&(AVS_RefTimeB<=AVS_MAX_T))
					{
						if(AVS_MillageB > AVS_DISPLAY_MILLAGE_THRESHOLD)
    					{	
    						if(AVS_RefTimeB != 0)
    						{
    							//AVS_TripB = (AVS_MillageB*(36*10)+(AVS_RefTimeB*5)) / (AVS_RefTimeB*10); //just get the intege part
    							AVS_TripB = AVS_MillageB*36/AVS_RefTimeB; 
    							AVS_TripBValid = 1;
    							if(AVS_TripB > AVS_MAX_VALUE)
    							{
    								AVS_TripB = AVS_MAX_VALUE;
    							}
    
    							AVS_TripTimeB = AVS_RefTimeB/600; //分钟	((x*100ms)/1000*60)(min)
    							AVS_RefTimeBValid = 1;
    						}
    					}
    					else
    					{
    						AVS_TripBValid = 0;
    						AVS_TripB = 0;
    
    						AVS_TripTimeB = 0;
    						AVS_RefTimeBValid = 0;
    					}
					}

				}
			#else
				AVS_TimecntA++;
				if((AVS_TimecntA >= 300u) && (AVS_MillageA > AVS_DISPLAY_MILLAGE_THRESHOLD))	//30s have already come //distance passed over 500 meter
				{
					AVS_TimecntA = 0;	//per 30s update
					if((AVS_MillageA <= AVS_MAX_S)&&(AVS_RefTimeA<=AVS_MAX_T))
					{
						if(AVS_RefTimeA != 0)
						{
							//AVS_TripA = (AVS_MillageA*(36*10)+(AVS_RefTimeA*5)) / (AVS_RefTimeA*10); //just get the intege part
							AVS_TripA = AVS_MillageA*36/AVS_RefTimeA; 
							AVS_TripAValid = 1;
							if(AVS_TripA > AVS_MAX_VALUE)
							{
								AVS_TripA = AVS_MAX_VALUE;
							}

							AVS_TripTimeA = AVS_RefTimeA/600; //分钟		((x*100ms)/1000*60)(min)
							AVS_RefTimeAValid = 1;
						}
					}
				}else
				{
					;
					//AVS_TripAValid = 0;
					//AVS_TripA = 0;
					//AVS_TripTimeA = 0;
					//AVS_RefTimeAValid = 0;
				}

				AVS_TimecntB++;
				if((AVS_TimecntB >= 300u) && (AVS_MillageB > AVS_DISPLAY_MILLAGE_THRESHOLD))
				{
					AVS_TimecntB = 0;	//per 30s update
					if((AVS_MillageB <= AVS_MAX_S)&&(AVS_RefTimeB<=AVS_MAX_T))
					{
						if(AVS_RefTimeB != 0)
						{
							//AVS_TripB = (AVS_MillageB*(36*10)+(AVS_RefTimeB*5)) / (AVS_RefTimeB*10); //just get the intege part
							AVS_TripB = AVS_MillageB*36/AVS_RefTimeB; 
							AVS_TripBValid = 1;
							if(AVS_TripB > AVS_MAX_VALUE)
							{
								AVS_TripB = AVS_MAX_VALUE;
							}

							AVS_TripTimeB = AVS_RefTimeB/600; //分钟	((x*100ms)/1000*60)(min)
							AVS_RefTimeBValid = 1;
						}
					}
				}else
				{
					;
					//AVS_TripBValid = 0;
					//AVS_TripB = 0;
					//AVS_TripTimeB = 0;
					//AVS_RefTimeBValid = 0;
				}
				
			#endif
			
				DEA_SetAVSA_Valid(AVS_TripAValid);
				DEA_SetAVSA(AVS_TripA);		//AVS_tripA
				DEA_SetAVSB_Valid(AVS_TripBValid);
				DEA_SetAVSB(AVS_TripB);		//AVS_tripB


			#if 0				
				if(TravelTimeA<36000ul)
				{
					DEA_SetTripTimeA(TravelTimeA/10u);	//AVS_TripTimeA
				}else if(TravelTimeA<3600000ul)
				{
					DEA_SetTripTimeA(TravelTimeA/600u);	//AVS_TripTimeA
				}else
				{
					TravelTimeA =0ul;
					DEA_SetTripTimeA(TravelTimeA);	//AVS_TripTimeA
				}
				//DEA_SetTripTimeAValid(AVS_RefTimeAValid);
				DEA_SetTripTimeAValid(1u);
				
				if(TravelTimeB<36000ul)
				{
					DEA_SetTripTimeB(TravelTimeB/10u);	//AVS_TripTimeB
				}else if(TravelTimeB<3600000ul)
				{
					DEA_SetTripTimeB(TravelTimeB/600u);	//AVS_TripTimeB
				}else
				{
					TravelTimeB =0ul;
					DEA_SetTripTimeB(TravelTimeB);	//AVS_TripTimeB
				}
				//DEA_SetTripTimeBValid(AVS_RefTimeBValid);
				DEA_SetTripTimeBValid(1u);
			#else //2019-10-14 10:28:19
			
				if(TravelTimeA<3600000ul)
				{
					DEA_SetTripTimeA(TravelTimeA/10u);	//AVS_TripTimeA
				}else
				{
					TravelTimeA =0ul;
					DEA_SetTripTimeA(TravelTimeA);	//AVS_TripTimeA
				}
				//DEA_SetTripTimeAValid(AVS_RefTimeAValid);
				DEA_SetTripTimeAValid(1u);
				
				if(TravelTimeB<3600000ul)
				{
					DEA_SetTripTimeB(TravelTimeB/10u);	//AVS_TripTimeB
				}else
				{
					TravelTimeB =0ul;
					DEA_SetTripTimeB(TravelTimeB);	//AVS_TripTimeB
				}
				//DEA_SetTripTimeBValid(AVS_RefTimeBValid);
				DEA_SetTripTimeBValid(1u);

			#endif

#endif 

				
			}
			else
			{
				DEA_SetAVSA_Valid(0);
				DEA_SetAVSA(0);
				DEA_SetAVSB_Valid(0);
				DEA_SetAVSB(0);

				DEA_SetTripTimeA(0);
				DEA_SetTripTimeAValid(0);

				DEA_SetTripTimeB(0);
				DEA_SetTripTimeBValid(0);
			}
		}

	}
}

