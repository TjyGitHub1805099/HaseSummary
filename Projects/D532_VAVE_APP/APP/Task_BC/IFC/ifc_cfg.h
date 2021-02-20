//*****************************************************************************
// file			: ifc_cfg.h
// Description	: configure parameters from eed or constant  
//
// Copyright (c) 2006-2037 HSAE co. Ltd. All rights reserved
// 
// Change Logs:
// Date				Author         Note	
// 2017/05/08 	eason	      First draft version
// 2017/06/09       eason          Modify for HC Cluster
//****************************************************************************

#ifndef IFCCFG_H
#define IFCCFG_H

#include "dea.h"
#include "tii_cfg.h" 

typedef enum
{
	IFC_UNIT_L_100KM = 0,	/*! l/100km */
	IFC_UNIT_L_H,		/*! l/h */
}IFC_UNIT_ENUM;

/** define ifc bool 1 */
#define IFC_True 1

/** define ifc bool 0 */
#define IFC_False 0

/** define max time count, 100ms*10=1000ms */
#define TIMEREACH 10

/** define every cycle 100ms */
#define CYCLE 100

/** define move speed */
#define MOVESPEED 40	//4km/h

/** define stop speed */
#define STOPSPEED 20	//2km/h

/** define maximum value */
#define IFC_MAXFUEL 42000000

/** define maximum value */
#define IFC_MAXDIS 4200000000

/** define maximum value, when unit is 0.1L/100km */
#define IFC_MAXL_100KM 200

/** define maximum value, when unit is 0.1L/h */
#define IFC_MAXL_H 100

#define IFC_FOR_591


/** IFC mileage information */
typedef struct  
{
	uint16_t u16Mileage;		/**< mileage signal value */
	uint8_t u8MileageAlive;	/**< mileage signal alive */
	uint8_t u8MileageValid;	/**< mileage signal valid */
	
}IFC_tstMileage;

typedef struct
{
	uint8_t IFC_boValid;
	IFC_UNIT_ENUM IFC_u8Unit;
	uint16_t IFC_u16InstCons;
}IFC_Data_STRU;



//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
TII_Data_STRU IFC_stGetTII(void);
IFC_Data_STRU IFC_stGetVel(void);
void IFC_vWrite(IFC_Data_STRU stIfcdata);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


#endif      //IFCCFG_H



