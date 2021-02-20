#ifndef _DTE_CFG_H_
#define _DTE_CFG_H_

#include "FuelConfig.h"

#define DTE_Cfg_MaxCanValue		(4094)			/*! km */
#define DTE_Cfg_MaxDisValue		(9999)			/*! km *//**591 project remain mileage range 0~9999km**/
#define DTE_Cfg_MinDisValue		(50)			/*! km */
//#define DTE_Cfg_DefaultCFC		(848)		    /*! 8,48L */	//initial fuel consume
#define DTE_Cfg_DefaultCFC_A		(653)		    /*! 6.53L */	//initial fuel consume yangxl 532A
#define DTE_Cfg_DefaultCFC_B		(610)		    /*! 6.53L */	//initial fuel consume yangxl 532B
#define DTE_Cfg_DefaultMillage	(100*1000)		/*! 100km */
#define DTE_Cfg_MaxChageStep	(1)				/*! km */
#define DTE_Cfg_MaxHalfMillage	(200*1000)		/*! km */
#define DTE_Cfg_MaxHalfFuelCons	(30*1000*1000)	/*! ul */
#define DTE_Cfg_FuelEmptyOffset	(2)				/*! l */
#define DTE_Cfg_TimeInterval	(300)			/*! 300*100ms = 30S */// 591 project require



//591 project require parameters

#define DTE_FT_VOL	(FUEL_FULL_VOL*10000)	//uint:ul
#define DTE_ET_VOL	(FUEL_E_VOL*10000)		//uint:ul

//#define DTE_DR		4600000		//uint: ul
//#define DTE_SM		500000		//uint: ul

#define DTE_DR		5000000		//uint: ul yangxl 532A
#define DTE_SM		500000		//uint: ul yangxl 532A


#define STEPUNIT	3 //Dte显示步进长度

#endif
