//*****************************************************************************
// file			: afc_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09         eason             Modify for HC Cluster
//****************************************************************************
#ifndef AFCCFG_H
#define AFCCFG_H
#include "tii_cfg.h" 
//*****************************************************************************
//
//! \addtogroup afc_cfg
//! @{
//
//*****************************************************************************
#include "afc.h"
#include "Std_Types.h"

/** define ifc bool 1 */
#define AFC_True 1

/** define ifc bool 0 */
#define AFC_False 0

/** define maximum value */
#define AFC_MAXDIS 4200000000

/** define maximum value */
#define AFC_MAXFUEL 4200000000


#define AFC_MAX_DIS_AFC 100000000UL



/** define AFC maximum can output value 0.1l/km */
#define AFC_MAXAFC 999	/**591 project range**/

/** define enable display distance, 500m */
#define AFC_EnDISPLAY 500	/**591 project range**/



/** Description: adaptation for DPOOL type of Speedo module */
//#define AFC__tstSpeedoDpoolData 		DPOOL_txVEL_tstDpoolData

/** Description: adaptation for DPOOL type of Fuel Injection module */
//#define AFC__tstFuelInjDpoolData    	DPOOL_txTII_tstDpoolData

/** AFC mileage information */
typedef struct  
{
	unsigned short u16Mileage;		/**< mileage signal value */
	unsigned char u8MileageAlive;	/**< mileage signal alive */
	unsigned char u8MileageValid;	/**< mileage signal valid */
	
}AFC_tstMileage;

typedef struct  
{
	uint32 u32ConsData;
	uint32 u32DisData;
	uint16 u16ShowData;	
	uint16 u16Timecnt;
	uint8  u8boValid;
	
}AFC_STR;



//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
AFC_tstMileage AFC_stGetNextMileageInfor(void);
unsigned char AFC_u8GetMileageTimeOut(void);
TII_Data_STRU AFC_u16GetFuelInc(void);
void AFC_vWriteData(AFC_Data_STRU stAfcData);
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif //  AFCCFG_H


