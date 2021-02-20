#define IOC_PLATFORM
#include "MaintainService.h"
#include "dea.h"
#include "NvM.h"
//#include "DeaCoding.h"
//#include "Task_IpcApp_Cfg.h"


//#pragma segment BACK_RAM_START
int32_t g_SERV_Distance = 0;
uint8_t g_SERV_DistanceValid = 0;
//#pragma segment BACK_RAM_END
uint8_t g_SERV_AccomplishFlag = 0;

int32_t SERV_GetDistance(void)
{
	return g_SERV_Distance;
}

uint8_t SERV_GetDistanceValid(void)
{
	return g_SERV_DistanceValid;
}

void SERV_SetAccomplishFlag(uint8_t stat)
{
	g_SERV_AccomplishFlag = stat;
}

void vMaintianServInit(void)
{
	if(DEA_GetSysRstType() == RST_IRRST)
	{
		InitServCfg();
	}
	
	if(DEA_GetOdoValid())//����ܼ���Ч
	{
		g_SERV_DistanceValid = 1;//���ñ��������Ч
		if(DEA_GetOdo() < g_SERV_Coding.u32OdoValueLastReset)
		{
			g_SERV_Coding.u32OdoValueLastReset = DEA_GetOdo();
		}
		g_SERV_Distance = (int32_t)g_SERV_Coding.u16ServValue - (DEA_GetOdo()-g_SERV_Coding.u32OdoValueLastReset);
	}
}

void vMaintianServMain(void)
{
	static IGN_STAT_ENUM s_IgnBak = IGN_OFF;
	static uint32_t s_odoBak = 0;
	//static uint32_t s_AccomplishNtfCnt = 0;
	static uint8_t s_sendAlarmFlag = 0;
	static uint8_t s_OdoInvalid = 0;
	//WARN_MSG_STRU l_warnMsg;
	//IPC_APP_NORMAL_MSG_STRU l_msg = {0};
	
	if(DEA_GetIgnStat() == IGN_ON)
	{
		
		#if 0
		if(g_SERV_AccomplishFlag == 1)//�������
		{

			if(s_AccomplishNtfCnt == 0)
			{
				l_msg.ID = NORMAL_MSG_ID_ServiceAccomplish;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				s_AccomplishNtfCnt++;
			}
			else if(s_AccomplishNtfCnt < 30)	//100*30=3S
			{
				s_AccomplishNtfCnt++;
			}
			else
			{
				g_SERV_AccomplishFlag = 0;
				l_msg.ID = NORMAL_MSG_ID_ServiceAccomplish;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
			}
			
		}
		else
		{
			s_AccomplishNtfCnt = 0;
		}
		#endif
		
		if(s_IgnBak == IGN_OFF) //�𿵱����������(�״δ��)
		{     
			if((g_SERV_Distance < 500) && (g_SERV_Distance > 0)) //0<~<500
			{
				//send alarm
				#if 0
				l_warnMsg.WarnMsgId = 149;
				l_warnMsg.BitBlinking = 0;
				l_warnMsg.BitStatus = 0;
				DEA_PutICWarnMsg(l_warnMsg);
				#endif
				
				if(g_SERV_Coding.AlarmFlag)
				{
					//IPC_PutAlarmMsg(NORMAL_MSG1_ID_LowServ, TELLTALE_OFF, ICON_ON, ACOUSTIC_ON);//������Ϩ��������ʾ��������������
				}
				s_sendAlarmFlag = 1;
			}
			else if(g_SERV_Distance == 0) //=0
			{
				if(g_SERV_Coding.AlarmFlag)
				{
					//IPC_PutAlarmMsg(NORMAL_MSG2_ID_LowServ, TELLTALE_OFF, ICON_ON, ACOUSTIC_ON);//������Ϩ��������ʾ��������������
				}
				s_sendAlarmFlag = 1;
			}
			
			s_IgnBak = IGN_ON;
		}
			
		if(DEA_GetOdoValid())//�ź���Ч
		{
			if(s_OdoInvalid == 1)
			{
				s_OdoInvalid = 0;

				if(DEA_GetOdo() < g_SERV_Coding.u32OdoValueLastReset)
				{
					g_SERV_Coding.u32OdoValueLastReset = DEA_GetOdo();
				}

				#if 0
				l_msg.ID = NORMAL_MSG_ID_ServiceError;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;
				l_msg.LcdStatus = NORMAL_MSG_STAT_RESET;
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				#endif

				//IPC_PutAlarmMsg(NORMAL_MSG3_ID_LowServ, TELLTALE_OFF, ICON_OFF, ACOUSTIC_OFF);//������Ϩ��������ʾϨ�𣬱�����Ϩ��
			}
			
			g_SERV_DistanceValid = 1;
			if(s_odoBak != DEA_GetOdo())
			{
				s_odoBak = DEA_GetOdo();
				g_SERV_Distance = (int32_t)g_SERV_Coding.u16ServValue - (s_odoBak-g_SERV_Coding.u32OdoValueLastReset);//���㱣�����ʣ�������
				
		
				if(g_SERV_Distance >= 32766)	//max 
				{
					g_SERV_Distance = 32766;
				}
				else
				{
					if(g_SERV_Distance < -32768)	//min
					{
						g_SERV_Distance = -32768;
					}
					
					if((g_SERV_Distance < 500) && (g_SERV_Distance > 0))
					{
						//send alarm
						if(s_sendAlarmFlag == 0)
						{
							#if 0
							l_warnMsg.WarnMsgId = 149;
							l_warnMsg.BitBlinking = 0;
							l_warnMsg.BitStatus = 0;
							DEA_PutICWarnMsg(l_warnMsg);
							#endif 
							
							if(g_SERV_Coding.AlarmFlag)
							{
								//IPC_PutAlarmMsg(NORMAL_MSG1_ID_LowServ, TELLTALE_OFF, ICON_ON, ACOUSTIC_ON);//������Ϩ��������ʾ��������������
							}
							s_sendAlarmFlag = 1;
						}
					}
					else if(g_SERV_Distance == 0)
					{
						//send alarm
						if(s_sendAlarmFlag == 0)
						{
							if(g_SERV_Coding.AlarmFlag)
							{
								//IPC_PutAlarmMsg(NORMAL_MSG2_ID_LowServ, TELLTALE_OFF, ICON_ON, ACOUSTIC_ON);//������Ϩ��������ʾ��������������
							}
							s_sendAlarmFlag = 1;
						}
					}
				}
			}
		}
		else //�ź���Ч
		{
			g_SERV_DistanceValid = 0;
			if(s_OdoInvalid == 0)
			{
				s_OdoInvalid = 1;
				#if 0
				l_msg.ID = NORMAL_MSG_ID_ServiceError;
				l_msg.Acoustic = NORMAL_MSG_STAT_RESET;//����
				l_msg.LcdStatus = NORMAL_MSG_STAT_SET; //������ʾ
				l_msg.TelltaleStatus = NORMAL_MSG_STAT_RESET;//ָʾ��
				PutToMsgRing(TYPE_MSG_NORMAL, (IPC_APP_MSG_STRU *)&l_msg);
				#endif
				
				//IPC_PutAlarmMsg(NORMAL_MSG4_ID_LowServ, TELLTALE_OFF, ICON_ON, ACOUSTIC_OFF);//������Ϩ��������ʾ�������������ر�
			}
		}
	}
	else
	{
		s_IgnBak = IGN_OFF;
		s_sendAlarmFlag = 0;
	}
}
