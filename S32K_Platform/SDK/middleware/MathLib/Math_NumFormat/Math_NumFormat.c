#include "Std_Types.h"

uint8 Math_Num_Bcd2Dec(uint8 bcd)
{
	return bcd - (bcd >> 4) * 6;
}

uint8 Math_Num_Dec2Bcd(uint8 dec)
{
	return (uint8) (dec + (dec / 10) * 6);
}
