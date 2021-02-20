
#include "IpcClr.h"
#include "odo.h"
#include "dea.h"


/**����ʱ���ʱ����ֵ**/
uint32_t Ipc_time = 0;

/**����ʱ���ʱ��ʼֵ**/
uint32_t Ipc_time_last = 0;

/**����Ϩ�𵽴�2h��־λ**/
uint8_t  Ipc_timeout = 0;

/**����һ�������־λ**/
uint8_t  First_mode = 0;


/**�ú�����ʱģ���RTCʱ��ģ���ȡʱ�䣬��ʱ���巵��ֵ��λΪS**/
uint32_t  RTC_Time()
{	
	static uint32_t Real_time = 0;
	return Real_time;
}


void Reset_CurrentTripData()
{
	if(Ipc_timeout == 1)
	{
		Ipc_timeout = 0;

		ODO_ResetTripB(); /**�����������**/

		AVS_ResetTripBAVS(); /**�������ƽ������,���������ʻʱ������**/

		AFC_vResetB(); /**�������ƽ���ͺ�����**/
		
	}
	
}

/*******/

/**Ϩ�𳬹���Сʱ������뱾�������ص����ݣ���:�������ƽ�����٣�����ƽ���ͺģ�������̣�������ʻʱ��**/
void CurrentTrip_MAIN()
{
	static IGN_STAT_ENUM IgnStatBak = IGN_OFF;
	uint32_t Ipc_time_err = 0; /**����Ϩ��ʱ��**/

	if(DEA_GetIgnStat() == IGN_OFF) //IGN_ON->IGN_OFF
	{
		if(IgnStatBak == IGN_ON) // IGN_ON before
		{
			IgnStatBak = IGN_OFF;
			
			Ipc_time_last = RTC_Time(); 

			Ipc_timeout = 0;
			
		}
		
	}
	else //IGN_OFF->IGN_ON
	{
		if(IgnStatBak == IGN_OFF) // IGN_OFF before
		{
			IgnStatBak = IGN_ON;

			Ipc_time = RTC_Time(); 
			
			Ipc_time_err = Ipc_time - Ipc_time_last;
			
			if(Ipc_time_err < 7200) /**Ϩ��ʱ�仹δ�ﵽ2h**/
			{
				Ipc_timeout = 0;
			}
			else  /**Ϩ��ﵽ2h**/          		
			{
				Ipc_timeout = 1;  /**��λ��ʱ��־λ**/
			}

			Reset_CurrentTripData();
		}
	}
	
}





