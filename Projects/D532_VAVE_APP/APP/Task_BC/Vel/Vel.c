//******************************************************************************
//file		  		: vel.c 
//Description	  	: calculate output speed information
//Copyright (c) 2017-2037 HSAE electronics co. Ltd. All rigths reserved
//Change Logs:
//Date			   Author		Note	       
//2017/5/8         eason             First draft version
//2017/6/1         eason             Modify for HC Cluster

//******************************************************************************
//*****************************************************************************
//
//! \addtogroup vel
//! @{
//
//*****************************************************************************

#include "Vel.h"
#include "Vel_cfg.h"
#include "alga.h"                          
#include "string.h" 
#include "dea.h"

/** define  Vel processing  state varible*/
 VEL_Mode VEL_enMode = VEL_Normal;     

/** define  DPOOL type for speedo information*/
uint16_t stSpeedo = 0;

uint8_t VehicleSpeedValid;



//boolean VEL_Initialized = FALSE;

/**
  * \brief judge if move
  *
  *\param VEL_u16Speed current speed  
  *   
  * \return move flag
  */

#if 0
static unsigned char VEL_u8MoveJudge(unsigned short VEL_u16Speed)
{
	unsigned char VEL_u8MoveFlag;
	
	if (VEL_u16Speed > VEL_u16MaxMove)
	{
		VEL_u8MoveFlag = VEL_True;
	}
	else if (VEL_u16Speed <= VEL_u16MinMove)
	{
		VEL_u8MoveFlag = VEL_False;
	}
	
	return VEL_u8MoveFlag;
}
#endif


uint16_t VEL_vCalDisplay(uint16_t vel)
{
	uint32_t l_vel;
	
	#if 0
	if(vel < 20)
	{
		l_vel = vel + vel*VEL_u8GetFactor()/100 + VEL_u8GetOffset1();
	}
	else if(vel < 60)
	{
		l_vel = vel + vel*VEL_u8GetFactor()/100 + VEL_u8GetOffset2();
	}
	else if(vel < 120)
	{
		l_vel = vel + vel*VEL_u8GetFactor()/100 + VEL_u8GetOffset3();
	}
	else
	{
		l_vel = vel + vel*VEL_u8GetFactor()/100 + VEL_u8GetOffset4();
	}

	if(l_vel > VEL_u16GetMaxVelocity())
	{
		l_vel =  VEL_u16GetMaxVelocity();
	}
	#endif

	l_vel = (vel*106)/100;	//591 project require 	l_vel = vel * 1.06
	
	return l_vel;
}





/**
  * \brief judge if exceed speed
  *
  *\param VEL_u16Speed current speed  
  *   
  * \return exceed speed flag
  */
static unsigned char VEL_u8ExceedSpeedJudge(unsigned short VEL_u16Speed)
{
	unsigned char VEL_u8ExceedSpeedFlag;
	
	if(VEL_u16Speed > VEL_u16GetMaxVelocity())
	{
		VEL_u8ExceedSpeedFlag = VEL_True;
	}
	else
	{
		VEL_u8ExceedSpeedFlag = VEL_False;
	}
	
	return VEL_u8ExceedSpeedFlag;
}

/**
  * \brief exception process except exceed speed
  *
  *\param None
  *   
  * \return None
  */
static void VEL_vExceptionProcess(void)
{
	stSpeedo = 0;
}

VEL_Mode VEL_vGetVelMode(void)
{
	return VEL_enMode;
}

/**
  * \brief Initial Vel processing  state , Initial dpool data
  *
  *\param None  
  *   
  * \return None
  */
void VEL_vInit(void)
{
	VEL_enMode = VEL_Normal;

	stSpeedo = 0;
	VehicleSpeedValid = 0;

	VEL_vWrite(stSpeedo,VehicleSpeedValid);

	VEL_InitCoding();
}

/**
  * \brief From vel input information , calurate dpool data
  *
  *\param None  
  *   
  * \return None
  */
void VEL_vMain(void)
{	

	uint32_t l_velspeed = 0;	
	uint32_t l_veltemp = 0;
	uint16_t l_targetspeed = 0;

	VEL_InputInfor VEL_stInputInfor;

	if(DEA_GetIgnStat() == IGN_ON)
	{
	
		VEL_stInputInfor = VEL_stUpdateSpeedInfor();//get the speed message have expand 10times

		switch(VEL_enMode)	//switch the vehicle mode
		{
			case VEL_Initial:
			case VEL_Normal:
				
				if((VEL_stInputInfor.u8SpeedValid == VEL_True) && (VEL_stInputInfor.u8SpeedAlive == VEL_True) && (VEL_stInputInfor.u8SpeedTimeoutFlag == VEL_False))
				{
				
					l_velspeed = VEL_stInputInfor.u16SpeedValue * (VEL_Rm1/VEL_Rc);	//expand 1000*10times
					l_veltemp = l_velspeed/1000;
				#if 0	
					if(VEL_stInputInfor.u16SpeedValue > 2400)
					{
						l_velspeed = 2400;
					}
					else if(VEL_stInputInfor.u16SpeedValue == 2400)
					{
						l_velspeed = l_veltemp + 100 + (180-100)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 2200)
					{
						l_velspeed = l_veltemp + 90 + (170-90)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 2000)
					{
						l_velspeed = l_veltemp + 90 + (150-90)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 1800)
					{
						l_velspeed = l_veltemp + 80 + (140-80)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 1600)
					{
						l_velspeed = l_veltemp + 70 + (130-70)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 1400)
					{
						l_velspeed = l_veltemp + 60 + (120-60)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 1200)
					{
						l_velspeed = l_veltemp + 55 + (110-55)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 1000)
					{
						l_velspeed = l_veltemp + 50 + (90-50)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 800)
					{
						l_velspeed = l_veltemp + 40 + (80-40)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 600)
					{
						l_velspeed = l_veltemp + 30 + (70-30)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 400)
					{
						l_velspeed = l_veltemp + 20 + (60-20)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 200)	//new add funcations
					{
						l_velspeed = l_veltemp + 10 + (50-10)/2;
					}
					else if(VEL_stInputInfor.u16SpeedValue >= 30)	//new add funcations
					{
						l_velspeed = l_veltemp + 10;
					}
					else	//new add funcations
					{
						
						l_velspeed = 0;
					}
					#else

					if(VEL_stInputInfor.u16SpeedValue >= 30u)
					{
						l_velspeed = l_veltemp*105ul/100ul + 20ul;
					}else
					{
						//l_velspeed = 0ul;
						l_velspeed = l_veltemp;
					}
					if(l_velspeed >=2400ul)
					{
						l_velspeed = 2400ul;
					}
					#endif
					
					l_targetspeed = (uint16_t)(l_velspeed);	//expand 10times

					#if 0
					if(l_targetspeed > 0)
					{
						l_targetspeed += 5;	//+0.5km/h
					}
					#endif
					
					//judge if need to fillter handle
					if(VEL_u8GetDampOff() == VEL_True)	//without filter handling
					{
						stSpeedo = l_targetspeed;		
					}
					else							//do filter handle
					{
						stSpeedo = ALGA_vDelayPT1W(l_targetspeed, stSpeedo, VEL_u16VelPT1Rise, VEL_u16VelPT1Fall); //fileter handle
					}

					//limit the MAX value
					if(VEL_u8ExceedSpeedJudge(stSpeedo))	//exception valid value Exceed max
					{
						stSpeedo = VEL_u16GetMaxVelocity();
					}
		
					VehicleSpeedValid = 1;
				}
				else	//exception Signal invalid, timeout, reserved or Alive signal not ok
				{
					l_velspeed = 0;	
	 				l_veltemp = 0;
	 				l_targetspeed = 0;
					
					VEL_vExceptionProcess();				
					VEL_enMode = VEL_Exception;
					VehicleSpeedValid = 0;
				}
				
				break;	
				
			case VEL_Exception:
				
				if((VEL_stInputInfor.u8SpeedValid == VEL_True) && (VEL_stInputInfor.u8SpeedAlive == VEL_True) && (VEL_stInputInfor.u8SpeedTimeoutFlag == VEL_False)) 
				{

					VEL_enMode = VEL_Normal;
				}
				
				break;		
		
		}
		
		VEL_vWrite(stSpeedo,VehicleSpeedValid);

	}
	else
	{
		if(stSpeedo != 0)
		{
			stSpeedo = 0;
			
		}
		VehicleSpeedValid = 0;
		VEL_vWrite(stSpeedo,VehicleSpeedValid);
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************




