
#ifndef _HISTORYAFC_H_
#define _HISTORYAFC_H_

#define HIS_AFC_MAX_LENTH (30U)

#define HIS_AFC_MAX_DATA (200U) //532 2019-6-10 11:24:26

#define HIS_AFC_INIT_DATA (0U) //532 2019-8-19 18:26:48



#define MAX_VOL		100000000	//(ml)
#define MAX_DIS		100000000ul	//(m)

typedef struct
{
	uint8_t HistoryAvgFuelEconomyFlag;
	uint8_t BestAvgFuelFlag	:1;
	uint8_t EcoRankFlag		:1;
	uint8_t EcoRank			:6;
	uint16_t total_afc_value;
	uint16_t best_afc_value;
	uint16_t LastAF[HIS_AFC_MAX_LENTH];
}IPC_APP_HISTORY_AFC_STRU;

typedef struct
{
	IPC_APP_HISTORY_AFC_STRU ipc;
	uint32_t mileage_value[HIS_AFC_MAX_LENTH];
	uint32_t fuelcons_value[HIS_AFC_MAX_LENTH];
	uint8_t  lenth;
	uint8_t  value_cnt;
	uint8_t  value_30s_flg;
	uint8_t  updateflg;
	uint16_t CurrentAF;// CAN tx 2019-12-20 19:26:13
		
}HIS_AFC_STR;




void HisAfcDataInit(void);
void HisAfcInit(void);
void ReSetHisAfc(void);
void HisAfcTotalSingleCalc(void);
void HisAfcMain(void);
uint8_t GetHisAfcUpdateFlg(void);
void SetHisAfcUpdateFlg(uint8_t flg);
void UpdateBestAfc(void);
uint16 GetHisBestAfcData(void);
uint16 GetHisLastAfcData(void);

uint16 GetCurrentAFData(void);
void SetCurrentAFData(uint16 CurrentAFData);








uint16_t HAFC_Distance(void);
IPC_APP_HISTORY_AFC_STRU GetHisAfcData(void);


void HisAfcDataTest(void);
//void HisAfcDataTest1(IPC_APP_HISTORY_AFC_STRU HisAvgFuelData);




/*
void HAFC_Init(void);

void ReSet_HAFC(void);

void Update_BestAfc(void);

void Get_HAFC(void);

void HisAfc_VMAIN(void);
*/


#endif
