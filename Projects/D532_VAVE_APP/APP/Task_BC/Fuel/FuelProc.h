#ifndef _FUEL_PROC_H_
#define _FUEL_PROC_H_
/***********************[I N C L U D E S]***********************/
#include "Std_Types.h"
#include "FuelConfig.h"
#include "alga.h"
//#include "BackRam.h"

#ifdef __cplusplus
#define EXTERN_FUEL_API extern "C"
#else
#define EXTERN_FUEL_API extern
#endif

//#define FUEL_INLINE_SUPPORT

#define FUEL_TRACE_DEBUG
#define FILTER_EXT
#define IPC_FUEL_STAT_REPORT

#define FUEL_IND_MODE_FAST  (0U)   /** Fast Indicate Response Mode */
#define FUEL_IND_MODE_MID   (1U)   /** Middle Indicate Response Mode */
#define FUEL_IND_MODE_SLOW  (2U)   /** Slow Indicate Response Mode */

typedef enum
{
	FUEL_PROC_IGN_OFF_TO_ON,	/** IGN OFF to IGN ON */
	FUEL_PROC_ING_ON,			/** IGN ON And vehicle speed == 0km/h */
	FUEL_PROC_CAR_RUN,			/** vehicle speed > 0km/h */
	FUEL_PROC_CAR_RUN_TO_STOP,	/** vehicle change to 0km/h */
	FUEL_PROC_IGN_ON_TO_OFF,	/** IGN ON to IGN OFF */
	FUEL_PROC_PRESLEEP,			/** presleep status */
	FUEL_PROC_SLEEP,			/** sleep status */
}FUEL_PROC_STAT_ENUM;	/** Car operation status */

typedef enum
{
	FuelSigNormal,				/** Fuel Sensor Normal */
	FuelSigShort,			    /** Fuel Sensor Short */
	FuelSigOpen,				/** Fuel Sensor Open */
	FuelSigUnknow,				/** Fuel Sensor Unknow */
}FUEL_SIG_ENUM;

typedef struct
{
	uint16_t SensorOpen			    :1;		/** Fuel Sensor Open Fault */
	uint16_t SensorShort			:1;		/** Fuel Sensor Short Fault */
	uint16_t FuelFillUpFlag		    :1;		/** Fuel Fill Up Flag */
	uint16_t IgnOnFuelFillUpFlag	:1;		/** IGN ON Fuel Fill Up Flag */
	uint16_t LowFuel				:1;		/** 0-Normal 1-Low */
	uint16_t SensorStat			    :1;		/** 0-Normal 1-Fault */
	uint16_t SleepEnable			:1;		/** Sleep Enable Flag 0-not allowed 1-allowed */
	uint16_t FuelVolSaved		    :1;		/** Fuel Memory Finish */
	uint16_t FuelIndSaved		    :1;		/** Fuel Indicate Position Memory Finsih */
    uint16_t BatOnOrSigRecoverlag   :1;     /** BAT ON or Fuel Sensor Signal Recover*/
    uint16_t FuelIndMode            :2;     /** Indicate Mode 00-Fast Indicate Response Mode 
                                               01--Middle Indicate Response Mode 02-Slow Indicate Response Mode */
	uint16_t FuelVolumeValid		:1;		/** 0-Invalid 1-Valid */
	uint16_t FirstBatOnFlag			:1;
	uint16_t FuelIgnOffVolSaved		:1;		/** Fuel Memory Finish at IGN OFF */
}FUEL_STAT_STRU;

#ifndef  FILTER_EXT
typedef struct
{
	uint16_t MaxData;			/** The maximum of sampled values */
	uint16_t MinData;			/** The minimum of sampled values */
	uint16_t AvgData;			/** The average of sampled values */
	uint16_t Cnt;				/** sample count */
	uint32_t AddData;			/** sample sum */
}SAMPLE_DATA_STRU;	/** sample data struct */
#endif

typedef struct
{
	SAMPLE_DATA_STRU SampleData;	/** sample data */
	uint16_t ResValue;				/** sample resistance value */
	uint16_t FuelVol;				/** sample fuel volume */
	uint16_t IndicatePos;			/** fuel indicate position  */
	uint8_t FinishFlag;				/** sample finish flag(meet the requirements of the sampling points,and got the average of samples) */
}FUEL_SAMPLE_DATA_STRU;	/** fuel vol sample data struct */

typedef struct
{
	FUEL_PROC_STAT_ENUM CurCarStat;			/** Car status now */
	FUEL_PROC_STAT_ENUM LastCarStat;		/** Car status before */
	FUEL_STAT_STRU Stat;					/** bit status flag */
	uint16_t ResValue;						/** Current fuel sensor resistance value unit:0.1 ohm*/
	uint16_t ResValueNow;					/** Current fuel sensor resistance value unit:0.1 ohm*/
	float32_t ResValueDamp;					/** Damp fuel sensor resitance value unit:0.1 ohm*/
	uint16_t FuelVolNow;					/** Current sampled fuel volume unit:0.01L*/
	uint16_t FuelVolDamp;					/** Damp fuel volume(Filtered fuel volume) Unit:0.01L */
	uint16_t FuelVolSave;					/** memorized fuel volume used for judge fillup fuel unit:0.01L*/
	//uint16_t FuelVolSaveIgnOff;				/** when ign off, memorized fuel volume used for judge fillup fuel unit:0.01L*/
	uint16_t IndicatePosTarget;			    /** fuel indicate target position */
    uint16_t IndicatePosCur;                /** fuel indicate current position */
	uint16_t IndicatePosSave;				/** memorized fuel indicate position */
	uint16_t FuelVolPercent;				/** fuel volume percent unit:0.1% */
	uint16_t FuelVolSaveTimeCnt;			/** the time count for memorize fuel volume */
    uint16_t IndicateSegTarget;             /** fuel volume segment display target value */
    uint16_t IndicateSegCur;                /** fuel volume segment display current value */
	uint16_t FuelFillUpCnt;					/** fuel fill up occur times count */
	FUEL_SAMPLE_DATA_STRU SampleData0p5;	/** 0.5S sample data */
	FUEL_SAMPLE_DATA_STRU SampleDataMid;	/** Mid(5S) sample data */
	//FUEL_SAMPLE_DATA_STRU SampleData5Min;	/** 5Min sample data */
}FUEL_PROC_STRU;	/** fuel module data struct */

typedef void (*FuelFillUpFuncType)(void);

//#pragma segment BACK_RAM_START
extern FUEL_PROC_STRU gFuelData;	/** fuel module variable(need save to protected ram(data-retention ram),data should no change after exception reset ) */
//#pragma segment BACK_RAM_END


/*********************[P R O T O T Y P E S]*********************/
EXTERN_FUEL_API void FUEL_vInit(void);		/** fuel module init func*/
EXTERN_FUEL_API void FUEL_FuelProc(void);                         /** fuel module process func ,call cycle:10ms */
EXTERN_FUEL_API uint16_t GetFuelPowerAdValue(void);	/** get sampled ad value of fuel sample circuit power voltage */
EXTERN_FUEL_API uint16_t GetFuelSensorAdValue(void);	/** get sampled ad value of fuel sensor access point voltage */
EXTERN_FUEL_API uint8_t GetIgnStatus(void);				    /** get IGN status 0-IGN_OFF  1-IGN_ON */
EXTERN_FUEL_API uint16_t GetVehicleSpeed(void);			/** get vehicle speed unit:0.1Km/h */
EXTERN_FUEL_API uint16_t GetRpmValue(void);				/** get RPM value unit: rpm/min */
//EXTERN_FUEL_API uint8_t GetSysRstType(void);              /** get system reset type 0-power on reset(BAT ON) 1-exception reset */

EXTERN_FUEL_API uint16_t FUEL_GetFuelResValue(void);

EXTERN_FUEL_API uint8_t FUEL_GetFillUpStatus(void);
EXTERN_FUEL_API FUEL_SIG_ENUM FUEL_GetFuelSigStatus(void);	    /** get fuel sensor status */
EXTERN_FUEL_API uint8_t FUEL_GetFuelLowStatus(void);          	/** get low fuel status 0-fuel volume normal 1-fuel volue low */
EXTERN_FUEL_API uint16_t FUEL_GetFuelVolNow(void);				/** get now fuel volume */
EXTERN_FUEL_API uint16_t FUEL_GetFuelVol(void);	                /** get current fuel volume */
EXTERN_FUEL_API uint16_t FUEL_GetIndicatePos(void);             /** get fuel indicate target position */
EXTERN_FUEL_API uint16_t FUEL_GetFuelSensorResNow(void);		/** get fuel indicate target position */
EXTERN_FUEL_API uint16_t FUEL_GetFuelPercent(void);             /** get fuel percent */
EXTERN_FUEL_API uint8_t FUEL_GetFuelIndMode(void);             	/** get fuel indicate response mode */
EXTERN_FUEL_API uint8_t FUEL_GetFuelSleepEnableStat(void);  /** get sleep enable flag 0-not allowed sleep 1-enable sleep */
EXTERN_FUEL_API uint8_t FUEL_GetFuelVolumeValid(void);			/** get fuel volume valid status */
EXTERN_FUEL_API uint16_t FUEL_GetIndicateSeg(void); 			/** get fuel display segment */
EXTERN_FUEL_API void FUEL_RegisterFillUpFunc(FuelFillUpFuncType pfuc); /** RegisterFillUpFunc */



#ifdef FUEL_DEBUG
EXTERN_FUEL_API void SetInitFuelProcFlag(uint8_t flag);
EXTERN_FUEL_API uint16_t GetFuelResValue(void);
EXTERN_FUEL_API FUEL_PROC_STRU GetFuelData(void);			/** get Fuel module data */
#endif

#endif
