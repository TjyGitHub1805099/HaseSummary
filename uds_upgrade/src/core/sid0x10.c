/**************************
description: public general process APIs for sid 0x10 of UDS
author: liqi@hangsheng.com.cn
version: v1.0.0
date:2017/05/4
**************************/
	
#include"sid0x10_if.h"
#include"sid0x27_if.h"
#include"sid0x28_if.h"
#include"uds_sid_if.h"
#include"download_if.h"

extern const tSid0x10CfgInfor gSid0x10CfgInfor;
extern const tDiagSidServiceInfor gDiagSidService[];

//tUdsSessionInnerType gUdsSessionType = UDS_DEFAULT_SESSION;
uint32 gUdsSessionType = UDS_DEFAULT_SESSION;


tUdsSessionInnerType sid0x10GetCurSession()
{
	return gUdsSessionType;
}



tUdsNRCType sid0x10SetSession(tUdsSessionInnerType newSessionType)
{
	uint8 i = 0;
	tUdsNRCType ret = NRC_NONE;
	uint8 supportedSidTotalCount = udsSidGetSupportedSidCount();
	
	switch(newSessionType)
	{
	case UDS_DEFAULT_SESSION:		
		for(i=0;i<supportedSidTotalCount;i++)
		{
			if(gDiagSidService[i].stopHookFunc != NULL)
			{
				gDiagSidService[i].stopHookFunc();
			}
		}
		gUdsSessionType = UDS_DEFAULT_SESSION;
		break;
	default:
		//default to None-default
		if(gUdsSessionType == UDS_DEFAULT_SESSION)
		{
			gUdsSessionType = newSessionType;
		}
		else // none-default to none-default
		{
			ret = sid0x10ChangeSessionStrategy(newSessionType);
			if(ret == NRC_NONE || ret == NRC_RESPONSE_PENDING)
			{
				//security shall be relocked
				#ifdef UDS_WITH_SID0X27
				sid0x27ResetStatus();
				#endif
				for(i=0;i<supportedSidTotalCount;i++)
				{
					if((gDiagSidService[i].sessionTypeFilter & newSessionType) == 0)
					{
						if(gDiagSidService[i].stopHookFunc != NULL)
						{
							gDiagSidService[i].stopHookFunc();
						}
					}
					else
					{
						//All other active diagnostic functionality that is supported in the new session 
						//and is not dependent upon security access shall be maintained.

						//nothing to do,just keep it
					}
				}

				gUdsSessionType = newSessionType;
			}
		}
	}

	return ret;
}



