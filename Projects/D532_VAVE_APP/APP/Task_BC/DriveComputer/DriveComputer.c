
#include "dea.h"
#include "DriveComputer.h"


/**��ѹ������**/
uint16_t	VpackData = 0; /**0~800v**/

uint8_t		Vpack_ValueValid = 0; //valid-flag	

/**����������**/

uint8_t Ipack_ValueValid = 0; //valid-flag

int16_t IpackData = 0; //Ipack data

/**�����������**/
uint8_t RMilge_ValueValid = 0; //valid-flag

uint32_t RemainMilge = 0; //RemainMileage data


DCP_Mode Remilge_mode = DCP_Normal;

DCP_Mode Ipack_mode = DCP_Normal;

DCP_Mode Vpack_mode = DCP_Normal;




void DriverCpt_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
	 	  Remilge_mode = DCP_Normal;
		  Ipack_mode = DCP_Normal;
		  Vpack_mode = DCP_Normal;


		RMilge_ValueValid = 0;
		Ipack_ValueValid = 0;
		Vpack_ValueValid = 0;

		RemainMilge = 0;
		IpackData = 0;
		VpackData = 0;
		
		DEA_SetReMilgeValueValid(RMilge_ValueValid);	
		DEA_SetFilterReMilgeValue(RemainMilge);

		DEA_SetFilterIpackValue(IpackData);
		DEA_SetIpackValueValid(Ipack_ValueValid);

		DEA_SetFilterVpackValue(VpackData);
		DEA_SetVpackValueValid(Vpack_ValueValid);

	}
}


/**�������**/
ReMilge_InputInfor UpdateReMilgeInfor()
{
	ReMilge_InputInfor ReMilgeInfor;

	if(DEA_GetReMilgeErrorValueStat() == 0)
	{
		ReMilgeInfor.u8ReMilgeValid = DCP_TRUE;
	}
	else
	{
		ReMilgeInfor.u8ReMilgeValid = DCP_FALSE;
	}

	/*
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_VCU_range) != 0)
	{
		ReMilgeInfor.u8ReMilgeTimeoutFlag = DCP_TRUE;
	}
	else
	{
		ReMilgeInfor.u8ReMilgeTimeoutFlag = DCP_FALSE;
	}
*/
	ReMilgeInfor.u32ReMilgeValue = DEA_GetRawReMilgeValue();	//ƫ����Ϊ0

	ReMilgeInfor.u8ReMilgeAlive = DCP_TRUE;

	return ReMilgeInfor;
	
}

/**������**/
Ipack_InputInfor UpdateIpackInfor()
{
	Ipack_InputInfor IpackInfor;

	if(DEA_GetIpackErrorValueStat() == 0) //signal normal
	{
		IpackInfor.u8IpackValid = DCP_TRUE;
	}
	else //signal  not normal
	{
		IpackInfor.u8IpackValid = DCP_FALSE;
	}
/*
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_BMS_meas) != 0)
	{
		IpackInfor.u8IpackTimeoutFlag = DCP_TRUE;
	}
	else
	{
		IpackInfor.u8IpackTimeoutFlag = DCP_FALSE;
	}
*/
	IpackInfor.u8IpackAlive = DCP_TRUE;

	IpackInfor.s16IpackValue = DEA_GetRawIpackValue(); //ƫ����Ϊ-600���˴���ʱ��δ����ƫ����
	if(IpackInfor.s16IpackValue >= 3100) //�޷�����
	{
		IpackInfor.s16IpackValue = 3100;
	}

	return IpackInfor;
	
}

/**��ѹ��**/
Vpack_InputInfor UpdateVpackInfor()
{
	Vpack_InputInfor VpackInfor;

	if(DEA_GetVpackErrorValueStat() == 0) //signal normal
	{
		VpackInfor.u8VpackValid = DCP_TRUE;
	}
	else //signal  not normal
	{
		VpackInfor.u8VpackValid = DCP_FALSE;
	}
/*
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_BMS_meas) != 0)
	{
		VpackInfor.u8VpackTimeoutFlag = DCP_TRUE;
	}
	else
	{
		VpackInfor.u8VpackTimeoutFlag = DCP_FALSE;
	}
*/
	VpackInfor.u8VpackAlive = DCP_TRUE;

	VpackInfor.u16VpackValue = DEA_GetRawVpackValue();	//ƫ����Ϊ0
	if(VpackInfor.u16VpackValue >= Vmax_value)	//�޷�����
	{
		VpackInfor.u16VpackValue = Vmax_value;
	}

	return VpackInfor;
}


//�������
void ReMilge_MAIN()
{
	ReMilge_InputInfor ReMilgeInformation;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		ReMilgeInformation = UpdateReMilgeInfor();

		switch(Remilge_mode)
		{
			case DCP_Normal:
				
					if((ReMilgeInformation.u8ReMilgeValid == DCP_TRUE) && (ReMilgeInformation.u8ReMilgeAlive == DCP_TRUE) && (ReMilgeInformation.u8ReMilgeTimeoutFlag == DCP_FALSE))
					{
						RemainMilge = ReMilgeInformation.u32ReMilgeValue;
						if(RemainMilge >= 1023)
						{
							RemainMilge = 1023; //�޷�����
						}
						RMilge_ValueValid = 1; //������Ч��־
					}
					else
					{
						RemainMilge = 0;
						Remilge_mode = DCP_Exception;
						RMilge_ValueValid = 0;
					}
				break;

			case DCP_Exception:
				
					if((ReMilgeInformation.u8ReMilgeValid == DCP_TRUE) && (ReMilgeInformation.u8ReMilgeAlive == DCP_TRUE) && (ReMilgeInformation.u8ReMilgeTimeoutFlag == DCP_FALSE))
					{
						
						Remilge_mode = DCP_Normal;
					}

				break;
				
			default:
				break;
		}
	
		DEA_SetReMilgeValueValid(RMilge_ValueValid);	
		DEA_SetFilterReMilgeValue(RemainMilge);
		
	}
	else
	{
		if(RemainMilge != 0)
		{
			RemainMilge = 0;
		}

		RMilge_ValueValid = 0;

		DEA_SetReMilgeValueValid(RMilge_ValueValid);	
		DEA_SetFilterReMilgeValue(RemainMilge);
		
	}
}


//������
void Ipack_MAIN()
{
	Ipack_InputInfor IpackInformation;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		IpackInformation = UpdateIpackInfor();

		switch(Ipack_mode)
		{
			case DCP_Normal:

					if((IpackInformation.u8IpackValid == DCP_TRUE) && (IpackInformation.u8IpackAlive == DCP_TRUE) && (IpackInformation.u8IpackTimeoutFlag == DCP_FALSE))
					{
						
						if((IpackInformation.s16IpackValue >= 0) && (IpackInformation.s16IpackValue <= 3100)) //���ݷ�Χ-600~2500
						{
							IpackData = IpackInformation.s16IpackValue - 600; // ƫ����-600
							Ipack_ValueValid = 1; //valid
						}
						else
						{
							IpackData = 0;
							Ipack_ValueValid = 0; // invalid
						}
					}
					else
					{
						IpackData = 0;
						Ipack_ValueValid = 0; // invalid

						Ipack_mode = DCP_Exception;
					}
				
				break;
				
			case DCP_Exception:

					if((IpackInformation.u8IpackValid == DCP_TRUE) && (IpackInformation.u8IpackAlive == DCP_TRUE) && (IpackInformation.u8IpackTimeoutFlag == DCP_FALSE))
					{
						Ipack_mode = DCP_Normal;
					}
					
				break;
				
			default:
				break;
		}

		DEA_SetFilterIpackValue(IpackData);
		DEA_SetIpackValueValid(Ipack_ValueValid);
		
	}
	else
	{
		if(IpackData != 0)
		{
			IpackData = 0;
		}

		Ipack_ValueValid = 0; // invalid

		DEA_SetFilterIpackValue(IpackData);
		DEA_SetIpackValueValid(Ipack_ValueValid);
	}
}

//��ѹ��
void Vpack_MAIN()
{
	Vpack_InputInfor VpackInformation;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		VpackInformation = UpdateVpackInfor(); //��ȡ����

		switch(Vpack_mode)
		{
			case DCP_Normal:

					if((VpackInformation.u8VpackValid == DCP_TRUE) && (VpackInformation.u8VpackAlive == DCP_TRUE) && (VpackInformation.u8VpackTimeoutFlag == DCP_FALSE))
					{
						VpackData = VpackInformation.u16VpackValue;
						Vpack_ValueValid = 1; //valid
					}
					else
					{
						VpackData = 0;
						Vpack_ValueValid = 0; //invalid
						Vpack_mode = DCP_Exception;
					}
					
				break;
				
			case DCP_Exception:

					if((VpackInformation.u8VpackValid == DCP_TRUE) && (VpackInformation.u8VpackAlive == DCP_TRUE) && (VpackInformation.u8VpackTimeoutFlag == DCP_FALSE))
					{
						Vpack_mode = DCP_Normal;
					}
					
				break;

			default:
				break;
				
			
		}

		DEA_SetFilterVpackValue(VpackData);
		DEA_SetVpackValueValid(Vpack_ValueValid);
	}
	else
	{
		if(VpackData != 0)
		{
			VpackData = 0;
		}
		Vpack_ValueValid = 0; //invalid

		DEA_SetFilterVpackValue(VpackData);
		DEA_SetVpackValueValid(Vpack_ValueValid);
	}
}




