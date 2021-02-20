
#include  "dea.h"
#include  "MotorSp.h"


int32_t		MotorSpData = 0;	//-26000~26000

int32_t		MotorSpCompare = 0; //定义一个滤波的中间传递值

uint8_t		MotorSp_ValueValid = 0; //valid-flag	


MSP_Mode Msp_mode = MSP_Normal; //定义程序运行状态选择基

void MotorSp_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
	 	Msp_mode = MSP_Normal; 
		MotorSp_ValueValid = 0;
		MotorSpData = 0;
	
	DEA_SetFilterMspValue(MotorSpData);
	DEA_SetMspValueValid(MotorSp_ValueValid);



	}
}



MotorSp_InputInfor UpdateReMotorSpInfor()
{
	MotorSp_InputInfor MotorSpInfor;

	if(DEA_GetMspErrorValueStat() == 0) //signal normal
	{
		MotorSpInfor.u8MspValid = MSP_TRUE;
	}
	else
	{
		MotorSpInfor.u8MspValid = MSP_FALSE;
	}
/*
	
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_mcuf0_info) != 0)
	{
		MotorSpInfor.u8MspTimeoutFlag = MSP_TRUE;
	}
	else
	{
		MotorSpInfor.u8MspTimeoutFlag = MSP_FALSE;
	}
*/
	MotorSpInfor.u8MspAlive = MSP_TRUE;

	MotorSpInfor.s32MspValue = DEA_GetRawMspValue();	//偏移量-26000，此处暂时不处理偏移量,且已经处理了精度

	return MotorSpInfor;

}


 
//电机转速滤波处理函数接口
signed int FilteringHandle(signed int s32TargetPos,signed int s32CurrentPos,unsigned short u16RiseStep,unsigned short u16FallStep)
{
	
	signed int u16ShowPosition;
	signed int u16TargetPos_Varible;
	signed int u16StepValue;

	u16TargetPos_Varible = s32TargetPos;
	
	if(u16TargetPos_Varible < s32CurrentPos)			//decrease 
	{
		u16StepValue = (s32CurrentPos - u16TargetPos_Varible)/u16FallStep;
		
		if(u16StepValue < 1)
		{
			u16StepValue = 1;
		}

		u16ShowPosition = s32CurrentPos - u16StepValue;
	}
	else if(u16TargetPos_Varible> s32CurrentPos)		//increase
	{		
		u16StepValue = (u16TargetPos_Varible - s32CurrentPos)/u16RiseStep;
		
		if(u16StepValue < 1)
		{
			u16StepValue = 1;
		}
		u16ShowPosition = s32CurrentPos + u16StepValue;
	}	
	else
	{
		u16ShowPosition = s32CurrentPos;
	}
	
	return u16ShowPosition; 
}


void MotorSp_MAIN()
{
	MotorSp_InputInfor MotorSpInformation;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		MotorSpInformation = UpdateReMotorSpInfor();

		switch(Msp_mode)
		{
			case MSP_Normal:

					if((MotorSpInformation.u8MspValid == MSP_TRUE) && (MotorSpInformation.u8MspTimeoutFlag == MSP_FALSE) && (MotorSpInformation.u8MspAlive == MSP_TRUE))
					{
						MotorSpCompare = FilteringHandle(MotorSpInformation.s32MspValue,MotorSpCompare,MSP_s32MspPT1Rise ,MSP_s32MspPT1Fall); //滤波处理


						if(MotorSpCompare >= MOTORSP_MAX) //限幅处理
						{
							MotorSpCompare = MOTORSP_MAX;
						}

						MotorSpData = MotorSpCompare - 26000; //数据偏移处理
						MotorSp_ValueValid = 1;//valid
		
					}
					else
					{
						MotorSpData = 0;
						MotorSp_ValueValid = 0;//invalid
						Msp_mode = MSP_Exception;
					}
					
				break;
			case MSP_Exception:

					if((MotorSpInformation.u8MspValid == MSP_TRUE) && (MotorSpInformation.u8MspTimeoutFlag == MSP_FALSE) && (MotorSpInformation.u8MspAlive == MSP_TRUE))
					{
						Msp_mode = MSP_Normal;
					}
					
				break;
		}
		DEA_SetFilterMspValue(MotorSpData);
		DEA_SetMspValueValid(MotorSp_ValueValid);
	}
	else
	{
		if(MotorSpData != 0)
		{
			MotorSpData = 0;
		}
		MotorSp_ValueValid = 0;//invalid

		DEA_SetFilterMspValue(MotorSpData);
		DEA_SetMspValueValid(MotorSp_ValueValid);
		
	}
}




