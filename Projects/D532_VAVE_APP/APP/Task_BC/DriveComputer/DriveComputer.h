

#ifndef  _DRIVECOMPUTER_H_
#define  _DRIVECOMPUTER_H_

#include "Std_Types.h"


#define DCP_TRUE	1	

#define DCP_FALSE	0	

typedef enum
{	
	DCP_Normal = 0,		/**< Normal State */
    DCP_Exception,	    /**< Exception State */		
}DCP_Mode;











typedef struct
{
	unsigned int   u32ReMilgeValue;      	
	unsigned char  u8ReMilgeValid; 		    
	unsigned char  u8ReMilgeAlive;			
	unsigned char  u8ReMilgeTimeoutFlag;	
	
}ReMilge_InputInfor;








typedef struct
{
	  signed short   s16IpackValue;      	
	unsigned char  	 u8IpackValid; 		    
	unsigned char    u8IpackAlive;			
	unsigned char    u8IpackTimeoutFlag;	
	
}Ipack_InputInfor;



#define		Vmax_value	800



typedef struct
{
	unsigned short   u16VpackValue;      	
	unsigned char  	 u8VpackValid; 		    
	unsigned char    u8VpackAlive;			
	unsigned char    u8VpackTimeoutFlag;	
	
}Vpack_InputInfor;


void DriverCpt_Init();

Vpack_InputInfor UpdateVpackInfor();
Ipack_InputInfor UpdateIpackInfor();
ReMilge_InputInfor UpdateReMilgeInfor();


void Vpack_MAIN();

void Ipack_MAIN();

void ReMilge_MAIN();







#endif 

