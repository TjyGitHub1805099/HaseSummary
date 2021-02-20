#ifndef _FUEL_CONFIG_H_
#define _FUEL_CONFIG_H_
#include "Std_Types.h"
#include "dea.h"


#ifdef __cplusplus
#define EXTERN_FUEL_API extern "C"
#else
#define EXTERN_FUEL_API extern
#endif

#define FUEL_SIG_FROM_RES			(1U)
#define FUEL_JUST_RUNNING_DOWN		(1U)

#define FUEL_IGN_ON					(IGN_ON)            /** IGN ON status */
#define FUEL_IGN_OFF				(IGN_OFF)           /** IGN OFF status */

//#define FUEL_DEBUG                                  /** fuel module debug switch */

#define FUEL_DISP_TYPE              (0U)            /** fuel display type 0-step 1-segment */


#define FUEL_AD_MAX					(3500U)		/** maxnum of valid AD value */
#define FUEL_ADAPT_PARA				(1000U)
#define FUEL_SAMPLE_RES_VALUE		(1000U)			/** sample circuit resistance value unit:0.1 ohm */
#define FUEL_FULL_VOL				(5800U)			/** fuel full volume(F_Point) unit:0.01L */
#define FUEL_E_VOL                  (470U)          /** fuel empty volume(E_Point) unit:0.01L */
#define FUEL_FILL_UP_THRESHOLD		(400U)			/** Refueling threshold unit:0.01L */ //for 591,IGN OFF 4L
#define FUEL_FILL_UP_THRESHOLD_ON	(900U)			/** Refueling threshold unit:0.01L */ //for 591,IGN ON 9L
#define FUEL_FILL_UP_IND_THRESHOLD	(40U)			/** Second time Refueling threshold(Refueling threshold of indicate position) 40% of fuel full*/ //591 project error of percent 40%
#define FUEL_SENSOR_ERR_JUDGE_TIME	(60*1000UL)		/** fuel sensor fault time unit:ms */
#define FUEL_SENSOR_OK_JUDGE_TIME	(60*1000UL)		/** fuel sensor recover time unit:ms */
#define FUEL_SAMPLE_CYCLE			(50U)			/** fuel sensor sample cycle unit:ms */
#define FUEL_SENSOR_SHORT			(50U)			/** fuel sensor short unit:0.1 ohm */
#define FUEL_SENSOR_OPEN			(3500U)			/** Fuel sensor open unit:0.1 ohm */
#define FUEL_CAR_RUN_DAMP_PARA		(2.3)			/** fuel damp para */
#define FUEL_IDLE_DAMP_PARA			(2.3)			/** idle speed damp para */
#define FUEL_MID_SAMPLE_CYCLE		(10)			/** fuel middle speed sample data cycle 10*0.5=5S */
/**SKONs_106 low fuel require**/
#define FUEL_LOW_VOL                (1100U)          /** low fuel threshold unit:0.01L */ //11.0L
#define FUEL_LOW_RELIEVE_VOL        (1425U)          /** low fuel relive threshold unit:0.01L */ //14.25L
/***/
#define FUEL_IGN_ON_DROP_CYCLE		(1U)			/** FUEL_SAMPLE_CYCLE */
#define FUEL_CAR_RUN_THRESHOLD		(100U)			/** unit:0.01km/h */


//fuel sensor resitance translate to fuel volume table
EXTERN_FUEL_API const uint16_t FuelResToVolTable[][2];

//fuel volume thranslate to indicate position table
EXTERN_FUEL_API const uint16_t FuelVolToIndPos[][2];

/** get table size */
EXTERN_FUEL_API uint16_t GetFuelResToVolTableSize(void);
/** get table size */
EXTERN_FUEL_API uint16_t GetFuelVolToIndPosSize(void);
/** get fuel indicate position max step */
EXTERN_FUEL_API uint16_t GetFuelIndPosMax(void);
/** get fuel segment display tabale up addr */
EXTERN_FUEL_API const uint16_t *GetFuelDisTableUp(void);
/** get fuel segment display tabale size */
EXTERN_FUEL_API uint16_t GetFuelDisTableSize(void);
/** get fuel segment display tabale down addr*/
EXTERN_FUEL_API const uint16_t *GetFuelDisTableDown(void);
/** get fuel display 0-step 1-segment */
EXTERN_FUEL_API uint16_t GetDispType(void);

#endif

