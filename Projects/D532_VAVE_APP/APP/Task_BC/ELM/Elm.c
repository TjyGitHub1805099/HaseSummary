
#include "Elm.h"
#include "dea.h"

#if 0
//��Ӧ�ı����Ƶ���Ϩ���������������ֱ�����Ϣ�Ľӿں���(����Ŀǰ�ײ㲿�Ż�ûд�ã���˴˴���ʱ��д���棬�ȵײ�д�ú����滻)
IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);
IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
void IPC_PutAlarmMsg(NORMAL_MSG_ID_ENUM id, /* message id,eg  NORMAL_MSG_ID_LowFuel*///��Ϣ��IDֵ
			TELLTALE_ENUM TelltaleStatus, /* Telltale Status, eg OFF, ON, Flash 1HZ, Flash 3Hz *///ָʾ��
			ICON_STAT_ENUM  LcdStatus, 	/* Lcd PopUp Status, eg OFF, ON *///��ʾ��������(���ֱ���)
			ACOUSTIC_STAT_ENUM Acoustic)	/* Acoustic Alarm Status, eg OFF, ON *///��������

//һ��Ϊ��Ӧ�ı������ܶ�Ӧ�Ĵ��ݲ���ֵ
typedef enum
{
	TELLTALE_OFF = 0,//��������
	TELLTALE_ON,//��������
	TELLTALE_1Hz,//��������˸Ƶ��
	TELLTALE_3Hz,
	TELLTALE_INVALID,//��Чֵ
}TELLTALE_ENUM;


typedef enum
{
	ICON_OFF = 0,
	ICON_ON,
	ICON_INVALID,
}ICON_STAT_ENUM;


typedef enum
{
	ACOUSTIC_OFF = 0,
	ACOUSTIC_ON,
	ACOUSTIC_INVALID,
}ACOUSTIC_STAT_ENUM;

#endif

/**���������Ǳ���ʾ��SOC��ֵ����**/
uint8_t SocData = 0;

/**���屨�������־λ**/
unsigned char ELM_WarnFlag = ELM_False;

/**���巵���ź���Ч�Ա�־**/
uint8_t Soc_ValueValid = 0;

ELM_Mode Elm_mode = ELM_Normal; /*��ʼ��Elm_modeʱ����SWITCH()һ��ʼ��ELM_Normal��ʼ����*/


void Soc_Init()
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
	 	Elm_mode = ELM_Normal;
		Soc_ValueValid = 0;
		SocData = 0;
		DEA_SetFilterBmsSocValue(SocData);	
		DEA_SetSocValueValid(Soc_ValueValid);
	}
}

//��ȡDEA��BMS_SOC��Ϣ
BmsSoc_InputInfor Bms_SocInputInfor()
{
	BmsSoc_InputInfor Elm_SocInfor;

	//�źŵ���Ч��
	if(DEA_GetSocErrorValueStat() == 0)
	{
		Elm_SocInfor.u8SocValid = ELM_True; //�ź���Ч
	}
	else
	{
		Elm_SocInfor.u8SocValid = ELM_False; //�ź���Ч
	}
/*
	//�ж��źų�ʱ
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_BMS_metrics) != 0)
	{
		Elm_SocInfor.u8SocTimeoutFlag = ELM_True; //��ʱ
	}
	else
	{
		Elm_SocInfor.u8SocTimeoutFlag = ELM_False; //δ��ʱ
	}
*/
	Elm_SocInfor.u8SocAlive = ELM_True;

	Elm_SocInfor.u8SocValue = DEA_GetRawSocValue(); //ƫ����Ϊ0

	return Elm_SocInfor;
}

//�ж�DEA�������������Ƿ񳬹���100%,���޷�����
uint8_t SocMaxJudge(uint8_t soc_value)
{
	BmsSoc_InputInfor Elm_SocInformation;

	if(soc_value >= BMS_SocMax)
	{
		Elm_SocInformation.u8SocValue = BMS_SocMax;
	}
	else
	{
		Elm_SocInformation.u8SocValue = soc_value;
	}

	return Elm_SocInformation.u8SocValue;
}

//����SocData
SOC_vExceptionProcess()
{
	SocData = 0;
}







void Elm_SocMain()
{
	BmsSoc_InputInfor SocInfor;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		SocInfor = Bms_SocInputInfor();//��ȡSOC��Ϣ
		
		switch(Elm_mode)
		{

			case ELM_Normal:
							if((SocInfor.u8SocAlive == ELM_True) && (SocInfor.u8SocTimeoutFlag == ELM_False) && (SocInfor.u8SocValid == ELM_True))
							{
								SocData = SocMaxJudge(SocInfor.u8SocValue);//�����޷�����
								Soc_ValueValid = 1;
							}
							else
							{
								SOC_vExceptionProcess();
								Elm_mode = ELM_Exception;
								Soc_ValueValid = 0;
							}
							
							break;

			case ELM_Exception:
							if((SocInfor.u8SocAlive == ELM_True) && (SocInfor.u8SocTimeoutFlag == ELM_False) && (SocInfor.u8SocValid == ELM_True))
							{
								Elm_mode = ELM_Normal;
							}
									
							break;

				    default:
							break;
		}
	#if 0
		/**UI to show this message**/
		if(SocData < 20)
		{
			if(ELM_WarnFlag == ELM_False)
			{
				//IPC_PutAlarmMsg(NORMAL_MSG_ID_LowSoc, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);//�����Ƶ�����������ʾ�������������ر�
				ELM_WarnFlag = ELM_True;
			}
		}
		else if(SocData > 22)
		{
			if(ELM_WarnFlag == ELM_True)
			{
				//IPC_PutAlarmMsg(NORMAL_MSG_ID_LowSoc, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//�����ƹرգ�������ʾ���������������ر�
				ELM_WarnFlag = ELM_False;
			}
		}
	#endif
		DEA_SetFilterBmsSocValue(SocData);
		DEA_SetSocValueValid(Soc_ValueValid);
	}
	else
	{
		if(SocData !=0)
		{
			SocData = 0;	
		}
		Soc_ValueValid = 0; // ivalid
		
		DEA_SetFilterBmsSocValue(SocData);	
		DEA_SetSocValueValid(Soc_ValueValid);
	}
}





