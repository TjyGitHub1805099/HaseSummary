/*============================[R E V I S I O N   H I S T O R Y]=================================*/
/************************************************************************************************/
/* These MISRA-C Rules not obeyed */

/* MISRA-C:2004 Rule 17.4,
 * The integer value 1 is being added or subtracted from a pointer.
 */
 
/* MISRA-C:2004 Rule 19.1,
 * #include statements in a file should only be preceded by other preprocessor directives or comments.
 */
  
/*============================[V E R S I O N  I N F O R M A T I O N]============================*/
/************************************************************************************************/

#if 0

#define ACCTL_C_AR_MAJOR_VERSION   1U
#define ACCTL_C_AR_MINOR_VERSION   3U
#define ACCTL_C_AR_PATCH_VERSION   0U

#define ACCTL_C_SW_MAJOR_VERSION   1U
#define ACCTL_C_SW_MINOR_VERSION   0U
#define ACCTL_C_SW_PATCH_VERSION   0U

/*============================[I N C L U D E S]=================================================*/
/************************************************************************************************/

#include "trace.h"
#include "AC_CTL.h"
#include "PanelKey.h"

#include "Task_M_IpcApp_Cfg.h"
#include "Spi_Ipc_gw_api.h"

#include "lpuart_driver.h"
#include "System_Tick.h"
/*============================[V E R S I O N  C H E C K]========================================*/
/************************************************************************************************/

#if (ACCTL_C_AR_MAJOR_VERSION != ACCTL_H_AR_MAJOR_VERSION)
#error "AcCtl.c:Mismatch in Specification Major Version"
#endif

#if (ACCTL_C_AR_MINOR_VERSION != ACCTL_H_AR_MINOR_VERSION)
#error "AcCtl.c:Mismatch in Specification Minor Version"
#endif

#if (ACCTL_C_AR_PATCH_VERSION != ACCTL_H_AR_PATCH_VERSION)
#error "AcCtl.c:Mismatch in Specification Patch Version"
#endif

#if (ACCTL_C_SW_MAJOR_VERSION != ACCTL_H_SW_MAJOR_VERSION)
#error "CanNm.c:Mismatch in Specification Major Version"
#endif

#if (ACCTL_C_SW_MINOR_VERSION != ACCTL_H_SW_MINOR_VERSION)
#error "CanNm.c:Mismatch in Specification Minor Version"
#endif

/*============================[M A C R O S]=====================================================*/
/************************************************************************************************/

/****************************************AC_DATA_3***********************************************/
const uint8 AcPanelMapAll[AC2DA_FRAME_LEN-4][8]=
{
{AC_KEY_VOICE_FB,AC_KEY_AC,AC_KEY_RECIRC,AC_KEY_FRESH,AC_KEY_MODE,0,0,0},//AC_DATA_3
{AC_KEY_ON_OFF,AC_KEY_REDEFROST,AC_KEY_DEFROST,AC_KEY_AUTO,AC_KEY_BLOWER_ADD,0,0,0},//AC_DATA_4
{AC_KEY_LTMP_ADD,0,0,0,0,0,0,0},//AC_DATA_5
{0,0,0,0,0,0,0,0},//AC_DATA_6
{0,0,0,0,0,0,0,0},//AC_DATA_7
{0,0,0,0,0,0,0,0},//AC_DATA_8
{AC_KEY_SYNC,AC_KEY_DISPLAY,0,0,0,0,0,0},//AC_DATA_9
{AC_KEY_RTMP_ADD,0,0,0,0,0,0,0},//AC_DATA_10
{0,0,0,0,0,0,0,0},//AC_DATA_11
{AC_KEY_CHAGE,0,0,0,0,0,0,0},//AC_DATA_12
{AC_KEY_CHAGE_STATUS,0,0,0,0,0,0,0},//AC_DATA_13
{0,0,0,0,0,0,0,0},//AC_DATA_14
};
const AcPanelMapStruct AcPanelMapEvent[]=
{
	{AC_KEY_NONE,		0X00,0X00,0X00},
	{AC_KEY_LTMP_ADD,	AC_DATA_5,0X00,0XFF},
	{AC_KEY_LTMP_DEC,	AC_DATA_5,0X00,0XFF},
	{AC_KEY_BLOWER_ADD,	AC_DATA_4,0X04,0XF0},
	{AC_KEY_BLOWER_DEC,	AC_DATA_4,0X04,0XF0},
	
	{AC_KEY_MODE,		AC_DATA_3,0X05,0XE0},
	
	{AC_KEY_ON_OFF,		AC_DATA_4,0X00,0X01},
	
	{AC_KEY_CYCLE,		AC_DATA_3,0X02,0X04},
	
	{AC_KEY_AUTO,		AC_DATA_4,0X03,0X08},
	
	{AC_KEY_DEFROST,	AC_DATA_4,0X02,0X04},
	{AC_KEY_REDEFROST,	AC_DATA_4,0X01,0X02},
	
	{AC_KEY_AC,			AC_DATA_3,0X01,0X02},
	
	{AC_KEY_RTMP_ADD,	AC_DATA_10,0X00,0XFF},
	{AC_KEY_RTMP_DEC,	AC_DATA_10,0X00,0XFF},
	{AC_KEY_SYNC,		AC_DATA_9,0X00,0X01},
	
	{AC_KEY_VOICE_FB,	AC_DATA_3,0X00,0X01},
	{AC_KEY_RECIRC,		AC_DATA_3,0X02,0X04},
	{AC_KEY_FRESH,		AC_DATA_3,0X03,0X08},
	{AC_KEY_DISPLAY,	AC_DATA_9,0X01,0X02},
	{AC_KEY_CHAGE,		AC_DATA_12,0X00,0XFF},
	{AC_KEY_CHAGE_STATUS,AC_DATA_13,0X00,0X01},
};
#define AC_PANEL_MAP_CYCLE STD_OFF
#if (STD_ON == AC_PANEL_MAP_CYCLE)
const AcPanelMapStruct AcPanelMapCycle[]=
{
	{AC_KEY_VOICE_FB,	AC_DATA_3,0X00,0X01},
	{AC_KEY_AC,			AC_DATA_3,0X01,0X02},
	{AC_KEY_RECIRC,		AC_DATA_3,0X02,0X04},
	{AC_KEY_FRESH,		AC_DATA_3,0X03,0X08},
	{AC_KEY_MODE,		AC_DATA_3,0X05,0XE0},

	{AC_KEY_ON_OFF,		AC_DATA_4,0X00,0X01},
	{AC_KEY_REDEFROST,	AC_DATA_4,0X01,0X02},
	{AC_KEY_DEFROST,	AC_DATA_4,0X02,0X04},
	{AC_KEY_AUTO,		AC_DATA_4,0X03,0X08},
	{AC_KEY_BLOWER_ADD,	AC_DATA_4,0X04,0XF0},

	{AC_KEY_LTMP_ADD,	AC_DATA_5,0X00,0XFF},
	
	{AC_KEY_SYNC,		AC_DATA_9,0X00,0X01},
	
	{AC_KEY_RTMP_ADD,	AC_DATA_10,0X00,0XFF},
};
#endif
const DaKeyMapStruct DaKeyMap[]=
{
	{AC_KEY_NONE,		0X00,0X00,0X00,0X00,0X00,0X00},
	
	{AC_KEY_LTMP_ADD,	DA_DATA_3,0X07,0X80,DA_DATA_6,0X00,0X7F},
	{AC_KEY_LTMP_DEC,	DA_DATA_3,0X06,0X40,DA_DATA_6,0X00,0X7F},
	
	{AC_KEY_BLOWER_ADD,	DA_DATA_4,0X01,0X02,DA_DATA_4,0X04,0XF0},
	{AC_KEY_BLOWER_DEC,	DA_DATA_4,0X00,0X01,DA_DATA_4,0X04,0XF0},
	
	{AC_KEY_MODE,		DA_DATA_3,0X05,0X20,DA_DATA_5,0X05,0XE0},
	{AC_KEY_ON_OFF,		DA_DATA_5,0X01,0X02,0X00,0X00,0X00},
	{AC_KEY_CYCLE,		DA_DATA_3,0X02,0X04,0X00,0X00,0X00},
	
	{AC_KEY_AUTO,		DA_DATA_4,0X02,0X04,0X00,0X00,0X00},
	
	{AC_KEY_DEFROST,	DA_DATA_3,0X04,0X10,0X00,0X00,0X00},
	{AC_KEY_REDEFROST,	DA_DATA_5,0X02,0X04,0X00,0X00,0X00},
	
	{AC_KEY_AC,			DA_DATA_3,0X01,0X02,0X00,0X00,0X00},
	
	{AC_KEY_RTMP_ADD,	DA_DATA_5,0X04,0X10,DA_DATA_7,0X00,0X7F},
	{AC_KEY_RTMP_DEC,	DA_DATA_5,0X03,0X08,DA_DATA_7,0X00,0X7F},
	
	{AC_KEY_SYNC,		DA_DATA_5,0X00,0X01,0X00,0X00,0X00},
	
	{AC_KEY_VOICE_FB,	DA_DATA_6,0X07,0X80,0X00,0X00,0X00},
	
	{AC_KEY_RECIRC,		0X00,0X00,0X00,0X00,0X00,0X00},
	{AC_KEY_FRESH,		0X00,0X00,0X00,0X00,0X00,0X00},
	{AC_KEY_DISPLAY,	0X00,0X00,0X00,0X00,0X00,0X00},
	{AC_KEY_CHAGE,		DA_DATA_8,0X01,0X02,0X00,0X00,0X00},
};

/*============================[L O C A L  V A R I A B L E]======================================*/
/************************************************************************************************/
AC_FrameInfoType acFrame;
DA_FrameInfoType daFrame;

const uint8 sAcPanelMapEventNum = sizeof(AcPanelMapEvent)/sizeof(AcPanelMapStruct);
#if (STD_ON == AC_PANEL_MAP_CYCLE)
const uint8 sAcPanelMapCycleNum = sizeof(AcPanelMapCycle)/sizeof(AcPanelMapStruct);
#endif
const uint8 sDaKeyMapNum = sizeof(DaKeyMap)/sizeof(DaKeyMapStruct);

/*============================[L O C A L  F U N C T I O N S  D E C L A R A T I O N] ============*/
/************************************************************************************************/

/*============================[A P P L I C A T I O N  S O F T W A R E] =========================*/
/************************************************************************************************/

/*============================[D A T A  M A N A G E M E N T] ===================================*/
/************************************************************************************************/

/*============================[L O C A L  F U N C T I O N S  D E F I N I T I O N] ==============*/
/************************************************************************************************/
/*
 * Function Name       DA_RequestAllPanelStatus
 * Brief               Deal the cycle data from AC side .
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
 /************************************************************************************************/
status_t DA_RequestAllPanelStatus(DA_FrameInfoType * pDaInfo , AC_FrameInfoType * pAcInfo )
{
    status_t status = STATUS_ERROR;
	uint8 shift = 0 ,mask = 0,Index_pos = DA_DATA_3 ;
#if (STD_ON == AC_PANEL_MAP_CYCLE)
	if( pDaInfo->cmdRequestall_i < sAcPanelMapCycleNum )
#else
	if( pDaInfo->cmdRequestall_i < sAcPanelMapEventNum )
#endif
	{
	#if (STD_ON == AC_PANEL_MAP_CYCLE)
		if( AcPanelMapCycle[pDaInfo->cmdRequestall_i].key_mask != 0 )
	#else
		if( AcPanelMapEvent[pDaInfo->cmdRequestall_i].key_mask != 0 )
	#endif
		{
			pDaInfo->txHMICmd[CMD_KEY_TYPE] = 0xff;
			
			#if (STD_ON == AC_PANEL_MAP_CYCLE)
				pDaInfo->txHMICmd[CMD_KEY_KEY] = AcPanelMapCycle[pDaInfo->cmdRequestall_i].key_key ;
				Index_pos = AcPanelMapCycle[pDaInfo->cmdRequestall_i].key_pos;
				shift = AcPanelMapCycle[pDaInfo->cmdRequestall_i].key_shift;
				mask = AcPanelMapCycle[pDaInfo->cmdRequestall_i].key_mask;
			#else
				pDaInfo->txHMICmd[CMD_KEY_KEY] = AcPanelMapEvent[pDaInfo->cmdRequestall_i].key_key ;
				Index_pos = AcPanelMapEvent[pDaInfo->cmdRequestall_i].key_pos;
				shift = AcPanelMapEvent[pDaInfo->cmdRequestall_i].key_shift;
				mask = AcPanelMapEvent[pDaInfo->cmdRequestall_i].key_mask;
			#endif

			pDaInfo->txHMICmd[CMD_KEY_VLU] = (pAcInfo->curData[Index_pos]&mask)>>shift;
			
			//pAcInfo->preData[Index_pos] &= ( 0xFF - mask ) ;
			//pAcInfo->preData[Index_pos] |= ( pAcInfo->curData[Index_pos] & mask );
			ipc_send_notification(ACCT_IPC_HW_CHANNEL,ACCT_IPC_SW_CHANNEL,ACCT_IPC_SW_FUNCTION_ID,&pDaInfo->txHMICmd[CMD_KEY_TYPE],ACCT_IPC_SW_FUNCTION_DATA_LEN);

			#if( STD_ON == ACCTL_DEBUG_TRACE )
				TracePrintf(NM_TRACE, TRACE_WARNING, "[txHMICmd-->]:%d %d %d\r\n",daFrame.txHMICmd[CMD_KEY_TYPE],daFrame.txHMICmd[CMD_KEY_KEY],daFrame.txHMICmd[CMD_KEY_VLU]);
			#endif

		}
		pDaInfo->cmdRequestall_i++;
		status = STATUS_BUSY ;
	}
	else
	{
		status = STATUS_SUCCESS ;
		pDaInfo->cmdRequestall_i = 0 ;	
	}
	return status;
}

/************************************************************************************************/
/*
 * Function Name       AC_CycleDataDeal
 * Brief               Deal the data from AC side .
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
 /************************************************************************************************/
void AC_DataDeal(DA_FrameInfoType * pDaInfo , AC_FrameInfoType * pAcInfo )
{
	uint8 i=0,shift = 0 ,mask = 0,Index_pos = AC_DATA_3 ;
	uint8 cur_key_key=0 ,cur_key_vlu = 0 ,pre_key_vlu=0 ;
	//static uint8 acFrameChanged = FALSE , acFrameChangedCount = 0 ;
#if 0
	/* Header + Cmd + DataLen + Data0 ~ Data5 + ADDC */
	/* DATA0  + DATA1+DATA2   + .....................*/
	pDaInfo->txHMICmd[CMD_KEY_TYPE] = 0xff;
	pDaInfo->txHMICmd[CMD_KEY_KEY] = pAcInfo->curData[AC_DATA_8];
	if( ( pDaInfo->txHMICmd[CMD_KEY_KEY] < sAcPanelMapEventNum ) && ( pDaInfo->txHMICmd[CMD_KEY_KEY]  == AcPanelMapEvent[pDaInfo->txHMICmd[CMD_KEY_KEY]].key_key ) )
	{
		Index_pos = AcPanelMapEvent[pDaInfo->txHMICmd[CMD_KEY_KEY]].key_pos;
		shift = AcPanelMapEvent[pDaInfo->txHMICmd[CMD_KEY_KEY]].key_shift;
		mask = AcPanelMapEvent[pDaInfo->txHMICmd[CMD_KEY_KEY]].key_mask;
	
		pre_key_vlu = (pAcInfo->preData[Index_pos]&mask)>>shift;
		pDaInfo->txHMICmd[CMD_KEY_VLU] = (pAcInfo->curData[Index_pos]&mask)>>shift;
	
		if( pre_key_vlu != pDaInfo->txHMICmd[CMD_KEY_VLU] )
		{
			pAcInfo->preData[Index_pos] &= (0xff - mask);
			pAcInfo->preData[Index_pos] |= (pDaInfo->txHMICmd[CMD_KEY_VLU]<<shift);
			ipc_send_notification(ACCT_IPC_HW_CHANNEL,ACCT_IPC_SW_CHANNEL,ACCT_IPC_SW_FUNCTION,&pDaInfo->txHMICmd[CMD_KEY_TYPE],DA_FRAME_HMICMD_LEN);
		}
	}

#endif

	for( Index_pos = AC_DATA_3 ; Index_pos < AC_ADDC ;Index_pos++ )//find out the change data
	{
		if(( pAcInfo->preData[Index_pos] ^ pAcInfo->curData[Index_pos] ) !=0)
			break;
	}

	if( Index_pos < AC_ADDC )//if find out Index_pos success
	{
		//acFrameChanged = TRUE;
		
		for( i = 0 ; i < 8; i++ )
		{
			cur_key_key = AcPanelMapAll[Index_pos-AC_DATA_3][i];
			if( cur_key_key != AC_KEY_NONE )
			{
				Index_pos = AcPanelMapEvent[cur_key_key].key_pos;
				shift = AcPanelMapEvent[cur_key_key].key_shift;
				mask = AcPanelMapEvent[cur_key_key].key_mask;
			
				pre_key_vlu = (pAcInfo->preData[Index_pos]&mask)>>shift;
				cur_key_vlu = (pAcInfo->curData[Index_pos]&mask)>>shift;

				if( pre_key_vlu != cur_key_vlu )
				{
					pDaInfo->txHMICmd[CMD_KEY_TYPE] = 0xff;
					pDaInfo->txHMICmd[CMD_KEY_KEY] = cur_key_key;
					pDaInfo->txHMICmd[CMD_KEY_VLU] = cur_key_vlu;
					
					pAcInfo->preData[Index_pos] &= (0xff - mask);
					pAcInfo->preData[Index_pos] |= (pAcInfo->curData[Index_pos]&mask);
					ipc_send_notification(ACCT_IPC_HW_CHANNEL,ACCT_IPC_SW_CHANNEL,ACCT_IPC_SW_FUNCTION_ID,&pDaInfo->txHMICmd[CMD_KEY_TYPE],ACCT_IPC_SW_FUNCTION_DATA_LEN);

					#if( STD_ON == ACCTL_DEBUG_TRACE )
						TracePrintf(NM_TRACE, TRACE_WARNING, "[txHMICmd-->]:%d %d %d\r\n",daFrame.txHMICmd[CMD_KEY_TYPE],daFrame.txHMICmd[CMD_KEY_KEY],daFrame.txHMICmd[CMD_KEY_VLU]);
					#endif

					//acFrameChangedCount++;
					
					break;
				}
			}
		}
		if( i == 8 )
			pAcInfo->preData[Index_pos] = pAcInfo->curData[Index_pos];
	}
	else
	{
		//last send the panel key ,triger hmi pop-up windows
		//because ac panel send Exception frame on SYNC,so must lager than 1
		//when ac panel deal this problem,this condition can remove.
		//if(( TRUE ==acFrameChanged ) /*&& ( acFrameChangedCount > 1 )*/)
		{
			cur_key_key = pAcInfo->curData[AC_DATA_8];
			if(( AC_KEY_NONE != cur_key_key ) && ( cur_key_key <= AC_KEY_SYNC ))
			{
				Index_pos = AcPanelMapEvent[cur_key_key].key_pos;
				shift = AcPanelMapEvent[cur_key_key].key_shift;
				mask = AcPanelMapEvent[cur_key_key].key_mask;
				pre_key_vlu = (pAcInfo->preData[Index_pos]&mask)>>shift;
				cur_key_vlu = (pAcInfo->curData[Index_pos]&mask)>>shift;
				
				pDaInfo->txHMICmd[CMD_KEY_TYPE] = 0xfe;
				pDaInfo->txHMICmd[CMD_KEY_KEY] = cur_key_key;
				pDaInfo->txHMICmd[CMD_KEY_VLU] = cur_key_vlu;

				ipc_send_notification(ACCT_IPC_HW_CHANNEL,ACCT_IPC_SW_CHANNEL,ACCT_IPC_SW_FUNCTION_ID,&pDaInfo->txHMICmd[CMD_KEY_TYPE],ACCT_IPC_SW_FUNCTION_DATA_LEN);
				#if( STD_ON == ACCTL_DEBUG_TRACE )
					TracePrintf(NM_TRACE, TRACE_WARNING, "[txHMICmd-->pop]:%d %d %d\r\n",daFrame.txHMICmd[CMD_KEY_TYPE],daFrame.txHMICmd[CMD_KEY_KEY],daFrame.txHMICmd[CMD_KEY_VLU]);
				#endif
			}
		}

		//acFrameChanged = FALSE;
		//acFrameChangedCount = 0 ;
		
		//deal done
		pAcInfo->dealDone = TRUE;
	}
}

/************************************************************************************************/
/*
 * Function Name       AcCtl_AcDataDeal
 * Brief               Deal the data from AC side.
 * ServiceId           0x00
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
 /************************************************************************************************/
 void DA_DataDeal(DA_FrameInfoType * pDaInfo , AC_FrameInfoType * pAcInfo )
{
	uint8 i = 0,shift = 0 ,mask = 0,Index_pos = DA_DATA_3 ;

	if( ( pDaInfo->rxHMICmd[CMD_KEY_KEY] < sDaKeyMapNum ) && ( pDaInfo->rxHMICmd[CMD_KEY_KEY] == DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_key ) )
	{
		for( i = 0 ; i < DA_ADDC ; i++ )
		{
			pAcInfo->txData[i] = 0 ;
		}

		Index_pos = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_pos ;
		shift = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_shift;
		mask = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_mask;

		if( mask != 0x00 )
		{
			pAcInfo->txData[Index_pos] &= (0xFF - mask );
			pAcInfo->txData[Index_pos] |= ( pDaInfo->rxHMICmd[CMD_KEY_VLU] << shift ) & mask ;
		}

		Index_pos = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_pos2 ;
		shift = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_shift2;
		mask = DaKeyMap[pDaInfo->rxHMICmd[CMD_KEY_KEY]].key_mask2;
		
		if( mask != 0x00 )
		{
			pAcInfo->txData[Index_pos] &= (0xFF - mask );
			pAcInfo->txData[Index_pos] |= ( pDaInfo->rxHMICmd[CMD_KEY_VLU] << shift ) & mask ;
		}

		/* Header + Cmd + DataLen + Data0 ~ Data5 + ADDC */
		/* DATA0  + DATA1+DATA2   + .....................*/
		pAcInfo->txData[0] = DA2AC_FRAME_HEAD ;
		pAcInfo->txData[1] = DA2AC_FRAME_CMD ;
		pAcInfo->txData[2] = DA_ADDC - DA_DATA_LEN - 1 ;
		pAcInfo->txData[DA_ADDC] = 0;
		for( i = DA_ID ; i < DA_ADDC ; i++ )
		{
			pAcInfo->txData[DA_ADDC] += pAcInfo->txData[i] ;
		}
		pAcInfo->txData[DA_ADDC] = (~pAcInfo->txData[DA_ADDC])&0xff;

		LPUART_DRV_SendDataPolling(PANEL_HW_CHANNEL,&pAcInfo->txData[0],DA2AC_FRAME_LEN);
	}
}
/************************************************************************************************/
/*
 * Function Name       NdsCanSm_MainFunction
 * Brief               Returns the state and the mode of the network management.
 * ServiceId           0x0b
 * Sync/Async          Synchronous
 * Reentrancy          Reentrant
 * Param-Name[in]      None
 * Param-Name[out]     None
 * Param-Name[in/out]  None
 * Return              None
 * PreCondition        None
 * CallByAPI           Task_NM
 */
 /************************************************************************************************/
 void AcCtl_MainFunction()
  {
 	uint8 i = 0 ;
 	uint16 acADDC=0;

 	//1000 ms cycle send heart package
 	//delay 3000ms send display CMD to HMI if disconnect with ac panel
 	static uint16 cycleSendToPanelTimer = 0 ,disConnctWithPanelTimer = 0 ,txEnable = TRUE;

 	//ac new data in
 	if( TRUE == acFrame.newData )
 	{
 		acFrame.newData = FALSE;

 		for( i = AC_ID ; i < AC_ADDC ; i++ )
 		{
 			acADDC += acFrame.rxData[i] ;
 		}
 		acADDC = (~acADDC)&0XFF;

 		if( acADDC ==  acFrame.rxData[AC_ADDC] )//ac addc pass
 		{
 			memcpy(&acFrame.curData[0],&acFrame.rxData[0],AC2DA_FRAME_LEN);
 			acFrame.dealDone = FALSE;

 			disConnctWithPanelTimer = 0 ;
 			txEnable = TRUE;
 		}
 	}

 	//ac new data data deal
 	if( FALSE == acFrame.dealDone )
 	{
 		AC_DataDeal(&daFrame,&acFrame);
 	}

 	//da new data data deal
 	if( TRUE == daFrame.newCmd )
 	{
 		daFrame.newCmd = FALSE;

 		if( daFrame.rxHMICmd[CMD_KEY_KEY] == ACCT_IPC_SW_CMDHMI_REQUEST_ALL )//da request ac all status
 		{
 			daFrame.cmdRequestall = TRUE;
 			daFrame.cmdRequestall_i = 0 ;
 		}
 		else
 		{
 			DA_DataDeal(&daFrame,&acFrame);
 		}

 		#if( STD_ON == ACCTL_DEBUG_TRACE )
 			TracePrintf(NM_TRACE, TRACE_WARNING, "[-->rxHMICmd]:%d %d %d\r\n",daFrame.rxHMICmd[CMD_KEY_TYPE],daFrame.rxHMICmd[CMD_KEY_KEY],daFrame.rxHMICmd[CMD_KEY_VLU]);
 		#endif
 	}

 	//da new data is request ac all status
 	if( TRUE == daFrame.cmdRequestall )
 	{
 		if( STATUS_SUCCESS == DA_RequestAllPanelStatus(&daFrame,&acFrame) )
 			daFrame.cmdRequestall = FALSE;
 	}


 	if( TRUE == txEnable )
 	{
 		//heart package with ac panel 1000ms
 		if( ( ++cycleSendToPanelTimer)%99 == 0 )
 		{
 			cycleSendToPanelTimer = 0 ;

 			for( i = 0 ; i < DA_ADDC ; i++ )
 			{
 				acFrame.txData[i] = 0 ;
 			}

 			/* Header + Cmd + DataLen + Data0 ~ Data5 + ADDC */
 			/* DATA0  + DATA1+DATA2   + .....................*/
 			acFrame.txData[0] = DA2AC_FRAME_HEAD ;
 			acFrame.txData[1] = DA2AC_FRAME_CMD ;
 			acFrame.txData[2] = DA_ADDC - DA_DATA_LEN - 1 ;
 			acFrame.txData[DA_ADDC] = 0;
 			for( i = DA_ID ; i < DA_ADDC ; i++ )
 			{
 				acFrame.txData[DA_ADDC] += acFrame.txData[i] ;
 			}
 			acFrame.txData[DA_ADDC] = (~acFrame.txData[DA_ADDC])&0xff;

 			LPUART_DRV_SendDataPolling(PANEL_HW_CHANNEL,&acFrame.txData[0],DA2AC_FRAME_LEN);
 		}

 		//if disconnect with ac panel ,notification to DA 3000ms
 		if( ( ++disConnctWithPanelTimer)%299 == 0 )
 		{
 			disConnctWithPanelTimer = 0 ;
 			txEnable = FALSE;

 			for( i = AC_HEADER ; i < AC_ADDC ; i++ )
 			{
 				acFrame.preData[i] = 0 ;
 				acFrame.curData[i] = 0 ;
 			}

 			daFrame.txHMICmd[CMD_KEY_TYPE] = 0XFF;
 			daFrame.txHMICmd[CMD_KEY_KEY] = AC_KEY_DISPLAY;
 			daFrame.txHMICmd[CMD_KEY_VLU] = 0;//ac panel lost communication
 			ipc_send_notification(ACCT_IPC_HW_CHANNEL,ACCT_IPC_SW_CHANNEL,ACCT_IPC_SW_FUNCTION_ID,&daFrame.txHMICmd[CMD_KEY_TYPE],ACCT_IPC_SW_FUNCTION_DATA_LEN);

 			#if( STD_ON == ACCTL_DEBUG_TRACE )
 				TracePrintf(NM_TRACE, TRACE_WARNING, "[iocMTRpanel]: mcu disconnect with ac panel !!!\r\n");
 			#endif
 		}
 	}

  }

#if (STD_ON == IOC_MONITOR_WITH_SOC_ENABLE)

IocMonitorSoc_InfoType IocMonitorSocFrame=
{
	.socPowerOn				= FALSE,
	.newCmd    				= FALSE,
	.rxHMICmd    			= {"MTR"},
	.txHMICmd 				= {"MTR"},
	.taskCycle   			= 10,
	.monitorCycle   		= 500,
	.monitorCycleTimeOut	= 2000,
};

void IocMonitorWithSoc(void)
 {
	static uint16 monitorCounter = 50,monitorTimeOutCounter=200,iocWaitSocPowerOnCounter=12000;

	if( IocMonitorSocFrame.newCmd == TRUE )
	{
		IocMonitorSocFrame.newCmd = FALSE;
		if( ( IocMonitorSocFrame.rxHMICmd[0] == 'M' ) && ( IocMonitorSocFrame.rxHMICmd[1] == 'T' ) && ( IocMonitorSocFrame.rxHMICmd[2] == 'R' ) )
		{
			//CLR
			IocMonitorSocFrame.rxHMICmd[0] = 0;
			IocMonitorSocFrame.rxHMICmd[1] = 0;
			IocMonitorSocFrame.rxHMICmd[2] = 0;
			//
			monitorCounter = IocMonitorSocFrame.monitorCycle / IocMonitorSocFrame.taskCycle ;
			monitorTimeOutCounter = IocMonitorSocFrame.monitorCycleTimeOut/IocMonitorSocFrame.taskCycle;
			iocWaitSocPowerOnCounter = 120000 / IocMonitorSocFrame.taskCycle;
			//
			IocMonitorSocFrame.socPowerOn = TRUE;
		}
	}

	if( IocMonitorSocFrame.socPowerOn == TRUE )
	{
		//cycle send
		if( monitorCounter > 0 )
			monitorCounter--;
		else
		{
			monitorCounter = IocMonitorSocFrame.monitorCycle/IocMonitorSocFrame.taskCycle;
			ipc_send_notification(IOC_MONITOR_SOC_IPC_HW_CHANNEL,IOC_MONITOR_SOC_IPC_SW_CHANNEL,IOC_MONITOR_SOC_IPC_SW_FUNCTION_ID,&IocMonitorSocFrame.txHMICmd[0],IOC_MONITOR_SOC_DATA_LEN);
		}

		//soc heart package monitor
		if( monitorTimeOutCounter > 0 )
			monitorTimeOutCounter--;
		else
		{
			IocMonitorSocFrame.socPowerOn = FALSE;
			TracePrintf(NM_TRACE, TRACE_WARNING, "[iocMTRsoc]:soc's heart package timeout(2s) in IPC access!!!\r\n");
		}
	}
	else
	{
		if( iocWaitSocPowerOnCounter > 0 )
			iocWaitSocPowerOnCounter--;
		else
		{
			iocWaitSocPowerOnCounter = (120000) / IocMonitorSocFrame.taskCycle ;//120s
			TracePrintf(NM_TRACE, TRACE_WARNING, "[iocMTRsoc]:soc's heart package lost(120s) in IPC access!!!\r\n");
		}
	}
 }

 #endif
 
 #endif
