#include "FuelProc.h"

//fuel sensor resitance translate to fuel volume table unit:0.1omh to 0.01 L
#if 0 //591
const uint16_t FuelResToVolTable[][2] = {
{  510, 6500 },  /* F Stop */
{  912, 6220 },  /* F Point */
{ 1172, 5500 },  /* 7/8 Point */
{ 1402, 4780 },  /* 3/4 Point */
{ 1632, 4060 },  /* 5/8 Point */
{ 1862, 3340 },  /* 1/2 Point */
{ 2092, 2620 },  /* 3/8 Point */
{ 2322, 1900 }, /* 1/4 Point */
{ 2552, 1180 }, /* 1/8 Point */
{ 2780,  460 },  /* E Point */

};

//fuel volume thranslate to indicate position table 0.01L
const uint16_t FuelVolToIndPos[][2] = {
{   460,   0 }, /* E Point */
{  3340,  50 }, /* 1/2 Point */
{  6220, 100 }, /* F Point */
};

#else //532 <2.532_Parameters_N_0704>
const uint16_t FuelResToVolTable[][2] = {
{  510, 5800 },  /* F Stop */
{  910, 5600 },  /* F Point */
{ 1170, 4970 },  /* 7/8 Point */
{ 1400, 4400 },  /* 3/4 Point */
{ 1630, 3730 },  /* 5/8 Point */
{ 1860, 3050 },  /* 1/2 Point */
{ 2090, 2410 },  /* 3/8 Point */
{ 2320, 1750 },  /* 1/4 Point */
{ 2550, 1100 },  /* 1/8 Point */
{ 2624,  800 },  
{ 2662,  710 },
{ 2830,  470 },  /* E Point */
{ 2840,  430 },  /* E Stop */
};

//fuel volume thranslate to indicate position table 0.01L
const uint16_t FuelVolToIndPos[][2] = {
{   470,   0 }, /* E Point */
{  1100,  13 }, /* LFW Point */
{  1425,  19 }, /*	Point */
{  3050,  50 }, /* 1/2 Point */
{  5600, 100 }, /* F Point */
};

#endif




//fuel segment show buffer 
const uint16_t FuelDisTableUp[] 	= { 300, 610, 870, 1150, 1440, 1740, 2000, 2270 };
const uint16_t FuelDisTableDown[] 	= { 320, 630, 890, 1170, 1460, 1760, 2020, 2290 };



/**************************************************************************** 
Function:       GetFuelResToVolTableSize
Description:    Get FuelResToVolTable Size
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
uint16_t GetFuelResToVolTableSize(void)
{
    return (sizeof(FuelResToVolTable)/sizeof(FuelResToVolTable[0]));
}

/**************************************************************************** 
Function:       GetFuelVolToIndPosSize
Description:    Get FuelVolToIndPos Size
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
uint16_t GetFuelVolToIndPosSize(void)
{
    return (sizeof(FuelVolToIndPos)/sizeof(FuelVolToIndPos[0]));
}

/**************************************************************************** 
Function:       GetFuelIndPosMax
Description:    get fuel indicate position max step
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
uint16_t GetFuelIndPosMax(void)
{
    return FuelVolToIndPos[(sizeof(FuelVolToIndPos)/sizeof(FuelVolToIndPos[0]))-1][1];
}

/**************************************************************************** 
Function:       GetDispType
Description:    get fuel display 0-step 1-segment
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
uint16_t GetDispType(void)
{
    return FUEL_DISP_TYPE;
}

/**************************************************************************** 
Function:       GetFuelDisTableUp
Description:    get fuel segment display tabale up addr
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
const uint16_t *GetFuelDisTableUp(void)
{
    return FuelDisTableUp;
}

/**************************************************************************** 
Function:       GetFuelDisTableSize
Description:    get fuel segment display tabale size
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
uint16_t GetFuelDisTableSize(void)
{
    return sizeof(FuelDisTableUp) / sizeof(FuelDisTableUp[0]);
}

/**************************************************************************** 
Function:       GetFuelDisTableDown
Description:    get fuel segment display tabale up addr
Input:          none
Output:         none
Return:         table size
Others:         none
*****************************************************************************/
const uint16_t *GetFuelDisTableDown(void)
{
    return FuelDisTableDown;
}


