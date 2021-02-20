/********************************************************************
File name: TempConvert.c
Author: Stephen Du
Version: V1.0
Date: 11/15/2016
Description:
Others:
Function List:
1. ....
History: /
1. Date:
Author:
Modification:
2. ...
********************************************************************/
#include "Std_Types.h"
#include "BinarySearch.h"

#define TEMP_MAX_TEMP (1250)
#define TEMP_MIN_TEMP (-400)

const uint32 Temp_ResisTab[35] =
{
    1747920, 1245428, 898485, 655802, 483954,
    360850, 271697, 206463, 158214, 122259,
    95227, 74730, 59065, 47000, 37643,
    30334, 24591, 20048, 16433, 13539,
    11209, 9328, 7798, 6544, 5518,
    4674, 3972, 3388, 2902, 2494,
    2150, 1860, 1615, 1406, 0
};

const uint32 Temp_ResisDeltaTab[33] =
{
    502492, 346943, 242683, 171848, 123104,
    89153, 65234, 48249, 35955, 27032,
    20497, 15665, 12065, 9357, 7309,
    5743, 4543, 3615, 2894, 2330,
    1881, 1530, 1254, 1026, 844,
    702, 584, 486, 408, 344,
    290, 245, 209
};

sint16 Temp_Convert(uint16 AdcValue)
{
    uint8 Index = 0xFF;
    sint16 tempValue = 0;
    uint32 resisValue = 0u;

    resisValue = (AdcValue * 47000) / (1024 - AdcValue);
    Index = BinSearch_Interval(Temp_ResisTab, 35, BSEARCH_BUFTYPE_UINT32, resisValue);
    if (35 == Index)
    {
        tempValue = TEMP_MAX_TEMP;
    }
    else if (0 == Index)
    {
        tempValue = TEMP_MIN_TEMP;
    }
    else
    {
        tempValue = ((Temp_ResisTab[Index] - resisValue) * 50) / Temp_ResisDeltaTab[Index] + Index * 50 + TEMP_MIN_TEMP;
    }
    return (tempValue);
}
