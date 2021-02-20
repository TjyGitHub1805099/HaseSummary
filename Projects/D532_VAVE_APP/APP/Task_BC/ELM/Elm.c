
#include "Elm.h"
#include "dea.h"

#if 0
//对应的报警灯点亮熄灭，声音报警，文字报警信息的接口函数(由于目前底层部门还没写好，因此此处暂时自写代替，等底层写好后再替换)
IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);
IPC_PutAlarmMsg(NORMAL_MSG_ID_LowFuel, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);
void IPC_PutAlarmMsg(NORMAL_MSG_ID_ENUM id, /* message id,eg  NORMAL_MSG_ID_LowFuel*///信息的ID值
			TELLTALE_ENUM TelltaleStatus, /* Telltale Status, eg OFF, ON, Flash 1HZ, Flash 3Hz *///指示灯
			ICON_STAT_ENUM  LcdStatus, 	/* Lcd PopUp Status, eg OFF, ON *///显示弹出窗口(文字报警)
			ACOUSTIC_STAT_ENUM Acoustic)	/* Acoustic Alarm Status, eg OFF, ON *///声音报警

//一下为对应的报警功能对应的传递参数值
typedef enum
{
	TELLTALE_OFF = 0,//报警灯灭
	TELLTALE_ON,//报警灯亮
	TELLTALE_1Hz,//报警灯闪烁频率
	TELLTALE_3Hz,
	TELLTALE_INVALID,//无效值
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

/**定义用于仪表显示的SOC数值变量**/
uint8_t SocData = 0;

/**定义报警虚拟标志位**/
unsigned char ELM_WarnFlag = ELM_False;

/**定义返回信号有效性标志**/
uint8_t Soc_ValueValid = 0;

ELM_Mode Elm_mode = ELM_Normal; /*初始化Elm_mode时程序SWITCH()一开始从ELM_Normal开始运行*/


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

//获取DEA的BMS_SOC信息
BmsSoc_InputInfor Bms_SocInputInfor()
{
	BmsSoc_InputInfor Elm_SocInfor;

	//信号的有效性
	if(DEA_GetSocErrorValueStat() == 0)
	{
		Elm_SocInfor.u8SocValid = ELM_True; //信号有效
	}
	else
	{
		Elm_SocInfor.u8SocValid = ELM_False; //信号无效
	}
/*
	//判断信号超时
	if(DEA_GetCanTimeOutStat(IPDU_HND_GTW_BMS_metrics) != 0)
	{
		Elm_SocInfor.u8SocTimeoutFlag = ELM_True; //超时
	}
	else
	{
		Elm_SocInfor.u8SocTimeoutFlag = ELM_False; //未超时
	}
*/
	Elm_SocInfor.u8SocAlive = ELM_True;

	Elm_SocInfor.u8SocValue = DEA_GetRawSocValue(); //偏移量为0

	return Elm_SocInfor;
}

//判断DEA传过来的数据是否超过了100%,做限幅处理
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

//清零SocData
SOC_vExceptionProcess()
{
	SocData = 0;
}







void Elm_SocMain()
{
	BmsSoc_InputInfor SocInfor;
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		SocInfor = Bms_SocInputInfor();//获取SOC信息
		
		switch(Elm_mode)
		{

			case ELM_Normal:
							if((SocInfor.u8SocAlive == ELM_True) && (SocInfor.u8SocTimeoutFlag == ELM_False) && (SocInfor.u8SocValid == ELM_True))
							{
								SocData = SocMaxJudge(SocInfor.u8SocValue);//数据限幅处理
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
				//IPC_PutAlarmMsg(NORMAL_MSG_ID_LowSoc, TELLTALE_ON, ICON_ON, ACOUSTIC_OFF);//报警灯点亮，文字提示弹出，报警声关闭
				ELM_WarnFlag = ELM_True;
			}
		}
		else if(SocData > 22)
		{
			if(ELM_WarnFlag == ELM_True)
			{
				//IPC_PutAlarmMsg(NORMAL_MSG_ID_LowSoc, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//报警灯关闭，文字提示不弹出，报警声关闭
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





