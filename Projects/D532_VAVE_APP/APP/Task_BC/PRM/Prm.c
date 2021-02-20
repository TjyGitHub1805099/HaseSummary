
#include "dea.h"
#include "Prm.h"


/**���幦�ʱ���**/
int16_t PpackData;
int16_t AvsPpackAData = 0; /**ƽ������A**/
int16_t AvsPpackBData = 0; /**ƽ������B**/


int16_t PpackDataCompare = 0;

/**������ֵ��Ч�Ա�־λ**/
uint8_t  Prm_ValueValid = 0;
uint8_t  AvPpackValueAValid = 0;
uint8_t  AvPpackValueBValid = 0;



PRM_Mode Prm_mode = PRM_Normal; /*��ʼ��switch()�������*/



void Prm_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
	 	Prm_mode = PRM_Normal; 
		Prm_ValueValid = 0;
		PpackData = 0;
	
		DEA_SetFilterPpackValue(PpackData);	
		DEA_SetPrmValueValid(Prm_ValueValid);

	}
}





Prm_InputInfor PrmInputInfor()
{
	Prm_InputInfor PrmInfor;
	
	if(DEA_GetPrmErrorValueStat() == 0) /*Valid*/
	{
		PrmInfor.u8PrmValid = VEL_True;
	}
	else
	{
		PrmInfor.u8PrmValid = VEL_False;
	}
/*
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_BMS_meas) != 0)
	{
		PrmInfor.u8PrmTimeoutFlag = VEL_True;
	}
	else
	{
		PrmInfor.u8PrmTimeoutFlag = VEL_False;
	}
*/
	PrmInfor.u8PrmAlive = VEL_True; /*Alive*/

	PrmInfor.s16PpackValue = DEA_GetRawPpackValue(); /*������ʻ����*/ //ƫ����Ϊ0

	return PrmInfor;
	
}


void PRM_vExceptionProcess() /*clear data*/
{
	PpackData = 0;
	Prm_ValueValid = 0;
}



	




void PRM_Main()
{
	static uint16_t timecntA = 0;
	static int32_t pPackSumA = 0;

	static uint16_t timecntB = 0;
	static int32_t pPackSumB = 0;
	
	Prm_InputInfor PrmInformation;

	if(DEA_GetIgnStat() == IGN_ON)
	{
		PrmInformation = PrmInputInfor();//��ȡ��������
		
		switch(Prm_mode)
		{
			case PRM_Normal:
							
							if((PrmInformation.u8PrmTimeoutFlag == VEL_False) && (PrmInformation.u8PrmAlive == VEL_True) && (PrmInformation.u8PrmValid == VEL_True))
							{
								PpackDataCompare = PrmInformation.s16PpackValue;
								if( DEA_GetFilterIpackValue() >= 0) /**current > 0**/
								{
									if(PpackDataCompare >= 400) //�޷�
									{
										PpackDataCompare = 400;
									}
									PpackData = PpackDataCompare; //˲ʱ����,Ϊ������Ǳ��������
									Prm_ValueValid = 1;

									/**ƽ������A**/
									pPackSumA += PpackData; //��δ��ƫ�ƴ���
									timecntA++;   /**50ms cycle**/

									if(pPackSumA >=AvPpackSumMax) //overflow
									{
										pPackSumA = pPackSumA/2; //������۰봦��
										timecntA = timecntA/2; 
									}
														
									AvsPpackAData = (pPackSumA/timecntA); /**ƽ������A**/
									AvPpackValueAValid = 1;

									/**ƽ������B**/
									pPackSumB += PpackData; //��δ��ƫ�ƴ���
									timecntB++;   /**50ms cycle**/

									if(pPackSumB >=AvPpackSumMax) //overflow
									{
										pPackSumB = pPackSumB/2; //������۰봦��
										timecntB = timecntB/2; 
									}
														
									AvsPpackBData = (pPackSumB/timecntB); /**ƽ������B**/
									AvPpackValueBValid = 1;
								}
								else	/**current < 0**/
								{
									if(PpackDataCompare >= 50) //�޷�
									{
										PpackDataCompare = 50;
									}
									PpackData = (-PpackDataCompare); //˲ʱ����,Ϊ������Ǳ��������
									Prm_ValueValid = 1;

									/**ƽ������A**/
									pPackSumA += PpackData; //��δ��ƫ�ƴ���
									timecntA++;   /**50ms cycle**/

									if((-pPackSumA) >=AvPpackSumMax) //overflow
									{
										pPackSumA = pPackSumA/2; //������۰봦��
										timecntA = timecntA/2; 
									}
														
									AvsPpackAData = (pPackSumA/timecntA); /**ƽ������A**/
									AvPpackValueAValid = 1;

									/**ƽ������B**/
									pPackSumB += PpackData; //��δ��ƫ�ƴ���
									timecntB++;   /**50ms cycle**/

									if((-pPackSumB) >=AvPpackSumMax) //overflow
									{
										pPackSumB = pPackSumB/2; //������۰봦��
										timecntB = timecntB/2; 
									}
														
									AvsPpackBData = (pPackSumB/timecntB); /**ƽ������B**/
									AvPpackValueBValid = 1;
								}

							}
							else
							{
								PRM_vExceptionProcess();	
								Prm_mode = PRM_Exception;
								AvsPpackAData = 0;
								AvPpackValueAValid = 0;
								
								AvsPpackBData = 0;
								AvPpackValueBValid = 0;

							}
							
							
						break;
			case PRM_Exception:

							if((PrmInformation.u8PrmTimeoutFlag == VEL_False) && (PrmInformation.u8PrmAlive == VEL_True) && (PrmInformation.u8PrmValid == VEL_True))
							{
								Prm_mode = PRM_Normal;					
							}
				
							
						break;
						
				default:
						break;
		}

		DEA_SetFilterPpackValue(PpackData);	
		DEA_SetPrmValueValid(Prm_ValueValid);

		DEA_SetAvpPackAValue(AvsPpackAData);
		DEA_SetAvpPackAValueValid(AvPpackValueAValid);

		DEA_SetAvpPackBValue(AvsPpackBData);
		DEA_SetAvpPackBValueValid(AvPpackValueBValid);

	}
	else
	{
		if(PpackData != 0)
		{
			PpackData = 0;
		}
		Prm_ValueValid = 0;
		AvsPpackAData = 0;


		
		DEA_SetFilterPpackValue(PpackData);	
		DEA_SetPrmValueValid(Prm_ValueValid);

		DEA_SetAvpPackAValue(AvsPpackAData);
		DEA_SetAvpPackAValueValid(AvPpackValueAValid);

		DEA_SetAvpPackBValue(AvsPpackBData);
		DEA_SetAvpPackBValueValid(AvPpackValueBValid);

	}
}




