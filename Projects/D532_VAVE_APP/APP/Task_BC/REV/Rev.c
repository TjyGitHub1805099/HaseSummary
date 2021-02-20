//******************************************************************************
//file		  		: rev.c 
//Description	  	: calculate output rev information
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/05/08         eason             First draft version
//2017/06/09         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup rev
//! @{
//
//*****************************************************************************

#include "Rev.h"
#include "Rev_cfg.h"
#include "alga.h"                          
#include "string.h"
#include "dea.h"

#include "NvM.h"
#include "MemMap.h"
#include "NvM_if.h"
#include "HardwareVersion.h"


REV_MODE Rev_mode = State_0;

BACKRAM_SEC_VAR_UNSPECIFIED uint8_t Recive3ECFlag;
BACKRAM_SEC_VAR_UNSPECIFIED uint8_t RevFirstMode;

/** define  DPOOL type for rev information */
static REV_DATA_STRU stTachoData; 	

/** define previous  valus varible in idle control */
#if (REV__IdleControl_Cfg == REV_On)
static unsigned short REV_u16RevolutionOldValue; 
//static unsigned short REV__u32RevolutionPT1_2nd; 
#endif

//static boolean REV_Initialized = FALSE;

/**
  * \brief judge if engin run
  *
  *\param REV_u16Revolution current engin revolution  
  *   
  * \return run flag
  */
static unsigned char REV_u8EnginRunJudge(unsigned short REV_u16Revolution)
{
	static unsigned char REV_u8RunFlag = REV_True;
	
	if (REV_u16Revolution > REV_u16MaxEnginRun)
	{
		REV_u8RunFlag = REV_True;
	}
	else if (REV_u16Revolution <= REV_u16MinEnginRun)
	{
		REV_u8RunFlag = REV_False;
	}
	
	return REV_u8RunFlag;
}

/**
  * \brief judge if exceed max revolution
  *
  *\param REV_u16Revolution current engin revolution 
  *   
  * \return exceed revolution flag
  */
static unsigned char REV_u8ExceedEnginMaxJudge(unsigned short REV_u16Revolution)
{
	unsigned char REV_u8ExceedEnginFlag;
	
	if(REV_u16Revolution > REV_u16MaxRevolution)
	{
		REV_u8ExceedEnginFlag = REV_True;
	}
	else
	{
		REV_u8ExceedEnginFlag = REV_False;
	}
	
	return REV_u8ExceedEnginFlag;
}

/**
  * \brief exception process except exceed revolution
  *
  *\param None
  *   
  * \return None
  */
static void REV_vExceptionProcess(void)
{
#if (REV__IdleControl_Cfg == REV_On)
	REV_u16RevolutionOldValue = 0;
#endif
	memset( &stTachoData, 0, sizeof( stTachoData));
}

/**
  * \brief calculate 2nd pt1 function
  *
  *\param current revolution
  *   
  * \return show revolution
  */
#if (REV__IdleControl_Cfg == REV_On)
static unsigned short REV__u16Calc2ndDamping (unsigned short u16CurrentValue)
{
	unsigned short u16Temp; 
	if( REV_u8GetDampOff() != 0) //damp off 
	{          
	   u16Temp = u16CurrentValue;
	}
	else 
	{                         
	   u16Temp = ALGA_vDelayPT1W( u16CurrentValue, REV_u16RevolutionOldValue, REV_u16IdlePT1Rise, REV_u16IdlePT1Fall);
	}
	return u16Temp;
}
#endif

/**
  * \brief Initial rev processing  state , Initial all varibels and update dpool data
  *
  *\param None  
  *   
  * \return None
  */
void REV_vInit(void)
{
	Rev_mode = State_0;
	if(DEA_GetSysRstType() == RST_IRRST)	//BAT ON
	{
		Recive3ECFlag = 0;
		RevFirstMode = 0;
	}

	#if (REV__IdleControl_Cfg == REV_On)
	REV_u16RevolutionOldValue= 0;             
    #endif
	memset( &stTachoData, 0, sizeof( stTachoData));

	REV_vWrite(stTachoData.REV_boEngineRun, stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
}

/**
  * \brief From rev input information , calurate dpool data
  *
  *\param None  
  *   
  * \return None
  */
#if 0
void REV_vMain(void)
{	
	static unsigned short u16CurrentRevolution;
	
	REV_InputInfor REV_stInputInfor; 
	#if (REV__IdleControl_Cfg == REV_On)
	unsigned short REV__u16DeltaRev;			//Delta between the current and old value of revolution 
	#endif


	if(DEA_GetIgnStat() == IGN_ON)
	{	
		REV_stInputInfor = REV_stUpdateEnginInfor();

		switch(REV_enMode)
		{

			case REV_Initial:
				
				if((REV_stInputInfor.u8EnginValid == REV_True) && (REV_stInputInfor.u8EnginAlive == REV_True) && (REV_stInputInfor.u8EnginTimeoutFlag == REV_False))
				{
					
					u16CurrentRevolution = REV_stInputInfor.u16EnginValue ;
					stTachoData.REV_u16Revolution = u16CurrentRevolution;//rpm value
		
					if(REV_u8ExceedEnginMaxJudge(stTachoData.REV_u16Revolution))	//exception valid value Exceed max
					{
						stTachoData.REV_u16Revolution = REV_u16MaxRevolution;
					}
					
					REV_enMode = REV_Normal;

					stTachoData.REV_boEngineRun = REV_u8EnginRunJudge(stTachoData.REV_u16Revolution);	//judge if the engine is running
					
					stTachoData.REV_boIsValidEngineRevolution = REV_True;//rpm valid 
				}
				else
				{
					REV_vExceptionProcess();
					REV_enMode = REV_Exception;
				}
				
				break;
				
			case REV_Normal:
				
				if((REV_stInputInfor.u8EnginValid == REV_True) && (REV_stInputInfor.u8EnginAlive == REV_True) && (REV_stInputInfor.u8EnginTimeoutFlag == REV_False))
				{
					if(REV_u8GetDampOff() == REV_True) 	//damp off
					{
						u16CurrentRevolution = REV_stInputInfor.u16EnginValue ;
					}
					else						
					{
						u16CurrentRevolution = ALGA_vDelayPT1W(REV_stInputInfor.u16EnginValue, u16CurrentRevolution, REV_u16PT1Rise, REV_u16PT1Fall);//fillter handle
					}
						
					stTachoData.REV_boEngineRun = REV_u8EnginRunJudge(u16CurrentRevolution);	//judge if the engine is running
					stTachoData.REV_boIsValidEngineRevolution = REV_True;

					//条件编译
					#if (REV__IdleControl_Cfg == REV_On)	// idle control on//怠速控制

					if ((stTachoData.REV_boEngineRun == REV_True) && (REV_u8GetDampOff() == 0))	//(Rc < Rmin) and (damping is off) 			
					{
						if (u16CurrentRevolution > REV_u16RevolutionOldValue)	//当前转速值大于之前转速值
						{
							REV__u16DeltaRev = u16CurrentRevolution - REV_u16RevolutionOldValue; //get the error value
							if ( REV__u16DeltaRev > REV__nu8IdleHyst)										//>H1(Idle hysterisis parameter) 
							{
								if ((u16CurrentRevolution < REV__nu16IdleMaxRPM ) && ( REV__u16DeltaRev < REV__nu8IdlePT1Hyst ))//R0+H1~R0+H2(2nd hysterisis parameter)
								{
									REV_u16RevolutionOldValue = REV__u16Calc2ndDamping(u16CurrentRevolution - REV__nu8IdleHyst);//fillter handle (Rc - H1) is calculated and this value is subjected to PT1 damping. The damped value is updated into dpool
								}
								else //(Rc > R0+H2) or (Rc >= Rmax): (Rc - H1) is calculated and this value is updated into dpool.					
								{ 
									REV_u16RevolutionOldValue = (u16CurrentRevolution - REV__nu8IdleHyst);
								}
							}
							else	//(R0<= Rc <= R0+H1):Rc is ignored and R0 will be used to update dpool	
							{

							}
						} 
						else		//current(Rc) < previous (R0) //当前转速值小于之前转速值
						{
							REV__u16DeltaRev = REV_u16RevolutionOldValue - u16CurrentRevolution;//get the error value
							
							if (REV__u16DeltaRev > REV__nu8IdleHyst) 	
							{
								if ((u16CurrentRevolution < REV__nu16IdleMaxRPM ) && (REV__u16DeltaRev < REV__nu8IdlePT1Hyst ))	//(R0-H1> Rc > R0-H2) and (Rc < Rmax)
								{
									REV_u16RevolutionOldValue = REV__u16Calc2ndDamping(u16CurrentRevolution + REV__nu8IdleHyst); //fillter handle
								}	
								else			// (Rc < R0-H2) or (Rc >= Rmax)
								{ 
									REV_u16RevolutionOldValue = (u16CurrentRevolution + REV__nu8IdleHyst);	
								}
							}
							else	//(R0>= Rc >= R0-H1)
							{

							}
						}
					} 
					else //(Rc < Rmin) and (damping is off),Rc is updated to dpool without any changes// update the oldRevolution
					{
						REV_u16RevolutionOldValue = u16CurrentRevolution;
					} 	
					stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue;//update rpm
					
	                #else
					stTachoData.REV_u16Revolution = u16CurrentRevolution;//during normal state,not in the idle state
					#endif 	
					
						
					if(REV_u8ExceedEnginMaxJudge(stTachoData.REV_u16Revolution))	//exception valid value Exceed max
					{
						stTachoData.REV_u16Revolution = REV_u16MaxRevolution;
					}
				}
				else	//exception Signal invalid, timeout, reserved or Alive signal not ok
				{
					REV_vExceptionProcess();
					u16CurrentRevolution = 0;
					REV_enMode = REV_Exception;
				}
				
				break;	
				
			case REV_Exception:
				
				if((REV_stInputInfor.u8EnginValid == REV_True) && (REV_stInputInfor.u8EnginAlive == REV_True) && (REV_stInputInfor.u8EnginTimeoutFlag == REV_False)) 
				{

					REV_enMode = REV_Normal;
				}
				
				break;		

		}
		
		REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
	}
	else
	{
		if((stTachoData.REV_boEngineRun != 0) || (stTachoData.REV_u16Revolution > 0) || (REV_u16RevolutionOldValue > 0) || (u16CurrentRevolution > 0))
		{
			stTachoData.REV_boEngineRun = 0;
			stTachoData.REV_u16Revolution =0;
			REV_u16RevolutionOldValue = 0;
			u16CurrentRevolution = 0;
			REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
		}
	}
}
#endif


//591 project require
void REV_vMain(void)
{

	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;	
	static unsigned short u16CurrentRevolution;
	#if (REV__IdleControl_Cfg == REV_On)
	unsigned short REV__u16DeltaRev;			//Delta between the current and old value of revolution 
	#endif

	//static unsigned char Xcnt = 0;
	
	static unsigned char flag = 0;

	REV_InputInfor REV_stInputInfor;

#if 0
	if(RevFirstMode != 1)
	{
		Recive3ECFlag = DEA_GetRawEngine3ECStat();
	}

	if(1 == Recive3ECFlag)
	{
		flag = 1; //rpmID:3EC
		RevFirstMode = 1;
	}
	else
	{
		flag = 0;  //rpmID:180
	}
	
	if(flag == 1)
	{
		//rpmID:3EC
		REV_stInputInfor =  REV_stUpdateEnginInfor3EC();	//received 0x3EC before
	}
	else
	{
		//rpmID:180
		REV_stInputInfor =  REV_stUpdateEnginInfor();	//never received 0X3EC before
	}
#else
	REV_stInputInfor =  REV_stUpdateEnginInfor(); //rpmID:180
#endif


	switch(Rev_mode)
	{
		case State_0:

					if(REV_False == REV_stInputInfor.u8EnginTimeoutFlag)	//not timeout
					{
						u16CurrentRevolution = 0 ;
						stTachoData.REV_u16Revolution = u16CurrentRevolution;//rpm value
						
						stTachoData.REV_boEngineRun = REV_u8EnginRunJudge(u16CurrentRevolution);	//judge if the engine is running
						stTachoData.REV_boIsValidEngineRevolution = REV_True;
					}
					else
					{
						memset(&stTachoData,0,sizeof(stTachoData));
					}		

					if((getITMasterCtrlVerion() >= 0b1110) || (getITMasterCtrlVerion() == 0b1100)) //yangxl 2019-9-24 11:19:18
					{
						if(0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2))
						{
							if((DEA_GetRawRevolutionIdleStat() == 0x01) && (s_IgnBak == IGN_ON))
							{
								Rev_mode = State_2;
							}
							else if((DEA_GetRawRevolutionIdleStat() == 0x00) && (s_IgnBak == IGN_ON))
							{
								Rev_mode = State_1;
							}
						}
						else
						{
							Rev_mode = State_1;
						}
					}else
					{
						Rev_mode = State_1;
					}
					
					
			break;
			
		case State_1:

					if(REV_False == REV_stInputInfor.u8EnginTimeoutFlag)	//not timeout
					{
						u16CurrentRevolution = REV_stInputInfor.u16EnginValue ;
						stTachoData.REV_u16Revolution = u16CurrentRevolution;//rpm value
						
						stTachoData.REV_boEngineRun = 1;//REV_u8EnginRunJudge(u16CurrentRevolution);	//judge if the engine is running
						stTachoData.REV_boIsValidEngineRevolution = REV_True;
					}
					else
					{
						memset(&stTachoData,0,sizeof(stTachoData));
					}
					
					if ((stTachoData.REV_boEngineRun == REV_True) && (REV_u8GetDampOff() == 0)) //(Rc < Rmin) and (damping is off)			
					{
						if(u16CurrentRevolution <= RevA)
						{		
							if(REV_u16RevolutionOldValue > RevA)
							{
								REV_u16RevolutionOldValue = RevA;
							}
							if(u16CurrentRevolution >= RevE)	//u16CurrentRevolution < E
							{
								if(REV_u16RevolutionOldValue >= RevB*1)
								{
									if((REV_u16RevolutionOldValue > u16CurrentRevolution) && ((REV_u16RevolutionOldValue - u16CurrentRevolution) >= RevC)) //two times fillter
									{
										if(REV_u16RevolutionOldValue >= (RevB*1 + 5))
										{
											stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue - 5; //fillter handle(591 project require)
										}
										else
										{
											stTachoData.REV_u16Revolution = RevB*1;
										}
									}
									else
									{
										if(REV_u16RevolutionOldValue > u16CurrentRevolution)
										{
											stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue;
										}
										else
										{
											//stTachoData.REV_u16Revolution =  u16CurrentRevolution;
											if (u16CurrentRevolution > REV_u16RevolutionOldValue)
											{
												REV__u16DeltaRev = u16CurrentRevolution - REV_u16RevolutionOldValue; //get the error value
												if ( REV__u16DeltaRev > REV__nu8IdleHyst)										//>H1(Idle hysterisis parameter) 
												{
													REV_u16RevolutionOldValue = (u16CurrentRevolution);
												}
												else	//(R0<= Rc <= R0+H1):Rc is ignored and R0 will be used to update dpool	
												{

												}
											} 
											else		//current(Rc) < previous (R0) 
											{
												REV__u16DeltaRev = REV_u16RevolutionOldValue - u16CurrentRevolution;//get the error value
												
												if (REV__u16DeltaRev > REV__nu8IdleHyst) 	
												{
													REV_u16RevolutionOldValue = (u16CurrentRevolution);	
												}
												else	//(R0>= Rc >= R0-H1)
												{

												}
											}

											stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue;//update rpm
										}
									}
								}
								else
								{
									stTachoData.REV_u16Revolution = RevB;
								}
							}
							else
							{
								stTachoData.REV_u16Revolution = u16CurrentRevolution;
							}
						}
						else
						{
							//fillter handle
							u16CurrentRevolution = ALGA_vDelayPT1W(REV_stInputInfor.u16EnginValue, u16CurrentRevolution, REV_u16PT1Rise, REV_u16PT1Fall);//fillter handle
							if (u16CurrentRevolution > REV_u16RevolutionOldValue)
							{
								REV__u16DeltaRev = u16CurrentRevolution - REV_u16RevolutionOldValue; //get the error value
								if ( REV__u16DeltaRev > REV__nu8Hyst)										//>H1(Idle hysterisis parameter) 
								{
									REV_u16RevolutionOldValue = u16CurrentRevolution;
								}
								else	//(R0<= Rc <= R0+H1):Rc is ignored and R0 will be used to update dpool	
								{

								}
							} 
							else		//current(Rc) < previous (R0) 
							{
								REV__u16DeltaRev = REV_u16RevolutionOldValue - u16CurrentRevolution;//get the error value
								
								if (REV__u16DeltaRev > REV__nu8Hyst) 	
								{
									REV_u16RevolutionOldValue = u16CurrentRevolution;	
								}
								else	//(R0>= Rc >= R0-H1)
								{

								}
							}
							stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue;//update rpm
						}
					} 
					else
					{
						stTachoData.REV_u16Revolution = 0;
					}
							
					REV_u16RevolutionOldValue = stTachoData.REV_u16Revolution; //update old rpm 				
							
							
					if(REV_u8ExceedEnginMaxJudge(stTachoData.REV_u16Revolution))	//exception valid value Exceed max
					{
						stTachoData.REV_u16Revolution = REV_u16MaxRevolution;
					}


					if(0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2))
					{
						if(s_IgnBak == IGN_OFF)
						{
							Rev_mode = State_0;
						}
						else if(DEA_GetRawRevolutionIdleStat() == 0x01)
						{
							Rev_mode = State_2;
						}
					}
					else
					{
						Rev_mode = State_1;
					}

			
				break;
				
		case State_2:
			
					if(REV_False == REV_stInputInfor.u8EnginTimeoutFlag)	//not timeout
					{
						u16CurrentRevolution = 0 ;
						stTachoData.REV_u16Revolution = u16CurrentRevolution;//rpm value
						
						stTachoData.REV_boEngineRun = REV_u8EnginRunJudge(u16CurrentRevolution);	//judge if the engine is running
						stTachoData.REV_boIsValidEngineRevolution = REV_True;
					}
					else
					{
						memset(&stTachoData,0,sizeof(stTachoData));
					}
							
					REV_u16RevolutionOldValue = stTachoData.REV_u16Revolution; //update old rpm 				
				#if 0		
					if((0 == DEA_GetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS))&&(0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2)))
					{
						if(s_IgnBak == IGN_OFF)
						{
							Rev_mode = State_0;
						}
						else if((DEA_GetRawRevolutionIdleStat() == 0x00) && (DEA_GetRawRevolutionIdleRequest() != 0x03))
						{
							Rev_mode = State_1;
						}
					}
					else
					{
						Rev_mode = State_1;
					}
				#else
					if((0 == DEA_GetCanTimeOutStat(IPDU_HND_ECCSMSG8_OBD_IS))&&(0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2)))
					{
						if(s_IgnBak == IGN_OFF)
						{
							Rev_mode = State_0;
						}
						else if((DEA_GetRawRevolutionIdleStat() == 0x00) && (DEA_GetRawRevolutionIdleRequest() != 0x03))
						{
							Rev_mode = State_1;
						}
					}
					else 
					{
						
						if(s_IgnBak == IGN_OFF)
						{
							Rev_mode = State_0;
						}else if(((0 == DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2))&&(DEA_GetRawRevolutionIdleStat() == 0x00))||(0 != DEA_GetCanTimeOutStat(IPDU_HND_ECM_TorqueControl_RN2)))
						{
							Rev_mode = State_1;
						}
					}
				#endif

			break;
			
		default:
			
			break;

	}

	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(s_IgnBak == IGN_OFF) //IGN_OFF -> IGN_ON
		{
			s_IgnBak = IGN_ON;
		}
		REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
	}
	else 
	{
		if(s_IgnBak == IGN_ON) //IGN_ON -> IGN_OFF
		{
			s_IgnBak = IGN_OFF;

			stTachoData.REV_boEngineRun = 0;
			stTachoData.REV_u16Revolution = 0;
			stTachoData.REV_boIsValidEngineRevolution = 0;
		}
		REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
	}


	
#if 0
	if(DEA_GetIgnStat() == IGN_ON)
	{
		if(DEA_GetRawRevolutionIdleStat() == 0x01)	//idle status = 1
		{
			
			if(DEA_GetRawRevolutionIdleRequest() != 11)
			{
				u16CurrentRevolution = REV_stInputInfor.u16EnginValue ;
			}
			else
			{
				
				u16CurrentRevolution = 0 ;
			}		
		}
		else if((DEA_GetRawRevolutionIdleStat() == 0x00))	//idle status = 0
		{
			u16CurrentRevolution = REV_stInputInfor.u16EnginValue ;
		}
		


		if((REV_stInputInfor.u8EnginValid == REV_True) && (REV_stInputInfor.u8EnginAlive == REV_True) && (REV_stInputInfor.u8EnginTimeoutFlag == REV_False))
		{

			stTachoData.REV_u16Revolution = u16CurrentRevolution;//rpm value
		
			stTachoData.REV_boEngineRun = REV_u8EnginRunJudge(u16CurrentRevolution);	//judge if the engine is running
			stTachoData.REV_boIsValidEngineRevolution = REV_True;
		

		
			if ((stTachoData.REV_boEngineRun == REV_True) && (REV_u8GetDampOff() == 0)) //(Rc < Rmin) and (damping is off)			
			{
				if(u16CurrentRevolution <= A)
				{
					Xcnt++;	//u16CurrentRevolution <= A twice
							
					if(u16CurrentRevolution >= E)	//u16CurrentRevolution < E
					{
						if(REV_u16RevolutionOldValue >= B*1)
						{
							if(Xcnt >= 2)
							{
								Xcnt = 0;
								if((REV_u16RevolutionOldValue - u16CurrentRevolution) >= C) //two times fillter
								{
									stTachoData.REV_u16Revolution -= 1; //fillter handle(591 project require)
								}
								else
								{
									if((REV_u16RevolutionOldValue - u16CurrentRevolution) > 0)
									{
										stTachoData.REV_u16Revolution = REV_u16RevolutionOldValue;
									}
									else
									{
										stTachoData.REV_u16Revolution =  u16CurrentRevolution;
									}
								}
							}
							else
							{
								stTachoData.REV_u16Revolution = u16CurrentRevolution;
							}
						}
						else
						{
							stTachoData.REV_u16Revolution = B;
						}
					}
					else
					{
						stTachoData.REV_u16Revolution = u16CurrentRevolution;
					}
				}
				else
				{
					//fillter handle
					stTachoData.REV_u16Revolution = ALGA_vDelayPT1W(REV_stInputInfor.u16EnginValue, u16CurrentRevolution, REV_u16PT1Rise, REV_u16PT1Fall);//fillter handle
				}
			} 
			else
			{
				stTachoData.REV_u16Revolution = u16CurrentRevolution;
			}
					
			REV_u16RevolutionOldValue = stTachoData.REV_u16Revolution; //update old rpm					
					
					
			if(REV_u8ExceedEnginMaxJudge(stTachoData.REV_u16Revolution))	//exception valid value Exceed max
			{
				stTachoData.REV_u16Revolution = REV_u16MaxRevolution;
			}
		}
		else	//exception Signal invalid, timeout, reserved or Alive signal not ok
		{
			REV_vExceptionProcess();
			u16CurrentRevolution = 0;
			REV_enMode = REV_Exception;
		}				

		REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
	}
	else
	{	
		stTachoData.REV_boEngineRun = 0;
		stTachoData.REV_u16Revolution = 0;
		stTachoData.REV_boIsValidEngineRevolution = 0;

		REV_vWrite(stTachoData.REV_boEngineRun , stTachoData.REV_u16Revolution,stTachoData.REV_boIsValidEngineRevolution);
	}
#endif

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




